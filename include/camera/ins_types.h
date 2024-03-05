#pragma once
#include <string>
#include <vector>
#include <functional>

#if WIN32
#ifdef USE_EXPORTDLL
#define CAMERASDK_API _declspec(dllexport)
#else
#define CAMERASDK_API _declspec(dllimport)
#endif
#else
#define CAMERASDK_API
#endif

namespace ins_camera {
    enum class CameraType {
        Unknown,
        Insta360OneX,
        Insta360OneR,
        Insta360OneRS,
        Insta360OneX2,
        Insta360X3
    };

    enum class CameraLensType {
        PanoDefault,
        Wide577,
        Pano577,
        Wide283,
        Pano283,
        Wide586,
        Pano586,
        Action577,
    };

    enum class ConnectionType {
        USB,
        Wifi,
        Bluetooth
    };

    enum class VideoEncodeType {
        H264,
        H265
    };

    struct DeviceConnectionInfo {
        ConnectionType connection_type;
        void* native_connection_info;
    };

    struct DeviceDescriptor {
        CameraType camera_type;
        CameraLensType lens_type;
        std::string serial_number;
        DeviceConnectionInfo info;
    };

    enum SensorDevice {
        SENSOR_DEVICE_FRONT = 1,
        SENSOR_DEVICE_REAR = 2,
        SENSOR_DEVICE_ALL = 3
    };

    enum CardState {
        STOR_CS_PASS = 0,
        STOR_CS_NOCARD = 1,
        STOR_CS_NOSPACE = 2,
        STOR_CS_INVALID_FORMAT = 3,
        STOR_CS_WPCARD = 4,
        STOR_CS_OTHER_ERROR = 5
    };

    struct StorageStatus {
        CardState state;
        uint64_t free_space;
        uint64_t total_space;
    };

    using DownloadProgressCallBack = std::function<void(int64_t download_size, int64_t total_size)>;

    class CAMERASDK_API MediaUrl {
    public:
        MediaUrl(const std::vector<std::string>& uris, const std::vector<std::string>& lrv_uris = std::vector<std::string>());
        bool Empty() const;
        bool IsSingleOrigin() const;
        bool IsSingleLRV() const;
        std::string GetSingleOrigin() const;
        std::string GetSingleLRV() const;
        const std::vector<std::string>& OriginUrls() const;
        const std::vector<std::string>& LRVUrls() const;
    private:
        std::vector<std::string> uris_;
        std::vector<std::string> lrv_uris_;
    };
}
