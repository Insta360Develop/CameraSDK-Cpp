# CameraSDK-Cpp
CameraSDK-Cpp is a C++ library to control Insta360 cameras.

### Supported cameras
| Model | Link |
| :------- | :-------- |
| ONE X | http://insta360.com/product/insta360-onex/ |
| ONE R Dual-Lens Edition | http://insta360.com/product/insta360-oner_twin-edition |
| ONE R Wide Edition | http://insta360.com/product/insta360-oner_twin-edition |
| ONE R 1-inch Edition | http://insta360.com/product/insta360-oner_1inch-edition |

### Supported platforms

| Platform | Version |
| :--- | :--- |
| Windows | Windows 7 or later, only x64 supported |
| MacOS | 10.8.2 or later, only x64 supported |
| Linux | Ubuntu/CentOS/Raspberry Pi, other distributions need to be tested |


## Table of contents
- [Prerequisite](#prerequistie)
  - [Setup your camera connection mode](#setup-camera).
  - [Install Driver](#install-driver)
- [Running test demo](#running-demo)
- [Get started](#get-started)
  + [Discover and connect to camera](#connect)
  + [Take Picture](#takepicture)
  + [Recording](#recording)
  + [Set Exposure Settings](#exposure-settings)
  + [Set Capature Settings](#capture-settings)


## <a name="prerequistie" />Prerequisite
Before running test demo, there are several work need to be prepared. 

### <a name="setup-camera" />Setup your camera connection mode

By default, when you connect Insta360 cameras to your computer, the camera will switch to udisk mode automatically, which makes the camera as a USB storage device.
We need to switch the camera to correct mode before we can connect and control it.

#### For ONE X

You need to upgrade to  a special version of firmware, [download here](https://insta360-dev.oss-cn-hangzhou.aliyuncs.com/developer/releases/a33b3362-4767-47c3-ba9d-6ed07febb210.zip)

After upgraded, on camera, go to settings, find USB, set it to **Android** mode.

#### For ONE R

On camera, swipe down the screen to main menu, go to Settings->General, Set USB Mode to **Android**, Set U-Disk Mode to **Mobile** Mode.

### <a name="install-driver" />Install Driver

**On Linux**, please make sure your distribution has libusb installed. 

You may install via yum or apt-get

```shell
sudo apt-get install libusb-dev
sudo apt-get install libudev-dev
```

or build from source

```shell
wget http://sourceforge.net/projects/libusb/files/libusb-1.0/libusb-1.0.9/libusb-1.0.9.tar.bz2
tar xjf libusb-1.0.9.tar.bz2
cd libusb-1.0.9
./configure 
make
sudo make install
```

After installing driver, check whether the camera is detected via `lsusb` command, if any USB device with vender id 0x2e1a is found, congratulations, your driver is successfully installed.

**Note**: On Linux，demo program must be run by sudo, for example

```
sudo ./CameraSDKDemo //for ubuntu
```

**On Mac OSX**, nothing need to be installed.

**On Windows**, please make sure libusbK driver is installed. You could do that by install [libusbK](https://sourceforge.net/projects/libusbk/files/libusbK-release/3.0.7.0/) directly, or using [zadig](https://zadig.akeo.ie/) to help install the libusbK driver.	

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
const auto url = cam.TakePhoto();
if (url.Empty()) {
    std::cout << "failed to take picture" << std::endl;
    return -1;
}
std::cout << "Take picture done: " << url.GetSingleOrigin() << std::endl;
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
if(url.Empty()) {
    LOG(ERROR) << "stop recording failed";
    continue;
}
auto& origins = url.OriginUrls();
LOG(INFO) << "stop recording success";
for(auto& origin_url: origins) {
    LOG(INFO) << "url:" << origin_url;
}
```
Same as Picture, you can access the video via http request. The returned MediaUrl instance may contains multiple origin urls as well as low bitrate video urls.

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

Note：On windows，use Vistual Studio 2015. On ubuntu, use g++;