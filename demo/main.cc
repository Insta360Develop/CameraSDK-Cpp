#include <iostream>
#include <thread>
#include <camera/camera.h>  
#include <camera/photography_settings.h>
#include <camera/device_discovery.h>
#include <llog/llog.h>
#include <regex>

class TestStreamDelegate: public ins_camera::StreamDelegate {
public:
    TestStreamDelegate() {
        file_ = fopen("/Users/capjason/Desktop/1.h264", "wb");
    }
    ~TestStreamDelegate() {
        fclose(file_);
    }
    
    void OnAudioData(const uint8_t* data, size_t size, int64_t timestamp) override {
//        LOG(INFO) << "on audio data:" << timestamp;
    }
    void OnVideoData(const uint8_t* data,size_t size, int64_t timestamp,int stream_index = 0) override {
//        LOG(INFO) << "on video frame:" << size << ";" << timestamp;
        if(stream_index == 0) {
            fwrite(data, sizeof(uint8_t), size, file_);
        }
    }
    void OnGyroData(const std::vector<ins_camera::GyroData>& data) override {
//        LOG(INFO) << "on gyro data:" << data.timestamp;
    }
    void OnExposureData(const ins_camera::ExposureData& data) override {
        std::cout << "time:" << std::setprecision(10) << data.timestamp << ";exposure:" << data.exposure_time << std::endl;
    }
    
private:
    FILE* file_;
};

