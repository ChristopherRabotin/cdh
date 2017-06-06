#include "cdh.hpp"

using namespace std;

typedef bool (*collect_telemetry_function)(cdh::telemetry::TMFrame &frame);

namespace cdh {
namespace subsystems {
uint tm_seq_no = 0;
uint tc_seq_no = 0;
uint exec_now_timestamp = UINT_MAX;

cdh::telemetry::TMFrame collect_all_telemetry() {
  // To ensure that each subsystem is called in the same order and to avoid code
  // duplication, we just have a list of these TM collecting functions.
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
      // TODO: A better error printing statement -- prob via kernel log
      cout << "*warning:* func call " << i << " ("
           << subsystem_name(subsystem_from_ID(i + 1)) << ") returned FALSE"
           << endl;
    }
  }
  // Finalize the telemetry frame
  frame.set_sequence_no((tm_seq_no++) % UINT_MAX);
  frame.set_timestamp(millis_since_epoch());
  return frame;
};

void process_all_telecommands(cdh::telecommand::TCFrame frame) {
  // TODO: Zephyr scheduling
  // Check the timestamp is in sync
  if (frame.timestamp() > millis_since_epoch()) {
    cout << "*warning:* received a TC sent in the future" << endl;
  }
  if ((uint)frame.sequence_no() != tc_seq_no + 1) {
    if (tc_seq_no == 0) {
      cout
          << "*warning:* CDH seems to have been reset, updating tc_seq_no with "
             "ground data"
          << endl;
    } else {
      // TODO: Buffer the missing sequences up to a set threshold in case they
      // arrive out of order and then execute them in order. This can be done
      // with a simple array.
      cout << "*warning:* missing " << (uint)frame.sequence_no() - tc_seq_no + 1
           << " TC sequences, updating tc_seq_no with "
              "ground data"
           << endl;
    }
    tc_seq_no = (uint)frame.sequence_no();
  }
  // TODO: Check signature.
  // Read through each TC in the frame and execute them in the requested order.
  // XXX: In Zephyr, this needs to use the scheduler in order to execute all
  // the TCs in the buffer (almost) exactly at the right time. This probably
  // means this whole chunk will be rewritten. Currently the execution time is
  // ignored and a warning is printed if the exec_time is not "NOW".
  for (int tc_frame = 0; tc_frame < frame.tc_size(); tc_frame++) {
    cdh::telecommand::Telecommand this_tc = frame.tc(tc_frame);
    if (this_tc.exec_time() != exec_now_timestamp) {
      cout << "*warning:*: exec_time currently unsupported" << endl;
    }
    bool success = false;
    switch (this_tc.sys()) {
    case CDH:
      success = process_telecommand(this_tc);
      break;
    case PWR:
      success = pwr::process_telecommand(this_tc);
      break;
    case IMU:
      success = imu::process_telecommand(this_tc);
      break;
    case HMI:
      success = hmi::process_telecommand(this_tc);
      break;
    }
    if (!success) {
      cout << "*warning:* executing TC_ID " << this_tc.id() << " on subsys "
           << subsystem_name(this_tc.sys()) << " failed" << endl;
    }
  }
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
