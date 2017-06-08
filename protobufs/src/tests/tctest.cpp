#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../cdh.hpp"
#include "../proto/telemetry.pb.h"
#include "../utils.hpp"
#include <map>
#include <stdint.h>

/*
IDEA: check the order of execution by having multiple "increment/decrement" and
"toggle" calls before reading the telemetry. Also ensure that single frame or no
frame works.
*/

SCENARIO("Mock telecommand", "[bdd][cdh][telecommand]") {
  GIVEN("Some mock telecommand logic,") {
    WHEN("A TCFrame is valid,") {
      cdh::telecommand::TCFrame frame;
      THEN("if it is in time stamped in the future") {
        frame.set_timestamp(millis_since_epoch() + 1000);
        frame.set_sequence_no(0);
        cdh::subsystems::process_all_telecommands(frame);
        // Read all telemetry and check that the error TMs are set.
        cdh::telemetry::TMFrame all_tm =
            cdh::subsystems::collect_all_telemetry();
        for (int i = 0; i < all_tm.tm_size(); i++) {
          cdh::telemetry::Telemetry tm = all_tm.tm(i);
          if (tm.sys() == cdh::subsystems::Subsystem::CDH) {
            if (tm.id() == 111) {
              REQUIRE(tm.bool_value()); // Default if not set is `false`
              break; // Nothing else to check in this test case.
            }
          } else if (tm.sys() != cdh::subsystems::Subsystem::IMU) {
            FAIL("there should only be TM of CDH and IMU");
          }
        }
      }
      THEN("if the sequence number is off") {
        frame.set_timestamp(millis_since_epoch());
        frame.set_sequence_no(1);
        cdh::subsystems::process_all_telecommands(frame);
        // Doing another call to prevent CDH from thinking it was reset
        frame.set_sequence_no(3);
        cdh::subsystems::process_all_telecommands(frame);
        // Read all telemetry and check that the error TMs are set.
        cdh::telemetry::TMFrame all_tm =
            cdh::subsystems::collect_all_telemetry();
        for (int i = 0; i < all_tm.tm_size(); i++) {
          cdh::telemetry::Telemetry tm = all_tm.tm(i);
          if (tm.sys() == cdh::subsystems::Subsystem::CDH) {
            if (tm.id() == 101) {
              REQUIRE(tm.data_case() ==
                      cdh::telemetry::Telemetry::DataCase::kBoolValue);
              AND_THEN("now check the value");
              REQUIRE(tm.bool_value()); // Default if not set is `false`
            } else if (tm.id() == 102) {
              REQUIRE(tm.data_case() ==
                      cdh::telemetry::Telemetry::DataCase::kIntValue);
              AND_THEN("now check the value");
              REQUIRE(tm.int_value() == 1); // Default if not set is `0`
            }
          }
        }
      }
    }
  }
}
