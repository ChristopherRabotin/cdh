#ifndef PWR_HPP_INCLUDED
#define PWR_HPP_INCLUDED

#include "../proto/telemetry.pb.h"

namespace cdh {
namespace subsystems {
namespace pwr {
bool collect_telemetry(cdh::telemetry::TMFrame &frame);
}
}
};
#endif
