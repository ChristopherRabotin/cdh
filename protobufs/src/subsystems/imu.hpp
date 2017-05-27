#ifndef IMU_HPP_INCLUDED
#define IMU_HPP_INCLUDED

#include "../proto/telemetry.pb.h"

namespace cdh {
namespace subsystems {
namespace imu {
bool collect_telemetry(cdh::telemetry::TMFrame &frame);
}
}
};
#endif
