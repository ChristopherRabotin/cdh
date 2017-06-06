#ifndef CDH_HPP_INCLUDED
#define CDH_HPP_INCLUDED

#include "./proto/subsystems.pb.h"
#include "./proto/telecommand.pb.h"
#include "./proto/telemetry.pb.h"
#include "./subsystems/hmi.hpp"
#include "./subsystems/imu.hpp"
#include "./subsystems/pwr.hpp"
#include "crc32.h"
#include "utils.hpp"
#include <iostream>
#include <stdint.h>

namespace cdh {
namespace subsystems {
cdh::telemetry::TMFrame collect_all_telemetry();
void process_all_telecommands(cdh::telecommand::TCFrame);
bool process_telecommand(cdh::telecommand::Telecommand tc); // CDH commands
std::string subsystem_name(const Subsystem sys);
Subsystem subsystem_from_ID(const int id);
Subsystem subsystem_from_name(const std::string name);
};
}
#endif
