#include "imu.hpp"

namespace cdh {
namespace subsystems {
namespace imu {
bool collect_telemetry(cdh::telemetry::TMFrame &frame) {
  cdh::telemetry::Telemetry *tm0 = frame.add_tm();
  tm0->set_id(0x01);
  tm_chk(tm0);
  return true;
};
}
}
};
