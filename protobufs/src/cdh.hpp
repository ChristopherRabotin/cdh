#ifndef CDH_HPP_INCLUDED
#define CDH_HPP_INCLUDED

#include "./proto/subsystems.pb.h"
#include "./proto/telemetry.pb.h"
#include "./subsystems/hmi.hpp"
#include "./subsystems/imu.hpp"
#include "./subsystems/pwr.hpp"
#include "crc32.h"
#include <chrono>
#include <iostream>
#include <stdint.h>

using namespace std::chrono;

namespace cdh {
namespace subsystems {
cdh::telemetry::TMFrame collect_all_telemetry();
std::string subsystemName(const Subsystem sys);
Subsystem subsystemFromID(const int id);
Subsystem subsystemFromName(const std::string name);
};
}
#endif
