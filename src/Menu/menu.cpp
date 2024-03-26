#pragma once
#include "menu.h"

#include <memory>

Menu::Menu(std::shared_ptr<LeticoCamera> camera): mCamera(camera)
{
}

void Menu::showOptions()
{
	while (true)
	{
		displayMenu();
		int option = getUserInput();
		if (option == 0)
		{
			break;
		}
		handleOption(option);
	}
}
void Menu::displayMenu()
{
	std::cout << "\nMenu Options:" << std::endl;
	std::cout << "1: take photo" << std::endl;
	std::cout << "2: get serial number" << std::endl;
	std::cout << "3: get file list(only video and photo)" << std::endl;
	std::cout << "4: delete file" << std::endl;
	std::cout << "5: download file" << std::endl;
	std::cout << "6: start recording" << std::endl;
	std::cout << "7: stop recording" << std::endl;
	std::cout << "8: test set exposure settings:" << std::endl;
	std::cout << "9: test set capture settings:" << std::endl;
	std::cout << "10: start preview live streaming:" << std::endl;
	std::cout << "11: stop preview live streaming:" << std::endl;
	std::cout << "16: get uuid " << std::endl;
	std::cout << "17: test take photo and download " << std::endl;
	std::cout << "18: get current capture status " << std::endl;
	std::cout << "19: start timelapse " << std::endl;
	std::cout << "20: stop timelapse " << std::endl;
	std::cout << "21: get batty " << std::endl;
	std::cout << "22: get storage info " << std::endl;
	std::cout << "23: Download all files " << std::endl;
	std::cout << "0: exit" << std::endl;
}

int Menu::getUserInput()
{
	int option;
	std::cout << "Please enter an option: ";
	std::cin >> option;
	return option;
}

void Menu::handleOption(int option)
{
	switch (option)
	{
	case 1:
	{
		mCamera->takePhoto();
		break;
	}
	case 2:
	{
		mCamera->getSerialNumber();
		break;
	}
	case 3:
	{
		mCamera->getFileList();
		break;
	}
	case 4:
	{
		std::string file_to_delete;
		std::cout << "please input full file path to delete: ";
		std::cin >> file_to_delete;
		mCamera->deleteFile(file_to_delete);
		break;
	}
	case 5:
	{
		std::string file_to_download;
		auto allFiles = mCamera->getFileList();
		std::cout << "please input full file path to download: ";
		std::cin >> file_to_download;
		mCamera->downloadFile(file_to_download);
		break;
	}
	case 6:
		mCamera->startRecording();
		break;
	case 7:
		mCamera->stopRecording();
		break;
	case 8:
		mCamera->setExposureSettings();
		break;
	case 9:
		mCamera->setCaptureSettings();
		break;
	case 10:
		mCamera->startPreviewLiveStream();
		break;
	case 11:
		mCamera->stopPreviewLiveStream();
		break;
	case 16:
		mCamera->getUUID();
		break;
	case 17:
		mCamera->takePhotoAndDownload();
		break;
	case 18:
		mCamera->getCurrentCaptureStatus();
		break;
	case 19:
		mCamera->startTimelapse();
		break;
	case 20:
		mCamera->stopTimelapse();
		break;
	case 21:
		mCamera->getBatteryStatus();
		break;
	case 22:
		mCamera->getStorageInfo();
		break;
	case 23:
		mCamera->downloadAllFiles();
		break;
	default:
		std::cout << "Invalid option" << std::endl;
		break;
	}
}
