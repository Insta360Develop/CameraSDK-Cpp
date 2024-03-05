#pragma once
#include <camera/ins_types.h>
#include <vector>


namespace ins_camera {
    /**
     * \class DeviceDiscovery
     * \brief a convenient class for discovery Insta360 camera devices. 
     */
    class CAMERASDK_API DeviceDiscovery {
    public:
        /**
         * \brief get a list of available devices. This method may establish a connection with
         *        detected cameras to get some basic information such as serial_number.
         * \return a list of DeviceDescriptor describing detected cameras. 
         *  Remember to call FreeDeviceDescriptors(std::vector<DeviceDescriptor> descs)
         *  on the returned list to avoid memory leak.
         */
        std::vector<DeviceDescriptor> GetAvailableDevices();
        /**
         * \brief free memory allocated for DeviceDescriptor
         */
        void FreeDeviceDescriptors(std::vector<DeviceDescriptor> descs);
    private:
        std::vector<DeviceDescriptor> GetAvailableUSBDevices();
        std::vector<DeviceDescriptor> GetAvailableWifiDevices();
    };
}
