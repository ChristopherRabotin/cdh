#include "cdh.hpp"

using namespace std;

typedef bool (*collect_telemetry_function)(cdh::telemetry::TMFrame &frame);

namespace cdh {
namespace subsystems {
uint seq_no = 0;

cdh::telemetry::TMFrame collect_all_telemetry() {
  // TODO: Move this out of the function.
  collect_telemetry_function funcs[] = {
      pwr::collect_telemetry, imu::collect_telemetry, hmi::collect_telemetry};

  cdh::telemetry::TMFrame frame;
  // Add the management TM point(s) first
  // TM ID 01 -- Max TM frame sequence number
  cdh::telemetry::Telemetry *tm = frame.add_tm();
  tm->set_sys(CDH);
  tm->set_id(0x01);
  tm->set_int_value(UINT_MAX);
  // Get the telemetry from all the subsystems
  for (int i = 0; i < 3; ++i) {
    if (!funcs[i](frame)) {
      // TODO: A better error printing statement.
      cout << "func call " << i << " ("
           << subsystem_name(subsystem_from_ID(i + 1)) << ") returned FALSE"
           << endl;
    }
  }
  // Finalize the telemetry frame
  frame.set_sequence_no((seq_no++) % UINT_MAX);
  frame.set_timestamp(millis_since_epoch());
  return frame;
};

std::string subsystem_name(const Subsystem sys) {
  switch (sys) {
  case CDH:
    return "CDH";
  case PWR:
    return "PWR";
  case IMU:
    return "IMU";
  case HMI:
    return "HMI";
  }
};

Subsystem subsystem_from_name(const std::string name) {
  if (name == "CDH")
    return CDH;
  if (name == "PWR")
    return PWR;
  if (name == "IMU")
    return IMU;
  if (name == "HMI")
    return HMI;
};

Subsystem subsystem_from_ID(const int id) {
  switch (id) {
  case 0:
    return CDH;
  case 1:
    return PWR;
  case 2:
    return IMU;
  case 3:
    return HMI;
  };
};
}
}
