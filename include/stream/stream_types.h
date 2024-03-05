//
//  stream_types.h
//  CameraSDK
//
//  Created by capjason on 2020/3/18.
//

#ifndef stream_types_h
#define stream_types_h
namespace ins_camera {
struct GyroData {
    int64_t timestamp;
    double ax;
    double ay;
    double az;
    double gx;
    double gy;
    double gz;
};

struct ExposureData {
    double timestamp;
    double exposure_time;
};
}

#endif /* stream_types_h */
