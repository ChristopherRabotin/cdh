#ifndef HMI_HPP_INCLUDED
#define HMI_HPP_INCLUDED

#include "../proto/telemetry.pb.h"
#include "../utils.hpp"
namespace cdh {
namespace subsystems {
namespace hmi {
bool collect_telemetry(cdh::telemetry::TMFrame &frame);
inline void tm_chk(cdh::telemetry::Telemetry *tm) {
  tm->set_sys(HMI);
  tm->set_checksum(compute_crc32(tm->SerializeAsString()));
  tm->set_timestamp(millis_since_epoch());
};
}
}
};
#endif
