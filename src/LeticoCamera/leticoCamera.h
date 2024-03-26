#pragma once

#include <camera/camera.h>
#include <camera/device_discovery.h>
#include <camera/photography_settings.h>

#include "../utils.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <vector>

class LeticoCamera
{
public:
	LeticoCamera();

	~LeticoCamera();

	std::string takePhoto();
	std::string getSerialNumber();
	std::vector<std::string> getAllSerialNumbers();
	std::vector<std::string> getFileList();
	void deleteFile(const std::string& file_to_delete);
	std::string downloadFile(const std::string& file_to_download);
	std::vector<std::string> downloadFile(const std::vector<std::string> &files_to_download);

	void downloadAllFiles();

	std::string startRecording();
	std::vector<std::string> stopRecording();
	void startPreviewLiveStream();
	void stopPreviewLiveStream();
	void setExposureSettings();
	void setCaptureSettings();
	void getUUID();
	void takePhotoAndDownload();
	void getCurrentCaptureStatus();
	void startTimelapse();
	void stopTimelapse();
	void getBatteryStatus();
	void getStorageInfo();

	// More camera-related methods...

private:
	std::shared_ptr<ins_camera::Camera> mCamera;
	std::vector<std::string> mSerialNumbersVec;
	void discoverAndOpenCamera();
	std::string mSerialNumber;
	size_t mCurrentDownloadIndex;
};

class TestStreamDelegate : public ins_camera::StreamDelegate
{
public:
	TestStreamDelegate()
	{
		file1_ = fopen("./01.h264", "wb");
		file2_ = fopen("./02.h264", "wb");
	}
	~TestStreamDelegate()
	{
		fclose(file1_);
		fclose(file2_);
	}

	void OnAudioData(const uint8_t* data, size_t size, int64_t timestamp) override
	{
		std::cout << "[TestStreamDelegate] on audio data:" << std::endl;
	}
	void OnVideoData(const uint8_t* data, size_t size, int64_t timestamp, uint8_t streamType, int stream_index = 0) override
	{
		std::cout << "[TestStreamDelegate] on video frame:" << size << ";" << timestamp << ";" << stream_index << std::endl;
		if (stream_index == 0)
		{
			fwrite(data, sizeof(uint8_t), size, file1_);
		}
		if (stream_index == 1)
		{
			fwrite(data, sizeof(uint8_t), size, file2_);
		}
	}
	void OnGyroData(const std::vector<ins_camera::GyroData>& data) override
	{
		//for (auto& gyro : data) {
		//	if (gyro.timestamp - last_timestamp > 2) {
		//		fprintf(file1_, "timestamp:%lld package_size = %d  offtimestamp = %lld gyro:[%f %f %f] accel:[%f %f %f]\n", gyro.timestamp, data.size(), gyro.timestamp - last_timestamp, gyro.gx, gyro.gy, gyro.gz, gyro.ax, gyro.ay, gyro.az);
		//	}
		//	last_timestamp = gyro.timestamp;
		//}
	}
	void OnExposureData(const ins_camera::ExposureData& data) override
	{
		//fprintf(file2_, "timestamp:%lld shutter_speed_s:%f\n", data.timestamp, data.exposure_time);
	}

private:
	FILE* file1_;
	FILE* file2_;
	int64_t last_timestamp = 0;
};