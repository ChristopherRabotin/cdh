#ifndef __CDH_HPP
#define __CDH_HPP

#include "./proto/telemetry.pb.h"
#include "./proto/subsystems.pb.h"
#include <iostream>

namespace cdh {
  namespace subsystems {
    void collect_all_telemetry();
    Subsystem getSubsystemFromName(std::string name);
    Subsystem getSubsystemFromID(int id);

    namespace imu {
      bool collect_telemetry(cdh::telemetry::TMFrame &frame);
    }
    namespace pwr {
      bool collect_telemetry(cdh::telemetry::TMFrame &frame);
    }
    namespace hmi {
      bool collect_telemetry(cdh::telemetry::TMFrame &frame);
    }
  }
}
#endif
