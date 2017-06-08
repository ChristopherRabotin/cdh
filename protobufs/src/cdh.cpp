#include "cdh.hpp"

using namespace std;

typedef bool (*collect_telemetry_function)(cdh::telemetry::TMFrame &frame);

namespace cdh {
namespace subsystems {
uint tm_seq_no = 0;
uint tc_seq_no = 0;
uint exec_now_timestamp = UINT_MAX;
bool err_tc_seq_no = false;
uint err_tc_seq_no_skipped = 0;
bool err_tc_timestamp = false;
bool err_tm_collection_pwr = false;
bool err_tm_collection_imu = false;
bool err_tm_collection_hmi = false;

cdh::telemetry::TMFrame collect_all_telemetry() {
  // To ensure that each subsystem is called in the same order and to avoid code
  // duplication, we just have a list of these TM collecting functions.
  // TODO: Move this out of the function.
  collect_telemetry_function funcs[] = {
      pwr::collect_telemetry, imu::collect_telemetry, hmi::collect_telemetry};
  bool *err_tm_collection[] = {&err_tm_collection_pwr, &err_tm_collection_imu,
                               &err_tm_collection_hmi};

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
      *err_tm_collection[i] = true;
    }
  }
  // XXX: Not sure that creating a scope for each is a great idea.
  // TM IDs 100: CDH errors
  {
    // TM ID 101 -- previous TC sequence number error
    cdh::telemetry::Telemetry *tm = frame.add_tm();
    tm->set_sys(CDH);
    tm->set_id(101);
    tm->set_bool_value(err_tc_seq_no);
    err_tc_seq_no = false; // Reset value
  }
  { // TM ID 102 -- previous TC sequences skipped
    cdh::telemetry::Telemetry *tm = frame.add_tm();
    tm->set_sys(CDH);
    tm->set_id(102);
    tm->set_int_value(err_tc_seq_no_skipped);
    err_tc_seq_no_skipped = 0; // Reset value
  }
  { // TM ID 111 -- previous TC timestamp error
    cdh::telemetry::Telemetry *tm = frame.add_tm();
    tm->set_sys(CDH);
    tm->set_id(111);
    tm->set_bool_value(err_tc_timestamp);
    err_tc_timestamp = false; // Reset value
  }
  // TM IDs 121+ are for collection errors of different subsystems.
  for (int i = 0; i < 3; ++i) {
    cdh::telemetry::Telemetry *tm = frame.add_tm();
    tm->set_sys(CDH);
    tm->set_id(121 + i);
    tm->set_bool_value(*err_tm_collection[i]);
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
    err_tc_timestamp = true;
  }
  if ((uint)frame.sequence_no() != tc_seq_no) {
    err_tc_seq_no = true;
    if (tc_seq_no == 0) {
      cout << "*warning:* CDH seems to have been reset, updating tc_seq_no "
              "with "
              "ground data"
           << endl;
    } else {
      // TODO: Buffer the missing sequences up to a set threshold in case they
      // arrive out of order and then execute them in order. This can be done
      // with a simple array.
      err_tc_seq_no_skipped = (uint)frame.sequence_no() - tc_seq_no;
      cout << "*warning:* missing " << err_tc_seq_no_skipped
           << " TC sequences, updating tc_seq_no with "
              "ground data"
           << endl;
    }
    tc_seq_no = (uint)frame.sequence_no() + 1;
  } else {
    tc_seq_no++;
  }
  // TODO: Check signature.
  // Read through each TC in the frame and execute them in the requested
  // order.
  // XXX: In Zephyr, this needs to use the scheduler in order to execute all
  // the TCs in the buffer (almost) exactly at the right time. This probably
  // means this whole chunk will be rewritten. Currently the execution time is
  // ignored and a warning is printed if the exec_time is not "NOW".
  for (int tc_frame = 0; tc_frame < frame.tc_size(); tc_frame++) {
    cdh::telecommand::Telecommand tc = frame.tc(tc_frame);
    if (tc.exec_time() != exec_now_timestamp) {
      cout << "*warning:*: exec_time currently unsupported" << endl;
    }
    // Verify the checksum.
    uint recv_crc = tc.checksum();
    // Checksum is computed with that field set to its default value.
    tc.set_checksum(0);
    uint computed_crc = compute_crc32(tc.SerializeAsString());
    if (computed_crc != recv_crc) {
      cout << "*warning:* TC_ID " << tc.id() << " for subsys "
           << subsystem_name(tc.sys())
           << " has an incorrect checksum\nrecv:" << recv_crc
           << "\ncomp: " << computed_crc << endl;
      continue;
    }
    bool success = false;
    switch (tc.sys()) {
    case CDH:
      success = process_telecommand(tc);
      break;
    case PWR:
      success = pwr::process_telecommand(tc);
      break;
    case IMU:
      success = imu::process_telecommand(tc);
      break;
    case HMI:
      success = hmi::process_telecommand(tc);
      break;
    }
    if (!success) {
      cout << "*warning:* executing TC_ID " << tc.id() << " on subsys "
           << subsystem_name(tc.sys()) << " failed" << endl;
    }
  }
};

bool process_telecommand(cdh::telecommand::Telecommand tc) { return false; }

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
