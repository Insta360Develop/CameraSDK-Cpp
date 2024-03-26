#pragma once
#include "httpServer.h"

#include <cstddef>
#include <string>
#include <utility>

HttpServer::HttpServer(std::vector<std::shared_ptr<LeticoCamera>> cameras):
	mCameras(std::move(cameras)), mServerAddress("localhost"), mPort(9091)
{
	mServer = std::make_shared<httplib::Server>();
	createServer();
}
HttpServer::~HttpServer()
{
	if (mServer != nullptr)
		mServer->stop();
	if (mServerThread.joinable())
		mServerThread.join();
}

void HttpServer::createServer()
{
	mServer.get()->set_base_dir(".");
	mServer.get()->set_mount_point("/", ".");

	mServer.get()->set_pre_routing_handler(
		[](const httplib::Request&, httplib::Response& res)
		{
			res.set_header("Access-Control-Allow-Origin", "*");
			res.set_header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
			return httplib::Server::HandlerResponse::Unhandled;
		}
	);

	createEndpoints();

	mServerThread = std::thread([&] { mServer->listen(mServerAddress, mPort); });
	const int RECONNECT_RETRIES_AMOUNT = 1000;
	for (int i = 0; i < RECONNECT_RETRIES_AMOUNT; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
		if (mServer->is_running())
		{
			std::cout << "HTTP server listen on port " << mPort;
			break;
		}
	}
}

