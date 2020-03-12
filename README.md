# CameraSDK-Cpp
CameraSDK-Cpp is a C++ library to control Insta360 cameras.

### Supported cameras
| Model | Link | 
| :------- | :-------- |
| ONE X | http://insta360.com/product/insta360-onex/ |
| ONE R Twin Edition | http://insta360.com/product/insta360-oner_twin-edition |
| ONE R Wide Edition | http://insta360.com/product/insta360-oner_twin-edition |
| ONE R 1-inch Edition | http://insta360.com/product/insta360-oner_1inch-edition |

### Supported platforms

| Platform | Version | 
| :--- | :--- |
| Windows | Windows 8 or later, only x64 supported |
| MacOS | 10.13 or later, only x64 supported | 
| Linux | Ubuntu/CentOS/Raspberry Pi | 


## Table of contents
- [Prerequisite](#prerequistie)
  + [Install Driver](#install-driver)
  + [Setup your camera connection mode](#setup-camera).
- [Running test demo](#running-demo)
- [Get started](#get-started)
  + [Discover and connect to camera](#connect)
  + [Take Picture](#takepicture)
  + [Recording](#recording)
  + [Set Exposure Settings](#exposure-settings)
  + [Set Capature Settings](#capture-settings)


## <a name="prerequistie" />Prerequisite
Before running test demo, there are several work need to be prepared. 
### <a name="install-driver" />Install Driver
### <a name="setup-camera" />Setup your camera connection mode
By default, when you connect Insta360 cameras to your computer, the camera will switch to udisk mode automatically, which makes the camera as a usb storage device.
We need to switch the camera to correct mode before we can connect and control it.
#### For ONE X
On camera, go to settings, find USB, set it to Android mode.
#### For ONE R
On camera, swipe down the screen to main menu, go to Settings->General, Set USB Mode to Android, Set U-Disk Mode to PC Mode.
## <a name="running-demo"/>Running test demo


## <a name="get-started" />Get Started
### <a name="connect" />Discover and connect to camera.
When both of your camera and computer are prepared, you can connect your camera to your computer via USB type-c.
#### Discover your camera
You can use class DeviceDiscovery to easily find available cameras.
  
```C++  
#include <camera/device_discovery.h>
...
ins_camera::DeviceDiscovery discovery;
auto list = discovery.GetAvailableDevices();
for(int i = 0;i < list.size(); ++i) {
    std::cout << "device:" << list[i].serial_number << std::endl;
}
```
The DeviceDiscovery class may establish a connection with the detected camera to retrieve some basic information, such as serial number.
The connection will be closed right after information retrieved. Remember to call **discovery.FreeDeviceDescriptors** to free allocated.resources when you no longer use the DeviveDescriptor in the list.

#### Connect to the camera
The **discovery.GetAvailableDevices()** method returns a list of **DeviceDescriptor**, which contains neccessary **DeviceConnectionInfo** of the camera.
```C++
#include <camera/camera.h>
...
ins_camera::Camera cam(list[0].info);
if(!cam.Open()) {
  std::cout << "failed to open camera" << std::endl;
  return -1;
}
... 
cam.Close(); // close the connection after use.
```
After **cam.Open()**, the DeviceDescriptor is no longer used.

### <a name='takepicture' />Take Picture
We try to simplify the SDK to make integration for developers more easily.
```C++ 
#include <camera/camera.h>
...
auto url = cam.TakePicture();
if(!url.empty()) {
  std::cout << "picture has been saved to url:" << url << std::endl; //url is the path to the picture on camera, like /DCIM/Camera01/IMG_20200312_112432_00_260.jpg
}
```
Once camera be opened, a http tunnel is created on http://localhost:9099. You could make a http request to access the image on camera.
The full url might like this: http://localhost:9099/DCIM/Camera01/IMG_20200312_113006_00_261.jpg

### <a name='recording' />Recording
Recording is as simple as TakePicture
```C++
#include <camera/camera.h>
...
if (!cam.SetVideoCaptureParams({ins_camera::VideoResolution::RES_3840_2160p30,
  1024 * 1024 * 60
})) {
  std::cout << "Failed to set recording params." << std::endl;
} else {
  auto success = cam.StartRecording();
  if (success) {
      std::cout << "Success!" << std::endl;
  } else {
      std::cout << "Failed to start recording" << std::endl;
  }
}
```

```C++
#include <camera/camera.h>
...
auto url = cam.StopRecording();
if(!url.empty()) {
   std::cout << "picture has been saved to url:" << url << std::endl; //url is the path to the video on camera
}
```
Same as Picture, you can access the video via http request.

### <a name="exposure-settings" />Set Exposure Settings
Exposure settings are independent between function modes(like TakePicture and Recording). So you need to specify function mode when you set exposure settings.
```C++
#include <camera/camera.h>
...
auto exposure = std::make_shared<ins_camera::ExposureSettings>();
exposure->SetExposureMode(ins_camera::PhotographyOptions_ExposureMode::PhotographyOptions_ExposureOptions_Program_MANUAL);//set to manual exposure mode
exposure->SetIso(400); // set iso to 400
exposure->SetShutterSpeed(1.0/120.0); // set shutter to 1/120 second.
auto success = cam.SetExposureSettings(ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO, exposure);
if (success) {
  std::cout << "Success!" << std::endl;
} else {
  std::cout << "Failed to set exposure settings" << std::endl;
}
```
### <a name="capture-settings"/>Set Capture Settings
Capture settings are similar to [Exposure Settings](#exposure-settings)

```C++
#include <camera/camera.h>
...
auto settings = std::make_shared<ins_camera::CaptureSettings>();
settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Saturation, 60);// set saturation to 60
settings->SetWhiteBalance(ins_camera::PhotographyOptions_WhiteBalance_WB_AUTO); // set white balance to AUTO mode
settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Brightness, 30); // set brightness to 30
settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Sharpness,4); // set sharpness to 4
settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Contrast,100); // set contrast to 100
auto ret = cam.SetCaptureSettings(ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_IMAGE, settings);
auto success = cam.SetCaptureSettings(ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO, settings);
if (success) {
  std::cout << "Success!" << std::endl;
} else {
  std::cout << "Failed to set exposure settings" << std::endl;
}
```

For more details, please view documentations under /doc directory.
