#ifndef CDH_HPP_INCLUDED
#define CDH_HPP_INCLUDED

#include "./proto/subsystems.pb.h"
#include "./proto/telemetry.pb.h"
#include "./subsystems/hmi.hpp"
#include "./subsystems/imu.hpp"
#include "./subsystems/pwr.hpp"
#include <iostream>

namespace cdh {
namespace subsystems {
void collect_all_telemetry();
std::string subsystemName(const Subsystem sys);
Subsystem subsystemFromID(const int id);
Subsystem subsystemFromName(const std::string name);
}
}
#endif