void HttpServer::createEndpoints()
{
	//======== HEALTHY ===========
	mServer.get()->Get(
		"/api/v1/healthy", [&](const httplib::Request&, httplib::Response& res) { res.set_content("Healthy", "text/plain"); }
	);
	//======== Live stream ===========

	mServer.get()->Get(
		"/stream.m3u8",
		[](const httplib::Request&, httplib::Response& res)
		{
			std::ifstream t("stream.m3u8");
			std::stringstream buffer;
			buffer << t.rdbuf();

			res.set_content(buffer.str(), "application/x-mpegURL");
		}
	);

	mServer.get()->Get(
		R"(/(.+\.ts))",
		[](const httplib::Request& req, httplib::Response& res)
		{
			std::ifstream file(req.matches[1], std::ios::binary | std::ios::ate);
			if (file.is_open())
			{
				auto size = file.tellg();
				std::string buffer(size,
								   '\0');  // Create string buffer to the size of the file
				file.seekg(0);
				file.read(&buffer[0], size);

				res.set_content(buffer, "video/MP2T");
			}
			else
			{
				res.status = 404;
			}
		}
	);
	//======== Get all cameras series ========
	mServer->Get(
		"/api/v1/serialNumbers",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			nlohmann::json jsonResponse;

			size_t cameraIndex = 0;
			auto it = req.params.find("index");
			if (it != req.params.end())
			{
				cameraIndex = std::stoi(it->second);
			}

			if (cameraIndex < 0 || cameraIndex >= mCameras.size())
			{
				jsonResponse = {{"status", "error"}, {"message", "Invalid camera index"}};
				res.status = 400;  // Bad Request
			}
			else
			{
				auto serialNumbers = mCameras[cameraIndex]->getAllSerialNumbers();

				nlohmann::json jsonSerialNumbers = nlohmann::json::array();
				for (const auto& serialNumber : serialNumbers)
				{
					jsonSerialNumbers.push_back(serialNumber);
				}

				jsonResponse = {{"status", "success"}, {"serialNumbers", jsonSerialNumbers}};
			}

			res.set_content(jsonResponse.dump(), "application/json");
		}
	);
	//========  Take photo ===========
	mServer->Post(
		"/api/v1/takePhoto",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			// Parse camera index from request
			size_t cameraIndex = std::stoi(req.get_param_value("cameraIndex"));
			nlohmann::json jsonResponse;

			if (cameraIndex < 0 || cameraIndex >= mCameras.size())
			{
				jsonResponse = {{"status", "error"}, {"message", "Invalid camera index"}};
				res.status = 400;  // Bad Request
			}
			else
			{
				auto deviceUrl = mCameras[cameraIndex]->takePhoto();
				if (deviceUrl.empty())
				{
					jsonResponse = {{"status", "error"}, {"message", "Something went wrong, unable to take photo"}};
				}
				else
				{
					auto folderUrl = mCameras[cameraIndex]->downloadFile(deviceUrl);
					jsonResponse = {
						{"status", "success"},
						{"message", "Photo taken successfully"},
						{"deviceUrl", deviceUrl},
						{"homeUrl", folderUrl},
						{"wwwUrl", folderUrl}};
				}
			}

			res.set_content(jsonResponse.dump(), "application/json");
		}
	);

	//======== getSerialNumber ===========
	mServer->Post(
		"/api/v1/getSerialNumber",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			nlohmann::json jsonResponse;

			try
			{
				size_t cameraIndex = std::stoi(req.get_param_value("cameraIndex"));
				if (cameraIndex < 0 || cameraIndex >= mCameras.size())
				{
					throw std::out_of_range("Invalid camera index");
				}
				std::string serialNumber = mCameras[cameraIndex]->getSerialNumber();
				jsonResponse = {{"status", "success"}, {"serialNumber", serialNumber}};
			}
			catch (const std::exception& e)
			{
				jsonResponse = {{"status", "error"}, {"message", e.what()}};
				res.status = 400;  // Bad Request
			}

			res.set_content(jsonResponse.dump(), "application/json");
		}
	);
	//======== Get list of files ========
	mServer->Post(
		"/api/v1/getFileLists",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			nlohmann::json jsonResponse;

			try
			{
				size_t cameraIndex = std::stoi(req.get_param_value("cameraIndex"));
				if (cameraIndex < 0 || cameraIndex >= mCameras.size())
				{
					throw std::out_of_range("Invalid camera index");
				}
				auto fileLists = mCameras[cameraIndex]->getFileList();
				jsonResponse = {{"status", "success"}, {"files", fileLists}};
			}
			catch (const std::exception& e)
			{
				jsonResponse = {{"status", "error"}, {"message", e.what()}};
				res.status = 400;  // Bad Request
			}

			res.set_content(jsonResponse.dump(), "application/json");
		}
	);
	//======== Start Recording ========
	mServer->Post(
		"/api/v1/startRecording",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			size_t cameraIndex = std::stoi(req.get_param_value("cameraIndex"));
			nlohmann::json jsonResponse;

			if (cameraIndex < 0 || cameraIndex >= mCameras.size())
			{
				jsonResponse = {{"status", "error"}, {"message", "Invalid camera index"}};
				res.status = 400;  // Bad Request
			}
			else
			{
				auto errorMessage = mCameras[cameraIndex]->startRecording();
				if (!errorMessage.empty())
				{
					jsonResponse = {{"status", "error"}, {"message", errorMessage}};
				}
				else
				{
					jsonResponse = {{"status", "success"}, {"message", "Start recording successfully"}};
				}
			}

			res.set_content(jsonResponse.dump(), "application/json");
		}
	);
	//======== Stop Recording ========
	mServer->Post(
		"/api/v1/stopRecording",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			size_t cameraIndex = std::stoi(req.get_param_value("cameraIndex"));
			nlohmann::json jsonResponse;

			if (cameraIndex < 0 || cameraIndex >= mCameras.size())
			{
				jsonResponse = {{"status", "error"}, {"message", "Invalid camera index"}};
				res.status = 400;  // Bad Request
			}
			else
			{
				auto deviceUrls = mCameras[cameraIndex]->stopRecording();
				if (deviceUrls.empty())
				{
					jsonResponse = {{"status", "error"}, {"message", "Unable to stop recording, no recordings found"}};
				}
				else
				{
					auto folderUrls = mCameras[cameraIndex]->downloadFile(deviceUrls);
					jsonResponse = {
						{"status", "success"},
						{"message", "Recording stopped and files downloaded successfully"},
						{"deviceUrls", deviceUrls},
						{"homeUrls", folderUrls},
						{"wwwUrls", folderUrls}};
				}
			}

			res.set_content(jsonResponse.dump(), "application/json");
		}
	);
}
