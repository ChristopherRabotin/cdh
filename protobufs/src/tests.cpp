/*
 *  Created by Phil on 29/11/2010.
 *  Copyright 2010 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "./proto/telemetry.pb.h"
#include "cdh.hpp"
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
        AND_THEN("the max sequence number should be set to the limit of uint");
        REQUIRE(cdh_mgmt.int_value() == UINT_MAX);
      }
    }
  }
}

SCENARIO("Test the telemetry", "[bdd][cdh][telemetry]") {
  cdh::telemetry::Telemetry TMTest;
  TMTest.set_id(0x10);
  TMTest.set_bool_value(true);
  cdh::telemetry::TMFrame Frame;
  cdh::telemetry::Telemetry *tm0 = Frame.add_tm();
  tm0 = &TMTest;
  std::cout << "num TMs:" << Frame.tm_size() << std::endl;
  std::cout << "Expected ID:" << TMTest.id() << std::endl;
  std::cout << "ID of first TM:" << Frame.tm(0).id() << std::endl;
  GIVEN("A new TM point") {
    // make stuff exist
    WHEN("I set its ID to 0x01 and its bool value to `true`") {
      // do this
      THEN("its bool value should be set to true") {
        REQUIRE(TMTest.bool_value());
      }
    }
  }
}
