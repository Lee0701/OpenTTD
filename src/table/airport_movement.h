/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file airport_movement.h Heart of the airports and their finite state machines */

#ifndef AIRPORT_MOVEMENT_H
#define AIRPORT_MOVEMENT_H


/**
 * State machine input struct (from external file, etc.)
 * Finite sTate mAchine --> FTA
 */
struct AirportFTAbuildup {
	uint16 position; ///< The position that an airplane is at.
	byte heading;    ///< The current orders (eg. TAKEOFF, HANGAR, ENDLANDING, etc.).
	uint64 block1;   ///< The block this position is on on the airport (st->airport.flags1).
	uint64 block2;   ///< The block this position is on on the airport (st->airport.flags2).
	byte next;       ///< Next position from this position.
};

///////////////////////////////////////////////////////////////////////
/////*********Movement Positions on Airports********************///////

/**
 * Airport movement data creation macro.
 * @param x     X position.
 * @param y     Y position.
 * @param flags Movement flags.
 * @param dir   Direction.
 */
#define AMD(x, y, flags, dir) { x, y, flags, dir }

/** Dummy airport. */
static const AirportMovingData _airport_moving_data_dummy[] = {
	AMD(    0,    0, AMED_NOSPDCLAMP | AMED_SLOWTURN,     DIR_N ),
	AMD(    0,   96, AMED_NOSPDCLAMP | AMED_SLOWTURN,     DIR_N ),
	AMD(   96,   96, AMED_NOSPDCLAMP | AMED_SLOWTURN,     DIR_N ),
	AMD(   96,    0, AMED_NOSPDCLAMP | AMED_SLOWTURN,     DIR_N ),
};

/** Country Airfield (small) 4x3. */
static const AirportMovingData _airport_moving_data_country[27] = {
	// Hangar
AMD(  56,    8, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar
AMD(  56,   24, 0,                               DIR_N), // 01 Taxi to right outside depot
	// Airplane Terminals
AMD(  40,   24, AMED_EXACTPOS,                   DIR_NW), // 02 Terminal 1
AMD(  24,   24, AMED_EXACTPOS,                   DIR_NW), // 03 Terminal 2
AMD(   8,   24, AMED_EXACTPOS,                   DIR_NW), // 04 Terminal 3
	// Holding Pattern
AMD(   1,  193, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 05 Fly around waiting for a landing spot (north-east)
AMD(   1,    1, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 06 Fly around waiting for a landing spot (north-west)
AMD( 257,    1, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 07 Fly around waiting for a landing spot (south-west)
AMD( 273,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 08 Fly around waiting for a landing spot (south)
	// Airplane Landing
AMD( 248,   40, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 09 Going Down for Landing
AMD(  56,   40, AMED_HOLD | AMED_LAND,           DIR_N), // 10 Touchdown
AMD(   8,   40, AMED_HOLD | AMED_BRAKE,          DIR_N), // 11 Just landed, brake until end of runway
	// Taxi
AMD(   8,   40, 0,                               DIR_N), // 12 Taxi 1
AMD(  24,   40, 0,                               DIR_N), // 13 Taxi 2
AMD(  40,   40, 0,                               DIR_N), // 14 Taxi 3
AMD(  56,   40, 0,                               DIR_N), // 15 Taxi 4
	// Takeoff
AMD(   8,   40, 0,                               DIR_N), // 16 Taxi to start of runway (takeoff)
AMD(  56,   40, AMED_NOSPDCLAMP,                 DIR_N), // 17 Accelerate to end of runway
AMD( 104,   40, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 18 Take off
	// Helicopter
AMD(  56,   24, AMED_HELI_RAISE,                 DIR_N), // 19 Helicopter takeoff
AMD(  40,   24, AMED_HELI_RAISE,                 DIR_N), // 20 Helicopter takeoff
AMD(  24,   24, AMED_HELI_RAISE,                 DIR_N), // 21 Helicopter takeoff
AMD(   8,   24, AMED_HELI_RAISE,                 DIR_N), // 22 Helicopter takeoff
	// Helicopter Landing
AMD(  48,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 23 In position above landing spot helicopter
AMD(  56,   24, AMED_HELI_LOWER,                 DIR_N), // 24 Helicopter landing
	// Helicopter Entry
AMD(  72,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 25 Fly to landing position in air
    // Extra Holding for Plane
AMD( 260,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 26 Plane PreLanding position

};

/** Commuter Airfield (small) 5x4. */
static const AirportMovingData _airport_moving_data_commuter[39] = {
AMD(  72,    8, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar
AMD(  72,   24, 0,                               DIR_N), // 01 Taxi to right outside depot
// Airplane Terminals
AMD(  56,   40, AMED_EXACTPOS,                   DIR_SE), // 02 Terminal 1
AMD(  40,   40, AMED_EXACTPOS,                   DIR_SE), // 03 Terminal 2
AMD(  24,   40, AMED_EXACTPOS,                   DIR_SE), // 04 Terminal 3
// Cargo terminals (Share with Helipads)
AMD(  56,    8, AMED_EXACTPOS,                   DIR_NW), // 05 Cargo 1
AMD(  40,    8, AMED_EXACTPOS,                   DIR_NW), // 06 Cargo 2
// Helipads (Share with Cargo)
AMD(  56,    8, AMED_EXACTPOS,                   DIR_NE), // 07 Helipad 1
AMD(  40,    8, AMED_EXACTPOS,                   DIR_NE), // 08 Helipad 2
// Airplane Holding Pattern
AMD( 152,  216, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 09 Fly around waiting for a landing spot (north-east)
AMD(-104,  -40, AMED_NOSPDCLAMP | AMED_SLOWTURN,DIR_N), // 10 Fly around waiting for a landing spot (north-west)
AMD( 152, -296, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 11 Fly around waiting for a landing spot (south-west)
AMD( 408,  -40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 12 Fly around waiting for a landing spot (south)
// Helicopter Holding Pattern
AMD(  80,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 13 Bufferspace before helipad
AMD(  80,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 14 Bufferspace before helipad
// Helicopter in air to Hangar
AMD(  68,   28, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 15 Go to position for Hangarentrance in air  -4 first number +4 second number
AMD(  72,   24, AMED_HELI_LOWER,                 DIR_N), // 16 Land in front of hangar
// Airplane Landing
AMD( 280,   56, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 17 Fly to landing position in air
AMD(  72,   56, AMED_HOLD | AMED_LAND,           DIR_N), // 18 Going down for land
AMD(   8,   56, AMED_HOLD | AMED_BRAKE,          DIR_N), // 19 Just landed, brake until end of runway
// Main FLow
AMD(   8,   40, 0,                               DIR_SW), // 20 TAXI01 Taxiing
AMD(   8,   24, 0,                               DIR_SW), // 21 TAXI02 Taxiing
AMD(  24,   24, 0,                               DIR_SW), // 22 TAXI03 Taxiing
AMD(  40,   24, 0,                               DIR_SW), // 23 TAXI04 Taxiing
AMD(  56,   24, 0,                               DIR_SW), // 24 TAXI05 Taxiing
AMD(  72,   40, 0,                               DIR_SW), // 25 TAXI06 Taxiing
// Airplane Takeoff Runway 01
AMD(  72,   56, 0,                               DIR_NE), // 26 Accelerate to end of runway
AMD(  40,   56, AMED_NOSPDCLAMP,                 DIR_N), // 27 Release control of runway, for smoother movement
AMD(   8,   56, AMED_NOSPDCLAMP,                 DIR_N), // 28 End of runway
AMD( -40,   56, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 29 Take off
// Helicopter position for Helipads
AMD(  48,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 30 Get in position for Helipad1 -8 on first number 
AMD(  32,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 31 Get in position for Helipad2 -8 on first number
// Helicopter Land at Helipads
AMD(  56,    8, AMED_HELI_LOWER,                 DIR_N), // 32 Get in position for Helipad1
AMD(  40,    8, AMED_HELI_LOWER,                 DIR_N), // 33 Get in position for Helipad2
// Helicopter Takeoff from Helipads
AMD(  56,    8, AMED_HELI_RAISE,                 DIR_N), // 34 Takeoff Helipad1
AMD(  40,    8, AMED_HELI_RAISE,                 DIR_N), // 35 Takeoff Helipad2
// Helicopter takeoff from Hangar
AMD(  72,    8, AMED_HELI_RAISE,                 DIR_N), // 36 Takeoff from Hangar01
// Runway01 Entry
AMD( 360,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 37 Runway01 Entry
// Helicopter Entry
AMD(  80,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 38 Helicopter Entry
};

/** City Airport (large) 6x6. */
static const AirportMovingData _airport_moving_data_city[36] = {
AMD(  85,    3, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar
AMD(  85,   22, 0,                               DIR_N), // 01 Taxi to right outside depot
AMD(  26,   41, AMED_EXACTPOS,                   DIR_SW), // 02 Terminal 1
AMD(  56,   22, AMED_EXACTPOS,                   DIR_SE), // 03 Terminal 2
AMD(  38,    8, AMED_EXACTPOS,                   DIR_SW), // 04 Terminal 3
AMD(  65,    6, 0,                               DIR_N), // 05 Taxi to right in infront of terminal 2/3
AMD(  80,   27, 0,                               DIR_N), // 06 Taxiway terminals 2-3
AMD(  44,   63, 0,                               DIR_N), // 07 Taxi to Airport center
AMD(  58,   71, 0,                               DIR_N), // 08 Towards takeoff
AMD(  72,   88, 0,                               DIR_N), // 09 Taxi to runway (takeoff)
AMD(  88,   88, AMED_EXACTPOS,                   DIR_NE), // 10 Taxi to start of runway (takeoff)
AMD(   8,   88, AMED_NOSPDCLAMP,                 DIR_N), // 11 Accelerate to end of runway
AMD( -56,   88, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 12 Take off
	// Runway01 Entry
AMD( 376,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 13 Runway01 Entry
	// Runway01 Landing
AMD( 280,   88, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 14 Fly to landing position in air
AMD(  88,   88, AMED_HOLD | AMED_LAND,           DIR_N), // 15 Going down for land
AMD(  19,   88, AMED_HOLD | AMED_BRAKE,          DIR_N), // 16 Just landed, brake until end of runway
	// Taxi
AMD(  36,   71, 0,                               DIR_N), // 17 Taxi from runway
	// Holding Pattern
AMD( 168,  248, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 18 Fly around waiting for a landing spot (north-east)
AMD( -88,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 19 Fly around waiting for a landing spot (north-west)
AMD( 168, -264, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 20 Fly around waiting for a landing spot (south-west)
AMD( 424,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 21 Fly around waiting for a landing spot (south)
	// Helicopter
AMD(  88,   24, AMED_HELI_RAISE,                 DIR_N), // 22 TakeOff Hangar01
AMD(  24,   40, AMED_HELI_RAISE,                 DIR_N), // 23 TakeOff Terminal 01
AMD(  56,   24, AMED_HELI_RAISE,                 DIR_N), // 24 TakeOff Terminal 02
AMD(  40,    8, AMED_HELI_RAISE,                 DIR_N), // 25 TakeOff Terminal 03
	// Helicopter Landing
AMD(  80,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 26 Get in position above landing spot Outside Hangar
AMD(  88,   24, AMED_HELI_LOWER,                 DIR_N), // 27 Helicopter landing
AMD( 168,  248, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 28 Fly around waiting for a landing spot (north-east)
	// Keep opening screen from crashing
AMD( -88,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 29 Helicopter Entry
AMD( 168, -264, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 30 Fly around waiting for a landing spot (south-west)
AMD( 424,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 31 Fly around waiting for a landing spot (south)
AMD( 168,  248, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 32 Fly around waiting for a landing spot (north-east)
AMD( -88,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 33 Fly around waiting for a landing spot (north-west)
AMD( 168, -264, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 34 Fly around waiting for a landing spot (south-west)
AMD( 424,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 35 Fly around waiting for a landing spot (south)
};

/** Metropolitan Airport (metropolitan) - 2 runways. */
static const AirportMovingData _airport_moving_data_metropolitan[31] = {
AMD(  88,    8, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar
AMD(  88,   24, 0,                               DIR_N), // 01 Taxi to right outside depot
AMD(  24,   40, AMED_EXACTPOS,                   DIR_SW), // 02 Terminal 1
AMD(  56,   24, AMED_EXACTPOS,                   DIR_SE), // 03 Terminal 2
AMD(  40,    8, AMED_EXACTPOS,                   DIR_SW), // 04 Terminal 3
AMD(  72,    8, 0,                               DIR_N), // 05 Taxi04 to right in infront of terminal 2/3
AMD(  72,   24, 0,                               DIR_N), // 06 Taxi03 Taxiway terminals 2-3
AMD(  56,   56, 0,                               DIR_N), // 07 Taxi02 to Airport center
AMD(  72,   56, 0,                               DIR_N), // 08 Taxi05 Towards takeoff
AMD(  72,   72, 0,                               DIR_N), // 09 Taxi to runway (takeoff)
	// Takeoff Runway02
AMD(  88,   72, AMED_EXACTPOS,                   DIR_NE), // 10 Taxi to start of runway (takeoff)
AMD(   8,   72, AMED_NOSPDCLAMP,                 DIR_N), // 11 Accelerate to end of runway
AMD( -40,   72, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 12 Take off
	// Runway01 Entry
AMD( 376,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 13 Runway01 Entry
	// Landing Runway01
AMD( 280,   88, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 14 Fly to landing position in air
AMD(  88,   88, AMED_HOLD | AMED_LAND,           DIR_N), // 15 Going down for land
AMD(  24,   88, AMED_HOLD | AMED_BRAKE,          DIR_N), // 16 Just landed, brake until end of runway
	// Cross Runway02
AMD(  24,   72, 0,                               DIR_N), // 17 Cross Runway02
AMD(  24,   56, 0,                               DIR_N), // 18 Taxi01
	// Holding Pattern
AMD( 168,  248, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 19 Fly around waiting for a landing spot (north-east)
AMD( -88,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 20 Fly around waiting for a landing spot (north-west)
AMD( 168, -264, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 21 Fly around waiting for a landing spot (south-west)
AMD( 424,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 22 Fly around waiting for a landing spot (south)
	// Helicopter
AMD(  88,   24, AMED_HELI_RAISE,                 DIR_N), // 23 TakeOff Hangar01
AMD(  24,   40, AMED_HELI_RAISE,                 DIR_N), // 24 TakeOff Terminal 01
AMD(  56,   24, AMED_HELI_RAISE,                 DIR_N), // 25 TakeOff Terminal 02
AMD(  40,    8, AMED_HELI_RAISE,                 DIR_N), // 26 TakeOff Terminal 03
	// Helicopter Landing
AMD(  80,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 27 Get in position above landing spot Outside Hangar
AMD(  88,   24, AMED_HELI_LOWER,                 DIR_N), // 28 Helicopter landing
	// Helicopter Entry
AMD( 104,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 29 Helicopter Entry
	// Abort Landing Runway 01
AMD( -40,   88, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 30 Abort Landing Runway 01
};

/** International Airport (international) - 2 runways, 6 terminals, dedicated helipad. */
static const AirportMovingData _airport_moving_data_international[54] = {
	/* Hangars and Outside Hangars */
AMD(   8,   56, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar 1
AMD( 104,   24, AMED_EXACTPOS,                   DIR_SE), // 01 In Hangar 2
AMD(   8,   72, 0,                               DIR_N), // 02 Taxi to right outside Hangar1
AMD( 104,   40, 0,                               DIR_N), // 03 Taxi to right outside Hangar2
	/* Airplane Terminals */
AMD(  40,   72, AMED_EXACTPOS,                   DIR_SW), // 04 Terminal 3
AMD(  40,   56, AMED_EXACTPOS,                   DIR_SW), // 05 Terminal 2
AMD(  40,   40, AMED_EXACTPOS,                   DIR_SW), // 06 Terminal 1
AMD(  72,   72, AMED_EXACTPOS,                   DIR_NE), // 07 Terminal 6
AMD(  72,   56, AMED_EXACTPOS,                   DIR_NE), // 08 Terminal 5
AMD(  72,   40, AMED_EXACTPOS,                   DIR_NE), // 09 Terminal 4
	/* Cargo Terminals (Helipads) */
AMD( 104,   56, AMED_EXACTPOS,                   DIR_SW), // 10 Cargo Term 1
AMD( 104,   72, AMED_EXACTPOS,                   DIR_SW), // 11 Cargo Term 2
	/* Helicopter Terminals */
AMD( 104,   56, AMED_EXACTPOS,                   DIR_NE), // 12 Helipad 1
AMD( 104,   72, AMED_EXACTPOS,                   DIR_NE), // 13 Helipad 2
	/* Airplane - Holding Pattern */
AMD(  40,  392, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 14 Fly around waiting for a landing spot (north-east)
AMD(-344,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 15 Fly around waiting for a landing spot (north-west)
AMD(  40, -376, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 16 Fly around waiting for a landing spot (south-west)
AMD( 424,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 17 Fly around waiting for a landing spot (south)
	/* Runway01 Entry */
AMD( 376,   56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 18 Runway01 Entry Point
	/* Airplane Landing */
AMD( 296,  104, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 19 Fly to landing position in air
AMD(  88,  104, AMED_HOLD | AMED_LAND,           DIR_N), // 20 Going down for land
AMD(   8,  104, AMED_HOLD | AMED_BRAKE,          DIR_N), // 21 Just landed, brake until end of runway
	/* Airplane Taxi */
AMD(   8,   88, 0,                               DIR_N), // 22 Taxi from runway to crossing
AMD(  24,   88, 0,                               DIR_N), // 23 Go to Taxi Position 03  (Towards Terminals 4,5,6)
AMD(  40,   88, 0,                               DIR_N), // 24 Go to Taxi Position 04  (Towards Terminals 4,5,6)
AMD(  56,   88, 0,                               DIR_N), // 25 Go to Taxi Position 05  (Towards Terminals 4,5,6)
AMD(  72,   88, 0,                               DIR_N), // 26 Go to Taxi Position 06  (Towards Terminals 4,5,6)
AMD(  88,   88, AMED_EXACTPOS,                   DIR_NW), // 27 Go to Taxi Position 07  (Towards Terminals 4,5,6)
AMD(  88,   72, 0,                               DIR_N), // 28 In Front of Terminal 4 / Helipad 1
AMD(  88,   56, 0,                               DIR_N), // 29 In Front of Terminal 5 / Helipad 2
AMD(  88,   40, 0,                               DIR_N), // 30 In Front of Terminal 6
AMD(  88,   24, 0,                               DIR_N), // 31 Towards Takeoff Runway
AMD(  72,   24, 0,                               DIR_N), // 32 Towards Takeoff Runway
AMD(  56,   24, 0,                               DIR_N), // 33 Towards Takeoff Runway
AMD(  40,   24, 0,                               DIR_N), // 34 Towards Takeoff Runway
AMD(  24,   24, 0,                               DIR_N), // 35 Towards Takeoff Runway
AMD(  24,   72, 0,                               DIR_N), // 36 In Front of Terminal 3
AMD(  24,   56, 0,                               DIR_N), // 37 In Front of Terminal 2
AMD(  24,   40, 0,                               DIR_N), // 38 In Front of Terminal 1
AMD(  24,    8, 0,                               DIR_N), // 39 On Runway-out, prepare for takeoff
	/* Airplane Takeoff */
AMD(  40,    8, AMED_NOSPDCLAMP,                 DIR_N), // 40 Release control of runway, for smoother movement
AMD(  88,    8, AMED_NOSPDCLAMP,                 DIR_N), // 41 Airborne point - Takeoff
AMD( 152,    8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 42 End Take off    Airplane leaves Airport Control
	/* Helicopter - Holding Pattern */
AMD( 128,   80, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 43 Bufferspace before helipad
AMD( 128,   80, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 44 Bufferspace before helipad
AMD(  96,   55, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 45 Get in position for Helipad1
AMD(  96,   71, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 46 Get in position for Helipad2
	/* Helicopter Landing */
AMD(  96,   55, AMED_HELI_LOWER,                 DIR_N), // 47 Land at Helipad1
AMD(  96,   71, AMED_HELI_LOWER,                 DIR_N), // 48 Land at Helipad2
	/* Helicopter Takeoff */
AMD( 104,   55, AMED_HELI_RAISE,                 DIR_N), // 49 Takeoff Helipad1
AMD( 104,   71, AMED_HELI_RAISE,                 DIR_N), // 50 Takeoff Helipad2
	/* Helicopter Hangar */
AMD( 104,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 51 Go to position for Hangarentrance in air
AMD( 104,   32, AMED_HELI_LOWER,                 DIR_N), // 52 Land in HANGAR2_AREA to go to hangar
	// Helicopter Entry
AMD( 152,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 53 Runway01 Entry Point
};

/** Terminus1 - 2 runways, 9 terminals, 3 helipad. */
static const AirportMovingData _airport_moving_data_terminus1[63] = {
	/* Hangars and Outside Hangars */
AMD(  40,   56, AMED_EXACTPOS,                   DIR_NE), // 00 In Hangar 1
AMD( 104,   56, AMED_EXACTPOS,                   DIR_SW), // 01 In Hangar 2
AMD(  24,   56, 0,                               DIR_N), // 02 Taxi to right outside Hangar1
AMD( 120,   56, 0,                               DIR_N), // 03 Taxi to right outside Hangar2
	/* Airplane Terminals */
AMD(   8,   24, AMED_EXACTPOS,                   DIR_NW), // 04 Terminal 01
AMD(  24,   24, AMED_EXACTPOS,                   DIR_NW), // 05 Terminal 02
AMD(  40,   24, AMED_EXACTPOS,                   DIR_NW), // 06 Terminal 03
AMD(  56,   24, AMED_EXACTPOS,                   DIR_NW), // 07 Terminal 04
AMD(  72,   24, AMED_EXACTPOS,                   DIR_NW), // 08 Terminal 05
AMD(  88,   24, AMED_EXACTPOS,                   DIR_NW), // 09 Terminal 06
AMD( 104,   24, AMED_EXACTPOS,                   DIR_NW), // 10 Terminal 07
AMD( 120,   24, AMED_EXACTPOS,                   DIR_NW), // 11 Terminal 08
AMD( 136,   24, AMED_EXACTPOS,                   DIR_NW), // 12 Terminal 09
	/* Cargo Terminals (Uses Helipads) */
AMD(  56,   56, AMED_EXACTPOS,                   DIR_SE), // 13 Cargo Terminal 01
AMD(  72,   56, AMED_EXACTPOS,                   DIR_SE), // 14 Cargo Terminal 02
AMD(  88,   56, AMED_EXACTPOS,                   DIR_SE), // 15 Cargo Terminal 03
	/* Helicopter Terminals */
AMD(  56,   56, AMED_EXACTPOS,                   DIR_SE), // 16 Helipad 01
AMD(  72,   56, AMED_EXACTPOS,                   DIR_SE), // 17 Helipad 02
AMD(  88,   56, AMED_EXACTPOS,                   DIR_SE), // 18 Helipad 03
	/* Airplane - Holding Pattern */
AMD( 168,  280, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 19 Fly around waiting for a landing spot (north-east)
AMD(-216, -104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 20 Fly around waiting for a landing spot (north-west)
AMD( 168, -488, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 21 Fly around waiting for a landing spot (south-west)
AMD( 552, -104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 22 Fly around waiting for a landing spot (south-east)
	/* Airplane Landing  Runway 01*/
AMD( 312,   88, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 23 Fly to landing position in air
AMD( 104,   88, AMED_HOLD | AMED_LAND,           DIR_N), // 24 Going down for land
AMD(   8,   88, AMED_HOLD | AMED_BRAKE,          DIR_N), // 25 Just landed, brake until end of runway
AMD(   8,   88, 0,                               DIR_N), // 26 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing  Runway 02*/
AMD( 328,   72, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 27 Fly to landing position in air
AMD( 120,   72, AMED_HOLD | AMED_LAND,           DIR_N), // 28 Going down for land
AMD(  24,   72, AMED_HOLD | AMED_BRAKE,          DIR_N), // 29 Just landed, brake until end of runway
	// Airplane End Landing for Runways 01 and 02
AMD(   8,   72, 0,                               DIR_N), // 30 End landing, select Terminal
	/* Airplane Taxi */
AMD(   8,   56, 0,                               DIR_N), // 31 Taxi 02
AMD(   8,   40, 0,                               DIR_N), // 32 Taxi 03
AMD(  24,   40, 0,                               DIR_N), // 33 Taxi 04
AMD(  40,   40, 0,                               DIR_N), // 34 Taxi 05
AMD(  56,   40, 0,                               DIR_N), // 35 Taxi 06
AMD(  72,   40, 0,                               DIR_N), // 36 Taxi 07
AMD(  88,   40, 0,                               DIR_N), // 37 Taxi 08
AMD( 104,   40, 0,                               DIR_N), // 38 Taxi 09
AMD( 120,   40, 0,                               DIR_N), // 39 Taxi 10
AMD( 136,   40, 0,                               DIR_N), // 40 Taxi 11
AMD( 136,   56, 0,                               DIR_N), // 41 Taxi 12
	/* Airplane Takeoff */
AMD( 136,   72, AMED_NOSPDCLAMP,                 DIR_N), // 42 Enter Runway02
AMD( 104,   72, AMED_NOSPDCLAMP,                 DIR_N), // 43 Airborne point - Takeoff
AMD(  56,   72, AMED_NOSPDCLAMP,                 DIR_N), // 44 Go Airborne
AMD(  -8,   72, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 45 End Take off    Airplane leaves Airport Control
	/* Helicopter - Holding Pattern */
AMD(  72,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 46 Bufferspace before helipad
AMD(  72,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 47 Bufferspace before helipad
	/* Helicopter Takeoff */
AMD(  56,   56, AMED_HELI_RAISE,                 DIR_N), // 48 Takeoff Helipad1
AMD(  72,   56, AMED_HELI_RAISE,                 DIR_N), // 49 Takeoff Helipad2
AMD(  88,   56, AMED_HELI_RAISE,                 DIR_N), // 50 Takeoff Helipad3
	/* Helicopter Hangar */
AMD(  16,   56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 51 Go to position for Hangarentrance in air  -8 on first number to align it to landing area
AMD(  24,   56, AMED_HELI_LOWER,                 DIR_N), // 52 Land in HANGAR01_block to go to hangar also HELI01, HELI02, HELI03 access.
	/*  Added holding spots */
AMD( 504,  -56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 53 Fly around waiting for a landing spot SW Check Runway 2
AMD( 488,  -40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 54 Fly around waiting for a landing spot SW Check Runway 1
	/* Helicopter Entry. Helicopters need a seperate entry or it will clear flags for airplanes causing then to run into each other.*/
AMD(  72,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 55 Helicopter Entry
	// Abort Landing Runway 01
AMD( -24,   88, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 56 Abort Landing Runway 01
	// Helicopter Positioning
AMD(  56,   64, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 57 Get in position for Helipad01
AMD(  72,   64, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 58 Get in position for Helipad02
AMD(  88,   64, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 59 Get in position for Helipad03
	// Helicopter Landing at Terminals
AMD(  56,   56, AMED_HELI_LOWER,                 DIR_N), // 60 Land at Helipad01
AMD(  72,   56, AMED_HELI_LOWER,                 DIR_N), // 61 Land at Helipad02
AMD(  88,   56, AMED_HELI_LOWER,                 DIR_N), // 62 Land at Helipad03
};

/** Terminus2 Airport - 4 hangars, 4 runways, 16 terminals, 6 helipad. */
static const AirportMovingData _airport_moving_data_terminus2[120] = {
	/* Hangars and Outside Hangars */
AMD(  40,  136, AMED_EXACTPOS,                   DIR_NE), //  00 In Hangar 01
AMD( 104,  136, AMED_EXACTPOS,                   DIR_SW), //  01 In Hangar 02
AMD( 104,   40, AMED_EXACTPOS,                   DIR_SW), //  02 In Hangar 03
AMD(  40,   40, AMED_EXACTPOS,                   DIR_NE), //  03 In Hangar 04
AMD(  24,  136, 0,                               DIR_N), //  04 Taxi to right outside Hangar01
AMD( 120,  136, 0,                               DIR_N), //  05 Taxi to right outside Hangar02
AMD( 120,   40, 0,                               DIR_N), //  06 Taxi to right outside Hangar03
AMD(  24,   40, 0,                               DIR_N), //  07 Taxi to right outside Hangar04
	/* Airplane Terminals */
AMD(   8,  104, AMED_EXACTPOS,                   DIR_NW), //  08 Terminal 01
AMD(  24,  104, AMED_EXACTPOS,                   DIR_NW), //  09 Terminal 02
AMD(  40,  104, AMED_EXACTPOS,                   DIR_NW), //  10 Terminal 03
AMD(  56,  104, AMED_EXACTPOS,                   DIR_NW), //  11 Terminal 04
AMD(  88,  104, AMED_EXACTPOS,                   DIR_NW), //  12 Terminal 05
AMD( 104,  104, AMED_EXACTPOS,                   DIR_NW), //  13 Terminal 06
AMD( 120,  104, AMED_EXACTPOS,                   DIR_NW), //  14 Terminal 07
AMD( 136,  104, AMED_EXACTPOS,                   DIR_NW), //  15 Terminal 08
AMD( 136,   72, AMED_EXACTPOS,                   DIR_SE), //  16 Terminal 09
AMD( 120,   72, AMED_EXACTPOS,                   DIR_SE), //  17 Terminal 10
AMD( 104,   72, AMED_EXACTPOS,                   DIR_SE), //  18 Terminal 11
AMD(  88,   72, AMED_EXACTPOS,                   DIR_SE), //  19 Terminal 12
AMD(  56,   72, AMED_EXACTPOS,                   DIR_SE), //  20 Terminal 13
AMD(  40,   72, AMED_EXACTPOS,                   DIR_SE), //  21 Terminal 14
AMD(  24,   72, AMED_EXACTPOS,                   DIR_SE), //  22 Terminal 15
AMD(   8,   72, AMED_EXACTPOS,                   DIR_SE), //  23 Terminal 16
	/* Cargo Terminals (Uses Helipads) */
AMD(  56,  136, AMED_EXACTPOS,                   DIR_SE), //  24 Cargo Terminal 01
AMD(  72,  136, AMED_EXACTPOS,                   DIR_SE), //  25 Cargo Terminal 02
AMD(  88,  136, AMED_EXACTPOS,                   DIR_SE), //  26 Cargo Terminal 03
AMD(  88,   40, AMED_EXACTPOS,                   DIR_NW), //  27 Cargo Terminal 04
AMD(  72,   40, AMED_EXACTPOS,                   DIR_NW), //  28 Cargo Terminal 05
AMD(  56,   40, AMED_EXACTPOS,                   DIR_NW), //  29 Cargo Terminal 06
	/* Helicopter Terminals */
AMD(  56,  136, AMED_EXACTPOS,                   DIR_SE), //  30 Helipad 01
AMD(  72,  136, AMED_EXACTPOS,                   DIR_SE), //  31 Helipad 02
AMD(  88,  136, AMED_EXACTPOS,                   DIR_SE), //  32 Helipad 03
AMD(  88,   40, AMED_EXACTPOS,                   DIR_NW), //  33 Helipad 04
AMD(  72,   40, AMED_EXACTPOS,                   DIR_NW), //  34 Helipad 05
AMD(  56,   40, AMED_EXACTPOS,                   DIR_NW), //  35 Helipad 06
	/* Airplane - Holding Pattern */
AMD( -40,  568, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  36 Fly around waiting for a landing spot (north-east)
AMD(-408,  200, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  37 Fly around waiting for a landing spot (north-west)
AMD( 184, -392, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  38 Fly around waiting for a landing spot (south-west)
AMD( 552,  -24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  39 Fly around waiting for a landing spot (south)
	/* Helicopter - Holding Pattern */
AMD(  72,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  40 Bufferspace before helipad
AMD(  72,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  41 Bufferspace before helipad
	/* Helicopter Hangar */
AMD(  20,  136, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  42 Go to position for Hangarentrance in air
AMD(  24,  136, AMED_HELI_LOWER,                 DIR_N), //  43 Land in HANG01_block to go to hangar or HELI01, HELI02, HELI03
	// Start of Pod 1
	/* Airplane Landing - Runway 01 */
AMD( 312,  168, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  44 Fly to landing position in air
AMD( 104,  168, AMED_HOLD | AMED_LAND,           DIR_N), //  45 Going down for land
AMD(   8,  168, AMED_HOLD | AMED_BRAKE,          DIR_N), //  46 Just landed, brake until end of runway
AMD(   8,  168, AMED_SLOWTURN,                   DIR_N), //  47 Just landed, Unused
	/* Airplane Landing - Runway 02 */
AMD( 328,  152, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  48 Fly to landing position in air
AMD( 120,  152, AMED_HOLD | AMED_LAND,           DIR_N), //  49 Going down for land
AMD(  24,  152, AMED_HOLD | AMED_BRAKE,          DIR_N), //  50 Just landed, brake until end of runway
	/* Airplane Taxi */
AMD(   8,  152, 0,                               DIR_N), //  51 Taxi02
AMD(   8,  136, 0,                               DIR_N), //  52 Taxi03
AMD(   8,  120, 0,                               DIR_N), //  53 Taxi04
AMD(  24,  120, 0,                               DIR_N), //  54 Taxi05
AMD(  40,  120, 0,                               DIR_N), //  55 Taxi06
AMD(  56,  120, 0,                               DIR_N), //  56 Taxi07
AMD(  72,  120, 0,                               DIR_N), //  57 Taxi08
AMD(  88,  120, 0,                               DIR_N), //  58 Taxi09
AMD( 104,  120, 0,                               DIR_N), //  59 Taxi10
AMD( 120,  120, 0,                               DIR_N), //  60 Taxi11
AMD( 136,  120, 0,                               DIR_N), //  61 Taxi12
AMD( 136,  136, 0,                               DIR_N), //  62 Taxi13
	/* Airplane Takeoff */
AMD( 136,  152, 0,                               DIR_N), //  63 On Runway 02, prepare for takeoff
AMD( 104,  152, AMED_NOSPDCLAMP,                 DIR_N), //  64 Release control of runway, for smoother movement
AMD(  56,  152, AMED_NOSPDCLAMP,                 DIR_N), //  65 Airborne point - Takeoff
AMD(  -8,  152, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), //  66 End Take off    Airplane leaves Airport Control
	/* Helicopter Takeoff */
AMD(  56,  136, AMED_HELI_RAISE,                 DIR_N), //  67 Takeoff Helipad1
AMD(  72,  136, AMED_HELI_RAISE,                 DIR_N), //  68 Takeoff Helipad2
AMD(  88,  136, AMED_HELI_RAISE,                 DIR_N), //  69 Takeoff Helipad3
	// End of Pod1
	// Start of Pod 2
	/* Airplane Landing - Runway 03 */
AMD(-168,    8, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  70 Fly to landing position in air
AMD(  40,    8, AMED_HOLD | AMED_LAND,           DIR_N), //  71 Going down for land
AMD( 136,    8, AMED_HOLD | AMED_BRAKE,          DIR_N), //  72 Just landed, brake until end of runway
AMD( 136,    8, AMED_SLOWTURN,                   DIR_N), //  73 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing - Runway 04 */
AMD(-184,   24, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  74 Fly to landing position in air
AMD(  24,   24, AMED_HOLD | AMED_LAND,           DIR_N), //  75 Going down for land
AMD( 120,   24, AMED_HOLD | AMED_BRAKE,          DIR_N), //  76 Just landed, brake until end of runway
	/* Airplane Taxi */
AMD( 136,   24, 0,                               DIR_N), //  77 Taxi15
AMD( 136,   40, 0,                               DIR_N), //  78 Taxi16
AMD( 136,   56, 0,                               DIR_N), //  79 Taxi17
AMD( 120,   56, 0,                               DIR_N), //  80 Taxi18
AMD( 104,   56, 0,                               DIR_N), //  81 Taxi19
AMD(  88,   56, 0,                               DIR_N), //  82 Taxi20
AMD(  72,   56, 0,                               DIR_N), //  83 Taxi21
AMD(  56,   56, 0,                               DIR_N), //  84 Taxi22
AMD(  40,   56, 0,                               DIR_N), //  85 Taxi23
AMD(  24,   56, 0,                               DIR_N), //  86 Taxi24
AMD(   8,   56, 0,                               DIR_N), //  87 Taxi25
AMD(   8,   40, 0,                               DIR_N), //  88 Taxi26
	/* Airplane Takeoff */
AMD(   8,   24, 0,                               DIR_N), //  89 On Runway 02, prepare for takeoff
AMD(  40,   24, AMED_NOSPDCLAMP,                 DIR_N), //  90 Release control of runway, for smoother movement
AMD(  88,   24, AMED_NOSPDCLAMP,                 DIR_N), //  91 Airborne point - Takeoff
AMD( 152,   24, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), //  92 End Take off    Airplane leaves Airport Control
	/* Helicopter Hangar */
AMD( 120,   36, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  93 Go to position for Hangarentrance in air
AMD( 120,   40, AMED_HELI_LOWER,                 DIR_N), //  94 Land in HANG03_block to go to hangar or HELI04, HELI05, HELI06
	/* Helicopter Takeoff */
AMD(  88,   40, AMED_HELI_RAISE,                 DIR_N), //  95 Takeoff Helipad 04
AMD(  72,   40, AMED_HELI_RAISE,                 DIR_N), //  96 Takeoff Helipad 05
AMD(  56,   40, AMED_HELI_RAISE,                 DIR_N), //  97 Takeoff Helipad 06
	// End of Pod2
	/* Crossover Taxi from Pod 1 to Pod 2 */
AMD(  72,  104, 0,                               DIR_N), //  98 Crossover 
AMD(  72,   88, 0,                               DIR_N), //  99 Crossover
AMD(  72,   72, 0,                               DIR_N), // 100 Crossover
	/*  Added holding spots */
AMD( 504,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 101 Fly around waiting for a landing spot SW Check Runway 2
AMD( 488,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 102 Fly around waiting for a landing spot SW Check Runway 1
AMD(-360,  152, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 103 Fly around waiting for a landing spot SE Check Runway 4
AMD(-344,  136, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 104 Fly around waiting for a landing spot SE Check Runway 3
	/* Helicopter Entry. Helicopters need a seperate entry or it will clear flags for airplanes causing then to run into each other.*/
AMD(  88,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 105 Helicopter Entry
	// Abort Landings
AMD( -24,  168, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 106 Abort Landing Runway 01
AMD( 152,    8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 107 Abort Landing Runway 03
	// Helicopter Positioning
AMD(  56,  144, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 108 Get in position for Helipad01
AMD(  72,  144, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 109 Get in position for Helipad02
AMD(  88,  144, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 110 Get in position for Helipad03
AMD(  88,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 111 Get in position for Helipad04
AMD(  72,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 112 Get in position for Helipad05
AMD(  56,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 113 Get in position for Helipad06
	// Helicopter Landing at Terminals
AMD(  56,  136, AMED_HELI_LOWER,                 DIR_N), // 114 Land at Helipad01
AMD(  72,  136, AMED_HELI_LOWER,                 DIR_N), // 115 Land at Helipad02
AMD(  88,  136, AMED_HELI_LOWER,                 DIR_N), // 116 Land at Helipad03
AMD(  88,   40, AMED_HELI_LOWER,                 DIR_N), // 117 Land at Helipad04
AMD(  72,   40, AMED_HELI_LOWER,                 DIR_N), // 118 Land at Helipad05
AMD(  56,   40, AMED_HELI_LOWER,                 DIR_N), // 119 Land at Helipad06
};

/** Terminus3 Airport - 6 hangars, 6 runways, 25 terminals, 9 helipad. */
static const AirportMovingData _airport_moving_data_terminus3[184] = {
	/* Hangars and Outside Hangars */
AMD( 136,  136, AMED_EXACTPOS,                   DIR_NE), //  00 In Hangar 01
AMD( 200,  136, AMED_EXACTPOS,                   DIR_SW), //  01 In Hangar 02
AMD( 200,   40, AMED_EXACTPOS,                   DIR_SW), //  02 In Hangar 03
AMD( 136,   40, AMED_EXACTPOS,                   DIR_NE), //  03 In Hangar 04
AMD(  40,   56, AMED_EXACTPOS,                   DIR_NW), //  04 In Hangar 05
AMD(  40,  120, AMED_EXACTPOS,                   DIR_SE), //  05 In Hangar 06
AMD( 120,  136, 0,                               DIR_N), //  06 Taxi to right outside Hangar01
AMD( 216,  136, 0,                               DIR_N), //  07 Taxi to right outside Hangar02
AMD( 216,   40, 0,                               DIR_N), //  08 Taxi to right outside Hangar03
AMD( 120,   40, 0,                               DIR_N), //  09 Taxi to right outside Hangar04
AMD(  40,   40, 0,                               DIR_N), //  10 Taxi to right outside Hangar05
AMD(  40,  136, 0,                               DIR_N), //  11 Taxi to right outside Hangar06
	/* Airplane Terminals */
AMD( 104,  104, AMED_EXACTPOS,                   DIR_NW), //  12 Terminal 01
AMD( 120,  104, AMED_EXACTPOS,                   DIR_NW), //  13 Terminal 02
AMD( 136,  104, AMED_EXACTPOS,                   DIR_NW), //  14 Terminal 03
AMD( 152,  104, AMED_EXACTPOS,                   DIR_NW), //  15 Terminal 04
AMD( 184,  104, AMED_EXACTPOS,                   DIR_NW), //  16 Terminal 05
AMD( 200,  104, AMED_EXACTPOS,                   DIR_NW), //  17 Terminal 06
AMD( 216,  104, AMED_EXACTPOS,                   DIR_NW), //  18 Terminal 07
AMD( 232,  104, AMED_EXACTPOS,                   DIR_NW), //  19 Terminal 08
AMD( 232,   72, AMED_EXACTPOS,                   DIR_SE), //  20 Terminal 09
AMD( 216,   72, AMED_EXACTPOS,                   DIR_SE), //  21 Terminal 10
AMD( 200,   72, AMED_EXACTPOS,                   DIR_SE), //  22 Terminal 11
AMD( 184,   72, AMED_EXACTPOS,                   DIR_SE), //  23 Terminal 12
AMD( 152,   72, AMED_EXACTPOS,                   DIR_SE), //  24 Terminal 13
AMD( 136,   72, AMED_EXACTPOS,                   DIR_SE), //  25 Terminal 14
AMD( 120,   72, AMED_EXACTPOS,                   DIR_SE), //  26 Terminal 15
AMD( 104,   72, AMED_EXACTPOS,                   DIR_SE), //  27 Terminal 16
AMD(  72,    8, AMED_EXACTPOS,                   DIR_SW), //  28 Terminal 17
AMD(  72,   24, AMED_EXACTPOS,                   DIR_SW), //  29 Terminal 18
AMD(  72,   40, AMED_EXACTPOS,                   DIR_SW), //  30 Terminal 19
AMD(  72,   72, AMED_EXACTPOS,                   DIR_SW), //  31 Terminal 20
AMD(  72,   88, AMED_EXACTPOS,                   DIR_SW), //  32 Terminal 21
AMD(  72,  104, AMED_EXACTPOS,                   DIR_SW), //  33 Terminal 22
AMD(  72,  136, AMED_EXACTPOS,                   DIR_SW), //  34 Terminal 23
AMD(  72,  152, AMED_EXACTPOS,                   DIR_SW), //  35 Terminal 24
AMD(  72,  168, AMED_EXACTPOS,                   DIR_SW), //  36 Terminal 25
	/* Cargo Terminals (Uses Helipads) */
AMD( 152,  136, AMED_EXACTPOS,                   DIR_SE), //  37 Cargo Terminal 01
AMD( 168,  136, AMED_EXACTPOS,                   DIR_SE), //  38 Cargo Terminal 02
AMD( 184,  136, AMED_EXACTPOS,                   DIR_SE), //  39 Cargo Terminal 03
AMD( 184,   40, AMED_EXACTPOS,                   DIR_NW), //  40 Cargo Terminal 04
AMD( 168,   40, AMED_EXACTPOS,                   DIR_NW), //  41 Cargo Terminal 05
AMD( 152,   40, AMED_EXACTPOS,                   DIR_NW), //  42 Cargo Terminal 06
AMD(  40,   72, AMED_EXACTPOS,                   DIR_NE), //  43 Cargo Terminal 07
AMD(  40,   88, AMED_EXACTPOS,                   DIR_NE), //  44 Cargo Terminal 08
AMD(  40,  104, AMED_EXACTPOS,                   DIR_NE), //  45 Cargo Terminal 09
	/* Helicopter Terminals */
AMD( 152,  136, AMED_EXACTPOS,                   DIR_SE), //  46 Helipad 01
AMD( 168,  136, AMED_EXACTPOS,                   DIR_SE), //  47 Helipad 02
AMD( 184,  136, AMED_EXACTPOS,                   DIR_SE), //  48 Helipad 03
AMD( 184,   40, AMED_EXACTPOS,                   DIR_NW), //  49 Helipad 04
AMD( 168,   40, AMED_EXACTPOS,                   DIR_NW), //  50 Helipad 05
AMD( 152,   40, AMED_EXACTPOS,                   DIR_NW), //  51 Helipad 06
AMD(  40,   72, AMED_EXACTPOS,                   DIR_NE), //  52 Helipad 07
AMD(  40,   88, AMED_EXACTPOS,                   DIR_NE), //  53 Helipad 08
AMD(  40,  104, AMED_EXACTPOS,                   DIR_NE), //  54 Helipad 09
	/* Airplane - Holding Pattern */
	//AMD(  200,  712, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), //  55 Fly around waiting for a landing spot (south-east)
	//AMD( -312,  200, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), //  56 Fly around waiting for a landing spot (north-east)
	//AMD(  424, -536, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), //  57 Fly around waiting for a landing spot (north-west)
	//AMD(  936,  -24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), //  58 Fly around waiting for a landing spot (south-west)
AMD( 456,  520, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  55 Fly around waiting for a landing spot (south-east)
AMD(-264,  456, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  56 Fly around waiting for a landing spot (north-east)
AMD( 200, -280, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  57 Fly around waiting for a landing spot (north-west)
AMD( 600, -280, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  58 Fly around waiting for a landing spot (south-west)
	/* Helicopter - Holding Pattern */
AMD( 120,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  59 Bufferspace before helipad
AMD( 120,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  60 Bufferspace before helipad
	/* Helicopter Hangar 01 */
AMD( 120,  144, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  61 Go to position for Hangarentrance in air
AMD( 120,  136, AMED_HELI_LOWER,                 DIR_N), //  62 Land in HANG01_block to go to hangar
	// Start of Pod 1
	/* Airplane Landing - Runway 01 */
AMD( 408,  168, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  63 Fly to landing position in air
AMD( 200,  168, AMED_HOLD | AMED_LAND,           DIR_N), //  64 Going down for land
AMD( 104,  168, AMED_HOLD | AMED_BRAKE,          DIR_N), //  65 Just landed, brake until end of runway
AMD( 104,  168, AMED_SLOWTURN,                   DIR_N), //  66 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing - Runway 02 - Unused */
AMD( 424,  152, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  67 Fly to landing position in air
AMD( 216,  152, AMED_HOLD | AMED_LAND,           DIR_N), //  68 Going down for land
AMD( 120,  152, AMED_HOLD | AMED_BRAKE,          DIR_N), //  69 Just landed, brake until end of runway
	/* Airplane Taxi */
AMD( 104,  152, 0,                               DIR_N), //  70 Taxi02
AMD( 104,  136, 0,                               DIR_N), //  71 Taxi03
AMD( 104,  120, 0,                               DIR_N), //  72 Taxi04
AMD( 120,  120, 0,                               DIR_N), //  73 Taxi05
AMD( 136,  120, 0,                               DIR_N), //  74 Taxi06
AMD( 152,  120, 0,                               DIR_N), //  75 Taxi07
AMD( 168,  120, 0,                               DIR_N), //  76 Taxi08
AMD( 184,  120, 0,                               DIR_N), //  77 Taxi09
AMD( 200,  120, 0,                               DIR_N), //  78 Taxi10
AMD( 216,  120, 0,                               DIR_N), //  79 Taxi11
AMD( 232,  120, 0,                               DIR_N), //  80 Taxi12
AMD( 232,  136, 0,                               DIR_N), //  81 Taxi13
	/* Airplane Takeoff */
AMD( 232,  152, 0,                               DIR_N), //  82 On Runway 02, prepare for takeoff
AMD( 200,  152, AMED_NOSPDCLAMP,                 DIR_N), //  83 Release control of runway, for smoother movement
AMD( 152,  152, AMED_NOSPDCLAMP,                 DIR_N), //  84 Airborne point - Takeoff
AMD(  88,  152, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), //  85 End Take off    Airplane leaves Airport Control
	/* Helicopter Takeoff */
AMD( 152,  136, AMED_HELI_RAISE,                 DIR_N), //  86 Takeoff Helipad1
AMD( 168,  136, AMED_HELI_RAISE,                 DIR_N), //  87 Takeoff Helipad2
AMD( 184,  136, AMED_HELI_RAISE,                 DIR_N), //  88 Takeoff Helipad2
	// End of Pod1
	// Start of Pod 2
	/* Airplane Landing - Runway 03 */
AMD( -72,    8, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  89 Fly to landing position in air
AMD( 136,    8, AMED_HOLD | AMED_LAND,           DIR_N), //  90 Going down for land
AMD( 232,    8, AMED_HOLD | AMED_BRAKE,          DIR_N), //  91 Just landed, brake until end of runway
AMD( 232,    8, AMED_SLOWTURN,                   DIR_N), //  92 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing - Runway 04 */
AMD( -72,   24, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  93 Fly to landing position in air
AMD( 136,   24, AMED_HOLD | AMED_LAND,           DIR_N), //  94 Going down for land
AMD( 216,   24, AMED_HOLD | AMED_BRAKE,          DIR_N), //  95 Just landed, brake until end of runway
	/* Airplane Taxi */
AMD( 232,   24, 0,                               DIR_N), //  96 Taxi15
AMD( 232,   40, 0,                               DIR_N), //  97 Taxi16
AMD( 232,   56, 0,                               DIR_N), //  98 Taxi17
AMD( 216,   56, 0,                               DIR_N), //  99 Taxi18
AMD( 200,   56, 0,                               DIR_N), // 100 Taxi19
AMD( 184,   56, 0,                               DIR_N), // 101 Taxi20
AMD( 168,   56, 0,                               DIR_N), // 102 Taxi21
AMD( 152,   56, 0,                               DIR_N), // 103 Taxi22
AMD( 136,   56, 0,                               DIR_N), // 104 Taxi23
AMD( 120,   56, 0,                               DIR_N), // 105 Taxi24
AMD( 104,   56, 0,                               DIR_N), // 106 Taxi25
AMD( 104,   40, 0,                               DIR_N), // 107 Taxi26
	/* Airplane Takeoff */
AMD( 104,   24, 0,                               DIR_N), // 108 On Runway 02, prepare for takeoff
AMD( 136,   24, AMED_NOSPDCLAMP,                 DIR_N), // 109 Release control of runway, for smoother movement
AMD( 184,   24, AMED_NOSPDCLAMP,                 DIR_N), // 110 Airborne point - Takeoff
AMD( 248,   24, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 111 End Take off    Airplane leaves Airport Control
	/* Helicopter Hangar 03 */
AMD( 224,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 112 Go to position for Hangarentrance in air
AMD( 216,   40, AMED_HELI_LOWER,                 DIR_N), // 113 Land in HANG03_block to go to hangar
	/* Helicopter Takeoff */
AMD( 152,   40, AMED_HELI_RAISE,                 DIR_N), // 114 Takeoff Helipad1
AMD( 168,   40, AMED_HELI_RAISE,                 DIR_N), // 115 Takeoff Helipad2
AMD( 184,   40, AMED_HELI_RAISE,                 DIR_N), // 116 Takeoff Helipad2
	// End of Pod2
	// Start of Pod 3
	/* Airplane Landing - Runway 05 */
AMD(   8,  328, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 117 Fly to landing position in air
AMD(   8,  120, AMED_HOLD | AMED_LAND,           DIR_N), // 118 Going down for land
AMD(   8,   24, AMED_HOLD | AMED_BRAKE,          DIR_N), // 119 Just landed, brake until end of runway
AMD(   8,   24, AMED_SLOWTURN,                   DIR_N), // 120 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing - Runway 06 */
AMD(  24,  344, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 121 Fly to landing position in air
AMD(  24,  136, AMED_HOLD | AMED_LAND,           DIR_N), // 122 Going down for land
AMD(  24,   40, AMED_HOLD | AMED_BRAKE,          DIR_N), // 123 Just landed, brake until end of runway
	/* Airplane Taxi */
AMD(  24,   24, 0,                               DIR_N), // 124 Taxi29
AMD(  40,   24, 0,                               DIR_N), // 125 Taxi30
AMD(  40,    8, 0,                               DIR_N), // 126 Taxi31
AMD(  56,    8, 0,                               DIR_N), // 127 Taxi32
AMD(  56,   24, 0,                               DIR_N), // 128 Taxi33
AMD(  56,   40, 0,                               DIR_N), // 129 Taxi34
AMD(  56,   56, 0,                               DIR_N), // 130 Taxi35
AMD(  56,   72, 0,                               DIR_N), // 131 Taxi36
AMD(  56,   88, 0,                               DIR_N), // 132 Taxi37
AMD(  56,  104, 0,                               DIR_N), // 133 Taxi38
AMD(  56,  120, 0,                               DIR_N), // 134 Taxi39
AMD(  56,  136, 0,                               DIR_N), // 135 Taxi40
AMD(  56,  152, 0,                               DIR_N), // 136 Taxi41
AMD(  56,  168, 0,                               DIR_N), // 137 Taxi42
AMD(  40,  168, 0,                               DIR_N), // 138 Taxi43
AMD(  40,  152, 0,                               DIR_N), // 139 Taxi44
	/* Airplane Takeoff */
AMD(  24,  152, 0,                               DIR_N), // 140 On Runway 06, prepare for takeoff
AMD(  24,  120, AMED_NOSPDCLAMP,                 DIR_N), // 141 Release control of runway, for smoother movement
AMD(  24,   72, AMED_NOSPDCLAMP,                 DIR_N), // 142 Airborne point - Takeoff
AMD(  24,    8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 143 End Take off    Airplane leaves Airport Control
	/* Helicopter Hangar 05 */
AMD(  32,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 112 Go to position for Hangarentrance in air
AMD(  40,   40, AMED_HELI_LOWER,                 DIR_N), // 113 Land in HANG05_block to go to hangar
	/* Helicopter Takeoff */
AMD(  40,   72, AMED_HELI_RAISE,                 DIR_N), // 146 Takeoff Helipad1
AMD(  40,   88, AMED_HELI_RAISE,                 DIR_N), // 147 Takeoff Helipad2
AMD(  40,  104, AMED_HELI_RAISE,                 DIR_N), // 148 Takeoff Helipad2
	// End of Pod3
	/* Crossover Taxi from Pod 1 to Pod 2 */
AMD( 168,  104, 0,                               DIR_N), // 149 Crossover 
AMD( 168,   88, 0,                               DIR_N), // 150 Crossover
AMD( 168,   72, 0,                               DIR_N), // 151 Crossover
	/* Crossover Taxi from Pod 2 to Pod 3 */
AMD(  88,   56, 0,                               DIR_N), // 152 Crossover 
AMD(  72,   56, 0,                               DIR_N), // 153 Crossover
	/* Crossover Taxi from Pod 1 to Pod 3 */
AMD(  88,  120, 0,                               DIR_N), // 154 Crossover 
AMD(  72,  120, 0,                               DIR_N), // 155 Crossover
	/*  Added holding spots */
	//AMD(  888,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 156 Fly around waiting for a landing spot SW Check Runway 2
	//AMD(  872,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 157 Fly around waiting for a landing spot SW Check Runway 1
AMD( 600,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 156 Fly around waiting for a landing spot SW Check Runway 2
AMD( 584,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 157 Fly around waiting for a landing spot SW Check Runway 1
AMD(-264,  152, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 158 Fly around waiting for a landing spot SE Check Runway 4
AMD(-248,  136, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 159 Fly around waiting for a landing spot SE Check Runway 3
	//AMD(  152,  664, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 160 Fly around waiting for a landing spot NE Check Runway 6
	//AMD(  136,  648, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 161 Fly around waiting for a landing spot NE Check Runway 5
AMD( 152,  520, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 160 Fly around waiting for a landing spot NE Check Runway 6
AMD( 136,  504, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 161 Fly around waiting for a landing spot NE Check Runway 5
	/* Helicopter Entry. Helicopters need a seperate entry or it will clear flags for airplanes causing then to run into each other.*/
AMD( 104,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 162 Helicopter Entry
	// Abort Landings
AMD(  72,  168, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 163 Abort Landing Runway 01
AMD( 280,    8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 164 Abort Landing Runway 03
AMD(   8,   -8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 165 Abort Landing Runway 05
	// Helicopter Positioning
AMD( 160,  136, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 166 Get in position for Helipad01
AMD( 176,  136, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 167 Get in position for Helipad02
AMD( 192,  136, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 168 Get in position for Helipad03
AMD( 184,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 169 Get in position for Helipad04
AMD( 168,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 170 Get in position for Helipad05
AMD( 152,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 171 Get in position for Helipad06
AMD(  32,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 172 Get in position for Helipad07
AMD(  32,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 173 Get in position for Helipad08
AMD(  32,  104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 174 Get in position for Helipad09
	// Helicopter Landing at Terminals
AMD( 152,  136, AMED_HELI_LOWER,                 DIR_N), // 175 Land at Helipad01
AMD( 168,  136, AMED_HELI_LOWER,                 DIR_N), // 176 Land at Helipad02
AMD( 184,  136, AMED_HELI_LOWER,                 DIR_N), // 177 Land at Helipad03
AMD( 184,   40, AMED_HELI_LOWER,                 DIR_N), // 178 Land at Helipad04
AMD( 168,   40, AMED_HELI_LOWER,                 DIR_N), // 179 Land at Helipad05
AMD( 152,   40, AMED_HELI_LOWER,                 DIR_N), // 180 Land at Helipad06
AMD(  40,   72, AMED_HELI_LOWER,                 DIR_N), // 181 Land at Helipad07
AMD(  40,   88, AMED_HELI_LOWER,                 DIR_N), // 182 Land at Helipad08
AMD(  40,  104, AMED_HELI_LOWER,                 DIR_N), // 183 Land at Helipad09
};

/** Terminus4 Airport - 8 hangars, 8 runways, 36 terminals, 12 helipad. */
static const AirportMovingData _airport_moving_data_terminus4[239] = {
	/* Hangars and Outside Hangars */
AMD( 136,  200, AMED_EXACTPOS,                   DIR_NE), //  00 In Hangar 01
AMD( 200,  200, AMED_EXACTPOS,                   DIR_SW), //  01 In Hangar 02
AMD(  40,  136, AMED_EXACTPOS,                   DIR_NW), //  02 In Hangar 03
AMD(  40,  200, AMED_EXACTPOS,                   DIR_SE), //  03 In Hangar 04
AMD( 104,   40, AMED_EXACTPOS,                   DIR_SW), //  04 In Hangar 05
AMD(  40,   40, AMED_EXACTPOS,                   DIR_NE), //  05 In Hangar 06
AMD( 200,  104, AMED_EXACTPOS,                   DIR_SE), //  06 In Hangar 07
AMD( 200,   40, AMED_EXACTPOS,                   DIR_NW), //  07 In Hangar 08
AMD( 120,  200, 0,                               DIR_N), //  08 Taxi to right outside Hangar01
AMD( 216,  200, 0,                               DIR_N), //  09 Taxi to right outside Hangar02
AMD(  40,  120, 0,                               DIR_N), //  10 Taxi to right outside Hangar03
AMD(  40,  216, 0,                               DIR_N), //  11 Taxi to right outside Hangar04
AMD( 120,   40, 0,                               DIR_N), //  12 Taxi to right outside Hangar05
AMD(  24,   40, 0,                               DIR_N), //  13 Taxi to right outside Hangar06
AMD( 200,  120, 0,                               DIR_N), //  14 Taxi to right outside Hangar07
AMD( 200,   24, 0,                               DIR_N), //  15 Taxi to right outside Hangar08
	/* Airplane Terminals */
AMD(  88,  168, AMED_EXACTPOS,                   DIR_NW), //  16 Terminal 01
AMD( 104,  168, AMED_EXACTPOS,                   DIR_NW), //  17 Terminal 02
AMD( 120,  168, AMED_EXACTPOS,                   DIR_NW), //  18 Terminal 03
AMD( 136,  168, AMED_EXACTPOS,                   DIR_NW), //  19 Terminal 04
AMD( 152,  168, AMED_EXACTPOS,                   DIR_NW), //  20 Terminal 05
AMD( 168,  168, AMED_EXACTPOS,                   DIR_N), //  21 Terminal 06
AMD( 200,  168, AMED_EXACTPOS,                   DIR_NW), //  22 Terminal 07
AMD( 216,  168, AMED_EXACTPOS,                   DIR_NW), //  23 Terminal 08
AMD( 232,  168, AMED_EXACTPOS,                   DIR_NW), //  24 Terminal 09
AMD(  72,   88, AMED_EXACTPOS,                   DIR_SW), //  25 Terminal 10
AMD(  72,  104, AMED_EXACTPOS,                   DIR_SW), //  26 Terminal 11
AMD(  72,  120, AMED_EXACTPOS,                   DIR_SW), //  27 Terminal 12
AMD(  72,  136, AMED_EXACTPOS,                   DIR_SW), //  28 Terminal 13
AMD(  72,  152, AMED_EXACTPOS,                   DIR_SW), //  29 Terminal 14
AMD(  72,  168, AMED_EXACTPOS,                   DIR_W), //  30 Terminal 15
AMD(  72,  200, AMED_EXACTPOS,                   DIR_SW), //  31 Terminal 16
AMD(  72,  216, AMED_EXACTPOS,                   DIR_SW), //  32 Terminal 17
AMD(  72,  232, AMED_EXACTPOS,                   DIR_SW), //  33 Terminal 18
AMD( 152,   72, AMED_EXACTPOS,                   DIR_SE), //  34 Terminal 19
AMD( 136,   72, AMED_EXACTPOS,                   DIR_SE), //  35 Terminal 20
AMD( 120,   72, AMED_EXACTPOS,                   DIR_SE), //  36 Terminal 21
AMD( 104,   72, AMED_EXACTPOS,                   DIR_SE), //  37 Terminal 22
AMD(  88,   72, AMED_EXACTPOS,                   DIR_SE), //  38 Terminal 23
AMD(  72,   72, AMED_EXACTPOS,                   DIR_S), //  39 Terminal 24
AMD(  40,   72, AMED_EXACTPOS,                   DIR_SE), //  40 Terminal 25
AMD(  24,   72, AMED_EXACTPOS,                   DIR_SE), //  41 Terminal 26
AMD(   8,   72, AMED_EXACTPOS,                   DIR_SE), //  42 Terminal 27
AMD( 168,  152, AMED_EXACTPOS,                   DIR_NE), //  43 Terminal 28
AMD( 168,  136, AMED_EXACTPOS,                   DIR_NE), //  44 Terminal 29
AMD( 168,  120, AMED_EXACTPOS,                   DIR_NE), //  45 Terminal 30
AMD( 168,  104, AMED_EXACTPOS,                   DIR_NE), //  46 Terminal 31
AMD( 168,   88, AMED_EXACTPOS,                   DIR_NE), //  47 Terminal 32
AMD( 168,   72, AMED_EXACTPOS,                   DIR_E), //  48 Terminal 33
AMD( 168,   40, AMED_EXACTPOS,                   DIR_NE), //  49 Terminal 34
AMD( 168,   24, AMED_EXACTPOS,                   DIR_NE), //  50 Terminal 35
AMD( 168,    8, AMED_EXACTPOS,                   DIR_NE), //  51 Terminal 36
	/* Cargo Terminals (Uses Helipads) */
AMD( 152,  200, AMED_EXACTPOS,                   DIR_SE), //  52 Cargo Terminal 01
AMD( 168,  200, AMED_EXACTPOS,                   DIR_SE), //  53 Cargo Terminal 02
AMD( 184,  200, AMED_EXACTPOS,                   DIR_SE), //  54 Cargo Terminal 03
AMD(  40,  152, AMED_EXACTPOS,                   DIR_NE), //  55 Cargo Terminal 04
AMD(  40,  168, AMED_EXACTPOS,                   DIR_NE), //  56 Cargo Terminal 05
AMD(  40,  184, AMED_EXACTPOS,                   DIR_NE), //  57 Cargo Terminal 06
AMD(  88,   40, AMED_EXACTPOS,                   DIR_NW), //  58 Cargo Terminal 07
AMD(  72,   40, AMED_EXACTPOS,                   DIR_NW), //  59 Cargo Terminal 08
AMD(  56,   40, AMED_EXACTPOS,                   DIR_NW), //  60 Cargo Terminal 09
AMD( 200,   88, AMED_EXACTPOS,                   DIR_SW), //  61 Cargo Terminal 10
AMD( 200,   72, AMED_EXACTPOS,                   DIR_SW), //  62 Cargo Terminal 11
AMD( 200,   56, AMED_EXACTPOS,                   DIR_SW), //  63 Cargo Terminal 12
	/* Helicopter Terminals */
AMD( 152,  200, AMED_EXACTPOS,                   DIR_SE), //  64 Helipad 01
AMD( 168,  200, AMED_EXACTPOS,                   DIR_SE), //  65 Helipad 02
AMD( 184,  200, AMED_EXACTPOS,                   DIR_SE), //  66 Helipad 03
AMD(  40,  152, AMED_EXACTPOS,                   DIR_NE), //  67 Helipad 04
AMD(  40,  168, AMED_EXACTPOS,                   DIR_NE), //  68 Helipad 05
AMD(  40,  184, AMED_EXACTPOS,                   DIR_NE), //  69 Helipad 06
AMD(  88,   40, AMED_EXACTPOS,                   DIR_NW), //  70 Helipad 07
AMD(  72,   40, AMED_EXACTPOS,                   DIR_NW), //  71 Helipad 08
AMD(  56,   40, AMED_EXACTPOS,                   DIR_NW), //  72 Helipad 09
AMD( 200,   88, AMED_EXACTPOS,                   DIR_SW), //  73 Helipad 10
AMD( 200,   72, AMED_EXACTPOS,                   DIR_SW), //  74 Helipad 11
AMD( 200,   56, AMED_EXACTPOS,                   DIR_SW), //  75 Helipad 12
	/* Airplane - Holding Pattern */
AMD( 120,  648, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  76 Fly around waiting for a landing spot (south-east) 56,  504
AMD(-408,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  77 Fly around waiting for a landing spot (north-east) -280,   56
AMD( 120, -408, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  78 Fly around waiting for a landing spot (north-west) 152, -280
AMD( 648,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  79 Fly around waiting for a landing spot (south-west) 504, 152
	/* Helicopter - Holding Pattern */
AMD( 120,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  80 Bufferspace before helipad
AMD( 120,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  81 Bufferspace before helipad
	/* Helicopter Hangar 01 */
AMD( 120,  208, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  82 Go to position for Hangarentrance in air
AMD( 120,  200, AMED_HELI_LOWER,                 DIR_N), //  83 Land in HANG01_block to go to hangar
	// Start of Pod 1
	/* Airplane Landing - Runway 01 */
AMD( 408,  232, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  84 Fly to landing position in air
AMD( 200,  232, AMED_HOLD | AMED_LAND,           DIR_N), //  85 Going down for land
AMD( 104,  232, AMED_HOLD | AMED_BRAKE,          DIR_N), //  86 Just landed, brake until end of runway
AMD( 104,  232, AMED_SLOWTURN,                   DIR_N), //  87 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing - Runway 02 */
AMD( 424,  216, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  88 Fly to landing position in air
AMD( 216,  216, AMED_HOLD | AMED_LAND,           DIR_N), //  89 Going down for land
AMD( 120,  216, AMED_HOLD | AMED_BRAKE,          DIR_N), //  90 Just landed, brake until end of runway
	/* Airplane End Landing */
AMD( 104,  216, 0,                               DIR_N), //  91 Taxi02
	/* Airplane Taxi */
AMD( 104,  200, 0,                               DIR_N), //  92 Taxi03
AMD( 104,  184, 0,                               DIR_N), //  93 Taxi04
AMD( 120,  184, 0,                               DIR_N), //  94 Taxi05
AMD( 136,  184, 0,                               DIR_N), //  95 Taxi06
AMD( 152,  184, 0,                               DIR_N), //  96 Taxi07
AMD( 168,  184, 0,                               DIR_N), //  97 Taxi08
AMD( 184,  184, 0,                               DIR_N), //  98 Taxi09
AMD( 200,  184, 0,                               DIR_N), //  99 Taxi10
AMD( 216,  184, 0,                               DIR_N), // 100 Taxi11
AMD( 232,  184, 0,                               DIR_N), // 101 Taxi12
AMD( 232,  200, 0,                               DIR_N), // 102 Taxi13
	/* Airplane Takeoff */
AMD( 232,  216, 0,                               DIR_N), // 103 On Runway 02, prepare for takeoff
AMD( 200,  216, AMED_NOSPDCLAMP,                 DIR_N), // 104 Release control of runway, for smoother movement
AMD( 152,  216, AMED_NOSPDCLAMP,                 DIR_N), // 105 Airborne point - Takeoff
AMD(  88,  216, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 106 End Take off    Airplane leaves Airport Control
	/* Helicopter Takeoff */
AMD( 152,  200, AMED_HELI_RAISE,                 DIR_N), // 107 Takeoff Helipad1
AMD( 168,  200, AMED_HELI_RAISE,                 DIR_N), // 108 Takeoff Helipad2
AMD( 184,  200, AMED_HELI_RAISE,                 DIR_N), // 109 Takeoff Helipad2
	// End of Pod1
	// Start of Pod 2
	/* Airplane Landing - Runway 03 */
AMD(   8,  408, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 110 Fly to landing position in air
AMD(   8,  200, AMED_HOLD | AMED_LAND,           DIR_N), // 111 Going down for land
AMD(   8,  104, AMED_HOLD | AMED_BRAKE,          DIR_N), // 112 Just landed, brake until end of runway
AMD(   8,  104, AMED_SLOWTURN,                   DIR_N), // 113 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing - Runway 04 */
AMD(  24,  424, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 114 Fly to landing position in air
AMD(  24,  216, AMED_HOLD | AMED_LAND,           DIR_N), // 115 Going down for land
AMD(  24,  120, AMED_HOLD | AMED_BRAKE,          DIR_N), // 116 Just landed, brake until end of runway
	/* Airplane End Landing */
AMD(  24,  104, 0,                               DIR_N), // 117 Taxi15
	/* Airplane Taxi */
AMD(  40,  104, 0,                               DIR_N), // 118 Taxi16
AMD(  56,  104, 0,                               DIR_N), // 119 Taxi17
AMD(  56,  120, 0,                               DIR_N), // 120 Taxi18
AMD(  56,  136, 0,                               DIR_N), // 121 Taxi19
AMD(  56,  152, 0,                               DIR_N), // 122 Taxi20
AMD(  56,  168, 0,                               DIR_N), // 123 Taxi21
AMD(  56,  184, 0,                               DIR_N), // 124 Taxi22
AMD(  56,  200, 0,                               DIR_N), // 125 Taxi23
AMD(  56,  216, 0,                               DIR_N), // 126 Taxi24
AMD(  56,  232, 0,                               DIR_N), // 127 Taxi25
AMD(  40,  232, 0,                               DIR_N), // 128 Taxi26
	/* Airplane Takeoff */
AMD(  24,  232, 0,                               DIR_N), // 129 On Runway 02, prepare for takeoff
AMD(  24,  200, AMED_NOSPDCLAMP,                 DIR_N), // 130 Release control of runway, for smoother movement
AMD(  24,  152, AMED_NOSPDCLAMP,                 DIR_N), // 131 Airborne point - Takeoff
AMD(  24,   88, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 132 End Take off    Airplane leaves Airport Control
	/* Helicopter Hangar 03 */
AMD(  32,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 133 Go to position for Hangarentrance in air
AMD(  40,  120, AMED_HELI_LOWER,                 DIR_N), // 134 Land in HANG03_block to go to hangar
	/* Helicopter Takeoff */
AMD(  40,  152, AMED_HELI_RAISE,                 DIR_N), // 135 Takeoff Helipad1
AMD(  40,  168, AMED_HELI_RAISE,                 DIR_N), // 136 Takeoff Helipad2
AMD(  40,  184, AMED_HELI_RAISE,                 DIR_N), // 137 Takeoff Helipad2
	// End of Pod2
	// Start of Pod 3
	/* Airplane Landing - Runway 05 */
AMD(-168,    8, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 138 Fly to landing position in air
AMD(  40,    8, AMED_HOLD | AMED_LAND,           DIR_N), // 139 Going down for land
AMD( 136,    8, AMED_HOLD | AMED_BRAKE,          DIR_N), // 140 Just landed, brake until end of runway
AMD( 136,    8, AMED_SLOWTURN,                   DIR_N), // 141 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing - Runway 06 */
AMD(-184,   24, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 142 Fly to landing position in air
AMD(  24,   24, AMED_HOLD | AMED_LAND,           DIR_N), // 143 Going down for land
AMD( 120,   24, AMED_HOLD | AMED_BRAKE,          DIR_N), // 144 Just landed, brake until end of runway
	/* Airplane Taxi - End Landing */
AMD( 136,   24, 0,                               DIR_N), // 145 Taxi28
	/* Airplane Taxi */
AMD( 136,   40, 0,                               DIR_N), // 146 Taxi29
AMD( 136,   56, 0,                               DIR_N), // 147 Taxi30
AMD( 120,   56, 0,                               DIR_N), // 148 Taxi31
AMD( 104,   56, 0,                               DIR_N), // 149 Taxi32
AMD(  88,   56, 0,                               DIR_N), // 150 Taxi33
AMD(  72,   56, 0,                               DIR_N), // 151 Taxi34
AMD(  56,   56, 0,                               DIR_N), // 152 Taxi35
AMD(  40,   56, 0,                               DIR_N), // 153 Taxi36
AMD(  24,   56, 0,                               DIR_N), // 154 Taxi37
AMD(   8,   56, 0,                               DIR_N), // 155 Taxi38
AMD(   8,   40, 0,                               DIR_N), // 156 Taxi39
	/* Airplane Takeoff */
AMD(   8,   24, 0,                               DIR_N), // 157 On Runway 06, prepare for takeoff
AMD(  40,   24, AMED_NOSPDCLAMP,                 DIR_N), // 158 Release control of runway, for smoother movement
AMD(  88,   24, AMED_NOSPDCLAMP,                 DIR_N), // 159 Airborne point - Takeoff
AMD( 152,   24, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 160 End Take off    Airplane leaves Airport Control
	/* Helicopter Hangar 05 */
AMD( 120,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 161 Go to position for Hangarentrance in air
AMD( 120,   40, AMED_HELI_LOWER,                 DIR_N), // 162 Land in HANG05_block to go to hangar
	/* Helicopter Takeoff */
AMD(  88,   40, AMED_HELI_RAISE,                 DIR_N), // 163 Takeoff Helipad07
AMD(  72,   40, AMED_HELI_RAISE,                 DIR_N), // 164 Takeoff Helipad08
AMD(  56,   40, AMED_HELI_RAISE,                 DIR_N), // 165 Takeoff Helipad09
	// End of Pod3
	// Start of Pod 4
	/* Airplane Landing - Runway 07 */
AMD( 232, -168, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 166 Fly to landing position in air
AMD( 232,   40, AMED_HOLD | AMED_LAND,           DIR_N), // 167 Going down for land
AMD( 232,  136, AMED_HOLD | AMED_BRAKE,          DIR_N), // 168 Just landed, brake until end of runway
AMD( 232,  136, AMED_SLOWTURN,                   DIR_N), // 169 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing - Runway 04 */
AMD( 216, -184, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 170 Fly to landing position in air
AMD( 216,   24, AMED_HOLD | AMED_LAND,           DIR_N), // 171 Going down for land
AMD( 216,  120, AMED_HOLD | AMED_BRAKE,          DIR_N), // 172 Just landed, brake until end of runway
	/* Airplane End Landing */
AMD( 216,  136, 0,                               DIR_N), // 173 Taxi15
	/* Airplane Taxi */
AMD( 200,  136, 0,                               DIR_N), // 174 Taxi16
AMD( 184,  136, 0,                               DIR_N), // 175 Taxi17
AMD( 184,  120, 0,                               DIR_N), // 176 Taxi18
AMD( 184,  104, 0,                               DIR_N), // 177 Taxi19
AMD( 184,   88, 0,                               DIR_N), // 178 Taxi20
AMD( 184,   72, 0,                               DIR_N), // 179 Taxi21
AMD( 184,   56, 0,                               DIR_N), // 180 Taxi22
AMD( 184,   40, 0,                               DIR_N), // 181 Taxi23
AMD( 184,   24, 0,                               DIR_N), // 182 Taxi24
AMD( 184,    8, 0,                               DIR_N), // 183 Taxi25
AMD( 200,    8, 0,                               DIR_N), // 184 Taxi26
	/* Airplane Takeoff */
AMD( 216,    8, 0,                               DIR_N), // 185 On Runway 02, prepare for takeoff
AMD( 216,   40, AMED_NOSPDCLAMP,                 DIR_N), // 186 Release control of runway, for smoother movement
AMD( 216,   88, AMED_NOSPDCLAMP,                 DIR_N), // 187 Airborne point - Takeoff
AMD( 216,  152, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 188 End Take off    Airplane leaves Airport Control
	/* Helicopter Hangar 07 */
AMD( 208,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 189 Go to position for Hangarentrance in air
AMD( 200,  120, AMED_HELI_LOWER,                 DIR_N), // 190 Land in HANG07_block to go to hangar
	/* Helicopter Takeoff */
AMD( 200,   88, AMED_HELI_RAISE,                 DIR_N), // 191 Takeoff Helipad1
AMD( 200,   72, AMED_HELI_RAISE,                 DIR_N), // 192 Takeoff Helipad2
AMD( 200,   56, AMED_HELI_RAISE,                 DIR_N), // 193 Takeoff Helipad2
	// End of Pod4
	/* Crossover Taxi from Pod 1 to Pod 2 */
AMD(  88,  184, 0,                               DIR_N), // 194 Crossover 
AMD(  72,  184, 0,                               DIR_N), // 195 Crossover
	/* Crossover Taxi from Pod 2 to Pod 3 */
AMD(  56,   88, 0,                               DIR_N), // 196 Crossover 
AMD(  56,   72, 0,                               DIR_N), // 197 Crossover
	/* Crossover Taxi from Pod 3 to Pod 4 */
AMD( 152,   56, 0,                               DIR_N), // 198 Crossover 
AMD( 168,   56, 0,                               DIR_N), // 199 Crossover
	/* Crossover Taxi from Pod 4 to Pod 1 */
AMD( 184,  152, 0,                               DIR_N), // 200 Crossover 
AMD( 184,  168, 0,                               DIR_N), // 201 Crossover
	/*  Added holding spots */
AMD( 600,  168, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 202 Fly around waiting for a landing spot SW Check Runway 2
AMD( 584,  184, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 203 Fly around waiting for a landing spot SW Check Runway 1
AMD(  72,  600, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 204 Fly around waiting for a landing spot SE Check Runway 4
AMD(  56,  584, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 205 Fly around waiting for a landing spot SE Check Runway 3
AMD(-360,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 206 Fly around waiting for a landing spot NE Check Runway 6
AMD(-344,   56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 207 Fly around waiting for a landing spot NE Check Runway 5
AMD( 168, -360, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 208 Fly around waiting for a landing spot NW Check Runway 8
AMD( 184, -344, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 209 Fly around waiting for a landing spot NW Check Runway 7
	/* Helicopter Entry. Helicopters need a seperate entry or it will clear flags for airplanes causing then to run into each other.*/
AMD( 136,  136, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 210 Helicopter Entry
	// Abort Landings
AMD(  72,  232, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 211 Abort Landing Runway 01
AMD(   8,   72, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 212 Abort Landing Runway 03
AMD( 168,    8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 213 Abort Landing Runway 05
AMD( 232,  168, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 214 Abort Landing Runway 07
	// Helicopter Positioning
AMD( 152,  208, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 215 Get in position for Helipad01
AMD( 168,  208, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 216 Get in position for Helipad02
AMD( 184,  208, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 217 Get in position for Helipad03
AMD(  32,  152, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 218 Get in position for Helipad04
AMD(  32,  168, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 219 Get in position for Helipad05
AMD(  32,  184, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 220 Get in position for Helipad06
AMD(  88,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 221 Get in position for Helipad07
AMD(  72,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 222 Get in position for Helipad08
AMD(  56,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 223 Get in position for Helipad09
AMD( 208,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 224 Get in position for Helipad10
AMD( 208,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 225 Get in position for Helipad11
AMD( 208,   56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 226 Get in position for Helipad12
	// Helicopter Landing at Terminals
AMD( 152,  200, AMED_HELI_LOWER,                 DIR_N), // 227 Land at Helipad01
AMD( 168,  200, AMED_HELI_LOWER,                 DIR_N), // 228 Land at Helipad02
AMD( 184,  200, AMED_HELI_LOWER,                 DIR_N), // 229 Land at Helipad03
AMD(  40,  152, AMED_HELI_LOWER,                 DIR_N), // 230 Land at Helipad04
AMD(  40,  168, AMED_HELI_LOWER,                 DIR_N), // 231 Land at Helipad05
AMD(  40,  184, AMED_HELI_LOWER,                 DIR_N), // 232 Land at Helipad06
AMD(  88,   40, AMED_HELI_LOWER,                 DIR_N), // 233 Land at Helipad07
AMD(  72,   40, AMED_HELI_LOWER,                 DIR_N), // 234 Land at Helipad08
AMD(  56,   40, AMED_HELI_LOWER,                 DIR_N), // 235 Land at Helipad09
AMD( 200,   88, AMED_HELI_LOWER,                 DIR_N), // 236 Land at Helipad10
AMD( 200,   72, AMED_HELI_LOWER,                 DIR_N), // 237 Land at Helipad11
AMD( 200,   56, AMED_HELI_LOWER,                 DIR_N), // 238 Land at Helipad12
};

/** Intercontinental Airport - 4 runways, 8 terminals, 2 dedicated helipads. */
static const AirportMovingData _airport_moving_data_intercontinental[96] = {
	//Hangar and area outside hangar
AMD(   8,   88, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar 1
AMD( 136,   72, AMED_EXACTPOS,                   DIR_SE), // 01 In Hangar 2
AMD(   8,  104, 0,                               DIR_N), // 02 Taxi to right outside depot 1
AMD( 136,   88, 0,                               DIR_N), // 03 Taxi to right outside depot 2
	// Airplane Terminals
AMD(  56,  120, AMED_EXACTPOS,                   DIR_SW), // 04 Terminal 1
AMD(  56,  104, AMED_EXACTPOS,                   DIR_SW), // 05 Terminal 2
AMD(  56,   88, AMED_EXACTPOS,                   DIR_SW), // 06 Terminal 3
AMD(  56,   72, AMED_EXACTPOS,                   DIR_SW), // 07 Terminal 4
AMD(  88,  120, AMED_EXACTPOS,                   DIR_NE), // 08 Terminal 5
AMD(  88,  104, AMED_EXACTPOS,                   DIR_NE), // 09 Terminal 6
AMD(  88,   88, AMED_EXACTPOS,                   DIR_NE), // 10 Terminal 7
AMD(  88,   72, AMED_EXACTPOS,                   DIR_NE), // 11 Terminal 8
	// Cargo Terminals
AMD(  88,   56, AMED_EXACTPOS,                   DIR_SE), // 12 Cargo Terminal 01
AMD(  72,   56, AMED_EXACTPOS,                   DIR_SE), // 13 Cargo Terminal 02
	// Helicopter Terminals
AMD(  88,   56, AMED_EXACTPOS,                   DIR_SE), // 14 Helipad 1
AMD(  72,   56, AMED_EXACTPOS,                   DIR_NE), // 15 Helipad 2
	// Airplane Holding Pattern
AMD( -40,  568, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 16 Fly around waiting for a landing spot (south-east)
AMD(-408,  200, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 17 Fly around waiting for a landing spot (north-east)
AMD( 184, -392, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 18 Fly around waiting for a landing spot (north-west)
AMD( 552,  -24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 19 Fly around waiting for a landing spot (south-west)
	// Runway01 Landing
AMD( 312,  168, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 20 Fly to landing position in air1
AMD( 104,  168, AMED_HOLD | AMED_LAND,           DIR_N), // 21 Going down for land
AMD(   8,  168, AMED_HOLD | AMED_BRAKE,          DIR_N), // 22 Just landed, brake until end of runway
	// Runway02 Landing
AMD( 328,  152, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 23 Fly to landing position in air1
AMD( 120,  152, AMED_HOLD | AMED_LAND,           DIR_N), // 24 Going down for land
AMD(  24,  152, AMED_HOLD | AMED_BRAKE,          DIR_N), // 25 Just landed, brake until end of runway
	// Runway03 - Landing
AMD(-168,    8, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 26 Fly to landing position for runway03
AMD(  40,    8, AMED_HOLD | AMED_LAND,           DIR_N), // 27 Going down for landing
AMD( 136,    8, AMED_HOLD | AMED_BRAKE,          DIR_N), // 28 Just landed, brake until end of runway 03
	// Runway1 to Main Loop
AMD(   8,  152, 0,                               DIR_N), // 29 Taxi 02
AMD(   8,  136, 0,                               DIR_N), // 30 Taxi 03
AMD(   8,  120, 0,                               DIR_N), // 31 Taxi 04
AMD(  24,  104, 0,                               DIR_N), // 32 Taxi 05
	//Main Loop
AMD(  40,  104, 0,                               DIR_N), // 33 Taxi 06  TERM02
AMD(  40,   88, 0,                               DIR_N), // 34 Taxi 07  TERM03
AMD(  40,   72, 0,                               DIR_N), // 35 Taxi 08  TERM04
AMD(  40,   56, 0,                               DIR_N), // 36 Taxi 09
AMD(  40,   40, 0,                               DIR_N), // 37 Taxi 10
AMD(  56,   40, 0,                               DIR_N), // 38 Taxi 11
AMD(  72,   40, 0,                               DIR_N), // 39 Taxi 12
AMD(  88,   40, 0,                               DIR_N), // 40 Taxi 13
AMD( 104,   40, 0,                               DIR_N), // 41 Taxi 14
AMD( 104,   56, 0,                               DIR_N), // 42 Taxi 15
AMD( 104,   72, 0,                               DIR_N), // 43 Taxi 16 TERM08
AMD( 104,   88, 0,                               DIR_N), // 44 Taxi 17 TERM07
AMD( 104,  104, 0,                               DIR_N), // 45 Taxi 18 TERM06
AMD( 104,  120, 0,                               DIR_N), // 46 Taxi 19 TERM05
AMD( 104,  136, 0,                               DIR_N), // 47 Taxi 20
AMD(  88,  136, 0,                               DIR_N), // 48 Taxi 21
AMD(  72,  136, 0,                               DIR_N), // 49 Taxi 22
AMD(  56,  136, 0,                               DIR_N), // 50 Taxi 23
AMD(  40,  136, 0,                               DIR_N), // 51 Taxi 24
AMD(  40,  120, 0,                               DIR_N), // 52 Taxi 25 TERM01
	//Enter Hangar 01
AMD(  24,  120, 0,                               DIR_N), // 53 Taxi 26
	// Runway 03 to Main Loop
AMD( 136,   24, 0,                               DIR_N), // 54 Taxi 28
AMD( 136,   40, 0,                               DIR_N), // 55 Taxi 29
AMD( 136,   56, 0,                               DIR_N), // 56 Taxi 30
AMD( 120,   56, 0,                               DIR_N), // 57 Taxi 31
	// Exit Hangar 02
AMD( 120,   88, 0,                               DIR_N), // 58 Taxi 32
	// To Runway02 and Hangar 02
AMD( 120,  104, 0,                               DIR_N), // 59 Taxi 33
AMD( 136,  104, 0,                               DIR_N), // 60 Taxi 34
AMD( 136,  120, 0,                               DIR_N), // 61 Taxi 35
AMD( 136,  136, 0,                               DIR_N), // 62 Taxi 36
	// To Runway 04
AMD(  24,   72, 0,                               DIR_N), // 63 Taxi 37
AMD(   8,   56, 0,                               DIR_N), // 64 Taxi 38
AMD(   8,   40, 0,                               DIR_N), // 65 Taxi 39
	//Takeoff Runway02
AMD( 136,  152, AMED_EXACTPOS,                   DIR_NE), // 66 Accelerate to end of runway2
AMD( 104,  152, AMED_NOSPDCLAMP,                 DIR_N), // 67 Release control of runway2, for smoother movement
AMD(  56,  152, AMED_NOSPDCLAMP,                 DIR_N), // 68 Airborne
AMD(  -8,  152, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 69 Take off2
	// Runway04 - Takeoff
AMD(   8,   24, AMED_EXACTPOS,                   DIR_SW), // 70 Accelerate to end of runway
AMD(  40,   24, AMED_NOSPDCLAMP,                 DIR_N), // 71 Release control of runway, for smoother movement
AMD(  88,   24, AMED_NOSPDCLAMP,                 DIR_N), // 72 Airborne
AMD( 152,   24, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 73 End Airport Control
	// Helicopter Holding Pattern
AMD(  96,   40, AMED_NOSPDCLAMP,                 DIR_N), // 74 Bufferspace before helipad
AMD(  96,   40, AMED_NOSPDCLAMP,                 DIR_N), // 75 Bufferspace before helipad
	// Helicopter Positioning
AMD(  88,   64, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 76 Get in position for Helipad1
AMD(  64,   56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 77 Get in position for Helipad2
	// Helicopter Landing
AMD(  88,   56, AMED_HELI_LOWER,                 DIR_N), // 78 Land at Helipad1
AMD(  72,   56, AMED_HELI_LOWER,                 DIR_N), // 79 Land at Helipad2
	// Helicopter Takeoff
AMD(  88,   56, AMED_HELI_RAISE,                 DIR_N), // 80 Takeoff Helipad1
AMD(  72,   56, AMED_HELI_RAISE,                 DIR_N), // 81 Takeoff Helipad2
AMD(   8,  104, AMED_HELI_RAISE,                 DIR_N), // 82 Helitakeoff outside hangar 1
AMD( 136,   88, AMED_HELI_RAISE,                 DIR_N), // 83 Helitakeoff outside hangar 2
	// Helicopter to Hangar
AMD(   8,  112, AMED_NOSPDCLAMP | AMED_SLOWTURN | AMED_EXACTPOS, DIR_NW), // 84 Go to position for Hangarentrance in air
AMD(   8,  104, AMED_HELI_LOWER,                 DIR_NW), // 85 Land in front of hangar2
	// Runway04 - Landing
AMD(-184,   24, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 86 Fly to landing position for runway03
AMD(  24,   24, AMED_HOLD | AMED_LAND,           DIR_N), // 87 Going down for landing
AMD( 120,   24, AMED_HOLD | AMED_BRAKE,          DIR_N), // 88 Just landed, brake until end of runway 03
	/*  Added holding spots */
AMD( 488,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 89 Fly around waiting for a landing spot SW Check Runway 1
AMD( 504,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 90 Fly around waiting for a landing spot SW Check Runway 2
AMD(-344,  136, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 91 Fly around waiting for a landing spot SW Check Runway 3
AMD(-360,  152, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 92 Fly around waiting for a landing spot SW Check Runway 4
	/* Helicopter Entry. Helicopters need a seperate entry or it will clear flags for airplanes causing then to run into each other.*/
AMD(  96,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 93 Helicopter Entry
	// Abort Landings
AMD( -24,  168, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 94 Abort Landing Runway 01
AMD( 168,    8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 95 Abort Landing Runway 03
};

/** San Francisco Airport - 2 Hangars, 4 Runways, 9 Terminals, 2 Helipads, 4 Cargo Terminals. */
static const AirportMovingData _airport_moving_data_sanfran[81] = {
	/* Hangars and Outside Hangars */
AMD(  24,   24, AMED_EXACTPOS,                   DIR_SW), // 00 In Hangar 1
AMD( 152,   24, AMED_EXACTPOS,                   DIR_SE), // 01 In Hangar 2
AMD(  40,   24, 0,                               DIR_N), // 02 Taxi to right outside Hangar1
AMD( 152,   40, 0,                               DIR_N), // 03 Taxi to right outside Hangar2
	/* Airplane Terminals */
AMD( 104,    8, AMED_EXACTPOS,                   DIR_SW), // 04 Terminal 1
AMD( 104,   24, AMED_EXACTPOS,                   DIR_SW), // 05 Terminal 2
AMD( 104,   40, AMED_EXACTPOS,                   DIR_SW), // 06 Terminal 3
AMD( 104,   56, AMED_EXACTPOS,                   DIR_SW), // 07 Terminal 4
AMD( 104,   72, AMED_EXACTPOS,                   DIR_W), // 08 Terminal 5
AMD( 120,   72, AMED_EXACTPOS,                   DIR_NW), // 09 Terminal 6
AMD( 136,   72, AMED_EXACTPOS,                   DIR_N), // 10 Terminal 7
AMD( 136,   56, AMED_EXACTPOS,                   DIR_NE), // 11 Terminal 8
AMD( 136,   40, AMED_EXACTPOS,                   DIR_NE), // 12 Terminal 9
	/* Cargo Terminals (Helipads) */
AMD(  24,   40, AMED_EXACTPOS,                   DIR_NE), // 13 Cargo Term 1
AMD(  24,   56, AMED_EXACTPOS,                   DIR_NE), // 14 Cargo Term 2
AMD(  24,   72, AMED_EXACTPOS,                   DIR_N), // 15 Cargo Term 3
AMD(   8,   72, AMED_EXACTPOS,                   DIR_NW), // 16 Cargo Term 4
	/* Helicopter Terminals */
AMD(  24,   40, AMED_EXACTPOS,                   DIR_NE), // 17 Helipad 1
AMD(  24,   56, AMED_EXACTPOS,                   DIR_NE), // 18 Helipad 2
	/* Airplane - Holding Pattern */
AMD( 152,  488, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 19 Fly around waiting for a landing spot SE
AMD(  88,  424, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 20 Fly around waiting for a landing spot SE Check Runway 1
AMD(  72,  408, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 21 Fly around waiting for a landing spot SE Check Runway 2
AMD(-104,  232, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 22 Fly around waiting for a landing spot NE
AMD( 152,  -24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 23 Fly around waiting for a landing spot NW
AMD( 128, -200, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 24 Fly around waiting for a landing spot NW Unused (No Runways this direction.)
AMD( 408,  232, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 25 Fly around waiting for a landing spot SW
AMD(  80,  164, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 26 Fly around waiting for a landing spot SW Unused (No Runways this direction.)
	/* Airplane Landing Runway 1*/
AMD(  72,  344, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 27 Fly to landing position in air
AMD(  72,  136, AMED_HOLD | AMED_LAND,           DIR_N), // 28 Going down for land
AMD(  72,   56, AMED_HOLD | AMED_BRAKE,          DIR_N), // 29 Just landed, brake until end of runway
AMD(  72,   40, AMED_SLOWTURN,                   DIR_N), // 30 Taxi01 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing Runway 2*/
AMD(  56,  344, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 31 Fly to landing position in air
AMD(  56,  136, AMED_HOLD | AMED_LAND,           DIR_N), // 32 Going down for land
AMD(  56,   56, AMED_HOLD | AMED_BRAKE,          DIR_N), // 33 Just landed, brake until end of runway
AMD(  56,   40, AMED_SLOWTURN,                   DIR_N), // 34 Taxi02 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Taxi */
AMD(  40,   40, 0,                               DIR_N), // 35 Taxi03 HELI01 CARG01
AMD(  40,   56, 0,                               DIR_N), // 36 Taxi04 HELI02 CARG02
AMD(  40,   72, 0,                               DIR_N), // 37 Taxi05 CARG03
AMD(  40,   88, 0,                               DIR_N), // 38 Taxi06
AMD(  24,   88, 0,                               DIR_N), // 39 Taxi07
AMD(   8,   88, 0,                               DIR_N), // 40 Taxi08 CARG04
AMD(   8,  104, 0,                               DIR_N), // 41 Taxi09
AMD(   8,  120, AMED_EXACTPOS,                   DIR_SW), // 42 Taxi10 Runway 3
AMD(  56,   24, 0,                               DIR_N), // 43 Taxi11
AMD(  72,   24, 0,                               DIR_N), // 44 Taxi12
AMD(  88,    8, 0,                               DIR_N), // 45 Taxi13 TERM01
AMD(  88,   24, 0,                               DIR_N), // 46 Taxi14 TERM02
AMD(  88,   40, 0,                               DIR_N), // 47 Taxi15 TERM03
AMD(  88,   56, 0,                               DIR_N), // 48 Taxi16 TERM04
AMD(  88,   72, 0,                               DIR_N), // 49 Taxi17 TERM05
AMD(  88,   88, 0,                               DIR_N), // 50 Taxi18
AMD( 104,   88, 0,                               DIR_N), // 51 Taxi19
AMD( 120,   88, 0,                               DIR_N), // 52 Taxi20 TERM06
AMD( 136,   88, 0,                               DIR_N), // 53 Taxi21 TERM07
AMD( 152,   88, 0,                               DIR_N), // 54 Taxi22
AMD( 152,   72, 0,                               DIR_N), // 55 Taxi23
AMD( 152,   56, 0,                               DIR_N), // 56 Taxi24 TERM08
AMD( 152,  104, 0,                               DIR_N), // 57 Taxi25
AMD( 136,  104, AMED_EXACTPOS,                   DIR_NE), // 58 Taxi26 Runway 4
	/* Airplane Takeoff Runway 3 */
AMD(   8,  120, AMED_NOSPDCLAMP,                 DIR_N), // 59 Start Takeoff
AMD(  24,  120, AMED_NOSPDCLAMP,                 DIR_N), // 60 Release control of runway, for smoother movement
AMD( 104,  120, AMED_NOSPDCLAMP,                 DIR_N), // 61 Airborne point - Takeoff
AMD( 168,  120, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 62 End Take off    Airplane leaves Airport Control
	/* Airplane Takeoff Runway 4 */
AMD( 136,  104, AMED_NOSPDCLAMP,                 DIR_N), // 63 Start Takeoff
AMD( 120,  104, AMED_NOSPDCLAMP,                 DIR_N), // 64 Release control of runway, for smoother movement
AMD(  40,  104, AMED_NOSPDCLAMP,                 DIR_N), // 65 Airborne point - Takeoff
AMD( -24,  104, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 66 End Take off    Airplane leaves Airport Control
	/* Helicopter - Holding Pattern */
AMD(  40,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 67 Bufferspace before helipad
AMD(  40,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 68 Bufferspace before helipad
	/* Helicopter Hangar */
AMD(  32,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 69 Go to position for Hangarentrance in air
AMD(  40,   24, AMED_HELI_LOWER,                 DIR_N), // 70 Land in HANGAR1_AREA to go to hangar
	/* Helicopter Takeoff */
AMD(  24,   40, AMED_HELI_RAISE,                 DIR_N), // 71 Takeoff Helipad1
AMD(  24,   56, AMED_HELI_RAISE,                 DIR_N), // 72 Takeoff Helipad2
	/* Helicopter Entry This airport is special because of crossing runways.  Helicopters need a seperate entry or it will clear flags for airplanes causing then to run into each other.*/
AMD(  88,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 73 Helicopter Entry
	// Abort Landings
AMD(  56,    8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 74 Abort Landing Runway 01
AMD(  72,    8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 75 Abort Landing Runway 02
	// Helicopter Positioning
AMD(  24,   48, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 76 Get in position for Helipad01
AMD(  24,   64, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 77 Get in position for Helipad02
	// Helicopter Landing at Terminals
AMD(  24,   40, AMED_HELI_LOWER,                 DIR_N), // 78 Land at Helipad01
AMD(  24,   56, AMED_HELI_LOWER,                 DIR_N), // 79 Land at Helipad02
	// Helicopter Takeoff Hanger 2
AMD( 152,   40, AMED_HELI_RAISE,                 DIR_N), // 71 Takeoff Helipad1
};

/** Leipzig Airport - 2 Hangars, 2 Runways, 7 Terminals, 2 Helipads, 4 Cargo Terminals. */
static const AirportMovingData _airport_moving_data_leipzig[79] = {
	/* Hangars and Outside Hangars */
AMD(  88,   24, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar 1
AMD( 168,    8, AMED_EXACTPOS,                   DIR_SE), // 01 In Hangar 2
AMD( 104,   24, 0,                               DIR_N), // 02 Taxi to right outside Hangar1
AMD( 152,    8, 0,                               DIR_N), // 03 Taxi to right outside Hangar2
	/* Airplane Terminals */
AMD( 104,   40, AMED_EXACTPOS,                   DIR_NE), // 04 Terminal 1
AMD( 104,   56, AMED_EXACTPOS,                   DIR_NE), // 05 Terminal 2
AMD( 104,   72, AMED_EXACTPOS,                   DIR_NE), // 06 Terminal 3
AMD( 104,   88, AMED_EXACTPOS,                   DIR_NE), // 07 Terminal 4
AMD( 104,  104, AMED_EXACTPOS,                   DIR_NE), // 08 Terminal 5
AMD( 168,   24, AMED_EXACTPOS,                   DIR_SW), // 09 Terminal 6
AMD( 168,   40, AMED_EXACTPOS,                   DIR_SW), // 10 Terminal 7
	/* Cargo Terminals (Helipads) */
AMD( 168,   56, AMED_EXACTPOS,                   DIR_SW), // 11 Cargo Term 1
AMD( 168,   72, AMED_EXACTPOS,                   DIR_SW), // 12 Cargo Term 2
AMD( 168,   88, AMED_EXACTPOS,                   DIR_SW), // 13 Cargo Term 3
AMD( 168,  104, AMED_EXACTPOS,                   DIR_SW), // 14 Cargo Term 4
	/* Helicopter Terminals */
AMD( 168,   56, AMED_EXACTPOS,                   DIR_SW), // 15 Helipad 1
AMD( 168,   72, AMED_EXACTPOS,                   DIR_SW), // 16 Helipad 2
	/* Airplane - Holding Pattern */
AMD( -56,  536, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 19 Fly around waiting for a landing spot SE
AMD(   8,  472, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 20 Fly around waiting for a landing spot SE Check Runway 1
AMD(  24,  456, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 21 Fly around waiting for a landing spot SE Check Runway 2
AMD( 200,  280, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 22 Fly around waiting for a landing spot SW
AMD( -56,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 23 Fly around waiting for a landing spot NW
AMD( 128, -200, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 24 Fly around waiting for a landing spot NW Unused (No Runways this direction.)
AMD(-312,  280, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 25 Fly around waiting for a landing spot NE
AMD(  80,  164, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 26 Fly around waiting for a landing spot SW Unused (No Runways this direction.)
	/* Airplane Landing Runway 1*/
AMD(   8,  312, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 25 Fly to landing position in air
AMD(   8,  104, AMED_HOLD | AMED_LAND,           DIR_N), // 26 Going down for land
AMD(   8,    8, AMED_HOLD | AMED_BRAKE,          DIR_N), // 27 Just landed, brake until end of runway
AMD(   8,    8, AMED_SLOWTURN,                   DIR_N), // 28 Taxi01 Unused
	/* Airplane Landing Runway 2*/
AMD( 136,  312, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 29 Fly to landing position in air
AMD( 136,  104, AMED_HOLD | AMED_LAND,           DIR_N), // 30 Going down for land
AMD( 136,    8, AMED_HOLD | AMED_BRAKE,          DIR_N), // 31 Just landed, brake until end of runway
AMD( 136,    8, AMED_SLOWTURN,                   DIR_N), // 32 Taxi02 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Taxi */
AMD(  24,    8, 0,                               DIR_N), // 33 Taxi02
AMD(  40,    8, 0,                               DIR_N), // 34 Taxi03
AMD(  56,    8, 0,                               DIR_N), // 35 Taxi04
AMD(  72,    8, 0,                               DIR_N), // 36 Taxi05
AMD(  88,    8, 0,                               DIR_N), // 37 Taxi06
AMD( 104,    8, 0,                               DIR_N), // 38 Taxi07
AMD( 120,    8, 0,                               DIR_N), // 39 Taxi08
AMD( 120,   24, 0,                               DIR_N), // 40 Taxi09
AMD( 120,   40, 0,                               DIR_N), // 41 Taxi10
AMD( 120,   56, 0,                               DIR_N), // 42 Taxi11
AMD( 120,   72, 0,                               DIR_N), // 43 Taxi12
AMD( 120,   88, 0,                               DIR_N), // 44 Taxi13
AMD( 120,  104, 0,                               DIR_N), // 45 Taxi14
AMD( 120,  120, 0,                               DIR_N), // 46 Taxi15
AMD( 104,  120, 0,                               DIR_N), // 47 Taxi16
AMD(  88,  120, 0,                               DIR_N), // 48 Taxi17
AMD(  72,  120, 0,                               DIR_N), // 49 Taxi18
AMD(  56,  120, 0,                               DIR_N), // 50 Taxi19
AMD(  40,  120, 0,                               DIR_N), // 51 Taxi20
AMD(  24,  120, 0,                               DIR_N), // 52 Taxi21
AMD( 152,   24, 0,                               DIR_N), // 53 Taxi23
AMD( 152,   40, 0,                               DIR_N), // 54 Taxi24
AMD( 152,   56, 0,                               DIR_N), // 55 Taxi25
AMD( 152,   72, 0,                               DIR_N), // 56 Taxi26
AMD( 152,   88, 0,                               DIR_N), // 57 Taxi27
AMD( 152,  104, 0,                               DIR_N), // 58 Taxi28
AMD( 152,  120, 0,                               DIR_N), // 59 Taxi29
	/* Airplane Takeoff Runway 1 */
AMD(   8,  120, AMED_NOSPDCLAMP,                 DIR_N), // 60 Start Takeoff
AMD(   8,  104, AMED_NOSPDCLAMP,                 DIR_N), // 61 Release control of runway, for smoother movement
AMD(   8,   24, AMED_NOSPDCLAMP,                 DIR_N), // 62 Airborne point - Takeoff
AMD(   8,  -40, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 63 End Take off    Airplane leaves Airport Control
	/* Airplane Takeoff Runway 2 */
AMD( 136,  120, AMED_NOSPDCLAMP,                 DIR_N), // 64 Start Takeoff
AMD( 136,  104, AMED_NOSPDCLAMP,                 DIR_N), // 65 Release control of runway, for smoother movement
AMD( 136,   24, AMED_NOSPDCLAMP,                 DIR_N), // 66 Airborne point - Takeoff
AMD( 136,  -40, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 67 End Take off    Airplane leaves Airport Control
	/* Helicopter - Holding Pattern */
AMD( 152,  104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 68 Bufferspace before helipad
AMD( 152,  104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 69 Bufferspace before helipad
	/* Helicopter Hangar */
AMD( 152,    0, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 70 Go to position for Hangarentrance in air
AMD( 152,    8, AMED_HELI_LOWER,                 DIR_N), // 71 Land in HANGAR2_AREA to go to hangar
	/* Helicopter Takeoff */
AMD( 168,   56, AMED_HELI_RAISE,                 DIR_N), // 72 Takeoff Helipad1
AMD( 168,   72, AMED_HELI_RAISE,                 DIR_N), // 73 Takeoff Helipad2
	/* Helicopter Entry */
AMD( 152,  104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 74 Helicopter Entry
	// Helicopter Positioning
AMD( 168,   48, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 75 Get in position for Helipad01
AMD( 168,   64, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 76 Get in position for Helipad02
	// Helicopter Landing at Terminals
AMD( 168,   56, AMED_HELI_LOWER,                 DIR_N), // 77 Land at Helipad01
AMD( 168,   72, AMED_HELI_LOWER,                 DIR_N), // 78 Land at Helipad02
};

/** Cargo Small Airport 2 Hangar, 1 Runway, 4 Cargo Terminals, 4 Helipads. */
static const AirportMovingData _airport_moving_data_cargos[57] = {
AMD(   8,   24, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar 1
AMD( 104,   24, AMED_EXACTPOS,                   DIR_SE), // 01 In Hangar 2
AMD(   8,   40, 0,                               DIR_N), // 02 Taxi to right outside hangar 1
AMD( 104,   40, 0,                               DIR_N), // 03 Taxi to right outside hangar 2
	// Cargo terminals (Share with Helipads)
AMD(  24,   24, AMED_EXACTPOS,                   DIR_NE), // 04 Cargo 1
AMD(  24,    8, AMED_EXACTPOS,                   DIR_NE), // 05 Cargo 2
AMD(  88,   24, AMED_EXACTPOS,                   DIR_SW), // 06 Cargo 3
AMD(  88,    8, AMED_EXACTPOS,                   DIR_SW), // 07 Cargo 4
	// Helipads
AMD(  24,   24, AMED_EXACTPOS,                   DIR_NE), // 08 Helipad 1
AMD(  24,    8, AMED_EXACTPOS,                   DIR_NE), // 09 Helipad 2
AMD(  88,   24, AMED_EXACTPOS,                   DIR_SW), // 10 Helipad 3
AMD(  88,    8, AMED_EXACTPOS,                   DIR_SW), // 11 Heliapd 4
	// Airplane Holding Pattern
AMD(   8,  -24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 12 Fly around waiting for a landing spot (north-east)
AMD( 264, -280, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 13 Fly around waiting for a landing spot (north-west)
AMD( 520,  -24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 14 Fly around waiting for a landing spot (south-west) and Runway 1 Entry
AMD( 408,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 15 Unused
AMD( 264,  232, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 16 Fly around waiting for a landing spot (south-east)
AMD(   1,    6, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 17 Unused
AMD( 193,    6, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 18 Unused
AMD( 225,   62, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 19 Unused
	// Airplane Landing
AMD( 408,   56, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 20 Fly to landing position in air
AMD( 104,   56, AMED_HOLD | AMED_LAND,           DIR_N), // 21 Going down for land
AMD(   8,   56, AMED_HOLD | AMED_BRAKE,          DIR_N), // 22 Just landed, brake until end of runway
AMD(   8,   56, 0,                               DIR_N), // 23 TAXI01 Just landed, Move forward 1 square
	// Main FLow
AMD(  24,   40, 0,                               DIR_N), // 24 TAXI02 Taxiing
AMD(  40,   40, 0,                               DIR_N), // 25 TAXI03 Taxiing
AMD(  56,   40, 0,                               DIR_N), // 26 TAXI04 Taxiing
AMD(  72,   40, 0,                               DIR_N), // 27 TAXI05 Taxiing
AMD(  88,   40, 0,                               DIR_N), // 28 TAXI06 Taxiing
AMD( 120,   40, 0,                               DIR_N), // 29 TAXI07 Taxiing
AMD(  40,   24, 0,                               DIR_N), // 30 TAXI08 Taxiing
AMD(  40,    8, 0,                               DIR_N), // 31 TAXI09 Taxiing
AMD(  72,   24, 0,                               DIR_N), // 32 TAXI10 Taxiing
AMD(  72,    8, 0,                               DIR_N), // 33 TAXI11 Taxiing
	// Airplane Takeoff Runway 01
AMD( 120,   56, AMED_EXACTPOS,                   DIR_NE), // 34 Accelerate to end of runway
AMD( 104,   56, AMED_NOSPDCLAMP,                 DIR_N), // 35 Release control of runway, for smoother movement
AMD(  40,   56, AMED_NOSPDCLAMP,                 DIR_N), // 36 End of runway
AMD( -24,   56, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 37 Take off
	/* Helicopter - Holding Pattern */
AMD(  56,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 38 Bufferspace before helipad
AMD(  56,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 39 Bufferspace before helipad
	/* Helicopter Hangar */
AMD(   0,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 40 Go to position for Hangarentrance in air
AMD(   8,   40, AMED_HELI_LOWER,                 DIR_N), // 41 Land in HANGAR1_AREA to go to hangar
	/* Helicopter Takeoff */
AMD(  24,   24, AMED_HELI_RAISE,                 DIR_N), // 42 Takeoff Helipad1
AMD(  24,    8, AMED_HELI_RAISE,                 DIR_N), // 43 Takeoff Helipad2
AMD(  88,   24, AMED_HELI_RAISE,                 DIR_N), // 44 Takeoff Helipad3
AMD(  88,    8, AMED_HELI_RAISE,                 DIR_N), // 45 Takeoff Helipad4
AMD(   8,   40, AMED_HELI_RAISE,                 DIR_N), // 46 Takeoff Hangar1
AMD( 104,   40, AMED_HELI_RAISE,                 DIR_N), // 47 Takeoff Hangar2
	/* Helicopter Entry */
AMD( 104,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 48 Helicopter Entry
	// Helicopter Positioning
AMD(  16,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 49 Get in position for Helipad01
AMD(  16,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 50 Get in position for Helipad02
AMD(  96,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 51 Get in position for Helipad03
AMD(  96,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 52 Get in position for Helipad04
	// Helicopter Landing at Terminals
AMD(  24,   24, AMED_HELI_LOWER,                 DIR_N), // 53 Land at Helipad01
AMD(  24,    8, AMED_HELI_LOWER,                 DIR_N), // 54 Land at Helipad02
AMD(  88,   24, AMED_HELI_LOWER,                 DIR_N), // 55 Land at Helipad03
AMD(  88,    8, AMED_HELI_LOWER,                 DIR_N), // 56 Land at Helipad04
};

/** Cargo Large Airport - 2 hangars, 2 runways, 8 Cargo Terminals. */
static const AirportMovingData _airport_moving_data_cargol[75] = {
	/* Hangars and Outside Hangars */
AMD(  24,   88, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar 1
AMD( 120,   56, AMED_EXACTPOS,                   DIR_NW), // 01 In Hangar 2
AMD(  24,  104, 0,                               DIR_N), // 02 Taxi to right outside Hangar1
AMD( 120,   40, 0,                               DIR_N), // 03 Taxi to right outside Hangar2
	/* Cargo Terminals (Helipads) */
AMD(  40,   72, AMED_EXACTPOS,                   DIR_N), // 04 Cargo Term 1
AMD(  56,   56, AMED_EXACTPOS,                   DIR_N), // 05 Cargo Term 2
AMD(  72,   40, AMED_EXACTPOS,                   DIR_N), // 06 Cargo Term 3
AMD(  88,   24, AMED_EXACTPOS,                   DIR_N), // 07 Cargo Term 4
AMD(  56,  120, AMED_EXACTPOS,                   DIR_S), // 08 Cargo Term 5
AMD(  72,  104, AMED_EXACTPOS,                   DIR_S), // 09 Cargo Term 6
AMD(  88,   88, AMED_EXACTPOS,                   DIR_S), // 10 Cargo Term 7
AMD( 104,   72, AMED_EXACTPOS,                   DIR_S), // 11 Cargo Term 8
	/* Helipads */
AMD(  40,   72, AMED_EXACTPOS,                   DIR_N), // 12 Helipad 1
AMD(  56,   56, AMED_EXACTPOS,                   DIR_N), // 13 Helipad 2
AMD(  72,   40, AMED_EXACTPOS,                   DIR_N), // 14 Helipad 3
AMD(  88,   24, AMED_EXACTPOS,                   DIR_N), // 15 Helipad 4
AMD(  56,  120, AMED_EXACTPOS,                   DIR_S), // 16 Helipad 5
AMD(  72,  104, AMED_EXACTPOS,                   DIR_S), // 17 Helipad 6
AMD(  88,   88, AMED_EXACTPOS,                   DIR_S), // 18 Helipad 7
AMD( 104,   72, AMED_EXACTPOS,                   DIR_S), // 19 Helipad 8
	// Airplane Holding Pattern
AMD(  24,   56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 20 Fly around waiting for a landing spot (north-east)
AMD( 280, -200, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 21 Fly around waiting for a landing spot (north-west)
AMD( 536,   56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 22 Fly around waiting for a landing spot (south-west) also entry runway 1
AMD( 352,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 23 Unused
AMD( 280,  312, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 24 Fly around waiting for a landing spot (south-east)
AMD(   1,    6, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 25 Unused
AMD( 193,    6, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 26 Unused
AMD( 225,   62, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 27 Unused
	/* Airplane Landing */
AMD( 424,  136, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 28 Fly to landing position in air
AMD( 120,  136, AMED_HOLD | AMED_LAND,           DIR_N), // 29 Going down for land
AMD(  24,  136, AMED_HOLD | AMED_BRAKE,          DIR_N), // 30 Just landed, brake until end of runway
AMD(  24,  136, AMED_SLOWTURN,                   DIR_N), // 31 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Taxi */
AMD(  24,  118, 0,                               DIR_N), // 32 TAXI02
AMD(  40,  104, 0,                               DIR_N), // 33 TAXI03
AMD(  56,   88, 0,                               DIR_N), // 34 TAXI04
AMD(  72,   72, 0,                               DIR_N), // 35 TAXI05
AMD(  88,   56, 0,                               DIR_N), // 36 TAXI06
AMD( 104,   40, 0,                               DIR_N), // 37 TAXI07
AMD( 120,   24, 0,                               DIR_N), // 38 TAXI08
	// Airplane Takeoff Runway 02
AMD( 120,    8, AMED_EXACTPOS,                   DIR_NE), // 39 Accelerate to end of runway
AMD( 104,    8, AMED_NOSPDCLAMP,                 DIR_N), // 40 Release control of runway, for smoother movement
AMD(  24,    8, AMED_NOSPDCLAMP,                 DIR_N), // 41 End of runway
AMD( -40,    8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 42 Take off
	/* Helicopter - Holding Pattern */
AMD(  24,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 43 Bufferspace before helipad
AMD(  24,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 44 Bufferspace before helipad
	/* Helicopter Hangar */
AMD(  24,  112, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 45 Go to position for Hangarentrance in air
AMD(  24,  104, AMED_HELI_LOWER,                 DIR_N), // 46 Land in HANGAR1_AREA to go to hangar
	/* Helicopter Takeoff Helipads*/
AMD(  40,   72, AMED_HELI_RAISE,                 DIR_N), // 47 Takeoff Helipad1
AMD(  56,   56, AMED_HELI_RAISE,                 DIR_N), // 48 Takeoff Helipad2
AMD(  72,   40, AMED_HELI_RAISE,                 DIR_N), // 49 Takeoff Helipad3
AMD(  88,   24, AMED_HELI_RAISE,                 DIR_N), // 50 Takeoff Helipad4
AMD(  56,  120, AMED_HELI_RAISE,                 DIR_N), // 51 Takeoff Helipad5
AMD(  72,  104, AMED_HELI_RAISE,                 DIR_N), // 52 Takeoff Helipad6
AMD(  88,   88, AMED_HELI_RAISE,                 DIR_N), // 53 Takeoff Helipad7
AMD( 104,   72, AMED_HELI_RAISE,                 DIR_N), // 54 Takeoff Helipad8
	/* Helicopter Takeoff Hangars*/
AMD(  24,  104, AMED_HELI_RAISE,                 DIR_N), // 55 Takeoff Hangar1
AMD( 120,   40, AMED_HELI_RAISE,                 DIR_N), // 56 Takeoff Hangar2
	/* Helicopter Entry */
AMD(  24,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 57 Helicopter Entry point.
	// Abort Landing Runway 01
AMD(  -8,  136, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 58 Abort Landing Runway 01
	// Helicopter Positioning
AMD(  48,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 59 Get in position for Helipad01
AMD(  64,   56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 60 Get in position for Helipad02
AMD(  80,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 61 Get in position for Helipad03
AMD(  96,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 62 Get in position for Helipad04
AMD(  56,  128, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 63 Get in position for Helipad05
AMD(  80,  104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 64 Get in position for Helipad06
AMD(  96,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 65 Get in position for Helipad07
AMD( 112,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 66 Get in position for Helipad08
	// Helicopter Landing at Terminals
AMD(  40,   72, AMED_HELI_LOWER,                 DIR_N), // 67 Land at Helipad01
AMD(  56,   56, AMED_HELI_LOWER,                 DIR_N), // 68 Land at Helipad02
AMD(  72,   40, AMED_HELI_LOWER,                 DIR_N), // 69 Land at Helipad03
AMD(  88,   24, AMED_HELI_LOWER,                 DIR_N), // 71 Land at Helipad04
AMD(  56,  120, AMED_HELI_LOWER,                 DIR_N), // 71 Land at Helipad05
AMD(  72,  104, AMED_HELI_LOWER,                 DIR_N), // 72 Land at Helipad06
AMD(  88,   88, AMED_HELI_LOWER,                 DIR_N), // 73 Land at Helipad07
AMD( 104,   72, AMED_HELI_LOWER,                 DIR_N), // 74 Land at Helipad08
};

/** Cargo X-Large Airport - 2 hangars, 2 runways, 12 Cargo Terminals. */
static const AirportMovingData _airport_moving_data_cargox[117] = {
	/* Hangars and Outside Hangars */
AMD(  40,    8, AMED_EXACTPOS,                   DIR_SE), //  00 In Hangar 1
AMD(  72,  136, AMED_EXACTPOS,                   DIR_SE), //  01 In Hangar 2
AMD(  24,    8, 0,                               DIR_N), //  02 Taxi to right outside Hangar1
AMD(  88,  136, 0,                               DIR_N), //  03 Taxi to right outside Hangar2
	/* Cargo Terminals (Helipads) */
AMD(  40,   24, AMED_EXACTPOS,                   DIR_NW), //  04 Cargo Term 01
AMD(  56,   24, AMED_EXACTPOS,                   DIR_NW), //  05 Cargo Term 02
AMD(  72,   24, AMED_EXACTPOS,                   DIR_NW), //  06 Cargo Term 03
AMD(  40,   56, AMED_EXACTPOS,                   DIR_SE), //  07 Cargo Term 04
AMD(  56,   56, AMED_EXACTPOS,                   DIR_SE), //  08 Cargo Term 05
AMD(  72,   56, AMED_EXACTPOS,                   DIR_SE), //  09 Cargo Term 06
AMD(  40,   88, AMED_EXACTPOS,                   DIR_NW), //  10 Cargo Term 07
AMD(  56,   88, AMED_EXACTPOS,                   DIR_NW), //  11 Cargo Term 08
AMD(  72,   88, AMED_EXACTPOS,                   DIR_NW), //  12 Cargo Term 09
AMD(  40,  120, AMED_EXACTPOS,                   DIR_SE), //  13 Cargo Term 10
AMD(  56,  120, AMED_EXACTPOS,                   DIR_SE), //  14 Cargo Term 11
AMD(  72,  120, AMED_EXACTPOS,                   DIR_SE), //  15 Cargo Term 12
	/* Helicopter Terminals */
AMD(  40,   24, AMED_EXACTPOS,                   DIR_NW), //  16 Helipad 01
AMD(  56,   24, AMED_EXACTPOS,                   DIR_NW), //  17 Helipad 02
AMD(  72,   24, AMED_EXACTPOS,                   DIR_NW), //  18 Helipad 03
AMD(  40,   56, AMED_EXACTPOS,                   DIR_SE), //  19 Helipad 04
AMD(  56,   56, AMED_EXACTPOS,                   DIR_SE), //  20 Helipad 05
AMD(  72,   56, AMED_EXACTPOS,                   DIR_SE), //  21 Helipad 06
AMD(  40,   88, AMED_EXACTPOS,                   DIR_NW), //  22 Helipad 07
AMD(  56,   88, AMED_EXACTPOS,                   DIR_NW), //  23 Helipad 08
AMD(  72,   88, AMED_EXACTPOS,                   DIR_NW), //  24 Helipad 09
AMD(  40,  120, AMED_EXACTPOS,                   DIR_SE), //  25 Helipad 10
AMD(  56,  120, AMED_EXACTPOS,                   DIR_SE), //  26 Helipad 11
AMD(  72,  120, AMED_EXACTPOS,                   DIR_SE), //  27 Helipad 12
	// Airplane Holding Pattern
AMD(-400,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  28 Fly around waiting for a landing spot (north-east)
AMD(  64, -376, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  29 Fly around waiting for a landing spot (north-west) Runway 2 Entry
AMD(  88, -216, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  30 Unused
AMD( 512,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  31 Fly around waiting for a landing spot (south-west)
AMD(  48,  520, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  32 Fly around waiting for a landing spot (south-east) and Runway 1 Entry
AMD(  24,  520, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  33 Unused
AMD( 193,    6, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  34 Fly around waiting for a landing spot Unused
AMD( 225,   62, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  35 Fly around waiting for a landing spot Unused
	/* Airplane Landing */
AMD(   8,  408, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  36 Fly to landing position in air
AMD(   8,  104, AMED_HOLD | AMED_LAND,           DIR_N), //  37 Going down for land
AMD(   8,    8, AMED_HOLD | AMED_BRAKE,          DIR_N), //  38 Just landed, brake until end of runway
AMD(   8,    8, AMED_SLOWTURN,                   DIR_N), //  39 TAXI01 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing */
AMD( 104, -264, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  40 Fly to landing position in air
AMD( 104,   40, AMED_HOLD | AMED_LAND,           DIR_N), //  41 Going down for land
AMD( 104,  136, AMED_HOLD | AMED_BRAKE,          DIR_N), //  42 Just landed, brake until end of runway
AMD( 104,  136, AMED_SLOWTURN,                   DIR_N), //  43 TAXI09 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Taxi Left Side*/
AMD(  24,   24, 0,                               DIR_N), //  44 TAXI02
AMD(  24,   40, 0,                               DIR_N), //  45 TAXI03
AMD(  40,   40, 0,                               DIR_N), //  46 TAXI04
AMD(  56,   40, 0,                               DIR_N), //  47 TAXI05
AMD(  72,   40, 0,                               DIR_N), //  48 TAXI06
AMD(  88,   40, 0,                               DIR_N), //  49 TAXI07
AMD(  88,   24, 0,                               DIR_N), //  50 TAXI08
	// Airplane Taxi Right Side
AMD(  88,  120, 0,                               DIR_N), //  51 TAXI10
AMD(  88,  104, 0,                               DIR_N), //  52 TAXI11
AMD(  72,  104, 0,                               DIR_N), //  53 TAXI12
AMD(  56,  104, 0,                               DIR_N), //  54 TAXI13
AMD(  40,  104, 0,                               DIR_N), //  55 TAXI14
AMD(  24,  104, 0,                               DIR_N), //  56 TAXI15
AMD(  24,  120, 0,                               DIR_N), //  57 TAXI16
	// Taxi Left to Right
AMD(  88,   56, 0,                               DIR_N), //  58 TAXI17
AMD(  88,   72, 0,                               DIR_N), //  59 TAXI18
AMD(  88,   88, 0,                               DIR_N), //  60 TAXI19
	// Taxi Right to Left
AMD(  24,   88, 0,                               DIR_N), //  61 TAXI20
AMD(  24,   72, 0,                               DIR_N), //  62 TAXI21
AMD(  24,   56, 0,                               DIR_N), //  63 TAXI22
	// Airplane Takeoff Runway 01
AMD(   8,  120, AMED_EXACTPOS,                   DIR_NW), //  64 Accelerate to end of runway
AMD(   8,  104, AMED_NOSPDCLAMP,                 DIR_N), //  65 Release control of runway, for smoother movement
AMD(   8,   40, AMED_NOSPDCLAMP,                 DIR_N), //  66 End of runway
AMD(   8,  -24, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), //  67 Take off
	// Airplane Takeoff Runway 02
AMD( 104,   24, AMED_EXACTPOS,                   DIR_SE), //  68 Accelerate to end of runway
AMD( 104,   40, AMED_NOSPDCLAMP,                 DIR_N), //  69 Release control of runway, for smoother movement
AMD( 104,  104, AMED_NOSPDCLAMP,                 DIR_N), //  70 End of runway
AMD( 104,  168, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), //  71 Take off
	/* Helicopter - Holding Pattern */
AMD(  56,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  72 Bufferspace before helipad
AMD(  56,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  73 Bufferspace before helipad
	/* Helicopter Hangar 1 */
AMD(  24,    0, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  74 Go to position for Hangarentrance in air
AMD(  24,    8, AMED_HELI_LOWER,                 DIR_N), //  75 Land in HANGAR1_AREA to go to hangar
	/* Helicopter Hangar 2 */
AMD(  88,  144, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  76 Go to position for Hangarentrance in air
AMD(  88,  136, AMED_HELI_LOWER,                 DIR_N), //  77 Land in HANGAR2_AREA to go to hangar
	/* Helicopter Takeoff Helipads*/
AMD(  40,   24, AMED_HELI_RAISE,                 DIR_N), //  78 Takeoff Helipad 01
AMD(  56,   24, AMED_HELI_RAISE,                 DIR_N), //  79 Takeoff Helipad 02
AMD(  72,   24, AMED_HELI_RAISE,                 DIR_N), //  80 Takeoff Helipad 03
AMD(  40,   56, AMED_HELI_RAISE,                 DIR_N), //  81 Takeoff Helipad 04
AMD(  56,   56, AMED_HELI_RAISE,                 DIR_N), //  82 Takeoff Helipad 05
AMD(  72,   56, AMED_HELI_RAISE,                 DIR_N), //  83 Takeoff Helipad 06
AMD(  40,   88, AMED_HELI_RAISE,                 DIR_N), //  84 Takeoff Helipad 07
AMD(  56,   88, AMED_HELI_RAISE,                 DIR_N), //  85 Takeoff Helipad 08
AMD(  72,   88, AMED_HELI_RAISE,                 DIR_N), //  86 Takeoff Helipad 09
AMD(  40,  104, AMED_HELI_RAISE,                 DIR_N), //  87 Takeoff Helipad 10
AMD(  56,  104, AMED_HELI_RAISE,                 DIR_N), //  88 Takeoff Helipad 11
AMD(  72,  104, AMED_HELI_RAISE,                 DIR_N), //  89 Takeoff Helipad 12
	/* Helicopter Takeoff Hangars*/
AMD(  24,    8, AMED_HELI_RAISE,                 DIR_N), //  90 Takeoff Hangar1
AMD(  88,  136, AMED_HELI_RAISE,                 DIR_N), //  91 Takeoff Hangar2
	/* Helicopter Entry */
AMD(  56,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  92 Helicopter Entry point.
	// Helicopter Positioning
AMD(  40,   16, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  93 Get in position for Helipad01
AMD(  56,   16, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  94 Get in position for Helipad02
AMD(  72,   16, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  95 Get in position for Helipad03
AMD(  36,   52, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  96 Get in position for Helipad04
AMD(  56,   48, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  97 Get in position for Helipad05
AMD(  76,   52, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  98 Get in position for Helipad06
AMD(  36,   92, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  99 Get in position for Helipad07
AMD(  56,   96, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 100 Get in position for Helipad08
AMD(  76,   92, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 101 Get in position for Helipad09
AMD(  40,  128, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 102 Get in position for Helipad10
AMD(  56,  128, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 103 Get in position for Helipad11
AMD(  72,  128, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 104 Get in position for Helipad12
	// Helicopter Landing at Terminals
AMD(  40,   24, AMED_HELI_LOWER,                 DIR_N), // 105 Land at Helipad01
AMD(  56,   24, AMED_HELI_LOWER,                 DIR_N), // 106 Land at Helipad02
AMD(  72,   24, AMED_HELI_LOWER,                 DIR_N), // 107 Land at Helipad03
AMD(  40,   56, AMED_HELI_LOWER,                 DIR_N), // 108 Land at Helipad04
AMD(  56,   56, AMED_HELI_LOWER,                 DIR_N), // 109 Land at Helipad05
AMD(  72,   56, AMED_HELI_LOWER,                 DIR_N), // 110 Land at Helipad06
AMD(  40,   88, AMED_HELI_LOWER,                 DIR_N), // 111 Land at Helipad07
AMD(  56,   88, AMED_HELI_LOWER,                 DIR_N), // 112 Land at Helipad08
AMD(  72,   88, AMED_HELI_LOWER,                 DIR_N), // 113 Land at Helipad09
AMD(  40,  120, AMED_HELI_LOWER,                 DIR_N), // 114 Land at Helipad10
AMD(  56,  120, AMED_HELI_LOWER,                 DIR_N), // 115 Land at Helipad11
AMD(  72,  120, AMED_HELI_LOWER,                 DIR_N), // 116 Land at Helipad12
};

/** Commuter Large Airport - 2 hangars, 1 runways, 6 Terminals, 2 Helipads, 2 Cargo Terminals. */
static const AirportMovingData _airport_moving_data_commuterl[47] = {
	/* Hangars and Outside Hangars */
AMD(  40,   56, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar 1
AMD(  88,   56, AMED_EXACTPOS,                   DIR_SE), // 01 In Hangar 2
AMD(  24,   56, 0,                               DIR_N), // 02 Taxi to right outside Hangar1
AMD( 104,   56, 0,                               DIR_N), // 03 Taxi to right outside Hangar2
	/* Airplane Terminals */
AMD(  24,   24, AMED_EXACTPOS,                   DIR_NW), // 04 Terminal 1
AMD(  40,   24, AMED_EXACTPOS,                   DIR_NW), // 05 Terminal 2
AMD(  56,   24, AMED_EXACTPOS,                   DIR_NW), // 06 Terminal 3
AMD(  72,   24, AMED_EXACTPOS,                   DIR_NW), // 07 Terminal 4
AMD(  88,   24, AMED_EXACTPOS,                   DIR_NW), // 08 Terminal 5
AMD( 104,   24, AMED_EXACTPOS,                   DIR_NW), // 09 Terminal 6
	/* Cargo Terminals (Helipads) */
AMD(  56,   56, AMED_EXACTPOS,                   DIR_SE), // 10 Cargo Term 1
AMD(  72,   56, AMED_EXACTPOS,                   DIR_SE), // 11 Cargo Term 2
	/* Helicopter Terminals */
AMD(  56,   56, AMED_EXACTPOS,                   DIR_SE), // 12 Helipad 1
AMD(  72,   56, AMED_EXACTPOS,                   DIR_SE), // 13 Helipad 2
	// Airplane Holding Pattern
AMD(  24, -104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 14 Fly around waiting for a landing spot (north-east)
AMD( 280, -360, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 15 Fly around waiting for a landing spot (north-west)
AMD( 536, -104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 16 Fly around waiting for a landing spot (south-west)
AMD( 488,  -56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 17 Fly around waiting for a landing spot Runway 1 Entry spot
AMD( 280,  152, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 18 Fly around waiting for a landing spot (south-east)
AMD(  24,  360, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 19 Fly around waiting for a landing spot Unused
AMD( 193,    6, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 20 Fly around waiting for a landing spot Unused
AMD( 225,   62, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 21 Fly around waiting for a landing spot Unused
	/* Airplane Landing */
AMD( 312,   72, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 22 Fly to landing position in air
AMD( 104,   72, AMED_HOLD | AMED_LAND,           DIR_N), // 23 Going down for land
AMD(   8,   72, AMED_HOLD | AMED_BRAKE,          DIR_N), // 24 Just landed, brake until end of runway
AMD(   8,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 25 Unused
	/* Airplane Taxi */
AMD(   8,   56, 0,                               DIR_N), // 26 TAXI02
AMD(  24,   40, 0,                               DIR_N), // 27 TAXI03
AMD(  40,   40, 0,                               DIR_N), // 28 TAXI04
AMD(  56,   40, 0,                               DIR_N), // 29 TAXI05
AMD(  72,   40, 0,                               DIR_N), // 30 TAXI06
AMD(  88,   40, 0,                               DIR_NW), // 31 TAXI07
AMD( 104,   40, 0,                               DIR_N), // 32 TAXI08
AMD( 120,   56, 0,                               DIR_N), // 33 TAXI09
	// Airplane Takeoff Runway 01
AMD( 120,   72, 0,                               DIR_NE), // 34 Accelerate to end of runway
AMD( 104,   72, AMED_NOSPDCLAMP,                 DIR_N), // 35 Release control of runway, for smoother movement
AMD(  40,   72, AMED_NOSPDCLAMP,                 DIR_N), // 36 End of runway
AMD( -24,   72, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 37 Take off
	/* Helicopter - Holding Pattern */
AMD(  24,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 38 Bufferspace before helipad
AMD(  24,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 39 Bufferspace before helipad
	/* Helicopter Hangar 1 */
AMD(  24,   64, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 40 Go to position for Hangarentrance in air
AMD(  24,   56, AMED_HELI_LOWER,                 DIR_N), // 41 Land in HANGAR1_AREA to go to hangar
	/* Helicopter Takeoff Helipads*/
AMD(  56,   56, AMED_HELI_RAISE,                 DIR_N), // 42 Takeoff Helipad 01
AMD(  72,   56, AMED_HELI_RAISE,                 DIR_N), // 43 Takeoff Helipad 02
	/* Helicopter Takeoff Hangars*/
AMD(  24,   56, AMED_HELI_RAISE,                 DIR_N), // 44 Takeoff Hangar1
AMD( 104,   56, AMED_HELI_RAISE,                 DIR_N), // 45 Takeoff Hangar2
	/* Helicopter Entry */
AMD(   8,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 46 Helicopter Entry point.
};

/** Windy Airport - 1 hangars, 1 runways, 1 Terminals, 1 Helipads, 1 Cargo Terminals. */
static const AirportMovingData _airport_moving_data_windy[36] = {
AMD(  40,   24, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar 1
AMD(  24,   24, 0,                               DIR_N), // 01 Taxi to right outside hangar 1
	// Terminals
AMD(  72,   24, AMED_EXACTPOS,                   DIR_SE), // 02 Terminal 1
	// Cargo terminals (Share with Helipads)
AMD(  88,   24, AMED_EXACTPOS,                   DIR_SE), // 03 Cargo 1
	// Helipads
AMD(  88,   24, AMED_EXACTPOS,                   DIR_SE), // 04 Helipad 1
	// Airplane Holding Pattern
AMD(  -8, -136, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 05 Fly around waiting for a landing spot (north-east)
AMD( 248, -392, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 06 Fly around waiting for a landing spot (north-west)
AMD( 504, -136, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 07 Fly around waiting for a landing spot (south-west)
AMD( 456,  -88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 08 Fly around waiting for a landing spot Runway 1 Entry spot
AMD( 248,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 09 Fly around waiting for a landing spot (south-east)
AMD(   1,    6, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 10 Fly around waiting for a landing spot (north-west)
AMD( 193,    6, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 11 Fly around waiting for a landing spot (south-west)
AMD( 225,   62, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 12 Fly around waiting for a landing spot (south)
	// Airplane Landing
AMD( 296,   40, AMED_HOLD | AMED_SLOWTURN,	     DIR_N), // 13 Fly to landing position in air
AMD(  72,   40, AMED_HOLD | AMED_LAND,           DIR_N), // 14 Going down for land
AMD(   8,   40, AMED_HOLD | AMED_BRAKE,          DIR_N), // 15 Just landed, brake until end of runway
AMD(   8,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 16 Unused
	// Main FLow
AMD(   8,   24, 0,                               DIR_N), // 17 TAXI02 Taxiing
AMD(  24,    8, 0,                               DIR_N), // 18 TAXI03 Taxiing
AMD(  40,    8, 0,                               DIR_N), // 19 TAXI04 Taxiing
AMD(  56,    8, 0,                               DIR_N), // 20 TAXI05 Taxiing
AMD(  72,    8, 0,                               DIR_N), // 21 TAXI06 Taxiing
AMD(  88,    8, 0,                               DIR_N), // 22 TAXI07 Taxiing
AMD( 104,    8, 0,                               DIR_N), // 23 TAXI08 Taxiing
AMD( 120,   24, 0,                               DIR_N), // 24 TAXI09 Taxiing
	// Airplane Takeoff Runway 01
AMD( 120,   40, AMED_EXACTPOS,                   DIR_NE), // 25 Accelerate to end of runway
AMD( 104,   40, AMED_NOSPDCLAMP,                 DIR_N), // 26 Release control of runway, for smoother movement
AMD(  40,   40, AMED_NOSPDCLAMP,                 DIR_N), // 27 End of runway
AMD( -24,   40, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 28 Take off
	/* Helicopter - Holding Pattern */
AMD(  72,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 29 Bufferspace before helipad
AMD(  72,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 30 Bufferspace before helipad
	/* Helicopter Hangar */
AMD(  16,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 31 Go to position for Hangarentrance in air
AMD(  24,   24, AMED_HELI_LOWER,                 DIR_N), // 32 Land in HANGAR1_AREA to go to hangar
	/* Helicopter Takeoff */
AMD(  88,   24, AMED_HELI_RAISE,                 DIR_N), // 33 Takeoff Helipad1
AMD(  24,   24, AMED_HELI_RAISE,                 DIR_N), // 34 Takeoff Hangar1
	/* Helicopter Entry */
AMD( 104,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 35 Helicopter Entry
};

/** Indianapolis Airport - 4 hangars, 4 runways, 26 terminals, 2 helipad, 6 Cargo. */
static const AirportMovingData _airport_moving_data_indianapolis[131] = {
	/* Hangars and Outside Hangars */
AMD(  24,  184, AMED_EXACTPOS,                   DIR_SW), //  00 In Hangar 01
AMD(  24,   40, AMED_EXACTPOS,                   DIR_SW), //  01 In Hangar 02
AMD( 168,  168, AMED_EXACTPOS,                   DIR_SE), //  02 In Hangar 03
AMD( 168,   56, AMED_EXACTPOS,                   DIR_NW), //  03 In Hangar 04
AMD(  40,  184, 0,                               DIR_N), //  04 Taxi to right outside Hangar01
AMD(  40,   40, 0,                               DIR_N), //  05 Taxi to right outside Hangar02
AMD( 168,  184, 0,                               DIR_N), //  06 Taxi to right outside Hangar03
AMD( 168,   40, 0,                               DIR_N), //  07 Taxi to right outside Hangar04
	/* Airplane Terminals */
AMD( 152,  168, AMED_EXACTPOS,                   DIR_NW), //  08 Terminal 01
AMD( 136,  168, AMED_EXACTPOS,                   DIR_NW), //  09 Terminal 02
AMD( 120,  168, AMED_EXACTPOS,                   DIR_NW), //  10 Terminal 03
AMD( 104,  168, AMED_EXACTPOS,                   DIR_NW), //  11 Terminal 04
AMD(  88,  168, AMED_EXACTPOS,                   DIR_NW), //  12 Terminal 05
AMD(  72,  168, AMED_EXACTPOS,                   DIR_W), //  13 Terminal 06
AMD(  72,  152, AMED_EXACTPOS,                   DIR_SW), //  14 Terminal 07
AMD(  72,  136, AMED_EXACTPOS,                   DIR_S), //  15 Terminal 08
AMD(  88,  136, AMED_EXACTPOS,                   DIR_SE), //  16 Terminal 09
AMD( 104,  136, AMED_EXACTPOS,                   DIR_SE), //  17 Terminal 10
AMD( 120,  136, AMED_EXACTPOS,                   DIR_SE), //  18 Terminal 11
AMD( 136,  136, AMED_EXACTPOS,                   DIR_SE), //  19 Terminal 12
AMD( 152,  136, AMED_EXACTPOS,                   DIR_SE), //  20 Terminal 13
AMD( 152,   88, AMED_EXACTPOS,                   DIR_NW), //  21 Terminal 14
AMD( 136,   88, AMED_EXACTPOS,                   DIR_NW), //  22 Terminal 15
AMD( 120,   88, AMED_EXACTPOS,                   DIR_NW), //  23 Terminal 16
AMD( 104,   88, AMED_EXACTPOS,                   DIR_NW), //  24 Terminal 17
AMD(  88,   88, AMED_EXACTPOS,                   DIR_NW), //  25 Terminal 18
AMD(  72,   88, AMED_EXACTPOS,                   DIR_W), //  26 Terminal 19
AMD(  72,   72, AMED_EXACTPOS,                   DIR_SW), //  27 Terminal 20
AMD(  72,   56, AMED_EXACTPOS,                   DIR_S), //  28 Terminal 21
AMD(  88,   56, AMED_EXACTPOS,                   DIR_SE), //  29 Terminal 22
AMD( 104,   56, AMED_EXACTPOS,                   DIR_SE), //  30 Terminal 23
AMD( 120,   56, AMED_EXACTPOS,                   DIR_SE), //  31 Terminal 24
AMD( 136,   56, AMED_EXACTPOS,                   DIR_SE), //  32 Terminal 25
AMD( 152,   56, AMED_EXACTPOS,                   DIR_SE), //  33 Terminal 26
	/* Cargo Terminals (Uses Helipads) */
AMD(  24,   72, AMED_EXACTPOS,                   DIR_SW), //  34 Cargo Terminal 01
AMD(  24,   88, AMED_EXACTPOS,                   DIR_SW), //  35 Cargo Terminal 02
AMD(  24,  104, AMED_EXACTPOS,                   DIR_SW), //  36 Cargo Terminal 03
AMD(  24,  120, AMED_EXACTPOS,                   DIR_SW), //  37 Cargo Terminal 04
AMD(  24,  136, AMED_EXACTPOS,                   DIR_SW), //  38 Cargo Terminal 05
AMD(  24,  152, AMED_EXACTPOS,                   DIR_SW), //  39 Cargo Terminal 06
	/* Helicopter Terminals */
AMD(  24,  104, AMED_EXACTPOS,                   DIR_NE), //  40 Helipad 03
AMD(  24,  120, AMED_EXACTPOS,                   DIR_NE), //  41 Helipad 04
	/* Airplane - Holding Pattern */
AMD(-104,  712, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  42 Fly around waiting for a landing spot (south-east)
AMD(-488,  328, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  43 Fly around waiting for a landing spot (north-east)
AMD( 312, -472, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  44 Fly around waiting for a landing spot (north-west)
AMD( 696,  -88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  45 Fly around waiting for a landing spot (south-west)
	/*  Added holding spots */
AMD( 648,  -40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  46 Fly around waiting for a landing spot SW Check Runway 3
AMD( 520,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  47 Fly around waiting for a landing spot SW Check Runway 1
AMD(-440,  280, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  48 Fly around waiting for a landing spot SE Check Runway 2
AMD(-232,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  49 Fly around waiting for a landing spot SE Check Runway 4
	/* Airplane Landing - Runway 01 */
AMD( 344,  216, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  50 Fly to landing position in air
AMD( 136,  216, AMED_HOLD | AMED_LAND,           DIR_N), //  51 Going down for land
AMD(  40,  216, AMED_HOLD | AMED_BRAKE,          DIR_N), //  52 Just landed, brake until end of runway
	/* Airplane Landing - Runway 02 */
AMD(-120,  200, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  53 Fly to landing position in air
AMD(  88,  200, AMED_HOLD | AMED_LAND,           DIR_N), //  54 Going down for land
AMD( 168,  200, AMED_HOLD | AMED_BRAKE,          DIR_N), //  55 Just landed, brake until end of runway
	/* Airplane Landing - Runway 03 */
AMD( 344,    8, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  56 Fly to landing position in air
AMD( 136,    8, AMED_HOLD | AMED_LAND,           DIR_N), //  57 Going down for land
AMD(  40,    8, AMED_HOLD | AMED_BRAKE,          DIR_N), //  58 Just landed, brake until end of runway
	/* Airplane Landing - Runway 04 */
AMD(-120,   24, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  59 Fly to landing position in air
AMD(  88,   24, AMED_HOLD | AMED_LAND,           DIR_N), //  60 Going down for land
AMD( 168,   24, AMED_HOLD | AMED_BRAKE,          DIR_N), //  61 Just landed, brake until end of runway
	/* Airplane Taxi */
AMD(  40,  200, 0,                               DIR_N), //  62 Taxi01
AMD(  40,  168, 0,                               DIR_N), //  63 Taxi02
AMD(  40,  152, 0,                               DIR_N), //  64 Taxi03
AMD(  40,  136, 0,                               DIR_N), //  65 Taxi04
AMD(  40,  120, 0,                               DIR_N), //  66 Taxi05
AMD(  40,  104, 0,                               DIR_N), //  67 Taxi06
AMD(  56,  104, 0,                               DIR_N), //  68 Taxi07
AMD(  72,  104, 0,                               DIR_N), //  69 Taxi08
AMD(  88,  104, 0,                               DIR_N), //  70 Taxi09
AMD( 104,  104, 0,                               DIR_N), //  71 Taxi10
AMD( 120,  104, 0,                               DIR_N), //  72 Taxi11
AMD( 136,  104, 0,                               DIR_N), //  73 Taxi12
AMD( 152,  104, 0,                               DIR_N), //  74 Taxi13
AMD( 152,  120, 0,                               DIR_N), //  75 Taxi14
AMD( 136,  120, 0,                               DIR_N), //  76 Taxi15
AMD( 120,  120, 0,                               DIR_N), //  77 Taxi16
AMD( 104,  120, 0,                               DIR_N), //  78 Taxi17
AMD(  88,  120, 0,                               DIR_N), //  79 Taxi18
AMD(  72,  120, 0,                               DIR_N), //  80 Taxi19
AMD(  56,  120, 0,                               DIR_N), //  81 Taxi20
AMD(  56,  136, 0,                               DIR_N), //  82 Taxi21
AMD(  56,  152, 0,                               DIR_N), //  83 Taxi22
AMD(  56,  168, 0,                               DIR_N), //  84 Taxi23
AMD(  56,  184, 0,                               DIR_N), //  85 Taxi24
AMD( 152,  184, 0,                               DIR_N), //  86 Taxi25
AMD( 136,  184, 0,                               DIR_N), //  87 Taxi26
AMD( 120,  184, 0,                               DIR_N), //  88 Taxi27
AMD( 104,  184, 0,                               DIR_N), //  89 Taxi28
AMD(  88,  184, 0,                               DIR_N), //  90 Taxi29
AMD(  72,  184, 0,                               DIR_N), //  91 Taxi30
AMD(  40,   24, 0,                               DIR_N), //  92 Taxi31
AMD(  40,   56, 0,                               DIR_N), //  93 Taxi32
AMD(  40,   72, 0,                               DIR_N), //  94 Taxi33
AMD(  40,   88, 0,                               DIR_N), //  95 Taxi34
AMD(  56,   88, 0,                               DIR_N), //  96 Taxi35
AMD(  56,   72, 0,                               DIR_N), //  97 Taxi36
AMD(  56,   56, 0,                               DIR_N), //  98 Taxi37
AMD(  56,   40, 0,                               DIR_N), //  99 Taxi38
AMD( 152,   40, 0,                               DIR_N), // 100 Taxi39
AMD( 136,   40, 0,                               DIR_N), // 101 Taxi40
AMD( 120,   40, 0,                               DIR_N), // 102 Taxi41
AMD( 104,   40, 0,                               DIR_N), // 103 Taxi42
AMD(  88,   40, 0,                               DIR_N), // 104 Taxi43
AMD(  72,   40, 0,                               DIR_N), // 105 Taxi44
	/* Airplane Takeoff - Runway02 */
AMD(  56,  200, 0,                               DIR_N), // 106 On Runway 02, prepare for takeoff
AMD(  88,  200, AMED_NOSPDCLAMP,                 DIR_N), // 107 Release control of runway, for smoother movement
AMD( 152,  200, AMED_NOSPDCLAMP,                 DIR_N), // 108 Airborne point - Takeoff
AMD( 200,  200, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 109 End Take off    Airplane leaves Airport Control
	/* Airplane Takeoff - Runway04 */
AMD(  56,   24, 0,                               DIR_N), // 110 On Runway 04, prepare for takeoff
AMD(  88,   24, AMED_NOSPDCLAMP,                 DIR_N), // 111 Release control of runway, for smoother movement
AMD( 152,   24, AMED_NOSPDCLAMP,                 DIR_N), // 112 Airborne point - Takeoff
AMD( 200,   24, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 113 End Take off    Airplane leaves Airport Control
	/* Helicopter Entry. Helicopters need a seperate entry or it will clear flags for airplanes causing then to run into each other.*/
AMD( 120,  104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 114 Helicopter Entry
	/* Helicopter - Holding Pattern */
AMD( 120,  104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 115 Bufferspace before helipad
AMD( 120,  104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 116 Bufferspace before helipad
	// Helicopter Positioning
AMD(  16,  104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 117 Get in position for Helipad3
AMD(  16,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 118 Get in position for Helipad4
	// Helicopter Landing at Terminals
AMD(  24,  104, AMED_HELI_LOWER,                 DIR_N), // 119 Land at Helipad03
AMD(  24,  120, AMED_HELI_LOWER,                 DIR_N), // 120 Land at Helipad04
	/* Helicopter Hangar 01 */
AMD(  40,  192, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 121 Go to position for Hangar01
AMD(  40,  184, AMED_HELI_LOWER,                 DIR_N), // 122 Land at HANG01_block to go to hangar 01
	/* Helicopter Takeoff */
AMD(  40,  184, AMED_HELI_RAISE,                 DIR_N), // 123 Takeoff Hangar01
AMD(  24,  104, AMED_HELI_RAISE,                 DIR_N), // 124 Takeoff Helipad3
AMD(  24,  120, AMED_HELI_RAISE,                 DIR_N), // 125 Takeoff Helipad4
	// Aborted Landing for Runway 1 and 3
AMD(   8,  216, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 126 Runway 1
AMD(   8,    8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 127 Runway 3
	// Helicopter Takeoff Hangars
AMD(  40,  40, AMED_HELI_RAISE,                  DIR_N), // 128 Takeoff Hangar02
AMD( 168, 184, AMED_HELI_RAISE,                  DIR_N), // 129 Takeoff Hangar03
AMD( 168,  40, AMED_HELI_RAISE,                  DIR_N), // 130 Takeoff Hangar04
};

/** Schiphol Airport - 3 hangars, 6 runways, 12 terminals, 2 helipad, 4 Cargo. */
static const AirportMovingData _airport_moving_data_schiphol[107] = {
	/* Hangars and Outside Hangars */
AMD(  56,   72, AMED_EXACTPOS,                   DIR_SW), //  00 In Hangar 01
AMD( 184,  200, AMED_EXACTPOS,                   DIR_NW), //  01 In Hangar 02
AMD(  88,  248, AMED_EXACTPOS,                   DIR_NW), //  02 In Hangar 03
AMD(  72,   72, 0,                               DIR_N), //  03 Taxi to right outside Hangar01
AMD( 184,  184, 0,                               DIR_N), //  04 Taxi to right outside Hangar02
AMD(  88,  232, 0,                               DIR_N), //  05 Taxi to right outside Hangar03
	/* Airplane Terminals */
AMD( 104,  104, AMED_EXACTPOS,                   DIR_SW), //  06 Terminal 01
AMD( 104,  120, AMED_EXACTPOS,                   DIR_SW), //  07 Terminal 02
AMD( 104,  136, AMED_EXACTPOS,                   DIR_SW), //  08 Terminal 03
AMD( 104,  152, AMED_EXACTPOS,                   DIR_W), //  09 Terminal 04
AMD( 120,  152, AMED_EXACTPOS,                   DIR_NW), //  10 Terminal 05
AMD( 136,  152, AMED_EXACTPOS,                   DIR_NW), //  11 Terminal 06
AMD( 152,  152, AMED_EXACTPOS,                   DIR_NW), //  12 Terminal 07
AMD( 168,  152, AMED_EXACTPOS,                   DIR_N), //  13 Terminal 08
AMD( 168,  136, AMED_EXACTPOS,                   DIR_NE), //  14 Terminal 09
AMD( 168,  120, AMED_EXACTPOS,                   DIR_NE), //  15 Terminal 10
AMD( 168,  104, AMED_EXACTPOS,                   DIR_NE), //  16 Terminal 11
AMD( 168,   88, AMED_EXACTPOS,                   DIR_NE), //  17 Terminal 12
	/* Cargo Terminals (Uses Helipads) */
AMD( 104,  248, AMED_EXACTPOS,                   DIR_NW), //  18 Cargo Terminal 01
AMD( 120,  248, AMED_EXACTPOS,                   DIR_NW), //  19 Cargo Terminal 02
AMD( 136,  248, AMED_EXACTPOS,                   DIR_NW), //  20 Cargo Terminal 03
AMD( 152,  248, AMED_EXACTPOS,                   DIR_NW), //  21 Cargo Terminal 04
	/* Helicopter Terminals */
AMD( 104,  248, AMED_EXACTPOS,                   DIR_SE), //  22 Helipad 01
AMD( 120,  248, AMED_EXACTPOS,                   DIR_SE), //  23 Helipad 02
	/* Airplane - Holding Pattern */
AMD( 136,  648, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  24 Fly around waiting for a landing spot (south-east)
AMD(-408,  104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  25 Fly around waiting for a landing spot (north-east)
AMD( 312, -616, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  26 Fly around waiting for a landing spot (north-west)
AMD( 856,  -72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  27 Fly around waiting for a landing spot (south-west)
	/*  Added holding spots */
AMD( 808,  -24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  28 Fly around waiting for a landing spot SW Check Runway 3
AMD( 104,  616, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  29 Fly around waiting for a landing spot SW Check Runway 2
AMD(-360,   56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  30 Fly around waiting for a landing spot SE Check Runway 5
AMD(-232,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  31 Unused
	/* Airplane Landing - Runway 02 */
AMD(  72,  392, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  32 Fly to landing position in air
AMD(  72,  184, AMED_HOLD | AMED_LAND,           DIR_N), //  33 Going down for land
AMD(  72,   88, AMED_HOLD | AMED_BRAKE,          DIR_N), //  34 Just landed, brake until end of runway
	/* Airplane Landing - Runway 03 */
AMD( 344,   40, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  35 Fly to landing position in air
AMD( 136,   40, AMED_HOLD | AMED_LAND,           DIR_N), //  36 Going down for land
AMD(  40,   40, AMED_HOLD | AMED_BRAKE,          DIR_N), //  37 Just landed, brake until end of runway
	/* Airplane Landing - Runway 05 */
AMD(-168,    8, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  38 Fly to landing position in air
AMD(  24,    8, AMED_HOLD | AMED_LAND,           DIR_N), //  39 Going down for land
AMD( 120,    8, AMED_HOLD | AMED_BRAKE,          DIR_N), //  40 Just landed, brake until end of runway
	/* Airplane Taxi */
AMD( 120,   24, 0,                               DIR_N), //  41 Taxi02
AMD( 104,   24, 0,                               DIR_N), //  42 Taxi03
AMD(  88,   24, 0,                               DIR_N), //  43 Taxi04
AMD(  88,   40, 0,                               DIR_N), //  44 Taxi05
AMD(  40,   56, 0,                               DIR_N), //  45 Taxi06
AMD(  56,   56, 0,                               DIR_N), //  46 Taxi07
AMD(  72,   56, 0,                               DIR_N), //  47 Taxi08
AMD(  88,   56, 0,                               DIR_N), //  48 Taxi09
AMD(  88,   72, 0,                               DIR_N), //  49 Taxi10
AMD(  88,   88, 0,                               DIR_N), //  50 Taxi12
AMD(  88,  104, 0,                               DIR_N), //  51 Taxi13
AMD(  88,  120, 0,                               DIR_N), //  52 Taxi14
AMD(  88,  136, 0,                               DIR_N), //  53 Taxi15
AMD(  88,  152, 0,                               DIR_N), //  54 Taxi16
AMD(  88,  168, 0,                               DIR_N), //  55 Taxi17
AMD( 104,  168, 0,                               DIR_N), //  56 Taxi18
AMD( 120,  168, 0,                               DIR_N), //  57 Taxi19
AMD( 136,  168, 0,                               DIR_N), //  58 Taxi20
AMD( 152,  168, 0,                               DIR_N), //  59 Taxi21
AMD( 168,  168, 0,                               DIR_N), //  60 Taxi22
AMD( 184,  168, 0,                               DIR_N), //  61 Taxi23
AMD( 184,  152, 0,                               DIR_N), //  62 Taxi24
AMD( 184,  136, 0,                               DIR_N), //  63 Taxi25
AMD( 184,  120, 0,                               DIR_N), //  64 Taxi26
AMD( 184,  104, 0,                               DIR_N), //  65 Taxi27
AMD( 184,   88, 0,                               DIR_N), //  66 Taxi28
AMD( 184,   72, 0,                               DIR_N), //  67 Taxi29
AMD( 184,   56, 0,                               DIR_N), //  68 Taxi30
AMD(  88,  184, 0,                               DIR_N), //  69 Runway01
AMD(  88,  200, 0,                               DIR_N), //  70 Taxi32
AMD( 104,  232, 0,                               DIR_N), //  71 Taxi33
AMD( 120,  232, 0,                               DIR_N), //  72 Taxi34
AMD( 136,  232, 0,                               DIR_N), //  73 Taxi35
AMD( 152,  232, 0,                               DIR_N), //  74 Taxi36
AMD( 168,  232, 0,                               DIR_N), //  75 Taxi37
AMD(  88,  216, 0,                               DIR_N), //  76 Runway06
	/* Airplane Takeoff - Runway01 */
AMD( 168,  184, AMED_EXACTPOS,                   DIR_NE), //  77 On Runway 01, Wait until runway free
AMD( 168,  184, 0,                               DIR_N), //  78 On Runway 01, prepare for takeoff
AMD( 152,  184, AMED_NOSPDCLAMP,                 DIR_N), //  79 Release control of runway, for smoother movement
AMD(  72,  184, AMED_NOSPDCLAMP,                 DIR_N), //  80 Airborne point - Takeoff
AMD(  24,  184, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), //  81 End Take off    Airplane leaves Airport Control
	/* Airplane Takeoff - Runway04 */
AMD( 200,   56, 0,                               DIR_N), //  82 On Runway 04, prepare for takeoff
AMD( 200,   72, AMED_NOSPDCLAMP,                 DIR_N), //  83 Release control of runway, for smoother movement
AMD( 200,  152, AMED_NOSPDCLAMP,                 DIR_N), //  84 Airborne point - Takeoff
AMD( 200,  200, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), //  85 End Take off    Airplane leaves Airport Control
	/* Airplane Takeoff - Runway06 */
AMD( 168,  216, AMED_EXACTPOS,                   DIR_NE), //  86 On Runway 01, Wait until runway free
AMD( 168,  216, 0,                               DIR_N), //  87 On Runway 01, prepare for takeoff
AMD( 152,  216, AMED_NOSPDCLAMP,                 DIR_N), //  88 Release control of runway, for smoother movement
AMD(  88,  216, AMED_NOSPDCLAMP,                 DIR_N), //  89 Airborne point - Takeoff
AMD(  40,  216, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), //  90 End Take off    Airplane leaves Airport Control
	// Aborted Landing for Runway 1 and 3
AMD(  72,   56, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), //  91 Runway 2
AMD(   8,   40, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), //  92 Runway 3
AMD( 152,    8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), //  93 Runway 5
	/* Helicopter Entry. Helicopters need a seperate entry or it will clear flags for airplanes causing then to run into each other.*/
AMD( 104,  280, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  94 Helicopter Entry
	/* Helicopter - Holding Pattern */
AMD( 104,  280, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  95 Bufferspace before helipad
AMD( 104,  280, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  96 Bufferspace before helipad
	// Helicopter Positioning
AMD( 104,  240, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  97 Get in position for Helipad1
AMD( 120,  240, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  98 Get in position for Helipad2
	// Helicopter Landing at Terminals
AMD( 104,  248, AMED_HELI_LOWER,                 DIR_N), //  99 Land at Helipad01
AMD( 120,  248, AMED_HELI_LOWER,                 DIR_N), // 100 Land at Helipad02
	/* Helicopter Hangar 01 */
AMD(  88,  224, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 101 Go to position for Hangar01
AMD(  88,  232, AMED_HELI_LOWER,                 DIR_N), // 102 Land at HANG01_block to go to hangar 01
	/* Helicopter Takeoff */
AMD( 184,  184, AMED_HELI_RAISE,                 DIR_N), // 103 Takeoff Hangar02
AMD(  88,  232, AMED_HELI_RAISE,                 DIR_N), // 104 Takeoff Hangar03
AMD( 104,  248, AMED_HELI_RAISE,                 DIR_N), // 105 Takeoff Helipad1
AMD( 120,  248, AMED_HELI_RAISE,                 DIR_N), // 106 Takeoff Helipad2
};

/** Heliport (heliport). */
static const AirportMovingData _airport_moving_data_heliport[9] = {
AMD(   5,    9, AMED_EXACTPOS,                   DIR_NE), // 0 - At heliport terminal
AMD(   2,    9, AMED_HELI_RAISE,                 DIR_N), // 1 - Take off (play sound)
AMD(  -3,    9, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 2 - In position above landing spot helicopter
AMD(  -3,    9, AMED_HELI_LOWER,                 DIR_N), // 3 - Land
AMD(   2,    9, 0,                               DIR_N), // 4 - Goto terminal on ground
AMD( -31,   59, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 5 - Circle #1 (north-east)
AMD( -31,  -49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 6 - Circle #2 (north-west)
AMD(  49,  -49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 7 - Circle #3 (south-west)
AMD(  70,    9, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 8 - Circle #4 (south)
};

/** HeliDepot 2x2 (heliport). */
static const AirportMovingData _airport_moving_data_helidepot[16] = {
AMD(  24,    8, AMED_EXACTPOS,                   DIR_NE), // 0 - At depot
AMD(  24,   24, 0,                               DIR_N), // 1 Taxi to right outside depot
	// Helipad Terminals
AMD(   8,   24, AMED_EXACTPOS,                   DIR_NW), // 2 - on helipad1
	// Holding Pattern
AMD( -15,  -15, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 3 - Circle #1 (north-east)
AMD( -15,  -49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 4 - Circle #2 (north-west)
AMD(  49,  -49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 5 - Circle #3 (south-west)
AMD(  49,  -15, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 6 - Circle #4 (south-east)
	// Go to Landing
AMD(   8,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 7 Flying
	// Select Helipad to land at
AMD(   8,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_NW), // 8 - PreHelipad
AMD(   8,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_NW), // 9 - Helipad
	// Positioning
AMD(   8,   16, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_NW), // 10 - Positioning for Helipad1
	// Land Helipad01
AMD(   8,   16, AMED_HELI_LOWER,                 DIR_NW), // 11 - Land
	// Takeoff
AMD(   8,   24, AMED_HELI_RAISE,                 DIR_N), // 12 - Take off (play sound)
	// Hangar Landing
AMD(  32,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_NW), // 13 Air to above hangar area
AMD(  32,   24, AMED_HELI_LOWER,                 DIR_NW), // 14 Taxi to right outside depot
	// Hangar Takeoff
AMD(  24,   24, AMED_HELI_RAISE,                 DIR_N), // 15 Takeoff right outside depot
};

/** HeliDepot 2x2 (heliport). */
static const AirportMovingData _airport_moving_data_helistation[33] = {
AMD(   8,    3, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar 1
AMD(   8,   22, 0,                               DIR_N), // 01 outside hangar 1
AMD( 116,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 02 Fly to landing position in air
AMD(  14,   22, AMED_HELI_RAISE,                 DIR_N), // 03 Helitakeoff outside hangar1(play sound)
AMD(  24,   22, 0,                               DIR_N), // 04 taxiing
AMD(  40,   22, 0,                               DIR_N), // 05 taxiing
AMD(  40,    8, AMED_EXACTPOS,                   DIR_NE), // 06 Helipad 1
AMD(  56,    8, AMED_EXACTPOS,                   DIR_NE), // 07 Helipad 2
AMD(  56,   24, AMED_EXACTPOS,                   DIR_NE), // 08 Helipad 3
AMD(  40,    8, AMED_EXACTPOS,                   DIR_N), // 09 pre-helitakeoff helipad 1
AMD(  56,    8, AMED_EXACTPOS,                   DIR_N), // 10 pre-helitakeoff helipad 2
AMD(  56,   24, AMED_EXACTPOS,                   DIR_N), // 11 pre-helitakeoff helipad 3
AMD(  32,    8, AMED_HELI_RAISE,                 DIR_N), // 12 Takeoff Helipad1
AMD(  48,    8, AMED_HELI_RAISE,                 DIR_N), // 13 Takeoff Helipad2
AMD(  48,   24, AMED_HELI_RAISE,                 DIR_N), // 14 Takeoff Helipad3
AMD(  84,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 15 Bufferspace before helipad
AMD(  68,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 16 Bufferspace before helipad
AMD(  32,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 17 Get in position for Helipad1
AMD(  48,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 18 Get in position for Helipad2
AMD(  48,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_NE), // 19 Get in position for Helipad3
AMD(  40,    8, AMED_HELI_LOWER,                 DIR_N), // 20 Land at Helipad1
AMD(  48,    8, AMED_HELI_LOWER,                 DIR_N), // 21 Land at Helipad2
AMD(  48,   24, AMED_HELI_LOWER,                 DIR_N), // 22 Land at Helipad3
AMD(   0,   22, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 23 Go to position for Hangarentrance in air
AMD(   0,   22, AMED_HELI_LOWER,                 DIR_N), // 24 Land in front of hangar
AMD( 148,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 25 Fly around waiting for a landing spot (south-east)
AMD( 148,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 26 Fly around waiting for a landing spot (south-west)
AMD( 132,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 27 Fly around waiting for a landing spot (south-west)
AMD( 100,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 28 Fly around waiting for a landing spot (north-east)
AMD(  84,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 29 Fly around waiting for a landing spot (south-east)
AMD(  84,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 30 Fly around waiting for a landing spot (south-west)
AMD( 100,  -24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 31 Fly around waiting for a landing spot (north-west)
AMD( 132,  -24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 32 Fly around waiting for a landing spot (north-east)
};

/** Oilrig. */
static const AirportMovingData _airport_moving_data_oilrig[9] = {
AMD(  31,    9, AMED_EXACTPOS,                   DIR_NE), // 0 - At oilrig terminal
AMD(  28,    9, AMED_HELI_RAISE,                 DIR_N), // 1 - Take off (play sound)
AMD(  23,    9, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 2 - In position above landing spot helicopter
AMD(  23,    9, AMED_HELI_LOWER,                 DIR_N), // 3 - Land
AMD(  28,    9, 0,                               DIR_N), // 4 - Goto terminal on ground
AMD( -31,   69, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 5 - circle #1 (north-east)
AMD( -31,  -49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 6 - circle #2 (north-west)
AMD(  69,  -49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 7 - circle #3 (south-west)
AMD(  69,    9, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 8 - circle #4 (south)
};

#undef AMD

///////////////////////////////////////////////////////////////////////
/////**********Movement Machine on Airports*********************///////
static const byte _airport_entries_dummy[] = { 0, 1, 2, 3 };
static const byte _airport_helientry_dummy[] = { 0, 1, 2, 3 };
static const AirportFTAbuildup _airport_fta_dummy[] = {
	{ 0, 0, 0, 0, 3 },
	{ 1, 0, 0, 0, 0 },
	{ 2, 0, 0, 0, 1 },
	{ 3, 0, 0, 0, 2 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/* First element of terminals array tells us how many depots there are (to know size of array)
 * this may be changed later when airports are moved to external file  */
/// Country
static const HangarTileTable _airport_depots_country[] = { { { 3, 0 }, DIR_SE, 0 } };
static const TerminalTileTable _airport_terminals_country[] = { { { 0, 1 }, TERM01_block },{ { 1, 1 }, TERM02_block },{ { 2, 1 }, TERM03_block } };
static const RunwayTerminalTable _airport_runways_country[] = { { TERM01_block | TERM02_block | TERM03_block, RUNW01_block, 0 } };
static const byte _airport_terminal_country[] = { 1, 3 };
static const byte _airport_entries_country[] = { 6, 5, 8, 7 };
static const byte _airport_helientry_country[] = { 6, 5, 8, 7 };
static const AirportFTAbuildup _airport_fta_country[] = {
	{ 0, HANGAR, 0, 0, 1 },{ 0, 0, 0, 0, 1 },{ 0, HELITAKEOFF, 0, 0, 1 },{ 0, TERM01, 0, 0, 1 },{ 0, TERM02, 0, 0, 1 },{ 0, TERM03, 0, 0, 1 },
	{ 1,    255, RUNW01_block | HANG01_block, 0, 0 },{ 1, HANGAR, 0, 0, 0 },{ 1, TERM01, 0, 0, 2 },{ 1, TERM02, RUNW01_block, 0, 15 },{ 1, TERM03, RUNW01_block, 0, 15 },{ 1, HELITAKEOFF, 0, 0, 19 },{ 1, 0, 0, 0, 15 },
	// Airplane Terminals
	{ 2, TERM01, TERM01_block, 0, 14 },{ 2, HELITAKEOFF, 0, 0, 20 },{ 2, HANGAR, HANG01_block, 0, 1 },{ 2, 0, 0, 0, 14 },
	{ 3, TERM02, TERM02_block, 0, 13 },{ 3, HELITAKEOFF, 0, 0, 21 },{ 3, HANGAR, HANG01_block | RUNW01_block, 0, 13 },{ 3, 0, 0, 0, 13 },
	{ 4, TERM03, TERM03_block, 0, 12 },{ 4, HELITAKEOFF, 0, 0, 22 },{ 4, HANGAR, HANG01_block | RUNW01_block, 0, 12 },{ 4, 0, 0, 0, 12 },
	// Holding Pattern
	{ 5, 0, 0, 0,  6 },
	{ 6, 0, 0, 0,  7 },
	{ 7, 0, 0, 0,  8 },
	{ 8, 0, 0, 0, 26 },
	/* landing */
	{  9, PRELANDING, RUNW01_block, 0, 10 }, { 9, LANDING, 0, 0, 10 },
	{ 10, LANDING, RUNW01_block, 0,  11 },{ 10, FLYING, 0, 0, 18 },{ 10, 0, 0, 0, 11 },
	{ 11, ENDLANDING, RUNW01_block, 0, 12 },{ 11, 0, 0, 0, 12 },{ 11, TERM03, 0, 0, 0 },{ 11, TERM02, 0, 0, 0 },{ 11, TERM01, 0, 0, 0 },
	// Taxi
	{ 12, 255, RUNW01_block, 0, 0 },{ 12, TERM03, 0, 0, 4 },{ 12, TERM02, 0, 0, 13 },{ 12, TERM01, 0, 0, 13 },{ 12, HANGAR, 0, 0, 13 },{ 12, TAKEOFF, 0, 0, 16 },
	{ 13, 255, RUNW01_block, 0, 0 },{ 13, TERM02, 0, 0, 3 },{ 13, TERM01, 0, 0, 14 },{ 13, HANGAR, 0, 0, 14 },{ 13, 0, 0, 0, 12 },
	{ 14, 255, RUNW01_block, 0, 0 },{ 14, TERM01, 0, 0, 2 },{ 14, HANGAR, 0, 0, 15 },{ 14, 0, 0, 0, 13 },
	{ 15, 255, RUNW01_block, 0, 0 },{ 15, HANGAR, 0, 0, 1 },{ 15, 0, 0, 0, 14 },
	/* takeoff */
	{ 16, TAKEOFF, RUNW01_block, 0,  17 },
	{ 17, STARTTAKEOFF, 0, 0, 18 },
	{ 18, ENDTAKEOFF, 0, 0, 0 },     //End airplane control
	// Helicopter Takeoff
	{ 19, HELITAKEOFF, 0, 0, 0 },
	{ 20, HELITAKEOFF, 0, 0, 0 },
	{ 21, HELITAKEOFF, 0, 0, 0 },
	{ 22, HELITAKEOFF, 0, 0, 0 },
	// Helicopter Landing
	{ 23, HELILANDING, RUNW01_block | HANG01_block, 0, 24 },
	{ 24, HELIENDLANDING, RUNW01_block | HANG01_block, 0, 24 }, { 24, FLYING, 0, 0, 5 }, { 24, TERM01, 0, 0, 1 },{ 24, TERM02, 0, 0, 1 },{ 24, TERM03, 0, 0, 1 }, { 24, 0, 0, 0, 1 },
	// Helicopter Entry
	{ 25, FLYING, 0, 0, 23 },{ 25, HELILANDING, 0, 0,  23 },{ 25, 0, 0, 0, 23 },
	// Plane PreLanding
	{ 26, FLYING, 0, 0, 5 },{ 26, PRELANDING, 0, 0, 9 }, { 26, HELILANDING, 0, 0, 23 }, { 26, 0, 0, 0, 5 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Commuter
static const HangarTileTable _airport_depots_commuter[] = { { { 4, 0 }, DIR_SE, 0 } };
static const TerminalTileTable _airport_terminals_commuter[] = { { { 3, 2 }, TERM01_block },{ { 2, 2 }, TERM02_block },{ { 1, 2 }, TERM03_block },{ { 3, 0 }, CARG01_block },{ { 2, 0 }, CARG02_block } };
static const RunwayTerminalTable _airport_runways_commuter[] = { { TERM01_block | TERM02_block | TERM03_block | CARG01_block | CARG02_block, RUNW01_block, 0 } };
static const byte _airport_terminal_commuter[] = { 1, 3 };
static const byte _airport_entries_commuter[] = { 10,  9, 12, 11 };  //{ 12, 11, 10,  9 };
static const byte _airport_helientry_commuter[] = { 10,  9, 12, 11 };
static const AirportFTAbuildup _airport_fta_commuter[] = {
	// Airplane & Helicopter Hangars
	{ 0, HANGAR, 0, 0, 1 },{ 0, 0, 0, 0, 1 },{ 0, TERM01, 0, 0, 1 },{ 0, TERM02, 0, 0, 1 },{ 0, TERM03, 0, 0, 1 },{ 0, CARG01, 0, 0, 1 },{ 0, CARG02, 0, 0, 1 },{ 0, HELI01, 0, 0, 1 },{ 0, HELI02, 0, 0, 1 },{ 0, HELITAKEOFF, 0, 0, 1 },{ 0, TAKEOFF, 0, 0, 1 },
	{ 1, 255, HANG01_block, 0, 0 },{ 1, HANGAR, 0, 0, 0 },{ 1, TAKEOFF, 0, 0, 25 },{ 1, TERM01, 0, TAXI05_block, 24 },{ 1, TERM02, 0, TAXI05_block | TAXI04_block, 24 },{ 1, TERM03, 0, TAXI05_block | TAXI04_block | TAXI03_block, 24 },{ 1, CARG01, 0, TAXI05_block, 24 },{ 1, CARG02, 0, TAXI05_block | TAXI04_block, 24 },{ 1, HELI01, 0, TAXI05_block, 24 },{ 1, HELI02, 0, TAXI05_block | TAXI04_block, 24 },{ 1, HELITAKEOFF, 0, 0, 36 },{ 1, 0, 0, 0, 0 },
	// Airplane Terminals
	{ 2, TERM01, TERM01_block, 0, 24 },{ 2, 0, 0, 0, 24 },
	{ 3, TERM02, TERM02_block, 0, 23 },{ 3, 0, 0, 0, 23 },
	{ 4, TERM03, TERM03_block, 0, 22 },{ 4, 0, 0, 0, 22 },
	// Cargo Terminals
	{ 5, CARG01, CARG01_block, 0, 24 },{ 5, 0, 0, 0, 24 },
	{ 6, CARG02, CARG02_block, 0, 23 },{ 6, 0, 0, 0, 23 },
	// Helicopter Terminals
	{ 7, HELI01, HELI01_block, 0, 24 },{ 7, HELITAKEOFF, 0, 0, 34 },{ 7, 0, 0, 0, 24 },
	{ 8, HELI02, HELI02_block, 0, 23 },{ 8, HELITAKEOFF, 0, 0, 35 },{ 8, 0, 0, 0, 23 },
	// Airplane Holding Pattern
	{ 9, 0, 0, 0, 10 },
	{ 10, 0, 0, 0, 11 },
	{ 11, 0, 0, 0, 12 },
	{ 12, 0, 0, 0, 37 },
	// Helicopter Holding Pattern
	{ 13, HELILANDING, 0, 0, 14 },{ 13, 0, 0, 0, 14 },
	{ 14, HELIENDLANDING, 0, 0, 14 }, { 14, FLYING, 0, 0, 9 }, { 14, HELI01, 0, 0, 30 },{ 14, HELI02, 0, 0, 31 },{ 14, HANGAR, 0, 0, 15 }, { 14, 0, 0, 0, 15},
	// Helicopter in air to Hangar
	{ 15, 0, 0, 0, 16 },
	{ 16, 0, HANG01_block, 0,  1 },
	// Airplane Landing Runway 01
	{ 17, PRELANDING, RUNW01_block, TAXI06_block, 18 },{ 17, LANDING, 0, 0, 18 },
	{ 18, LANDING, RUNW01_block, 0, 19 },{ 18, FLYING, 0, 0, 29 },{ 18, 0, 0, 0, 19 },
	{ 19, ENDLANDING, 0, TAXI07_block, 20 },{ 19, 0, 0, 0, 20 },{ 19, TERM03, 0, 0, 0 },{ 19, TERM02, 0, 0, 0 },{ 19, TERM01, 0, 0, 0 },{ 19, CARG02, 0, 0, 0 },{ 19, CARG01, 0, 0, 0 },
	// Main Airport Flow
	{ 20,      0, 0, TAXI01_block, 21 },
	{ 21,      0, 0, TAXI02_block, 22 },
	{ 22, BACKUP, 0, TAXI03_block, 23 },{ 22, TERM03, 0, 0,  4 },{ 22, 0, 0, 0,  23 },
	{ 23, BACKUP, 0, TAXI04_block, 24 },{ 23, TERM02, 0, 0,  3 },{ 23, CARG02, 0, 0,  6 },{ 23, HELI02, 0, 0,  8 },{ 23, TERM03, 0, 0, 22 },{ 23, 0, 0, 0, 24 },
	{ 24, BACKUP, 0, TAXI05_block,  1 },{ 24, TERM01, 0, 0,  2 },{ 24, CARG01, 0, 0,  5 },{ 24, CARG02, 0, 0, 23 },{ 24, HELI01, 0, 0,  7 },{ 24, HELI02, 0, 0, 23 },{ 24, TERM02, 0, 0, 23 },{ 24, TERM03, 0, 0, 23 },{ 24, 0, 0, 0, 1 },
	{ 25,      0, 0, TAXI06_block, 26 },
	// Airplane Takeoff - Runway01
	{ 26, TAKEOFF, RUNW01_block, 0, 27 },
	{ 27, 0, 0, 0, 28 },
	{ 28, STARTTAKEOFF, 0, 0, 29 },
	{ 29, ENDTAKEOFF, 0, 0, 0 },{ 29, 0, 0, 0, 10 },  // End Airport Control of planes.
	// Helicopter position for Helipads
	{ 30, 0, 0, 0, 32 },
	{ 31, 0, 0, 0, 33 },
	// Helicopter Land at Helipads
	{ 32, HELI01, 0, 0, 7 },
	{ 33, HELI02, 0, 0, 8 },
	// Helicopter Takeoff from Helipads
	{ 34, HELITAKEOFF, 0, 0, 0 }, // End Airport Control of helicopters.
	{ 35, HELITAKEOFF, 0, 0, 0 }, // End Airport Control of helicopters.
	// Helicopter takeoff from Hangar
	{ 36, HELITAKEOFF, 0, 0, 0 }, // End Airport Control of helicopters.
	// Runway01 Entry
	{ 37, FLYING, 0, 0,  9 },{ 37, PRELANDING, 0, 0, 17 }, { 37, HELILANDING, 0, 0, 13 }, { 37, 0, 0, 0,  9 },
	// Runway01 Entry
	{ 38, FLYING, 0, 0, 13 },{ 38, HELILANDING, 0, 0, 13 },{ 38, 0, 0, 0, 13 },
{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// City
static const HangarTileTable _airport_depots_city[] = { { { 5, 0 }, DIR_SE, 0 } };
static const TerminalTileTable _airport_terminals_city[] = { { { 1, 2 }, TERM01_block },{ { 3, 1 }, TERM02_block },{ { 2, 0 }, TERM03_block } };
static const RunwayTerminalTable _airport_runways_city[] = { { TERM01_block | TERM02_block | TERM03_block, RUNW01_block, 0 } };
static const byte _airport_terminal_city[] = { 1, 3 };
static const byte _airport_entries_city[] = { 19, 18, 21, 20 };
static const byte _airport_helientry_city[] = { 19, 18, 21, 20 };
static const AirportFTAbuildup _airport_fta_city[] = {
	{ 0, HANGAR, 0, 0, 1 },{ 0, 0, 0, 0, 1 },{ 0, TERM02, 0, 0, 1 },{ 0, TERM03, 0, 0, 1 },{ 0, TERM01, 0, 0, 1 },
	{ 1,    255, HANG01_block, 0, 0 },{ 1, HANGAR, 0, 0, 0 },{ 1, TERM02, 0, TAXI03_block, 6 },{ 1, TERM03, 0, TAXI03_block | TAXI04_block, 6 },{ 1, TERM01, 0, TAXI03_block | TAXI02_block, 6 },{ 1, TAKEOFF, 0, TAXI03_block | TAXI02_block | TAXI05_block, 6 },{ 1, HELITAKEOFF, 0, 0, 22 },{ 1, 0, 0, 0, 6 },
	{ 2, TERM01, TERM01_block, 0, 7 },{ 2, HANGAR, HANG01_block, TAXI02_block | TAXI03_block, 7 },{ 2, HELITAKEOFF, 0, 0, 23 },{ 2, TAKEOFF, 0, TAXI02_block | TAXI05_block, 7 },{ 2, 0, 0, 0, 7 },
	{ 3, TERM02, TERM02_block, 0, 6 },{ 3, HANGAR, HANG01_block, TAXI03_block, 6 },{ 3, TAKEOFF, 0, TAXI03_block | TAXI02_block | TAXI05_block, 6 },{ 3, HELITAKEOFF, 0, 0, 24 },{ 3, 0, 0, 0, 6 },
	{ 4, TERM03, TERM03_block, 0, 5 },{ 4, HANGAR, HANG01_block, TAXI04_block | TAXI03_block, 5 },{ 4, TAKEOFF, 0, TAXI04_block | TAXI03_block | TAXI02_block | TAXI05_block, 5 },{ 4, HELITAKEOFF, 0, 0, 25 },{ 4, 0, 0, 0, 5 },
	{ 5,    255, 0, TAXI04_block, 0 },{ 5, TERM03, 0, 0, 4 },{ 5, 0, 0, 0, 6 },
	{ 6,    255, 0, TAXI03_block, 0 },{ 6, TERM02, 0, 0, 3 },{ 6, TERM03, 0, 0, 5 },{ 6, HANGAR, 0, 0, 1 },{ 6, 0, 0, 0, 7 },
	{ 7,    255, 0, TAXI02_block, 0 },{ 7, TERM01, 0, 0, 2 },{ 7, TAKEOFF, 0, 0, 8 },{ 7, HELITAKEOFF, 0, 0, 22 },{ 7, 0, 0, 0, 6 },
	{ 8, 0, 0, TAXI05_block, 9 },
	{ 9, 0, RUNW01_block, 0, 10 },
	/* takeoff */
	{ 10, TAKEOFF, RUNW01_block, 0, 11 },
	{ 11, STARTTAKEOFF, RUNW01_block, 0, 12 },
	{ 12, ENDTAKEOFF, 0, 0, 0 }, { 12, 0, 0, 0, 18 },
	// Runway01 Entry
	{ 13, FLYING, 0, 0, 18 },{ 13, PRELANDING, 0, 0, 14 },{ 13, HELILANDING, 0, 0, 26 }, { 13, 0, 0, 0, 18 },
	/* landing */
	{ 14, PRELANDING, RUNW01_block, 0, 15 },{ 14, LANDING, 0, 0, 15 },
	{ 15, LANDING, RUNW01_block, 0, 16 },{ 15, FLYING, 0, 0, 12 },{ 15, 0, 0, 0, 16 },
	{ 16, ENDLANDING, 0, TAXI06_block, 17 },{ 16, 0, 0, 0, 17 },{ 16, TERM01, 0, 0, 0 },{ 16, TERM02, 0, 0, 0 },{ 16, TERM03, 0, 0, 0 },
	// Taxi01
	{ 17, 255, 0, TAXI01_block, 7 },{ 17, HANGAR, HANG01_block, TAXI02_block | TAXI03_block, 7 },{ 17, 0, 0, 0, 7 },
	/* In Air */
	{ 18, 0, 0, 0, 19 },
	{ 19, 0, 0, 0, 20 },
	{ 20, 0, 0, 0, 21 },
	{ 21, 0, 0, 0, 13 },
	/* helicopter */
	{ 22, HELITAKEOFF, 0, 0, 0 },
	{ 23, HELITAKEOFF, 0, 0, 0 },
	{ 24, HELITAKEOFF, 0, 0, 0 },
	{ 25, HELITAKEOFF, 0, 0, 0 },
	// Helicopter Landing
	{ 26, HELILANDING, HANG01_block, TAXI04_block | TAXI03_block | TAXI02_block, 27 },
	{ 27, HELIENDLANDING, HANG01_block, TAXI04_block | TAXI03_block | TAXI02_block, 1 }, { 27, FLYING, 0, 0, 18 }, { 27, TERM03, 0, 0, 0 },{ 27, TERM02, 0, 0, 0 },{ 27, TERM01, 0, 0, 0 }, { 27, 0, 0, 0, 1 },
	/* These are to keep the opening screen from crashing. */
	{ 28, FLYING, 0, 0, 26 },{ 28, HELILANDING, 0, 0, 26 },{ 28, 0, 0, 0, 26 },
	{ 29, 0, 0, 0, 18 },
	{ 30, 0, 0, 0, 18 },
	{ 31, 0, 0, 0, 18 },
	{ 32, 0, 0, 0, 18 },
	{ 33, 0, 0, 0, 18 },
	{ 34, 0, 0, 0, 18 },
	{ 35, 0, 0, 0, 18 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Metropolitan
static const HangarTileTable _airport_depots_metropolitan[] = { { { 5, 0 }, DIR_SE, 0 } };
static const TerminalTileTable _airport_terminals_metropolitan[] = { { { 1, 2 }, TERM01_block },{ { 3, 1 }, TERM02_block },{ { 2, 0 }, TERM03_block } };
static const RunwayTerminalTable _airport_runways_metropolitan[] = { { TERM01_block | TERM02_block | TERM03_block, RUNW01_block, 0 } };
static const byte _airport_terminal_metropolitan[] = { 1, 3 };
static const byte _airport_entries_metropolitan[] = { 20, 19, 22, 21 };
static const byte _airport_helientry_metropolitan[] = { 20, 19, 22, 21 };
static const AirportFTAbuildup _airport_fta_metropolitan[] = {
	{ 0, HANGAR, 0, 0, 1 },{ 0, 0, 0, 0, 1 },{ 0, TERM02, 0, 0, 1 },{ 0, TERM03, 0, 0, 1 },{ 0, TERM01, 0, 0, 1 },
	{ 1,    255, HANG01_block, 0, 0 },{ 1, HANGAR, 0, 0, 0 },{ 1, TERM02, 0, TAXI03_block, 6 },{ 1, TERM03, 0, TAXI03_block | TAXI04_block, 6 },{ 1, TERM01, 0, TAXI03_block | TAXI02_block, 6 },{ 1, TAKEOFF, 0, TAXI03_block, 6 },{ 1, HELITAKEOFF, 0, 0, 23 },{ 1, 0, 0, 0, 6 },
	{ 2, TERM01, TERM01_block, 0, 7 },{ 2, HANGAR, HANG01_block, TAXI02_block | TAXI03_block, 7 },{ 2, TAKEOFF, 0, TAXI02_block | TAXI05_block, 7 },{ 2, HELITAKEOFF, 0, 0, 24 },{ 2, 0, 0, 0, 7 },
	{ 3, TERM02, TERM02_block, 0, 6 },{ 3, HANGAR, HANG01_block, TAXI03_block , 6 },{ 3, TAKEOFF, 0, TAXI03_block | TAXI05_block, 6 },{ 3, HELITAKEOFF, 0, 0, 25 },{ 3, 0, 0, 0, 6 },
	{ 4, TERM03, TERM03_block, 0, 5 },{ 4, HANGAR, HANG01_block, TAXI04_block | TAXI03_block, 5 },{ 4, TAKEOFF, 0, TAXI04_block | TAXI03_block | TAXI05_block, 5 },{ 4, HELITAKEOFF, 0, 0, 26 },{ 4, 0, 0, 0, 5 },
	{ 5,    255, 0, TAXI04_block, 0 },{ 5, TERM03, 0, 0, 4 },{ 5, 0, 0, 0, 6 },
	{ 6,    255, 0, TAXI03_block, 0 },{ 6, TERM02, 0, 0, 3 },{ 6, TERM03, 0, 0, 5 },{ 6, HANGAR, 0, 0, 1 },{ 6, TAKEOFF, 0, 0, 8 },{ 6, 0, 0, 0, 7 },
	{ 7,    255, 0, TAXI02_block, 0 },{ 7, TERM01, 0, 0, 2 },{ 7, TAKEOFF, 0, 0, 8 },{ 7, HELITAKEOFF, 0, 0, 23 },{ 7, 0, 0, 0, 6 },
	{ 8, 0, 0, TAXI05_block, 9 },
	{ 9, 0, RUNW02_block, 0, 10 },
	/* takeoff */
	{ 10, TAKEOFF, RUNW02_block, 0, 11 },
	{ 11, STARTTAKEOFF, 0, 0, 12 },
	{ 12, ENDTAKEOFF, 0, 0, 0 },
	// Runway01 Entry
	{ 13, FLYING, 0, 0, 19 },{ 13, PRELANDING, 0, 0, 14 }, { 13, HELILANDING, 0, 0, 27 },  { 13, 0, 0, 0, 19 },
	/* landing */
	{ 14, PRELANDING, RUNW01_block, 0, 15 },{ 14, LANDING, 0, 0, 15 },
	{ 15, LANDING, RUNW01_block, 0, 16 },{ 15, FLYING, 0, 0, 30 },{ 15, 0, 0, 0, 16 },
	{ 16, ENDLANDING, 0, TAXI06_block, 17 },{ 16, 0, RUNW02_block, TAXI01_block, 17 },{ 16, TERM01, 0, 0, 0 },{ 16, TERM02, 0, 0, 0 },{ 16, TERM03, 0, 0, 0 },
	// Cross Runway02
	{ 17,   0, RUNW02_block, 0, 18 },
	{ 18, 255, 0, TAXI01_block,  7 },{ 18, TERM01, 0, 0, 2 }, { 18, HANGAR, HANG01_block, TAXI02_block | TAXI03_block, 7},  { 18, 0, 0, 0,  7 },
	/* In Air */
	{ 19, 0, 0, 0, 20 },
	{ 20, 0, 0, 0, 21 },
	{ 21, 0, 0, 0, 22 },
	{ 22, 0, 0, 0, 13 },
	/* helicopter */
	{ 23, HELITAKEOFF, 0, 0, 0 },
	{ 24, HELITAKEOFF, 0, 0, 0 },
	{ 25, HELITAKEOFF, 0, 0, 0 },
	{ 26, HELITAKEOFF, 0, 0, 0 },
	// Helicopter Landing
	{ 27, HELILANDING, HANG01_block, TAXI04_block | TAXI03_block | TAXI02_block, 28 },
	{ 28, HELIENDLANDING, HANG01_block, TAXI04_block | TAXI03_block | TAXI02_block, 1 }, { 28, FLYING, 0, 0, 19 }, { 28, TERM01, 0, 0, 0 },{ 28, TERM02, 0, 0, 0 },{ 28, TERM03, 0, 0, 0 }, { 28, 0, 0, 0, 1 },
	// Helicopter Entry
	{ 29, FLYING, 0, 0, 27 },{ 29, HELILANDING, 0, 0, 27 },{ 29, 0, 0, 0, 27 },
	// Abort Landing Runway 01
	{ 30, 0, 0, 0, 20 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// International
static const HangarTileTable _airport_depots_international[] = { { { 0, 3 }, DIR_SE, 0 },{ { 6, 1 }, DIR_SE, 1 } };
static const TerminalTileTable _airport_terminals_international[] = { { { 2, 2 }, TERM01_block },{ { 2, 3 }, TERM02_block },{ { 2, 4 }, TERM03_block },{ { 4, 2 }, TERM04_block },{ { 4, 3 }, TERM05_block },{ { 4, 4 }, TERM06_block },{ { 6, 4 }, CARG01_block },{ { 6, 3 }, CARG02_block } };
static const RunwayTerminalTable _airport_runways_international[] = { { TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | CARG01_block | CARG02_block, RUNW01_block, 0 } };
static const byte _airport_terminal_international[] = { 1, 6 };
static const byte _airport_entries_international[] = { 15, 14, 17, 16 };
static const byte _airport_helientry_international[] = { 15, 14, 17, 16 };
static const AirportFTAbuildup _airport_fta_international[] = {
	/* Hangars and Outside Hangars */
	{ 0, HANGAR, 0, 0, 2 },{ 0, 0, 0, 0,  2 },{ 0, TERM01, 0, 0, 2 },{ 0, TERM02, 0, 0, 2 },{ 0, TERM03, 0, 0, 2 },{ 0, TERM04, 0, 0, 2 },{ 0, TERM05, 0, 0, 2 },{ 0, TERM06, 0, 0, 2 },{ 0, HELI01, 0, 0, 2 },{ 0, HELI02, 0, 0, 2 },{ 0, CARG01, 0, 0, 2 },{ 0, CARG02, 0, 0, 2 },
	{ 1, HANGAR, 0, 0, 3 },{ 1, 0, 0, 0,  3 },{ 1, TERM04, 0, 0, 3 },{ 1, TERM05, 0, 0, 3 },{ 1, TERM06, 0, 0, 3 },{ 1, HELI01, 0, 0, 3 },{ 1, HELI02, 0, 0, 3 },{ 1, CARG01, 0, 0, 3 },{ 1, CARG02, 0, 0, 3 },
	{ 2, 255, HANG01_block, 0, 0 },{ 2, HANGAR, 0, 0, 0 },{ 2, HELITAKEOFF, HELI02_block, 0, 36 }, { 2, 0, 0, 0, 36 },
	{ 3, 255, HANG02_block, 0, 0 },{ 3, HANGAR, 0, 0, 1 },{ 3, TERM04, 0, TAXI10_block, 30 },{ 3, TERM05, 0, TAXI10_block | TAXI09_block, 30 },{ 3, TERM06, 0, TAXI10_block | TAXI09_block | TAXI08_block, 30 },{ 3, HELI01, 0, 0, 12 },{ 3, HELI02, 0, TAXI10_block | TAXI09_block | TAXI08_block, 30 },{ 3, HELITAKEOFF, HELI01_block, 0, 12 },{ 3, CARG01, 0, TAXI10_block | TAXI09_block, 30 },{ 3, CARG02, 0, TAXI10_block | TAXI09_block | TAXI08_block, 30 },{ 3, 0, 0, 0, 30 },
	/* Airplane Terminals */
	{ 4, TERM03, TERM03_block, 0, 36 },{ 4, HANGAR, HANG01_block, TAXI16_block, 36 },{ 4, 0, 0, 0, 36 },
	{ 5, TERM02, TERM02_block, 0, 37 },{ 5, HANGAR, HANG01_block, TAXI17_block | TAXI16_block, 37 },{ 5, 0, 0, 0, 37 },
	{ 6, TERM01, TERM01_block, 0, 38 },{ 6, HANGAR, HANG01_block, TAXI18_block | TAXI17_block | TAXI16_block, 38 },{ 6, 0, 0, 0, 38 },
	{ 7, TERM06, TERM06_block, 0, 28 },{ 7, HANGAR, 0, 0, 28 },{ 7, 0, 0, 0, 28 },
	{ 8, TERM05, TERM05_block, 0, 29 },{ 8, HANGAR, 0, 0, 29 },{ 8, 0, 0, 0, 29 },
	{ 9, TERM04, TERM04_block, 0, 30 },{ 9, HANGAR, 0, 0, 30 },{ 9, 0, 0, 0, 30 },
	/* Cargo Terminals (Uses Helipads) */
	{ 10, CARG01, CARG01_block, 0, 29 },{ 10, HANGAR, 0, 0, 29 },{ 10, 0, 0, 0, 29 },
	{ 11, CARG02, CARG02_block, 0, 28 },{ 11, HANGAR, 0, 0, 28 },{ 11, 0, 0, 0, 28 },
	/* Helicopter Terminals */
	{ 12, HELI01, HELI01_block, 0, 12 },{ 12, HANGAR, 0, 0,  3 },{ 12, HELITAKEOFF, 0, 0, 49 },
	{ 13, HELI02, HELI02_block, 0, 13 },{ 13, HANGAR, 0, 0, 28 },{ 13, HELITAKEOFF, 0, 0, 50 },
	/* Airplane - Holding Pattern */
	{ 14, 0, 0, 0, 15 },
	{ 15, 0, 0, 0, 16 },
	{ 16, 0, 0, 0, 17 },
	{ 17, 0, 0, 0, 18 },
	// Runway01 Entry
	{ 18, FLYING, 0, 0, 14 }, { 18, PRELANDING, 0, 0,  19 }, { 18, HELILANDING, 0, 0, 43 }, { 18, 0, 0, 0, 14 },
	/* Airplane Landing */
	{ 19, PRELANDING, RUNW01_block, 0, 20 },{ 19, LANDING, 0, 0, 20 },
	{ 20, LANDING, RUNW01_block, 0, 21 },{ 20, FLYING, 0, 0, 42 },{ 20, 0, 0, 0, 21 },
	{ 21, ENDLANDING, 0, TAXI01_block, 22 },{ 21, 0, 0, 0, 22 },{ 21, TERM01, 0, 0, 0 },{ 21, TERM02, 0, 0, 0 },{ 21, TERM03, 0, 0, 0 },{ 21, TERM04, 0, 0, 0 },{ 21, TERM05, 0, 0, 0 },{ 21, TERM06, 0, 0, 0 },{ 21, CARG01, 0, 0, 0 },{ 21, CARG02, 0, 0, 0 }, // This is the format to control ENDLANDING to Terminal
	/* Airplane Taxi */
	{ 22, 255, 0, TAXI02_block,  0 },{ 22, HANGAR, 0, 0,  2 },{ 22, TAKEOFF, 0, 0, 36 },{ 22, TERM01, 0, 0, 36 },{ 22, TERM02, 0, 0, 36 },{ 22, TERM03, 0, 0, 36 },{ 22, 0, 0, 0, 23 },
	{ 23, 255, 0, TAXI03_block,  0 },{ 23, HANGAR, 0, 0, 36 },{ 23, TERM04, 0, 0,  24 },{ 23, TERM05, 0, 0, 24 },{ 23, TERM06, 0, 0, 24 },{ 23, HELI01, 0, 0, 24 },{ 23, HELI02, 0, 0, 24 },{ 23, HELITAKEOFF, 0, 0, 24 },{ 23, CARG01, 0, 0, 24 },{ 23, CARG02, 0, 0, 24 },{ 23, 0, 0, 0, 36 },
	{ 24,   0, 0, TAXI04_block, 25 },
	{ 25,   0, 0, TAXI05_block, 26 },
	{ 26, 255, 0, TAXI06_block,  0 },{ 26, TERM06, TERM06_block, 0, 7 },{ 26, 0, 0, 0, 27 },  // Shortcut for Terminal 6
	{ 27,   0, 0, TAXI07_block, 28 },
	/* Terminals 4,5,6 */
	{ 28, BACKUP, 0, TAXI08_block, 0 },{ 28, TERM06, 0, 0, 7 },{ 28, HELI02, 0, 0, 13 },{ 28, HELITAKEOFF, 0, 0, 13 },{ 28, CARG02, 0, 0, 11 },{ 28, 0, 0, 0, 29 },
	{ 29, BACKUP, 0, TAXI09_block, 0 },{ 29, TERM05, 0, 0, 8 },{ 29, HANGAR, 0, 0,  3 },{ 29, TERM06, 0, 0, 28 },{ 29, HELI01, 0, 0, 12 },{ 29, HELI02, 0, 0, 28 },{ 29, CARG01, 0, 0, 10 },{ 29, CARG02, 0, 0, 28 },{ 29, 0, 0, 0, 30 },
	{ 30, BACKUP, 0, TAXI10_block, 0 },{ 30, TERM04, 0, 0, 9 },{ 30, HANGAR, 0, 0,  3 },{ 30, TERM05, 0, 0, 29 },{ 30, TERM06, 0, 0, 29 },{ 30, HELI02, 0, 0, 29 },{ 30, CARG01, 0, 0, 29 },{ 30, CARG02, 0, 0, 29 },{ 30, 0, 0, 0, 31 },
	{ 31, 0, 0, TAXI11_block, 32 },
	{ 32, 0, 0, TAXI12_block, 33 },
	{ 33, 0, 0, TAXI13_block, 34 },
	{ 34, 0, 0, TAXI14_block, 35 },
	{ 35, 0, 0, TAXI15_block, 39 },
	/* Terminals 1,2,3 */
	{ 36, BACKUP, 0, TAXI16_block, 0 },{ 36, TERM03, 0, 0, 4 },{ 36, HANGAR, 0, 0,  2 },{ 36, TERM04, 0, 0, 23 },{ 36, TERM05, 0, 0, 23 },{ 36, TERM06, 0, 0, 23 },{ 36, HELI01, 0, 0, 23 },{ 36, HELI02, 0, 0, 23 },{ 36, CARG01, 0, 0, 23 },{ 36, CARG02, 0, 0, 23 },{ 36, HELITAKEOFF, 0, 0, 23 },{ 36, 0, 0, 0, 37 },
	{ 37, BACKUP, 0, TAXI17_block, 0 },{ 37, TERM02, 0, 0, 5 },{ 37, HANGAR, 0, 0, 36 },{ 37, 0, 0, 0, 38 },
	{ 38, BACKUP, 0, TAXI18_block, 0 },{ 38, TERM01, 0, 0, 6 },{ 38, HANGAR, 0, 0, 37 },{ 38, 0, 0, 0, 35 },
	{ 39, TAKEOFF, RUNW02_block, 0, 40 },
	/* Airplane Takeoff */
	{ 40, 0, 0, 0, 41 },
	{ 41, STARTTAKEOFF, 0, 0, 42 },
	{ 42, ENDTAKEOFF, 0, 0, 0 },{ 42, 0, 0, 0, 15 },      //Airplane leaves Airport Control
	/* Helicopter - Holding Pattern */
	{ 43, HELILANDING, 0, 0, 44 },
	{ 44, HELIENDLANDING, HANG02_block, 0, 44 }, { 44, FLYING, 0, 0, 14 }, { 44, HELI02, 0, 0, 46 },{ 44, HELI01, 0, 0, 45 },{ 44, HANGAR, 0, 0, 51 }, { 44, 0, 0, 0, 51 },
	{ 45, 0, 0, 0, 47 },
	{ 46, 0, 0, 0, 48 },
	/* Helicopter Landing */
	{ 47, HELI01, 0, 0, 12 },
	{ 48, HELI02, 0, 0, 13 },
	/* Helicopter Takeoff */
	{ 49, HELITAKEOFF, 0, 0, 0 },
	{ 50, HELITAKEOFF, 0, 0, 0 },
	/* Helicopter Hangar */
	{ 51, 0, HANG02_block, 0, 52 }, // need to go to hangar when waiting in air
	{ 52, 0, HANG02_block, 0, 3 },
	// Helicopter Entry
	{ 53, FLYING, 0, 0, 43 },{ 53, HELILANDING, 0, 0, 43 },{ 53, 0, 0, 0, 43 },
	/* End Marker */
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Terminus1
static const HangarTileTable _airport_depots_terminus1[] = { { { 2, 3 }, DIR_NE, 0 },{ { 6, 3 }, DIR_SW, 1 } };
static const TerminalTileTable _airport_terminals_terminus1[] = { { { 0, 1 }, TERM01_block },{ { 1, 1 }, TERM02_block },{ { 2, 1 }, TERM03_block },{ { 3, 1 }, TERM04_block },{ { 4, 1 }, TERM05_block },{ { 5, 1 }, TERM06_block },{ { 6, 1 }, TERM07_block },{ { 7, 1 }, TERM08_block },{ { 8, 1 }, TERM09_block },{ { 3, 3 }, CARG01_block },{ { 4, 3 }, CARG02_block },{ { 5, 3 }, CARG03_block } };
static const RunwayTerminalTable _airport_runways_terminus1[] = { { TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | TERM07_block | TERM08_block | TERM09_block | CARG01_block | CARG02_block | CARG03_block, RUNW01_block | RUNW02_block, 0 } };
static const byte _airport_terminal_terminus1[] = { 1, 9 };
static const byte _airport_entries_terminus1[] = { 20, 19, 22, 21 };
static const byte _airport_helientry_terminus1[] = { 20, 19, 22, 21 };
static const AirportFTAbuildup _airport_fta_terminus1[] = {
	// Airplane & Helicopter Hangars
	{ 0, HANGAR, 0, 0, 2 },{ 0, 0, 0, 0, 2 },{ 0, TERM09, 0, 0, 2 },{ 0, TERM07, 0, 0, 2 },{ 0, TERM05, 0, 0, 2 },{ 0, TERM03, 0, 0, 2 },{ 0, TERM01, 0, 0, 2 },{ 0, TERM08, 0, 0, 2 },{ 0, TERM06, 0, 0, 2 },{ 0, TERM04, 0, 0, 2 },{ 0, TERM02, 0, 0, 2 },{ 0, HELI03, 0, 0, 2 },{ 0, HELI02, 0, 0, 2 },{ 0, HELI01, 0, 0, 2 },{ 0, CARG01, 0, 0, 2 },{ 0, CARG02, 0, 0, 2 },{ 0, CARG03, 0, 0, 2 },
	{ 1, HANGAR, 0, 0, 3 },{ 1, 0, 0, 0, 3 },{ 1, TERM09, 0, 0, 3 },{ 1, TERM08, 0, 0, 3 },{ 1, TERM07, 0, 0, 3 },{ 1, TERM06, 0, 0, 3 },{ 1, HELI03, 0, 0, 3 },{ 1, HELI02, 0, 0, 3 },{ 1, HELI01, 0, 0, 3 },{ 1, CARG03, 0, 0, 3 },{ 1, CARG02, 0, 0, 3 },{ 1, CARG01, 0, 0, 3 },
	{ 2, 255, HANG01_block, 0, 0 },{ 2, HANGAR, 0, 0, 0 },{ 2, TERM01 , 0, TAXI05_block | TAXI04_block, 33 }, { 2, HELITAKEOFF, HELI01_block, 0, 33 }, { 2, 0, 0, 0, 33 },
	{ 3, 255, HANG02_block, 0, 0 },{ 3, HANGAR, 0, 0, 1 },{ 3, TERM06, 0, TAXI11_block | TAXI10_block | TAXI09_block, 39 },{ 3, TERM07, 0, TAXI11_block | TAXI10_block, 39 },{ 3, TERM08, 0, TAXI11_block, 39 },{ 3, TERM09, 0, TAXI11_block, 39 },{ 3, HELI01, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block | TAXI07_block,  39 },{ 3, HELI02, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block,  39 },{ 3, HELI03, 0, TAXI11_block | TAXI10_block | TAXI09_block,  39 },{ 3, CARG01, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block | TAXI07_block,  39 },{ 3, CARG02, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block,  39 },{ 3, CARG03, 0, TAXI11_block | TAXI10_block | TAXI09_block,  39 },{ 3, TAKEOFF, 0, TAXI13_block, 41 }, { 3, HELITAKEOFF, HELI03_block, TAXI11_block | TAXI10_block | TAXI09_block, 39 }, { 3, 0, 0, TAXI13_block, 41 },
	// Airplane Terminals
	{ 4, TERM01, TERM01_block, 0, 32 },{ 4, HANGAR, HANG01_block, TAXI04_block | TAXI05_block, 32 },{ 4, 0, 0, 0, 32 },
	{ 5, TERM02, TERM02_block, 0, 33 },{ 5, HANGAR, HANG01_block, TAXI05_block, 33 },{ 5, 0, 0, 0, 33 },
	{ 6, TERM03, TERM03_block, 0, 34 },{ 6, HANGAR, HANG01_block, TAXI06_block | TAXI05_block, 34 },{ 6, 0, 0, 0, 34 },
	{ 7, TERM04, TERM04_block, 0, 35 },{ 7, HANGAR, 0, 0, 35 },{ 7, 0, 0, 0, 35 },
	{ 8, TERM05, TERM05_block, 0, 36 },{ 8, HANGAR, 0, 0, 36 },{ 8, 0, 0, 0, 36 },
	{ 9, TERM06, TERM06_block, 0, 37 },{ 9, HANGAR, 0, 0, 37 },{ 9, 0, 0, 0, 37 },
	{ 10, TERM07, TERM07_block, 0, 38 },{ 10, HANGAR, 0, 0, 38 },{ 10, 0, 0, 0, 38 },
	{ 11, TERM08, TERM08_block, 0, 39 },{ 11, HANGAR, 0, 0, 39 },{ 11, 0, 0, 0, 39 },
	{ 12, TERM09, TERM09_block, 0, 40 },{ 12, HANGAR, HANG02_block, TAXI11_block | TAXI12_block, 40 },{ 12, 0, 0, 0, 40 },
	// Cargo Terminals (Uses Helipads.)
	{ 13, CARG01, CARG01_block, 0, 35 },{ 13, HANGAR, 0, 0, 35 },{ 13, 0, 0, 0, 35 },
	{ 14, CARG02, CARG02_block, 0, 36 },{ 14, HANGAR, 0, 0, 36 },{ 14, 0, 0, 0, 36 },
	{ 15, CARG03, CARG03_block, 0, 37 },{ 15, HANGAR, 0, 0, 37 },{ 15, 0, 0, 0, 37 },
	// Helicopter Terminals
	{ 16, HELI01, HELI01_block, 0, 35 },{ 16, HANGAR, 0, 0, 35 },{ 16, HELITAKEOFF, 0, 0, 48 },
	{ 17, HELI02, HELI02_block, 0, 36 },{ 17, HANGAR, 0, 0, 36 },{ 17, HELITAKEOFF, 0, 0, 49 },
	{ 18, HELI03, HELI03_block, 0, 37 },{ 18, HANGAR, 0, 0, 37 },{ 18, HELITAKEOFF, 0, 0, 50 },
	// Airplane In Air - Holding Pattern
	{ 19, 0, 0, 0, 20 },
	{ 20, 0, 0, 0, 21 },
	{ 21, 0, 0, 0, 22 },
	{ 22, 0, 0, 0, 53 },
	// Airplane Landing Runway01
	{ 23, PRELANDING, RUNW01_block, 0, 24 },{ 23, LANDING, 0, 0, 24 },
	{ 24, LANDING, 0, 0, 25 },{ 24, FLYING, 0, 0,  56 },{ 24, 0, 0, 0, 25 },
	{ 25, ENDLANDING, 0, TAXI01_block, 30 },{ 25, 0, 0, 0, 30 },{ 25, TERM09, 0, 0, 0 },{ 25, TERM07, 0, 0, 0 },{ 25, TERM05, 0, 0, 0 },{ 25, TERM03, 0, 0, 0 },{ 25, TERM01, 0, 0, 0 },{ 25, TERM08, 0, 0, 0 },{ 25, TERM06, 0, 0, 0 },{ 25, TERM04, 0, 0, 0 },{ 25, TERM02, 0, 0, 0 },{ 25, CARG01, 0, 0, 0 },{ 25, CARG02, 0, 0, 0 },{ 25, CARG03, 0, 0, 0 },
	{ 26, 0, 0, TAXI01_block, 30 },
	// Airplane Landing Runway02
	{ 27, PRELANDING, RUNW02_block, TAXI09_block | TAXI10_block | TAXI11_block | TAXI13_block, 28 },{ 27, LANDING, 0, 0, 28 },
	{ 28, LANDING, RUNW02_block, 0, 29 },{ 28, FLYING, 0, 0, 45 },{ 28, 0, 0, 0, 29 },
	{ 29, ENDLANDING, 0, TAXI14_block, 30 },{ 29, 0, 0, 0, 30 },{ 29, TERM09, 0, 0, 0 },{ 29, TERM07, 0, 0, 0 },{ 29, TERM05, 0, 0, 0 },{ 29, TERM03, 0, 0, 0 },{ 29, TERM01, 0, 0, 0 },{ 29, TERM08, 0, 0, 0 },{ 29, TERM06, 0, 0, 0 },{ 29, TERM04, 0, 0, 0 },{ 29, TERM02, 0, 0, 0 },{ 29, CARG01, 0, 0, 0 },{ 29, CARG02, 0, 0, 0 },{ 29, CARG03, 0, 0, 0 },
	// End Landing for Runways 1 & 2
	{ 30,      0, 0, TAXI02_block, 31 },
	// Airplane & Helicopter Taxi  (Main Airport flow)
	{ 31,    255, 0, TAXI03_block, 0 },{ 31, HANGAR,  0, 0,  2 },{ 31, TERM01, 0, 0, 32 },{ 31, 0, 0, 0, 33 },
	{ 32, BACKUP, 0, TAXI04_block, 0 },{ 32, TERM01,  0, 0,  4 },{ 32, 0, 0, 0, 33 },
	{ 33, BACKUP, 0, TAXI05_block, 0 },{ 33, TERM02,  0, 0,  5 },{ 33, HANGAR, 0, 0,  2 },{ 33, TERM01, 0, 0, 32 },{ 33, 0, 0, 0, 34 },
	{ 34, BACKUP, 0, TAXI06_block, 0 },{ 34, TERM03,  0, 0,  6 },{ 34, HANGAR, 0, 0, 33 },{ 34, 0, 0, 0, 35 },
	{ 35, BACKUP, 0, TAXI07_block, 0 },{ 35, TERM04,  0, 0,  7 },{ 35, CARG01, 0, 0, 13 },{ 35, HELI01, 0, 0, 16 }, { 35, HELITAKEOFF, 0, 0, 16 }, { 35, 0, 0, 0, 36 },
	{ 36, BACKUP, 0, TAXI08_block, 0 },{ 36, TERM05,  0, 0,  8 },{ 36, CARG02, 0, 0, 14 },{ 36, HELI02, 0, 0, 17 },{ 36, HELI01, 0, 0, 35 },{ 36, CARG01, 0, 0, 35 },{ 36, 0, 0, 0, 37 },
	{ 37, BACKUP, 0, TAXI09_block, 0 },{ 37, TERM06,  0, 0,  9 },{ 37, CARG03, 0, 0, 15 },{ 37, HELI03, 0, 0, 18 },{ 37, HELI02, 0, 0, 36 },{ 37, HELI01, 0, 0, 36 },{ 37, CARG02, 0, 0, 36 },{ 37, CARG01, 0, 0, 36 },{ 37, HELITAKEOFF, 0, 0, 18 },{ 37, 0, 0, 0, 38 },
	{ 38, BACKUP, 0, TAXI10_block, 0 },{ 38, TERM07,  0, 0, 10 },{ 38, TERM06, 0, 0, 37 },{ 38, HELI03, 0, 0, 37 },{ 38, HELI02, 0, 0, 37 },{ 38, HELI01, 0, 0, 37 },{ 38, CARG03, 0, 0, 37 },{ 38, CARG02, 0, 0, 37 },{ 38, CARG01, 0, 0, 37 },{ 38, HELITAKEOFF, 0, 0, 37 },{ 38, 0, 0, 0, 39 },
	{ 39, BACKUP, 0, TAXI11_block, 0 },{ 39, TERM08,  0, 0, 11 },{ 39, HANGAR, 0, 0,  3 },{ 39, TERM09, 0, 0, 40 },{ 39, TERM07, 0, 0, 38 },{ 39, TERM06, 0, 0, 38 },{ 39, HELI03, 0, 0, 38 },{ 39, HELI02, 0, 0, 38 },{ 39, HELI01, 0, 0, 38 },{ 39, CARG03, 0, 0, 38 },{ 39, CARG02, 0, 0, 38 },{ 39, CARG01, 0, 0, 38 },{ 39, HELITAKEOFF, 0, 0, 38 },{ 39, 0, 0, 0, 41 },
	{ 40, BACKUP, 0, TAXI12_block, 0 },{ 40, TERM09,  0, 0, 12 },{ 40, HANGAR, 0, 0, 39 },{ 40, 0, 0, 0, 41 },
	{ 41,    255, 0, TAXI13_block, 0 },{ 41, 0, 0, 0, 42 },
	// Airplane Takeoff
	{ 42, TAKEOFF, RUNW02_block, TAXI14_block, 43 },
	{ 43, 0, 0, 0, 44 },
	{ 44, STARTTAKEOFF, 0, 0, 45 },
	{ 45, ENDTAKEOFF, 0, 0, 0 },{ 45, 0, 0, 0, 20 },   // End Airport control of airplane.
	// Helicopter -- stay in air in special place as a buffer to choose from helipads
	{ 46, HELILANDING, 0, 0, 47 },{ 46, 0, 0, 0, 47 },
	{ 47, HELIENDLANDING, 0, 0, 51 }, { 47, FLYING, 0, 0, 19 }, { 47, HELI03, 0, 0, 59 },{ 47, HELI01, 0, 0, 57 },{ 47, HELI02, 0, 0, 58 },{ 47, HANGAR, 0, 0, 51 },{ 47, 0, 0, 0, 51 },
	// Helicopter -- takeoff
	{ 48, HELITAKEOFF, 0, 0, 0 },
	{ 49, HELITAKEOFF, 0, 0, 0 },
	{ 50, HELITAKEOFF, 0, 0, 0 },
	//Helicopter -- Hangar
	{ 51, 0, HANG01_block, 0, 52 }, // need to go to hangar when waiting in air
	{ 52, 0, 0, 0,  2 },
	// Added Holding Pattern spots.
	{ 53, FLYING, 0, 0, 54 },{ 53, PRELANDING, 0, 0,  27 },{ 53, HELILANDING, 0, 0, 46 }, { 53, 0, 0, 0, 54 },
	{ 54, FLYING, 0, 0, 19 },{ 54, PRELANDING, 0, 0,  23 },{ 54, 0, 0, 0, 19 },
	// Helicopter -- Entry .  Keeps it from messing up the planes.
	{ 55, FLYING, 0, 0, 55 },{ 55, HELILANDING, 0, 0, 46 },
	// Abort Landing Runway 01
	{ 56, 0, 0, 0, 20 },
	// Helicopter Positioning
	{ 57, 0, 0, 0, 60 },
	{ 58, 0, 0, 0, 61 },
	{ 59, 0, 0, 0, 62 },
	//  Helicopter landing
	{ 60,   0, 0, 0, 16 },
	{ 61,   0, 0, 0, 17 },
	{ 62,   0, 0, 0, 18 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Terminus2
static const HangarTileTable _airport_depots_terminus2[] = { { { 2, 8 }, DIR_NE, 0 },{ { 6, 8 }, DIR_SW, 1 },{ { 6, 2 }, DIR_SW, 2 },{ { 2, 2 }, DIR_NE, 3 } };
static const TerminalTileTable _airport_terminals_terminus2[] = { { { 0, 6 }, TERM01_block },{ { 1, 6 }, TERM02_block },{ { 2, 6 }, TERM03_block },{ { 3, 6 }, TERM04_block },{ { 5, 6 }, TERM05_block },{ { 6, 6 }, TERM06_block },{ { 7, 6 }, TERM07_block },{ { 8, 6 }, TERM08_block },
{ { 8, 4 }, TERM09_block },{ { 7, 4 }, TERM10_block },{ { 6, 4 }, TERM11_block },{ { 5, 4 }, TERM12_block },{ { 3, 4 }, TERM13_block },{ { 2, 4 }, TERM14_block },{ { 1, 4 }, TERM15_block },{ { 0, 4 }, TERM16_block },
{ { 3, 8 }, CARG01_block },{ { 4, 8 }, CARG02_block },{ { 5, 8 }, CARG03_block },{ { 5, 2 }, CARG04_block },{ { 4, 2 }, CARG05_block },{ { 3, 2 }, CARG06_block } };
static const RunwayTerminalTable _airport_runways_terminus2[] = { { TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | TERM07_block | TERM08_block | CARG01_block | CARG02_block | CARG03_block, RUNW01_block | RUNW02_block, 0 },
{ TERM09_block | TERM10_block | TERM11_block | TERM12_block | TERM13_block | TERM14_block | TERM15_block | TERM16_block | CARG04_block | CARG05_block | CARG06_block, RUNW03_block | RUNW04_block, 0 } };
static const byte _airport_terminal_terminus2[] = { 1, 16 };
static const byte _airport_entries_terminus2[] = { 37, 36, 39, 38 };
static const byte _airport_helientry_terminus2[] = { 37, 36, 39, 38 };
static const AirportFTAbuildup _airport_fta_terminus2[] = {
	// Airplane & Helicopter Hangars
	{ 0, HANGAR, 0, 0, 4 },{ 0, 0, 0, 0, 4 },{ 0, TERM07, 0, 0, 4 },{ 0, TERM05, 0, 0, 4 },{ 0, TERM03, 0, 0, 4 },{ 0, TERM01, 0, 0, 4 },{ 0, TERM08, 0, 0, 4 },{ 0, TERM06, 0, 0, 4 },{ 0, TERM04, 0, 0, 4 },{ 0, TERM02, 0, 0, 4 },{ 0, TERM13, 0, 0, 4 },{ 0, TERM14, 0, 0, 4 },{ 0, TERM15, 0, 0, 4 },{ 0, TERM16, 0, 0, 4 },{ 0, HELI01, 0, 0, 4 },{ 0, HELI02, 0, 0, 4 },{ 0, HELI03, 0, 0, 4 },{ 0, CARG01, 0, 0, 4 },{ 0, CARG02, 0, 0, 4 },{ 0, CARG03, 0, 0, 4 },{ 0, CARG05, 0, 0, 4 },{ 0, CARG06, 0, 0, 4 },
	{ 1, HANGAR, 0, 0, 5 },{ 1, 0, 0, 0, 5 },{ 1, TERM08, 0, 0, 5 },{ 1, TERM07, 0, 0, 5 },{ 1, TERM06, 0, 0, 5 },{ 1, TERM05, 0, 0, 5 },{ 1, HELI03, 0, 0, 5 },{ 1, HELI02, 0, 0, 5 },{ 1, HELI01, 0, 0, 5 },{ 1, CARG03, 0, 0, 5 },{ 1, CARG02, 0, 0, 5 },{ 1, CARG01, 0, 0, 5 },
	{ 2, HANGAR, 0, 0, 6 },{ 2, 0, 0, 0, 6 },{ 2, TERM15, 0, 0, 6 },{ 2, TERM13, 0, 0, 6 },{ 2, TERM11, 0, 0, 6 },{ 2, TERM09, 0, 0, 6 },{ 2, TERM16, 0, 0, 6 },{ 2, TERM14, 0, 0, 6 },{ 2, TERM12, 0, 0, 6 },{ 2, TERM10, 0, 0, 6 },{ 2, TERM05, 0, 0, 6 },{ 2, TERM06, 0, 0, 6 },{ 2, TERM07, 0, 0, 6 },{ 2, TERM08, 0, 0, 6 },{ 2, HELI04, 0, 0, 6 },{ 2, HELI05, 0, 0, 6 },{ 2, HELI06, 0, 0, 6 },{ 2, CARG04, 0, 0, 6 },{ 2, CARG05, 0, 0, 6 },{ 2, CARG06, 0, 0, 6 },{ 2, CARG02, 0, 0, 6 },{ 2, CARG03, 0, 0, 6 },
	{ 3, HANGAR, 0, 0, 7 },{ 3, 0, 0, 0, 7 },{ 3, TERM16, 0, 0, 7 },{ 3, TERM15, 0, 0, 7 },{ 3, TERM14, 0, 0, 7 },{ 3, TERM13, 0, 0, 7 },{ 3, HELI06, 0, 0, 7 },{ 3, HELI05, 0, 0, 7 },{ 3, HELI04, 0, 0, 7 },{ 3, CARG06, 0, 0, 7 },{ 3, CARG05, 0, 0, 7 },{ 3, CARG04, 0, 0, 7 },
	{ 4, 255, HANG01_block, 0, 0 },{ 4, HANGAR, 0, 0, 0 },{ 4, TERM01, 0, TAXI04_block | TAXI05_block, 54 },{ 4, TERM13, 0, TAXI27_block, 54 },{ 4, TERM14, 0, TAXI27_block, 54 },{ 4, TERM15, 0, TAXI27_block, 54 },{ 4, TERM16, 0, TAXI27_block, 54 },{ 4, CARG05, 0, TAXI27_block, 54 },{ 4, CARG06, 0, TAXI27_block, 54 }, { 4, HELITAKEOFF, HELI01_block, 0, 54 }, { 4, 0, 0, 0, 54 },
	{ 5, 255, HANG02_block, 0, 0 },{ 5, HANGAR, 0, 0, 1 },{ 5, TERM08, 0, TAXI11_block, 60 },{ 5, TERM07, 0, TAXI11_block, 60 },{ 5, TERM06, 0, TAXI11_block | TAXI10_block, 60 },{ 5, TERM05, 0, TAXI11_block | TAXI10_block | TAXI09_block, 60 },{ 5, HELI03, 0, TAXI11_block | TAXI10_block | TAXI09_block, 60 },{ 5, HELI02, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block, 60 },{ 5, HELI01, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block | TAXI07_block, 60 },{ 5, CARG03, 0, TAXI11_block | TAXI10_block | TAXI09_block, 60 },{ 5, CARG02, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block, 60 },{ 5, CARG01, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block | TAXI07_block, 60 },{ 5, HELITAKEOFF, HELI03_block, TAXI11_block | TAXI10_block | TAXI09_block, 60 },{ 5, TAKEOFF, 0, TAXI13_block, 62 }, { 5, 0, 0, TAXI13_block, 62 },
	{ 6, 255, HANG03_block, 0, 0 },{ 6, HANGAR, 0, 0, 2 },{ 6, TERM09, 0, TAXI18_block | TAXI17_block, 80 },{ 6, TERM05, 0, TAXI27_block, 80 },{ 6, TERM06, 0, TAXI27_block, 80 },{ 6, TERM07, 0, TAXI27_block, 80 },{ 6, TERM08, 0, TAXI27_block, 80 },{ 6, CARG02, 0, TAXI27_block, 80 },{ 6, CARG03, 0, TAXI27_block, 80 }, { 6, HELITAKEOFF, HELI04_block, 0, 80 }, { 6, 0, 0, 0, 80 },
	{ 7, 255, HANG04_block, 0, 0 },{ 7, HANGAR, 0, 0, 3 },{ 7, TERM13, 0, TAXI24_block | TAXI23_block | TAXI22_block, 86 },{ 7, TERM14, 0, TAXI24_block | TAXI23_block, 86 },{ 7, TERM15, 0, TAXI24_block, 86 },{ 7, TERM16, 0, TAXI24_block, 86 },{ 7, HELI06, 0, TAXI24_block | TAXI23_block | TAXI22_block, 86 },{ 7, HELI05, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI21_block, 86 },{ 7, HELI04, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI21_block | TAXI20_block, 86 },{ 7, CARG06, 0, TAXI24_block | TAXI23_block | TAXI22_block, 86 },{ 7, CARG05, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI21_block, 86 },{ 7, CARG04, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI21_block | TAXI20_block, 86 },{ 7, HELITAKEOFF, HELI06_block, TAXI24_block | TAXI23_block | TAXI22_block, 86 },{ 7, TAKEOFF, 0, TAXI26_block, 88 },{ 7, 0, 0, TAXI26_block, 88 },
	// Airplane Terminals
	{ 8, TERM01, TERM01_block, 0,  53 },{ 8, HANGAR, HANG01_block, TAXI04_block | TAXI05_block, 53 },{ 8, 0, 0, 0, 53 },
	{ 9, TERM02, TERM02_block, 0,  54 },{ 9, HANGAR, HANG01_block, TAXI05_block, 54 },{ 9, 0, 0, 0, 54 },
	{ 10, TERM03, TERM03_block, 0,  55 },{ 10, HANGAR, HANG01_block, TAXI06_block | TAXI05_block, 55 },{ 10, 0, 0, 0, 55 },
	{ 11, TERM04, TERM04_block, 0,  56 },{ 11, HANGAR, 0, 0, 56 },{ 11, 0, 0, 0, 56 },
	{ 12, TERM05, TERM05_block, 0,  58 },{ 12, HANGAR, 0, 0, 58 },{ 12, 0, 0, 0, 58 },
	{ 13, TERM06, TERM06_block, 0,  59 },{ 13, HANGAR, 0, 0, 59 },{ 13, 0, 0, 0, 59 },
	{ 14, TERM07, TERM07_block, 0,  60 },{ 14, HANGAR, 0, 0, 60 },{ 14, 0, 0, 0, 60 },
	{ 15, TERM08, TERM08_block, 0,  61 },{ 15, HANGAR, HANG02_block, TAXI11_block | TAXI12_block, 61 },{ 15, 0, 0, 0, 61 },
	{ 16, TERM09, TERM09_block, 0,  79 },{ 16, HANGAR, HANG03_block, TAXI17_block | TAXI18_block, 79 },{ 16, 0, 0, 0, 79 },
	{ 17, TERM10, TERM10_block, 0,  80 },{ 17, HANGAR, HANG03_block, TAXI18_block, 80 },{ 17, 0, 0, 0, 80 },
	{ 18, TERM11, TERM11_block, 0,  81 },{ 18, HANGAR, HANG03_block, TAXI19_block | TAXI18_block, 81 },{ 18, 0, 0, 0, 81 },
	{ 19, TERM12, TERM12_block, 0,  82 },{ 19, HANGAR, 0, 0, 82 },{ 19, 0, 0, 0, 82 },
	{ 20, TERM13, TERM13_block, 0,  84 },{ 20, HANGAR, 0, 0, 84 },{ 20, 0, 0, 0, 84 },
	{ 21, TERM14, TERM14_block, 0,  85 },{ 21, HANGAR, 0, 0, 85 },{ 21, 0, 0, 0, 85 },
	{ 22, TERM15, TERM15_block, 0,  86 },{ 22, HANGAR, 0, 0, 86 },{ 22, 0, 0, 0, 86 },
	{ 23, TERM16, TERM16_block, 0,  87 },{ 23, HANGAR, HANG04_block, TAXI24_block | TAXI25_block, 87 },{ 23, 0, 0, 0, 87 },
	// Cargo Terminals (Uses Helipads.)
	{ 24, CARG01, CARG01_block, 0,  56 },{ 24, HANGAR, 0, 0, 56 },{ 24, 0, 0, 0, 56 },
	{ 25, CARG02, CARG02_block, 0,  57 },{ 25, HANGAR, 0, 0, 57 },{ 25, 0, 0, 0, 57 },
	{ 26, CARG03, CARG03_block, 0,  58 },{ 26, HANGAR, 0, 0, 58 },{ 26, 0, 0, 0, 58 },
	{ 27, CARG04, CARG04_block, 0,  82 },{ 27, HANGAR, 0, 0, 82 },{ 27, 0, 0, 0, 82 },
	{ 28, CARG05, CARG05_block, 0,  83 },{ 28, HANGAR, 0, 0, 83 },{ 28, 0, 0, 0, 83 },
	{ 29, CARG06, CARG06_block, 0,  84 },{ 29, HANGAR, 0, 0, 84 },{ 29, 0, 0, 0, 84 },
	// Helicopter Terminals
	{ 30, HELI01, HELI01_block, 0,  56 },{ 30, HANGAR, 0, 0, 56 },{ 30, HELITAKEOFF, 0, 0,  67 },
	{ 31, HELI02, HELI02_block, 0,  57 },{ 31, HANGAR, 0, 0, 57 },{ 31, HELITAKEOFF, 0, 0,  68 },
	{ 32, HELI03, HELI03_block, 0,  58 },{ 32, HANGAR, 0, 0, 58 },{ 32, HELITAKEOFF, 0, 0,  69 },
	{ 33, HELI04, HELI04_block, 0,  82 },{ 33, HANGAR, 0, 0, 82 },{ 33, HELITAKEOFF, 0, 0, 95 },
	{ 34, HELI05, HELI05_block, 0,  83 },{ 34, HANGAR, 0, 0, 83 },{ 34, HELITAKEOFF, 0, 0, 96 },
	{ 35, HELI06, HELI06_block, 0,  84 },{ 35, HANGAR, 0, 0, 84 },{ 35, HELITAKEOFF, 0, 0, 97 },
	// Airplane In Air - Holding Pattern
	{ 36, 0, 0, 0,  37 },
	{ 37, 0, 0, 0, 103 },
	{ 38, 0, 0, 0,  39 },
	{ 39, 0, 0, 0, 101 },
	// Helicopter -- Holding Pattern
	{ 40, HELILANDING, 0, 0, 41 },{ 40, 0, 0, 0, 41 },
	{ 41, HELIENDLANDING, 0, 0, 42 }, { 41, FLYING, 0, 0, 37 }, { 41, HELI03, 0, 0, 110 },{ 41, HELI06, 0, 0, 113 },{ 41, HELI02, 0, 0, 109 },{ 41, HELI05, 0, 0, 112 },{ 41, HELI01, 0, 0, 108 },{ 41, HELI04, 0, 0, 111 },{ 41, HANGAR, 0, 0, 42 },{ 41, 0, 0, 0, 42},
	//Helicopter -- Hangar
	{ 42, 0, HANG01_block, 0, 43 }, // need to go to hangar when waiting in air
	{ 43, 0, 0, 0, 4 },
	// Start of Pod #1
	// Airplane Landing Runway01
	{ 44, PRELANDING, RUNW01_block, 0, 45 },{ 44, LANDING, 0, 0, 45 },
	{ 45, LANDING, RUNW01_block, 0, 46 },{ 45, FLYING, 0, 0, 106 },{ 45, 0, 0, 0, 46 },
	{ 46, ENDLANDING, 0, TAXI01_block, 51 },{ 46, 0, 0, 0, 51 },{ 46, TERM07, 0, 0, 0 },{ 46, TERM05, 0, 0, 0 },{ 46, TERM03, 0, 0, 0 },{ 46, TERM01, 0, 0, 0 },{ 46, TERM08, 0, 0, 0 },{ 46, TERM06, 0, 0, 0 },{ 46, TERM04, 0, 0, 0 },{ 46, TERM02, 0, 0, 0 },{ 46, CARG01, 0, 0, 0 },{ 46, CARG02, 0, 0, 0 },{ 46, CARG03, 0, 0, 0 },
	{ 47, 0, 0, TAXI01_block, 51 }, // Unused
	// Airplane Landing Runway02
	{ 48, PRELANDING, RUNW02_block, TAXI09_block | TAXI10_block | TAXI11_block | TAXI13_block, 49 },{ 48, LANDING, 0, 0, 49 },
	{ 49, LANDING, RUNW02_block, 0, 50 },{ 49, FLYING, 0, 0, 66 },{ 49, 0, 0, 0, 50 },
	{ 50, ENDLANDING, 0, TAXI28_block, 51 },{ 50, 0, 0, 0, 51 },{ 50, TERM07, 0, 0, 0 },{ 50, TERM05, 0, 0, 0 },{ 50, TERM03, 0, 0, 0 },{ 50, TERM01, 0, 0, 0 },{ 50, TERM08, 0, 0, 0 },{ 50, TERM06, 0, 0, 0 },{ 50, TERM04, 0, 0, 0 },{ 50, TERM02, 0, 0, 0 },{ 50, CARG01, 0, 0, 0 },{ 50, CARG02, 0, 0, 0 },{ 50, CARG03, 0, 0, 0 },
	// End Landing for Runways 1 & 2
	{ 51,      0, 0, TAXI02_block, 52 },
	// Airplane & Helicopter Taxi  (Main Airport flow)
	{ 52,    255, 0, TAXI03_block,  0 },{ 52, HANGAR, 0, 0,   4 },{ 52, TERM01, 0, 0,  53 },{ 52, 0, 0, 0, 54 },
	{ 53, BACKUP, 0, TAXI04_block,  0 },{ 53, TERM01, 0, 0,   8 },{ 53, 0, 0, 0, 54 },
	{ 54, BACKUP, 0, TAXI05_block,  0 },{ 54, TERM02, 0, 0,   9 },{ 54, TERM01, 0, 0,  53 },{ 54, HANGAR, 0, 0, 4 },{ 54, 0, 0, 0, 55 },
	{ 55, BACKUP, 0, TAXI06_block,  0 },{ 55, TERM03, 0, 0,  10 },{ 55, HANGAR, 0, 0,  54 },{ 55, 0, 0, 0, 56 },
	{ 56, BACKUP, 0, TAXI07_block,  0 },{ 56, TERM04, 0, 0,  11 },{ 56, HELI01, 0, 0,  30 },{ 56, CARG01, 0, 0,  24 },{ 56, HELITAKEOFF, 0, 0, 30 }, { 56, 0, 0, 0, 57 },
	{ 57, BACKUP, 0, TAXI08_block,  0 },{ 57, TERM13, 0, 0,  98 },{ 57, HELI02, 0, 0,  31 },{ 57, CARG02, 0, 0,  25 },{ 57, TERM14, 0, 0, 98 },{ 57, TERM15, 0, 0, 98 },{ 57, TERM16, 0, 0, 98 },{ 57, HELI01, 0, 0, 56 },{ 57, CARG01, 0, 0, 56 },{ 57, CARG05, 0, 0, 98 },{ 57, CARG06, 0, 0, 98 },{ 57, 0, 0, 0, 58 },
	{ 58, BACKUP, 0, TAXI09_block,  0 },{ 58, TERM05, 0, 0,  12 },{ 58, HELI03, 0, 0,  32 },{ 58, CARG03, 0, 0,  26 },{ 58, HELITAKEOFF, 0, 0, 32 },{ 58, HELI01, 0, 0, 57 },{ 58, HELI02, 0, 0, 57 },{ 58, CARG01, 0, 0, 57 },{ 58, CARG02, 0, 0, 57 },{ 58, 0, 0, 0, 59 },
	{ 59, BACKUP, 0, TAXI10_block,  0 },{ 59, TERM06, 0, 0,  13 },{ 59, TERM05, 0, 0,  58 },{ 59, HELI01, 0, 0,  58 },{ 59, HELI02, 0, 0, 58 },{ 59, HELI03, 0, 0, 58 },{ 59, CARG01, 0, 0, 58 },{ 59, CARG02, 0, 0, 58 },{ 59, CARG03, 0, 0, 58 },{ 59, HELITAKEOFF, 0, 0, 58 },{ 59, 0, 0, 0, 60 },
	{ 60, BACKUP, 0, TAXI11_block,  0 },{ 60, TERM07, 0, 0,  14 },{ 60, HANGAR, 0, 0,   5 },{ 60, TERM08, 0, 0,  61 },{ 60, TERM05, 0, 0, 59 },{ 60, TERM06, 0, 0, 59 },{ 60, HELI01, 0, 0, 59 },{ 60, HELI02, 0, 0, 59 },{ 60, HELI03, 0, 0, 59 },{ 60, CARG01, 0, 0, 59 },{ 60, CARG02, 0, 0, 59 },{ 60, CARG03, 0, 0, 59 },{ 60, HELITAKEOFF, 0, 0, 59 },{ 60, 0, 0, 0, 62 },
	{ 61, BACKUP, 0, TAXI12_block,  0 },{ 61, TERM08, 0, 0,  15 },{ 61, HANGAR, 0, 0,  60 },{ 61, 0, 0, 0, 62 },
	{ 62,      0, 0, TAXI13_block, 63 },
	// Airplane Takeoff - Runway02
	{ 63, TAKEOFF, RUNW02_block, TAXI28_block, 64 },
	{ 64, 0, 0, 0, 65 },
	{ 65, STARTTAKEOFF, 0, 0, 66 },
	{ 66, ENDTAKEOFF, 0, 0, 0 },{ 66, 0, 0, 0, 37 },   // End Airport control of airplane.
	// Helicopter -- takeoff
	{ 67, HELITAKEOFF, 0, 0, 0 },
	{ 68, HELITAKEOFF, 0, 0, 0 },
	{ 69, HELITAKEOFF, 0, 0, 0 },
	// End Pod #1
	// Start of Pod #2
	// Airplane Landing Runway03
	{ 70, PRELANDING, RUNW03_block, 0, 71 },{ 70, LANDING, 0, 0, 71 },
	{ 71, LANDING, RUNW03_block, 0, 72 },{ 71, FLYING, 0, 0, 107 },{ 71, 0, 0, 0, 72 },
	{ 72, ENDLANDING, 0, TAXI14_block, 77 },{ 72, 0, 0, 0, 77 },{ 72, TERM15, 0, 0, 0 },{ 72, TERM13, 0, 0, 0 },{ 72, TERM11, 0, 0, 0 },{ 72, TERM09, 0, 0, 0 },{ 72, TERM16, 0, 0, 0 },{ 72, TERM14, 0, 0, 0 },{ 72, TERM12, 0, 0, 0 },{ 72, TERM10, 0, 0, 0 },{ 72, CARG04, 0, 0, 0 },{ 72, CARG05, 0, 0, 0 },{ 72, CARG06, 0, 0, 0 },
	{ 73, 0, 0, TAXI14_block, 77 }, //Unused
	// Airplane Landing Runway04
	{ 74, PRELANDING, RUNW04_block, TAXI22_block | TAXI23_block | TAXI24_block | TAXI26_block, 75 },{ 74, LANDING, 0, 0, 75 },
	{ 75, LANDING, RUNW04_block, 0, 76 },{ 75, FLYING, 0, 0, 92 },{ 75, 0, 0, 0, 76 },
	{ 76, ENDLANDING, 0, TAXI29_block, 77 },{ 76, 0, 0, 0, 77 },{ 76, TERM15, 0, 0, 0 },{ 76, TERM13, 0, 0, 0 },{ 76, TERM11, 0, 0, 0 },{ 76, TERM09, 0, 0, 0 },{ 76, TERM16, 0, 0, 0 },{ 76, TERM14, 0, 0, 0 },{ 76, TERM12, 0, 0, 0 },{ 76, TERM10, 0, 0, 0 },{ 76, CARG04, 0, 0, 0 },{ 76, CARG05, 0, 0, 0 },{ 76, CARG06, 0, 0, 0 },
	// End Landing for Runways 3 & 4
	{ 77,      0, 0, TAXI15_block, 78 },
	// Airplane & Helicopter Taxi  (Main Airport flow)
	{ 78,    255, 0, TAXI16_block,  0 },{ 78, HANGAR, 0, 0,   6 },{ 78, TERM09, 0, 0,  79 },{ 78, 0, 0, 0, 80 },
	{ 79, BACKUP, 0, TAXI17_block,  0 },{ 79, TERM09, 0, 0,  16 },{ 79, 0, 0, 0, 80 },
	{ 80, BACKUP, 0, TAXI18_block,  0 },{ 80, TERM10, 0, 0,  17 },{ 80, HANGAR, 0, 0,   6 },{ 80, TERM09, 0, 0, 79 },{ 80, 0, 0, 0, 81 },
	{ 81, BACKUP, 0, TAXI19_block,  0 },{ 81, TERM11, 0, 0,  18 },{ 81, HANGAR, 0, 0,  80 },{ 81, 0, 0, 0, 82 },
	{ 82, BACKUP, 0, TAXI20_block,  0 }, { 82, TERM12, 0, 0,  19 }, { 82, HELI04, 0, 0,  33 }, { 82, CARG04, 0, 0,  27 }, { 82, HELITAKEOFF, 0, 0, 33 }, { 82, 0, 0, 0, 83 },
	{ 83, BACKUP, 0, TAXI21_block,  0 },{ 83, TERM05, 0, 0, 100 },{ 83, HELI05, 0, 0,  34 },{ 83, CARG05, 0, 0,  28 },{ 83, HELI04, 0, 0, 82 },{ 83, CARG04, 0, 0, 82 },{ 83, TERM06, 0, 0, 100 },{ 83, TERM07, 0, 0, 100 },{ 83, TERM08, 0, 0, 100 },{ 83, CARG02, 0, 0, 100 },{ 83, CARG03, 0, 0, 100 },{ 83, 0, 0, 0, 84 },
	{ 84, BACKUP, 0, TAXI22_block,  0 },{ 84, TERM13, 0, 0,  20 },{ 84, HELI06, 0, 0,  35 },{ 84, CARG06, 0, 0,  29 },{ 84, HELI04, 0, 0, 83 },{ 84, HELI05, 0, 0, 83 },{ 84, CARG04, 0, 0, 83 },{ 84, CARG05, 0, 0, 83 },{ 84, HELITAKEOFF, 0, 0, 35 },{ 84, 0, 0, 0, 85 },
	{ 85, BACKUP, 0, TAXI23_block,  0 },{ 85, TERM14, 0, 0,  21 },{ 85, TERM13, 0, 0,  84 },{ 85, HELI04, 0, 0,  84 },{ 85, HELI05, 0, 0, 84 },{ 85, HELI06, 0, 0, 84 },{ 85, CARG04, 0, 0, 84 },{ 85, CARG05, 0, 0, 84 },{ 85, CARG06, 0, 0, 84 },{ 85, HELITAKEOFF, 0, 0, 84 },{ 85, 0, 0, 0, 86 },
	{ 86, BACKUP, 0, TAXI24_block,  0 },{ 86, TERM15, 0, 0,  22 },{ 86, HANGAR, 0, 0,   7 },{ 86, TERM16, 0, 0,  87 },{ 86, TERM13, 0, 0, 85 },{ 86, TERM14, 0, 0, 85 },{ 86, HELI04, 0, 0, 85 },{ 86, HELI05, 0, 0, 85 },{ 86, HELI06, 0, 0, 85 },{ 86, CARG04, 0, 0, 85 },{ 86, CARG05, 0, 0, 85 },{ 86, CARG06, 0, 0, 85 },{ 86, HELITAKEOFF, 0, 0, 85 },{ 86, 0, 0, 0, 88 },
	{ 87, BACKUP, 0, TAXI25_block,  0 },{ 87, TERM16, 0, 0,  23 },{ 87, HANGAR, 0, 0,  86 },{ 87, 0, 0, 0, 88 },
	{ 88,      0, 0, TAXI26_block, 89 },
	// Airplane Takeoff - Runway 04
	{ 89, TAKEOFF, RUNW04_block, TAXI29_block, 90 },
	{ 90, 0, 0, 0, 91 },
	{ 91, STARTTAKEOFF, 0, 0, 92 },
	{ 92, ENDTAKEOFF, 0, 0, 0 },{ 92, 0, 0, 0, 39 },   // End Airport control of airplane.
	// Helicopter Position for Helipads.
	{ 93, 0, HANG03_block, 0, 94 },
	{ 94, 0, 0, 0, 6 },
	// Helicopter -- takeoff
	{ 95, HELITAKEOFF, 0, 0, 0 },
	{ 96, HELITAKEOFF, 0, 0, 0 },
	{ 97, HELITAKEOFF, 0, 0, 0 },
	// End Pod #2
	// Crossover between Pod1 and Pod2
	{ 98, 255, 0, TAXI27_block, 0 },{ 98, TERM13, 0, 0, 99 },{ 98, TERM14, 0, 0, 99 },{ 98, TERM15, 0, 0, 99 },{ 98, TERM16, 0, 0, 99 },{ 98, CARG05, 0, 0, 99 },{ 98, CARG06, 0, 0, 99 },{ 98, 0, 0, 0,  57 },
	{ 99, 255, 0, TAXI27_block, 0 },{ 99, TERM13, 0, 0, 100 },{ 99, TERM14, 0, 0, 100 },{ 99, TERM15, 0, 0, 100 },{ 99, TERM16, 0, 0, 100 },{ 99, TERM05, 0, 0, 98 },{ 99, TERM06, 0, 0, 98 },{ 99, TERM07, 0, 0, 98 },{ 99, TERM08, 0, 0, 98 },{ 99, CARG05, 0, 0, 100 },{ 99, CARG06, 0, 0, 100 },{ 99, CARG02, 0, 0, 98 },{ 99, CARG03, 0, 0, 98 },
	{ 100, 255, 0, TAXI27_block, 0 },{ 100, TERM05, 0, 0, 99 },{ 100, TERM06, 0, 0, 99 },{ 100, TERM07, 0, 0, 99 },{ 100, TERM08, 0, 0, 99 },{ 100, CARG02, 0, 0, 99 },{ 100, CARG03, 0, 0, 99 },{ 100, 0, 0, 0,  83 },
	// Added Holding Pattern spots.
	{ 101, FLYING, 0, 0, 102 },{ 101, PRELANDING, 0, 0, 48 }, { 101, HELILANDING, 0, 0, 40 }, { 101, 0, 0, 0, 102 },
	{ 102, FLYING, 0, 0,  36 },{ 102, PRELANDING, 0, 0, 44 }, { 102, HELILANDING, 0, 0, 40 }, { 102, 0, 0, 0,  36 },
	{ 103, FLYING, 0, 0, 104 },{ 103, PRELANDING, 0, 0, 74 }, { 103, HELILANDING, 0, 0, 40 }, { 103, 0, 0, 0, 104 },
	{ 104, FLYING, 0, 0,  38 },{ 104, PRELANDING, 0, 0, 70 }, { 104, HELILANDING, 0, 0, 40 }, { 104, 0, 0, 0,  38 },
	// Helicopter -- Entry .  Keeps it from messing up the planes.
	{ 105, FLYING, 0, 0, 105 },{ 105, HELILANDING, 0, 0, 40 },
	// Abort Landings
	{ 106, 0, 0, 0, 37 },
	{ 107, 0, 0, 0, 39 },
	// Helicopter Positioning
	{ 108, 0, 0, 0, 114 },
	{ 109, 0, 0, 0, 115 },
	{ 110, 0, 0, 0, 116 },
	{ 111, 0, 0, 0, 117 },
	{ 112, 0, 0, 0, 118 },
	{ 113, 0, 0, 0, 119 },
	//  Helicopter landing
	{ 114,   0, 0, 0, 30 },
	{ 115,   0, 0, 0, 31 },
	{ 116,   0, 0, 0, 32 },
	{ 117,   0, 0, 0, 33 },
	{ 118,   0, 0, 0, 34 },
	{ 119,   0, 0, 0, 35 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Terminus3
static const HangarTileTable _airport_depots_terminus3[] = { { { 8, 8 }, DIR_NE, 0 },{ { 12, 8 }, DIR_SW, 1 },{ { 12, 2 }, DIR_SW, 2 },{ { 8, 2 }, DIR_NE, 3 },{ { 2, 3 }, DIR_NW, 4 },{ { 2, 7 }, DIR_SE, 5 } };
static const TerminalTileTable _airport_terminals_terminus3[] = { { { 6, 6 }, TERM01_block },{ { 7, 6 }, TERM02_block },{ { 8, 6 }, TERM03_block },{ { 9, 6 }, TERM04_block },{ { 11, 6 }, TERM05_block },{ { 12, 6 }, TERM06_block },{ { 13, 6 }, TERM07_block },{ { 14, 6 }, TERM08_block },
{ { 14, 4 }, TERM09_block },{ { 13, 4 }, TERM10_block },{ { 12, 4 }, TERM11_block },{ { 11, 4 }, TERM12_block },{ { 9, 4 }, TERM13_block },{ { 8, 4 }, TERM14_block },{ { 7, 4 }, TERM15_block },{ { 6, 4 }, TERM16_block },
{ { 4, 0 }, TERM17_block },{ { 4, 1 }, TERM18_block },{ { 4, 2 }, TERM19_block },{ { 4, 4 }, TERM20_block },{ { 4, 5 }, TERM21_block },{ { 4, 6 }, TERM22_block },{ { 4, 8 }, TERM23_block },{ { 4, 9 }, TERM24_block },{ { 4,10 }, TERM25_block },
{ { 9, 8 }, CARG01_block },{ { 10, 8 }, CARG02_block },{ { 11, 8 }, CARG03_block },{ { 11, 2 }, CARG04_block },{ { 10, 2 }, CARG05_block },{ { 9, 2 }, CARG06_block },{ { 2, 4 }, CARG07_block },{ { 2, 5 }, CARG08_block },{ { 2, 6 }, CARG09_block } };
static const RunwayTerminalTable _airport_runways_terminus3[] = { { TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | TERM07_block | TERM08_block | CARG01_block | CARG02_block | CARG03_block, RUNW01_block | RUNW02_block, 0 },
{ TERM09_block | TERM10_block | TERM11_block | TERM12_block | TERM13_block | TERM14_block | TERM15_block | TERM16_block | CARG04_block | CARG05_block | CARG06_block, RUNW03_block | RUNW04_block, 0 },
{ TERM17_block | TERM18_block | TERM19_block | TERM20_block | TERM21_block | TERM22_block | TERM23_block | TERM24_block | TERM25_block | CARG07_block | CARG08_block | CARG09_block, RUNW05_block | RUNW06_block, 0 } };
static const byte _airport_terminal_terminus3[] = { 1, 25 };
static const byte _airport_entries_terminus3[] = { 56, 55, 58, 57 };
static const byte _airport_helientry_terminus3[] = { 56, 55, 58, 57 };
static const AirportFTAbuildup _airport_fta_terminus3[] = {
	// Airplane & Helicopter Hangars
	{ 0, HANGAR, 0, 0,  6 },{ 0, 0, 0, 0,  6 },{ 0, TERM07, 0, 0,  6 },{ 0, TERM05, 0, 0,  6 },{ 0, TERM03, 0, 0,  6 },{ 0, TERM01, 0, 0,  6 },{ 0, TERM08, 0, 0,  6 },{ 0, TERM06, 0, 0 , 6 },{ 0, TERM04, 0, 0,  6 },{ 0, TERM02, 0, 0,  6 },{ 0, TERM13, 0, 0,  6 },{ 0, TERM23, 0, 0,  6 },{ 0, TERM14, 0, 0,  6 },{ 0, TERM24, 0, 0,  6 },{ 0, TERM15, 0, 0,  6 },{ 0, TERM25, 0, 0,  6 },{ 0, TERM16, 0, 0,  6 },{ 0, HELI01, 0, 0,  6 },{ 0, HELI02, 0, 0, 6 },{ 0, HELI03, 0, 0, 6 },{ 0, CARG01, 0, 0,  6 },{ 0, CARG02, 0, 0, 6 },{ 0, CARG03, 0, 0, 6 },{ 0, CARG05, 0, 0, 6 },{ 0, CARG06, 0, 0, 6 },
	{ 1, HANGAR, 0, 0,  7 },{ 1, 0, 0, 0,  7 },{ 1, TERM08, 0, 0,  7 },{ 1, TERM07, 0, 0,  7 },{ 1, TERM06, 0, 0,  7 },{ 1, TERM05, 0, 0,  7 },{ 1, HELI03, 0, 0,  7 },{ 1, HELI02, 0, 0,  7 },{ 1, HELI01, 0, 0,  7 },{ 1, CARG03, 0, 0,  7 },{ 1, CARG02, 0, 0,  7 },{ 1, CARG01, 0, 0,  7 },
	{ 2, HANGAR, 0, 0,  8 },{ 2, 0, 0, 0,  8 },{ 2, TERM15, 0, 0,  8 },{ 2, TERM13, 0, 0,  8 },{ 2, TERM11, 0, 0,  8 },{ 2, TERM09, 0, 0,  8 },{ 2, TERM16, 0, 0,  8 },{ 2, TERM14, 0, 0,  8 },{ 2, TERM12, 0, 0 , 8 },{ 2, TERM10, 0, 0,  8 },{ 2, TERM05, 0, 0,  8 },{ 2, TERM20, 0, 0,  8 },{ 2, TERM06, 0, 0,  8 },{ 2, TERM21, 0, 0,  8 },{ 2, TERM07, 0, 0,  8 },{ 2, TERM22, 0, 0,  8 },{ 2, TERM08, 0, 0,  8 },{ 2, HELI04, 0, 0,  8 },{ 2, HELI05, 0, 0, 8 },{ 2, HELI06, 0, 0, 8 },{ 2, CARG04, 0, 0,  8 },{ 2, CARG05, 0, 0, 8 },{ 2, CARG06, 0, 0, 8 },{ 2, CARG07, 0, 0, 8 },{ 2, CARG08, 0, 0, 8 },{ 2, CARG09, 0, 0, 8 },
	{ 3, HANGAR, 0, 0,  9 },{ 3, 0, 0, 0,  9 },{ 3, TERM16, 0, 0,  9 },{ 3, TERM15, 0, 0,  9 },{ 3, TERM14, 0, 0,  9 },{ 3, TERM13, 0, 0,  9 },{ 3, HELI06, 0, 0,  9 },{ 3, HELI05, 0, 0,  9 },{ 3, HELI04, 0, 0,  9 },{ 3, CARG07, 0, 0,  9 },{ 3, CARG08, 0, 0,  9 },{ 3, CARG09, 0, 0,  9 },{ 3, CARG06, 0, 0,  9 },{ 3, CARG05, 0, 0,  9 },{ 3, CARG04, 0, 0,  9 },
	{ 4, HANGAR, 0, 0, 10 },{ 4, 0, 0, 0, 10 },{ 4, TERM25, 0, 0, 10 },{ 4, TERM23, 0, 0, 10 },{ 4, TERM21, 0, 0, 10 },{ 4, TERM19, 0, 0, 10 },{ 4, TERM17, 0, 0, 10 },{ 4, TERM24, 0, 0, 10 },{ 4, TERM22, 0, 0, 10 },{ 4, TERM20, 0, 0, 10 },{ 4, TERM18, 0, 0, 10 },{ 4, TERM01, 0, 0, 10 },{ 4, TERM02, 0, 0, 10 },{ 4, TERM03, 0, 0, 10 },{ 4, TERM04, 0, 0, 10 },{ 4, HELI07, 0, 0, 10 },{ 4, HELI08, 0, 0, 10 },{ 4, HELI09, 0, 0, 10 },{ 4, CARG07, 0, 0, 10 },{ 4, CARG08, 0, 0, 10 },{ 4, CARG09, 0, 0, 10 },{ 4, CARG01, 0, 0, 10 },{ 4, CARG02, 0, 0, 10 },{ 4, CARG03, 0, 0, 10 },
	{ 5, HANGAR, 0, 0, 11 },{ 5, 0, 0, 0, 11 },{ 5, TERM25, 0, 0, 11 },{ 5, TERM24, 0, 0, 11 },{ 5, TERM23, 0, 0, 11 },{ 5, TERM22, 0, 0, 11 },{ 5, HELI09, 0, 0, 11 },{ 5, HELI08, 0, 0, 11 },{ 5, HELI07, 0, 0, 11 },{ 5, CARG01, 0, 0, 11 },{ 5, CARG02, 0, 0, 11 },{ 5, CARG03, 0, 0, 11 },{ 5, CARG09, 0, 0, 11 },{ 5, CARG08, 0, 0, 11 },{ 5, CARG07, 0, 0, 11 },
	{ 6, 255, HANG01_block, 0, 0 },{ 6, HANGAR, 0, 0, 0 },{ 6, TERM01, 0, TAXI04_block | TAXI05_block, 73 },{ 6, TERM23, 0, TAXI04_block | TAXI05_block | TAXI46_block, 73 },{ 6, TERM24, 0, TAXI04_block | TAXI05_block | TAXI46_block, 73 },{ 6, TERM25, 0, TAXI04_block | TAXI05_block | TAXI46_block, 73 },{ 6, TERM13, 0, TAXI27_block, 73 },{ 6, TERM14, 0, TAXI27_block, 73 },{ 6, TERM15, 0, TAXI27_block, 73 },{ 6, TERM16, 0, TAXI27_block, 73 },{ 6, CARG05, 0, TAXI27_block, 73 },{ 6, CARG06, 0, TAXI27_block, 73 },{ 6, HELITAKEOFF, HELI01_block, 0, 73 }, { 6, 0, 0, 0, 73 },
	{ 7, 255, HANG02_block, 0, 0 },{ 7, HANGAR, 0, 0, 1 },{ 7, TERM05, 0, TAXI11_block | TAXI10_block | TAXI09_block, 79 },{ 7, TERM06, 0, TAXI11_block | TAXI10_block, 79 },{ 7, TERM07, 0, TAXI11_block, 79 },{ 7, TERM08, 0, TAXI11_block, 79 },{ 7, HELI03, 0, TAXI11_block | TAXI10_block | TAXI09_block, 79 },{ 7, HELI02, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block, 79 },{ 7, HELI01, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block | TAXI07_block, 79 },{ 7, CARG03, 0, TAXI11_block | TAXI10_block | TAXI09_block, 79 },{ 7, CARG02, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block, 79 },{ 7, CARG01, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block | TAXI07_block, 79 },{ 7, HELITAKEOFF, HELI03_block, TAXI11_block | TAXI10_block | TAXI09_block, 79 },{ 7, TAKEOFF, 0, TAXI13_block, 81 },{ 7, 0, 0, TAXI13_block, 81 },
	{ 8, 255, HANG03_block, 0, 0 },{ 8, HANGAR, 0, 0, 2 },{ 8, TERM09, 0, TAXI18_block | TAXI17_block, 99 },{ 8, TERM05, 0, TAXI27_block, 99 },{ 8, TERM06, 0, TAXI27_block, 99 },{ 8, TERM07, 0, TAXI27_block, 99 },{ 8, TERM08, 0, TAXI27_block, 99 },{ 8, TERM20, 0, TAXI45_block, 99 },{ 8, TERM21, 0, TAXI45_block, 99 },{ 8, TERM22, 0, TAXI45_block, 99 },{ 8, CARG07, 0, TAXI45_block, 99 },{ 8, CARG08, 0, TAXI45_block, 99 },{ 8, CARG09, 0, TAXI45_block, 99 },{ 8, HELITAKEOFF, HELI04_block, 0, 99 }, { 8, 0, 0, 0, 99 },
	{ 9, 255, HANG04_block, 0, 0 },{ 9, HANGAR, 0, 0, 3 },{ 9, TERM13, 0, TAXI24_block | TAXI23_block | TAXI22_block, 105 },{ 9, TERM14, 0, TAXI24_block | TAXI23_block, 105 },{ 9, TERM15, 0, TAXI24_block, 105 },{ 9, TERM16, 0, TAXI24_block, 105 },{ 9, HELI06, 0, TAXI24_block | TAXI23_block | TAXI22_block, 105 },{ 9, HELI05, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI21_block, 105 },{ 9, HELI04, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI21_block | TAXI20_block, 105 },{ 9, CARG06, 0, TAXI24_block | TAXI23_block | TAXI22_block, 105 },{ 9, CARG05, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI21_block, 105 },{ 9, CARG04, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI21_block | TAXI20_block, 105 },{ 9, HELITAKEOFF, HELI06_block, TAXI24_block | TAXI23_block | TAXI22_block, 105 },{ 9, CARG07, 0, TAXI24_block | TAXI45_block, 105 },{ 9, CARG08, 0, TAXI24_block | TAXI45_block, 105 },{ 9, CARG09, 0, TAXI24_block | TAXI45_block, 105 },{ 9, TAKEOFF, 0, TAXI26_block, 107 },{ 9, 0, 0, TAXI26_block, 107 },
	{ 10, 255, HANG05_block, 0, 0 },{ 10, HANGAR, 0, 0, 4 },{ 10, TERM18, 0, TAXI30_block | TAXI33_block, 125 },{ 10, TERM17, 0, TAXI30_block | TAXI31_block | TAXI32_block, 125 },{ 10, TERM01, 0, TAXI46_block, 129 },{ 10, TERM02, 0, TAXI46_block, 129 },{ 10, TERM03, 0, TAXI46_block, 129 },{ 10, TERM04, 0, TAXI46_block, 129 },{ 10, CARG01, 0, TAXI46_block, 129 },{ 10, CARG02, 0, TAXI46_block, 129 },{ 10, CARG03, 0, TAXI46_block, 129 },{ 10, HELITAKEOFF, HELI07_block, 0, 129 }, { 10, 0, 0, 0, 129 },
	{ 11, 255, HANG06_block, 0, 0 },{ 11, HANGAR, 0, 0, 5 },{ 11, TERM22, 0, TAXI40_block | TAXI39_block | TAXI38_block, 135 },{ 11, TERM23, 0, TAXI40_block, 135 },{ 11, TERM24, 0, TAXI40_block, 135 },{ 11, TERM25, 0, TAXI40_block, 135 },{ 11, HELI09, 0,  TAXI40_block | TAXI39_block | TAXI38_block, 135 },{ 11, HELI08, 0,  TAXI40_block | TAXI39_block | TAXI38_block | TAXI37_block, 135 },{ 11, HELI07, 0,  TAXI40_block | TAXI39_block | TAXI38_block | TAXI37_block | TAXI36_block, 135 },{ 11, CARG01, 0, TAXI39_block | TAXI40_block | TAXI46_block, 135 },{ 11, CARG02, 0, TAXI39_block | TAXI40_block | TAXI46_block, 135 },{ 11, CARG03, 0, TAXI39_block | TAXI40_block | TAXI46_block, 135 },{ 11, CARG09, 0,  TAXI40_block | TAXI39_block | TAXI38_block, 135 },{ 11, CARG08, 0,  TAXI40_block | TAXI39_block | TAXI38_block | TAXI37_block, 135 },{ 11, CARG07, 0,  TAXI40_block | TAXI39_block | TAXI38_block | TAXI37_block | TAXI36_block, 135 },{ 11, HELITAKEOFF, HELI09_block,  TAXI40_block | TAXI39_block | TAXI38_block, 135 },{ 11, TAKEOFF, 0, TAXI44_block, 139 },{ 11, 0, 0, TAXI44_block, 139 },
	// Airplane Terminals
	{ 12, TERM01, TERM01_block, 0,  72 },{ 12, HANGAR, HANG01_block, TAXI05_block | TAXI04_block,  72 },{ 12, 0, 0, 0,  72 },
	{ 13, TERM02, TERM02_block, 0,  73 },{ 13, HANGAR, HANG01_block, TAXI05_block,  73 },{ 13, 0, 0, 0,  73 },
	{ 14, TERM03, TERM03_block, 0,  74 },{ 14, HANGAR, HANG01_block, TAXI06_block | TAXI05_block, 74 },{ 14, 0, 0, 0, 74 },
	{ 15, TERM04, TERM04_block, 0,  75 },{ 15, HANGAR, 0, 0,  75 },{ 15, 0, 0, 0,  75 },
	{ 16, TERM05, TERM05_block, 0,  77 },{ 16, HANGAR, 0, 0,  77 },{ 16, 0, 0, 0,  77 },
	{ 17, TERM06, TERM06_block, 0,  78 },{ 17, HANGAR, 0, 0,  78 },{ 17, 0, 0, 0,  78 },
	{ 18, TERM07, TERM07_block, 0,  79 },{ 18, HANGAR, 0, 0,  79 },{ 18, 0, 0, 0,  79 },
	{ 19, TERM08, TERM08_block, 0,  80 },{ 19, HANGAR, HANG02_block, TAXI11_block | TAXI12_block,  80 },{ 19, 0, 0, 0,  80 },
	{ 20, TERM09, TERM09_block, 0,  98 },{ 20, HANGAR, HANG03_block, TAXI18_block | TAXI17_block,  98 },{ 20, 0, 0, 0,  98 },
	{ 21, TERM10, TERM10_block, 0,  99 },{ 21, HANGAR, HANG03_block, TAXI18_block,  99 },{ 21, 0, 0, 0,  99 },
	{ 22, TERM11, TERM11_block, 0, 100 },{ 22, HANGAR, HANG03_block, TAXI19_block | TAXI18_block, 100 },{ 22, 0, 0, 0, 100 },
	{ 23, TERM12, TERM12_block, 0, 101 },{ 23, HANGAR, 0, 0,  101 },{ 23, 0, 0, 0,  101 },
	{ 24, TERM13, TERM13_block, 0, 103 },{ 24, HANGAR, 0, 0, 103 },{ 24, 0, 0, 0, 103 },
	{ 25, TERM14, TERM14_block, 0, 104 },{ 25, HANGAR, 0, 0, 104 },{ 25, 0, 0, 0, 104 },
	{ 26, TERM15, TERM15_block, 0, 105 },{ 26, HANGAR, 0, 0, 105 },{ 26, 0, 0, 0, 105 },
	{ 27, TERM16, TERM16_block, 0, 106 },{ 27, HANGAR, HANG04_block, TAXI24_block | TAXI25_block, 106 },{ 27, 0, 0, 0, 106 },
	{ 28, TERM17, TERM17_block, 0, 127 },{ 28, HANGAR, HANG05_block, TAXI32_block | TAXI33_block | TAXI34_block, 127 },{ 28, 0, 0, 0, 127 },
	{ 29, TERM18, TERM18_block, 0, 128 },{ 29, HANGAR, HANG05_block, TAXI33_block | TAXI34_block, 128 },{ 29, 0, 0, 0, 128 },
	{ 30, TERM19, TERM19_block, 0, 129 },{ 30, HANGAR, HANG05_block, TAXI34_block, 129 },{ 30, 0, 0, 0, 129 },
	{ 31, TERM20, TERM20_block, 0, 131 },{ 31, HANGAR, 0, 0, 131 },{ 31, 0, 0, 0, 131 },
	{ 32, TERM21, TERM21_block, 0, 132 },{ 32, HANGAR, 0, 0, 132 },{ 32, 0, 0, 0, 132 },
	{ 33, TERM22, TERM22_block, 0, 133 },{ 33, HANGAR, 0, 0, 133 },{ 33, 0, 0, 0, 133 },
	{ 34, TERM23, TERM23_block, 0, 135 },{ 34, HANGAR, HANG06_block, TAXI40_block, 135 },{ 34, 0, 0, 0, 135 },
	{ 35, TERM24, TERM24_block, 0, 136 },{ 35, HANGAR, HANG06_block, TAXI41_block | TAXI44_block, 136 },{ 35, 0, 0, 0, 136 },
	{ 36, TERM25, TERM25_block, 0, 137 },{ 36, HANGAR, HANG06_block, TAXI42_block | TAXI43_block | TAXI44_block, 137 },{ 36, 0, 0, 0, 137 },
	// Cargo Terminals (Uses Helipads)
	{ 37, CARG01, CARG01_block, 0,  75 },{ 37, HANGAR, 0, 0,  75 },{ 37, 0, 0, 0,  75 },
	{ 38, CARG02, CARG02_block, 0,  76 },{ 38, HANGAR, 0, 0,  76 },{ 38, 0, 0, 0,  76 },
	{ 39, CARG03, CARG03_block, 0,  77 },{ 39, HANGAR, 0, 0,  77 },{ 39, 0, 0, 0,  77 },
	{ 40, CARG04, CARG04_block, 0, 101 },{ 40, HANGAR, 0, 0, 101 },{ 40, 0, 0, 0, 101 },
	{ 41, CARG05, CARG05_block, 0, 102 },{ 41, HANGAR, 0, 0, 102 },{ 41, 0, 0, 0, 102 },
	{ 42, CARG06, CARG06_block, 0, 103 },{ 42, HANGAR, 0, 0, 103 },{ 42, 0, 0, 0, 103 },
	{ 43, CARG07, CARG07_block, 0, 131 },{ 43, HANGAR, 0, 0, 131 },{ 43, 0, 0, 0, 131 },
	{ 44, CARG08, CARG08_block, 0, 132 },{ 44, HANGAR, 0, 0, 132 },{ 44, 0, 0, 0, 132 },
	{ 45, CARG09, CARG09_block, 0, 133 },{ 45, HANGAR, 0, 0, 133 },{ 45, 0, 0, 0, 133 },
	// Helicopter Terminals
	{ 46, HELI01, HELI01_block, 0,  75 },{ 46, HANGAR, 0, 0,  75 },{ 46, HELITAKEOFF, 0, 0,  86 },
	{ 47, HELI02, HELI02_block, 0,  76 },{ 47, HANGAR, 0, 0,  76 },{ 47, HELITAKEOFF, 0, 0,  87 },
	{ 48, HELI03, HELI03_block, 0,  77 },{ 48, HANGAR, 0, 0,  77 },{ 48, HELITAKEOFF, 0, 0,  88 },
	{ 49, HELI04, HELI04_block, 0, 101 },{ 49, HANGAR, 0, 0, 101 },{ 49, HELITAKEOFF, 0, 0, 114 },
	{ 50, HELI05, HELI05_block, 0, 102 },{ 50, HANGAR, 0, 0, 102 },{ 50, HELITAKEOFF, 0, 0, 115 },
	{ 51, HELI06, HELI06_block, 0, 103 },{ 51, HANGAR, 0, 0, 103 },{ 51, HELITAKEOFF, 0, 0, 116 },
	{ 52, HELI07, HELI07_block, 0, 131 },{ 52, HANGAR, 0, 0, 131 },{ 52, HELITAKEOFF, 0, 0, 146 },
	{ 53, HELI08, HELI08_block, 0, 132 },{ 53, HANGAR, 0, 0, 132 },{ 53, HELITAKEOFF, 0, 0, 147 },
	{ 54, HELI09, HELI09_block, 0, 133 },{ 54, HANGAR, 0, 0, 133 },{ 54, HELITAKEOFF, 0, 0, 148 },
	// Airplane In Air - Holding Pattern
	{ 55, 0, 0, 0, 160 },  //  56
	{ 56, 0, 0, 0, 158 },
	{ 57, 0, 0, 0,  58 },
	{ 58, 0, 0, 0, 156 },
	// Helicopter -- Holding Pattern
	{ 59, HELILANDING, 0, 0, 60 },
	{ 60, HELIENDLANDING, 0, 0, 61 }, { 60, FLYING, 0, 0, 55 }, { 60, HELI09, 0, 0, 174 },{ 60, HELI06, 0, 0, 171 },{ 60, HELI03, 0, 0, 168 },{ 60, HELI08, 0, 0, 173 },{ 60, HELI05, 0, 0, 170 },{ 60, HELI02, 0, 0, 167 },{ 60, HELI07, 0, 0, 172 },{ 60, HELI04, 0, 0, 169 },{ 60, HELI01, 0, 0, 166 },{ 60, HANGAR, 0, 0, 61 },{ 60, 0, 0, 0, 61 },{ 60, 0, 0, 0, 61 },
	//Helicopter -- Hangar
	{ 61, 0, HANG01_block, 0, 62 }, // need to go to hangar when waiting in air
	{ 62, 0, HANG01_block, 0,  6 },
	// Start of Pod #1
	// Airplane Landing Runway01
	{ 63, PRELANDING, RUNW01_block, 0, 64 },{ 63, LANDING, 0, 0, 64 },
	{ 64, LANDING, RUNW01_block, 0, 65 },{ 64, FLYING, 0, 0, 163 },{ 64, 0, 0, 0, 65 },
	{ 65, ENDLANDING, 0, TAXI01_block, 70 },{ 65, 0, 0, 0, 70 },{ 65, TERM07, 0, 0, 0 },{ 65, TERM05, 0, 0, 0 },{ 65, TERM03, 0, 0, 0 },{ 65, TERM01, 0, 0, 0 },{ 65, TERM08, 0, 0, 0 },{ 65, TERM06, 0, 0, 0 },{ 65, TERM04, 0, 0, 0 },{ 65, TERM02, 0, 0, 0 },{ 65, CARG01, 0, 0, 0 },{ 65, CARG02, 0, 0, 0 },{ 65, CARG03, 0, 0, 0 },
	{ 66, 0, 0, TAXI01_block, 70 },  //Unused
	// Airplane Landing Runway02
	{ 67, PRELANDING, RUNW02_block, TAXI09_block | TAXI10_block | TAXI11_block | TAXI13_block, 68 },{ 67, LANDING, 0, 0, 68 },
	{ 68, LANDING, RUNW02_block, 0, 69 },{ 68, FLYING, 0, 0, 85 },{ 68, 0, 0, 0, 69 },
	{ 69, ENDLANDING, 0, TAXI47_block, 70 },{ 69, 0, 0, 0, 70 },{ 69, TERM07, 0, 0, 0 },{ 69, TERM05, 0, 0, 0 },{ 69, TERM03, 0, 0, 0 },{ 69, TERM01, 0, 0, 0 },{ 69, TERM08, 0, 0, 0 },{ 69, TERM06, 0, 0, 0 },{ 69, TERM04, 0, 0, 0 },{ 69, TERM02, 0, 0, 0 },{ 69, CARG01, 0, 0, 0 },{ 69, CARG02, 0, 0, 0 },{ 69, CARG03, 0, 0, 0 },
	// End Landing for Runways 1 & 2
	{ 70,      0, 0, TAXI02_block, 71 },
	// Airplane & Helicopter Taxi  (Main Airport flow)
	{ 71,    255, 0, TAXI03_block, 0 },{ 71, HANGAR, 0, 0,   6 },{ 71, TERM01, 0, 0,  72 },{ 71, 0, 0, 0, 73 },
	{ 72, BACKUP, 0, TAXI04_block, 0 },{ 72, TERM01, 0, 0,  12 },{ 72, TERM23, 0, 0, 154 },{ 72, TERM24, 0, 0, 154 },{ 72, TERM25, 0, 0, 154 },{ 72, 0, 0, 0, 73 },
	{ 73, BACKUP, 0, TAXI05_block, 0 },{ 73, TERM02, 0, 0,  13 },{ 73, TERM23, 0, 0,  72 },{ 73, TERM24, 0, 0,  72 },{ 73, TERM25, 0, 0,  72 },{ 73, TERM01, 0, 0, 72 },{ 73, HANGAR, 0, 0,   6 },{ 73, 0, 0, 0, 74 },
	{ 74, BACKUP, 0, TAXI06_block, 0 },{ 74, TERM03, 0, 0,  14 },{ 74, HANGAR, 0, 0,  73 },{ 74, 0, 0, 0, 75 },
	{ 75, BACKUP, 0, TAXI07_block, 0 },{ 75, TERM04, 0, 0,  15 },{ 75, HELI01, 0, 0,  46 },{ 75, CARG01, 0, 0,  37 },{ 75, HELITAKEOFF, 0, 0, 46 }, { 75, 0, 0, 0, 76 },
	{ 76, BACKUP, 0, TAXI08_block, 0 },{ 76, TERM13, 0, 0, 149 },{ 76, HELI02, 0, 0,  47 },{ 76, CARG02, 0, 0,  38 },{ 76, TERM14, 0, 0, 149 },{ 76, TERM15, 0, 0, 149 },{ 76, TERM16, 0, 0, 149 },{ 76, HELI01, 0, 0, 75 },{ 76, CARG01, 0, 0, 75 },{ 76, CARG05, 0, 0, 149 },{ 76, CARG06, 0, 0, 149 },{ 76, 0, 0, 0, 77 },
	{ 77, BACKUP, 0, TAXI09_block, 0 },{ 77, TERM05, 0, 0,  16 },{ 77, HELI03, 0, 0,  48 },{ 77, CARG03, 0, 0,  39 },{ 77, HELI01, 0, 0,  76 },{ 77, HELI02, 0, 0,  76 },{ 77, CARG01, 0, 0,  76 },{ 77, CARG02, 0, 0, 76 },{ 77, HELITAKEOFF, 0, 0,  48 },{ 77, 0, 0, 0, 78 },
	{ 78, BACKUP, 0, TAXI10_block, 0 },{ 78, TERM06, 0, 0,  17 },{ 78, TERM05, 0, 0,  77 },{ 78, HELI01, 0, 0,  77 },{ 78, HELI02, 0, 0,  77 },{ 78, HELI03, 0, 0,  77 },{ 78, CARG01, 0, 0,  77 },{ 78, CARG02, 0, 0, 77 },{ 78, CARG03, 0, 0, 77 },{ 78, HELITAKEOFF, 0, 0, 77 },{ 78, 0, 0, 0, 79 },
	{ 79, BACKUP, 0, TAXI11_block, 0 },{ 79, TERM07, 0, 0,  18 },{ 79, HANGAR, 0, 0,   7 },{ 79, TERM08, 0, 0,  80 },{ 79, TERM05, 0, 0,  78 },{ 79, TERM06, 0, 0,  78 },{ 79, HELI01, 0, 0,  78 },{ 79, HELI02, 0, 0, 78 },{ 79, HELI03, 0, 0, 78 },{ 79, CARG01, 0, 0, 78 },{ 79, CARG02, 0, 0, 78 },{ 79, CARG03, 0, 0, 78 },{ 79, HELITAKEOFF, 0, 0, 78 },{ 79, 0, 0, 0, 81 },
	{ 80, BACKUP, 0, TAXI12_block, 0 },{ 80, TERM08, 0, 0,  19 },{ 80, HANGAR, 0, 0,  79 },{ 80, 0, 0, 0, 81 },
	{ 81,    255, 0, TAXI13_block, 0 },{ 81, 0, 0, 0, 82 },
	// Airplane Takeoff - Runway02
	{ 82, TAKEOFF, RUNW02_block, TAXI47_block, 83 },
	{ 83, 0, 0, 0, 84 },
	{ 84, STARTTAKEOFF, 0, 0, 85 },
	{ 85, ENDTAKEOFF, 0, 0, 0 },{ 85, 0, 0, 0, 56 },   // End Airport control of airplane.
	// Helicopter -- takeoff
	{ 86, HELITAKEOFF, 0, 0, 0 },
	{ 87, HELITAKEOFF, 0, 0, 0 },
	{ 88, HELITAKEOFF, 0, 0, 0 },
	// End Pod #1
	// Start of Pod #2
	// Airplane Landing Runway03
	{ 89, PRELANDING, RUNW03_block, 0, 90 },{ 89, LANDING, 0, 0, 90 },
	{ 90, LANDING, RUNW03_block, 0, 91 },{ 90, FLYING, 0, 0, 164 },{ 90, 0, 0, 0, 91 },
	{ 91, ENDLANDING, 0, TAXI14_block, 96 },{ 91, 0, 0, 0, 96 },{ 91, TERM15, 0, 0, 0 },{ 91, TERM13, 0, 0, 0 },{ 91, TERM11, 0, 0, 0 },{ 91, TERM09, 0, 0, 0 },{ 91, TERM16, 0, 0, 0 },{ 91, TERM14, 0, 0, 0 },{ 91, TERM12, 0, 0, 0 },{ 91, TERM10, 0, 0, 0 },
	{ 92, 0, 0, TAXI14_block, 96 }, //Unused
	// Airplane Landing Runway04
	{ 93, PRELANDING, RUNW04_block, TAXI22_block | TAXI23_block | TAXI24_block | TAXI26_block, 94 },{ 93, LANDING, 0, 0, 94 },
	{ 94, LANDING, RUNW04_block, 0, 95 },{ 94, FLYING, 0, 0, 111 },{ 94, 0, 0, 0, 95 },
	{ 95, ENDLANDING, 0, TAXI48_block, 96 },{ 95, 0, 0, 0, 96 },{ 95, TERM15, 0, 0, 0 },{ 95, TERM13, 0, 0, 0 },{ 95, TERM11, 0, 0, 0 },{ 95, TERM09, 0, 0, 0 },{ 95, TERM16, 0, 0, 0 },{ 95, TERM14, 0, 0, 0 },{ 95, TERM12, 0, 0, 0 },{ 95, TERM10, 0, 0, 0 },
	// End Landing for Runways 3 & 4
	{ 96,      0, 0, TAXI15_block, 97 },
	// Airplane & Helicopter Taxi  (Main Airport flow)
	{ 97,    255, 0, TAXI16_block, 0 },{ 97, HANGAR,  0, 0,   8 },{ 97, TERM09, 0, 0,  98 },{ 97, 0, 0, 0,  99 },
	{ 98, BACKUP, 0, TAXI17_block, 0 },{ 98, TERM09,  0, 0,  20 },{ 98, 0, 0, 0,  99 },
	{ 99, BACKUP, 0, TAXI18_block, 0 },{ 99, TERM10,  0, 0,  21 },{ 99, HANGAR, 0, 0,   8 },{ 99, TERM09, 0, 0, 98 },{ 99, 0, 0, 0,  100 },
	{ 100, BACKUP, 0, TAXI19_block, 0 },{ 100, TERM11,  0, 0,  22 },{ 100, HANGAR, 0, 0,  99 },{ 100, 0, 0, 0,  101 },
	{ 101, BACKUP, 0, TAXI20_block, 0 }, { 101, TERM12,  0, 0,  23 }, { 101, HELI04, 0, 0,  49 }, { 101, CARG04, 0, 0,  40 }, { 101, HELITAKEOFF, 0, 0, 49 }, { 101, 0, 0, 0, 102 },
	{ 102, BACKUP, 0, TAXI21_block, 0 },{ 102, TERM05,  0, 0, 151 },{ 102, HELI05, 0, 0,  50 },{ 102, CARG05, 0, 0,  41 },{ 102, TERM06, 0, 0, 151 },{ 102, TERM07, 0, 0, 151 },{ 102, TERM08, 0, 0, 151 },{ 102, HELI04, 0, 0, 101 },{ 102, CARG04, 0, 0, 101 },{ 102, 0, 0, 0, 103 },
	{ 103, BACKUP, 0, TAXI22_block, 0 },{ 103, TERM13,  0, 0,  24 },{ 103, HELI06, 0, 0,  51 },{ 103, CARG06, 0, 0,  42 },{ 103, HELI04, 0, 0, 102 },{ 103, HELI05, 0, 0, 102 },{ 103, CARG04, 0, 0, 102 },{ 103, CARG05, 0, 0, 102 },{ 103, HELITAKEOFF, 0, 0,  51 },{ 103, 0, 0, 0, 104 },
	{ 104, BACKUP, 0, TAXI23_block, 0 },{ 104, TERM14,  0, 0,  25 },{ 104, TERM13, 0, 0, 103 },{ 104, HELI04, 0, 0, 103 },{ 104, HELI05, 0, 0, 103 },{ 104, HELI06, 0, 0, 103 },{ 104, CARG04, 0, 0, 103 },{ 104, CARG05, 0, 0, 103 },{ 104, CARG06, 0, 0, 103 },{ 104, HELITAKEOFF, 0, 0, 103 },{ 104, 0, 0, 0, 105 },
	{ 105, BACKUP, 0, TAXI24_block, 0 },{ 105, TERM15,  0, 0,  26 },{ 105, HANGAR, 0, 0,   9 },{ 105, TERM16, 0, 0, 106 },{ 105, TERM13, 0, 0, 104 },{ 105, TERM14, 0, 0, 104 },{ 105, HELI04, 0, 0, 104 },{ 105, HELI05, 0, 0, 104 },{ 105, HELI06, 0, 0, 104 },{ 105, CARG04, 0, 0, 104 },{ 105, CARG05, 0, 0, 104 },{ 105, CARG06, 0, 0, 104 },{ 105, HELITAKEOFF, 0, 0, 104 },{ 105, TERM20, 0, 0, 106 },{ 105, TERM21, 0, 0, 106 },{ 105, TERM22, 0, 0, 106 },{ 105, CARG07, 0, 0, 106 },{ 105, CARG08, 0, 0, 106 },{ 105, CARG09, 0, 0, 106 },{ 105, 0, 0, 0, 107 },
	{ 106, BACKUP, 0, TAXI25_block, 0 },{ 106, TERM16,  0, 0,  27 },{ 106, HANGAR, 0, 0, 105 },{ 106, TERM20, 0, 0, 152 },{ 106, TERM21, 0, 0, 152 },{ 106, TERM22, 0, 0, 152 },{ 106, CARG07, 0, 0, 152 },{ 106, CARG08, 0, 0, 152 },{ 106, CARG09, 0, 0, 152 },{ 106, 0, 0, 0, 107 },
	{ 107,    255, 0, TAXI26_block, 0 },{ 107, 0, 0, 0, 108 },
	// Airplane Takeoff - Runway 04
	{ 108, TAKEOFF, RUNW04_block, TAXI48_block, 109 },
	{ 109, 0, 0, 0, 110 },
	{ 110, STARTTAKEOFF, 0, 0, 111 },
	{ 111, ENDTAKEOFF, 0, 0, 0 },{ 111, 0, 0, 0, 58 },   // End Airport control of airplane.
	// Helicopter Position for Helipads.
	{ 112, 0, HANG03_block, 0, 113 },
	{ 113, 0, HANG03_block, 0,   8 },
	// Helicopter -- takeoff
	{ 114, HELITAKEOFF, 0, 0, 0 },
	{ 115, HELITAKEOFF, 0, 0, 0 },
	{ 116, HELITAKEOFF, 0, 0, 0 },
	// End Pod #2
	// Start of Pod #3
	// Airplane Landing Runway05
	{ 117, PRELANDING, RUNW05_block, 0, 118 },{ 117, LANDING, 0, 0, 118 },
	{ 118, LANDING, RUNW05_block, 0, 119 },{ 118, FLYING, 0, 0,  165 },{ 118, 0, 0, 0, 119 },
	{ 119, ENDLANDING, 0, TAXI28_block, 124 },{ 119, 0, 0, 0, 124 },{ 119, TERM25, 0, 0, 0 },{ 119, TERM23, 0, 0, 0 },{ 119, TERM21, 0, 0, 0 },{ 119, TERM19, 0, 0, 0 },{ 119, TERM17, 0, 0, 0 },{ 119, TERM24, 0, 0, 0 },{ 119, TERM22, 0, 0, 0 },{ 119, TERM20, 0, 0, 0 },{ 119, TERM18, 0, 0, 0 },
	{ 120, 0, 0, TAXI28_block, 124 }, //Unused
	// Airplane Landing Runway06
	{ 121, PRELANDING, RUNW06_block, TAXI38_block | TAXI39_block | TAXI40_block | TAXI43_block | TAXI44_block, 122 },{ 121, LANDING, 0, 0, 122 },
	{ 122, LANDING, RUNW06_block, 0, 123 },{ 122, FLYING, 0, 0, 143 },{ 122, 0, 0, 0, 123 },
	{ 123, ENDLANDING, 0, TAXI49_block, 124 },{ 123, 0, 0, 0, 124 },{ 123, TERM25, 0, 0, 0 },{ 123, TERM23, 0, 0, 0 },{ 123, TERM21, 0, 0, 0 },{ 123, TERM19, 0, 0, 0 },{ 123, TERM17, 0, 0, 0 },{ 123, TERM24, 0, 0, 0 },{ 123, TERM22, 0, 0, 0 },{ 123, TERM20, 0, 0, 0 },{ 123, TERM18, 0, 0, 0 },
	// End Landing for Runways 5 & 6
	{ 124,      0, 0, TAXI29_block, 125 },
	// Airplane & Helicopter Taxi  (Main Airport flow)
	{ 125,    255, 0, TAXI30_block, 0 },{ 125, HANGAR, 0, 0,  10 },{ 125, TERM17, 0, 0, 126 },{ 125, TERM18, 0, 0, 128 },{ 125, 0, 0, 0, 129 },
	{ 126,    255, 0, TAXI31_block, 0 },{ 126, 0, 0, 0, 127 },
	{ 127, BACKUP, 0, TAXI32_block, 0 },{ 127, TERM17, 0, 0,  28 },{ 127, 0, 0, 0, 128 },
	{ 128, BACKUP, 0, TAXI33_block, 0 },{ 128, TERM18, 0, 0,  29 },{ 128, 0, 0, 0, 129 },
	{ 129, BACKUP, 0, TAXI34_block, 0 },{ 129, TERM19, 0, 0,  30 },{ 129, HANGAR, 0, 0,  10 },{ 129, 0, 0, 0, 130 },
	{ 130,    255, 0, TAXI35_block, 0 },{ 130, 0, 0, 0, 131 },
	{ 131, BACKUP, 0, TAXI36_block, 0 }, { 131, TERM20, 0, 0,  31 }, { 131, HELI07, 0, 0,  52 }, { 131, CARG07, 0, 0,  43 }, { 131, HELITAKEOFF, 0, 0, 52 }, { 131, 0, 0, 0, 132 },
	{ 132, BACKUP, 0, TAXI37_block, 0 },{ 132, TERM21, 0, 0,  32 },{ 132, HELI08, 0, 0,  53 },{ 132, CARG08, 0, 0,  44 },{ 132, HELI07, 0, 0, 131 },{ 132, CARG07, 0, 0, 131 },{ 132, 0, 0, 0, 133 },
	{ 133, BACKUP, 0, TAXI38_block, 0 },{ 133, TERM22, 0, 0,  33 },{ 133, HELI09, 0, 0,  54 },{ 133, CARG09, 0, 0,  45 },{ 133, HELI08, 0, 0, 132 },{ 133, HELI07, 0, 0, 132 },{ 133, CARG08, 0, 0, 132 },{ 133, CARG07, 0, 0, 132 },{ 133, HELITAKEOFF, 0, 0,  54 },{ 133, 0, 0, 0, 134 },
	{ 134,    255, 0, TAXI39_block, 0 },{ 134, HELI07, 0, 0, 133 },{ 134, HELI08, 0, 0, 133 },{ 134, HELI09, 0, 0, 133 },{ 134, CARG07, 0, 0, 133 },{ 134, CARG08, 0, 0, 133 },{ 134, CARG09, 0, 0, 133 },{ 134, HELITAKEOFF, 0, 0, 133 },{ 134, TERM22, 0, 0, 133 },{ 134, TERM01, 0, 0, 155 },{ 134, TERM02, 0, 0, 155 },{ 134, TERM03, 0, 0, 155 },{ 134, TERM04, 0, 0, 155 },{ 134, CARG01, 0, 0, 155 },{ 134, CARG02, 0, 0, 155 },{ 134, CARG03, 0, 0, 155 },{ 134, 0, 0, 0, 135 },
	{ 135, BACKUP, 0, TAXI40_block, 0 },{ 135, TERM23, 0, 0,  34 },{ 135, HANGAR, 0, 0,  11 },{ 135, TERM22, 0, 0, 134 },{ 135, TERM24, 0, 0, 136 },{ 135, TERM25, 0, 0, 136 },{ 135, HELI07, 0, 0, 134 },{ 135, HELI08, 0, 0, 134 },{ 135, HELI09, 0, 0, 134 },{ 135, CARG07, 0, 0, 134 },{ 135, CARG08, 0, 0, 134 },{ 135, CARG09, 0, 0, 134 },{ 135, CARG01, 0, 0, 134 },{ 135, CARG02, 0, 0, 134 },{ 135, CARG03, 0, 0, 134 },{ 135, HELITAKEOFF, 0, 0, 134 },{ 135, 0, 0, 0, 139 },
	{ 136, BACKUP, 0, TAXI41_block, 0 },{ 136, TERM24, 0, 0,  35 },{ 136, HANGAR, 0, 0, 139 },{ 136, TERM25, 0, 0, 137 },{ 136, 0, 0, 0, 139 },
	{ 137, BACKUP, 0, TAXI42_block, 0 },{ 137, TERM25, 0, 0,  36 },{ 137, 0, 0, 0, 138 },
	{ 138,    255, 0, TAXI43_block, 0 },{ 138, 0, 0, 0, 139 },
	{ 139,    255, 0, TAXI44_block, 0 },{ 139, HANGAR, 0, 0, 11 },{ 139, 0, 0, 0, 140 },
	// Airplane Takeoff - Runway 06
	{ 140, TAKEOFF, RUNW06_block, TAXI49_block, 141 },
	{ 141, 0, 0, 0, 142 },
	{ 142, STARTTAKEOFF, 0, 0, 143 },
	{ 143, ENDTAKEOFF, 0, 0, 0 },{ 143, 0, 0, 0, 57 },   // End Airport control of airplane.
	// Helicopter Position for Helipads.
	{ 144, 0, HANG05_block, 0, 145 },
	{ 145, 0, HANG05_block, 0,  10 },
	// Helicopter -- takeoff
	{ 146, HELITAKEOFF, 0, 0, 0 },
	{ 147, HELITAKEOFF, 0, 0, 0 },
	{ 148, HELITAKEOFF, 0, 0, 0 },
	// End Pod #3
	// Crossover between Pod1 and Pod2
	{ 149, 255, 0, TAXI27_block, 0 },{ 149, TERM13, 0, 0, 150 },{ 149, TERM14, 0, 0, 150 },{ 149, TERM15, 0, 0, 150 },{ 149, TERM16, 0, 0, 150 },{ 149, CARG05, 0, 0, 150 },{ 149, CARG06, 0, 0, 150 },{ 149, 0, 0, 0,  76 },
	{ 150, 255, 0, TAXI27_block, 0 },{ 150, TERM13, 0, 0, 151 },{ 150, TERM14, 0, 0, 151 },{ 150, TERM15, 0, 0, 151 },{ 150, TERM16, 0, 0, 151 },{ 150, TERM05, 0, 0, 149 },{ 150, TERM06, 0, 0, 149 },{ 150, TERM07, 0, 0, 149 },{ 150, TERM08, 0, 0, 149 },{ 150, CARG05, 0, 0, 151 },{ 150, CARG06, 0, 0, 151 },
	{ 151, 255, 0, TAXI27_block, 0 },{ 151, TERM05, 0, 0, 150 },{ 151, TERM06, 0, 0, 150 },{ 151, TERM07, 0, 0, 150 },{ 151, TERM08, 0, 0, 150 },{ 151, 0, 0, 0,  102 },
	// Crossover between Pod2 and Pod3
	{ 152, 255, 0, TAXI45_block, 0 },{ 152, TERM20, 0, 0, 153 },{ 152, TERM21, 0, 0, 153 },{ 152, TERM22, 0, 0, 153 },{ 152, CARG07, 0, 0, 153 },{ 152, CARG08, 0, 0, 153 },{ 152, CARG09, 0, 0, 153 },{ 152, 0, 0, 0,  102 },
	{ 153, 255, 0, TAXI45_block, 0 },{ 153, 0, 0, 0,  130 },
	// Crossover between Pod3 and Pod1
	{ 154, 255, 0, TAXI46_block, 0 },{ 154, TERM23, 0, 0, 155 },{ 154, TERM24, 0, 0, 155 },{ 154, TERM25, 0, 0, 155 },{ 154, 0, 0, 0,  72 },
	{ 155, 255, 0, TAXI46_block, 0 },{ 155, TERM01, 0, 0, 154 },{ 155, TERM02, 0, 0, 154 },{ 155, TERM03, 0, 0, 154 },{ 155, TERM04, 0, 0, 154 },{ 155, CARG01, 0, 0, 154 },{ 155, CARG02, 0, 0, 154 },{ 155, CARG03, 0, 0, 154 },{ 155, 0, 0, 0,  134 },
	// Added Holding Pattern spots.
	{ 156, FLYING, 0, 0, 157 },{ 156, PRELANDING, 0, 0,  67 }, { 156, HELILANDING, 0, 0, 59 }, { 156, 0, 0, 0, 157 },
	{ 157, FLYING, 0, 0,  55 },{ 157, PRELANDING, 0, 0,  63 }, { 157, HELILANDING, 0, 0, 59 }, { 157, 0, 0, 0,  55 },
	{ 158, FLYING, 0, 0, 159 },{ 158, PRELANDING, 0, 0,  93 }, { 158, HELILANDING, 0, 0, 59 }, { 158, 0, 0, 0, 159 },
	{ 159, FLYING, 0, 0,  57 },{ 159, PRELANDING, 0, 0,  89 }, { 159, HELILANDING, 0, 0, 59 }, { 159, 0, 0, 0,  89 },
	{ 160, FLYING, 0, 0, 161 },{ 160, PRELANDING, 0, 0, 121 }, { 160, HELILANDING, 0, 0, 59 }, { 160, 0, 0, 0, 161 },
	{ 161, FLYING, 0, 0,  56 },{ 161, PRELANDING, 0, 0, 117 }, { 161, HELILANDING, 0, 0, 59 }, { 161, 0, 0, 0,  56 },
	// Helicopter -- Entry .  Keeps it from messing up the planes.
	{ 162, FLYING, 0, 0, 162 },{ 162, HELILANDING, 0, 0, 59 },
	// Abort Landings
	{ 163, 0, 0, 0,  56 },
	{ 164, 0, 0, 0,  58 },
	{ 165, 0, 0, 0,  57 },
	// Helicopter Positioning
	{ 166, 0, 0, 0, 175 },
	{ 167, 0, 0, 0, 176 },
	{ 168, 0, 0, 0, 177 },
	{ 169, 0, 0, 0, 178 },
	{ 170, 0, 0, 0, 179 },
	{ 171, 0, 0, 0, 180 },
	{ 172, 0, 0, 0, 181 },
	{ 173, 0, 0, 0, 182 },
	{ 174, 0, 0, 0, 183 },
	//  Helicopter landing
	{ 175, 0, 0, 0,  46 },
	{ 176, 0, 0, 0,  47 },
	{ 177, 0, 0, 0,  48 },
	{ 178, 0, 0, 0,  49 },
	{ 179, 0, 0, 0,  50 },
	{ 180, 0, 0, 0,  51 },
	{ 181, 0, 0, 0,  52 },
	{ 182, 0, 0, 0,  53 },
	{ 183, 0, 0, 0,  54 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Terminus4
static const HangarTileTable _airport_depots_terminus4[] = { { { 8, 12 }, DIR_NE, 0 },{ { 12, 12 }, DIR_SW, 1 },{ { 2, 8 }, DIR_NW, 2 },{ { 2, 12 }, DIR_SE, 3 },{ { 6, 2 }, DIR_SW, 4 },{ { 2, 2 }, DIR_NE, 5 },{ { 12, 6 }, DIR_SE, 6 },{ { 12, 2 }, DIR_NW, 7 } };
static const TerminalTileTable _airport_terminals_terminus4[] = { { { 5,10 }, TERM01_block },{ { 6,10 }, TERM02_block },{ { 7,10 }, TERM03_block },{ { 8,10 }, TERM04_block },{ { 9,10 }, TERM05_block },{ { 10,10 }, TERM06_block },{ { 12,10 }, TERM07_block },{ { 13,10 }, TERM08_block },{ { 14,10 }, TERM09_block },
{ { 4, 5 }, TERM10_block },{ { 4, 6 }, TERM11_block },{ { 4, 7 }, TERM12_block },{ { 4, 8 }, TERM13_block },{ { 4, 9 }, TERM14_block },{ { 4,10 }, TERM15_block },{ { 4,12 }, TERM16_block },{ { 4,13 }, TERM17_block },{ { 4,14 }, TERM18_block },
{ { 9, 4 }, TERM19_block },{ { 8, 4 }, TERM20_block },{ { 7, 4 }, TERM21_block },{ { 6, 4 }, TERM22_block },{ { 5, 4 }, TERM23_block },{ { 4, 4 }, TERM24_block },{ { 2, 4 }, TERM25_block },{ { 1, 4 }, TERM26_block },{ { 0, 4 }, TERM27_block },
{ { 10, 9 }, TERM28_block },{ { 10, 8 }, TERM29_block },{ { 10, 7 }, TERM30_block },{ { 10, 6 }, TERM31_block },{ { 10, 5 }, TERM32_block },{ { 10, 4 }, TERM33_block },{ { 10, 2 }, TERM34_block },{ { 10, 1 }, TERM35_block },{ { 10, 0 }, TERM36_block },
{ { 11,12 }, CARG01_block },{ { 10,12 }, CARG02_block },{ { 9,12 }, CARG03_block },{ { 2, 9 }, CARG04_block },{ { 2,10 }, CARG05_block },{ { 2,11 }, CARG06_block },
{ { 5, 2 }, CARG07_block },{ { 4, 2 }, CARG08_block },{ { 3, 2 }, CARG09_block },{ { 12, 5 }, CARG10_block },{ { 12, 4 }, CARG11_block },{ { 12, 3 }, CARG12_block } };
static const RunwayTerminalTable _airport_runways_terminus4[] = { { TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | TERM07_block | TERM08_block | TERM09_block | CARG01_block | CARG02_block | CARG03_block, RUNW01_block | RUNW02_block, 0 },
{ TERM10_block | TERM11_block | TERM12_block | TERM13_block | TERM14_block | TERM15_block | TERM16_block | TERM17_block | TERM18_block | CARG04_block | CARG05_block | CARG06_block, RUNW03_block | RUNW04_block, 0 },
{ TERM19_block | TERM20_block | TERM21_block | TERM22_block | TERM23_block | TERM24_block | TERM25_block | TERM26_block | TERM27_block | CARG07_block | CARG08_block | CARG09_block, RUNW05_block | RUNW06_block, 0 },
{ TERM28_block | TERM29_block | TERM30_block | TERM31_block | TERM32_block | TERM33_block | TERM34_block | TERM35_block | TERM36_block | CARG10_block | CARG11_block | CARG12_block, 0, RUNW07_block | RUNW08_block } };
static const byte _airport_terminal_terminus4[] = { 1, 36 };
static const byte _airport_entries_terminus4[] = { 77, 76, 79, 78 };
static const byte _airport_helientry_terminus4[] = { 77, 76, 79, 78 };
static const AirportFTAbuildup _airport_fta_terminus4[] = {
	// Airplane & Helicopter Hangars
	{ 0, HANGAR, 0, 0,  8 },{ 0, 0, 0, 0,  8 },{ 0, TERM09, 0, 0,  8 },{ 0, TERM07, 0, 0,  8 },{ 0, TERM05, 0, 0,  8 },{ 0, TERM03, 0, 0,  8 },{ 0, TERM01, 0, 0,  8 },{ 0, TERM08, 0, 0,  8 },{ 0, TERM06, 0, 0 , 8 },{ 0, TERM04, 0, 0,  8 },{ 0, TERM02, 0, 0,  8 },{ 0, TERM16, 0, 0,  8 },{ 0, TERM28, 0, 0,  8 },{ 0, TERM17, 0, 0,  8 },{ 0, TERM29, 0, 0,  8 },{ 0, TERM18, 0, 0,  8 },{ 0, TERM30, 0, 0,  8 },{ 0, TERM31, 0, 0,  8 },{ 0, HELI01, 0, 0,  8 },{ 0, HELI02, 0, 0,  8 },{ 0, HELI03, 0, 0,  8 },{ 0, CARG01, 0, 0,  8 },{ 0, CARG02, 0, 0,  8 },{ 0, CARG03, 0, 0,  8 },{ 0, CARG10, 0, 0,  8 },{ 0, CARG11, 0, 0,  8 },{ 0, CARG12, 0, 0,  8 },
	{ 1, HANGAR, 0, 0,  9 },{ 1, 0, 0, 0,  9 },{ 1, TERM09, 0, 0,  9 },{ 1, TERM08, 0, 0,  9 },{ 1, TERM07, 0, 0,  9 },{ 1, TERM28, 0, 0,  9 },{ 1, TERM29, 0, 0,  9 },{ 1, TERM30, 0, 0,  9 },{ 1, TERM31, 0, 0,  9 },{ 1, HELI03, 0, 0,  9 },{ 1, HELI02, 0, 0,  9 },{ 1, HELI01, 0, 0,  9 },{ 1, CARG10, 0, 0,  9 },{ 1, CARG11, 0, 0,  9 },{ 1, CARG12, 0, 0,  9 },{ 1, CARG03, 0, 0,  9 },{ 1, CARG02, 0, 0,  9 },{ 1, CARG01, 0, 0,  9 },{ 1, HELITAKEOFF, 0, 0,  9 },
	{ 2, HANGAR, 0, 0, 10 },{ 2, 0, 0, 0, 10 },{ 2, TERM18, 0, 0, 10 },{ 2, TERM16, 0, 0, 10 },{ 2, TERM14, 0, 0, 10 },{ 2, TERM12, 0, 0, 10 },{ 2, TERM10, 0, 0, 10 },{ 2, TERM17, 0, 0, 10 },{ 2, TERM15, 0, 0 ,10 },{ 2, TERM13, 0, 0, 10 },{ 2, TERM11, 0, 0, 10 },{ 2, TERM25, 0, 0, 10 },{ 2, TERM01, 0, 0, 10 },{ 2, TERM26, 0, 0, 10 },{ 2, TERM02, 0, 0, 10 },{ 2, TERM27, 0, 0, 10 },{ 2, TERM03, 0, 0, 10 },{ 2, TERM04, 0, 0, 10 },{ 2, HELI04, 0, 0, 10 },{ 2, HELI05, 0, 0, 10 },{ 2, HELI06, 0, 0, 10 },{ 2, CARG04, 0, 0, 10 },{ 2, CARG05, 0, 0, 10 },{ 2, CARG06, 0, 0, 10 },{ 2, CARG01, 0, 0, 10 },{ 2, CARG02, 0, 0, 10 },{ 2, CARG03, 0, 0, 10 },
	{ 3, HANGAR, 0, 0, 11 },{ 3, 0, 0, 0, 11 },{ 3, TERM18, 0, 0, 11 },{ 3, TERM17, 0, 0, 11 },{ 3, TERM16, 0, 0, 11 },{ 3, TERM01, 0, 0, 11 },{ 3, TERM02, 0, 0, 11 },{ 3, TERM03, 0, 0, 11 },{ 3, TERM04, 0, 0, 11 },{ 3, HELI06, 0, 0, 11 },{ 3, HELI05, 0, 0, 11 },{ 3, HELI04, 0, 0, 11 },{ 3, CARG01, 0, 0, 11 },{ 3, CARG02, 0, 0, 11 },{ 3, CARG03, 0, 0, 11 },{ 3, CARG06, 0, 0, 11 },{ 3, CARG05, 0, 0, 11 },{ 3, CARG04, 0, 0, 11 },{ 3, HELITAKEOFF, 0, 0, 11 },
	{ 4, HANGAR, 0, 0, 12 },{ 4, 0, 0, 0, 12 },{ 4, TERM27, 0, 0, 12 },{ 4, TERM25, 0, 0, 12 },{ 4, TERM23, 0, 0, 12 },{ 4, TERM21, 0, 0, 12 },{ 4, TERM19, 0, 0, 12 },{ 4, TERM26, 0, 0, 12 },{ 4, TERM24, 0, 0, 12 },{ 4, TERM22, 0, 0, 12 },{ 4, TERM20, 0, 0, 12 },{ 4, TERM34, 0, 0, 12 },{ 4, TERM10, 0, 0, 12 },{ 4, TERM35, 0, 0, 12 },{ 4, TERM11, 0, 0, 12 },{ 4, TERM36, 0, 0, 12 },{ 4, TERM12, 0, 0, 12 },{ 4, TERM13, 0, 0, 12 },{ 4, HELI07, 0, 0, 12 },{ 4, HELI08, 0, 0, 12 },{ 4, HELI09, 0, 0, 12 },{ 4, CARG07, 0, 0, 12 },{ 4, CARG08, 0, 0, 12 },{ 4, CARG09, 0, 0, 12 },{ 4, CARG04, 0, 0, 12 },{ 4, CARG05, 0, 0, 12 },{ 4, CARG06, 0, 0, 12 },
	{ 5, HANGAR, 0, 0, 13 },{ 5, 0, 0, 0, 13 },{ 5, TERM27, 0, 0, 13 },{ 5, TERM26, 0, 0, 13 },{ 5, TERM25, 0, 0, 13 },{ 5, TERM10, 0, 0, 13 },{ 5, TERM11, 0, 0, 13 },{ 5, TERM12, 0, 0, 13 },{ 5, TERM13, 0, 0, 13 },{ 5, HELI09, 0, 0, 13 },{ 5, HELI10, 0, 0, 13 },{ 5, HELI11, 0, 0, 13 },{ 5, CARG04, 0, 0, 13 },{ 5, CARG05, 0, 0, 13 },{ 5, CARG06, 0, 0, 13 },{ 5, CARG09, 0, 0, 13 },{ 5, CARG08, 0, 0, 13 },{ 5, CARG07, 0, 0, 13 },{ 5, HELITAKEOFF, 0, 0, 13 },
	{ 6, HANGAR, 0, 0, 14 },{ 6, 0, 0, 0, 14 },{ 6, TERM36, 0, 0, 14 },{ 6, TERM34, 0, 0, 14 },{ 6, TERM32, 0, 0, 14 },{ 6, TERM30, 0, 0, 14 },{ 6, TERM28, 0, 0, 14 },{ 6, TERM35, 0, 0, 14 },{ 6, TERM33, 0, 0, 14 },{ 6, TERM31, 0, 0, 14 },{ 6, TERM29, 0, 0, 14 },{ 6, TERM07, 0, 0, 14 },{ 6, TERM19, 0, 0, 14 },{ 6, TERM08, 0, 0, 14 },{ 6, TERM20, 0, 0, 14 },{ 6, TERM09, 0, 0, 14 },{ 6, TERM21, 0, 0, 14 },{ 6, TERM22, 0, 0, 14 },{ 6, HELI10, 0, 0, 14 },{ 6, HELI11, 0, 0, 14 },{ 6, HELI12, 0, 0, 14 },{ 6, CARG10, 0, 0, 14 },{ 6, CARG11, 0, 0, 14 },{ 6, CARG12, 0, 0, 14 },{ 6, CARG07, 0, 0, 14 },{ 6, CARG08, 0, 0, 14 },{ 6, CARG09, 0, 0, 14 },
	{ 7, HANGAR, 0, 0, 15 },{ 7, 0, 0, 0, 15 },{ 7, TERM36, 0, 0, 15 },{ 7, TERM35, 0, 0, 15 },{ 7, TERM34, 0, 0, 15 },{ 7, TERM19, 0, 0, 15 },{ 7, TERM20, 0, 0, 15 },{ 7, TERM21, 0, 0, 15 },{ 7, TERM22, 0, 0, 15 },{ 7, HELI12, 0, 0, 15 },{ 7, HELI11, 0, 0, 15 },{ 7, HELI10, 0, 0, 15 },{ 7, CARG07, 0, 0, 15 },{ 7, CARG08, 0, 0, 15 },{ 7, CARG09, 0, 0, 15 },{ 7, CARG12, 0, 0, 15 },{ 7, CARG11, 0, 0, 15 },{ 7, CARG10, 0, 0, 15 },{ 7, HELITAKEOFF, 0, 0, 15 },
	{ 8, 255, HANG01_block, 0, 0 },{ 8, HANGAR, 0, 0, 0 },{ 8, TERM01, 0, TAXI04_block | TAXI05_block | TAXI53_block,  94 },{ 8, TERM02, 0, TAXI04_block | TAXI05_block,  94 },{ 8, TERM16, 0, TAXI04_block | TAXI05_block | TAXI53_block,  94 },{ 8, TERM17, 0, TAXI04_block | TAXI05_block | TAXI53_block,  94 },{ 8, TERM18, 0, TAXI04_block | TAXI05_block | TAXI53_block,  94 },{ 8, TERM28, 0, TAXI56_block,  94 },{ 8, TERM29, 0, TAXI56_block,  94 },{ 8, TERM30, 0, TAXI56_block,  94 },{ 8, TERM31, 0, TAXI56_block,  94 },{ 8, CARG10, 0, TAXI56_block,  94 },{ 8, CARG11, 0, TAXI56_block,  94 },{ 8, CARG12, 0, TAXI56_block,  94 },{ 8, HELITAKEOFF, HELI01_block, 0, 94 }, { 8, 0, 0, 0,  94 },
	{ 9, 255, HANG02_block, 0, 0 },{ 9, HANGAR, 0, 0, 1 },{ 9, TERM07, 0, TAXI11_block | TAXI10_block, 100 },{ 9, TERM08, 0, TAXI11_block, 100 },{ 9, TERM09, 0, TAXI11_block, 100 },{ 9, HELI01, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block | TAXI07_block, 100 },{ 9, HELI02, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block, 100 },{ 9, HELI03, 0, TAXI11_block | TAXI10_block | TAXI09_block, 100 },{ 9, HELITAKEOFF, HELI03_block, TAXI11_block | TAXI10_block | TAXI09_block, 100 },{ 9, TERM28, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI56_block, 100 },{ 9, TERM29, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI56_block, 100 },{ 9, TERM30, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI56_block, 100 },{ 9, TERM31, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI56_block, 100 },{ 9, CARG01, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block | TAXI07_block, 100 },{ 9, CARG02, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI08_block, 100 },{ 9, CARG03, 0, TAXI11_block | TAXI10_block | TAXI09_block, 100 },{ 9, CARG10, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI56_block, 100 },{ 9, CARG11, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI56_block, 100 },{ 9, CARG12, 0, TAXI11_block | TAXI10_block | TAXI09_block | TAXI56_block, 100 },{ 9, TAKEOFF, 0, TAXI13_block, 102 },{ 9, 0, 0, TAXI13_block, 102 },
	{ 10, 255, HANG03_block, 0, 0 },{ 10, HANGAR, 0, 0, 2 },{ 10, TERM10, 0, TAXI18_block | TAXI17_block | TAXI54_block, 120 },{ 10, TERM11, 0, TAXI18_block | TAXI17_block, 120 },{ 10, TERM25, 0, TAXI18_block | TAXI17_block | TAXI54_block, 120 },{ 10, TERM26, 0, TAXI18_block | TAXI17_block | TAXI54_block, 120 },{ 10, TERM27, 0, TAXI18_block | TAXI17_block | TAXI54_block, 120 },{ 10, TERM01, 0, TAXI53_block, 120 },{ 10, TERM02, 0, TAXI53_block, 120 },{ 10, TERM03, 0, TAXI53_block, 120 },{ 10, TERM04, 0, TAXI53_block, 120 },{ 10, CARG01, 0, TAXI53_block, 120 },{ 10, CARG02, 0, TAXI53_block, 120 },{ 10, CARG03, 0, TAXI53_block, 120 },{ 10, HELITAKEOFF, HELI04_block, 0, 120 }, { 10, 0, 0, 0, 120 },
	{ 11, 255, HANG04_block, 0, 0 },{ 11, HANGAR, 0, 0, 3 },{ 11, TERM16, 0, TAXI24_block | TAXI23_block, 126 },{ 11, TERM17, 0, TAXI24_block, 126 },{ 11, TERM18, 0, TAXI24_block, 126 },{ 11, HELI04, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI21_block | TAXI20_block, 126 },{ 11, HELI05, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI21_block, 126 },{ 11, HELI06, 0, TAXI24_block | TAXI23_block | TAXI22_block, 126 },{ 11, HELITAKEOFF, HELI06_block, TAXI24_block | TAXI23_block | TAXI22_block, 126 },{ 11, TERM01, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI53_block, 126 },{ 11, TERM02, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI53_block, 126 },{ 11, TERM03, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI53_block, 126 },{ 11, TERM04, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI53_block, 126 },{ 11, CARG04, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI21_block | TAXI20_block, 126 },{ 11, CARG05, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI21_block, 126 },{ 11, CARG06, 0, TAXI24_block | TAXI23_block | TAXI22_block, 126 },{ 11, CARG01, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI53_block, 126 },{ 11, CARG02, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI53_block, 126 },{ 11, CARG03, 0, TAXI24_block | TAXI23_block | TAXI22_block | TAXI53_block, 126 },{ 11, TAKEOFF, 0, TAXI26_block, 128 },{ 11, 0, 0, TAXI26_block, 128 },
	{ 12, 255, HANG05_block, 0, 0 },{ 12, HANGAR, 0, 0, 4 },{ 12, TERM19, 0, TAXI31_block | TAXI30_block | TAXI55_block, 148 },{ 12, TERM20, 0, TAXI31_block | TAXI30_block, 148 },{ 12, TERM34, 0, TAXI31_block | TAXI30_block | TAXI55_block, 148 },{ 12, TERM35, 0, TAXI31_block | TAXI30_block | TAXI55_block, 148 },{ 12, TERM36, 0, TAXI31_block | TAXI30_block | TAXI55_block, 148 },{ 12, TERM10, 0, TAXI54_block, 148 },{ 12, TERM11, 0, TAXI54_block, 148 },{ 12, TERM12, 0, TAXI54_block, 148 },{ 12, TERM13, 0, TAXI54_block, 148 },{ 12, CARG04, 0, TAXI54_block, 148 },{ 12, CARG05, 0, TAXI54_block, 148 },{ 12, CARG06, 0, TAXI54_block, 148 },{ 12, HELITAKEOFF, HELI07_block, 0, 148 }, { 12, 0, 0, 0, 148 },
	{ 13, 255, HANG06_block, 0, 0 },{ 13, HANGAR, 0, 0, 5 },{ 13, TERM25, 0, TAXI37_block | TAXI36_block, 154 },{ 13, TERM26, 0, TAXI37_block, 154 },{ 13, TERM27, 0, TAXI37_block, 154 },{ 13, HELI07, 0, TAXI37_block | TAXI36_block | TAXI35_block | TAXI34_block | TAXI33_block, 154 },{ 13, HELI08, 0, TAXI37_block | TAXI36_block | TAXI35_block | TAXI34_block, 154 },{ 13, HELI09, 0, TAXI37_block | TAXI36_block | TAXI35_block, 154 },{ 13, HELITAKEOFF, HELI09_block, TAXI37_block | TAXI36_block | TAXI35_block, 154 },{ 13, TERM10, 0, TAXI37_block | TAXI36_block | TAXI35_block | TAXI54_block, 154 },{ 13, TERM11, 0, TAXI37_block | TAXI36_block | TAXI35_block | TAXI54_block, 154 },{ 13, TERM12, 0, TAXI37_block | TAXI36_block | TAXI35_block | TAXI54_block, 154 },{ 13, TERM13, 0, TAXI37_block | TAXI36_block | TAXI35_block | TAXI54_block, 154 },{ 13, CARG07, 0, TAXI37_block | TAXI36_block | TAXI35_block | TAXI34_block | TAXI33_block, 154 },{ 13, CARG08, 0, TAXI37_block | TAXI36_block | TAXI35_block | TAXI34_block, 154 },{ 13, CARG09, 0, TAXI37_block | TAXI36_block | TAXI35_block, 154 },{ 13, CARG04, 0, TAXI37_block | TAXI36_block | TAXI35_block | TAXI54_block, 154 },{ 13, CARG05, 0, TAXI37_block | TAXI36_block | TAXI35_block | TAXI54_block, 154 },{ 13, CARG06, 0, TAXI37_block | TAXI36_block | TAXI35_block | TAXI54_block, 154 },{ 13, TAKEOFF, 0, TAXI39_block, 156 },{ 13, 0, 0, TAXI39_block, 156 },
	{ 14, 255, HANG07_block, 0, 0 },{ 14, HANGAR, 0, 0, 6 },{ 14, TERM28, 0, TAXI44_block | TAXI43_block | TAXI56_block, 176 },{ 14, TERM29, 0, TAXI44_block | TAXI43_block, 176 },{ 14, TERM07, 0, TAXI44_block | TAXI43_block | TAXI56_block, 176 },{ 14, TERM08, 0, TAXI44_block | TAXI43_block | TAXI56_block, 176 },{ 14, TERM09, 0, TAXI44_block | TAXI43_block | TAXI56_block, 176 },{ 14, TERM19, 0, TAXI55_block, 176 },{ 14, TERM20, 0, TAXI55_block, 176 },{ 14, TERM21, 0, TAXI55_block, 176 },{ 14, TERM22, 0, TAXI55_block, 176 },{ 14, CARG07, 0, TAXI55_block, 176 },{ 14, CARG08, 0, TAXI55_block, 176 },{ 14, CARG09, 0, TAXI55_block, 176 },{ 14, HELITAKEOFF, HELI10_block, 0, 176 }, { 14, 0, 0, 0, 176 },
	{ 15, 255, HANG08_block, 0, 0 },{ 15, HANGAR, 0, 0, 7 },{ 15, TERM34, 0, TAXI50_block | TAXI49_block, 182 },{ 15, TERM35, 0, TAXI50_block, 182 },{ 15, TERM36, 0, TAXI50_block, 182 },{ 15, HELI10, 0, TAXI50_block | TAXI49_block | TAXI48_block | TAXI47_block | TAXI46_block, 182 },{ 15, HELI11, 0, TAXI50_block | TAXI49_block | TAXI48_block | TAXI47_block, 182 },{ 15, HELI12, 0, TAXI50_block | TAXI49_block | TAXI48_block, 182 },{ 15, HELITAKEOFF, HELI12_block, TAXI50_block | TAXI49_block | TAXI48_block, 182 },{ 15, TERM19, 0, TAXI50_block | TAXI49_block | TAXI48_block | TAXI55_block, 182 },{ 15, TERM20, 0, TAXI50_block | TAXI49_block | TAXI48_block | TAXI55_block, 182 },{ 15, TERM21, 0, TAXI50_block | TAXI49_block | TAXI48_block | TAXI55_block, 182 },{ 15, TERM22, 0, TAXI50_block | TAXI49_block | TAXI48_block | TAXI55_block, 182 },{ 15, CARG10, 0, TAXI50_block | TAXI49_block | TAXI48_block | TAXI47_block | TAXI46_block, 182 },{ 15, CARG11, 0, TAXI50_block | TAXI49_block | TAXI48_block | TAXI47_block, 182 },{ 15, CARG12, 0, TAXI50_block | TAXI49_block | TAXI48_block, 182 },{ 15, CARG07, 0, TAXI50_block | TAXI49_block | TAXI48_block | TAXI55_block, 182 },{ 15, CARG08, 0, TAXI50_block | TAXI49_block | TAXI48_block | TAXI55_block, 182 },{ 15, CARG09, 0, TAXI50_block | TAXI49_block | TAXI48_block | TAXI55_block, 182 },{ 15, TAKEOFF, 0, TAXI52_block, 184 },{ 15, 0, 0, TAXI52_block, 184 },
	// Airplane Terminals
	{ 16, TERM01, TERM01_block, 0, 194 },{ 16, HANGAR, 0, 0, 194 },{ 16, 0, 0, 0, 194 },
	{ 17, TERM02, TERM02_block, 0,  93 },{ 17, HANGAR, HANG01_block, TAXI05_block | TAXI04_block,  93 },{ 17, 0, 0, 0,  93 },
	{ 18, TERM03, TERM03_block, 0,  94 },{ 18, HANGAR, HANG01_block, TAXI05_block,  94 },{ 18, 0, 0, 0,  94 },
	{ 19, TERM04, TERM04_block, 0,  95 },{ 19, HANGAR, HANG01_block, TAXI06_block | TAXI05_block,  95 },{ 19, 0, 0, 0,  95 },
	{ 20, TERM05, TERM05_block, 0,  96 },{ 20, HANGAR, 0, 0,  96 },{ 20, 0, 0, 0,  96 },
	{ 21, TERM06, TERM06_block, 0,  98 },{ 21, HANGAR, 0, 0,  98 },{ 21, 0, 0, 0,  98 },
	{ 22, TERM07, TERM07_block, 0,  99 },{ 22, HANGAR, 0, 0,  99 },{ 22, 0, 0, 0,  99 },
	{ 23, TERM08, TERM08_block, 0, 100 },{ 23, HANGAR, 0, 0, 100 },{ 23, 0, 0, 0, 100 },
	{ 24, TERM09, TERM09_block, 0, 101 },{ 24, HANGAR, HANG02_block, TAXI11_block | TAXI12_block, 101 },{ 24, 0, 0, 0, 101 },
	{ 25, TERM10, TERM10_block, 0, 196 },{ 25, HANGAR, 0, 0, 196 },{ 25, 0, 0, 0, 196 },
	{ 26, TERM11, TERM11_block, 0, 119 },{ 26, HANGAR, HANG03_block, TAXI18_block | TAXI17_block, 119 },{ 26, 0, 0, 0, 119 },
	{ 27, TERM12, TERM12_block, 0, 120 },{ 27, HANGAR, HANG03_block, TAXI18_block, 120 },{ 27, 0, 0, 0, 120 },
	{ 28, TERM13, TERM13_block, 0, 121 },{ 28, HANGAR, HANG03_block, TAXI19_block | TAXI18_block, 121 },{ 28, 0, 0, 0, 121 },
	{ 29, TERM14, TERM14_block, 0, 122 },{ 29, HANGAR, 0, 0, 122 },{ 29, 0, 0, 0, 122 },
	{ 30, TERM15, TERM15_block, 0, 124 },{ 30, HANGAR, 0, 0, 124 },{ 30, 0, 0, 0, 124 },
	{ 31, TERM16, TERM16_block, 0, 125 },{ 31, HANGAR, 0, 0, 125 },{ 31, 0, 0, 0, 125 },
	{ 32, TERM17, TERM17_block, 0, 126 },{ 32, HANGAR, 0, 0, 126 },{ 32, 0, 0, 0, 126 },
	{ 33, TERM18, TERM18_block, 0, 127 },{ 33, HANGAR, HANG04_block, TAXI24_block | TAXI25_block, 127 },{ 33, 0, 0, 0, 127 },
	{ 34, TERM19, TERM19_block, 0, 198 },{ 34, HANGAR, 0, 0, 198 },{ 34, 0, 0, 0, 198 },
	{ 35, TERM20, TERM20_block, 0, 147 },{ 35, HANGAR, HANG05_block, TAXI31_block | TAXI30_block, 147 },{ 35, 0, 0, 0, 147 },
	{ 36, TERM21, TERM21_block, 0, 148 },{ 36, HANGAR, HANG05_block, TAXI31_block, 148 },{ 36, 0, 0, 0, 148 },
	{ 37, TERM22, TERM22_block, 0, 149 },{ 37, HANGAR, HANG05_block, TAXI32_block | TAXI31_block, 149 },{ 37, 0, 0, 0, 149 },
	{ 38, TERM23, TERM23_block, 0, 150 },{ 38, HANGAR, 0, 0, 150 },{ 38, 0, 0, 0, 150 },
	{ 39, TERM24, TERM24_block, 0, 152 },{ 39, HANGAR, 0, 0, 152 },{ 39, 0, 0, 0, 152 },
	{ 40, TERM25, TERM25_block, 0, 153 },{ 40, HANGAR, 0, 0, 153 },{ 40, 0, 0, 0, 153 },
	{ 41, TERM26, TERM26_block, 0, 154 },{ 41, HANGAR, 0, 0, 154 },{ 41, 0, 0, 0, 154 },
	{ 42, TERM27, TERM27_block, 0, 155 },{ 42, HANGAR, HANG06_block, TAXI38_block | TAXI37_block, 155 },{ 42, 0, 0, 0, 155 },
	{ 43, TERM28, TERM28_block, 0, 200 },{ 43, HANGAR, 0, 0, 200 },{ 43, 0, 0, 0, 200 },
	{ 44, TERM29, TERM29_block, 0, 175 },{ 44, HANGAR, HANG07_block, TAXI44_block | TAXI43_block, 175 },{ 44, 0, 0, 0, 175 },
	{ 45, TERM30, TERM30_block, 0, 176 },{ 45, HANGAR, HANG07_block, TAXI44_block, 176 },{ 45, 0, 0, 0, 176 },
	{ 46, TERM31, TERM31_block, 0, 177 },{ 46, HANGAR, HANG07_block, TAXI45_block | TAXI44_block, 177 },{ 46, 0, 0, 0, 177 },
	{ 47, TERM32, TERM32_block, 0, 178 },{ 47, HANGAR, 0, 0, 178 },{ 47, 0, 0, 0, 178 },
	{ 48, TERM33, TERM33_block, 0, 180 },{ 48, HANGAR, 0, 0, 180 },{ 48, 0, 0, 0, 180 },
	{ 49, TERM34, TERM34_block, 0, 181 },{ 49, HANGAR, 0, 0, 181 },{ 49, 0, 0, 0, 181 },
	{ 50, TERM35, TERM35_block, 0, 182 },{ 50, HANGAR, 0, 0, 182 },{ 50, 0, 0, 0, 182 },
	{ 51, TERM36, TERM36_block, 0, 183 },{ 51, HANGAR, HANG08_block, TAXI50_block | TAXI51_block, 183 },{ 51, 0, 0, 0, 183 },
	// Cargo Terminals (Uses Helipads)
	{ 52, CARG01, CARG01_block, 0,  96 },{ 52, HANGAR, 0, 0,  96 },{ 52, 0, 0, 0,  96 },
	{ 53, CARG02, CARG02_block, 0,  97 },{ 53, HANGAR, 0, 0,  97 },{ 53, 0, 0, 0,  97 },
	{ 54, CARG03, CARG03_block, 0,  98 },{ 54, HANGAR, 0, 0,  98 },{ 54, 0, 0, 0,  98 },
	{ 55, CARG04, CARG04_block, 0, 122 },{ 55, HANGAR, 0, 0, 122 },{ 55, 0, 0, 0, 122 },
	{ 56, CARG05, CARG05_block, 0, 123 },{ 56, HANGAR, 0, 0, 123 },{ 56, 0, 0, 0, 123 },
	{ 57, CARG06, CARG06_block, 0, 124 },{ 57, HANGAR, 0, 0, 124 },{ 57, 0, 0, 0, 124 },
	{ 58, CARG07, CARG07_block, 0, 150 },{ 58, HANGAR, 0, 0, 150 },{ 58, 0, 0, 0, 150 },
	{ 59, CARG08, CARG08_block, 0, 151 },{ 59, HANGAR, 0, 0, 151 },{ 59, 0, 0, 0, 151 },
	{ 60, CARG09, CARG09_block, 0, 152 },{ 60, HANGAR, 0, 0, 152 },{ 60, 0, 0, 0, 152 },
	{ 61, CARG10, CARG10_block, 0, 178 },{ 61, HANGAR, 0, 0, 178 },{ 61, 0, 0, 0, 178 },
	{ 62, CARG11, CARG11_block, 0, 179 },{ 62, HANGAR, 0, 0, 179 },{ 62, 0, 0, 0, 179 },
	{ 63, CARG12, CARG12_block, 0, 180 },{ 63, HANGAR, 0, 0, 180 },{ 63, 0, 0, 0, 180 },
	// Helicopter Terminals
	{ 64, HELI01, HELI01_block, 0,  96 },{ 64, HANGAR, 0, 0,  96 },{ 64, HELITAKEOFF, 0, 0, 107 },
	{ 65, HELI02, HELI02_block, 0,  97 },{ 65, HANGAR, 0, 0,  97 },{ 65, HELITAKEOFF, 0, 0, 108 },
	{ 66, HELI03, HELI03_block, 0,  98 },{ 66, HANGAR, 0, 0,  98 },{ 66, HELITAKEOFF, 0, 0, 109 },
	{ 67, HELI04, HELI04_block, 0, 122 },{ 67, HANGAR, 0, 0, 122 },{ 67, HELITAKEOFF, 0, 0, 135 },
	{ 68, HELI05, HELI05_block, 0, 123 },{ 68, HANGAR, 0, 0, 123 },{ 68, HELITAKEOFF, 0, 0, 136 },
	{ 69, HELI06, HELI06_block, 0, 124 },{ 69, HANGAR, 0, 0, 124 },{ 69, HELITAKEOFF, 0, 0, 137 },
	{ 70, HELI07, HELI07_block, 0, 150 },{ 70, HANGAR, 0, 0, 150 },{ 70, HELITAKEOFF, 0, 0, 163 },
	{ 71, HELI08, HELI08_block, 0, 151 },{ 71, HANGAR, 0, 0, 151 },{ 71, HELITAKEOFF, 0, 0, 164 },
	{ 72, HELI09, HELI09_block, 0, 152 },{ 72, HANGAR, 0, 0, 152 },{ 72, HELITAKEOFF, 0, 0, 165 },
	{ 73, HELI10, HELI10_block, 0, 178 },{ 73, HANGAR, 0, 0, 178 },{ 73, HELITAKEOFF, 0, 0, 191 },
	{ 74, HELI11, HELI11_block, 0, 179 },{ 74, HANGAR, 0, 0, 179 },{ 74, HELITAKEOFF, 0, 0, 192 },
	{ 75, HELI12, HELI12_block, 0, 180 },{ 75, HANGAR, 0, 0, 180 },{ 75, HELITAKEOFF, 0, 0, 193 },
	// Airplane In Air - Holding Pattern
	{ 76, 0, 0, 0, 204 },  // Pod 2
	{ 77, 0, 0, 0, 206 },  // Pod 3
	{ 78, 0, 0, 0, 208 },  // Pod 4
	{ 79, 0, 0, 0, 202 },  // Pod 1
	// Helicopter -- Holding Pattern
	{ 80, HELILANDING, 0, 0, 81 },
	{ 81, HELIENDLANDING, 0, 0, 81 },{ 81, HELI12, 0, 0, 226 },{ 81, HELI09, 0, 0, 223 },{ 81, HELI06, 0, 0, 220 },{ 81, HELI03, 0, 0, 217 },{ 81, HELI11, 0, 0, 225 },{ 81, HELI08, 0, 0, 222 },{ 81, HELI05, 0, 0, 219 },{ 81, HELI02, 0, 0, 216 },{ 81, HELI10, 0, 0, 224 },{ 81, HELI07, 0, 0, 221 },{ 81, HELI04, 0, 0, 218 },{ 81, HELI01, 0, 0, 215 },{ 81, HANGAR, 0, 0, 82 },{ 81, 0, 0, 0, 82 },
	//Helicopter -- Hangar 01
	{ 82, 0, HANG01_block, 0, 83 }, // need to go to hangar when waiting in air
	{ 83, 0, HANG01_block, 0,  8 },
	// Start of Pod #1
	// Airplane Landing Runway01
	{ 84, PRELANDING, RUNW01_block, 0, 85 },{ 84, LANDING, 0, 0, 85 },
	{ 85, LANDING, RUNW01_block, 0, 86 },{ 85, FLYING, 0, 0, 211 },{ 85, 0, 0, 0, 86 },
	{ 86, ENDLANDING, 0, TAXI01_block, 91 },{ 86, 0, 0, 0, 91 },{ 86, TERM09, 0, 0, 0 },{ 86, TERM07, 0, 0, 0 },{ 86, TERM05, 0, 0, 0 },{ 86, TERM03, 0, 0, 0 },{ 86, TERM01, 0, 0, 0 },{ 86, TERM08, 0, 0, 0 },{ 86, TERM06, 0, 0, 0 },{ 86, TERM04, 0, 0, 0 },{ 86, TERM02, 0, 0, 0 },{ 86, CARG01, 0, 0, 0 },{ 86, CARG02, 0, 0, 0 },{ 86, CARG03, 0, 0, 0 },
	{ 87,       0, 0, TAXI01_block, 91 }, // Unused
									  // Airplane Landing Runway02
	{ 88, PRELANDING, RUNW02_block, TAXI09_block | TAXI10_block | TAXI11_block | TAXI13_block, 89 },{ 88, LANDING, 0, 0, 89 },
	{ 89, LANDING, RUNW02_block, 0, 90 },{ 89, FLYING, 0, 0, 106 },{ 89, 0, 0, 0, 90 },
	{ 90, ENDLANDING, 0, TAXI57_block, 91 },{ 90, 0, 0, 0, 91 },{ 90, TERM09, 0, 0, 0 },{ 90, TERM07, 0, 0, 0 },{ 90, TERM05, 0, 0, 0 },{ 90, TERM03, 0, 0, 0 },{ 90, TERM01, 0, 0, 0 },{ 90, TERM08, 0, 0, 0 },{ 90, TERM06, 0, 0, 0 },{ 90, TERM04, 0, 0, 0 },{ 90, TERM02, 0, 0, 0 },{ 90, CARG01, 0, 0, 0 },{ 90, CARG02, 0, 0, 0 },{ 90, CARG03, 0, 0, 0 },
	// End Landing for Runways 1 & 2
	{ 91,      0, 0, TAXI02_block, 92 },
	// Airplane & Helicopter Taxi  (Main Airport flow Pod 1)
	{ 92,    255, 0, TAXI03_block, 0 },{ 92, HANGAR, 0, 0,   8 },{ 92, TERM01, 0, TAXI04_block | TAXI53_block,  93 },{ 92, TERM02, 0, 0,  93 },{ 92, 0, 0, 0, 94 },
	{ 93, BACKUP, 0, TAXI04_block, 0 },{ 93, TERM02, 0, 0,  17 },{ 93, TERM01, 0, 0, 194 },{ 93, TERM16, 0, 0, 194 },{ 93, TERM17, 0, 0, 194 },{ 93, TERM18, 0, 0, 194 },{ 93, 0, 0, 0, 94 },
	{ 94, BACKUP, 0, TAXI05_block, 0 },{ 94, TERM03, 0, 0,  18 },{ 94, TERM16, 0, 0,  93 },{ 94, TERM17, 0, 0,  93 },{ 94, TERM18, 0, 0,  93 },{ 94, TERM01, 0, 0, 93 },{ 94, TERM02, 0, 0, 93 },{ 94, HANGAR, 0, 0,   8 },{ 94, 0, 0, 0, 95 },
	{ 95, BACKUP, 0, TAXI06_block, 0 },{ 95, TERM04, 0, 0,  19 },{ 95, HANGAR, 0, 0,  94 },{ 95, 0, 0, 0, 96 },
	{ 96, BACKUP, 0, TAXI07_block, 0 }, { 96, TERM05, 0, 0,  20 }, { 96, HELI01, 0, 0,  64 }, { 96, CARG01, 0, 0,  52 }, { 96, HELITAKEOFF, 0, 0, 64 }, { 96, 0, 0, 0, 97 },
	{ 97, BACKUP, 0, TAXI08_block, 0 },{ 97, TERM06, 0, 0,  21 },{ 97, HELI02, 0, 0,  65 },{ 97, CARG02, 0, 0,  53 },{ 97, HELI01, 0, 0,  96 },{ 97, CARG01, 0, 0,  96 },{ 97, 0, 0, 0, 98 },
	{ 98, BACKUP, 0, TAXI09_block, 0 },{ 98, TERM28, 0, 0, 201 },{ 98, HELI03, 0, 0,  66 },{ 98, CARG03, 0, 0,  54 },{ 98, HELI01, 0, 0,  97 },{ 98, HELI02, 0, 0,  97 },{ 98, CARG01, 0, 0,  97 },{ 98, CARG02, 0, 0,  97 },{ 98, TERM29, 0, 0, 201 },{ 98, TERM30, 0, 0, 201 },{ 98, TERM31, 0, 0, 201 },{ 98, HELITAKEOFF, 0, 0, 66 },{ 98, CARG10, 0, 0, 201 },{ 98, CARG11, 0, 0, 201 },{ 98, CARG12, 0, 0, 201 },{ 98, 0, 0, 0, 99 },
	{ 99, BACKUP, 0, TAXI10_block, 0 },{ 99, TERM07, 0, 0,  22 },{ 99, TERM28, 0, 0,  98 },{ 99, TERM29, 0, 0,  98 },{ 99, TERM30, 0, 0,  98 },{ 99, TERM31, 0, 0,  98 },{ 99, HELI01, 0, 0, 98 },{ 99, HELI02, 0, 0, 98 },{ 99, HELI03, 0, 0, 98 },{ 99, HELITAKEOFF, 0, 0, 98 },{ 99, CARG01, 0, 0, 98 },{ 99, CARG02, 0, 0, 98 },{ 99, CARG03, 0, 0, 98 },{ 99, CARG10, 0, 0, 98 },{ 99, CARG11, 0, 0, 98 },{ 99, CARG12, 0, 0, 98 },{ 99, 0, 0, 0, 100 },
	{ 100, BACKUP, 0, TAXI11_block, 0 },{ 100, TERM08, 0, 0,  23 },{ 100, HANGAR, 0, 0,   9 },{ 100, TERM09, 0, 0,  101 },{ 100, TERM28, 0, 0,  99 },{ 100, TERM29, 0, 0,  99 },{ 100, TERM30, 0, 0, 99 },{ 100, TERM31, 0, 0, 99 },{ 100, TERM07, 0, 0, 99 },{ 100, HELI01, 0, 0, 99 },{ 100, HELI02, 0, 0, 99 },{ 100, HELI03, 0, 0, 99 },{ 100, HELITAKEOFF, 0, 0, 99 },{ 100, CARG01, 0, 0, 99 },{ 100, CARG02, 0, 0, 99 },{ 100, CARG03, 0, 0, 99 },{ 100, CARG10, 0, 0, 99 },{ 100, CARG11, 0, 0, 99 },{ 100, CARG12, 0, 0, 99 },{ 100, 0, 0, 0, 102 },
	{ 101, BACKUP, 0, TAXI12_block, 0 },{ 101, TERM09, 0, 0,  24 },{ 101, HANGAR, 0, 0, 100 },{ 101, 0, 0, 0, 102 },
	{ 102,    255, 0, TAXI13_block, 0 },{ 102, 0, 0, 0, 103 },
	// Airplane Takeoff - Runway02
	{ 103, TAKEOFF, RUNW02_block, TAXI57_block, 104 },
	{ 104, 0, 0, 0, 105 },
	{ 105, STARTTAKEOFF, 0, 0, 106 },
	{ 106, ENDTAKEOFF, 0, 0, 0 },{ 106, 0, 0, 0, 77 },   // End Airport control of airplane.
	// Helicopter -- takeoff
	{ 107, HELITAKEOFF, 0, 0, 0 },
	{ 108, HELITAKEOFF, 0, 0, 0 },
	{ 109, HELITAKEOFF, 0, 0, 0 },
	// End Pod #1
	// Start of Pod #2
	// Airplane Landing Runway03
	{ 110, PRELANDING, RUNW03_block, 0, 111 },{ 110, LANDING, 0, 0, 111 },
	{ 111, LANDING, RUNW03_block, 0, 112 },{ 111, FLYING, 0, 0, 212 },{ 111, 0, 0, 0, 112 },
	{ 112, ENDLANDING, 0, TAXI14_block, 117 },{ 112, 0, 0, 0, 117 },{ 112, TERM18, 0, 0, 0 },{ 112, TERM16, 0, 0, 0 },{ 112, TERM14, 0, 0, 0 },{ 112, TERM12, 0, 0, 0 },{ 112, TERM10, 0, 0, 0 },{ 112, TERM17, 0, 0, 0 },{ 112, TERM15, 0, 0, 0 },{ 112, TERM13, 0, 0, 0 },{ 112, TERM11, 0, 0, 0 },{ 112, CARG04, 0, 0, 0 },{ 112, CARG05, 0, 0, 0 },{ 112, CARG06, 0, 0, 0 },
	{ 113,       0, 0, TAXI14_block, 117 }, // Unused
	// Airplane Landing Runway04
	{ 114, PRELANDING, RUNW04_block, TAXI22_block | TAXI23_block | TAXI24_block | TAXI26_block, 115 },{ 114, LANDING, 0, 0, 115 },
	{ 115, LANDING, RUNW04_block, 0, 116 },{ 115, FLYING, 0, 0, 132 },{ 115, 0, 0, 0, 116 },
	{ 116, ENDLANDING, 0, TAXI58_block, 117 },{ 116, 0, 0, 0, 117 },{ 116, TERM18, 0, 0, 0 },{ 116, TERM16, 0, 0, 0 },{ 116, TERM14, 0, 0, 0 },{ 116, TERM12, 0, 0, 0 },{ 116, TERM10, 0, 0, 0 },{ 116, TERM17, 0, 0, 0 },{ 116, TERM15, 0, 0, 0 },{ 116, TERM13, 0, 0, 0 },{ 116, TERM11, 0, 0, 0 },{ 116, CARG04, 0, 0, 0 },{ 116, CARG05, 0, 0, 0 },{ 116, CARG06, 0, 0, 0 },
	// End Landing for Runways 3 & 4
	{ 117,      0, 0, TAXI15_block, 118 },
	// Airplane & Helicopter Taxi  (Main Airport flow - Pod 2)
	{ 118,    255, 0, TAXI16_block, 0 },{ 118, HANGAR,  0, 0,  10 },{ 118, TERM10, 0, TAXI17_block | TAXI54_block, 119 },{ 118, TERM11, 0, 0, 119 },{ 118, 0, 0, 0, 120 },
	{ 119, BACKUP, 0, TAXI17_block, 0 },{ 119, TERM11,  0, 0,  26 },{ 119, TERM10, 0, 0, 196 },{ 119, TERM25, 0, 0, 196 },{ 119, TERM26, 0, 0, 196 },{ 119, TERM27, 0, 0, 196 },{ 119, 0, 0, 0, 120 },
	{ 120, BACKUP, 0, TAXI18_block, 0 },{ 120, TERM12,  0, 0,  27 },{ 120, HANGAR, 0, 0,  10 },{ 120, TERM10, 0, 0, 119 },{ 120, TERM11, 0, 0, 119 },{ 120, TERM25, 0, 0, 119 },{ 120, TERM26, 0, 0, 119 },{ 120, TERM27, 0, 0, 119 },{ 120, 0, 0, 0, 121 },
	{ 121, BACKUP, 0, TAXI19_block, 0 },{ 121, TERM13,  0, 0,  28 },{ 121, HANGAR, 0, 0, 120 },{ 121, 0, 0, 0, 122 },
	{ 122, BACKUP, 0, TAXI20_block, 0 }, { 122, TERM14,  0, 0,  29 }, { 122, HELI04, 0, 0,  67 }, { 122, CARG04, 0, 0,  55 }, { 122, HELITAKEOFF, 0, 0, 67 }, { 122, 0, 0, 0, 123 },
	{ 123, BACKUP, 0, TAXI21_block, 0 },{ 123, TERM15,  0, 0,  30 },{ 123, HELI05, 0, 0,  68 },{ 123, CARG05, 0, 0,  56 },{ 123, HELI04, 0, 0, 122 },{ 123, CARG04, 0, 0, 122 },{ 123, 0, 0, 0, 124 },
	{ 124, BACKUP, 0, TAXI22_block, 0 },{ 124, TERM01,  0, 0, 195 },{ 124, HELI06, 0, 0,  69 },{ 124, CARG06, 0, 0,  57 },{ 124, HELI04, 0, 0, 123 },{ 124, CARG04, 0, 0, 123 },{ 124, HELI05, 0, 0, 123 },{ 124, CARG05, 0, 0, 123 },{ 124, TERM02, 0, 0, 195 },{ 124, TERM03, 0, 0, 195 },{ 124, TERM04, 0, 0, 195 },{ 124, HELITAKEOFF, 0, 0,  69 },{ 124, CARG01, 0, 0, 195 },{ 124, CARG02, 0, 0, 195 },{ 124, CARG03, 0, 0, 195 },{ 124, 0, 0, 0, 125 },
	{ 125, BACKUP, 0, TAXI23_block, 0 },{ 125, TERM16,  0, 0,  31 },{ 125, TERM01, 0, 0, 124 },{ 125, TERM02, 0, 0, 124 },{ 125, TERM03, 0, 0, 124 },{ 125, TERM04, 0, 0, 124 },{ 125, HELI04, 0, 0, 124 },{ 125, HELI05, 0, 0, 124 },{ 125, HELI06, 0, 0, 124 },{ 125, HELITAKEOFF, 0, 0, 124 },{ 125, CARG04, 0, 0, 124 },{ 125, CARG05, 0, 0, 124 },{ 125, CARG06, 0, 0, 124 },{ 125, CARG01, 0, 0, 124 },{ 125, CARG02, 0, 0, 124 },{ 125, CARG03, 0, 0, 124 },{ 125, 0, 0, 0, 126 },
	{ 126, BACKUP, 0, TAXI24_block, 0 },{ 126, TERM17,  0, 0,  32 },{ 126, HANGAR, 0, 0,  11 },{ 126, TERM18, 0, 0, 127 },{ 126, TERM16, 0, 0, 125 },{ 126, TERM01, 0, 0, 125 },{ 126, TERM02, 0, 0, 125 },{ 126, TERM03, 0, 0, 125 },{ 126, TERM04, 0, 0, 125 },{ 126, HELI04, 0, 0, 125 },{ 126, HELI05, 0, 0, 125 },{ 126, HELI06, 0, 0, 125 },{ 126, HELITAKEOFF, 0, 0, 125 },{ 126, CARG01, 0, 0, 125 },{ 126, CARG02, 0, 0, 125 },{ 126, CARG03, 0, 0, 125 },{ 126, CARG04, 0, 0, 125 },{ 126, CARG05, 0, 0, 125 },{ 126, CARG06, 0, 0, 125 },{ 126, 0, 0, 0, 128 },
	{ 127, BACKUP, 0, TAXI25_block, 0 },{ 127, TERM18,  0, 0,  33 },{ 127, HANGAR, 0, 0, 126 },{ 127, 0, 0, 0, 128 },
	{ 128,    255, 0, TAXI26_block, 0 },{ 128, 0, 0, 0, 129 },
	// Airplane Takeoff - Runway 04
	{ 129, TAKEOFF, RUNW04_block, TAXI58_block, 130 },
	{ 130, 0, 0, 0, 131 },
	{ 131, STARTTAKEOFF, 0, 0, 132 },
	{ 132, ENDTAKEOFF, 0, 0, 0 },{ 132, 0, 0, 0, 78 },   // End Airport control of airplane.
	//Helicopter -- Hangar 03
	{ 133, 0, HANG03_block, 0, 134 },
	{ 134, 0, HANG03_block, 0,  10 },
	// Helicopter -- takeoff
	{ 135, HELITAKEOFF, 0, 0, 0 },
	{ 136, HELITAKEOFF, 0, 0, 0 },
	{ 137, HELITAKEOFF, 0, 0, 0 },
	// End Pod #2
	// Start of Pod #3
	// Airplane Landing Runway05
	{ 138, PRELANDING, RUNW05_block, 0, 139 },{ 138, LANDING, 0, 0, 139 },
	{ 139, LANDING, RUNW05_block, 0, 140 },{ 139, FLYING, 0, 0, 213 },{ 139, 0, 0, 0, 140 },
	{ 140, ENDLANDING, 0, TAXI27_block, 145 },{ 140, 0, 0, 0, 145 },{ 140, TERM27, 0, 0, 0 },{ 140, TERM25, 0, 0, 0 },{ 140, TERM23, 0, 0, 0 },{ 140, TERM21, 0, 0, 0 },{ 140, TERM19, 0, 0, 0 },{ 140, TERM26, 0, 0, 0 },{ 140, TERM24, 0, 0, 0 },{ 140, TERM22, 0, 0, 0 },{ 140, TERM20, 0, 0, 0 },{ 140, CARG07, 0, 0, 0 },{ 140, CARG08, 0, 0, 0 },{ 140, CARG09, 0, 0, 0 },
	{ 141,       0, 0, TAXI27_block, 145 }, //Unused
	// Airplane Landing Runway06 
	{ 142, PRELANDING, RUNW06_block, TAXI35_block | TAXI36_block | TAXI37_block | TAXI39_block, 143 },{ 142, LANDING, 0, 0, 143 },
	{ 143, LANDING, RUNW06_block, 0, 144 },{ 143, FLYING, 0, 0, 160 },{ 143, 0, 0, 0, 144 },
	{ 144, ENDLANDING, 0, TAXI59_block, 145 },{ 144, 0, 0, 0, 145 },{ 144, TERM27, 0, 0, 0 },{ 144, TERM25, 0, 0, 0 },{ 144, TERM23, 0, 0, 0 },{ 144, TERM21, 0, 0, 0 },{ 144, TERM19, 0, 0, 0 },{ 144, TERM26, 0, 0, 0 },{ 144, TERM24, 0, 0, 0 },{ 144, TERM22, 0, 0, 0 },{ 144, TERM20, 0, 0, 0 },{ 144, CARG07, 0, 0, 0 },{ 144, CARG08, 0, 0, 0 },{ 144, CARG09, 0, 0, 0 },
	// End Landing for Runways 5 & 6
	{ 145,      0, 0, TAXI28_block, 146 },
	// Airplane & Helicopter Taxi  (Main Airport flow - Pod 3)
	{ 146,    255, 0, TAXI29_block, 0 },{ 146, HANGAR, 0, 0,  12 },{ 146, TERM20, 0, 0, 147 },{ 146, TERM19, 0, TAXI30_block | TAXI55_block, 147 },{ 146, 0, 0, 0, 148 },
	{ 147, BACKUP, 0, TAXI30_block, 0 },{ 147, TERM20, 0, 0,  35 },{ 147, TERM19, 0, 0, 198 },{ 147, TERM34, 0, 0, 198 },{ 147, TERM35, 0, 0, 198 },{ 147, TERM36, 0, 0, 198 },{ 147, 0, 0, 0, 148 },
	{ 148, BACKUP, 0, TAXI31_block, 0 },{ 148, TERM21, 0, 0,  36 },{ 148, TERM19, 0, 0, 147 },{ 148, TERM20, 0, 0, 147 },{ 148, TERM34, 0, 0, 147 },{ 148, TERM35, 0, 0, 147 },{ 148, TERM36, 0, 0, 147 },{ 148, HANGAR, 0, 0, 12 },{ 148, 0, 0, 0, 149 },
	{ 149, BACKUP, 0, TAXI32_block, 0 },{ 149, TERM22, 0, 0,  37 },{ 149, HANGAR, 0, 0, 148 },{ 149, 0, 0, 0, 150 },
	{ 150, BACKUP, 0, TAXI33_block, 0 }, { 150, TERM23, 0, 0,  38 }, { 150, HELI07, 0, 0,  70 }, { 150, CARG07, 0, 0,  58 }, { 150, HELITAKEOFF, 0, 0, 70 }, { 150, 0, 0, 0, 151 },
	{ 151, BACKUP, 0, TAXI34_block, 0 },{ 151, TERM24, 0, 0,  39 },{ 151, HELI08, 0, 0,  71 },{ 151, CARG08, 0, 0,  59 },{ 151, HELI07, 0, 0, 150 },{ 151, CARG07, 0, 0, 150 },{ 151, 0, 0, 0, 152 },
	{ 152, BACKUP, 0, TAXI35_block, 0 },{ 152, TERM10, 0, 0, 197 },{ 152, HELI09, 0, 0,  72 },{ 152, CARG09, 0, 0,  60 },{ 152, HELI07, 0, 0, 151 },{ 152, CARG07, 0, 0, 151 },{ 152, HELI08, 0, 0, 151 },{ 152, CARG08, 0, 0, 151 },{ 152, TERM11, 0, 0, 197 },{ 152, TERM12, 0, 0, 197 },{ 152, TERM13, 0, 0, 197 },{ 152, HELITAKEOFF, 0, 0,  72 },{ 152, CARG04, 0, 0, 197 },{ 152, CARG05, 0, 0, 197 },{ 152, CARG06, 0, 0, 197 },{ 152, 0, 0, 0, 153 },
	{ 153, BACKUP, 0, TAXI36_block, 0 },{ 153, TERM25, 0, 0,  40 },{ 153, TERM10, 0, 0, 152 },{ 153, TERM11, 0, 0, 152 },{ 153, TERM12, 0, 0, 152 },{ 153, TERM13, 0, 0, 152 },{ 153, HELI07, 0, 0, 152 },{ 153, HELI08, 0, 0, 152 },{ 153, HELI09, 0, 0, 152 },{ 153, HELITAKEOFF, 0, 0, 152 },{ 153, CARG07, 0, 0, 152 },{ 153, CARG08, 0, 0, 152 },{ 153, CARG09, 0, 0, 152 },{ 153, CARG04, 0, 0, 152 },{ 153, CARG05, 0, 0, 152 },{ 153, CARG06, 0, 0, 152 },{ 153, 0, 0, 0, 154 },
	{ 154, BACKUP, 0, TAXI37_block, 0 },{ 154, TERM26, 0, 0,  41 },{ 154, HANGAR, 0, 0,  13 },{ 154, TERM10, 0, 0, 153 },{ 154, TERM11, 0, 0, 153 },{ 154, TERM12, 0, 0, 153 },{ 154, TERM13, 0, 0, 153 },{ 154, TERM25, 0, 0, 153 },{ 154, TERM27, 0, 0, 155 },{ 154, HELI07, 0, 0, 153 },{ 154, HELI08, 0, 0, 153 },{ 154, HELI09, 0, 0, 153 },{ 154, HELITAKEOFF, 0, 0, 153 },{ 154, CARG07, 0, 0, 153 },{ 154, CARG08, 0, 0, 153 },{ 154, CARG09, 0, 0, 153 },{ 154, CARG04, 0, 0, 153 },{ 154, CARG05, 0, 0, 153 },{ 154, CARG06, 0, 0, 153 },{ 154, 0, 0, 0, 156 },
	{ 155, BACKUP, 0, TAXI38_block, 0 },{ 155, TERM27, 0, 0,  42 },{ 155, HANGAR, 0, 0, 154 },{ 155, 0, 0, 0, 156 },
	{ 156,    255, 0, TAXI39_block, 0 },{ 156, 0, 0, 0, 157 },
	// Airplane Takeoff - Runway 06
	{ 157, TAKEOFF, RUNW06_block, TAXI59_block, 158 },
	{ 158, 0, 0, 0, 159 },
	{ 159, STARTTAKEOFF, 0, 0, 160 },
	{ 160, ENDTAKEOFF, 0, 0, 0 },{ 160, 0, 0, 0, 79 },   // End Airport control of airplane.
														 //Helicopter -- Hangar 05
	{ 161, 0, HANG05_block, 0, 162 },
	{ 162, 0, HANG05_block, 0,  12 },
	// Helicopter -- takeoff
	{ 163, HELITAKEOFF, 0, 0, 0 },
	{ 164, HELITAKEOFF, 0, 0, 0 },
	{ 165, HELITAKEOFF, 0, 0, 0 },
	// End Pod #3
	// Start of Pod #4
	// Airplane Landing Runway07
	{ 166, PRELANDING, 0, RUNW07_block, 167 },{ 166, LANDING, 0, 0, 167 },
	{ 167, LANDING, 0, RUNW07_block, 168 },{ 167, FLYING, 0, 0, 214 },{ 167, 0, 0, 0, 168 },
	{ 168, ENDLANDING, 0, TAXI40_block, 173 },{ 168, 0, 0, 0, 173 },{ 168, TERM36, 0, 0, 0 },{ 168, TERM34, 0, 0, 0 },{ 168, TERM32, 0, 0, 0 },{ 168, TERM30, 0, 0, 0 },{ 168, TERM28, 0, 0, 0 },{ 168, TERM35, 0, 0, 0 },{ 168, TERM33, 0, 0, 0 },{ 168, TERM31, 0, 0, 0 },{ 168, TERM29, 0, 0, 0 },{ 168, CARG10, 0, 0, 0 },{ 168, CARG11, 0, 0, 0 },{ 168, CARG12, 0, 0, 0 },
	{ 169,       0, 0, TAXI40_block, 173 },  //Unused
											 // Airplane Landing Runway08
	{ 170, PRELANDING, 0, RUNW08_block | TAXI48_block | TAXI49_block | TAXI50_block | TAXI52_block, 171 },{ 170, LANDING, 0, 0, 171 },
	{ 171, LANDING, 0, RUNW08_block, 172 },{ 171, FLYING, 0, 0, 188 },{ 171, 0, 0, 0, 172 },
	{ 172, ENDLANDING, 0, TAXI60_block, 173 },{ 172, 0, 0, 0, 173 },{ 172, TERM36, 0, 0, 0 },{ 172, TERM34, 0, 0, 0 },{ 172, TERM32, 0, 0, 0 },{ 172, TERM30, 0, 0, 0 },{ 172, TERM28, 0, 0, 0 },{ 172, TERM35, 0, 0, 0 },{ 172, TERM33, 0, 0, 0 },{ 172, TERM31, 0, 0, 0 },{ 172, TERM29, 0, 0, 0 },{ 172, CARG10, 0, 0, 0 },{ 172, CARG11, 0, 0, 0 },{ 172, CARG12, 0, 0, 0 },
	// End Landing for Runways 7 & 8
	{ 173,      0, 0, TAXI41_block, 174 },
	// Airplane & Helicopter Taxi  (Main Airport flow - Pod 3)
	{ 174,    255, 0, TAXI42_block, 0 },{ 174, HANGAR, 0, 0,  14 },{ 174, TERM28, 0, TAXI43_block | TAXI56_block, 175 },{ 174, TERM29, 0, 0, 175 },{ 174, 0, 0, 0, 176 },
	{ 175, BACKUP, 0, TAXI43_block, 0 },{ 175, TERM29, 0, 0,  44 },{ 175, TERM28, 0, 0, 200 },{ 175, TERM07, 0, 0, 200 },{ 175, TERM08, 0, 0, 200 },{ 175, TERM09, 0, 0, 200 },{ 175, 0, 0, 0, 176 },
	{ 176, BACKUP, 0, TAXI44_block, 0 },{ 176, TERM30, 0, 0,  45 },{ 176, TERM28, 0, 0, 175 },{ 176, TERM29, 0, 0, 175 },{ 176, TERM07, 0, 0, 175 },{ 176, TERM08, 0, 0, 175 },{ 176, TERM09, 0, 0, 175 },{ 176, HANGAR, 0, 0, 14 },{ 176, 0, 0, 0, 177 },
	{ 177, BACKUP, 0, TAXI45_block, 0 },{ 177, TERM31, 0, 0,  46 },{ 177, HANGAR, 0, 0, 176 },{ 177, 0, 0, 0, 178 },
	{ 178, BACKUP, 0, TAXI46_block, 0 }, { 178, TERM32, 0, 0,  47 }, { 178, HELI10, 0, 0,  73 }, { 178, CARG10, 0, 0,  61 }, { 178, HELITAKEOFF, 0, 0, 73 }, { 178, 0, 0, 0, 179 },
	{ 179, BACKUP, 0, TAXI47_block, 0 },{ 179, TERM33, 0, 0,  48 },{ 179, HELI11, 0, 0,  74 },{ 179, CARG11, 0, 0,  62 },{ 179, HELI10, 0, 0, 178 },{ 179, CARG10, 0, 0, 178 },{ 179, 0, 0, 0, 180 },
	{ 180, BACKUP, 0, TAXI48_block, 0 },{ 180, TERM19, 0, 0, 199 },{ 180, HELI12, 0, 0,  75 },{ 180, CARG12, 0, 0,  63 },{ 180, HELI10, 0, 0, 179 },{ 180, CARG10, 0, 0, 179 },{ 180, HELI11, 0, 0, 179 },{ 180, CARG11, 0, 0, 179 },{ 180, TERM20, 0, 0, 199 },{ 180, TERM21, 0, 0, 199 },{ 180, TERM22, 0, 0, 199 },{ 180, HELITAKEOFF, 0, 0,  75 },{ 180, CARG07, 0, 0, 199 },{ 180, CARG08, 0, 0, 199 },{ 180, CARG09, 0, 0, 199 },{ 180, 0, 0, 0, 181 },
	{ 181, BACKUP, 0, TAXI49_block, 0 },{ 181, TERM34, 0, 0,  49 },{ 181, TERM19, 0, 0, 180 },{ 181, TERM20, 0, 0, 180 },{ 181, TERM21, 0, 0, 180 },{ 181, TERM22, 0, 0, 180 },{ 181, HELI10, 0, 0, 180 },{ 181, HELI11, 0, 0, 180 },{ 181, HELI12, 0, 0, 180 },{ 181, HELITAKEOFF, 0, 0, 180 },{ 181, CARG07, 0, 0, 180 },{ 181, CARG08, 0, 0, 180 },{ 181, CARG09, 0, 0, 180 },{ 181, CARG10, 0, 0, 180 },{ 181, CARG11, 0, 0, 180 },{ 181, CARG12, 0, 0, 180 },{ 181, 0, 0, 0, 182 },
	{ 182, BACKUP, 0, TAXI50_block, 0 },{ 182, TERM35, 0, 0,  50 },{ 182, HANGAR, 0, 0,  15 },{ 182, TERM19, 0, 0, 181 },{ 182, TERM20, 0, 0, 181 },{ 182, TERM21, 0, 0, 181 },{ 182, TERM22, 0, 0, 181 },{ 182, TERM34, 0, 0, 181 },{ 182, TERM36, 0, 0, 183 },{ 182, HELI10, 0, 0, 181 },{ 182, HELI11, 0, 0, 181 },{ 182, HELI12, 0, 0, 181 },{ 182, HELITAKEOFF, 0, 0, 181 },{ 182, CARG07, 0, 0, 181 },{ 182, CARG08, 0, 0, 181 },{ 182, CARG09, 0, 0, 181 },{ 182, CARG10, 0, 0, 181 },{ 182, CARG11, 0, 0, 181 },{ 182, CARG12, 0, 0, 181 },{ 182, 0, 0, 0, 184 },
	{ 183, BACKUP, 0, TAXI51_block, 0 },{ 183, TERM36, 0, 0,  51 },{ 183, HANGAR, 0, 0, 182 },{ 183, 0, 0, 0, 184 },
	{ 184,    255, 0, TAXI52_block, 0 },{ 184, 0, 0, 0, 185 },
	// Airplane Takeoff - Runway 08
	{ 185, TAKEOFF, 0, RUNW08_block | TAXI60_block, 186 },
	{ 186, 0, 0, 0, 187 },
	{ 187, STARTTAKEOFF, 0, 0, 188 },
	{ 188, ENDTAKEOFF, 0, 0, 0 },{ 188, 0, 0, 0, 76 },   // End Airport control of airplane.
														 //Helicopter -- Hangar 07
	{ 189, 0, HANG07_block, 0, 190 },
	{ 190, 0, HANG07_block, 0,  14 },
	// Helicopter -- takeoff
	{ 191, HELITAKEOFF, 0, 0, 0 },
	{ 192, HELITAKEOFF, 0, 0, 0 },
	{ 193, HELITAKEOFF, 0, 0, 0 },
	// End Pod #4
	// Crossover between Pod1 and Pod2
	{ 194, BACKUP, 0, TAXI53_block, 0 },{ 194, TERM01, 0, 0,  16 },{ 194, TAKEOFF, 0, 0, 195 },{ 194, HANGAR, 0, 0, 195 },{ 194, TERM16, 0, 0, 195 },{ 194, TERM17, 0, 0, 195 },{ 194, TERM18, 0, 0, 195 },{ 194, 0, 0, 0,  93 },
	{ 195,    255, 0, TAXI53_block, 0 },{ 195, TERM01, 0, 0, 194 },{ 195, TERM02,  0, 0, 194 },{ 195, TERM03, 0, 0, 194 },{ 195, TERM04, 0, 0, 194 },{ 195, CARG01, 0, 0, 194 },{ 195, CARG02, 0, 0, 194 },{ 195, CARG03, 0, 0, 194 },{ 195, 0, 0, 0, 124 },
	// Crossover between Pod2 and Pod3
	{ 196, BACKUP, 0, TAXI54_block, 0 },{ 196, TERM10, 0, 0,  25 },{ 196, TAKEOFF, 0, 0, 197 },{ 196, HANGAR, 0, 0, 197 },{ 196, TERM25, 0, 0, 197 },{ 196, TERM26, 0, 0, 197 },{ 196, TERM27, 0, 0, 197 },{ 196, 0, 0, 0, 119 },
	{ 197,    255, 0, TAXI54_block, 0 },{ 197, TERM10, 0, 0, 196 },{ 197, TERM11,  0, 0, 196 },{ 197, TERM12, 0, 0, 196 },{ 197, TERM13, 0, 0, 196 },{ 197, CARG04, 0, 0, 196 },{ 197, CARG05, 0, 0, 196 },{ 197, CARG06, 0, 0, 196 },{ 197, 0, 0, 0, 152 },
	// Crossover between Pod3 and Pod4
	{ 198, BACKUP, 0, TAXI55_block, 0 },{ 198, TERM19, 0, 0,  34 },{ 198, TAKEOFF, 0, 0, 199 },{ 198, HANGAR, 0, 0, 199 },{ 198, TERM34, 0, 0, 199 },{ 198, TERM35, 0, 0, 199 },{ 198, TERM36, 0, 0, 199 },{ 198, 0, 0, 0, 147 },
	{ 199,    255, 0, TAXI55_block, 0 },{ 199, TERM19, 0, 0, 198 },{ 199, TERM20,  0, 0, 198 },{ 199, TERM21, 0, 0, 198 },{ 199, TERM22, 0, 0, 198 },{ 199, CARG07, 0, 0, 198 },{ 199, CARG08, 0, 0, 198 },{ 199, CARG09, 0, 0, 198 },{ 199, 0, 0, 0, 180 },
	// Crossover between Pod4 and Pod1
	{ 200, BACKUP, 0, TAXI56_block, 0 },{ 200, TERM28, 0, 0,  43 },{ 200, TAKEOFF, 0, 0, 201 },{ 200, HANGAR, 0, 0, 201 },{ 200, TERM07, 0, 0, 201 },{ 200, TERM08, 0, 0, 201 },{ 200, TERM09, 0, 0, 201 },{ 200, 0, 0, 0, 175 },
	{ 201,    255, 0, TAXI56_block, 0 },{ 201, TERM28, 0, 0, 200 },{ 201, TERM29,  0, 0, 200 },{ 201, TERM30, 0, 0, 200 },{ 201, TERM31, 0, 0, 200 },{ 201, CARG10, 0, 0, 200 },{ 201, CARG11, 0, 0, 200 },{ 201, CARG12, 0, 0, 200 },{ 201, 0, 0, 0,  98 },
	// Added Holding Pattern spots.
	{ 202, FLYING, 0, 0, 203 },{ 202, PRELANDING, 0, 0,  88 }, { 202, HELILANDING, 0, 0, 80 }, { 202, 0, 0, 0, 203 },
	{ 203, FLYING, 0, 0,  76 },{ 203, PRELANDING, 0, 0,  84 }, { 203, HELILANDING, 0, 0, 80 }, { 203, 0, 0, 0,  76 },
	{ 204, FLYING, 0, 0, 205 },{ 204, PRELANDING, 0, 0, 114 }, { 204, HELILANDING, 0, 0, 80 }, { 204, 0, 0, 0, 205 },
	{ 205, FLYING, 0, 0,  77 },{ 205, PRELANDING, 0, 0, 110 }, { 205, HELILANDING, 0, 0, 80 }, { 205, 0, 0, 0,  77 },
	{ 206, FLYING, 0, 0, 207 },{ 206, PRELANDING, 0, 0, 142 }, { 206, HELILANDING, 0, 0, 80 }, { 206, 0, 0, 0, 207 },
	{ 207, FLYING, 0, 0,  78 },{ 207, PRELANDING, 0, 0, 138 }, { 207, HELILANDING, 0, 0, 80 }, { 207, 0, 0, 0,  78 },
	{ 208, FLYING, 0, 0, 209 },{ 208, PRELANDING, 0, 0, 170 }, { 208, HELILANDING, 0, 0, 80 }, { 208, 0, 0, 0, 209 },
	{ 209, FLYING, 0, 0,  79 },{ 209, PRELANDING, 0, 0, 166 }, { 209, HELILANDING, 0, 0, 80 }, { 209, 0, 0, 0,  79 },
	// Helicopter -- Entry .  Keeps it from messing up the planes.
	{ 210, FLYING, 0, 0, 210 },{ 210, HELILANDING, 0, 0, 80 },
	// Abort Landings
	{ 211, 0, 0, 0,  77 },
	{ 212, 0, 0, 0,  78 },
	{ 213, 0, 0, 0,  79 },
	{ 214, 0, 0, 0,  76 },
	// Helicopter Positioning
	{ 215, 0, 0, 0, 227 },
	{ 216, 0, 0, 0, 228 },
	{ 217, 0, 0, 0, 229 },
	{ 218, 0, 0, 0, 230 },
	{ 219, 0, 0, 0, 231 },
	{ 220, 0, 0, 0, 232 },
	{ 221, 0, 0, 0, 233 },
	{ 222, 0, 0, 0, 234 },
	{ 223, 0, 0, 0, 235 },
	{ 224, 0, 0, 0, 236 },
	{ 225, 0, 0, 0, 237 },
	{ 226, 0, 0, 0, 238 },
	//  Helicopter landing
	{ 227, 0, 0, 0,  64 },
	{ 228, 0, 0, 0,  65 },
	{ 229, 0, 0, 0,  66 },
	{ 230, 0, 0, 0,  67 },
	{ 231, 0, 0, 0,  68 },
	{ 232, 0, 0, 0,  69 },
	{ 233, 0, 0, 0,  70 },
	{ 234, 0, 0, 0,  71 },
	{ 235, 0, 0, 0,  72 },
	{ 236, 0, 0, 0,  73 },
	{ 237, 0, 0, 0,  74 },
	{ 238, 0, 0, 0,  75 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Intercontinental
static const HangarTileTable _airport_depots_intercontinental[] = { { { 0, 5 }, DIR_SE, 0 },{ { 8, 4 }, DIR_SE, 1 } };
static const TerminalTileTable _airport_terminals_intercontinental[] = { { { 3, 7 }, TERM01_block },{ { 3, 6 }, TERM02_block },{ { 3, 5 }, TERM03_block },{ { 3, 4 }, TERM04_block },{ { 5, 7 }, TERM05_block },{ { 5, 6 }, TERM06_block },{ { 5, 5 }, TERM07_block },{ { 5, 4 }, TERM08_block },{ { 5, 3 }, CARG01_block },{ { 4, 3 }, CARG02_block } };
static const RunwayTerminalTable _airport_runways_intercontinental[] = { { TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | TERM07_block | TERM08_block | CARG01_block | CARG02_block, RUNW01_block | RUNW02_block | RUNW03_block | RUNW04_block, 0 } };
static const byte _airport_terminal_intercontinental[] = { 1, 8 };
static const byte _airport_entries_intercontinental[] = { 17, 16, 19, 18 };
static const byte _airport_helientry_intercontinental[] = { 17, 16, 19, 18 };
static const AirportFTAbuildup _airport_fta_intercontinental[] = {
	// Hangar tiles and areas outside hangars
	{ 0, HANGAR, 0, 0, 2 },{ 0, 0, 0, 0, 2 },{ 0, TERM02, 0, 0, 2 },{ 0, TERM03, 0, 0, 2 },{ 0, TERM04, 0, 0, 2 },{ 0, TERM08, 0, 0, 2 },{ 0, TERM07, 0, 0, 2 },{ 0, TERM06, 0, 0, 2 },{ 0, TERM05, 0, 0, 2 },{ 0, TERM01, 0, 0, 2 },{ 0, HELI02, 0, 0, 0 },{ 0, HELI01, 0, 0, 0 },{ 0, CARG02, 0, 0, 0 },{ 0, CARG01, 0, 0, 0 },
	{ 1, HANGAR, 0, 0, 3 },{ 1, 0, 0, 0, 3 },{ 1, TERM06, 0, 0, 3 },{ 1, TERM05, 0, 0, 3 },{ 1, TERM01, 0, 0, 3 },{ 1, TERM02, 0, 0, 3 },{ 1, TERM03, 0, 0, 3 },{ 1, TERM04, 0, 0, 3 },{ 1, TERM08, 0, 0, 3 },{ 1, TERM07, 0, 0, 3 },{ 1, HELI01, 0, 0, 0 },{ 1, HELI02, 0, 0, 0 },{ 1, CARG01, 0, 0, 0 },{ 1, CARG02, 0, 0, 0 },
	{ 2, 255, HANG01_block, 0, 0 },{ 2, HANGAR, 0, 0, 0 },{ 2, HELITAKEOFF, 0, 0, 82 },{ 2, 0, 0, 0, 32 },
	{ 3, 255, HANG02_block, 0, 0 },{ 3, HANGAR, 0, 0, 1 },{ 3, HELITAKEOFF, 0, 0, 83 },{ 3, TAKEOFF, 0, 0, 60 },{ 3, 0, 0, 0, 60 },
	// Airplane Terminals
	{ 4, TERM01, TERM01_block, 0, 52 },{ 4, 0, 0, 0, 52 },
	{ 5, TERM02, TERM02_block, 0, 33 },{ 5, 0, 0, 0, 33 },
	{ 6, TERM03, TERM03_block, 0, 34 },{ 6, 0, 0, 0, 34 },
	{ 7, TERM04, TERM04_block, 0, 35 },{ 7, 0, 0, 0, 35 },
	{ 8, TERM05, TERM05_block, 0, 46 },{ 8, 0, 0, 0, 46 },
	{ 9, TERM06, TERM06_block, 0, 45 },{ 9, 0, 0, 0, 45 },
	{ 10, TERM07, TERM07_block, 0, 44 },{ 10, 0, 0, 0, 44 },
	{ 11, TERM08, TERM08_block, 0, 43 },{ 11, 0, 0, 0, 43 },
	// Cargo Terminals (Uses Helipads)
	{ 12, CARG01, CARG01_block, 0, 40 },{ 12, 0, 0, 0, 40 },
	{ 13, CARG02, CARG02_block, 0, 39 },{ 13, 0, 0, 0, 39 },
	// Helicopter Terminals
	{ 14, HELI01, HELI01_block, 0, 40 },{ 14, HELITAKEOFF, 0, 0, 80 },{ 14, 0, 0, 0, 40 },
	{ 15, HELI02, HELI02_block, 0, 39 },{ 15, HELITAKEOFF, 0, 0, 81 },{ 15, 0, 0, 0, 39 },
	/* In Air */
	{ 16, 0, 0, 0, 17 },
	{ 17, 0, 0, 0, 92 },
	{ 18, 0, 0, 0, 19 },
	{ 19, 0, 0, 0, 90 },
	// Airplane Landing - Runway01
	{ 20, PRELANDING, RUNW01_block, 0, 21 },{ 20, LANDING, 0, 0, 21 },
	{ 21, LANDING, RUNW01_block, 0, 22 },{ 21, FLYING, 0, 0, 94 },{ 21, 0, 0, 0, 22 },
	{ 22, ENDLANDING, 0, TAXI01_block, 29 },{ 22, 0, 0, 0, 29 },{ 22, TERM02, 0, 0, 0 },{ 22, TERM03, 0, 0, 0 },{ 22, TERM04, 0, 0, 0 },{ 22, TERM08, 0, 0, 0 },{ 22, TERM07, 0, 0, 0 },{ 22, TERM06, 0, 0, 0 },{ 22, TERM05, 0, 0, 0 },{ 22, TERM01, 0, 0, 0 },{ 22, CARG02, 0, 0, 0 },{ 22, CARG01, 0, 0, 0 },
	// Airplane Landing - Runway02
	{ 23, PRELANDING, RUNW02_block, TAXI35_block | TAXI36_block, 24 },{ 23, LANDING, 0, 0, 24 },
	{ 24, LANDING, RUNW02_block, 0, 25 },{ 24, FLYING, 0, 0, 69 },{ 24, 0, 0, 0, 25 },
	{ 25, ENDLANDING, 0, TAXI40_block, 29 },{ 25, 0, 0, 0, 29 },{ 25, TERM02, 0, 0, 0 },{ 25, TERM03, 0, 0, 0 },{ 25, TERM04, 0, 0, 0 },{ 25, TERM08, 0, 0, 0 },{ 25, TERM07, 0, 0, 0 },{ 25, TERM06, 0, 0, 0 },{ 25, TERM05, 0, 0, 0 },{ 25, TERM01, 0, 0, 0 },{ 25, CARG02, 0, 0, 0 },{ 25, CARG01, 0, 0, 0 },
	// Airplane Landing - Runway03
	{ 26, PRELANDING, RUNW03_block, 0, 27 },{ 26, LANDING, 0, 0, 27 },
	{ 27, LANDING, RUNW03_block, 0, 28 },{ 27, FLYING, 0, 0, 95 },{ 27, 0, 0, 0, 28 },
	{ 28, ENDLANDING, 0, TAXI27_block, 54 },{ 28, 0, 0, 0, 54 },{ 28, TERM08, 0, 0, 0 },{ 28, TERM07, 0, 0, 0 },{ 28, TERM06, 0, 0, 0 },{ 28, TERM05, 0, 0, 0 },{ 28, TERM01, 0, 0, 0 },{ 28, TERM02, 0, 0, 0 },{ 28, TERM03, 0, 0, 0 },{ 28, TERM04, 0, 0, 0 },{ 28, CARG01, 0, 0, 0 },{ 28, CARG02, 0, 0, 0 },
	// Runway1 to Main Loop
	{ 29,   0, 0, TAXI02_block, 30 },
	{ 30,   0, 0, TAXI03_block, 31 },
	{ 31, 255, 0, TAXI04_block, 32 },{ 31, HANGAR, 0, 0,  2 },{ 31, 0, 0, 0, 32 },
	{ 32, 255, 0, TAXI05_block, 34 },{ 32, TERM02, 0, 0, 33 },{ 32, 0, 0, 0, 34 },
	//Main Loop
	{ 33, BACKUP, 0, TAXI06_block,  0 },{ 33, TERM02, 0, 0,  5 },{ 33, 0, 0, 0, 34 },
	{ 34, BACKUP, 0, TAXI07_block,  0 },{ 34, TERM03, 0, 0,  6 },{ 34, 0, 0, 0, 35 },
	{ 35, BACKUP, 0, TAXI08_block,  0 },{ 35, TERM04, 0, 0,  7 },{ 35, TAKEOFF, 0, 0, 63 },{ 35, 0, 0, 0, 36 },
	{ 36,    255, 0, TAXI09_block,  0 },{ 36, 0, 0, 0, 37 },
	{ 37,    255, 0, TAXI10_block,  0 },{ 37, 0, 0, 0, 38 },
	{ 38,    255, 0, TAXI11_block,  0 },{ 38, 0, 0, 0, 39 },
	{ 39, BACKUP, 0, TAXI12_block,  0 },{ 39, CARG02, 0, 0, 13 },{ 39, HELI02, 0, 0, 15 },{ 39, 0, 0, 0, 40 },
	{ 40, BACKUP, 0, TAXI13_block,  0 },{ 40, CARG01, 0, 0, 12 },{ 40, HELI01, 0, 0, 14 },{ 40, 0, 0, 0, 41 },
	{ 41,    255, 0, TAXI14_block,  0 },{ 41, 0, 0, 0, 42 },
	{ 42,    255, 0, TAXI15_block,  0 },{ 42, CARG01, 0, 0, 12 },{ 42, 0, 0, 0, 43 },
	{ 43, BACKUP, 0, TAXI16_block,  0 },{ 43, TERM08, 0, 0, 11 },{ 43, 0, 0, 0, 44 },
	{ 44, BACKUP, 0, TAXI17_block,  0 },{ 44, TERM07, 0, 0, 10 },{ 44, HANGAR, 0, 0, 58 },{ 44, TAKEOFF, 0, 0, 58 },{ 44, 0, 0, 0, 45 },
	{ 45, BACKUP, 0, TAXI18_block,  0 },{ 45, TERM06, 0, 0,  9 },{ 45, 0, 0, 0, 46 },
	{ 46, BACKUP, 0, TAXI19_block,  0 },{ 46, TERM05, 0, 0,  8 },{ 46, 0, 0, 0, 47 },
	{ 47,    255, 0, TAXI20_block,  0 },{ 47, 0, 0, 0, 48 },
	{ 48,    255, 0, TAXI21_block,  0 },{ 48, 0, 0, 0, 49 },
	{ 49,    255, 0, TAXI22_block,  0 },{ 49, 0, 0, 0, 50 },
	{ 50,    255, 0, TAXI23_block,  0 },{ 50, TERM01, 0, 0,  4 },{ 50, 0, 0, 0, 51 },
	{ 51,    255, 0, TAXI24_block,  0 },{ 51, 0, 0, 0, 52 },
	{ 52, BACKUP, 0, TAXI25_block,  0 },{ 52, TERM01, 0, 0,  4 },{ 52, HANGAR, 0, 0, 53 },{ 52, 0, 0, 0, 33 },
	// Enter Hangar 1
	{ 53, 0, 0, TAXI26_block, 31 },
	// From Runway 3 to Main Loop
	{ 54,   0, 0, TAXI28_block, 55 },
	{ 55,   0, 0, TAXI29_block, 56 },
	{ 56,   0, 0, TAXI30_block, 57 },
	{ 57,   0, 0, TAXI31_block, 42 },
	// Exit Hangar 2
	{ 58, 0, 0, TAXI32_block,  3 },
	// To Runway 02 and Hangar2
	{ 59,   0, 0, TAXI33_block, 45 },
	{ 60, 255, 0, TAXI34_block, 59 },{ 60, TAKEOFF, 0, 0, 61 },{ 60, 0, 0, 0, 59 },
	{ 61,   0, 0, TAXI35_block, 62 },
	{ 62,   0, 0, TAXI36_block, 66 },
	// To Runway 04
	{ 63, 0, 0, TAXI37_block, 64 },
	{ 64, 0, 0, TAXI38_block, 65 },
	{ 65, 0, 0, TAXI39_block, 70 },
	// Runway02 - Takeoff
	{ 66, TAKEOFF, RUNW02_block, TAXI40_block, 67 }, // takeoff
	{ 67, 0, 0, 0, 68 },
	{ 68, STARTTAKEOFF, 0, 0, 69 },
	{ 69, ENDTAKEOFF, 0, 0, 0 },{ 69, 0, 0, 0, 17 },     // End Airport Control  
	// Runway04 - Takeoff
	{ 70, TAKEOFF, RUNW04_block, TAXI41_block, 71 },
	{ 71, 0, 0, 0, 72 },
	{ 72, STARTTAKEOFF, 0, 0, 73 },
	{ 73, ENDTAKEOFF, 0, 0, 0 },{ 73, 0, 0, 0, 19 },    // End Airport Control
	/* Helicopter -- stay in air in special place as a buffer to choose from helipads */
	{ 74, HELILANDING, 0, 0, 75 },
	{ 75, HELIENDLANDING, 0, 0,  75 }, { 75, FLYING, 0, 0, 17 }, { 75, HELI01, 0, 0, 76 },{ 75, HELI02, 0, 0, 77 },{ 75, HANGAR, 0, 0, 84 }, { 75, 0, 0, 0, 84 },
	// Helicopter Positioning
	{ 76, 0, 0, 0, 78 },
	{ 77, 0, 0, 0, 79 },
	//  Helicopter landing
	{ 78, HELI01, 0, 0, 14 },
	{ 79, HELI02, 0, 0, 15 },
	/* Helicopter -- takeoff */
	{ 80, HELITAKEOFF, 0, 0, 0 },
	{ 81, HELITAKEOFF, 0, 0, 0 },
	{ 82, HELITAKEOFF, 0, 0, 0 },
	{ 83, HELITAKEOFF, 0, 0, 0 },
	// Helicopter to Hangar
	{ 84, 0, 0, 0, 85 }, // need to go to hangar when waiting in air
	{ 85, 0, HANG01_block, 0, 2 },
	// Airplane Landing - Runway04
	{ 86, PRELANDING, RUNW04_block, TAXI38_block | TAXI39_block, 87 },{ 86, LANDING, 0, 0, 87 },
	{ 87, LANDING, RUNW04_block, 0, 88 },{ 87, FLYING, 0, 0, 73 },{ 87, 0, 0, 0, 88 },
	{ 88, ENDLANDING, 0, TAXI41_block, 54 },{ 88, 0, 0, 0, 54 },{ 88, TERM08, 0, 0, 0 },{ 88, TERM07, 0, 0, 0 },{ 88, TERM06, 0, 0, 0 },{ 88, TERM05, 0, 0, 0 },{ 88, TERM01, 0, 0, 0 },{ 88, TERM02, 0, 0, 0 },{ 88, TERM03, 0, 0, 0 },{ 88, TERM04, 0, 0, 0 },{ 88, CARG01, 0, 0, 0 },{ 88, CARG02, 0, 0, 0 },
	// Added Holding Pattern spots.
	{ 89, FLYING, 0, 0, 16 },{ 89, PRELANDING, 0, 0,  20 }, { 89, HELILANDING, 0, 0, 74 }, { 89, 0, 0, 0, 16 },
	{ 90, FLYING, 0, 0, 89 },{ 90, PRELANDING, 0, 0,  23 }, { 90, HELILANDING, 0, 0, 74 }, { 90, 0, 0, 0, 89 },
	{ 91, FLYING, 0, 0, 18 },{ 91, PRELANDING, 0, 0,  26 }, { 91, HELILANDING, 0, 0, 74 }, { 91, 0, 0, 0, 18 },
	{ 92, FLYING, 0, 0, 91 },{ 92, PRELANDING, 0, 0,  86 }, { 92, HELILANDING, 0, 0, 74 }, { 92, 0, 0, 0, 91 },
	// Helicopter -- Entry .  Keeps it from messing up the planes.
	{ 93, FLYING, 0, 0, 93 },{ 93, HELILANDING, 0, 0, 74 },
	// Abort Landings
	{ 94, 0, 0, 0, 17 },
	{ 95, 0, 0, 0, 19 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// SanFran
static const HangarTileTable _airport_depots_sanfran[] = { { { 1, 1 }, DIR_SW, 0 },{ { 9, 1 }, DIR_SE, 1 } };
static const TerminalTileTable _airport_terminals_sanfran[] = { { { 6, 0 }, TERM01_block },{ { 6, 1 }, TERM02_block },{ { 6, 2 }, TERM03_block },{ { 6, 3 }, TERM04_block },{ { 6, 4 }, TERM05_block },{ { 7, 4 }, TERM06_block },{ { 8, 4 }, TERM07_block },{ { 8, 5 }, TERM08_block },{ { 8, 6 }, TERM09_block },
{ { 1, 2 }, CARG01_block },{ { 1, 3 }, CARG02_block },{ { 1, 4 }, CARG03_block },{ { 0, 4 }, CARG04_block } };
static const RunwayTerminalTable _airport_runways_sanfran[] = { { TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | TERM07_block | TERM08_block | TERM09_block | CARG01_block | CARG02_block | CARG03_block | CARG04_block, RUNW01_block | RUNW02_block, 0 } };
static const byte _airport_terminal_sanfran[] = { 1, 9 };
static const byte _airport_entries_sanfran[] = { 22, 19, 25, 23 };  // SW, SE, NE, NW
static const byte _airport_helientry_sanfran[] = { 22, 19, 25, 23 };
static const AirportFTAbuildup _airport_fta_sanfran[] = {
	// Airplane & Helicopter Hangars
	{ 0, HANGAR, 0, 0, 2 },{ 0, 0, 0, 0, 2 },{ 0, TERM07, 0, 0, 2 },{ 0, TERM05, 0, 0, 2 },{ 0, TERM03, 0, 0, 2 },{ 0, TERM01, 0, 0, 2 },{ 0, TERM08, 0, 0, 2 },{ 0, TERM06, 0, 0, 2 },{ 0, TERM04, 0, 0, 2 },{ 0, TERM02, 0, 0, 2 },{ 0, TERM09, 0, 0, 2 },{ 0, HELI01, 0, 0, 2 },{ 0, HELI02, 0, 0, 2 },{ 0, CARG01, 0, 0, 2 },{ 0, CARG02, 0, 0, 2 },{ 0, CARG03, 0, 0, 2 },{ 0, CARG04, 0, 0, 2 },
	{ 1, HANGAR, 0, 0, 3 },{ 1, 0, 0, 0, 3 },{ 1, TERM09, 0, 0, 3 },{ 1, TERM08, 0, 0, 3 },{ 1, TERM07, 0, 0, 3 },
	{ 2,    255, HANG01_block, 0, 0 },{ 2, HANGAR, 0, 0, 0 },{ 2, TAKEOFF, 0, 0, 35 },{ 2, CARG01, 0, 0, 35 },{ 2, CARG02, 0, 0, 35 },{ 2, CARG03, 0, 0, 35 },{ 2, CARG04, 0, 0, 35 },{ 2, HELI01, 0, 0, 35 },{ 2, HELI02, 0, 0, 35 },{ 2, HELITAKEOFF, HELI01_block, 0, 35 }, { 2, 0, 0, 0, 43 },
	{ 3, BACKUP, HANG02_block, 0, 0 },{ 3, HANGAR, 0, 0, 1 },{ 3, TERM09, 0, 0, 12 },{ 3, HELITAKEOFF, HELI01_block, 0, 80 }, { 3, 0, 0, 0, 56 },
	// Airplane Terminals
	{ 4, TERM01, TERM01_block, 0, 45 },{ 4, HANGAR, HANG01_block, TAXI13_block | TAXI12_block | TAXI11_block, 45 },{ 4, 0, 0, 0, 45 },
	{ 5, TERM02, TERM02_block, 0, 46 },{ 5, HANGAR, HANG01_block, TAXI14_block | TAXI12_block | TAXI11_block, 46 },{ 5, 0, 0, 0, 46 },
	{ 6, TERM03, TERM03_block, 0, 47 },{ 6, HANGAR, HANG01_block, TAXI15_block | TAXI14_block | TAXI12_block | TAXI11_block, 47 },{ 6, 0, 0, 0, 47 },
	{ 7, TERM04, TERM04_block, 0, 48 },{ 7, HANGAR, HANG01_block, TAXI16_block | TAXI15_block | TAXI14_block | TAXI12_block | TAXI11_block, 48 },{ 7, 0, 0, 0, 48 },
	{ 8, TERM05, TERM05_block, 0, 50 },{ 8, HANGAR, 0, 0, 50 },{ 8, 0, 0, 0, 50 },
	{ 9, TERM06, TERM06_block, 0, 52 },{ 9, 0, 0, 0, 52 },
	{ 10, TERM07, TERM07_block, 0, 54 },{ 10, HANGAR, HANG02_block, TAXI22_block | TAXI23_block | TAXI24_block, 54 },{ 10, 0, 0, 0, 54 },
	{ 11, TERM08, TERM08_block, 0, 56 },{ 11, HANGAR, HANG02_block, TAXI24_block, 56 },{ 11, 0, 0, 0, 56 },
	{ 12, TERM09, TERM09_block, 0,  3 },{ 12, 0, 0, 0,  3 },
	// Cargo Terminals (Uses Helipads.)
	{ 13, CARG01, CARG01_block, 0, 35 },{ 13, HANGAR, HANG01_block, TAXI03_block, 35 },{ 13, 0, 0, 0, 35 },
	{ 14, CARG02, CARG02_block, 0, 36 },{ 14, HANGAR, HANG01_block, TAXI04_block | TAXI03_block, 36 },{ 14, 0, 0, 0, 36 },
	{ 15, CARG03, CARG03_block, 0, 38 },{ 15, HANGAR, HANG01_block, TAXI06_block | TAXI05_block | TAXI04_block | TAXI03_block, 38 },{ 15, 0, 0, 0, 38 },
	{ 16, CARG04, CARG04_block, 0, 40 },{ 16, HANGAR, HANG01_block, TAXI08_block | TAXI07_block | TAXI06_block | TAXI05_block | TAXI04_block | TAXI03_block, 40 },{ 16, 0, 0, 0, 40 },
	// Helicopter Terminals
	{ 17, HELI01, HELI01_block, 0, 35 },{ 17, HANGAR, HANG01_block, TAXI03_block, 35 },{ 17, HELITAKEOFF, 0, 0, 71 },
	{ 18, HELI02, HELI02_block, 0, 36 },{ 18, HANGAR, HANG01_block, TAXI03_block | TAXI04_block, 36 },{ 18, HELITAKEOFF, 0, 0, 72 },
	// Airplane In Air - Holding Pattern
	{ 19, 0, 0, 0, 20 },
	{ 20, FLYING, 0, 0, 21 },{ 20, PRELANDING, RUNW01_block | RUNW03_block | RUNW04_block, TAXI01_block, 27 }, { 20, 0, 0, 0, 21 },
	{ 21, FLYING, 0, 0, 22 },{ 21, PRELANDING, RUNW02_block | RUNW03_block | RUNW04_block, TAXI02_block, 31 }, { 21, 0, 0, 0, 22 },
	{ 22, 0, 0, 0, 23 },
	{ 23, 0, 0, 0, 25 },
	{ 24, 0, 0, 0, 24 },
	{ 25, FLYING, 0, 0, 19 },{ 25, HELILANDING, 0, 0, 73 },{ 25, 0, 0, 0, 19 },
	{ 26, 0, 0, 0, 26 },
	// Airplane Landing Runway01
	{ 27, PRELANDING, RUNW01_block | RUNW03_block | RUNW04_block, 0, 21 },{ 27, LANDING, RUNW01_block, 0, 28 },{ 27, HELILANDING, 0, 0, 67 },{ 27, FLYING, 0, 0, 21 },
	{ 28, LANDING, RUNW01_block, 0, 29 },{ 28, 0, 0, 0, 29 },
	{ 29,       0, 0, 0, 30 },
	{ 30, ENDLANDING, 0, TAXI01_block, 47 },{ 30, HANGAR, HANG01_block, TAXI12_block | TAXI11_block, 44 },{ 30, TERM01, 0, 0, 44 },{ 30, TERM02, 0, 0, 44 },{ 30, CARG04, HANG01_block, TAXI12_block | TAXI11_block, 44 },{ 30, CARG03, HANG01_block, TAXI12_block | TAXI11_block, 44 },{ 30, CARG02, HANG01_block, TAXI12_block | TAXI11_block, 44 },{ 30, CARG01, HANG01_block, TAXI12_block | TAXI11_block, 44 },{ 30, 0, 0, 0, 47 },{ 30, TERM08, 0, 0, 47 },{ 30, TERM06, 0, 0, 47 },{ 30, TERM04, 0, 0, 47 },{ 30, TERM02, 0, 0, 47 },{ 30, TERM07, 0, 0, 47 },{ 30, TERM05, 0, 0, 47 },{ 30, TERM03, 0, 0, 47 },{ 30, TERM09, 0, 0, 47 },
	// Airplane Landing Runway02
	{ 31, PRELANDING, RUNW02_block | RUNW03_block | RUNW04_block, 0, 22 },{ 31, LANDING, RUNW02_block, 0, 32 },{ 31, HELILANDING, 0, 0, 67 },{ 31, FLYING, 0, 0, 22 },
	{ 32, LANDING, RUNW02_block, 0, 33 },{ 32, 0, 0, 0, 33 },
	{ 33,	      0, 0, 0, 34 },
	{ 34, ENDLANDING, 0, TAXI02_block, 43 },{ 34, HANGAR, HANG01_block, 0,  2 },{ 34, CARG04, 0, 0, 35 },{ 34, CARG03, 0, 0, 35 },{ 34, CARG02, 0, 0, 35 },{ 34, CARG01, 0, 0, 35 },{ 34, 0, 0, 0, 43 },{ 34, TERM09, 0, 0, 43 },{ 34, TERM07, 0, 0, 243 },{ 34, TERM05, 0, 0, 43 },{ 34, TERM03, 0, 0, 43 },{ 34, TERM01, 0, 0, 43 },{ 34, TERM08, 0, 0, 43 },{ 34, TERM06, 0, 0, 43 },{ 34, TERM04, 0, 0, 43 },{ 34, TERM02, 0, 0, 43 },
	// End Landing for Runways 1 & 2
	// Airplane & Helicopter Taxi  (Main Airport flow)
	{ 35, BACKUP, 0, TAXI03_block, 0 },{ 35, HANGAR, 0, 0,  2 },{ 35, CARG01, 0, 0, 13 },{ 35, HELI01, 0, 0, 17 },{ 35, HELITAKEOFF, 0, 0, 17 }, { 35, 0, 0, 0, 36 },
	{ 36, BACKUP, 0, TAXI04_block, 0 },{ 36, HANGAR, 0, 0, 35 },{ 36, CARG02, 0, 0, 14 },{ 36, HELI02, 0, 0, 18 },{ 36, 0, 0, 0, 37 },
	{ 37, BACKUP, 0, TAXI05_block, 0 },{ 37, HANGAR, 0, 0, 36 },{ 37, CARG03, 0, 0, 15 },{ 37, 0, 0, 0, 38 },
	{ 38, BACKUP, 0, TAXI06_block, 0 },{ 38, HANGAR, 0, 0, 37 },{ 38, 0, 0, 0, 39 },
	{ 39,    255, 0, TAXI07_block, 0 },{ 39, HANGAR, 0, 0, 38 },{ 39, 0, 0, 0, 40 },
	{ 40, BACKUP, 0, TAXI08_block, 0 },{ 40, HANGAR, 0, 0, 39 },{ 40, CARG04, 0, 0, 16 },{ 40, 0, 0, 0, 41 },
	{ 41,    255, 0, TAXI09_block, 0 },{ 41, 0, 0, 0, 42 },
	{ 42,    255, 0, TAXI10_block, 0 },{ 42, 0, 0, 0, 59 },
	{ 43,    255, 0, TAXI11_block, 0 },{ 43, HANGAR, 0, 0,  2 },{ 43, CARG01, 0, 0,  2 },{ 43, CARG02, 0, 0,  2 },{ 43, CARG03, 0, 0,  2 },{ 43, CARG04, 0, 0,  2 },{ 43, 0, 0, 0, 44 },
	{ 44,    255, 0, TAXI12_block, 0 },{ 44, TERM01, 0, 0, 45 },{ 44, HANGAR, 0, 0, 43 },{ 44, CARG01, 0, 0, 43 },{ 44, CARG02, 0, 0, 43 },{ 44, CARG03, 0, 0, 43 },{ 44, CARG04, 0, 0, 43 },{ 44, 0, 0, 0, 46 },
	{ 45, BACKUP, 0, TAXI13_block, 0 },{ 45, TERM01, 0, 0,  4 },{ 45, HANGAR, 0, 0, 44 },{ 45, 0, 0, 0, 46 },
	{ 46, BACKUP, 0, TAXI14_block, 0 },{ 46, TERM02, 0, 0,  5 },{ 46, HANGAR, 0, 0, 44 },{ 46, 0, 0, 0, 47 },
	{ 47, BACKUP, 0, TAXI15_block, 0 },{ 47, TERM03, 0, 0,  6 },{ 47, HANGAR, 0, 0, 46 },{ 47, 0, 0, 0, 48 },
	{ 48, BACKUP, 0, TAXI16_block, 0 },{ 48, TERM04, 0, 0,  7 },{ 48, HANGAR, 0, 0, 47 },{ 48, 0, 0, 0, 49 },
	{ 49,    255, 0, TAXI17_block, 0 },{ 49, TERM05, 0, 0,  8 },{ 49, 0, 0, 0, 50 },
	{ 50, BACKUP, 0, TAXI18_block, 0 },{ 50, 0, 0, 0, 51 },
	{ 51,    255, 0, TAXI19_block, 0 },{ 51, 0, 0, 0, 52 },
	{ 52, BACKUP, 0, TAXI20_block, 0 },{ 52, TERM06, 0, 0,  9 },{ 52, 0, 0, 0, 53 },
	{ 53,    255, 0, TAXI21_block, 0 },{ 53, TERM07, 0, 0, 10 },{ 53, TAKEOFF, 0, 0, 58 },{ 53, HANGAR, HANG02_block, TAXI24_block | TAXI23_block | TAXI22_block, 54 },{ 53, TERM09, HANG02_block, TAXI24_block | TAXI23_block | TAXI22_block, 54 },{ 53, TERM08, 0, TAXI24_block | TAXI23_block | TAXI22_block, 54 },{ 53, 0, 0, 0, 54 },
	{ 54, BACKUP, 0, TAXI22_block, 0 },{ 54, HANGAR, 0, 0, 55 },{ 54, TERM09, 0, 0, 55 },{ 54, TERM08, 0, 0, 55 },{ 54, 0, 0, 0, 57 },
	{ 55,    255, 0, TAXI23_block, 0 },{ 55, HANGAR, 0, 0, 56 },{ 55, TERM07, 0, 0, 10 },{ 55, TERM09, 0, 0, 56 },{ 55, TERM08, 0, 0, 56 },{ 55, 0, 0, 0, 54 },
	{ 56, BACKUP, 0, TAXI24_block, 0 },{ 56, TERM08, 0, 0, 11 },{ 56, HANGAR, 0, 0,  3 },{ 56, TERM09, 0, 0,  3 },{ 56, 0, 0, 0, 55 },
	{ 57,    255, 0, TAXI25_block, 0 },{ 57, 0, 0, 0, 58 },
	{ 58,    255, 0, TAXI26_block, 0 },{ 58, 0, 0, 0, 63 },
	// Airplane Takeoff Runway 03
	{ 59, TAKEOFF, RUNW03_block | RUNW01_block | RUNW02_block, 0, 60 },
	{ 60, 0, 0, 0, 61 },
	{ 61, STARTTAKEOFF, 0, 0, 62 },
	{ 62, ENDTAKEOFF, 0, 0, 0 },    // End Airport control of airplane.
	// Airplane Takeoff Runway 04
	{ 63, TAKEOFF, RUNW04_block | RUNW01_block | RUNW02_block, 0, 64 },
	{ 64, 0, 0, 0, 65 },
	{ 65, STARTTAKEOFF, 0, 0, 66 },
	{ 66, ENDTAKEOFF, 0, 0, 0 },    // End Airport control of airplane.
	// Helicopter -- stay in air in special place as a buffer to choose from helipads
	{ 67, HELILANDING, 0, 0, 19 },{ 67, HELIENDLANDING, 0, 0, 68 },{ 67, 0, 0, 0, 19 },
	{ 68, HELIENDLANDING, 0, 0, 69 },{ 68, HELI01, 0, 0, 76 },{ 68, HELI02, 0, 0, 77 },{ 68, HANGAR, 0, 0, 69 },
	//Helicopter -- Hangar
	{ 69, 0, HANG01_block, 0, 70 }, // need to go to hangar when waiting in air
	{ 70, 0, 0, 0,  2 },
	// Helicopter -- takeoff
	{ 71, HELITAKEOFF, 0, 0, 0 },
	{ 72, HELITAKEOFF, 0, 0, 0 },
	// Helicopter -- Entry  This airport is special because of crossing runways.  Helicopters need a seperate entry or it will clear flags for airplanes causing then to run into each other.
	{ 73, FLYING, 0, 0, 67 },{ 73, HELILANDING, 0, 0, 67 },
	// Abort Landings
	{ 74, 0, 0, 0, 22 },
	{ 75, 0, 0, 0, 22 },
	// Helicopter Positioning
	{ 76, 0, 0, 0, 78 },
	{ 77, 0, 0, 0, 79 },
	//  Helicopter landing
	{ 78, 0, 0, 0, 17 },
	{ 79, 0, 0, 0, 18 },
	// Helicopter takeoff outside hangar 2
	{ 80, HELITAKEOFF, 0, 0, 0 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Leipzig
static const HangarTileTable _airport_depots_leipzig[] = { { { 5, 1 }, DIR_SW, 0 },{ { 10, 0 }, DIR_NE, 1 } };
static const TerminalTileTable _airport_terminals_leipzig[] = { { { 6, 2 }, TERM01_block },{ { 6, 3 }, TERM02_block },{ { 6, 4 }, TERM03_block },{ { 6, 5 }, TERM04_block },{ { 6, 6 }, TERM05_block },{ { 10, 1 }, TERM06_block },{ { 10, 2 }, TERM07_block },
{ { 10, 3 }, CARG01_block },{ { 10, 4 }, CARG02_block },{ { 10, 5 }, CARG03_block },{ { 10, 6 }, CARG04_block } };
static const RunwayTerminalTable _airport_runways_leipzig[] = { { TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | TERM07_block | CARG01_block | CARG02_block | CARG03_block | CARG04_block, RUNW01_block | RUNW02_block, 0 } };
static const byte _airport_terminal_leipzig[] = { 1, 7 };
static const byte _airport_entries_leipzig[] = { 23, 17, 20, 21 }; // SW, SE, NE, NW
static const byte _airport_helientry_leipzig[] = { 23, 17, 20, 21 };
static const AirportFTAbuildup _airport_fta_leipzig[] = {
	// Airplane & Helicopter Hangars
	{ 0, HANGAR, 0, 0, 2 },{ 0, 0, 0, 0, 2 },{ 0, TERM05, 0, 0, 2 },{ 0, TERM03, 0, 0, 2 },{ 0, TERM01, 0, 0, 2 },{ 0, TERM04, 0, 0, 2 },{ 0, TERM02, 0, 0, 2 },{ 0, TERM07, 0, 0, 2 },{ 0, TERM06, 0, 0, 2 },{ 0, HELI01, 0, 0, 2 },{ 0, HELI02, 0, 0, 2 },{ 0, CARG01, 0, 0, 2 },{ 0, CARG02, 0, 0, 2 },{ 0, CARG03, 0, 0, 2 },{ 0, CARG04, 0, 0, 2 },
	{ 1, HANGAR, 0, 0, 3 },{ 1, 0, 0, 0, 3 },{ 1, TERM07, 0, 0, 3 },{ 1, TERM06, 0, 0, 3 },{ 1, TERM05, 0, 0, 3 },{ 1, TERM03, 0, 0, 3 },{ 1, TERM01, 0, 0, 3 },{ 1, TERM04, 0, 0, 3 },{ 1, TERM02, 0, 0, 3 },{ 1, HELI02, 0, 0, 3 },{ 1, HELI01, 0, 0, 3 },{ 1, CARG04, 0, 0, 3 },{ 1, CARG02, 0, 0, 3 },{ 1, CARG03, 0, 0, 3 },{ 1, CARG01, 0, 0, 3 },
	{ 2, 255, HANG01_block, 0, 0 },{ 2, HANGAR, 0, 0, 0 },{ 2, TERM06, HANG02_block, TAXI07_block | TAXI08_block | TAXI22_block, 38 },{ 2, TERM07, HANG02_block, TAXI07_block | TAXI08_block | TAXI22_block, 38 },{ 2, CARG01, HANG02_block, TAXI07_block | TAXI08_block | TAXI22_block, 38 },{ 2, CARG02, HANG02_block, TAXI07_block | TAXI08_block | TAXI22_block, 38 },{ 2, CARG03, HANG02_block, TAXI07_block | TAXI08_block | TAXI22_block, 38 },{ 2, CARG04, HANG02_block, TAXI07_block | TAXI08_block | TAXI22_block, 38 },{ 2, HELI01, HANG02_block, TAXI07_block | TAXI08_block | TAXI22_block, 38 },{ 2, HELI02, HANG02_block, TAXI07_block | TAXI08_block | TAXI22_block, 38 },{ 2, HELITAKEOFF, HELI01_block | HANG02_block, TAXI07_block | TAXI08_block | TAXI22_block, 38 }, { 2, 0, 0, 0, 40 },
	{ 3, 255, HANG02_block, 0, 0 },{ 3, HANGAR, 0, 0, 1 },{ 3, TERM01, 0, TAXI08_block | TAXI22_block, 31 },{ 3, TERM02, 0, TAXI08_block | TAXI22_block, 31 },{ 3, TERM03, 0, TAXI08_block | TAXI22_block, 31 },{ 3, TERM04, 0, TAXI08_block | TAXI22_block, 31 },{ 3, TERM05, 0, TAXI08_block | TAXI22_block, 31 },{ 3, HELITAKEOFF, HELI01_block, 0, 53 }, { 3, 0, 0, 0, 53 },
	// Airplane Terminals
	{ 4, TERM01, TERM01_block, 0, 41 },{ 4, HANGAR, HANG01_block, TAXI10_block | TAXI09_block, 41 },{ 4, 0, 0, 0, 41 },
	{ 5, TERM02, TERM02_block, 0, 42 },{ 5, HANGAR, HANG01_block, TAXI11_block | TAXI10_block | TAXI09_block, 42 },{ 5, 0, 0, 0, 42 },
	{ 6, TERM03, TERM03_block, 0, 43 },{ 6, HANGAR, HANG01_block, TAXI12_block | TAXI11_block | TAXI10_block | TAXI09_block, 43 },{ 6, 0, 0, 0, 43 },
	{ 7, TERM04, TERM04_block, 0, 44 },{ 7, HANGAR, HANG01_block, TAXI13_block | TAXI12_block | TAXI11_block | TAXI10_block | TAXI09_block, 44 },{ 7, 0, 0, 0, 44 },
	{ 8, TERM05, TERM05_block, 0, 45 },{ 8, HANGAR, HANG01_block, TAXI14_block | TAXI13_block | TAXI12_block | TAXI11_block | TAXI10_block | TAXI09_block, 45 },{ 8, 0, 0, 0, 45 },
	{ 9, TERM06, TERM06_block, 0, 53 },{ 9, HANGAR, HANG02_block, TAXI23_block, 53 },{ 9, 0, 0, 0, 53 },
	{ 10, TERM07, TERM07_block, 0, 54 },{ 10, HANGAR, HANG02_block, TAXI24_block | TAXI23_block, 54 },{ 10, 0, 0, 0, 54 },
	// Cargo Terminals (Uses Helipads.)
	{ 11, CARG01, CARG01_block, 0, 55 },{ 11, HANGAR, HANG02_block, TAXI25_block | TAXI24_block | TAXI23_block, 55 },{ 11, 0, 0, 0, 55 },
	{ 12, CARG02, CARG02_block, 0, 56 },{ 12, HANGAR, HANG02_block, TAXI26_block | TAXI25_block | TAXI24_block | TAXI23_block, 56 },{ 12, 0, 0, 0, 56 },
	{ 13, CARG03, CARG03_block, 0, 57 },{ 13, HANGAR, HANG02_block, TAXI27_block | TAXI26_block | TAXI25_block | TAXI24_block | TAXI23_block, 57 },{ 13, 0, 0, 0, 57 },
	{ 14, CARG04, CARG04_block, 0, 58 },{ 14, HANGAR, HANG02_block, TAXI28_block | TAXI27_block | TAXI26_block | TAXI25_block | TAXI24_block | TAXI23_block, 58 },{ 14, 0, 0, 0, 58 },
	// Helicopter Terminals
	{ 15, HELI01, HELI01_block, 0, 72 },{ 15, HANGAR, HANG02_block, TAXI25_block | TAXI24_block | TAXI23_block, 55 },{ 15, HELITAKEOFF, 0, 0, 72 },
	{ 16, HELI02, HELI02_block, 0, 73 },{ 16, HANGAR, HANG02_block, TAXI26_block | TAXI25_block | TAXI24_block | TAXI23_block, 56 },{ 16, HELITAKEOFF, 0, 0, 73 },
	// Airplane In Air - Holding Pattern
	{ 17, 0, 0, 0, 18 },
	{ 18, FLYING, 0, 0, 19 },{ 18, PRELANDING, 0, 0, 25 },{ 18, HELILANDING, 0, 0, 68 },{ 18, 0, 0, 0, 19 },
	{ 19, FLYING, 0, 0, 20 },{ 19, PRELANDING, 0, 0, 29 },{ 19, HELILANDING, 0, 0, 68 },{ 19, 0, 0, 0, 20 },
	{ 20, 0, 0, 0, 21 },
	{ 21, 0, 0, 0, 23 },
	{ 22, 0, 0, 0, 22 },
	{ 23, 0, 0, 0, 17 },
	{ 24, 0, 0, 0, 24 },
	// Airplane Landing Runway01
	{ 25, PRELANDING, RUNW01_block, TAXI20_block | TAXI21_block, 26 },{ 25, LANDING, 0, 0, 26 },
	{ 26, LANDING, RUNW01_block, 0, 27 },{ 26, FLYING, 0, 0, 63 },{ 26, 0, 0, 0, 27 },
	{ 27, ENDLANDING, 0, TAXI01_block, 33 },{ 27, 0, 0, 0, 33 },{ 27, TERM05, 0, 0, 0 },{ 27, TERM03, 0, 0, 0 },{ 27, TERM01, 0, 0, 0 },{ 27, TERM04, 0, 0, 0 },{ 27, TERM02, 0, 0, 0 },{ 27, TERM07, 0, 0, 0 },{ 27, TERM06, 0, 0, 0 },{ 27, CARG01, 0, 0, 0 },{ 27, CARG02, 0, 0, 0 },{ 27, CARG03, 0, 0, 0 },{ 27, CARG04, 0, 0, 0 },
	{ 28, 0, 0, TAXI01_block, 28 },  //Unused
	// Airplane Landing Runway02
	{ 29, PRELANDING, RUNW02_block, TAXI28_block | TAXI29_block, 30 },{ 29, LANDING, 0, 0, 30 },
	{ 30, LANDING, RUNW02_block, 0, 31 },{ 30, FLYING, 0, 0, 67 },{ 30, 0, 0, 0, 31 },
	{ 31, ENDLANDING, 0, TAXI22_block,  3 },{ 31, TERM05, 0, 0, 39 },{ 31, TERM03, 0, 0, 39 },{ 31, TERM01, 0, 0, 39 },{ 31, TERM04, 0, 0, 39 },{ 31, TERM02, 0, 0, 39 },{ 31, 0, 0, 0,  3 },{ 31, TERM07, 0, 0,  0 },{ 31, TERM06, 0, 0,  0 },{ 31, CARG04, 0, 0,  0 },{ 31, CARG02, 0, 0,  0 },{ 31, CARG03, 0, 0,  0 },{ 31, CARG01, 0, 0, 0 },
	{ 32,	      0, TAXI22_block, 0, 3 },  //Unused
	// End Landing for Runways 1 & 2
	// Airplane & Helicopter Taxi  (Main Airport flow)
	{ 33,      0, 0, TAXI02_block, 34 },
	{ 34,      0, 0, TAXI03_block, 35 },
	{ 35,      0, 0, TAXI04_block, 36 },
	{ 36,      0, 0, TAXI05_block, 37 },
	{ 37,    255, 0, TAXI06_block, 0 },{ 37, TERM07, HANG02_block, TAXI08_block | TAXI22_block, 38 },{ 37, TERM06, HANG02_block, TAXI08_block | TAXI22_block, 38 },{ 37, CARG01, HANG02_block, TAXI08_block | TAXI22_block, 38 },{ 37, CARG02, HANG02_block, TAXI08_block | TAXI22_block, 38 },{ 37, CARG03, HANG02_block, TAXI08_block | TAXI22_block, 38 },{ 37, CARG04, HANG02_block, TAXI08_block | TAXI22_block, 38 },{ 37, 0, 0, 0, 38 },
	{ 38,    255, 0, TAXI07_block, 0 },{ 38, HANGAR, 0, 0,  2 },{ 38, 0, 0, 0, 39 },
	{ 39,    255, 0, TAXI08_block, 0 },{ 39, TERM07, 0, 0, 31 },{ 39, TERM06, 0, 0, 31 },{ 39, CARG01, 0, 0, 31 },{ 39, CARG02, 0, 0, 31 },{ 39, CARG03, 0, 0, 31 },{ 39, CARG04, 0, 0, 31 },{ 39, HELI01, 0, 0, 31 },{ 39, HELI02, 0, 0, 31 },{ 39, HELITAKEOFF, 0, 0, 31 }, { 39, 0, 0, 0, 40 },
	{ 40,    255, 0, TAXI09_block, 0 },{ 40, HANGAR, 0, 0,  2 },{ 40, 0, 0, 0, 41 },
	{ 41, BACKUP, 0, TAXI10_block, 0 },{ 41, TERM01, 0, 0,  4 },{ 41, HANGAR, 0, 0, 40 },{ 41, 0, 0, 0, 42 },
	{ 42, BACKUP, 0, TAXI11_block, 0 },{ 42, TERM02, 0, 0,  5 },{ 42, HANGAR, 0, 0, 41 },{ 42, 0, 0, 0, 43 },
	{ 43, BACKUP, 0, TAXI12_block, 0 },{ 43, TERM03, 0, 0,  6 },{ 43, HANGAR, 0, 0, 42 },{ 43, 0, 0, 0, 44 },
	{ 44, BACKUP, 0, TAXI13_block, 0 },{ 44, TERM04, 0, 0,  7 },{ 44, HANGAR, 0, 0, 43 },{ 44, 0, 0, 0, 45 },
	{ 45, BACKUP, 0, TAXI14_block, 0 },{ 45, TERM05, 0, 0,  8 },{ 45, HANGAR, 0, 0, 44 },{ 45, 0, 0, 0, 46 },
	{ 46,    255, 0, TAXI15_block, 0 },{ 46, 0, 0, 0, 47 },
	{ 47,    255, 0, TAXI16_block, 0 },{ 47, 0, 0, 0, 48 },
	{ 48,    255, 0, TAXI17_block, 0 },{ 48, 0, 0, 0, 49 },
	{ 49,    255, 0, TAXI18_block, 0 },{ 49, 0, 0, 0, 50 },
	{ 50,    255, 0, TAXI19_block, 0 },{ 50, 0, 0, 0, 51 },
	{ 51,    255, 0, TAXI20_block, 0 },{ 51, 0, 0, 0, 52 },
	{ 52,    255, 0, TAXI21_block, 0 },{ 52, TAKEOFF, RUNW01_block, 0, 60 },
	{ 53, BACKUP, 0, TAXI23_block, 0 },{ 53, TERM06, 0, 0,  9 },{ 53, HANGAR, 0, 0,  3 },{ 53, 0, 0, 0, 54 },
	{ 54, BACKUP, 0, TAXI24_block, 0 },{ 54, TERM07, 0, 0, 10 },{ 54, HANGAR, 0, 0, 53 },{ 54, 0, 0, 0, 55 },
	{ 55, BACKUP, 0, TAXI25_block, 0 },{ 55, CARG01, 0, 0, 11 },{ 55, HELI01, 0, 0, 15 },{ 55, HANGAR, 0, 0, 54 },{ 55, HELITAKEOFF, 0, 0, 15 }, { 55, 0, 0, 0, 56 },
	{ 56, BACKUP, 0, TAXI26_block, 0 },{ 56, CARG02, 0, 0, 12 },{ 56, HELI02, 0, 0, 16 },{ 56, HANGAR, 0, 0, 55 },{ 56, 0, 0, 0, 57 },
	{ 57, BACKUP, 0, TAXI27_block, 0 },{ 57, CARG03, 0, 0, 13 },{ 57, HANGAR, 0, 0, 56 },{ 57, 0, 0, 0, 58 },
	{ 58, BACKUP, 0, TAXI28_block, 0 },{ 58, CARG04, 0, 0, 14 },{ 58, HANGAR, 0, 0, 57 },{ 58, 0, 0, 0, 59 },
	{ 59,    255, 0, TAXI29_block, 0 },{ 59, TAKEOFF, 0, 0, 64 },
	// Airplane Takeoff Runway 01
	{ 60, TAKEOFF, RUNW01_block, TAXI01_block, 61 },
	{ 61, 0, 0, 0, 62 },
	{ 62, STARTTAKEOFF, 0, 0, 63 },
	{ 63, ENDTAKEOFF, 0, 0, 0 },{ 63, 0, 0, 0, 20 },   // End Airport control of airplane.
	// Airplane Takeoff Runway 02
	{ 64, TAKEOFF, RUNW02_block, TAXI22_block, 66 },
	{ 65, 0, 0, TAXI22_block, 66 },  // Skipped at this airport only to keep a plane from taxiing in front of a plane taking off.
	{ 66, STARTTAKEOFF, 0, 0, 67 },
	{ 67, ENDTAKEOFF, 0, 0, 0 },{ 67, 0, 0, 0, 23 },   // End Airport control of airplane.
	// Helicopter -- stay in air in special place as a buffer to choose from helipads
	{ 68, HELILANDING, 0, 0, 17 },{ 68, HELIENDLANDING, 0, 0, 69 },{ 68, 0, 0, 0, 17 },
	{ 69, HELIENDLANDING, 0, 0, 69 },{ 69, HELI01, 0, 0, 75 },{ 69, HELI02, 0, 0, 76 },{ 69, HANGAR, 0, 0, 70 },
	//Helicopter -- Hangar
	{ 70, 0, HANG02_block, 0, 71 }, // need to go to hangar when waiting in air
	{ 71, 0, 0, 0,  3 },
	// Helicopter -- takeoff
	{ 72, HELITAKEOFF, 0, 0, 0 },
	{ 73, HELITAKEOFF, 0, 0, 0 },
	// Helicopter -- Entry
	{ 74, FLYING, 0, 0, 68 },{ 74, HELILANDING, 0, 0, 68 },
	// Helicopter Positioning
	{ 75, 0, 0, 0, 77 },
	{ 76, 0, 0, 0, 78 },
	//  Helicopter landing
	{ 77, 0, 0, 0, 15 },
	{ 78, 0, 0, 0, 16 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Cargo Small
static const HangarTileTable _airport_depots_cargos[] = { { { 0, 1 }, DIR_SE, 0 },{ { 6, 1 }, DIR_SE, 1 } };
static const TerminalTileTable _airport_terminals_cargos[] = { { { 1, 1 }, CARG01_block },{ { 1, 0 }, CARG02_block },{ { 5, 1 }, CARG03_block },{ { 5, 0 }, CARG04_block } };
static const RunwayTerminalTable _airport_runways_cargos[] = { { CARG01_block | CARG02_block | CARG03_block | CARG04_block, RUNW01_block, 0 } };
static const byte _airport_terminal_cargos[] = { 1, 0 };
static const byte _airport_entries_cargos[] = { 13, 12, 16, 14 };
static const byte _airport_helientry_cargos[] = { 13, 12, 16, 14 };
static const AirportFTAbuildup _airport_fta_cargos[] = {
	// Airplane & Helicopter Hangars
	{ 0, HANGAR, 0, 0, 0 },{ 0, 0, 0, 0, 2 },{ 0, CARG03, 0, 0, 2 },{ 0, CARG01, 0, 0, 2 },{ 0, CARG04, 0, 0, 2 },{ 0, CARG02, 0, 0, 2 },{ 0, HELI03, 0, 0, 2 },{ 0, HELI01, 0, 0, 2 },{ 0, HELI04, 0, 0, 2 },{ 0, HELI02, 0, 0, 2 },
	{ 1, HANGAR, 0, 0, 1 },{ 1, 0, 0, 0, 3 },{ 1, CARG03, 0, 0, 3 },{ 1, CARG04, 0, 0, 3 },{ 1, HELI03, 0, 0, 3 },{ 1, HELI04, 0, 0, 3 },
	{ 2,    255, HANG01_block, 0, 0 },{ 2, HANGAR, 0, 0, 0 },{ 2, HELITAKEOFF, 0, 0, 46 },{ 2, 0, 0, 0, 24 },
	{ 3,    255, HANG02_block, 0, 1 },{ 3, HANGAR, 0, 0, 1 },{ 3, CARG04, 0, TAXI11_block | TAXI10_block | TAXI05_block | TAXI06_block, 28 },{ 3, CARG03, 0, TAXI06_block, 28 },{ 3, HELI04, 0, TAXI11_block | TAXI10_block | TAXI05_block | TAXI06_block, 28 },{ 3, HELI03, 0, TAXI06_block, 28 },{ 3, HELITAKEOFF, 0, 0, 47 },{ 3, 0, 0, 0, 29 },
	// Cargo Terminals
	{ 4, CARG01, CARG01_block, 0, 30 },{ 4, HANGAR, HANG01_block, TAXI08_block | TAXI03_block | TAXI02_block, 30 },{ 4, 0, 0, 0, 30 },
	{ 5, CARG02, CARG02_block, 0, 31 },{ 5, HANGAR, HANG01_block, TAXI09_block | TAXI08_block | TAXI03_block | TAXI02_block, 31 },{ 5, 0, 0, 0, 31 },
	{ 6, CARG03, CARG03_block, 0, 32 },{ 6, 0, 0, 0, 32 },
	{ 7, CARG04, CARG04_block, 0, 33 },{ 7, 0, 0, 0, 33 },
	// Helicopter Terminals
	{ 8, HELI01, HELI01_block, 0, 30 },{ 8, HELITAKEOFF, 0, 0, 42 },{ 8, HANGAR, HANG01_block, TAXI08_block | TAXI03_block | TAXI02_block, 30 },{ 8, 0, 0, 0, 30 },
	{ 9, HELI02, HELI02_block, 0, 31 },{ 9, HELITAKEOFF, 0, 0, 43 },{ 9, HANGAR, HANG01_block, TAXI09_block | TAXI08_block | TAXI03_block | TAXI02_block, 31 },{ 9, 0, 0, 0, 31 },
	{ 10, HELI03, HELI03_block, 0, 32 },{ 10, HELITAKEOFF, 0, 0, 44 },{ 10, 0, 0, 0, 32 },
	{ 11, HELI04, HELI04_block, 0, 33 },{ 11, HELITAKEOFF, 0, 0, 45 },{ 11, 0, 0, 0, 33 },
	// Airplane Holding Pattern
	{ 12, FLYING, 0, 0, 13 }, { 12, HELILANDING, 0, 0, 38 }, { 12, 0, 0, 0, 13 },
	{ 13, FLYING, 0, 0, 14 }, { 13, HELILANDING, 0, 0, 38 }, { 13, 0, 0, 0, 14 },
	{ 14, FLYING, 0, 0, 16 }, { 14, PRELANDING, 0, 0, 20 }, { 14, HELILANDING, 0, 0, 38 }, { 14, 0, 0, 0, 16 },
	{ 15, 0, 0, 0, 16 }, //Unused
	{ 16, FLYING, 0, 0, 12 }, { 16, HELILANDING, 0, 0, 38 }, { 16, 0, 0, 0, 12 },
	{ 17, 0, 0, 0, 11 },
	{ 18, 0, 0, 0, 12 },
	{ 19, 0, 0, 0, 17 },
	// Airplane Landing Runway 01
	{ 20, PRELANDING, RUNW01_block, TAXI07_block, 21 },{ 20, LANDING, 0, 0, 21 },
	{ 21, LANDING, 0, 0, 22 },{ 21, FLYING, 0, 0, 37 },{ 21, 0, 0, 0, 22 },
	{ 22, ENDLANDING, 0, TAXI01_block,  2 },{ 22, 0, 0, 0,  2 },{ 22, CARG03, 0, 0, 0 },{ 22, CARG01, 0, 0, 0 },{ 22, CARG04, 0, 0, 0 },{ 22, CARG02, 0, 0, 0 },
	{ 23, 0, 0, TAXI01_block, 2 },  //Unused
	// Main Airport Flow
	{ 24,    255, 0, TAXI02_block, 25 },{ 24, CARG01, 0, 0,  4 },{ 24, HELI01, 0, 0,  8 },{ 24, CARG02, 0, TAXI09_block | TAXI08_block | TAXI03_block, 25 },{ 24, HELI02, 0, TAXI09_block | TAXI08_block | TAXI03_block, 25 },{ 24, HANGAR, 0, 0, 2 },{ 24, 0, 0, 0, 25 },
	{ 25,    255, 0, TAXI03_block, 26 },{ 25, CARG01, 0, 0, 30 },{ 25, CARG02, 0, 0, 30 },{ 25, HELI01, 0, 0, 30 },{ 25, HELI02, 0, 0, 30 },{ 25, HANGAR, 0, 0, 24 },{ 25, 0, 0, 0, 26 },
	{ 26,    255, 0, TAXI04_block, 27 },{ 26, CARG04, 0, TAXI11_block | TAXI10_block | TAXI05_block, 27 },{ 26, HELI04, 0, TAXI11_block | TAXI10_block | TAXI05_block, 27 },{ 26, 0, 0, 0, 27 },
	{ 27,    255, 0, TAXI05_block, 28 },{ 27, CARG04, 0, 0, 32 },{ 27, HELI04, 0, 0, 32 },{ 27, 0, 0, 0, 28 },
	{ 28,    255, 0, TAXI06_block,  3 },{ 28, CARG03, 0, 0,  6 },{ 28, HELI03, 0, 0, 10 },{ 28, CARG04, 0, 0, 27 },{ 28, HELI04, 0, 0, 27 },{ 28, 0, 0, 0,  3 },
	{ 29,    255, 0, TAXI07_block, 34 },{ 29, 0, 0, 0, 34 },
	{ 30, BACKUP, 0, TAXI08_block, 25 },{ 30, CARG01, 0, 0,  4 },{ 30, HELI01, 0, 0,  8 },{ 30, CARG02, 0, 0, 31 },{ 30, HELI02, 0, 0, 31 },{ 30, 0, 0, 0, 25 },
	{ 31, BACKUP, 0, TAXI09_block, 30 },{ 31, CARG02, 0, 0,  5 },{ 31, HELI02, 0, 0,  9 },{ 31, 0, 0, 0, 30 },
	{ 32, BACKUP, 0, TAXI10_block, 27 },{ 32, CARG03, 0, 0,  6 },{ 32, HELI03, 0, 0, 10 },{ 32, CARG04, 0, 0, 33 },{ 32, HELI04, 0, 0, 33 },{ 32, 0, 0, 0, 27 },
	{ 33, BACKUP, 0, TAXI11_block, 32 },{ 33, CARG04, 0, 0,  7 },{ 33, HELI04, 0, 0, 11 },{ 33, 0, 0, 0, 32 },
	// Airplane Takeoff - Runway01
	{ 34, TAKEOFF, RUNW01_block, TAXI01_block, 35 },
	{ 35, 0, 0, 0, 36 },
	{ 36, STARTTAKEOFF, 0, 0, 37 },
	{ 37, ENDTAKEOFF, 0, 0, 0 },{ 37, 0, 0, 0, 13 }, // End Airport Control of planes.
	// Helicopter -- stay in air in special place as a buffer to choose from helipads
	{ 38, HELILANDING, 0, 0, 39 }, { 38, 0, 0, 0, 39 },
	{ 39, HELIENDLANDING, 0, 0, 40 }, { 39, FLYING, 0, 0, 16 }, { 39, HELI01, 0, 0, 49 },{ 39, HELI02, 0, 0, 50 },{ 39, HELI03, 0, 0, 51 },{ 39, HELI04, 0, 0, 52 },{ 39, HANGAR, 0, 0, 40 }, { 39, 0, 0, 0, 40 },
	//Helicopter -- Hangar
	{ 40, 0, HANG01_block, 0, 41 }, // need to go to hangar when waiting in air
	{ 41, 0, 0, 0,  2 },
	// Helicopter -- takeoff
	{ 42, HELITAKEOFF, 0, 0, 0 },
	{ 43, HELITAKEOFF, 0, 0, 0 },
	{ 44, HELITAKEOFF, 0, 0, 0 },
	{ 45, HELITAKEOFF, 0, 0, 0 },
	{ 46, HELITAKEOFF, 0, 0, 0 },
	{ 47, HELITAKEOFF, 0, 0, 0 },
	// Helicopter -- Entry
	{ 48, FLYING, 0, 0, 38 },{ 48, HELILANDING, 0, 0, 38 },
	// Helicopter Positioning
	{ 49, 0, 0, 0, 53 },
	{ 50, 0, 0, 0, 54 },
	{ 51, 0, 0, 0, 55 },
	{ 52, 0, 0, 0, 56 },
	//  Helicopter landing
	{ 53, 0, 0, 0,  8 },
	{ 54, 0, 0, 0,  9 },
	{ 55, 0, 0, 0, 10 },
	{ 56, 0, 0, 0, 11 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Cargo Large
static const HangarTileTable _airport_depots_cargol[] = { { { 1, 5 }, DIR_SE, 0 },{ { 7, 3 }, DIR_SW, 1 } };
static const TerminalTileTable _airport_terminals_cargol[] = { { { 2, 4 }, CARG01_block },{ { 3, 3 }, CARG02_block },{ { 4, 2 }, CARG03_block },{ { 5, 1 }, CARG04_block },{ { 3, 7 }, CARG05_block },{ { 4, 6 }, CARG06_block },{ { 5, 5 }, CARG07_block },{ { 6, 4 }, CARG08_block } };
static const RunwayTerminalTable _airport_runways_cargol[] = { { CARG01_block | CARG02_block | CARG03_block | CARG04_block | CARG05_block | CARG06_block | CARG07_block | CARG08_block, RUNW01_block, 0 } };
static const byte _airport_terminal_cargol[] = { 1, 0 };
static const byte _airport_entries_cargol[] = { 21, 20, 24, 22 };
static const byte _airport_helientry_cargol[] = { 21, 20, 24, 22 };
static const AirportFTAbuildup _airport_fta_cargol[] = {
	/* Hangars and Outside Hangars */
	{ 0, HANGAR, 0, 0, 0 },{ 0, 0, 0, 0, 2 },{ 0, CARG04, 0, 0, 2 },{ 0, CARG08, 0, 0, 2 },{ 0, CARG03, 0, 0, 2 },{ 0, CARG07, 0, 0, 2 },{ 0, CARG02, 0, 0, 2 },{ 0, CARG06, 0, 0, 2 },{ 0, CARG01, 0, 0, 2 },{ 0, CARG05, 0, 0, 2 },{ 0, HELI04, 0, 0, 2 },{ 0, HELI08, 0, 0, 2 },{ 0, HELI03, 0, 0, 2 },{ 0, HELI07, 0, 0, 2 },{ 0, HELI02, 0, 0, 2 },{ 0, HELI06, 0, 0, 2 },{ 0, HELI01, 0, 0, 2 },{ 0, HELI05, 0, 0, 2 },
	{ 1, HANGAR, 0, 0, 1 },{ 1, 0, 0, 0, 3 },{ 1, CARG04, 0, 0, 3 },{ 1, CARG08, 0, 0, 3 },{ 1, CARG03, 0, 0, 3 },{ 1, CARG07, 0, 0, 3 },{ 1, CARG02, 0, 0, 3 },{ 1, HELI04, 0, 0, 3 },{ 1, HELI08, 0, 0, 3 },{ 1, HELI03, 0, 0, 3 },{ 1, HELI07, 0, 0, 3 },{ 1, HELI02, 0, 0, 3 },
	{ 2,    255, HANG01_block, 0, 0 },{ 2, HANGAR, 0, 0, 0 },{ 2, HELITAKEOFF, 0, 0, 55 },{ 2, CARG05, 0, TAXI02_block, 32 },{ 2, HELI05, 0, TAXI02_block, 32 },{ 2, 0, 0, 0, 33 },
	{ 3,    255, HANG02_block, 0, 1 },{ 3, HANGAR, 0, 0, 1 },{ 3, HELITAKEOFF, 0, 0, 56 },{ 3, CARG04, 0, TAXI08_block, 38 },{ 3, CARG08, 0, TAXI07_block, 37 },{ 3, CARG03, 0, TAXI07_block, 37 },{ 3, CARG07, 0, TAXI07_block | TAXI06_block, 37 },{ 3, CARG02, 0, TAXI07_block | TAXI06_block, 37 },{ 3, HELI04, 0, TAXI08_block, 38 },{ 3, HELI08, 0, TAXI07_block, 37 },{ 3, HELI03, 0, TAXI07_block, 37 },{ 3, HELI07, 0, TAXI07_block | TAXI06_block, 37 },{ 3, HELI02, 0, TAXI07_block | TAXI06_block, 37 },{ 3, TAKEOFF, 0, 0, 38 },{ 3, 0, 0, 0, 38 },
	// Cargo Terminals
	{ 4, CARG01, CARG01_block, 0, 34 },{ 4, HANGAR, HANG01_block, TAXI04_block | TAXI03_block, 34 },{ 4, 0, 0, 0, 34 },
	{ 5, CARG02, CARG02_block, 0, 35 },{ 5, 0, 0, 0, 35 },
	{ 6, CARG03, CARG03_block, 0, 36 },{ 6, 0, 0, 0, 36 },
	{ 7, CARG04, CARG04_block, 0, 37 },{ 7, 0, 0, 0, 37 },
	{ 8, CARG05, CARG05_block, 0, 33 },{ 8, HANGAR, HANG01_block, TAXI03_block, 33 },{ 8, 0, 0, 0, 33 },
	{ 9, CARG06, CARG06_block, 0, 34 },{ 9, HANGAR, HANG01_block, TAXI04_block | TAXI03_block, 34 },{ 9, 0, 0, 0, 34 },
	{ 10, CARG07, CARG07_block, 0, 35 },{ 10, 0, 0, 0, 35 },
	{ 11, CARG08, CARG08_block, 0, 36 },{ 11, 0, 0, 0, 36 },
	// Helipads
	{ 12, HELI01, HELI01_block, 0, 34 },{ 12, HELITAKEOFF, 0, 0, 47 },{ 12, HANGAR, HANG01_block, TAXI04_block | TAXI03_block, 34 },{ 12, 0, 0, 0, 34 },
	{ 13, HELI02, HELI02_block, 0, 35 },{ 13, HELITAKEOFF, 0, 0, 48 },{ 13, 0, 0, 0, 35 },
	{ 14, HELI03, HELI03_block, 0, 36 },{ 14, HELITAKEOFF, 0, 0, 49 },{ 14, 0, 0, 0, 36 },
	{ 15, HELI04, HELI04_block, 0, 37 },{ 15, HELITAKEOFF, 0, 0, 50 },{ 15, 0, 0, 0, 37 },
	{ 16, HELI05, HELI05_block, 0, 33 },{ 16, HELITAKEOFF, 0, 0, 51 },{ 16, HANGAR, HANG01_block, TAXI03_block, 33 },{ 16, 0, 0, 0, 33 },
	{ 17, HELI06, HELI06_block, 0, 34 },{ 17, HELITAKEOFF, 0, 0, 52 },{ 17, HANGAR, HANG01_block, TAXI04_block | TAXI03_block, 34 },{ 17, 0, 0, 0, 34 },
	{ 18, HELI07, HELI07_block, 0, 35 },{ 18, HELITAKEOFF, 0, 0, 53 },{ 18, 0, 0, 0, 35 },
	{ 19, HELI08, HELI08_block, 0, 36 },{ 19, HELITAKEOFF, 0, 0, 54 },{ 19, 0, 0, 0, 36 },
	// Airplane Holding Pattern
	{ 20, FLYING, 0, 0, 21 }, { 20, HELILANDING, 0, 0, 43 }, { 20, 0, 0, 0, 21 },
	{ 21, FLYING, 0, 0, 22 }, { 21, HELILANDING, 0, 0, 43 }, { 21, 0, 0, 0, 22 },
	{ 22, FLYING, 0, 0, 24 },{ 22, PRELANDING, 0, 0, 28 }, { 22, HELILANDING, 0, 0, 43 }, { 22, 0, 0, 0, 24 },
	{ 23, 0, 0, 0, 24 },
	{ 24, FLYING, 0, 0, 20 }, { 24, HELILANDING, 0, 0, 43 }, { 24, 0, 0, 0, 20 },
	{ 25, 0, 0, 0, 26 },
	{ 26, 0, 0, 0, 27 },
	{ 27, 0, 0, 0, 28 },
	// Airplane Landing Runway 01
	{ 28, PRELANDING, RUNW01_block, 0, 29 },{ 28, LANDING, 0, 0, 29 },
	{ 29, LANDING, 0, 0, 30 },{ 29, FLYING, 0, 0, 58 },{ 29, 0, 0, 0, 30 },
	{ 30, ENDLANDING, 0, TAXI01_block, 32 },{ 30, 0, 0, 0, 32 },{ 30, CARG04, 0, 0, 0 },{ 30, CARG08, 0, 0, 0 },{ 30, CARG03, 0, 0, 0 },{ 30, CARG07, 0, 0, 0 },{ 30, CARG02, 0, 0, 0 },{ 30, CARG06, 0, 0, 0 },{ 30, CARG01, 0, 0, 0 },{ 30, CARG05, 0, 0, 0 },
	{ 31, 0, 0, TAXI01_block, 31 },   //Unused
	// Main Airport Flow
	{ 32,    255, 0, TAXI02_block, 33 },{ 32, CARG05, 0, 0,  8 },{ 32, HELI05, 0, 0, 16 },{ 32, HANGAR, 0, 0,  2 },{ 32, 0, 0, 0, 33 },
	{ 33, BACKUP, 0, TAXI03_block, 34 },{ 33, CARG01, 0, 0,  4 },{ 33, CARG06, 0, 0,  9 },{ 33, HELI01, 0, 0, 12 },{ 33, HELI06, 0, 0, 17 },{ 33, HANGAR, 0, 0,  2 },{ 33, 0, 0, 0, 34 },
	{ 34, BACKUP, 0, TAXI04_block, 35 },{ 34, CARG02, 0, 0,  5 },{ 34, CARG07, 0, 0, 10 },{ 34, HELI02, 0, 0, 13 },{ 34, HELI07, 0, 0, 18 },{ 34, HANGAR, 0, 0, 33 },{ 34, 0, 0, 0, 35 },
	{ 35, BACKUP, 0, TAXI05_block, 36 },{ 35, CARG03, 0, 0,  6 },{ 35, CARG08, 0, 0, 11 },{ 35, HELI03, 0, 0, 14 },{ 35, HELI08, 0, 0, 19 },{ 35, 0, 0, 0, 36 },
	{ 36, BACKUP, 0, TAXI06_block, 37 },{ 36, CARG04, 0, 0,  7 },{ 36, HELI04, 0, 0, 15 },{ 36, CARG07, 0, 0, 10 },{ 36, CARG02, 0, 0,  5 },{ 36, HELI07, 0, 0, 18 },{ 36, HELI02, 0, 0, 13 },{ 36, 0, 0, 0, 37 },
	{ 37, BACKUP, 0, TAXI07_block, 38 },{ 37, CARG08, 0, 0, 11 },{ 37, CARG03, 0, 0,  6 },{ 37, CARG07, 0, 0, 36 },{ 37, CARG02, 0, 0, 36 },{ 37, HELI08, 0, 0, 19 },{ 37, HELI03, 0, 0, 14 },{ 37, HELI07, 0, 0, 36 },{ 37, HELI02, 0, 0, 36 },{ 37, HANGAR, 0, 0,  3 },{ 37, 0, 0, 0, 38 },
	{ 38,    255, 0, TAXI08_block, 39 },{ 38, CARG04, 0, 0,  7 },{ 38, HELI04, 0, 0, 15 },{ 38, HANGAR, 0, 0,  3 },{ 38, 0, 0, 0, 39 },
	// Airplane Takeoff - Runway02
	{ 39, TAKEOFF, RUNW02_block, 0, 40 },
	{ 40, 0, 0, 0, 41 },
	{ 41, STARTTAKEOFF, 0, 0, 42 },
	{ 42, ENDTAKEOFF, 0, 0, 0 },  // End Airport Control of planes.
	// Helicopter -- stay in air in special place as a buffer to choose from helipads
	{ 43, HELILANDING, 0, 0, 44 }, { 43, 0, 0, 0, 44 },
	{ 44, HELIENDLANDING, 0, 0, 45 }, { 44, FLYING, 0, 0, 24 }, { 44, HELI04, 0, 0, 62 },{ 44, HELI08, 0, 0, 66 },{ 44, HELI03, 0, 0, 61 },{ 44, HELI07, 0, 0, 65 },{ 44, HELI02, 0, 0, 60 },{ 44, HELI06, 0, 0, 64 },{ 44, HELI01, 0, 0, 59 },{ 44, HELI05, 0, 0, 63 },{ 44, HANGAR, 0, 0, 45 }, { 44, 0, 0, 0, 45 },
	//Helicopter -- Hangar
	{ 45, 0, HANG01_block, 0, 46 }, // need to go to hangar when waiting in air
	{ 46, 0, 0, 0,  2 },
	// Helicopter -- takeoff Helipads
	{ 47, HELITAKEOFF, 0, 0, 0 },
	{ 48, HELITAKEOFF, 0, 0, 0 },
	{ 49, HELITAKEOFF, 0, 0, 0 },
	{ 50, HELITAKEOFF, 0, 0, 0 },
	{ 51, HELITAKEOFF, 0, 0, 0 },
	{ 52, HELITAKEOFF, 0, 0, 0 },
	{ 53, HELITAKEOFF, 0, 0, 0 },
	{ 54, HELITAKEOFF, 0, 0, 0 },
	// Helicopter -- takeoff Hangars
	{ 55, HELITAKEOFF, 0, 0, 0 },
	{ 56, HELITAKEOFF, 0, 0, 0 },
	// Helicopter -- Entry
	{ 57, FLYING, 0, 0, 43 },{ 57, HELILANDING, 0, 0, 43 },
	// Abort Landing Runway 01
	{ 58, 0, 0, 0, 21 },
	// Helicopter Positioning
	{ 59, 0, 0, 0, 67 },
	{ 60, 0, 0, 0, 68 },
	{ 61, 0, 0, 0, 69 },
	{ 62, 0, 0, 0, 70 },
	{ 63, 0, 0, 0, 71 },
	{ 64, 0, 0, 0, 72 },
	{ 65, 0, 0, 0, 73 },
	{ 66, 0, 0, 0, 74 },
	//  Helicopter landing
	{ 67, 0, 0, 0, 12 },
	{ 68, 0, 0, 0, 13 },
	{ 69, 0, 0, 0, 14 },
	{ 70, 0, 0, 0, 15 },
	{ 71, 0, 0, 0, 16 },
	{ 72, 0, 0, 0, 17 },
	{ 73, 0, 0, 0, 18 },
	{ 74, 0, 0, 0, 19 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Cargo X-Large
static const HangarTileTable _airport_depots_cargox[] = { { { 2, 0 }, DIR_NE, 0 },{ { 4, 8 }, DIR_SW, 1 } };
static const TerminalTileTable _airport_terminals_cargox[] = { { { 2, 1 }, CARG01_block },{ { 3, 1 }, CARG02_block },{ { 4, 1 }, CARG03_block },{ { 2, 3 }, CARG04_block },{ { 3, 3 }, CARG05_block },{ { 4, 3 }, CARG06_block },
{ { 2, 5 }, CARG07_block },{ { 3, 5 }, CARG08_block },{ { 4, 5 }, CARG09_block },{ { 2, 7 }, CARG10_block },{ { 3, 7 }, CARG11_block },{ { 4, 7 }, CARG12_block } };
static const RunwayTerminalTable _airport_runways_cargox[] = { { CARG01_block | CARG02_block | CARG03_block | CARG04_block | CARG05_block | CARG06_block | CARG07_block | CARG08_block | CARG09_block | CARG10_block | CARG11_block | CARG12_block, RUNW01_block | RUNW02_block, 0 } };
static const byte _airport_terminal_cargox[] = { 1, 0 };
static const byte _airport_entries_cargox[] = { 29, 28, 32, 31 };
static const byte _airport_helientry_cargox[] = { 29, 28, 32, 31 };
static const AirportFTAbuildup _airport_fta_cargox[] = {
	/* Hangars and Outside Hangars */
	{ 0, HANGAR, 0, 0, 2 },{ 0, 0, 0, 0,  2 },{ 0, CARG06, 0, 0, 2 },{ 0, CARG03, 0, 0, 2 },{ 0, CARG05, 0, 0, 2 },{ 0, CARG02, 0, 0, 2 },{ 0, CARG04, 0, 0, 2 },{ 0, CARG01, 0, 0, 2 },{ 0, CARG12, 0, 0, 2 },{ 0, CARG09, 0, 0, 2 },{ 0, CARG11, 0, 0, 2 },{ 0, CARG08, 0, 0, 2 },{ 0, CARG10, 0, 0, 2 },{ 0, CARG07, 0, 0, 2 },{ 0, HELI06, 0, 0, 2 },{ 0, HELI03, 0, 0, 2 },{ 0, HELI05, 0, 0, 2 },{ 0, HELI02, 0, 0, 2 },{ 0, HELI04, 0, 0, 2 },{ 0, HELI01, 0, 0, 2 },{ 0, HELI12, 0, 0, 2 },{ 0, HELI09, 0, 0, 2 },{ 0, HELI11, 0, 0, 2 },{ 0, HELI08, 0, 0, 2 },{ 0, HELI10, 0, 0, 2 },{ 0, HELI07, 0, 0, 2 },
	{ 1, HANGAR, 0, 0, 3 },{ 1, 0, 0, 0,  3 },{ 1, CARG07, 0, 0, 3 },{ 1, CARG10, 0, 0, 3 },{ 1, CARG08, 0, 0, 3 },{ 1, CARG11, 0, 0, 3 },{ 1, CARG12, 0, 0, 3 },{ 1, CARG09, 0, 0, 3 },{ 1, CARG04, 0, 0, 3 },{ 1, CARG01, 0, 0, 3 },{ 1, CARG05, 0, 0, 3 },{ 1, CARG02, 0, 0, 3 },{ 1, CARG06, 0, 0, 3 },{ 1, CARG03, 0, 0, 3 },{ 1, HELI07, 0, 0, 3 },{ 1, HELI10, 0, 0, 3 },{ 1, HELI08, 0, 0, 3 },{ 1, HELI11, 0, 0, 3 },{ 1, HELI12, 0, 0, 3 },{ 1, HELI09, 0, 0, 3 },{ 1, HELI04, 0, 0, 3 },{ 1, HELI01, 0, 0, 3 },{ 1, HELI05, 0, 0, 3 },{ 1, HELI02, 0, 0, 3 },{ 1, HELI06, 0, 0, 3 },{ 1, HELI03, 0, 0, 3 },
	{ 2,    255, HANG01_block, 0, 0 },{ 2, HANGAR, 0, 0, 0 },{ 2, HELITAKEOFF, 0, 0, 90 },{ 2, 0, 0, 0, 44 },
	{ 3,    255, HANG02_block, 0, 0 },{ 3, HANGAR, 0, 0, 1 },{ 3, HELITAKEOFF, 0, 0, 91 },{ 3, 0, 0, 0, 51 },
	/* Cargo Terminals (Uses Helipads) */
	{ 4, CARG01, CARG01_block, 0, 46 },{ 4, 0, 0, 0, 46 },
	{ 5, CARG02, CARG02_block, 0, 47 },{ 5, 0, 0, 0, 47 },
	{ 6, CARG03, CARG03_block, 0, 48 },{ 6, 0, 0, 0, 48 },
	{ 7, CARG04, CARG04_block, 0, 46 },{ 7, 0, 0, 0, 46 },
	{ 8, CARG05, CARG05_block, 0, 47 },{ 8, 0, 0, 0, 47 },
	{ 9, CARG06, CARG06_block, 0, 48 },{ 9, 0, 0, 0, 48 },
	{ 10, CARG07, CARG07_block, 0, 55 },{ 10, 0, 0, 0, 55 },
	{ 11, CARG08, CARG08_block, 0, 54 },{ 11, 0, 0, 0, 54 },
	{ 12, CARG09, CARG09_block, 0, 53 },{ 12, 0, 0, 0, 53 },
	{ 13, CARG10, CARG10_block, 0, 55 },{ 13, 0, 0, 0, 55 },
	{ 14, CARG11, CARG11_block, 0, 54 },{ 14, 0, 0, 0, 54 },
	{ 15, CARG12, CARG12_block, 0, 53 },{ 15, 0, 0, 0, 53 },
	/* Helicopter Terminals */
	{ 16, HELI01, HELI01_block, 0, 46 },{ 16, HELITAKEOFF, 0, 0, 78 },{ 16, 0, 0, 0, 46 },
	{ 17, HELI02, HELI02_block, 0, 47 },{ 17, HELITAKEOFF, 0, 0, 79 },{ 17, 0, 0, 0, 47 },
	{ 18, HELI03, HELI03_block, 0, 48 },{ 18, HELITAKEOFF, 0, 0, 80 },{ 18, 0, 0, 0, 48 },
	{ 19, HELI04, HELI04_block, 0, 46 },{ 19, HELITAKEOFF, 0, 0, 81 },{ 19, 0, 0, 0, 46 },
	{ 20, HELI05, HELI05_block, 0, 47 },{ 20, HELITAKEOFF, 0, 0, 82 },{ 20, 0, 0, 0, 47 },
	{ 21, HELI06, HELI06_block, 0, 48 },{ 21, HELITAKEOFF, 0, 0, 83 },{ 21, 0, 0, 0, 48 },
	{ 22, HELI07, HELI07_block, 0, 55 },{ 22, HELITAKEOFF, 0, 0, 84 },{ 22, 0, 0, 0, 55 },
	{ 23, HELI08, HELI08_block, 0, 54 },{ 23, HELITAKEOFF, 0, 0, 85 },{ 23, 0, 0, 0, 54 },
	{ 24, HELI09, HELI09_block, 0, 53 },{ 24, HELITAKEOFF, 0, 0, 86 },{ 24, 0, 0, 0, 53 },
	{ 25, HELI10, HELI10_block, 0, 55 },{ 25, HELITAKEOFF, 0, 0, 87 },{ 25, 0, 0, 0, 55 },
	{ 26, HELI11, HELI11_block, 0, 54 },{ 26, HELITAKEOFF, 0, 0, 88 },{ 26, 0, 0, 0, 54 },
	{ 27, HELI12, HELI12_block, 0, 53 },{ 27, HELITAKEOFF, 0, 0, 89 },{ 27, 0, 0, 0, 53 },
	// Airplane Holding Pattern
	{ 28, 0, 0, 0, 29 },
	{ 29, FLYING, 0, 0, 31 },{ 29, PRELANDING, 0, 0, 40 }, { 29, HELILANDING, 0, 0, 72 }, { 29, 0, 0, 0, 31 },
	{ 30, 0, 0, 0, 31 },
	{ 31, 0, 0, 0, 32 },
	{ 32, FLYING, 0, 0, 28 },{ 32, PRELANDING, 0, 0, 36 }, { 32, HELILANDING, 0, 0, 72 }, { 32, 0, 0, 0, 28 },
	{ 33, 0, 0, 0, 28 },
	{ 34, 0, 0, 0, 34 },
	{ 35, 0, 0, 0, 35 },
	// Airplane Landing Runway 01
	{ 36, PRELANDING, RUNW01_block, TAXI16_block, 37 },{ 36, LANDING, 0, 0, 37 },
	{ 37, LANDING, RUNW01_block, 0, 38 },{ 37, FLYING, 0, 0, 67 },{ 37, 0, 0, 0, 38 },
	{ 38, ENDLANDING, 0, TAXI01_block,  2 },{ 38, 0, 0, 0,  2 },{ 38, CARG06, 0, 0, 0 },{ 38, CARG03, 0, 0, 0 },{ 38, CARG05, 0, 0, 0 },{ 38, CARG02, 0, 0, 0 },{ 38, CARG04, 0, 0, 0 },{ 38, CARG01, 0, 0, 0 },{ 38, CARG12, 0, 0, 0 },{ 38, CARG09, 0, 0, 0 },{ 38, CARG11, 0, 0, 0 },{ 38, CARG08, 0, 0, 0 },{ 38, CARG10, 0, 0, 0 },{ 38, CARG07, 0, 0, 0 },
	{ 39, 0, 0, TAXI01_block, 39 }, //Unused
	// Airplane Landing Runway 02
	{ 40, PRELANDING, RUNW02_block, TAXI08_block, 41 },{ 40, LANDING, 0, 0, 41 },
	{ 41, LANDING, RUNW02_block, 0, 42 },{ 41, FLYING, 0, 0, 71 },{ 41, 0, 0, 0, 42 },
	{ 42, ENDLANDING, 0, TAXI09_block,  3 },{ 42, 0, 0, 0,  3 },{ 42, CARG07, 0, 0, 0 },{ 42, CARG10, 0, 0, 0 },{ 42, CARG08, 0, 0, 0 },{ 42, CARG11, 0, 0, 0 },{ 42, CARG09, 0, 0, 0 },{ 42, CARG12, 0, 0, 0 },{ 42, CARG01, 0, 0, 0 },{ 42, CARG04, 0, 0, 0 },{ 42, CARG02, 0, 0, 0 },{ 42, CARG05, 0, 0, 0 },{ 42, CARG03, 0, 0, 0 },{ 42, CARG06, 0, 0, 0 },
	{ 43, 0, 0, TAXI09_block, 43 },  // Unused
	/* Airplane Taxi Left side */
	{ 44,    255, 0, TAXI02_block, 45 },{ 44, HANGAR, 0, 0,  2 },{ 44, 0, 0, 0, 45 },
	{ 45,    255, 0, TAXI03_block, 46 },{ 45, HANGAR, 0, 0, 44 },{ 45, 0, 0, 0, 46 },
	{ 46, BACKUP, 0, TAXI04_block, 47 },{ 46, CARG01, 0, 0,  4 },{ 46, CARG04, 0, 0,  7 },{ 46, HELI01, 0, 0, 16 },{ 46, HELI04, 0, 0, 19 },{ 46, 0, 0, 0, 47 },
	{ 47, BACKUP, 0, TAXI05_block, 48 },{ 47, CARG02, 0, 0,  5 },{ 47, CARG05, 0, 0,  8 },{ 47, HELI02, 0, 0, 17 },{ 47, HELI05, 0, 0, 20 },{ 47, 0, 0, 0, 48 },
	{ 48, BACKUP, 0, TAXI06_block, 49 },{ 48, CARG03, 0, 0,  6 },{ 48, CARG06, 0, 0,  9 },{ 48, HELI03, 0, 0, 18 },{ 48, HELI06, 0, 0, 21 },{ 48, 0, 0, 0, 49 },
	{ 49,    255, 0, TAXI07_block, 50 },{ 49, HANGAR, 0, 0, 58 },{ 49, CARG07, 0, 0, 58 },{ 49, CARG08, 0, 0, 58 },{ 49, CARG09, 0, 0, 58 },{ 49, CARG10, 0, 0, 58 },{ 49, CARG11, 0, 0, 58 },{ 49, CARG12, 0, 0, 58 },{ 49, HELI07, 0, 0, 58 },{ 49, HELI08, 0, 0, 58 },{ 49, HELI09, 0, 0, 58 },{ 49, HELI10, 0, 0, 58 },{ 49, HELI11, 0, 0, 58 },{ 49, HELI12, 0, 0, 58 },{ 49, 0, 0, 0, 50 },
	{ 50,      0, 0, TAXI08_block, 68 },
	// Airplane Taxi Right side
	{ 51,    255, 0, TAXI10_block, 52 },{ 51, HANGAR, 0, 0,  3 },{ 51, 0, 0, 0, 52 },
	{ 52,    255, 0, TAXI11_block, 53 },{ 52, HANGAR, 0, 0, 51 },{ 52, 0, 0, 0, 53 },
	{ 53, BACKUP, 0, TAXI12_block, 54 },{ 53, CARG09, 0, 0, 12 },{ 53, CARG12, 0, 0, 15 },{ 53, HELI09, 0, 0, 24 },{ 53, HELI12, 0, 0, 27 },{ 53, 0, 0, 0, 54 },
	{ 54, BACKUP, 0, TAXI13_block, 55 },{ 54, CARG08, 0, 0, 11 },{ 54, CARG11, 0, 0, 14 },{ 54, HELI08, 0, 0, 23 },{ 54, HELI11, 0, 0, 26 },{ 54, 0, 0, 0, 55 },
	{ 55, BACKUP, 0, TAXI14_block, 56 },{ 55, CARG07, 0, 0, 10 },{ 55, CARG10, 0, 0, 13 },{ 55, HELI07, 0, 0, 22 },{ 55, HELI10, 0, 0, 25 },{ 55, 0, 0, 0, 56 },
	{ 56,    255, 0, TAXI15_block, 57 },{ 56, HANGAR, 0, 0, 61 },{ 56, CARG01, 0, 0, 61 },{ 56, CARG02, 0, 0, 61 },{ 56, CARG03, 0, 0, 61 },{ 56, CARG04, 0, 0, 61 },{ 56, CARG05, 0, 0, 61 },{ 56, CARG06, 0, 0, 61 },{ 56, HELI01, 0, 0, 61 },{ 56, HELI02, 0, 0, 61 },{ 56, HELI03, 0, 0, 61 },{ 56, HELI04, 0, 0, 61 },{ 56, HELI05, 0, 0, 61 },{ 56, HELI06, 0, 0, 61 },{ 56, 0, 0, 0, 57 },
	{ 57,      0, 0, TAXI16_block, 64 },
	// Left side to Right side
	{ 58,      0, 0, TAXI17_block, 59 },
	{ 59,      0, 0, TAXI18_block, 60 },
	{ 60,    255, 0, TAXI19_block, 52 },{ 60, HANGAR, HANG02_block, TAXI11_block | TAXI10_block, 52 },{ 60, 0, 0, 0, 52 },
	// Right side to Left Side
	{ 61,      0, 0, TAXI20_block, 62 },
	{ 62,      0, 0, TAXI21_block, 63 },
	{ 63,    255, 0, TAXI22_block, 45 },{ 63, HANGAR, HANG01_block, TAXI03_block | TAXI02_block, 45 },{ 63, 0, 0, 0, 45 },
	// Airplane Takeoff - Runway01
	{ 64, TAKEOFF, RUNW01_block, TAXI01_block, 65 },
	{ 65, 0, 0, 0, 66 },
	{ 66, STARTTAKEOFF, 0, 0, 67 },
	{ 67, ENDTAKEOFF, 0, 0, 0 },{ 67, 0, 0, 0, 29 }, // End Airport Control of planes.
	// Airplane Takeoff - Runway02
	{ 68, TAKEOFF, RUNW02_block, TAXI09_block, 69 },
	{ 69, 0, 0, 0, 70 },
	{ 70, STARTTAKEOFF, 0, 0, 71 },
	{ 71, ENDTAKEOFF, 0, 0, 0 },{ 71, 0, 0, 0, 32 }, // End Airport Control of planes.
	// Helicopter -- stay in air in special place as a buffer to choose from helipads
	{ 72, HELILANDING, 0, 0, 92 },{ 72, HELIENDLANDING, 0, 0, 73 },{ 72, 0, 0, 0, 28 },
	{ 73, HELIENDLANDING, 0, 0, 74 },{ 73, HELI03, 0, 0, 95 },{ 73, HELI09, 0, 0, 101 },{ 73, HELI02, 0, 0, 94 },{ 73, HELI08, 0, 0, 100 },{ 73, HELI01, 0, 0, 93 },{ 73, HELI07, 0, 0, 99 },{ 73, HELI06, 0, 0, 98 },{ 73, HELI12, 0, 0, 104 },{ 73, HELI05, 0, 0, 97 },{ 73, HELI11, 0, 0, 103 },{ 73, HELI04, 0, 0, 96 },{ 73, HELI10, 0, 0, 102 },{ 73, HANGAR, 0, 0, 74 },
	//Helicopter -- Hangar 1
	{ 74, 0, HANG01_block, 0, 75 }, // need to go to hangar when waiting in air
	{ 75, 0, 0, 0,  2 },
	//Helicopter -- Hangar 2
	{ 76, 0, HANG02_block, 0, 77 }, // need to go to hangar when waiting in air
	{ 77, 0, 0, 0,  3 },
	// Helicopter -- takeoff Helipads
	{ 78, HELITAKEOFF, 0, 0, 0 },
	{ 79, HELITAKEOFF, 0, 0, 0 },
	{ 80, HELITAKEOFF, 0, 0, 0 },
	{ 81, HELITAKEOFF, 0, 0, 0 },
	{ 82, HELITAKEOFF, 0, 0, 0 },
	{ 83, HELITAKEOFF, 0, 0, 0 },
	{ 84, HELITAKEOFF, 0, 0, 0 },
	{ 85, HELITAKEOFF, 0, 0, 0 },
	{ 86, HELITAKEOFF, 0, 0, 0 },
	{ 87, HELITAKEOFF, 0, 0, 0 },
	{ 88, HELITAKEOFF, 0, 0, 0 },
	{ 89, HELITAKEOFF, 0, 0, 0 },
	// Helicopter -- takeoff Hangars
	{ 90, HELITAKEOFF, 0, 0, 0 },
	{ 91, HELITAKEOFF, 0, 0, 0 },
	// Helicopter -- Entry
	{ 92, FLYING, 0, 0, 72 },{ 92, HELILANDING, 0, 0, 72 },
	// Helicopter Positioning
	{ 93, 0, 0, 0, 105 },
	{ 94, 0, 0, 0, 106 },
	{ 95, 0, 0, 0, 107 },
	{ 96, 0, 0, 0, 108 },
	{ 97, 0, 0, 0, 109 },
	{ 98, 0, 0, 0, 110 },
	{ 99, 0, 0, 0, 111 },
	{ 100, 0, 0, 0, 112 },
	{ 101, 0, 0, 0, 113 },
	{ 102, 0, 0, 0, 114 },
	{ 103, 0, 0, 0, 115 },
	{ 104, 0, 0, 0, 116 },
	//  Helicopter landing
	{ 105, 0, 0, 0, 16 },
	{ 106, 0, 0, 0, 17 },
	{ 107, 0, 0, 0, 18 },
	{ 108, 0, 0, 0, 19 },
	{ 109, 0, 0, 0, 20 },
	{ 110, 0, 0, 0, 21 },
	{ 111, 0, 0, 0, 22 },
	{ 112, 0, 0, 0, 23 },
	{ 113, 0, 0, 0, 24 },
	{ 114, 0, 0, 0, 25 },
	{ 115, 0, 0, 0, 26 },
	{ 116, 0, 0, 0, 27 },
	/* End Marker */
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Commuter Large
static const HangarTileTable _airport_depots_commuterl[] = { { { 2, 3 }, DIR_NE, 0 },{ { 5, 3 }, DIR_SW, 1 } };
static const TerminalTileTable _airport_terminals_commuterl[] = { { { 1, 1 }, TERM01_block },{ { 2, 1 }, TERM02_block },{ { 3, 1 }, TERM03_block },{ { 4, 1 }, TERM04_block },{ { 5, 1 }, TERM05_block },{ { 6, 1 }, TERM06_block },{ { 3, 3 }, CARG01_block },{ { 4, 3 }, CARG02_block } };
static const RunwayTerminalTable _airport_runways_commuterl[] = { { TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | CARG01_block | CARG02_block, RUNW01_block, 0 } };
static const byte _airport_terminal_commuterl[] = { 1, 6 };
static const byte _airport_entries_commuterl[] = { 15, 14, 18, 16 };
static const byte _airport_helientry_commuterl[] = { 15, 14, 18, 16 };
static const AirportFTAbuildup _airport_fta_commuterl[] = {
	/* Hangars and Outside Hangars */
	{ 0, HANGAR, 0, 0, 2 },{ 0, 0, 0, 0,  2 },{ 0, TERM06, 0, 0, 2 },{ 0, TERM04, 0, 0, 2 },{ 0, TERM02, 0, 0, 2 },{ 0, TERM05, 0, 0, 2 },{ 0, TERM03, 0, 0, 2 },{ 0, TERM01, 0, 0, 2 },{ 0, HELI02, 0, 0, 2 },{ 0, HELI01, 0, 0, 2 },{ 0, CARG02, 0, 0, 2 },{ 0, CARG01, 0, 0, 2 },
	{ 1, HANGAR, 0, 0, 3 },{ 1, 0, 0, 0,  3 },{ 1, TERM04, 0, 0, 3 },{ 1, TERM05, 0, 0, 3 },{ 1, TERM06, 0, 0, 3 },{ 1, CARG02, 0, 0, 3 },{ 1, HELI02, 0, 0, 3 },
	{ 2,    255, HANG01_block, 0, 0 },{ 2, HANGAR, 0, 0, 0 },{ 2, HELITAKEOFF, 0, 0, 44 },{ 2, 0, 0, 0, 27 },
	{ 3,    255, HANG02_block, 0, 0 },{ 3, HANGAR, 0, 0, 1 },{ 3, HELITAKEOFF, 0, 0, 45 },{ 3, TERM04, 0, TAXI06_block | TAXI07_block | TAXI08_block, 32 },{ 3, TERM05, 0, TAXI07_block | TAXI08_block, 32 },{ 3, TERM06, 0, TAXI08_block, 32 },{ 3, CARG02, 0, TAXI06_block | TAXI07_block | TAXI08_block, 32 },{ 3, HELI02, 0, TAXI06_block | TAXI07_block | TAXI08_block, 32 },{ 3, 0, 0, 0, 32 },
	/* Passenger Terminals (Uses Helipads) */
	{ 4, TERM01, TERM01_block, 0, 27 },{ 4, HANGAR, HANG01_block, TAXI03_block, 27 },{ 4, 0, 0, 0, 27 },
	{ 5, TERM02, TERM02_block, 0, 28 },{ 5, HANGAR, HANG01_block, TAXI04_block | TAXI03_block, 28 },{ 5, 0, 0, 0, 28 },
	{ 6, TERM03, TERM03_block, 0, 29 },{ 6, HANGAR, HANG01_block, TAXI05_block | TAXI04_block | TAXI03_block, 29 },{ 6, 0, 0, 0, 29 },
	{ 7, TERM04, TERM04_block, 0, 30 },{ 7, 0, 0, 0, 30 },
	{ 8, TERM05, TERM05_block, 0, 31 },{ 8, 0, 0, 0, 31 },
	{ 9, TERM06, TERM06_block, 0, 32 },{ 9, 0, 0, 0, 32 },
	/* Cargo Terminals (Uses Helipads) */
	{ 10, CARG01, CARG01_block, 0, 29 },{ 10, HANGAR, HANG01_block, TAXI05_block | TAXI04_block | TAXI03_block, 29 },{ 10, 0, 0, 0, 29 },
	{ 11, CARG02, CARG02_block, 0, 30 },{ 11, 0, 0, 0, 30 },
	/* Helicopter Terminals */
	{ 12, HELI01, HELI01_block, 0, 29 },{ 12, HELITAKEOFF, 0, 0, 42 },{ 12, HANGAR, HANG01_block, TAXI05_block | TAXI04_block | TAXI03_block, 29 },{ 12, 0, 0, 0, 29 },
	{ 13, HELI02, HELI02_block, 0, 30 },{ 13, HELITAKEOFF, 0, 0, 43 },{ 13, 0, 0, 0, 30 },
	// Airplane Holding Pattern
	{ 14,      0, 0, 0, 15 },
	{ 15,      0, 0, 0, 16 },
	{ 16,      0, 0, 0, 17 },
	{ 17, FLYING, 0, 0, 18 },{ 17, PRELANDING, 0, 0, 22 }, { 17, HELILANDING, 0, 0, 38 }, { 17, 0, 0, 0, 18 },
	{ 18,      0, 0, 0, 14 },
	{ 19, 0, 0, 0, 14 },
	{ 20, 0, 0, 0, 14 },
	{ 21, 0, 0, 0, 14 },
	// Airplane Landing Runway 01
	{ 22, PRELANDING, RUNW01_block, TAXI09_block, 23 },{ 22, LANDING, 0, 0, 23 },
	{ 23, LANDING, RUNW01_block, 0, 24 },{ 23, FLYING, 0, 0, 37 },{ 23, 0, 0, 0, 24 },
	{ 24, ENDLANDING, 0, TAXI01_block, 26 },{ 24, 0, 0, 0, 26 },{ 24, TERM06, 0, 0, 0 },{ 24, TERM04, 0, 0, 0 },{ 24, TERM02, 0, 0, 0 },{ 24, TERM05, 0, 0, 0 },{ 24, TERM03, 0, 0, 0 },{ 24, TERM01, 0, 0, 0 },{ 24, CARG02, 0, 0, 0 },{ 24, CARG01, 0, 0, 0 },
	{ 25, 0, 0, 0, 25 },   // Unused
	/* Airplane Taxi Left side */
	{ 26,    255, 0, TAXI02_block, 27 },{ 26, HANGAR, 0, 0, 2 },{ 26, 0, 0, 0, 27 },
	{ 27, BACKUP, 0, TAXI03_block, 28 },{ 27, TERM01, 0, 0,  4 },{ 27, HANGAR, 0, 0,  2 },{ 27, 0, 0, 0, 28 },
	{ 28, BACKUP, 0, TAXI04_block, 29 },{ 28, TERM02, 0, 0,  5 },{ 28, HANGAR, 0, 0, 27 },{ 28, 0, 0, 0, 29 },
	{ 29, BACKUP, 0, TAXI05_block, 30 },{ 29, TERM03, 0, 0,  6 },{ 29, CARG01, 0, 0, 10 },{ 29, HELI01, 0, 0, 12 },{ 29, HANGAR, 0, 0, 28 },{ 29, 0, 0, 0, 30 },
	{ 30, BACKUP, 0, TAXI06_block, 31 },{ 30, TERM04, 0, 0,  7 },{ 30, CARG02, 0, 0, 11 },{ 30, HELI02, 0, 0, 13 },{ 30, 0, 0, 0, 31 },
	{ 31, BACKUP, 0, TAXI07_block, 32 },{ 31, TERM05, 0, 0,  8 },{ 31, TERM04, 0, 0, 30 },{ 31, CARG02, 0, 0, 30 },{ 31, HELI02, 0, 0, 30 },{ 31, 0, 0, 0, 32 },
	{ 32, BACKUP, 0, TAXI08_block, 33 },{ 32, TERM06, 0, 0,  9 },{ 32, TERM05, 0, 0, 31 },{ 32, TERM04, 0, 0, 31 },{ 32, CARG02, 0, 0, 31 },{ 32, HELI02, 0, 0, 31 },{ 32, HANGAR, 0, 0, 3 },{ 32, 0, 0, 0, 33 },
	{ 33,    255, 0, TAXI09_block, 34 },{ 33, HANGAR, 0, 0, 3 } ,{ 33, TAKEOFF, 0, 0, 34 },{ 33, 0, 0, 0, 34 },
	// Airplane Takeoff - Runway01
	{ 34, TAKEOFF, RUNW01_block, TAXI01_block, 35 },
	{ 35, 0, 0, 0, 36 },
	{ 36, STARTTAKEOFF, 0, 0, 37 },
	{ 37, ENDTAKEOFF, 0, 0, 0 },{ 37, 0, 0, 0, 14 }, // End Airport Control of planes.
	// Helicopter -- stay in air in special place as a buffer to choose from helipads
	{ 38, HELILANDING, 0, 0, 14 },{ 38, HELIENDLANDING, 0, 0, 39 },{ 38, 0, 0, 0, 14 },
	{ 39, HELIENDLANDING, 0, 0, 40 },{ 39, HELI01, 0, 0, 40 },{ 39, HELI02, 0, 0, 40 },{ 39, HANGAR, 0, 0, 40 },
	//Helicopter -- Hangar 1
	{ 40, 0, HANG01_block, 0, 41 }, // need to go to hangar when waiting in air
	{ 41, 0, 0, 0,  2 },
	// Helicopter -- takeoff Helipads
	{ 42, HELITAKEOFF, 0, 0, 0 },
	{ 43, HELITAKEOFF, 0, 0, 0 },
	// Helicopter -- takeoff Hangars
	{ 44, HELITAKEOFF, 0, 0, 0 },
	{ 45, HELITAKEOFF, 0, 0, 0 },
	// Helicopter -- Entry
	{ 46, FLYING, 0, 0, 38 },{ 46, HELILANDING, 0, 0, 38 },
	/* End Marker */
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Windy City
static const HangarTileTable _airport_depots_windy[] = { { { 2, 1 }, DIR_NE, 0 } };
static const TerminalTileTable _airport_terminals_windy[] = { { { 4, 1 }, TERM01_block },{ { 5, 1 }, CARG01_block } };
static const RunwayTerminalTable _airport_runways_windy[] = { { TERM01_block | CARG01_block, RUNW01_block, 0 } };
static const byte _airport_terminal_windy[] = { 1, 1 };
static const byte _airport_entries_windy[] = { 6, 5, 9, 7 };
static const byte _airport_helientry_windy[] = { 6, 5, 9, 7 };
static const AirportFTAbuildup _airport_fta_windy[] = {
	// Airplane & Helicopter Hangars
	{ 0, HANGAR, 0, 0, 0 },{ 0, 0, 0, 0, 1 },{ 0, TERM01, 0, 0, 1 },{ 0, CARG01, 0, 0, 1 },{ 0, HELI01, 0, 0, 1 },
	{ 1,    255, HANG01_block, 0, 0 },{ 1, HANGAR, 0, 0, 0 },{ 1, HELITAKEOFF, 0, 0, 34 },{ 1, 0, 0, 0, 18 },
	// Terminals
	{ 2, TERM01, TERM01_block, 0, 21 },{ 2, HANGAR, HANG01_block, TAXI03_block | TAXI04_block | TAXI05_block | TAXI06_block, 21 },{ 2, 0, 0, 0, 21 },
	// Cargo Terminals
	{ 3, CARG01, CARG01_block, 0, 22 },{ 3, HANGAR, HANG01_block, TAXI03_block | TAXI04_block | TAXI05_block | TAXI06_block | TAXI07_block, 22 },{ 3, 0, 0, 0, 22 },
	// Cargo Terminals
	{ 4, HELI01, HELI01_block, 0, 22 },{ 4, HELITAKEOFF, 0, 0, 33 },{ 4, HANGAR, HANG01_block, TAXI03_block | TAXI04_block | TAXI05_block | TAXI06_block | TAXI07_block, 22 },{ 4, 0, 0, 0, 22 },
	// Airplane Holding Pattern
	{ 5, 0, 0, 0,  6 },
	{ 6, 0, 0, 0,  7 },
	{ 7, 0, 0, 0,  8 },
	{ 8, FLYING, 0, 0,  9 },{ 8, PRELANDING, 0, 0, 13 },{  8, HELILANDING, 0, 0, 29 },{ 8, 0, 0, 0,  9 },
	{ 9, 0, 0, 0,  5 },
	{ 10, 0, 0, 0, 10 },
	{ 11, 0, 0, 0, 11 },
	{ 12, 0, 0, 0, 12 },
	// Airplane Landing Runway 01
	{ 13, PRELANDING, RUNW01_block, TAXI09_block, 14 },{ 13, LANDING, 0, 0, 14 },
	{ 14, LANDING, RUNW01_block, 0, 15 },{ 14, FLYING, 0, 0, 28 },{ 14, 0, 0, 0, 15 },  // there is a direct reference to this position in aircraft_cmd.cpp, if this position changes, change it there also.
	{ 15, ENDLANDING, RUNW01_block, TAXI01_block, 17 },{ 15, 0, 0, 0, 17 },{ 15, TERM01, 0, 0, 0 },{ 15, CARG01, 0, 0, 0 },
	{ 16, 0, 0, 0, 16 },  // Unused
	// Main Airport Flow
	{ 17,    255, 0, TAXI02_block, 18 },{ 17, HANGAR, 0, 0,  1 },{ 17, 0, 0, 0, 18 },
	{ 18,    255, 0, TAXI03_block, 19 },{ 18, HANGAR, 0, 0,  1 },{ 18, 0, 0, 0, 19 },
	{ 19,    255, 0, TAXI04_block, 20 },{ 19, HANGAR, 0, 0, 18 },{ 19, 0, 0, 0, 20 },
	{ 20,    255, 0, TAXI05_block, 21 },{ 20, HANGAR, 0, 0, 19 },{ 20, 0, 0, 0, 21 },
	{ 21, BACKUP, 0, TAXI06_block, 22 },{ 21, HANGAR, 0, 0, 20 },{ 21, TERM01, 0, 0,  2 },{ 21, 0, 0, 0, 22 },
	{ 22, BACKUP, 0, TAXI07_block, 23 },{ 22, HANGAR, 0, 0, 21 },{ 22, CARG01, 0, 0,  3 },{ 22, HELI01, 0, 0,  4 },{ 22, 0, 0, 0, 23 },
	{ 23,    255, 0, TAXI08_block, 24 },{ 23, 0, 0, 0, 24 },
	{ 24,    255, 0, TAXI09_block, 25 },{ 24, 0, 0, 0, 25 },
	// Airplane Takeoff - Runway01
	{ 25, TAKEOFF, RUNW01_block, 0, 26 },
	{ 26, 0, RUNW01_block, 0, 27 },
	{ 27, STARTTAKEOFF, 0, 0, 28 },  // there is a direct reference to this position in aircraft_cmd.cpp, if this position changes, change it there also.
	{ 28, ENDTAKEOFF, 0, 0, 0 },{ 28, 0, 0, 0,  5 }, // End Airport Control of planes.
	// Helicopter -- stay in air in special place as a buffer to choose from helipads
	{ 29, HELILANDING, 0, 0, 35 },{ 29, HELIENDLANDING, 0, 0, 30 },{ 29, 0, 0, 0, 35 },
	{ 30, HELIENDLANDING, 0, 0, 31 },{ 30, HELI01, 0, 0, 31 },{ 30, HANGAR, 0, 0, 31 },
	//Helicopter -- Hangar
	{ 31, 0, HANG01_block, 0, 32 }, // need to go to hangar when waiting in air
	{ 32, 0, 0, 0,  1 },
	// Helicopter -- takeoff
	{ 33, HELITAKEOFF, 0, 0, 0 },
	{ 34, HELITAKEOFF, 0, 0, 0 },
	// Helicopter -- Entry
	{ 35, FLYING, 0, 0, 29 },{ 35, HELILANDING, 0, 0, 29 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Indianapolis
static const HangarTileTable _airport_depots_indianapolis[] = { { { 1,11 }, DIR_SW, 0 },{ { 1, 2 }, DIR_SW, 1 },{ { 10,10 }, DIR_SE, 2 },{ { 10, 3 }, DIR_NW, 3 } };
static const TerminalTileTable _airport_terminals_indianapolis[] = { { { 9,10 }, TERM01_block },{ { 8,10 }, TERM02_block },{ { 7,10 }, TERM03_block },{ { 6,10 }, TERM04_block },{ { 5,10 }, TERM05_block },{ { 4,10 }, TERM06_block },{ { 4, 9 }, TERM07_block },{ { 4, 8 }, TERM08_block },{ { 5, 8 }, TERM09_block },
{ { 6, 8 }, TERM10_block },{ { 7, 8 }, TERM11_block },{ { 8, 8 }, TERM12_block },{ { 9, 8 }, TERM13_block },{ { 9, 5 }, TERM14_block },{ { 8, 5 }, TERM15_block },{ { 7, 5 }, TERM16_block },{ { 6, 5 }, TERM17_block },{ { 5, 5 }, TERM18_block },
{ { 4, 5 }, TERM19_block },{ { 4, 4 }, TERM20_block },{ { 4, 3 }, TERM21_block },{ { 5, 3 }, TERM22_block },{ { 6, 3 }, TERM23_block },{ { 7, 3 }, TERM24_block },{ { 8, 3 }, TERM25_block },{ { 9, 3 }, TERM26_block },
{ { 1, 4 }, CARG01_block },{ { 1, 5 }, CARG02_block },{ { 1, 6 }, CARG03_block },{ { 1, 7 }, CARG04_block },{ { 1, 8 }, CARG05_block },{ { 1, 9 }, CARG06_block } };
static const RunwayTerminalTable _airport_runways_indianapolis[] = { { TERM06_block | TERM07_block | TERM08_block | TERM09_block | TERM10_block | TERM11_block | TERM12_block | TERM13_block | TERM14_block | TERM15_block | TERM16_block | TERM17_block | TERM18_block | TERM19_block | CARG04_block | CARG05_block | CARG06_block, RUNW01_block, 0 },
{ TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | CARG04_block | CARG05_block | CARG06_block, RUNW02_block, 0 },
{ TERM08_block | TERM09_block | TERM10_block | TERM11_block | TERM12_block | TERM13_block | TERM14_block | TERM15_block | TERM16_block | TERM17_block | TERM18_block | TERM19_block | TERM20_block | TERM21_block | CARG01_block | CARG02_block | CARG03_block, RUNW03_block, 0 },
{ TERM21_block | TERM22_block | TERM23_block | TERM24_block | TERM25_block | TERM26_block | CARG01_block | CARG02_block | CARG03_block, RUNW04_block, 0 } };
static const byte _airport_terminal_indianapolis[] = { 1, 26 };
static const byte _airport_entries_indianapolis[] = { 43, 42, 45, 44 };
static const byte _airport_helientry_indianapolis[] = { 43, 42, 45, 44 };
static const AirportFTAbuildup _airport_fta_indianapolis[] = {
	// Airplane & Helicopter Hangars
	{ 0, HANGAR, 0, 0, 4 },{ 0, 0, 0, 0, 4 },{ 0, TERM06, 0, 0, 4 },{ 0, TERM07, 0, 0, 4 },{ 0, TERM08, 0, 0, 4 },{ 0, TERM19, 0, 0, 4 },{ 0, TERM18, 0, 0, 4 },{ 0, TERM09, 0, 0, 4 },{ 0, TERM17, 0, 0, 4 },{ 0, TERM10, 0, 0, 4 },{ 0, TERM16, 0, 0, 4 },{ 0, TERM11, 0, 0, 4 },{ 0, TERM15, 0, 0, 4 },{ 0, TERM12, 0, 0, 4 },{ 0, TERM14, 0, 0, 4 },{ 0, TERM13, 0, 0, 4 },{ 0, CARG05, 0, 0, 4 },{ 0, CARG06, 0, 0, 4 },{ 0, CARG04, 0, 0, 4 },{ 0, HELI04, 0, 0, 4 },{ 0, HELI03, 0, 0, 4 },
	{ 1, HANGAR, 0, 0, 5 },{ 1, 0, 0, 0, 5 },{ 1, TERM21, 0, 0, 5 },{ 1, TERM20, 0, 0, 5 },{ 1, TERM19, 0, 0, 5 },{ 1, TERM08, 0, 0, 5 },{ 1, TERM18, 0, 0, 5 },{ 1, TERM09, 0, 0, 5 },{ 1, TERM17, 0, 0, 5 },{ 1, TERM10, 0, 0, 5 },{ 1, TERM16, 0, 0, 5 },{ 1, TERM11, 0, 0, 5 },{ 1, TERM15, 0, 0, 5 },{ 1, TERM12, 0, 0, 5 },{ 1, TERM14, 0, 0, 5 },{ 1, TERM13, 0, 0, 5 },{ 1, CARG02, 0, 0, 5 },{ 1, CARG01, 0, 0, 5 },{ 1, CARG03, 0, 0, 5 },{ 1, HELI03, 0, 0, 5 },
	{ 2, HANGAR, 0, 0, 6 },{ 2, 0, 0, 0, 6 },{ 2, TERM06, 0, 0, 6 },{ 2, TERM05, 0, 0, 6 },{ 2, TERM04, 0, 0, 6 },{ 2, TERM03, 0, 0, 6 },{ 2, TERM02, 0, 0, 6 },{ 2, TERM01, 0, 0, 6 },{ 2, CARG06, 0, 0, 6 },{ 2, CARG05, 0, 0, 6 },{ 2, CARG04, 0, 0, 6 },{ 2, HELI04, 0, 0, 6 },
	{ 3, HANGAR, 0, 0, 7 },{ 3, 0, 0, 0, 7 },{ 3, TERM21, 0, 0, 7 },{ 3, TERM22, 0, 0, 7 },{ 3, TERM23, 0, 0, 7 },{ 3, TERM24, 0, 0, 7 },{ 3, TERM25, 0, 0, 7 },{ 3, TERM26, 0, 0, 7 },{ 3, CARG01, 0, 0, 7 },{ 3, CARG02, 0, 0, 7 },{ 3, CARG03, 0, 0, 7 },{ 3, HELI03, 0, 0, 7 },
	{ 4, 255, HANG01_block, 0, 0 },{ 4, HANGAR, 0, 0, 0 },{ 4, TERM06, 0, TAXI02_block | TAXI23_block, 63 },{ 4, TERM07, 0, TAXI02_block | TAXI03_block | TAXI22_block, 63 },{ 4, TERM08, 0, TAXI02_block | TAXI03_block | TAXI04_block | TAXI21_block, 63 },{ 4, TERM09, 0, TAXI18_block, 63 },{ 4, TERM10, 0, TAXI17_block, 63 },{ 4, TERM11, 0, TAXI16_block, 63 },{ 4, TERM12, 0, TAXI15_block, 63 },{ 4, HELITAKEOFF, 0, 0, 123 },{ 4, 0, 0, 0, 63 },
	{ 5, 255, HANG02_block, 0, 0 },{ 5, HANGAR, 0, 0, 1 },{ 5, TERM21, 0, TAXI32_block | TAXI37_block, 93 },{ 5, TERM20, 0, TAXI32_block | TAXI33_block | TAXI36_block, 93 },{ 5, TERM19, 0, TAXI32_block | TAXI33_block | TAXI34_block | TAXI35_block, 93 },{ 5, TERM09, 0, TAXI18_block, 93 },{ 5, TERM10, 0, TAXI17_block, 93 },{ 5, TERM11, 0, TAXI16_block, 93 },{ 5, TERM12, 0, TAXI15_block, 93 },{ 5, HELITAKEOFF, 0, 0, 128 },{ 5, 0, 0, 0, 93 },
	{ 6, 255, HANG03_block, 0, 0 },{ 6, HANGAR, 0, 0, 2 },{ 6, HELITAKEOFF, 0, 0, 129 },{ 6, 0, 0, 0, 86 },
	{ 7, 255, HANG04_block, 0, 0 },{ 7, HANGAR, 0, 0, 3 },{ 7, HELITAKEOFF, 0, 0, 130 },{ 7, 0, 0, 0,100 },
	// Airplane Terminals
	{ 8, TERM01, TERM01_block, 0,  86 },{ 8, HANGAR, HANG03_block, TAXI25_block, 86 },{ 8, 0, 0, 0, 86 },
	{ 9, TERM02, TERM02_block, 0,  87 },{ 9, HANGAR, HANG03_block, TAXI26_block | TAXI25_block, 87 },{ 9, 0, 0, 0, 87 },
	{ 10, TERM03, TERM03_block, 0,  88 },{ 10, 0, 0, 0, 88 },
	{ 11, TERM04, TERM04_block, 0,  89 },{ 11, 0, 0, 0, 89 },
	{ 12, TERM05, TERM05_block, 0,  90 },{ 12, 0, 0, 0, 90 },
	{ 13, TERM06, TERM06_block, 0,  85 },{ 13, 0, 0, 0, 85 },
	{ 14, TERM07, TERM07_block, 0,  83 },{ 14, 0, 0, 0, 83 },
	{ 15, TERM08, TERM08_block, 0,  81 },{ 15, 0, 0, 0, 81 },
	{ 16, TERM09, TERM09_block, 0,  79 },{ 16, 0, 0, 0, 79 },
	{ 17, TERM10, TERM10_block, 0,  78 },{ 17, 0, 0, 0, 78 },
	{ 18, TERM11, TERM11_block, 0,  77 },{ 18, 0, 0, 0, 77 },
	{ 19, TERM12, TERM12_block, 0,  76 },{ 19, 0, 0, 0, 76 },
	{ 20, TERM13, TERM13_block, 0,  75 },{ 20, 0, 0, 0, 75 },
	{ 21, TERM14, TERM14_block, 0,  74 },{ 21, 0, 0, TAXI13_block | TAXI14_block, 74 },
	{ 22, TERM15, TERM15_block, 0,  73 },{ 22, 0, 0, TAXI12_block | TAXI15_block, 73 },
	{ 23, TERM16, TERM16_block, 0,  72 },{ 23, 0, 0, TAXI11_block | TAXI16_block, 72 },
	{ 24, TERM17, TERM17_block, 0,  71 },{ 24, 0, 0, TAXI10_block | TAXI17_block, 71 },
	{ 25, TERM18, TERM18_block, 0,  70 },{ 25, 0, 0, TAXI09_block | TAXI18_block, 70 },
	{ 26, TERM19, TERM19_block, 0,  68 },{ 26, 0, 0, 0, 68 },
	{ 27, TERM20, TERM20_block, 0,  97 },{ 27, 0, 0, 0, 97 },
	{ 28, TERM21, TERM21_block, 0,  99 },{ 28, 0, 0, 0, 99 },
	{ 29, TERM22, TERM22_block, 0, 104 },{ 29, 0, 0, 0,104 },
	{ 30, TERM23, TERM23_block, 0, 103 },{ 30, 0, 0, 0,103 },
	{ 31, TERM24, TERM24_block, 0, 102 },{ 31, 0, 0, 0,102 },
	{ 32, TERM25, TERM25_block, 0, 101 },{ 32, HANGAR, HANG04_block, TAXI40_block | TAXI39_block, 101 },{ 32, 0, 0, 0, 101 },
	{ 33, TERM26, TERM26_block, 0, 100 },{ 33, HANGAR, HANG04_block, TAXI39_block, 100 },{ 33, 0, 0, 0, 100 },
	// Cargo Terminals (Uses Helipads.)
	{ 34, CARG01, CARG01_block, 0,  94 },{ 34, HANGAR, 0, TAXI33_block | TAXI36_block, 94 },{ 34, TAKEOFF, 0, TAXI33_block | TAXI36_block, 94 },{ 34, 0, 0, 0, 94 },
	{ 35, CARG02, CARG02_block, 0,  95 },{ 35, HANGAR, 0, TAXI34_block | TAXI35_block, 95 },{ 35, TAKEOFF, 0, TAXI34_block | TAXI35_block, 95 },{ 35, 0, 0, 0, 95 },
	{ 36, CARG03, CARG03_block, 0,  67 },{ 36, 0, 0, 0, 67 },
	{ 37, CARG04, CARG04_block, 0,  66 },{ 37, HANGAR, 0, TAXI05_block | TAXI20_block, 66 },{ 37, TAKEOFF, 0, TAXI05_block | TAXI20_block, 66 },{ 37, 0, 0, 0, 66 },
	{ 38, CARG05, CARG05_block, 0,  65 },{ 38, HANGAR, 0, TAXI04_block | TAXI21_block, 65 },{ 38, TAKEOFF, 0, TAXI04_block | TAXI21_block, 65 },{ 38, 0, 0, 0, 65 },
	{ 39, CARG06, CARG06_block, 0,  64 },{ 39, HANGAR, 0, TAXI03_block | TAXI22_block, 64 },{ 39, TAKEOFF, 0, TAXI03_block | TAXI22_block, 64 },{ 39, 0, 0, 0, 64 },
	// Helicopter Terminals
	{ 40, HELI03, HELI03_block, 0,  56 },{ 40, HANGAR, 0, 0, 67 },{ 40, HELITAKEOFF, 0, 0, 124 },
	{ 41, HELI04, HELI04_block, 0,  57 },{ 41, HANGAR, 0, 0, 66 },{ 41, HELITAKEOFF, 0, 0, 125 },
	// Airplane In Air - Holding Pattern
	{ 42, 0, 0, 0,  43 },
	{ 43, 0, 0, 0,  48 },
	{ 44, 0, 0, 0,  45 },
	{ 45, 0, 0, 0,  46 },
	// Added Holding Pattern spots.
	{ 46, FLYING, 0, 0,  47 },{ 46, PRELANDING, 0, 0, 56 },{ 46, HELILANDING, 0, 0, 115 },{ 46, 0, 0, 0, 47 },
	{ 47, FLYING, 0, 0,  42 },{ 47, PRELANDING, 0, 0, 50 },{ 47, HELILANDING, 0, 0, 115 },{ 47, 0, 0, 0, 42 },
	{ 48, FLYING, 0, 0,  49 },{ 48, PRELANDING, 0, 0, 53 },{ 48, HELILANDING, 0, 0, 115 },{ 48, 0, 0, 0, 49 },
	{ 49, FLYING, 0, 0,  44 },{ 49, PRELANDING, 0, 0, 59 },{ 49, HELILANDING, 0, 0, 115 },{ 49, 0, 0, 0, 44 },
	// Airplane Landing Runway01
	{ 50, PRELANDING, RUNW01_block, 0, 42 },{ 50, LANDING, 0, 0, 51 },
	{ 51, LANDING, RUNW01_block, 0, 52 },{ 51, FLYING, 0, 0, 126 },{ 51, 0, 0, 0, 52 },
	{ 52, ENDLANDING, 0, TAXI45_block, 62 },{ 52, 0, 0, 0, 62 },{ 52, TERM06, 0, 0, 0 },{ 52, TERM08, 0, 0, 0 },{ 52, TERM19, 0, 0, 0 },{ 52, TERM10, 0, 0, 0 },{ 52, TERM17, 0, 0, 0 },{ 52, TERM12, 0, 0, 0 },{ 52, TERM15, 0, 0, 0 },{ 52, TERM07, 0, 0, 0 },{ 52, TERM18, 0, 0, 0 },{ 52, TERM09, 0, 0, 0 },{ 52, TERM16, 0, 0, 0 },{ 52, TERM11, 0, 0, 0 },{ 52, TERM14, 0, 0, 0 },{ 52, TERM13, 0, 0, 0 },{ 52, CARG05, 0, 0, 0 },{ 52, CARG06, 0, 0, 0 },{ 52, CARG04, 0, 0, 0 },
	// Airplane Landing Runway02
	{ 53, PRELANDING, RUNW02_block, TAXI01_block | TAXI24_block, 49 },{ 53, LANDING, 0, 0, 54 },
	{ 54, LANDING, RUNW02_block, 0, 55 },{ 54, FLYING, 0, 0, 109 },{ 54, 0, 0, 0, 55 },
	{ 55, ENDLANDING, 0, TAXI46_block,  6 },{ 55, 0, 0, 0,  6 },{ 55, TERM06, 0, 0, 0 },{ 55, TERM04, 0, 0, 0 },{ 55, TERM02, 0, 0, 0 },{ 55, TERM05, 0, 0, 0 },{ 55, TERM03, 0, 0, 0 },{ 55, TERM01, 0, 0, 0 },{ 55, CARG05, 0, 0, 0 },{ 55, CARG06, 0, 0, 0 },{ 55, CARG04, 0, 0, 0 },
	// Airplane Landing Runway03
	{ 56, PRELANDING, RUNW03_block, 0, 47 },{ 56, LANDING, 0, 0, 57 },
	{ 57, LANDING, RUNW03_block, 0, 58 },{ 57, FLYING, 0, 0, 127 },{ 57, 0, 0, 0, 58 },
	{ 58, ENDLANDING, 0, TAXI47_block, 92 },{ 58, 0, 0, 0, 92 },{ 58, TERM21, 0, 0, 0 },{ 58, TERM19, 0, 0, 0 },{ 58, TERM08, 0, 0, 0 },{ 58, TERM17, 0, 0, 0 },{ 58, TERM10, 0, 0, 0 },{ 58, TERM15, 0, 0, 0 },{ 58, TERM12, 0, 0, 0 },{ 58, TERM20, 0, 0, 0 },{ 58, TERM18, 0, 0, 0 },{ 58, TERM09, 0, 0, 0 },{ 58, TERM16, 0, 0, 0 },{ 58, TERM11, 0, 0, 0 },{ 58, TERM14, 0, 0, 0 },{ 58, TERM13, 0, 0, 0 },{ 58, CARG02, 0, 0, 0 },{ 58, CARG01, 0, 0, 0 },{ 58, CARG03, 0, 0, 0 },
	// Airplane Landing Runway04
	{ 59, PRELANDING, RUNW04_block, TAXI31_block | TAXI38_block, 44 },{ 59, LANDING, 0, 0, 60 },
	{ 60, LANDING, RUNW04_block, 0, 61 },{ 60, FLYING, 0, 0, 113 },{ 60, 0, 0, 0, 61 },
	{ 61, ENDLANDING, 0, TAXI48_block,  7 },{ 61, 0, 0, 0,  7 },{ 61, TERM21, 0, 0, 0 },{ 61, TERM23, 0, 0, 0 },{ 61, TERM25, 0, 0, 0 },{ 61, TERM22, 0, 0, 0 },{ 61, TERM24, 0, 0, 0 },{ 61, TERM26, 0, 0, 0 },{ 61, CARG02, 0, 0, 0 },{ 61, CARG01, 0, 0, 0 },{ 61, CARG03, 0, 0, 0 },
	// Airplane Taxi  (Main Airport flow)
	{ 62,    255, 0, TAXI01_block,  4 },{ 62, TERM06, HANG01_block, TAXI02_block | TAXI23_block, 4 },{ 62, TERM07, HANG01_block, TAXI02_block | TAXI03_block | TAXI22_block, 4 },{ 62, TERM08, HANG01_block, TAXI02_block | TAXI03_block | TAXI04_block | TAXI21_block, 4 },{ 62,      0, 0, TAXI01_block,  4 },
	{ 63,    255, 0, TAXI02_block,  0 },{ 63, TERM06, 0, 0, 84 },{ 63, 0, 0, 0, 64 },
	{ 64,    255, 0, TAXI03_block,  0 },{ 64, CARG06, 0, 0, 39 },{ 64, TERM07, 0, 0, 83 },{ 64, HANGAR, 0, 0, 83 },{ 64, TAKEOFF, 0, 0, 83 },{ 64, 0, 0, 0, 65 },
	{ 65,    255, 0, TAXI04_block,  0 },{ 65, CARG05, 0, 0, 38 },{ 65, TERM08, 0, 0, 82 },{ 65, HANGAR, 0, 0, 82 },{ 65, TAKEOFF, 0, 0, 82 },{ 65, 0, 0, 0, 66 },
	{ 66,    255, 0, TAXI05_block,  0 },{ 66, CARG04, 0, 0, 37 },{ 66, HELI04, 0, 0, 41 },{ 66, HANGAR, 0, 0, 81 },{ 66, TAKEOFF, 0, 0, 81 },{ 66, HELITAKEOFF, 0, 0, 41 },{ 66, 0, 0, 0, 67 },
	{ 67,    255, 0, TAXI06_block,  0 },{ 67, CARG03, 0, 0, 36 },{ 67, HELI03, 0, 0, 40 },{ 67, HELITAKEOFF, 0, 0, 40 },{ 67, 0, 0, 0, 68 },
	{ 68, BACKUP, 0, TAXI07_block,  0 },{ 68, TERM19, 0, 0, 26 },{ 68, HANGAR, 0, 0, 96 },{ 68, TAKEOFF, 0, 0, 96 },{ 68, 0, 0, 0, 69 },
	{ 69,    255, 0, TAXI08_block,  0 },{ 69, TERM08, 0, 0, 80 },{ 69, 0, 0, 0, 70 },
	{ 70, BACKUP, 0, TAXI09_block,  0 },{ 70, TERM18, 0, 0, 25 },{ 70, TERM09, 0, 0, 79 },{ 70, HANGAR, 0, 0, 79 },{ 70, TAKEOFF, 0, 0, 79 },{ 70, 0, 0, 0, 71 },
	{ 71, BACKUP, 0, TAXI10_block,  0 },{ 71, TERM17, 0, 0, 24 },{ 71, TERM10, 0, 0, 78 },{ 71, HANGAR, 0, 0, 78 },{ 71, TAKEOFF, 0, 0, 78 },{ 71, 0, 0, 0, 72 },
	{ 72, BACKUP, 0, TAXI11_block,  0 },{ 72, TERM16, 0, 0, 23 },{ 72, TERM11, 0, 0, 77 },{ 72, HANGAR, 0, 0, 77 },{ 72, TAKEOFF, 0, 0, 77 },{ 72, 0, 0, 0, 73 },
	{ 73, BACKUP, 0, TAXI12_block,  0 },{ 73, TERM15, 0, 0, 22 },{ 73, TERM12, 0, 0, 76 },{ 73, HANGAR, 0, 0, 76 },{ 73, TAKEOFF, 0, 0, 76 },{ 73, 0, 0, 0, 74 },
	{ 74, BACKUP, 0, TAXI13_block,  0 },{ 74, TERM14, 0, 0, 21 },{ 74, 0, 0, 0, 75 },
	{ 75, BACKUP, 0, TAXI14_block,  0 },{ 75, TERM13, 0, 0, 20 },{ 75, 0, 0, 0, 76 },
	{ 76, BACKUP, 0, TAXI15_block,  0 },{ 76, TERM12, 0, 0, 19 },{ 76, 0, 0, 0, 77 },
	{ 77, BACKUP, 0, TAXI16_block,  0 },{ 77, TERM11, 0, 0, 18 },{ 77, 0, 0, 0, 78 },
	{ 78, BACKUP, 0, TAXI17_block,  0 },{ 78, TERM10, 0, 0, 17 },{ 78, 0, 0, 0, 79 },
	{ 79, BACKUP, 0, TAXI18_block,  0 },{ 79, TERM09, 0, 0, 16 },{ 79, 0, 0, 0, 80 },
	{ 80,    255, 0, TAXI19_block,  0 },{ 80, TERM08, 0, 0, 15 },{ 80, 0, 0, 0, 81 },
	{ 81, BACKUP, 0, TAXI20_block,  0 },{ 81, 0, 0, 0, 82 },
	{ 82,    255, 0, TAXI21_block,  0 },{ 82, TERM08, 0, 0, 15 },{ 82, 0, 0, 0, 83 },
	{ 83, BACKUP, 0, TAXI22_block,  0 },{ 83, TERM07, 0, 0, 14 },{ 83, 0, 0, 0, 84 },
	{ 84,    255, 0, TAXI23_block,  0 },{ 84, TERM06, 0, 0, 13 },{ 84, 0, 0, 0, 85 },
	{ 85, BACKUP, 0, TAXI24_block,  0 },{ 85, HANGAR, 0, 0,  4 },{ 85, CARG04, 0, 0,  4 },{ 85, CARG05, 0, 0,  4 },{ 85, CARG06, 0, 0,  4 },{ 85, HELI03, 0, 0, 4 },{ 85, HELI04, 0, 0,  4 },{ 85, 0, 0, 0,106 },
	{ 86, BACKUP, 0, TAXI25_block,  0 },{ 86, TERM01, 0, 0,  8 },{ 86, HANGAR, 0, 0,  6 },{ 86, 0, 0, 0, 87 },
	{ 87, BACKUP, 0, TAXI26_block,  0 },{ 87, TERM02, 0, 0,  9 },{ 87, HANGAR, 0, 0, 86 },{ 87, 0, 0, 0, 88 },
	{ 88, BACKUP, 0, TAXI27_block,  0 },{ 88, TERM03, 0, 0, 10 },{ 88, 0, 0, 0, 89 },
	{ 89, BACKUP, 0, TAXI28_block,  0 },{ 89, TERM04, 0, 0, 11 },{ 89, 0, 0, 0, 90 },
	{ 90, BACKUP, 0, TAXI29_block,  0 },{ 90, TERM05, 0, 0, 12 },{ 90, 0, 0, 0, 91 },
	{ 91,    255, 0, TAXI30_block,  0 },{ 91, TERM06, 0, 0, 13 },{ 91, 0, 0, 0, 85 },
	{ 92,    255, 0, TAXI31_block,  5 },{ 92, TERM21, HANG02_block, TAXI32_block | TAXI37_block, 5 },{ 92, TERM20, HANG02_block, TAXI32_block | TAXI33_block | TAXI36_block, 5 },{ 92, TERM19, HANG02_block, TAXI32_block | TAXI33_block | TAXI34_block | TAXI35_block, 5 }, { 92,      0, 0, TAXI31_block,  5 },
	{ 93,    255, 0, TAXI32_block,  0 },{ 93, TERM21, 0, 0, 98 },{ 93, 0, 0, 0, 94 },
	{ 94,    255, 0, TAXI33_block,  0 },{ 94, CARG01, 0, 0, 34 },{ 94, TERM20, 0, 0, 97 },{ 94, HANGAR, 0, 0, 97 },{ 94, TAKEOFF, 0, 0, 97 },{ 94, 0, 0, 0, 95 },
	{ 95,    255, 0, TAXI34_block,  0 },{ 95, CARG02, 0, 0, 35 },{ 95, TERM19, 0, 0, 96 },{ 95, HANGAR, 0, 0, 96 },{ 95, TAKEOFF, 0, 0, 96 },{ 95, 0, 0, 0, 67 },
	{ 96,    255, 0, TAXI35_block,  0 },{ 96, TERM19, 0, 0, 26 },{ 96, 0, 0, 0, 97 },
	{ 97, BACKUP, 0, TAXI36_block,  0 },{ 97, TERM20, 0, 0, 27 },{ 97, 0, 0, 0, 98 },
	{ 98,    255, 0, TAXI37_block,  0 },{ 98, TERM21, 0, 0, 28 },{ 98, 0, 0, 0, 99 },
	{ 99, BACKUP, 0, TAXI38_block,  0 },{ 99, HANGAR, 0, 0,  5 },{ 99, CARG01, 0, 0,  5 },{ 99, CARG02, 0, 0,  5 },{ 99, CARG03, 0, 0,  5 },{ 99, HELI03, 0, 0,  5 },{ 99, HELI04, 0, 0, 5 },{ 99, 0, 0, 0,110 },
	{ 100, BACKUP, 0, TAXI39_block,  0 },{ 100, TERM26, 0, 0, 33 },{ 100, HANGAR, 0, 0,  7 },{ 100, 0, 0, 0,101 },
	{ 101, BACKUP, 0, TAXI40_block,  0 },{ 101, TERM25, 0, 0, 32 },{ 101, HANGAR, 0, 0,100 },{ 101, 0, 0, 0,102 },
	{ 102, BACKUP, 0, TAXI41_block,  0 },{ 102, TERM24, 0, 0, 31 },{ 102, 0, 0, 0,103 },
	{ 103, BACKUP, 0, TAXI42_block,  0 },{ 103, TERM23, 0, 0, 30 },{ 103, 0, 0, 0,104 },
	{ 104, BACKUP, 0, TAXI43_block,  0 },{ 104, TERM22, 0, 0, 29 },{ 104, 0, 0, 0,105 },
	{ 105,    255, 0, TAXI44_block,  0 },{ 105, TERM21, 0, 0, 28 },{ 105, 0, 0, 0, 99 },
	// Airplane Takeoff - Runway02
	{ 106, TAKEOFF, RUNW02_block, 0, 107 },
	{ 107, 0, 0, 0, 108 },
	{ 108, STARTTAKEOFF, 0, 0, 109 },
	{ 109, ENDTAKEOFF, 0, 0, 0 },{ 109, 0, 0, 0, 42 },   // End Airport control of airplane.
	// Airplane Takeoff - Runway 04
	{ 110, TAKEOFF, RUNW04_block, 0, 111 },
	{ 111, 0, 0, 0, 112 },
	{ 112, STARTTAKEOFF, 0, 0, 113 },
	{ 113, ENDTAKEOFF, 0, 0, 0 },{ 113, 0, 0, 0, 42 },   // End Airport control of airplane.
	// Helicopter -- Entry .  Keeps it from messing up the planes.
	{ 114, FLYING, 0, 0, 114 },{ 114, HELILANDING, 0, 0, 115 },
	// Helicopter -- Holding Pattern
	{ 115, HELILANDING, 0, 0, 116 },{ 115, 0, 0, 0, 116 },
	{ 116, HELIENDLANDING, 0, 0, 42 },{ 116, HELI03, 0, 0, 117 },{ 116, HELI04, 0, 0, 118 },{ 116, HANGAR, 0, 0, 121 },
	// Helicopter Positioning
	{ 117, 0, 0, 0, 119 },
	{ 118, 0, 0, 0, 120 },
	//  Helicopter landing
	{ 119,   0, 0, 0, 40 },
	{ 120,   0, 0, 0, 41 },
	//Helicopter -- Hangar
	{ 121, 0, HANG01_block, 0, 122 }, // need to go to hangar when waiting in air
	{ 122, 0, 0, 0,   4 },
	// Helicopter -- takeoff
	{ 123, HELITAKEOFF, 0, 0, 0 },  // Hangar01
	{ 124, HELITAKEOFF, 0, 0, 0 },  // Helipad03
	{ 125, HELITAKEOFF, 0, 0, 0 },  // Helipad04
	// Aborted landings for Runway 1 and Runway 3
	{ 126, 0, 0, 0, 44 },
	{ 127, 0, 0, 0, 44 },
		// Helicopter -- takeoff
	{ 128, HELITAKEOFF, 0, 0, 0 },  // Hangar02
	{ 129, HELITAKEOFF, 0, 0, 0 },  // Hangar03
	{ 130, HELITAKEOFF, 0, 0, 0 },  // Hangar04
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/// Schiphol
static const HangarTileTable _airport_depots_schiphol[] = { { { 3, 4 }, DIR_SW, 0 },{ { 11, 12 }, DIR_NW, 1 },{ { 5,15 }, DIR_NW, 2 } };
static const TerminalTileTable _airport_terminals_schiphol[] = { { { 6, 6 }, TERM01_block },{ { 6, 7 }, TERM02_block },{ { 6, 8 }, TERM03_block },{ { 6, 9 }, TERM04_block },{ { 7, 9 }, TERM05_block },{ { 8, 9 }, TERM06_block },
{ { 9, 9 }, TERM07_block },{ { 10, 9 }, TERM08_block },{ { 10, 8 }, TERM09_block },{ { 10, 7 }, TERM10_block },{ { 10, 6 }, TERM11_block },{ { 10, 5 }, TERM12_block },
{ { 6,15 }, CARG01_block },{ { 7,15 }, CARG02_block },{ { 8,15 }, CARG03_block },{ { 9,15 }, CARG04_block } };
static const RunwayTerminalTable _airport_runways_schiphol[] = { { TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | TERM07_block | TERM08_block | TERM09_block | TERM10_block | TERM11_block | TERM12_block | CARG01_block | CARG02_block | CARG03_block | CARG04_block, RUNW02_block, 0 },
{ TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | TERM07_block | TERM08_block | TERM09_block | TERM10_block | TERM11_block | TERM12_block | CARG01_block | CARG02_block | CARG03_block | CARG04_block, RUNW03_block, 0 },
{ TERM01_block | TERM02_block | TERM03_block | TERM04_block | TERM05_block | TERM06_block | TERM07_block | TERM08_block | TERM09_block | TERM10_block | TERM11_block | TERM12_block | CARG01_block | CARG02_block | CARG03_block | CARG04_block, RUNW05_block, 0 } };
static const byte _airport_terminal_schiphol[] = { 1, 12 };
static const byte _airport_entries_schiphol[] = { 25, 24, 27, 26 };
static const byte _airport_helientry_schiphol[] = { 25, 24, 27, 26 };
static const AirportFTAbuildup _airport_fta_schiphol[] = {
	// Airplane & Helicopter Hangars
	{ 0, HANGAR, 0, 0, 3 },{ 0, 0, 0, 0, 3 },{ 0, TERM12, 0, 0, 3 },{ 0, TERM10, 0, 0, 3 },{ 0, TERM08, 0, 0, 3 },{ 0, TERM06, 0, 0, 3 },{ 0, TERM04, 0, 0, 3 },{ 0, TERM02, 0, 0, 3 },{ 0, TERM11, 0, 0, 3 },{ 0, TERM09, 0, 0, 3 },{ 0, TERM07, 0, 0, 3 },{ 0, TERM05, 0, 0, 3 },{ 0, TERM03, 0, 0, 3 },{ 0, TERM01, 0, 0, 3 },{ 0, CARG04, 0, 0, 3 },{ 0, CARG02, 0, 0, 3 },{ 0, CARG03, 0, 0, 3 },{ 0, CARG01, 0, 0, 3 },{ 0, HELI02, 0, 0, 3 },{ 0, HELI01, 0, 0, 3 },
	{ 1, HANGAR, 0, 0, 4 },{ 1, 0, 0, 0, 4 },{ 1, TERM05, 0, 0, 4 },{ 1, TERM06, 0, 0, 4 },{ 1, TERM07, 0, 0, 4 },{ 1, TERM08, 0, 0, 4 },{ 1, TERM12, 0, 0, 4 },{ 1, TERM10, 0, 0, 4 },{ 1, TERM11, 0, 0, 4 },{ 1, TERM09, 0, 0, 4 },
	{ 2, HANGAR, 0, 0, 5 },{ 2, 0, 0, 0, 5 },{ 2, CARG04, 0, 0, 5 },{ 2, CARG02, 0, 0, 5 },{ 2, CARG03, 0, 0, 5 },{ 2, CARG01, 0, 0, 5 },{ 2, HELI02, 0, 0, 5 },{ 2, HELI01, 0, 0, 5 },
	{ 3, 255, HANG01_block, 0, 0 },{ 3, HANGAR, 0, 0, 0 },{ 3, HELITAKEOFF, HANG03_block, 0, 49 }, { 3, 0, 0, 0, 49 },
	{ 4, 255, HANG02_block, 0, 0 },{ 4, HANGAR, 0, 0, 1 },{ 4, HELITAKEOFF, 0, 0, 103 },{ 4, TERM05, 0, TAXI19_block | TAXI20_block | TAXI21_block | TAXI22_block | TAXI23_block, 61 },{ 4, TERM06, 0, TAXI20_block | TAXI21_block | TAXI22_block | TAXI23_block, 61 },{ 4, TERM07, 0, TAXI21_block | TAXI22_block | TAXI23_block, 61 },{ 4, 0, 0, 0, 61 },
	{ 5, 255, HANG03_block, 0, 0 },{ 5, HANGAR, 0, 0, 2 },{ 5, HELITAKEOFF, 0, 0, 104 },{ 5, 0, 0, 0, 71 },
	// Airplane Terminals
	{ 6, TERM01, TERM01_block, 0,  51 },{ 6, HANGAR, HANG01_block, TAXI13_block | TAXI12_block | TAXI10_block, 51 },{ 6, 0, 0, 0, 51 },
	{ 7, TERM02, TERM02_block, 0,  52 },{ 7, HANGAR, HANG01_block, TAXI14_block | TAXI13_block | TAXI12_block | TAXI10_block, 52 },{ 7, 0, 0, 0, 52 },
	{ 8, TERM03, TERM03_block, 0,  53 },{ 8, HANGAR, HANG01_block, TAXI15_block | TAXI14_block | TAXI13_block | TAXI12_block | TAXI10_block, 53 },{ 8, 0, 0, 0, 53 },
	{ 9, TERM04, TERM04_block, 0,  55 },{ 9, 0, 0, 0, 55 },
	{ 10, TERM05, TERM05_block, 0,  57 },{ 10, 0, 0, 0, 57 },
	{ 11, TERM06, TERM06_block, 0,  58 },{ 11, 0, 0, 0, 58 },
	{ 12, TERM07, TERM07_block, 0,  59 },{ 12, 0, 0, 0, 59 },
	{ 13, TERM08, TERM08_block, 0,  61 },{ 13, HANGAR, HANG02_block, TAXI23_block, 61 },{ 13, 0, 0, 0, 61 },
	{ 14, TERM09, TERM09_block, 0,  63 },{ 14, HANGAR, HANG02_block, TAXI25_block | TAXI24_block | TAXI23_block, 63 },{ 14, 0, 0, 0, 63 },
	{ 15, TERM10, TERM10_block, 0,  64 },{ 15, HANGAR, HANG02_block, TAXI26_block | TAXI25_block | TAXI24_block | TAXI23_block, 64 },{ 15, 0, 0, 0, 64 },
	{ 16, TERM11, TERM11_block, 0,  65 },{ 16, HANGAR, HANG02_block, TAXI27_block | TAXI26_block | TAXI25_block | TAXI24_block | TAXI23_block, 65 },{ 16, 0, 0, 0, 65 },
	{ 17, TERM12, TERM12_block, 0,  66 },{ 17, HANGAR, HANG02_block, TAXI28_block | TAXI27_block | TAXI26_block | TAXI25_block | TAXI24_block | TAXI23_block, 66 },{ 17, 0, 0, 0, 66 },
	// Cargo Terminals (Uses Helipads.)
	{ 18, CARG01, CARG01_block, 0,  71 },{ 18, HANGAR, HANG03_block, TAXI33_block, 71 },{ 18, 0, 0, 0, 71 },
	{ 19, CARG02, CARG02_block, 0,  72 },{ 19, HANGAR, HANG03_block, TAXI34_block | TAXI33_block, 72 },{ 19, 0, 0, 0, 72 },
	{ 20, CARG03, CARG03_block, 0,  73 },{ 20, HANGAR, HANG03_block, TAXI35_block | TAXI34_block | TAXI33_block, 73 },{ 20, 0, 0, 0, 73 },
	{ 21, CARG04, CARG04_block, 0,  74 },{ 21, HANGAR, HANG03_block, TAXI36_block | TAXI35_block | TAXI34_block | TAXI33_block, 74 },{ 21, 0, 0, 0, 74 },
	// Helicopter Terminals
	{ 22, HELI01, HELI01_block, 0,  71 },{ 22, HANGAR, HANG03_block, TAXI33_block, 71 },{ 22, HELITAKEOFF, 0, 0, 105 },
	{ 23, HELI02, HELI02_block, 0,  72 },{ 23, HANGAR, HANG03_block, TAXI34_block | TAXI33_block, 72 },{ 23, HELITAKEOFF, 0, 0, 106 },
	// Airplane In Air - Holding Pattern
	{ 24, 0, 0, 0,  29 },
	{ 25, 0, 0, 0,  30 },
	{ 26, 0, 0, 0,  27 },
	{ 27, 0, 0, 0,  28 },
	// Added Holding Pattern spots.
	{ 28, FLYING, 0, 0,  24 },{ 28, PRELANDING, RUNW03_block, TAXI05_block, 35 },{ 28, HELILANDING, 0, 0,  95 },{ 28, 0, 0, 0, 24 },
	{ 29, FLYING, 0, 0,  25 },{ 29, PRELANDING, RUNW01_block | RUNW02_block | RUNW06_block, TAXI11_block, 32 },{ 29, HELILANDING, 0, 0,  95 },{ 29, 0, 0, 0, 25 },
	{ 30, FLYING, 0, 0,  26 },{ 30, PRELANDING, RUNW05_block, TAXI01_block, 38 },{ 30, HELILANDING, 0, 0,  95 },{ 30, 0, 0, 0, 26 },
	{ 31, FLYING, 0, 0,  24 },{ 31, PRELANDING, 0, 0, 35 },{ 31, HELILANDING, 0, 0,  95 },{ 31, 0, 0, 0, 24 },
	// Airplane Landing Runway02
	{ 32, PRELANDING, RUNW01_block | RUNW02_block | RUNW06_block, 0, 33 },{ 32, LANDING, 0, 0, 33 },
	{ 33, LANDING, RUNW02_block, 0, 34 },{ 33, FLYING, 0, 0, 91 },{ 33, 0, 0, 0, 34 },
	{ 34, ENDLANDING, 0, TAXI11_block, 50 },{ 34, HANGAR, 0, 0,  3 },{ 34, 0, 0, 0, 50 },{ 34, TERM12, 0, 0, 0 },{ 34, TERM10, 0, 0, 0 },{ 34, TERM08, 0, 0, 0 },{ 34, TERM06, 0, 0, 0 },{ 34, TERM04, 0, 0, 0 },{ 34, TERM02, 0, 0, 0 },{ 34, TERM11, 0, 0, 0 },{ 34, TERM09, 0, 0, 0 },{ 34, TERM07, 0, 0, 0 },{ 34, TERM05, 0, 0, 0 },{ 34, TERM03, 0, 0, 0 },{ 34, TERM01, 0, 0, 0 },{ 34, CARG04, 0, 0, 0 },{ 34, CARG03, 0, 0, 0 },{ 34, CARG02, 0, 0, 0 },{ 34, CARG01, 0, 0, 0 },
	// Airplane Landing Runway03
	{ 35, PRELANDING, RUNW03_block, 0, 36 },{ 35, LANDING, 0, 0, 36 },
	{ 36, LANDING, RUNW03_block, 0, 37 },{ 36, FLYING, 0, 0, 92 },{ 36, 0, 0, 0, 37 },
	{ 37, ENDLANDING, 0, TAXI05_block, 45 },{ 37, 0, 0, 0, 45 },{ 37, TERM12, 0, 0, 0 },{ 37, TERM10, 0, 0, 0 },{ 37, TERM08, 0, 0, 0 },{ 37, TERM06, 0, 0, 0 },{ 37, TERM04, 0, 0, 0 },{ 37, TERM02, 0, 0, 0 },{ 37, TERM11, 0, 0, 0 },{ 37, TERM09, 0, 0, 0 },{ 37, TERM07, 0, 0, 0 },{ 37, TERM05, 0, 0, 0 },{ 37, TERM03, 0, 0, 0 },{ 37, TERM01, 0, 0, 0 },{ 37, CARG04, 0, 0, 0 },{ 37, CARG03, 0, 0, 0 },{ 37, CARG02, 0, 0, 0 },{ 37, CARG01, 0, 0, 0 },
	// Airplane Landing Runway05
	{ 38,  PRELANDING, RUNW05_block, 0, 39 },{ 38, LANDING, 0, 0, 39 },
	{ 39, LANDING, RUNW05_block, 0, 40 },{ 39, FLYING, 0, 0, 93 },{ 39, 0, 0, 0, 40 },
	{ 40, ENDLANDING, 0, TAXI01_block, 41 },{ 40, 0, 0, 0, 41 },{ 40, TERM12, 0, 0, 0 },{ 40, TERM10, 0, 0, 0 },{ 40, TERM08, 0, 0, 0 },{ 40, TERM06, 0, 0, 0 },{ 40, TERM04, 0, 0, 0 },{ 40, TERM02, 0, 0, 0 },{ 40, TERM11, 0, 0, 0 },{ 40, TERM09, 0, 0, 0 },{ 40, TERM07, 0, 0, 0 },{ 40, TERM05, 0, 0, 0 },{ 40, TERM03, 0, 0, 0 },{ 40, TERM01, 0, 0, 0 },{ 40, CARG04, 0, 0, 0 },{ 40, CARG03, 0, 0, 0 },{ 40, CARG02, 0, 0, 0 },{ 40, CARG01, 0, 0, 0 },
	// Airplane Taxi  (Main Airport flow)
	{ 41,      0, 0, TAXI02_block, 42 },
	{ 42,      0, 0, TAXI03_block, 43 },
	{ 43,      0, 0, TAXI04_block, 44 },
	{ 44,      0, 0, TAXI05_block, 48 },
	{ 45,      0, 0, TAXI06_block, 46 },
	{ 46,      0, 0, TAXI07_block, 47 },
	{ 47,    255, 0, TAXI08_block,  0 },{ 47, HANGAR, 0, 0,  3 },{ 47, 0, 0, 0, 48 },
	{ 48,    255, 0, TAXI09_block,  0 },{ 48, HANGAR, HANG01_block, TAXI10_block, 49 },{ 48, 0, 0, 0, 49 },
	{ 49,    255, 0, TAXI10_block,  0 },{ 49, HANGAR, 0, 0,  3 },{ 49, 0, 0, 0, 50 },
	{ 50,    255, 0, TAXI12_block,  0 },{ 50, HANGAR, 0, 0, 49 },{ 50, 0, 0, 0, 51 },
	{ 51, BACKUP, 0, TAXI13_block,  0 },{ 51, TERM01, 0, 0,  6 },{ 51, HANGAR, 0, 0, 50 },{ 51, 0, 0, 0, 52 },
	{ 52, BACKUP, 0, TAXI14_block,  0 },{ 52, TERM02, 0, 0,  7 },{ 52, HANGAR, 0, 0, 51 },{ 52, 0, 0, 0, 53 },
	{ 53, BACKUP, 0, TAXI15_block,  0 },{ 53, TERM03, 0, 0,  8 },{ 53, HANGAR, 0, 0, 52 },{ 53, 0, 0, 0, 54 },
	{ 54,    255, 0, TAXI16_block,  0 },{ 54, TERM04, 0, 0,  9 },{ 54, 0, 0, 0, 55 },
	{ 55, BACKUP, 0, TAXI17_block,  0 },{ 55, CARG01, 0, 0, 69 },{ 55, CARG02, 0, 0, 69 },{ 55, CARG03, 0, 0, 69 },{ 55, CARG04, 0, 0, 69 },{ 55, HELI01, 0, 0, 69 },{ 55, HELI02, 0, 0, 69 },{ 55, HELITAKEOFF, 0, 0, 69 }, { 55, 0, 0, 0, 56 },
	{ 56,      0, 0, TAXI18_block, 57 },
	{ 57, BACKUP, 0, TAXI19_block,  0 },{ 57, TERM05, 0, 0, 10 },{ 57, 0, 0, 0, 58 },
	{ 58, BACKUP, 0, TAXI20_block,  0 },{ 58, TERM06, 0, 0, 11 },{ 58, TERM05, 0, 0, 57 },{ 58, 0, 0, 0, 59 },
	{ 59, BACKUP, 0, TAXI21_block,  0 },{ 59, TERM07, 0, 0, 12 },{ 59, TERM05, 0, 0, 58 },{ 59, TERM06, 0, 0, 58 },{ 59, 0, 0, 0, 60 },
	{ 60,    255, 0, TAXI22_block,  0 },{ 60, TERM08, 0, 0, 13 },{ 60, TAKEOFF, 0, 0, 77 },{ 60, TERM05, 0, 0, 59 },{ 60, TERM06, 0, 0, 59 },{ 60, TERM07, 0, 0, 59 },{ 60, HANGAR, HANG02_block, TAXI23_block, 61 },{ 60, 0, 0, 0, 61 },
	{ 61, BACKUP, 0, TAXI23_block,  0 },{ 61, HANGAR, 0, 0,  4 },{ 61, TERM05, 0, 0, 60 },{ 61, TERM06, 0, 0, 60 },{ 61, TERM07, 0, 0, 60 },{ 61, 0, 0, 0, 62 },
	{ 62,    255, 0, TAXI24_block,  0 },{ 62, TERM08, 0, 0, 13 },{ 62, HANGAR, 0, 0, 61 },{ 62, 0, 0, 0, 63 },
	{ 63, BACKUP, 0, TAXI25_block,  0 },{ 63, TERM09, 0, 0, 14 },{ 63, HANGAR, 0, 0, 62 },{ 63, 0, 0, 0, 64 },
	{ 64, BACKUP, 0, TAXI26_block,  0 },{ 64, TERM10, 0, 0, 15 },{ 64, HANGAR, 0, 0, 63 },{ 64, 0, 0, 0, 65 },
	{ 65, BACKUP, 0, TAXI27_block,  0 },{ 65, TERM11, 0, 0, 16 },{ 65, HANGAR, 0, 0, 64 },{ 65, 0, 0, 0, 66 },
	{ 66, BACKUP, 0, TAXI28_block,  0 },{ 66, TERM12, 0, 0, 17 },{ 66, HANGAR, 0, 0, 65 },{ 66, 0, 0, 0, 67 },
	{ 67,      0, 0, TAXI29_block, 68 },
	{ 68,      0, 0, TAXI30_block, 82 },
	{ 69,      0, RUNW01_block, 0, 70 },
	{ 70,      0, 0, TAXI32_block, 76 },
	{ 71,    255, 0, TAXI33_block,  0 },{ 71, CARG01, 0, 0, 18 },{ 71, HELI01, 0, 0, 22 },{ 71, HANGAR, 0, 0,  5 },{ 71, 0, 0, 0, 72 },
	{ 72,    255, 0, TAXI34_block,  0 },{ 72, CARG02, 0, 0, 19 },{ 72, HELI02, 0, 0, 23 },{ 72, HANGAR, 0, 0, 71 },{ 72, 0, 0, 0, 73 },
	{ 73,    255, 0, TAXI35_block,  0 },{ 73, CARG03, 0, 0, 20 },{ 73, HANGAR, 0, 0, 72 },{ 73, 0, 0, 0, 74 },
	{ 74,    255, 0, TAXI36_block,  0 },{ 74, CARG04, 0, 0, 21 },{ 74, HANGAR, 0, 0, 73 },{ 74, 0, 0, 0, 75 },
	{ 75,      0, 0, TAXI37_block, 86 },
	{ 76,      0, RUNW06_block, 0,  5 },
	// Airplane Takeoff - Runway01
	{ 77,      0, 0, TAXI31_block, 78 },
	{ 78, TAKEOFF, RUNW01_block | RUNW02_block, 0, 80 },
	{ 79, 0, 0, 0, 80 }, // Unused
	{ 80, STARTTAKEOFF, 0, 0, 81 },
	{ 81, ENDTAKEOFF, 0, 0, 0 },   // End Airport control of airplane.
	// Airplane Takeoff - Runway 04
	{ 82, TAKEOFF, RUNW04_block, 0, 83 },
	{ 83, 0, 0, 0, 84 },
	{ 84, STARTTAKEOFF, 0, 0, 85 },
	{ 85, ENDTAKEOFF, 0, 0, 0 },   // End Airport control of airplane.
	// Airplane Takeoff - Runway 06
	{ 86,      0, 0, TAXI38_block, 87 },
	{ 87, TAKEOFF, RUNW06_block | RUNW02_block, 0, 89 },
	{ 88, 0, 0, 0, 89 },  // Unused
	{ 89, STARTTAKEOFF, 0, 0, 90 },
	{ 90, ENDTAKEOFF, 0, 0, 0 },   // End Airport control of airplane.
	// Aborted landings for Runway 2, 3, 5
	{ 91, 0, 0, 0, 26 },
	{ 92, 0, 0, 0, 25 },
	{ 93, 0, 0, 0, 27 },
	// Helicopter -- Entry .  Keeps it from messing up the planes.
	{ 94, FLYING, 0, 0,  94 },{ 94, HELILANDING, 0, 0,  95 },
	// Helicopter -- Holding Pattern
	{ 95, HELILANDING, 0, 0,  96 },{ 95, 0, 0, 0,  96 },
	{ 96, HELIENDLANDING, 0, 0, 96 },{ 96, HELI02, 0, 0,  98 },{ 96, HELI01, 0, 0,  97 },{ 96, HANGAR, 0, 0, 101 },
	// Helicopter Positioning
	{ 97, 0, 0, 0,  99 },
	{ 98, 0, 0, 0, 100 },
	//  Helicopter landing
	{ 99,   0, 0, 0, 22 },
	{ 100,   0, 0, 0, 23 },
	//Helicopter -- Hangar
	{ 101, 0, HANG03_block, 0, 102 }, // need to go to hangar when waiting in air
	{ 102, 0, 0, 0,   5 },
	// Helicopter -- takeoff
	{ 103, HELITAKEOFF, 0, 0, 0 },  // Hangar02
	{ 104, HELITAKEOFF, 0, 0, 0 },  // Hangar03
	{ 105, HELITAKEOFF, 0, 0, 0 },  // Helipad01
	{ 106, HELITAKEOFF, 0, 0, 0 },  // Helipad02
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};


/* heliports, oilrigs don't have depots */
static const byte _airport_entries_heliport[] = { 7, 7, 7, 7 };
static const TerminalTileTable _airport_terminals_heliport[] = { { { 0, 0 }, HELI01_block } };
static const RunwayTerminalTable _airport_runways_heliport[] = { { 0, 0, 0 } };      // 0 = no runways.
static const byte _airport_helientry_heliport[] = { 7, 7, 7, 7 };
static const AirportFTAbuildup _airport_fta_heliport[] = {
	{ 0, HELI01, HELI01_block, 0, 1 },{ 0, HELITAKEOFF, 0, 0, 1 },{ 0, 0, 0, 0, 1 },
	{ 1, HELITAKEOFF, 0, 0, 0 }, // takeoff
	{ 2, 255, 0, 0, 0 },{ 2, HELILANDING, 0, 0, 3 },{ 2, HELITAKEOFF, 0, 0, 1 },
	{ 3, HELILANDING, HELI01_block, 0, 4 },
	{ 4, HELIENDLANDING, 0, 0, 4 }, { 4, 0, 0, 0, 0}, { 4, HELI01, 0, 0, 0 },
	/* In Air */
	{ 5, 0, 0, 0, 6 },
	{ 6, 0, 0, 0, 7 },
	{ 7, 0, 0, 0, 8 },
	{ 8, FLYING, 0, 0, 5 },{ 8, HELILANDING, 0, 0, 2 }, { 8, 0, 0, 0, 5 },// landing
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};
#define _airport_entries_oilrig _airport_entries_heliport
#define _airport_terminals_oilrig _airport_terminals_heliport
#define _airport_fta_oilrig _airport_fta_heliport
#define _airport_helientry_oilrig _airport_helientry_heliport

/* helidepots */
static const HangarTileTable _airport_depots_helidepot[] = { { { 1, 0 }, DIR_SE, 0 } };
static const TerminalTileTable _airport_terminals_helidepot[] = { { { 0, 1 }, HELI01_block } };
static const RunwayTerminalTable _airport_runways_helidepot[] = { { 0, 0, 0 } };      // 0 = no runways.
static const byte _airport_entries_helidepot[] = { 4, 4, 4, 4 };
static const byte _airport_helientry_helidepot[] = { 4, 4, 4, 4 };
static const AirportFTAbuildup _airport_fta_helidepot[] = {
	{ 0, HANGAR, 0, 0, 1 },{ 0, 0, 0, 0, 1 },{ 0, HELI01, 0, 0, 14 },{ 0, HELITAKEOFF, 0, 0, 15 },
	{ 1, 255, HANG02_block, 0, 0 },{ 1, HANGAR, 0, 0, 0 },{ 1, HELI01, 0, 0, 2 },{ 1, HELITAKEOFF, 0, 0, 15 },{ 1, 0, 0, 0, 0 },
	// Helipad Terminal
	{ 2, HELI01, HELI01_block, 0, 12 },{ 2, HANGAR, 0, 0, 1 },{ 2, HELITAKEOFF, 0, 0, 12 },{ 2, 0, 0, 0, 12 },
	/* In Air */
	{ 3, 0, 0, 0, 4 },
	{ 4, 0, 0, 0, 5 },
	{ 5, 0, 0, 0, 6 },
	{ 6, 0, 0, 0, 7 },
	// Attempt Landing
	{ 7, FLYING, 0, 0, 3 },{ 7, HELILANDING, 0, 0, 8 },{ 7, HANGAR, 0, 0, 13 },
	/* Helicopter -- stay in air in special place as a buffer to choose from helipads */
	{ 8, HELILANDING, PRE_HELIPAD_block, 0, 9 },
	{ 9, HELIENDLANDING, PRE_HELIPAD_block, 0, 9 },{ 9, HELI01, 0, 0, 10 },{ 9, HANGAR, 0, 0, 13 },
	//Positioning
	{ 10,   0, 0, 0, 11 },
	// Landing Helipad01
	{ 11, 255, 0, 0, 11 },{ 11, HELI01, HELI01_block, 0, 2 },{ 11, HANGAR, 0, 0, 1 },{ 11, 0, 0, 0, 1 },
	// Takeoff Helipad01
	{ 12, HELITAKEOFF, 0, 0, 0 },
	// Hangar Landing
	{ 13, 0, 0, 0, 14 }, // need to go to hangar when waiting in air
	{ 14, 0, 0, 0, 1 },
	// Hangar - Takeoff
	{ 15, HELITAKEOFF, 0, 0, 0 }, // takeoff outside depot
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

/* helistation */
static const HangarTileTable _airport_depots_helistation[] = { { { 0, 0 }, DIR_SE, 0 } };
static const TerminalTileTable _airport_terminals_helistation[] = { { { 2, 0 }, HELI01_block },{ { 3, 0 }, HELI02_block },{ { 3, 1 }, HELI03_block } };  // 0 = No terminals.
static const RunwayTerminalTable _airport_runways_helistation[] = { { 0, 0, 0 } };      // 0 = no runways.
static const byte _airport_entries_helistation[] = { 25, 25, 25, 25 };
static const byte _airport_helientry_helistation[] = { 25, 25, 25, 25 };
static const AirportFTAbuildup _airport_fta_helistation[] = {
	{ 0, HANGAR, 0, 0, 0 },{ 0, HELI01, 0, TAXI01_block, 1 },{ 0, HELI02, 0, TAXI01_block, 1 },{ 0, HELI03, 0, TAXI01_block, 1 },{ 0, HELITAKEOFF, 0, 0, 1 },{ 0, 0, 0, 0, 1 },
	{ 1, 255, HANG02_block, 0, 0 },{ 1, HANGAR, 0, 0, 0 },{ 1, HELITAKEOFF, 0, 0, 3 },{ 1, 0, 0, 0, 4 },
	/* landing */
	{ 2, FLYING, NOTHING_block, 0, 28 },{ 2, HELILANDING, 0, 0, 15 },{ 2, 0, 0, 0, 28 },
	/* helicopter side */
	{ 3, HELITAKEOFF, 0, 0, 0 }, // helitakeoff outside hangar2
	{ 4, 255, 0, TAXI01_block, 0 },{ 4, HANGAR, 0, 0, 1 },{ 4, HELITAKEOFF, 0, 0, 1 },{ 4, 0, 0, 0, 5 },
	{ 5, 255, 0, TAXI01_block, 0 },{ 5, HELI01, 0, 0, 6 },{ 5, HELI02, 0, 0, 7 },{ 5, HELI03, 0, 0, 8 },{ 5, 0, 0, 0, 4 },
	{ 6, HELI01, HELI01_block, 0, 6 },{ 6, HANGAR, HANG02_block, 0, 5 },{ 6, HELITAKEOFF, 0, 0, 9 },{ 6, 0, 0, 0, 6 },
	{ 7, HELI02, HELI02_block, 0, 7 },{ 7, HANGAR, HANG02_block, 0, 5 },{ 7, HELITAKEOFF, 0, 0, 10 },{ 7, 0, 0, 0, 7 },
	{ 8, HELI03, HELI03_block, 0, 8 },{ 8, HANGAR, HANG02_block, 0, 5 },{ 8, HELITAKEOFF, 0, 0, 11 },{ 8, 0, 0, 0, 8 },
	{ 9, 0, HELI01_block, 0, 12 },
	{ 10, 0, HELI02_block, 0, 13 },
	{ 11, 0, HELI03_block, 0, 14 },
	{ 12, HELITAKEOFF, 0, 0, 0 },
	{ 13, HELITAKEOFF, 0, 0, 0 },
	{ 14, HELITAKEOFF, 0, 0, 0 },
	/* heli - in flight moves */
	{ 15, HELILANDING, PRE_HELIPAD_block, 0, 16 },
	{ 16, HELIENDLANDING, PRE_HELIPAD_block, 0, 16 },{ 16, HELI01, 0, 0, 17 },{ 16, HELI02, 0, 0, 18 },{ 16, HELI03, 0, 0, 19 },{ 16, HANGAR, 0, 0, 23 },
	{ 17, 0, 0, 0, 20 },
	{ 18, 0, 0, 0, 21 },
	{ 19, 0, 0, 0, 22 },
	/* heli landing */
	{ 20, 255, 0, 0, 0 },{ 20, HELI01, HELI01_block, 0, 6 },{ 20, HANGAR, 0, 0, 23 },{ 20, 0, 0, 0, 6 },
	{ 21, 255, 0, 0, 0 },{ 21, HELI02, HELI02_block, 0, 7 },{ 21, HANGAR, 0, 0, 23 },{ 21, 0, 0, 0, 7 },
	{ 22, 255, 0, 0, 0 },{ 22, HELI03, HELI03_block, 0, 8 },{ 22, HANGAR, 0, 0, 23 },{ 22, 0, 0, 0, 8 },
	{ 23, 0, HANG02_block, 0, 24 }, // need to go to helihangar when waiting in air
	{ 24, 0, HANG02_block, 0, 1 },
	{ 25, 0, 0, 0, 26 },
	{ 26, 0, 0, 0, 27 },
	{ 27, 0, 0, 0, 2 },
	{ 28, 0, 0, 0, 29 },
	{ 29, 0, 0, 0, 30 },
	{ 30, 0, 0, 0, 31 },
	{ 31, 0, 0, 0, 32 },
	{ 32, 0, 0, 0, 25 },
	{ MAX_ELEMENTS, 0, 0, 0, 0 } // end marker. DO NOT REMOVE
};

#endif /* AIRPORT_MOVEMENT_H */
