#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include "ins_types.h"

namespace ins_camera {
	struct EnumClassHash {
		template<typename T>
		std::size_t operator()(T const& v) const noexcept {
			return static_cast<std::size_t>(v);
		}
	};

	enum CameraFunctionMode {
		FUNCTION_MODE_NORMAL = 0,
		FUNCTION_MODE_LIVE_STREAM = 1,
		FUNCTION_MODE_MOBILE_TIMELAPSE = 2,
		FUNCTION_MODE_INTERVAL_SHOOTING = 3,
		FUNCTION_MODE_HIGH_FRAME_RATE = 4,
		FUNCTION_MODE_BURST = 5,
		FUNCTION_MODE_NORMAL_IMAGE = 6,
		FUNCTION_MODE_NORMAL_VIDEO = 7,
		FUNCTION_MODE_HDR_IMAGE = 8,   // HDR Image
		FUNCTION_MODE_HDR_VIDEO = 9,
		FUNCTION_MODE_INTERVAL_VIDEO = 10,
		FUNCTION_MODE_STATIC_TIMELAPSE = 11,
		FUNCTION_MODE_TIMESHIFT = 12,
		FUNCTION_MODE_AEB_NIGHT_IMAGE = 13,
		FUNCTION_MODE_NORMAL_POWER_PANO_IMAGE = 14,
		FUNCTION_MODE_HDR_POWER_PANO_IMAGE = 15,
		FUNCTION_MODE_SUPER_VIDEO = 16,
		FUNCTION_MODE_LOOP_RECORDING_VIDEO = 17,
		FUNCTION_MODE_STARLAPSE_IMAGE = 18,
		FUNCTION_MODE_FPV_VIDEO = 19,
		FUNCTION_MODE_MOVIE_VIDEO = 20,
		FUNCTION_MODE_SLOWMOTION_VIDEO = 21,
		FUNCTION_MODE_SELFIE_VIDEO = 22,
	};

	enum CameraTimelapseMode {
		TIMELAPSE_MIXED = 0,				// VIDEO 和 IMAGE 混合的设置. [为了兼容而保留]
		MOBILE_TIMELAPSE_VIDEO = 1,			// 移动延时 的设置
		TIMELAPSE_INTERVAL_SHOOTING = 2,	// 间隔拍照 的设置
		STATIC_TIMELAPSE_VIDEO = 3,			//定位延时的设置
		TIMELAPSE_INTERVAL_VIDEO = 4,		// 间隔录像的设置
		TIMELAPSE_STARLAPSE_SHOOTING = 5,	// 星空模式拍照的设置
	};

	enum CaptureStatus {
		NOT_CAPTURE = 0,
		NORMAL_CAPTURE = 1,
		TIMELAPSE_CAPTURE = 2,
		INTERVAL_SHOOTING_CAPTURE = 3,
		SINGLE_SHOOTING = 4,
		HDR_SHOOTING = 5,
		SELF_TIMER_SHOOTING = 6,
		BULLET_TIME_CAPTURE = 7,
		SETTINGS_NEW_VALUE = 8,
		HDR_CAPTURE = 9,
		BURST_SHOOTING = 10,
		STATIC_TIMELAPSE_SHOOTING = 11,
		INTERVAL_VIDEO_CAPTURE = 12,
		TIMESHIFT_CAPTURE = 13,
		AEB_NIGHT_SHOOTING = 14,
		SINGLE_POWER_PANO_SHOOTING = 15,
		HDR_POWER_PANO_SHOOTING = 16,
		SUPER_NORMAL_CAPTURE = 17,
		LOOP_RECORDING_CAPTURE = 18,
		STARLAPSE_SHOOTING = 19,
	};

