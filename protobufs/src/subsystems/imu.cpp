#include "imu.hpp"

namespace cdh {
namespace subsystems {
namespace imu {
bool collect_telemetry(cdh::telemetry::TMFrame &frame) {
#ifdef TEST_MOCK_TELEMETRY
  for (int tmid = 0; tmid < 6; tmid++) {
    cdh::telemetry::Telemetry *tm = frame.add_tm();
    tm->set_id(tmid * 10);
    // Test the different types
    switch (tmid) {
    case 0:
      tm->set_bool_value(true);
      break;
    case 1:
      tm->set_double_value(1e32);
      break;
    case 2:
      tm->set_int_value(5000);
      break;
    case 3:
      tm->set_bytes_value(std::string{"TM string test"});
      break;
    }
    tm_chk(tm);
  }
  return true;
#else
  return false;
#endif
};
}
}
};
