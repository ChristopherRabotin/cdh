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
Subsystem getSubsystemFromName(std::string name);
Subsystem getSubsystemFromID(int id);
}
}
#endif
