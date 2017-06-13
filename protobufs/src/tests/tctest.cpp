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
      THEN("trying out the IMU demo logic and sequentiality") {
        frame.set_timestamp(millis_since_epoch());
        frame.set_sequence_no(4);
        // Add some IMU TCs
        // START: Test bool values
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(1);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(1);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(1);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        // END: Test bool values -- should now be `false`
        // START: Test double values
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(12);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(12);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(14);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        // END: Test double values -- should now be `3.0879`
        // START: Test int values
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(22);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(22);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(22);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(24);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        // END: Test int values -- should now be `1`
        // START: Test uint values
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(32);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(32);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(34);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(32);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(32);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        // END: Test uint values -- should now be `2`
        // START: Test bytes values
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(42);
          tc->set_exec_time(-1);
          tc->set_bytes_value(std::string{"light"});
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        // END: Test bytes values -- should now be `light`
        cdh::subsystems::process_all_telecommands(frame);
        // Read all telemetry and check that the error TMs are set.
        cdh::telemetry::TMFrame all_tm =
            cdh::subsystems::collect_all_telemetry();
        for (int i = 0; i < all_tm.tm_size(); i++) {
          cdh::telemetry::Telemetry tm = all_tm.tm(i);
          if (tm.sys() == cdh::subsystems::Subsystem::IMU) {
            switch (tm.id()) {
            case 0:
              REQUIRE(tm.bool_value() == false);
              break;
            case 10:
              REQUIRE(tm.double_value() == 3.0879);
              break;
            case 20:
              REQUIRE(tm.int_value() == 1);
              break;
            case 30:
              REQUIRE(tm.bytes_value() == std::string{"light"});
              break;
            case 40:
              break; // Nothing to check here, there is no data.
            case 50:
              REQUIRE(tm.int_value() == 2);
              break;
            default:
              FAIL("unexpected TM_ID: " << tm.id());
            }
          } else if (tm.sys() == cdh::subsystems::Subsystem::CDH) {
            // Check that only the IMU subsystem returns a valid
            if (tm.id() == 121) {
              INFO("TM CDH 121 should be true (PWR TM collection failed)");
              REQUIRE(tm.bool_value() == true);
            } else if (tm.id() == 123) {
              INFO("TM CDH 123 should be true (HMI TM collection failed)");
              REQUIRE(tm.bool_value() == true);
            }
          }
        }
        // Test setting values for other TCs (i.e. not bytes, since already
        // tested).
        frame.set_timestamp(millis_since_epoch());
        frame.set_sequence_no(5);
        // Add some IMU TCs
        // START: Test bool values
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(3);
          tc->set_bool_value(true);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(16);
          tc->set_double_value(3.1415);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(26);
          tc->set_int_value(-502012);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        {
          cdh::telecommand::Telecommand *tc = frame.add_tc();
          tc->set_sys(cdh::subsystems::Subsystem::IMU);
          tc->set_id(36);
          tc->set_int_value(2018);
          tc->set_exec_time(-1);
          tc->set_checksum(compute_crc32(tc->SerializeAsString()));
        }
        cdh::subsystems::process_all_telecommands(frame);
        // Read all telemetry and check that the error TMs are set.
        all_tm = cdh::subsystems::collect_all_telemetry();
        for (int i = 0; i < all_tm.tm_size(); i++) {
          cdh::telemetry::Telemetry tm = all_tm.tm(i);
          if (tm.sys() == cdh::subsystems::Subsystem::IMU) {
            switch (tm.id()) {
            case 0:
              REQUIRE(tm.bool_value() == true);
              break;
            case 10:
              REQUIRE(tm.double_value() == 3.1415);
              break;
            case 20:
              REQUIRE(tm.int_value() == -502012);
              break;
            case 30:
              break;
            case 40:
              break; // Nothing to check here, there is no data.
            case 50:
              REQUIRE((uint)tm.int_value() == 2018);
              break;
            default:
              FAIL("unexpected TM_ID: " << tm.id());
            }
          }
        }
      }
    }
  }
}
