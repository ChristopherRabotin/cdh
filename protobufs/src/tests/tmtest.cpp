#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../cdh.hpp"
#include "../proto/telemetry.pb.h"
#include <map>
#include <stdint.h>

SCENARIO("Check that subsystems are fully defined", "[bdd][cdh][subsystems]") {
  GIVEN("The subsystem enum") {
    using namespace cdh::subsystems;
    WHEN("iterating over the enum") {
      THEN("the symmetry in numbering should be correct") {
        int subsys_id = CDH;
        do {
          INFO("checking subsys_id = " << subsys_id << " out of " << HMI);
          REQUIRE(subsystem_from_ID(subsys_id) == subsys_id);
          AND_THEN(
              "it should also have a std::string name and can be reversed");
          REQUIRE(subsystem_from_name(subsystem_name(
                      subsystem_from_ID(subsys_id))) == subsys_id);
          subsys_id++;
        } while (subsys_id < HMI);
      }
    }
  }
}

SCENARIO("Mock telemetry", "[bdd][cdh][telemetry]") {
  GIVEN("Some mock telemetry collection,") {
    WHEN("Collecting all telemetry,") {
      cdh::telemetry::TMFrame frame = cdh::subsystems::collect_all_telemetry();
      THEN("the first seq_no should be 0,") {
        REQUIRE(frame.sequence_no() == 0);
      }
      THEN("the second seq_no should be 1,") {
        REQUIRE(frame.sequence_no() == 1);
      }
      THEN("the first management telemetry point should be a valid CDH frame") {
        cdh::telemetry::Telemetry cdh_mgmt = frame.tm(0);
        REQUIRE(cdh_mgmt.sys() == cdh::subsystems::CDH);
        AND_THEN("its ID should 0x01");
        REQUIRE(cdh_mgmt.id() == 0x01);
        AND_THEN("the case of the data should be int");
        REQUIRE(cdh_mgmt.data_case() ==
                cdh::telemetry::Telemetry::DataCase::kIntValue);
        AND_THEN("the max sequence number should be set to the limit of uint");
        // NOTE: We convert this manually to a unit because the TM does not
        // support unit, only int, but we know what this should be.
        REQUIRE((uint)cdh_mgmt.int_value() == UINT_MAX);
      }
      THEN("each subsystem should have the same mock telemetry points") {
        // NOTE: Using `.at()` for the maps to throw an exception if unknown
        // key.
        using namespace cdh::subsystems;
        std::map<Subsystem, char> tm_count;
        tm_count[PWR] = 0;
        tm_count[IMU] = 0;
        tm_count[HMI] = 0;
        std::map<Subsystem, char> tm_id;
        tm_id[PWR] = 0;
        tm_id[IMU] = 0;
        tm_id[HMI] = 0;
        std::map<int, cdh::telemetry::Telemetry::DataCase> tm_data_case;
        tm_data_case[0] = cdh::telemetry::Telemetry::DataCase::kBoolValue;
        tm_data_case[10] = cdh::telemetry::Telemetry::DataCase::kDoubleValue;
        tm_data_case[20] = cdh::telemetry::Telemetry::DataCase::kIntValue;
        tm_data_case[30] = cdh::telemetry::Telemetry::DataCase::kBytesValue;
        tm_data_case[40] = cdh::telemetry::Telemetry::DataCase::DATA_NOT_SET;
        tm_data_case[50] = cdh::telemetry::Telemetry::DataCase::kBoolValue;
        for (int tm_frame = 1; tm_frame < frame.tm_size(); tm_frame++) {
          cdh::telemetry::Telemetry this_tm = frame.tm(tm_frame);
          Subsystem subsys = this_tm.sys();
          if (subsys == CDH) {
            if (this_tm.data_case() ==
                cdh::telemetry::Telemetry::DataCase::kBoolValue) {
              REQUIRE(!this_tm.bool_value());
            }
            AND_THEN("continue checking the other kind of TM");
            continue;
          } else {
            tm_count.at(subsys)++;
          }
          AND_THEN("check the increment in TM IDs");
          REQUIRE(this_tm.id() == (tm_id.at(subsys)++) * 10);
          AND_THEN("check the data case based on the mock TM ID");
          REQUIRE(this_tm.data_case() == tm_data_case.at(this_tm.id()));
          AND_THEN("check that the value is the expected one");
          switch (this_tm.id()) {
          case 0:
            REQUIRE(this_tm.bool_value() == true);
            break;
          case 10:
            REQUIRE(this_tm.double_value() == 3.141592653589793);
            break;
          case 20:
            REQUIRE(this_tm.int_value() == -5000);
            break;
          case 30:
            REQUIRE(this_tm.bytes_value() == subsystem_name(subsys));
            break;
          case 40:
            break; // Nothing to check here, there is no data.
          case 50:
            REQUIRE(this_tm.bool_value() == false);
            break;
          default:
            FAIL("unexpected TM_ID: " << this_tm.id());
          }
          AND_THEN("repeat");
          INFO("TM_Frame: " << tm_frame << " SS: " << subsys
                            << " TM_ID:" << this_tm.id());
        }
        REQUIRE(tm_count[PWR] == 6);
        REQUIRE(tm_count[PWR] == tm_count[IMU]);
        REQUIRE(tm_count[HMI] == tm_count[IMU]);
      }
    }
  }
}
