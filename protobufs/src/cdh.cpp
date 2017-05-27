#include "cdh.hpp"

using namespace std;

typedef bool (*collect_telemetry_function)(cdh::telemetry::TMFrame &frame);

namespace cdh {
namespace subsystems {

void collect_all_telemetry() {
  // TODO: Move this out of the function.
  collect_telemetry_function funcs[] = {
      imu::collect_telemetry, pwr::collect_telemetry, hmi::collect_telemetry};
  cdh::telemetry::TMFrame frame;
  for (int i = 0; i < 3; ++i) {
    cout << "func call " << i << " (" << getSubsystemFromID(i) << ") returned "
         << funcs[i](frame) << endl;
  }
};

inline Subsystem getSubsystemFromName(std::string name) {
  if (name == "PWR")
    return PWR;
  if (name == "IMU")
    return IMU;
  if (name == "HMI")
    return HMI;
};

Subsystem getSubsystemFromID(int id) {
  switch (id) {
  case 0:
    return PWR;
  case 1:
    return IMU;
  case 2:
    return HMI;
  };
};
}
}
