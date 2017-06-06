#ifndef PWR_HPP_INCLUDED
#define PWR_HPP_INCLUDED

#include "../proto/telecommand.pb.h"
#include "../proto/telemetry.pb.h"
#include "../utils.hpp"

namespace cdh {
namespace subsystems {
namespace pwr {
bool collect_telemetry(cdh::telemetry::TMFrame &frame);
bool process_telecommand(cdh::telecommand::Telecommand tc);
inline void tm_chk(cdh::telemetry::Telemetry *tm) {
  tm->set_sys(PWR);
  tm->set_checksum(compute_crc32(tm->SerializeAsString()));
  tm->set_timestamp(millis_since_epoch());
};
}
}
};
#endif
