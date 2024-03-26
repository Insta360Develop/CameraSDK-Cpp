#pragma once
#include <httplib.h>

#include <json.hpp>
#include <memory>
#include <string>
#include <vector>

#include "../LeticoCamera/leticoCamera.h"

using json = nlohmann::json;

class HttpServer
{
public:
	enum RequestType
	{
		GET,
		POST
	};

	HttpServer(std::vector<std::shared_ptr<LeticoCamera>> cameras);
	~HttpServer();

private:
	void createEndpoints();
	void createServer();

	std::shared_ptr<httplib::Server> mServer;
	std::vector<std::shared_ptr<LeticoCamera>> mCameras;
	std::thread mServerThread;

	std::string mServerAddress;
	int mPort;
};