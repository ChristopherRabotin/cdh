#ifndef HMI_HPP_INCLUDED
#define HMI_HPP_INCLUDED

#include "../proto/telemetry.pb.h"

namespace cdh {
namespace subsystems {
namespace hmi {
bool collect_telemetry(cdh::telemetry::TMFrame &frame);
}
}
};
#endif
