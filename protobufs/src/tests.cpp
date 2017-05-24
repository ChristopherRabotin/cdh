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

inline bool itDoesThis(){ return true; }
inline bool itDoesThat(){ return true; }

SCENARIO( "Do that thing with the thing", "[Tags]" ) {
    GIVEN( "This stuff exists" ) {
        // make stuff exist
        WHEN( "I do this" ) {
            // do this
            THEN( "it should do this")
            {
                REQUIRE( itDoesThis() );
                AND_THEN( "do that")
                    REQUIRE( itDoesThat() );
            }
        }
    }
}

SCENARIO( "Test the telemetry", "[bdd][cdh][telemetry]" ) {
    cdh::telemetry::TelemetryData TMTest;
    TMTest.set_id(0x01);
    TMTest.set_bool_value(true);
    GIVEN( "A new TM point" ) {
        // make stuff exist
        WHEN( "I set its ID to 0x01 and its bool value to `true`" ) {
            // do this
            THEN( "its bool value should be set to true")
            {
                  REQUIRE( TMTest.bool_value() );
            }
        }
    }
}
