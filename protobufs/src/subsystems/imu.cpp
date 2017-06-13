#include "imu.hpp"

namespace cdh {
namespace subsystems {
namespace imu {
#ifdef TEST_MOCK_TELECOMMAND
bool some_bool_value = true;
uint some_uint_value = -1;
int some_int_value = -1;
double some_double_value = 2.5879;
std::string some_bytes = std::string{"a string"};
#endif

bool collect_telemetry(cdh::telemetry::TMFrame &frame) {
#ifdef TEST_MOCK_TELEMETRY
  for (int tmid = 0; tmid < 6; tmid++) {
    cdh::telemetry::Telemetry *tm = frame.add_tm();
    tm->set_id(tmid * 10);
    // Test the different types
    switch (tmid) {
    case 0:
      tm->set_bool_value(true);
      break;
    case 1:
      tm->set_double_value(3.141592653589793);
      break;
    case 2:
      tm->set_int_value(-5000);
      break;
    case 3:
      tm->set_bytes_value(std::string{"IMU"});
      break;
    case 5:
      tm->set_bool_value(false);
      break;
    }
    tm_chk(tm);
  }
  return true;

#elif defined(TEST_MOCK_TELECOMMAND)
  for (int tmid = 0; tmid < 6; tmid++) {
    cdh::telemetry::Telemetry *tm = frame.add_tm();
    tm->set_id(tmid * 10);
    // Test the different types
    switch (tmid) {
    case 0:
      tm->set_bool_value(some_bool_value);
      break;
    case 1:
      tm->set_double_value(some_double_value);
      break;
    case 2:
      tm->set_int_value(some_int_value);
      break;
    case 3:
      tm->set_bytes_value(some_bytes);
      break;
    case 5:
      tm->set_int_value(some_uint_value);
      break;
    }
    tm_chk(tm);
  }
  return true;

#else
  return false;
#endif
};

bool process_telecommand(cdh::telecommand::Telecommand tc) {
#ifdef TEST_MOCK_TELECOMMAND
  switch (tc.id()) {
  case 1:
    // Toggle the bool value;
    some_bool_value = !some_bool_value;
    break;
  case 3:
    // Set bool value to provided value
    if (tc.parameter_case() !=
        cdh::telecommand::Telecommand::ParameterCase::kBoolValue) {
      return false;
    }
    some_bool_value = tc.bool_value();
    break;
  case 12:
    // Increment the double by 0.5
    some_double_value += 0.5;
    break;
  case 14:
    // Decrement the double by 0.5
    some_double_value -= 0.5;
    break;
  case 16:
    // Set the double value to the provided value
    if (tc.parameter_case() !=
        cdh::telecommand::Telecommand::ParameterCase::kDoubleValue) {
      return false;
    }
    some_double_value = tc.double_value();
    break;
  case 22:
    // Increment the int value
    some_int_value++;
    break;
  case 24:
    // Decrement the int value
    some_int_value--;
    break;
  case 26:
    // Set the integer value to the provided value
    if (tc.parameter_case() !=
        cdh::telecommand::Telecommand::ParameterCase::kIntValue) {
      return false;
    }
    some_int_value = tc.int_value();
    break;
  case 32:
    // Increment the uint value
    some_uint_value++;
    break;
  case 34:
    // Decrement the uint value
    some_uint_value--;
    break;
  case 36:
    // Set the unsigned integer value to the provided value
    if (tc.parameter_case() !=
        cdh::telecommand::Telecommand::ParameterCase::kIntValue) {
      return false;
    }
    some_uint_value = (uint)tc.int_value();
    break;
  case 42:
    // Set the bytes value to the provided value
    if (tc.parameter_case() !=
        cdh::telecommand::Telecommand::ParameterCase::kBytesValue) {
      return false;
    }
    some_bytes = tc.bytes_value();
    break;
  default:
    std::cout << "*warning:* unknown TC_ID " << tc.id() << " for subsys IMU"
              << std::endl;
    return false;
  }
  return true;
#else
  return false;
#endif
}
}
}
};
