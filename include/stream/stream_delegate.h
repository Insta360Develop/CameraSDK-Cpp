//
//  stream_delegate.h
//  CameraSDK
//
//  Created by capjason on 2020/3/18.
//

#ifndef stream_delegate_h
#define stream_delegate_h
#include "stream_types.h"
#include <camera/ins_types.h>

namespace ins_camera {
	class CAMERASDK_API StreamDelegate {
	public:
		/**
		 * @brief 音频数据的回调接口
		 * @param data 音频数据
		 * @param size 音频数据大小
		 * @param timeStamp 音频数据的时间戳
		 */
		virtual void OnAudioData(const uint8_t* data, size_t size, int64_t timestamp) = 0;

		/**
		* @brief 视频数据的回调接口
		* @param data 视频数据(h264或者h265的格式)
		* @param size 视频数据大小
		* @param timeStamp 视频数据的时间戳
		* @param streamType 不需要关注
		* @param stream_index 如果双路流的，通过这个参数去区别
		*/
		virtual void OnVideoData(const uint8_t* data, size_t size, int64_t timestamp, uint8_t streamType, int stream_index = 0) = 0;

		/**
		* @brief 防抖数据的回调接口
		* @param data 防抖数据的数组 详细可以参数这个'GyroData'
		*/
		virtual void OnGyroData(const std::vector<GyroData>& data) = 0;

		/**
		* @brief 曝光数据的回调接口
		* @param data 防抖数据的数组 详细可以参数这个'ExposureData'
		*/
		virtual void OnExposureData(const ExposureData& data) = 0;
	};
}

#endif /* stream_delegate_h */