int main(int argc, char* argv[])
{    
    ins::Configuration::GetInstance().Configure(ins::Configuration::LOG_LEVEL, "INFO");
     std::cout << "begin open camera" << std::endl;
    ins_camera::DeviceDiscovery discovery;
    auto list = discovery.GetAvailableDevices();
    for(int i = 0;i < list.size(); ++i) {
        auto desc = list[i];
        LOG(INFO) << "serial:" << desc.serial_number <<"\t"
                    << "camera type:" << int(desc.camera_type) << "\t"
                    << "lens type:" << int(desc.lens_type);
    }
     
    if (list.size() <= 0) {
        LOG(ERROR) << "no device found.";
        return -1;
    }

	ins_camera::Camera cam(list[0].info);
	if (!cam.Open()) {
		std::cout << "failed to open camera" << std::endl;
		return -1;
	}
    
    std::shared_ptr<ins_camera::StreamDelegate> delegate = std::make_shared<TestStreamDelegate>();
    cam.SetStreamDelegate(delegate);

	discovery.FreeDeviceDescriptors(list);

	std::cout << "Succeed to open camera..." << std::endl;

	std::cout << "Usage" << std::endl;
	std::cout << "1: take photo" << std::endl;
	std::cout << "2: get serial number" << std::endl;
	std::cout << "3: get file list(only video and photo)" << std::endl;
	std::cout << "4: delete file" << std::endl;
	std::cout << "5: download file" << std::endl;
    std::cout << "6: start recording" << std::endl;
    std::cout << "7: stop recording" << std::endl;
    std::cout << "8: test set exposure settings:" << std::endl;
    std::cout << "9: test set capture settings:" << std::endl;
	std::cout << "0: exit" << std::endl;

	int option;
	while (true) {
		std::cout << "please enter index: ";
		std::cin >> option;
		if (option < 0 || option > 20) {
			std::cout << "Invalid index" << std::endl;
			return 0;
		}

		if (option == 0) {
			break;
		}

		// take photo
		if (option == 1) {
			const auto url = cam.TakePhoto();
			if (!url.isSingleOrigin() || url.Empty()) {
				std::cout << "failed to take picture" << std::endl;
				return -1;
			}
			std::cout << "Take picture done: " << url.GetSingleOrigin() << std::endl;
		}

		// get serial number from camera
		if (option == 2) {
			const auto serial_number = cam.GetSerialNumber();
			if (serial_number.empty()) {
				std::cout << "failed to get serial number" << std::endl;
				return -1;
			}
			std::cout << "serial number: " << serial_number << std::endl;
		}

		// get camera file list(only video and photo for now)
		if (option == 3) {
			const auto file_list = cam.GetCameraFilesList();
			for (const auto& file : file_list) {
				std::cout << "File: " << file << std::endl;
			}
		}

		// delete file from camera
		if (option == 4) {
			std::string file_to_delete;
			std::cout << "please input full file path to delete: ";
			std::cin >> file_to_delete;
			const auto ret = cam.DeleteCameraFile(file_to_delete);
			if (ret) {
				std::cout << "Deletion succeed" << std::endl;
			}
		}

		// download file from camera
		if (option == 5) {
			std::string file_to_download;
			std::string file_to_save;
			std::cout << "please input full file path to download: ";
			std::cin >> file_to_download;
			std::cout << "please input full file path to save: ";
			std::cin >> file_to_save;
			const auto ret = cam.DownloadCameraFile(file_to_download,
				file_to_save);
			if (ret) {
				std::cout << "Download " << file_to_download << " succeed!!!" << std::endl;
			}
			else {
				std::cout << "Download " << file_to_download << " failed!!!" << std::endl;
			}
		}

        if (option == 6) {
            if (!cam.SetVideoCaptureParams({
                    ins_camera::VideoResolution::RES_2880_2880P30,
                    1024 * 1024 * 90
                })) {
                LOG(ERROR) << "failed to set capture settings.";
            }
            else {
                auto ret = cam.StartRecording();
                if (ret) {
                    LOG(INFO) << "success!";
                }
                else {
                    LOG(ERROR) << "failed to start recording";
                }
            }
        }

        if (option == 7) {
            auto url = cam.StopRecording();
            if(url.Empty()) {
                LOG(ERROR) << "stop recording failed";
                continue;
            }
            auto& origins = url.OriginUrls();
            LOG(INFO) << "stop recording success";
            for(auto& origin_url: origins) {
                LOG(INFO) << "url:" << origin_url;
            }
        }

        if (option == 8) {
            auto exposure = std::make_shared<ins_camera::ExposureSettings>();
            exposure->SetExposureMode(ins_camera::PhotographyOptions_ExposureMode::PhotographyOptions_ExposureOptions_Program_MANUAL);
            exposure->SetIso(400);
            exposure->SetShutterSpeed(1.0/120.0);
            auto ret = cam.SetExposureSettings(ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO, exposure);
            if (ret) {
                auto exposure_settings = cam.GetExposureSettings(ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO);;
                LOG(INFO) << "success!";
            }
            else {
                LOG(ERROR) << "failed to set exposure";
            }
        }

        if (option == 9) {
            auto settings = std::make_shared<ins_camera::CaptureSettings>();
            settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Saturation, 0);
            settings->SetWhiteBalance(ins_camera::PhotographyOptions_WhiteBalance_WB_AUTO);
            settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Brightness, 250);
            auto ret = cam.SetCaptureSettings(ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_IMAGE, settings);
            if (ret) {
                auto capture_settings = cam.GetCaptureSettings(ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_IMAGE);
                LOG(INFO) << "success!";
            }
            else {
                LOG(ERROR) << "failed to set capture settings";
            }
        }
        
        if(option == 10) {
            ins_camera::LiveStreamParam param;
            param.video_resolution = ins_camera::VideoResolution::RES_960_480P30;
            param.lrv_video_resulution = ins_camera::VideoResolution::RES_720_360P30;
            param.using_lrv = false;
            if(cam.StartLiveStreaming(param)) {
                LOG(INFO) << "successfully started live stream";
            }
        }
        if (option == 11) {
            if(cam.StopLiveStreaming()) {
                LOG(INFO) << "success!";
            } else {
                LOG(ERROR) << "failed to stop live.";
            }
        }
	}
	cam.Close();
	return 0;
}
