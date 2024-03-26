#pragma once
#include <cstdlib> 
#include <filesystem>
#include <iostream> 

class Utils
{
public:
	static std::filesystem::path getSavePath()
	{
		const char* homeDir = std::getenv("HOME");
		if (!homeDir)
		{
			throw std::runtime_error("HOME environment variable is not set.");
		}

		std::filesystem::path savePath(homeDir);
		savePath /= SAVE_DIR;	

		if (!std::filesystem::exists(savePath))
		{
			std::filesystem::create_directories(savePath);
		}
        std::cout << savePath;
		return savePath;
	}
private:
    inline static const std::string SAVE_DIR = "insta360Downloads/";
};