	enum VideoResolution {
		RES_3840_1920P30 = 0,
		RES_2560_1280P30 = 1,
		RES_1920_960P30 = 2,
		RES_2560_1280P60 = 3,
		RES_2048_512P120 = 4,
		RES_3328_832P60 = 5,
		RES_3072_1536P30 = 6,
		RES_2240_1120P30 = 7,
		RES_2240_1120P24 = 8,
		RES_1440_720P30 = 9,
		RES_2880_2880P30 = 10,
		RES_3840_1920P60 = 11,
		RES_3840_1920P50 = 12,
		RES_3008_1504P100 = 13,
		RES_960_480P30 = 14,
		RES_3040_1520P30 = 15,
		RES_2176_1088P30 = 16,
		RES_720_360P30 = 17,
		RES_480_240P30 = 18,
		RES_2880_2880P25 = 19,
		RES_2880_2880P24 = 20,
		RES_3840_1920P20 = 21,
		RES_1920_960P20 = 22,
		RES_3840_2160P60 = 23,
		RES_3840_2160P30 = 24,
		RES_2720_1530P100 = 25,
		RES_1920_1080P200 = 26,
		RES_1920_1080P240 = 27,
		RES_1920_1080P120 = 28,
		RES_1920_1080P30 = 29,
		RES_5472_3078P30 = 30,
		RES_4000_3000P30 = 31,
		RES_854_640P30 = 32,
		RES_720_406P30 = 33,
		RES_424_240P15 = 34,
		RES_1024_512P30 = 35,
		RES_640_320P30 = 36,
		RES_5312_2988P30 = 37,
		RES_2720_1530P60 = 38,
		RES_2720_1530P30 = 39,
		RES_1920_1080P60 = 40,
		RES_2720_2040P30 = 41,
		RES_1920_1440P30 = 42,
		RES_1280_720P30 = 43,
		RES_1280_960P30 = 44,
		RES_1152_768P30 = 45,
		RES_5312_2988P25 = 46,
		RES_5312_2988P24 = 47,
		RES_3840_2160P25 = 48,
		RES_3840_2160P24 = 49,
		RES_2720_1530P25 = 50,
		RES_2720_1530P24 = 51,
		RES_1920_1080P25 = 52,
		RES_1920_1080P24 = 53,
		RES_4000_3000P25 = 54,
		RES_4000_3000P24 = 55,
		RES_2720_2040P25 = 56,
		RES_2720_2040P24 = 57,
		RES_1920_1440P25 = 58,
		RES_1920_1440P24 = 59,
		RES_2560_1440P30 = 60,
		RES_2560_1440P60 = 61,
		RES_1920_1440P60 = 62,
		RES_1440_2560P30 = 63,
		RES_1080_1920P30 = 64,
		RES_1440_1920P30 = 65,
		RES_1440_2560P60 = 66,
		RES_1440_1920P60 = 67,
		RES_1080_1920P60 = 68,
		RES_2560_1440P25 = 69,
		RES_1440_2560P25 = 70,
		RES_1080_1920P25 = 71,
		RES_720_1280P30 = 72,
		RES_960_1280P30 = 73,
		RES_1152_1152P30 = 74,
		RES_640_360P30 = 75,
		RES_640_480P30 = 76,
		RES_360_640P30 = 77,
		RES_480_640P30 = 78,
		RES_368_368P30 = 79,
		RES_854_854P30 = 80,
		RES_1920_1080P50 = 81,
		RES_1080_1920P50 = 82,
		RES_2560_1440P50 = 83,
		RES_1440_2560P50 = 84,
		RES_1080_1920P24 = 85,
		RES_2560_1440P24 = 86,
		RES_1440_2560P24 = 87,
		RES_1080_1920P120 = 88,
		RES_1280_1280P30 = 89,
		RES_2880_2880P15 = 90,
		RES_1440_720P15 = 91,
		RES_3840_2160P50 = 92,
		RES_1280_1280P25 = 93,
		RES_3040_3040P24 = 94,
		RES_3040_3040P25 = 95,
		RES_3040_3040P30 = 96,
		RES_3840_1920P24 = 97,
		RES_3840_1920P25 = 98,
		RES_3040_1520P50 = 99,
		RES_2720_1530P50 = 100,
		RES_5312_3552P24 = 101,
		RES_5312_3552P25 = 102,
		RES_5312_3552P30 = 103,
		RES_5472_2328P24 = 104,
		RES_5472_2328P25 = 105,
		RES_5472_2328P30 = 106,
		RES_1600_900P30 = 107,
		RES_1760_990P30 = 108,
		RES_3840_1634P30 = 109,
		RES_3840_1634P25 = 110,
		RES_3840_1634P24 = 111,
		RES_6720_2856P25 = 112,
		RES_6720_2856P24 = 113,
		RES_6016_2560P25 = 114,
		RES_6016_2560P24 = 115,
		RES_3200_3200P24 = 116,
		RES_3200_3200P25 = 117,
		RES_3200_3200P30 = 118,
		RES_3072_3072P24 = 119,
		RES_3072_3072P25 = 120,
		RES_3072_3072P30 = 121,
		RES_2160_3840P60 = 122,
		RES_2160_3840P50 = 123,
		RES_2160_3840P30 = 124,
		RES_2160_3840P25 = 125,
		RES_2160_3840P24 = 126,
		RES_2880_2880P5 = 127,
		RES_2880_2880P2 = 128,
		RES_2880_2880P1 = 129,
		RES_2880_2880P05 = 130,
		RES_3000_1500P120 = 131,
		RES_1530_2720P60 = 132,
		RES_1530_2720P50 = 133,
		RES_1530_2720P30 = 134,
		RES_1530_2720P25 = 135,
		RES_1530_2720P24 = 136,
		RES_3840_960P120 = 137,
		RES_1152_648P30 = 138,
		RES_648_1152P30 = 139,
		RES_1152_864P30 = 140,
		RES_864_1152P30 = 141,
		RES_2944_2880P30 = 142,
		RES_3920_1920P30 = 143,
		RES_4000_2000P24 = 144,
		RES_4000_2000P25 = 145,
		RES_4000_2000P30 = 146,
		RES_4000_2000P50 = 147,
		RES_4000_2000P60 = 148,
		RES_1530_2720P100 = 149,
		RES_1920_1080P100 = 150,
		RES_1080_1920P100 = 151,
		RES_4000_3000P50 = 152,
		RES_4000_3000P60 = 153,
		RES_7680_4320P30 = 154,
		RES_2944_736P200 = 155,
		RES_3456_1944P24 = 156,
		RES_3456_1944P25 = 157,
		RES_3456_1944P30 = 158,
		RES_3456_1944P50 = 159,
		RES_3456_1944P60 = 160,
		RES_1944_3456P24 = 161,
		RES_1944_3456P25 = 162,
		RES_1944_3456P30 = 163,
		RES_2944_736P180 = 164,
		RES_3840_3840P24 = 165,
		RES_3840_3840P25 = 166,
		RES_3840_3840P30 = 167,
		RES_3584_2400P24 = 168,
		RES_3584_2400P25 = 169,
		RES_3584_2400P30 = 170,
		RES_3584_2400P50 = 171,
		RES_3584_2400P60 = 172,
		RES_2400_3584P24 = 173,
		RES_2400_3584P25 = 174,
		RES_2400_3584P30 = 175,
		RES_2400_3584P50 = 176,
		RES_2400_3584P60 = 177,
		RES_2880_1440P24 = 178,
		RES_2880_1440P25 = 179,
		RES_2880_1440P30 = 180,
		RES_2880_1440P50 = 181,
		RES_2880_1440P60 = 182,
		RES_1440_2880P24 = 183,
		RES_1440_2880P25 = 184,
		RES_1440_2880P30 = 185,
		RES_1440_2880P50 = 186,
		RES_1440_2880P60 = 187,
		RES_2944_2944P30 = 188,
		RES_2944_2944P25 = 189,
		RES_2944_2944P24 = 190,
		RES_1472_1472P30 = 191,
		RES_3584_2016P30 = 192,
		RES_3584_2016P25 = 193,
		RES_3584_2016P24 = 194,
		RES_2016_3584P30 = 195,
		RES_2016_3584P25 = 196,
		RES_2016_3584P24 = 197,
		RES_2880_720P30 = 198,
		RES_3584_2016P50 = 199,
		RES_3584_2016P60 = 200,
		RES_2016_3584P50 = 201,
		RES_2016_3584P60 = 202,
		RES_3840_2880P30 = 203,
		RES_3840_2880P25 = 204,
		RES_3840_2880P24 = 205,
	};

