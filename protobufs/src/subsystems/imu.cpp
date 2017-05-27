#include "imu.hpp"

namespace cdh {
namespace subsystems {
namespace imu {
bool collect_telemetry(cdh::telemetry::TMFrame &frame) {
  cdh::telemetry::Telemetry *tm0 = frame.add_tm();
  tm0->set_sys(IMU); // XXX: This is error prone because the human decides the
                     // subsytem it's applicable to. Not sure how to make this
                     // much better from the PB def.
  tm0->set_id(0x01);
  return true;
};
}
}
};
