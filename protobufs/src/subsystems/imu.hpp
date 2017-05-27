#ifndef IMU_HPP_INCLUDED
#define IMU_HPP_INCLUDED

#include "../proto/telemetry.pb.h"

namespace cdh {
namespace subsystems {
namespace imu {
bool collect_telemetry(cdh::telemetry::TMFrame &frame);
inline void tm_chk(cdh::telemetry::Telemetry *tm) {
  tm->set_sys(IMU);
  tm->set_checksum(0);
};
}
}
};
#endif