	//照片大小
	enum PhotoSize {
		Size_6912_3456 = 0,	   // X3 18MP
		Size_6272_3136 = 1,
		Size_6080_3040 = 2,
		Size_4000_3000 = 3,
		Size_4000_2250 = 4,
		Size_5212_3542 = 5,
		Size_5312_2988 = 6,
		Size_8000_6000 = 7,
		Size_8000_4500 = 8,
		Size_2976_2976 = 9,
		Size_5984_5984 = 10,
		Size_11968_5984 = 11,  // X3 72MP
		Size_5952_2976 = 12,
	};

	enum PhotographyOptions_ExposureMode {
		PhotographyOptions_ExposureOptions_Program_AUTO = 0,
		PhotographyOptions_ExposureOptions_Program_ISO_PRIORITY = 1,
		PhotographyOptions_ExposureOptions_Program_SHUTTER_PRIORITY = 2,
		PhotographyOptions_ExposureOptions_Program_MANUAL = 3,
		PhotographyOptions_ExposureOptions_Program_ADAPTIVE = 4,
		PhotographyOptions_ExposureOptions_Program_FULL_AUTO = 5 // X3
	};

	enum PhotographyOptions_WhiteBalance {
		PhotographyOptions_WhiteBalance_WB_UNKNOWN = -1,
		PhotographyOptions_WhiteBalance_WB_AUTO = 0,
		PhotographyOptions_WhiteBalance_WB_2700K = 1,
		PhotographyOptions_WhiteBalance_WB_4000K = 2,
		PhotographyOptions_WhiteBalance_WB_5000K = 3,
		PhotographyOptions_WhiteBalance_WB_6500K = 4,
		PhotographyOptions_WhiteBalance_WB_7500K = 5
	};

