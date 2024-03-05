#pragma once

#include <string>
#include <memory>
#include <vector>
#include "ins_types.h"
#include "photography_settings.h"
#include "../stream/stream_delegate.h"
namespace ins_camera {
    class CameraImpl;
    /**
     * \class Camera
     * \brief A class that represents Insta360 camera,support Insta360 ONE X, Insta360 ONE R
     */
    class CAMERASDK_API Camera {
    public:
        /**
         * \brief see also DeviceDiscovery to get DeviceConnectionInfo.
         */
        Camera(const DeviceConnectionInfo& info);
        /**
         * \brief Open camera and start session
         * \return true if succeed, otherwise return false;
         */
        bool Open() const;
        /**
         * \brief Close camera and release resources
         */
        void Close() const;
        /**
         * \brief Get the serial number from camera
         * \return The serial number string
         */
        std::string GetSerialNumber() const;

        /**
         * \brief get type of camera. Camera type are models like OneX, OneR
         * be sure to call this function after Open()
         * \return lens type of the camera
         */
        CameraType GetCameraType() const;

        /**
         * \brief get lens type of camera. For model earlier than OneR, the lens type will be PanoDefault
         * be sure to call this function after Open()
         * \return lens type of the camera
         */
        CameraLensType GetCameraLensType() const;

        /**
        * \brief get uuid of camera. For model earlier than OneR, the lens type will be PanoDefault
        * be sure to call this function after Open()
        * \return lens type of the camera
        */
        std::string GetCameraUUID() const;

        /**
        * \brief get capture current status of camera.
        * be sure to call this function after Open()
        * \return lens type of the camera
        */
        CaptureStatus GetCaptureCurrentStatus() const;

        /**
        * \brief set service port of download service
        * be sure to call this function before Open()
        */
        void SetServicePort(int port);

        /**
        * \brief get offset of camera
        * be sure to call this function after Open()
        */
        std::string GetCameraOffset() const;

        /**
        * \brief get current camera encode type(h264 or h265)
        */
        VideoEncodeType GetVideoEncodeType() const;

        /**
         * \brief Control camera to take photo
         * \return The url of the photo if success, otherwise empty
         */
        MediaUrl TakePhoto() const;

        /**
         * \brief set exposure settings, the settings will only be applied to specified mode
         * \param mode the target mode you want to apply exposure settings, for normal video recording,
         *      use CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO, for normal still image capture,
         *      use CaperaFunctionMode::FUNCTION_MODE_NORMAL_IMAGE.
         * \param settings ExposureSettings containing exposure mode/iso/shutter/ev to be applied.
         * \return true on success, false otherwise.
         */
        bool SetExposureSettings(CameraFunctionMode mode, std::shared_ptr<ExposureSettings> settings);

        std::shared_ptr<ExposureSettings> GetExposureSettings(CameraFunctionMode mode) const;

        /**
         * \brief set capture settings, the settings will only be applied to specified mode
         * \param mode the target mode you want to apply exposure settings, for normal video recording,
         *      use CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO, for normal still image capture,
         *      use CaperaFunctionMode::FUNCTION_MODE_NORMAL_IMAGE.
         * \param settings CaptureSettings containing capture settings like saturation,contrast,whitebalance,sharpness,brightness and etc.
         * \return true on success, false otherwise.
         */
        bool SetCaptureSettings(CameraFunctionMode mode, std::shared_ptr<CaptureSettings> settings);

        std::shared_ptr<CaptureSettings> GetCaptureSettings(CameraFunctionMode mode) const;

        /**
         * \brief set capture settings such as resolutions, bitrate,
         * \param params RecordParams containing settings you want to apply
         * \param mode the target mode you want to apply capture settings, the mode must be one of video modes.
         * \return true on success, false otherwise.
         */
        bool SetVideoCaptureParams(RecordParams params, CameraFunctionMode mode = CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO);

        /**
         * \brief Start Recording.
         * \return true on success, false otherwise
         */
        bool StartRecording();

        /**
         * \brief Stop Recording.
         * \return video uri of the video, it may contain low bitrate proxy video
         */
        MediaUrl StopRecording();

        /**
        * \brief start preview stream
        */
        bool StartLiveStreaming(const LiveStreamParam& param);

        /**
        * \brief stop preview stream
        */
        bool StopLiveStreaming();

        /**
            \brief set a stream delegate, you may implement StreamDelegate to handle stream data.
         */
        void SetStreamDelegate(std::shared_ptr<StreamDelegate>& delegate);

        /**
         * \brief Delete the specified file from camera
         */
        bool DeleteCameraFile(const std::string& filePath) const;

        /**
         * \brief Download the specified file from camera
         */
        bool DownloadCameraFile(const std::string& remoteFilePath, const std::string& localFilePath, DownloadProgressCallBack callback = nullptr) const;

        /**
         * \brief Get the list of files stored in camera storage card
         */
        std::vector<std::string> GetCameraFilesList() const;

        /**
        * \brief set timelapse param
        */
        bool SetTimeLapseOption(TimelapseParam params);

        /**
        * \brief start timelapse
        */
        bool StartTimeLapse(CameraTimelapseMode mode);

        /**
        * \brief stop timelapse
        * \return video uri of the video, it may contain low bitrate proxy video
        */
        MediaUrl StopTimeLapse(CameraTimelapseMode mode);

        /**
        * \brief sync local time to camera
        */
        bool SyncLocalTimeToCamera(uint64_t time);

        /**
        * \brief device is connected
        */
        bool IsConnected();

        /**
        * \brief get battry info
        */
        bool GetBatteryStatus(BatteryStatus& status);

        /**
        * \brief get storage state
        */
        bool GetStorageState(StorageStatus& status);

        /**
         * \brief get http base url with trailing slash. eg. "http://127.0.0.1:9099/",
         *  you could get url of camera file by concanate base url and file uri
         */
        std::string GetHttpBaseUrl() const;

        bool SetPhotoSize(CameraFunctionMode mode, const PhotoSize& photo_size);

        bool SetActiveSensor(SensorDevice sensor_device);

    private:
        std::shared_ptr<CameraImpl> impl_;
    };
}
