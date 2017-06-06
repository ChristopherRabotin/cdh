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
        INFO("should print a time warning");
        REQUIRE(true);
      }
      THEN("if the sequence number is off") {
        frame.set_timestamp(millis_since_epoch());
        frame.set_sequence_no(1);
        cdh::subsystems::process_all_telecommands(frame);
        INFO("should print a seq_no warning");
        REQUIRE(true);
      }
    }
  }
}