	enum PowerType {
		BATTERY = 0,
		ADAPTER = 1,
	};

	struct BatteryStatus {
		PowerType power_type;
		uint32_t battery_level;
		uint32_t battery_scale;
	};

	class ExposureSettingsPrivate;
	class CAMERASDK_API ExposureSettings {
	public:
		friend class Camera;
		ExposureSettings();
		/**
		 * \brief set iso value,this value will only take effect in
		 *   manual and PhotographyOptions_ExposureMode::PhotographyOptions_ExposureOptions_Program_ISO_PRIORITY mode.
		 * \param value int value representing iso. available values are like 100,400,800,1600,etc.
		 */
		void SetIso(int32_t value);
		/**
		 * \brief set shutter,this value will only take effect in manual
		 *  and PhotographyOptions_ExposureMode::PhotographyOptions_ExposureOptions_Program_SHUTTER_PRIORITY mode.
		 * \param value double value representing shutter speed in second. available values are like 1/30,1/60,1/120,etc.
		 */
		void SetShutterSpeed(double speed);
		void SetExposureMode(PhotographyOptions_ExposureMode mode);
		void SetEVBias(int32_t value);

		int32_t Iso() const;
		double ShutterSpeed() const;
		PhotographyOptions_ExposureMode ExposureMode() const;
		int32_t EVBias() const;

	private:
		std::shared_ptr<ExposureSettingsPrivate> private_impl_;
	};
	/**
	 * \class CaptureSettings
	 * \brief a class wrapping capture settings. CaptureSettings holds the temperary settings value
	 * you want to apply to the camera. The exactly settings to be applied are implied by a list of
	 * SettingType,which you may get by calling GetSettingTypes(). The types will be automatically recorded
	 * when you call any SetXXX() function. However, you can also ResetSettingTypes() or UpdateSettingTypes() mannually.
	 *
	 * ## available settings and value range:
	 * - Contrast: 0~256, default 64
	 * - Saturation:0~256, default 64
	 * - Brightness:-256~256, default 0
	 * - WhiteBalance: see #PhotographyOptions_WhiteBalance
	 * - Sharpness: 0~6, default 3
	 */
	class CAMERASDK_API CaptureSettings {
	public:

		CaptureSettings() = default;
		CaptureSettings(const CaptureSettings& other);
		enum SettingsType {
			CaptureSettings_Contrast = 0,
			CaptureSettings_Saturation,
			CaptureSettings_Brightness,
			CaptureSettings_Sharpness,
			CaptureSettings_WhiteBalance
		};

		/**
		 * \brief get a list of settings types you have set. Any settings not included in this list
		 *   will not be applied to the camera.
		 * \return a list of SettingsType that will be applied
		 */
		std::vector<SettingsType> GetSettingTypes() const;

		/**
		 * \brief set setting types to be applied mannually.
		 */
		void UpdateSettingTypes(std::vector<SettingsType>& types);

		/**
		 * \brief reset setting types, any types recorded when you call SetXXX()
		 * previously will not be applied until you call SetXXX next time.
		 * however, the value already be applied will be kept
		 */
		void ResetSettingTypes();

		void SetValue(SettingsType type, int32_t value, bool apply_to_camera = true);
		void SetWhiteBalance(PhotographyOptions_WhiteBalance wb, bool apply_to_camera = true);
		int32_t GetIntValue(SettingsType type) const;

		PhotographyOptions_WhiteBalance WhiteBalance();

	private:
		std::unordered_map<SettingsType, int32_t, EnumClassHash> int_values_;
		std::vector<SettingsType> types_;
	};

	struct RecordParams {
		VideoResolution resolution;
		int32_t bitrate{ 0 };
	};

	struct LiveStreamParam {
		bool enable_audio = true;
		bool enable_video = true;
		uint32_t audio_samplerate = 48000;
		uint32_t audio_bitrate = 128000;
		uint32_t video_bitrate = 1024 * 1024 * 10;
		VideoResolution video_resolution;
		uint32_t lrv_video_bitrate = 1024 * 1024 * 1;
		VideoResolution lrv_video_resulution;
		bool enable_gyro = true;
		bool using_lrv = true;
	};

	struct TimelapseParam {
		CameraTimelapseMode mode;
		uint32_t duration;			  // 总拍摄时长，以s 为单位
		uint32_t lapseTime;			  // 间隔时间，以ms 为单位
		uint32_t accelerate_fequency; // timelapse视频加速倍数(移动延时)
	};
}
