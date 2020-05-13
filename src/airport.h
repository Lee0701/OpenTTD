/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file airport.h Various declarations for airports */

#ifndef AIRPORT_H
#define AIRPORT_H

#include "direction_type.h"
#include "tile_type.h"

/** Some airport-related constants */
static const uint MAX_TERMINALS = 36;                        ///< maximum number of terminals per airport
static const uint MAX_CARGO_TERM = 12;						 ///< maximum number of cargo terminals per airport
static const uint MAX_HELIPADS = 12;                         ///< maximum number of helipads per airport
static const uint MAX_ELEMENTS = 1280;                       ///< maximum number of aircraft positions at airport

static const uint NUM_AIRPORTTILES_PER_GRF = 255;            ///< Number of airport tiles per NewGRF; limited to 255 to allow extending Action3 with an extended byte later on.

static const uint NUM_AIRPORTTILES       = 256;              ///< Total number of airport tiles.
static const uint NEW_AIRPORTTILE_OFFSET = 149;              ///< offset of first newgrf airport tile
static const uint INVALID_AIRPORTTILE    = NUM_AIRPORTTILES; ///< id for an invalid airport tile

/** Airport types */
enum AirportTypes {
	AT_SMALL             =   0, ///< Small airport.  Country
	AT_LARGE             =   1, ///< Large airport.  City
	AT_HELIPORT          =   2, ///< Heli port.
	AT_METROPOLITAN      =   3, ///< Metropolitan airport.
	AT_INTERNATIONAL     =   4, ///< International airport.
	AT_COMMUTER          =   5, ///< Commuter airport.
	AT_HELIDEPOT         =   6, ///< Heli depot.
	AT_INTERCON          =   7, ///< Intercontinental airport.
	AT_HELISTATION       =   8, ///< Heli station airport.
	AT_OILRIG            =   9, ///< Oilrig airport.
	AT_TERMINUS1         =  10, ///< Terminus1 Airport 2 Hangars, 2 Runways,  9 Terminals,  3 Helipads,  3 Cargo Terminals
	AT_TERMINUS2         =  11, ///< Terminus2 Airport 4 Hangars, 4 Runways, 16 Terminals,  6 Helipads,  6 Cargo Terminals
	AT_TERMINUS3         =  12, ///< Terminus3 Airport 6 Hangars, 6 Runways, 25 Terminals,  9 Helipads,  9 Cargo Terminals
	AT_TERMINUS4         =  13, ///< Terminus4 Airport 8 Hangars, 8 Runways, 36 Terminals, 12 Helipads, 12 Cargo Terminals
	AT_COMMUTERL         =  14, ///< Commuter Large  2 Hangars 1 Runway, 6 Terminals, 2 Helipads, 2 Cargo Terminals.
	AT_LEIPZIG           =  15, ///< Leipzig Airport 2 Hangars, 2 Runways, 7 Terminals, 2 Helipads, 4 Cargo Terminals
	AT_SANFRAN           =  16, ///< San Francisco Airport 2 Hangars, 4 Runways, 9 Terminals, 2 Helipads, 4 Cargo Terminals
	AT_CARGOS            =  17, ///< Cargo Small Airport 2 Hangars, 1 Runway, 4 Cargo Terminals
	AT_CARGOL            =  18, ///< Cargo Large Airport 2 Hangars, 2 Runways, 8 Cargo Terminals
	AT_CARGOX            =  19, ///< Cargo X-Large Airport  2 Hangars, 2 Runways, 12 cargo Terminals
	AT_WINDY             =  20, ///< Windy City Airport  2 Hangars, 1 Runway, 2 Terminals, 2 Helipads, 2 Cargo terminals.
	AT_INDIANAPOLIS      =  21, ///< Indianapolis Airport  4 Hangars, 4 Runways, 26 Terminals, 2 Helipads, 6 Cargo Terminals
	AT_SCHIPHOL          =  22, ///< Schiphol Airport  3 Hangars, 6 Runways, 12 Terminals, 2 Helipads, 4 Cargo Terminals
	NEW_AIRPORT_OFFSET   =  23, ///< Number of the first newgrf airport.
	NUM_AIRPORTS_PER_GRF = 128, ///< Maximal number of airports per NewGRF.
	NUM_AIRPORTS         = 128, ///< Maximal number of airports in total.
	AT_INVALID           = 254, ///< Invalid airport.
	AT_DUMMY             = 255, ///< Dummy airport.
};

/** Flags for airport movement data. */
enum AirportMovingDataFlags {
	AMED_NOSPDCLAMP = 1 << 0, ///< No speed restrictions.
	AMED_TAKEOFF    = 1 << 1, ///< Takeoff movement.
	AMED_SLOWTURN   = 1 << 2, ///< Turn slowly (mostly used in the air).
	AMED_LAND       = 1 << 3, ///< Landing onto landing strip.
	AMED_EXACTPOS   = 1 << 4, ///< Go exactly to the destination coordinates.
	AMED_BRAKE      = 1 << 5, ///< Taxiing at the airport.
	AMED_HELI_RAISE = 1 << 6, ///< Helicopter take-off.
	AMED_HELI_LOWER = 1 << 7, ///< Helicopter landing.
	AMED_HOLD       = 1 << 8, ///< Holding pattern movement (above the airport).
};

/** Movement States on Airports (headings target) */
enum AirportMovementStates {
	TO_ALL         =   0, ///< Go in this direction for every target.
	HANGAR         =   1, ///< Heading for hangar.
	TERM01         =   2, ///< Heading for terminal 01.
	TERM02         =   3, ///< Heading for terminal 02.
	TERM03         =   4, ///< Heading for terminal 03.
	TERM04         =   5, ///< Heading for terminal 04.
	TERM05         =   6, ///< Heading for terminal 05.
	TERM06         =   7, ///< Heading for terminal 06.
	TERM07         =   8, ///< Heading for terminal 07.
	TERM08         =   9, ///< Heading for terminal 08.
	TERM09         =  10, ///< Heading for terminal 09.
	TERM10         =  11, ///< Heading for terminal 10.
	TERM11         =  12, ///< Heading for terminal 11.
	TERM12         =  13, ///< Heading for terminal 12.
	TERM13         =  14, ///< Heading for terminal 13.
	TERM14         =  15, ///< Heading for terminal 14.
	TERM15         =  16, ///< Heading for terminal 15.
	TERM16         =  17, ///< Heading for terminal 16.
	TERM17         =  18, ///< Heading for terminal 17.
	TERM18         =  19, ///< Heading for terminal 18.
	TERM19         =  20, ///< Heading for terminal 19.
	TERM20         =  21, ///< Heading for terminal 20.
	TERM21         =  22, ///< Heading for terminal 21.
	TERM22         =  23, ///< Heading for terminal 22.
	TERM23         =  24, ///< Heading for terminal 23.
	TERM24         =  25, ///< Heading for terminal 24.
	TERM25         =  26, ///< Heading for terminal 25.
	TERM26         =  27, ///< Heading for terminal 26.
	TERM27         =  28, ///< Heading for terminal 27.
	TERM28         =  29, ///< Heading for terminal 28.
	TERM29         =  30, ///< Heading for terminal 29.
	TERM30         =  31, ///< Heading for terminal 30.
	TERM31         =  32, ///< Heading for terminal 31.
	TERM32         =  33, ///< Heading for terminal 32.
	TERM33         =  34, ///< Heading for terminal 33.
	TERM34         =  35, ///< Heading for terminal 34.
	TERM35         =  36, ///< Heading for terminal 35.
	TERM36         =  37, ///< Heading for terminal 36.
	HELI01         =  38, ///< Heading for helipad 01.
	HELI02         =  39, ///< Heading for helipad 02.
	HELI03         =  40, ///< Heading for helipad 03.
	HELI04         =  41, ///< Heading for helipad 04.
	HELI05         =  42, ///< Heading for helipad 05.
	HELI06         =  43, ///< Heading for helipad 06.
	HELI07         =  44, ///< Heading for helipad 07.
	HELI08         =  45, ///< Heading for helipad 08.
	HELI09         =  46, ///< Heading for helipad 09.
	HELI10         =  47, ///< Heading for helipad 10.
	HELI11         =  48, ///< Heading for helipad 11.
	HELI12         =  49, ///< Heading for helipad 12.
	CARG01         =  50, ///< Heading for Cargo 01 terminal.
	CARG02         =  51, ///< Heading for Cargo 02 terminal.
	CARG03         =  52, ///< Heading for Cargo 03 terminal.
	CARG04         =  53, ///< Heading for Cargo 04 terminal.
	CARG05         =  54, ///< Heading for Cargo 05 terminal.
	CARG06         =  55, ///< Heading for Cargo 06 terminal.
	CARG07         =  56, ///< Heading for Cargo 07 terminal.
	CARG08         =  57, ///< Heading for Cargo 08 terminal.
	CARG09         =  58, ///< Heading for Cargo 09 terminal.
	CARG10         =  59, ///< Heading for Cargo 10 terminal.
	CARG11         =  60, ///< Heading for Cargo 11 terminal.
	CARG12         =  61, ///< Heading for Cargo 12 terminal.
	TAKEOFF        =  62, ///< Airplane wants to leave the airport.
	STARTTAKEOFF   =  63, ///< Airplane has arrived at a runway for take-off.
	ENDTAKEOFF     =  64, ///< Airplane has reached end-point of the take-off runway.
	HELITAKEOFF    =  65, ///< Helicopter wants to leave the airport.
	FLYING         =  66, ///< %Vehicle is flying in the air.
	PRELANDING      = 67, ///< Airplane wants to land but still in holding pattern.
	LANDING        =  68, ///< Airplane wants to land.
	ENDLANDING     =  69, ///< Airplane wants to finish landing.
	HELILANDING    =  70, ///< Helicopter wants to land.
	HELIENDLANDING =  71, ///< Helicopter wants to finish landing.
	BACKUP         =  72, ///< Used to push back from terminal.
	MAX_HEADINGS   =  72, ///< Last valid target to head for.
};

/** Movement Blocks on Airports blocks (eg_airport_flags1). */
// Somewhere down deep in the code is a restriction on the HANGAR blocks...
//  HANGAR blocks need to be less than 32.  Others do not seem to matter.
static const uint64
	// Special Blocks Both sets of flags are 64bits.  0-63  0 and 63 are both reserved in both flags.
NOTHING_block = 1ULL << 0, ///< flags1, flags2, Nothing blocked. (All to)
AIRPORT_CLOSED_block = 1ULL << 63, ///< flags1, flags2, Dummy block for indicating a closed airport.

	// Hangar Blocks
HANG01_block = 1ULL << 1, ///< flags1, Hangar 01 Block
HANG02_block = 1ULL << 2, ///< flags1, Hangar 02 Block
HANG03_block = 1ULL << 3, ///< flags1, Hangar 03 Block
HANG04_block = 1ULL << 4, ///< flags1, Hangar 04 Block
HANG05_block = 1ULL << 5, ///< flags1, Hangar 05 Block
HANG06_block = 1ULL << 6, ///< flags1, Hangar 06 Block
HANG07_block = 1ULL << 7, ///< flags1, Hangar 07 Block
HANG08_block = 1ULL << 8, ///< flags1, Hangar 08 Block
	// Terminal Blocks
TERM01_block = 1ULL << 9, ///< flags1, Terminal 01 Block
TERM02_block = 1ULL << 10, ///< flags1, Terminal 02 Block
TERM03_block = 1ULL << 11, ///< flags1, Terminal 03 Block
TERM04_block = 1ULL << 12, ///< flags1, Terminal 04 Block
TERM05_block = 1ULL << 13, ///< flags1, Terminal 05 Block
TERM06_block = 1ULL << 14, ///< flags1, Terminal 06 Block
TERM07_block = 1ULL << 15, ///< flags1, Terminal 07 Block
TERM08_block = 1ULL << 16, ///< flags1, Terminal 08 Block
TERM09_block = 1ULL << 17, ///< flags1, Terminal 09 Block
TERM10_block = 1ULL << 18, ///< flags1, Terminal 10 Block
TERM11_block = 1ULL << 19, ///< flags1, Terminal 11 Block
TERM12_block = 1ULL << 20, ///< flags1, Terminal 12 Block
TERM13_block = 1ULL << 21, ///< flags1, Terminal 13 Block
TERM14_block = 1ULL << 22, ///< flags1, Terminal 14 Block
TERM15_block = 1ULL << 23, ///< flags1, Terminal 15 Block
TERM16_block = 1ULL << 24, ///< flags1, Terminal 16 Block
TERM17_block = 1ULL << 25, ///< flags1, Terminal 17 Block
TERM18_block = 1ULL << 26, ///< flags1, Terminal 18 Block
TERM19_block = 1ULL << 27, ///< flags1, Terminal 19 Block
TERM20_block = 1ULL << 28, ///< flags1, Terminal 20 Block
TERM21_block = 1ULL << 29, ///< flags1, Terminal 21 Block
TERM22_block = 1ULL << 30, ///< flags1, Terminal 22 Block
TERM23_block = 1ULL << 31, ///< flags1, Terminal 23 Block
TERM24_block = 1ULL << 32, ///< flags1, Terminal 24 Block
TERM25_block = 1ULL << 33, ///< flags1, Terminal 25 Block
TERM26_block = 1ULL << 34, ///< flags1, Terminal 26 Block
TERM27_block = 1ULL << 35, ///< flags1, Terminal 27 Block
TERM28_block = 1ULL << 36, ///< flags1, Terminal 28 Block
TERM29_block = 1ULL << 37, ///< flags1, Terminal 29 Block
TERM30_block = 1ULL << 38, ///< flags1, Terminal 30 Block
TERM31_block = 1ULL << 39, ///< flags1, Terminal 31 Block
TERM32_block = 1ULL << 40, ///< flags1, Terminal 32 Block
TERM33_block = 1ULL << 41, ///< flags1, Terminal 33 Block
TERM34_block = 1ULL << 42, ///< flags1, Terminal 34 Block
TERM35_block = 1ULL << 43, ///< flags1, Terminal 35 Block
TERM36_block = 1ULL << 44, ///< flags1, Terminal 36 Block
		// Helipad Blocks
HELI01_block = 1ULL << 45, ///< flags1, Helipad 01 Block.
HELI02_block = 1ULL << 46, ///< flags1, Helipad 02 Block.
HELI03_block = 1ULL << 47, ///< flags1, Helipad 03 Block.
HELI04_block = 1ULL << 48, ///< flags1, Helipad 04 Block.
HELI05_block = 1ULL << 49, ///< flags1, Helipad 05 Block.
HELI06_block = 1ULL << 50, ///< flags1, Helipad 06 Block.
HELI07_block = 1ULL << 51, ///< flags1, Helipad 07 Block.
HELI08_block = 1ULL << 52, ///< flags1, Helipad 08 Block.
HELI09_block = 1ULL << 53, ///< flags1, Helipad 09 Block.
HELI10_block = 1ULL << 54, ///< flags1, Helipad 10 Block.
HELI11_block = 1ULL << 55, ///< flags1, Helipad 11 Block.
HELI12_block = 1ULL << 56, ///< flags1, Helipad 12 Block.
		// Runway Blocks
RUNW01_block = 1ULL << 57, ///< flags1, Runway 01 Block.
RUNW02_block = 1ULL << 58, ///< flags1, Runway 02 Block.
RUNW03_block = 1ULL << 59, ///< flags1, Runway 03 Block.
RUNW04_block = 1ULL << 60, ///< flags1, Runway 04 Block.
RUNW05_block = 1ULL << 61, ///< flags1, Runway 05 Block.
RUNW06_block = 1ULL << 62, ///< flags1, Runway 06 Block.
		// Runways 7 and 8 are in flags2

		// Cargo Terminal Blocks,  These share with the Helicopter terminal blocks.  Do not attempt to use them both at the same time.
CARG01_block = 1ULL << 45, ///< flags1 Alt, Shares with HELI01_block.
CARG02_block = 1ULL << 46, ///< flags1 Alt, Shares with HELI02_block.
CARG03_block = 1ULL << 47, ///< flags1 Alt, Shares with HELI03_block.
CARG04_block = 1ULL << 48, ///< flags1 Alt, Shares with HELI04_block.
CARG05_block = 1ULL << 49, ///< flags1 Alt, Shares with HELI05_block.
CARG06_block = 1ULL << 50, ///< flags1 Alt, Shares with HELI06_block.
CARG07_block = 1ULL << 51, ///< flags1 Alt, Shares with HELI07_block.
CARG08_block = 1ULL << 52, ///< flags1 Alt, Shares with HELI08_block.
CARG09_block = 1ULL << 53, ///< flags1 Alt, Shares with HELI09_block.
CARG10_block = 1ULL << 54, ///< flags1 Alt, Shares with HELI10_block.
CARG11_block = 1ULL << 55, ///< flags1 Alt, Shares with HELI11_block.
CARG12_block = 1ULL << 56, ///< flags1 Alt, Shares with HELI12_block.

		// Old blocks to be retired
RUNWAY_IN_block = 1ULL << 8, ///< flags1 Alt, Shares with HANG08_block.
AIRPORT_BUSY_block = 1ULL << 8, ///< flags1 Alt, Shares with HANG08_block.
PRE_HELIPAD_block = 1ULL << 21, ///< flags1 Alt, Shares with TERM13_block.

		// Taxi Blocks
TAXI01_block = 1ULL << 1, ///< flags2, Taxiway 01 Block.
TAXI02_block = 1ULL << 2, ///< flags2, Taxiway 02 Block.
TAXI03_block = 1ULL << 3, ///< flags2, Taxiway 03 Block.
TAXI04_block = 1ULL << 4, ///< flags2, Taxiway 04 Block.
TAXI05_block = 1ULL << 5, ///< flags2, Taxiway 05 Block.
TAXI06_block = 1ULL << 6, ///< flags2, Taxiway 06 Block.
TAXI07_block = 1ULL << 7, ///< flags2, Taxiway 07 Block.
TAXI08_block = 1ULL << 8, ///< flags2, Taxiway 08 Block.
TAXI09_block = 1ULL << 9, ///< flags2, Taxiway 09 Block.
TAXI10_block = 1ULL << 10, ///< flags2, Taxiway 10 Block.
TAXI11_block = 1ULL << 11, ///< flags2, Taxiway 11 Block.
TAXI12_block = 1ULL << 12, ///< flags2, Taxiway 12 Block.
TAXI13_block = 1ULL << 13, ///< flags2, Taxiway 13 Block.
TAXI14_block = 1ULL << 14, ///< flags2, Taxiway 14 Block.
TAXI15_block = 1ULL << 15, ///< flags2, Taxiway 15 Block.
TAXI16_block = 1ULL << 16, ///< flags2, Taxiway 16 Block.
TAXI17_block = 1ULL << 17, ///< flags2, Taxiway 17 Block.
TAXI18_block = 1ULL << 18, ///< flags2, Taxiway 18 Block.
TAXI19_block = 1ULL << 19, ///< flags2, Taxiway 19 Block.
TAXI20_block = 1ULL << 20, ///< flags2, Taxiway 20 Block.
TAXI21_block = 1ULL << 21, ///< flags2, Taxiway 21 Block.
TAXI22_block = 1ULL << 22, ///< flags2, Taxiway 22 Block.
TAXI23_block = 1ULL << 23, ///< flags2, Taxiway 23 Block.
TAXI24_block = 1ULL << 24, ///< flags2, Taxiway 24 Block.
TAXI25_block = 1ULL << 25, ///< flags2, Taxiway 25 Block.
TAXI26_block = 1ULL << 26, ///< flags2, Taxiway 26 Block.
TAXI27_block = 1ULL << 27, ///< flags2, Taxiway 27 Block.
TAXI28_block = 1ULL << 28, ///< flags2, Taxiway 28 Block.
TAXI29_block = 1ULL << 29, ///< flags2, Taxiway 29 Block.
TAXI30_block = 1ULL << 30, ///< flags2, Taxiway 30 Block.
TAXI31_block = 1ULL << 31, ///< flags2, Taxiway 31 Block.
TAXI32_block = 1ULL << 32, ///< flags2, Taxiway 32 Block.
TAXI33_block = 1ULL << 33, ///< flags2, Taxiway 33 Block.
TAXI34_block = 1ULL << 34, ///< flags2, Taxiway 34 Block.
TAXI35_block = 1ULL << 35, ///< flags2, Taxiway 35 Block.
TAXI36_block = 1ULL << 36, ///< flags2, Taxiway 36 Block.
TAXI37_block = 1ULL << 37, ///< flags2, Taxiway 37 Block.
TAXI38_block = 1ULL << 38, ///< flags2, Taxiway 38 Block.
TAXI39_block = 1ULL << 39, ///< flags2, Taxiway 39 Block.
TAXI40_block = 1ULL << 40, ///< flags2, Taxiway 40 Block.
TAXI41_block = 1ULL << 41, ///< flags2, Taxiway 41 Block.
TAXI42_block = 1ULL << 42, ///< flags2, Taxiway 42 Block.
TAXI43_block = 1ULL << 43, ///< flags2, Taxiway 43 Block.
TAXI44_block = 1ULL << 44, ///< flags2, Taxiway 44 Block.
TAXI45_block = 1ULL << 45, ///< flags2, Taxiway 45 Block.
TAXI46_block = 1ULL << 46, ///< flags2, Taxiway 46 Block.
TAXI47_block = 1ULL << 47, ///< flags2, Taxiway 47 Block.
TAXI48_block = 1ULL << 48, ///< flags2, Taxiway 48 Block.
TAXI49_block = 1ULL << 49, ///< flags2, Taxiway 49 Block.
TAXI50_block = 1ULL << 50, ///< flags2, Taxiway 50 Block.
TAXI51_block = 1ULL << 51, ///< flags2, Taxiway 51 Block.
TAXI52_block = 1ULL << 52, ///< flags2, Taxiway 52 Block.
TAXI53_block = 1ULL << 53, ///< flags2, Taxiway 53 Block.
TAXI54_block = 1ULL << 54, ///< flags2, Taxiway 54 Block.
TAXI55_block = 1ULL << 55, ///< flags2, Taxiway 55 Block.
TAXI56_block = 1ULL << 56, ///< flags2, Taxiway 56 Block.
TAXI57_block = 1ULL << 57, ///< flags2, Taxiway 57 Block.
TAXI58_block = 1ULL << 58, ///< flags2, Taxiway 58 Block.
TAXI59_block = 1ULL << 59, ///< flags2, Taxiway 59 Block.
TAXI60_block = 1ULL << 62, ///< flags2, Taxiway 60 Block.
		// end Taxi Blocks
RUNW07_block = 1ULL << 60, ///< flags2, Runway 07 Block.
RUNW08_block = 1ULL << 61; ///< flags2, Runway 08 Block.

/** A single location on an airport where aircraft can move to. */
struct AirportMovingData {
	int16 x;             ///< x-coordinate of the destination.
	int16 y;             ///< y-coordinate of the destination.
	uint16 flag;         ///< special flags when moving towards the destination.
	Direction direction; ///< Direction to turn the aircraft after reaching the destination.
};

AirportMovingData RotateAirportMovingData(const AirportMovingData *orig, Direction rotation, uint num_tiles_x, uint num_tiles_y);

struct AirportFTAbuildup;

/** Finite sTate mAchine (FTA) of an airport. */
struct AirportFTAClass {
public:
	/** Bitmask of airport flags. */
	enum Flags {
		AIRPLANES   = 0x1,                     ///< Can planes land on this airport type?
		HELICOPTERS = 0x2,                     ///< Can helicopters land on this airport type?
		ALL         = AIRPLANES | HELICOPTERS, ///< Mask to check for both planes and helicopters.
		SHORT_STRIP = 0x4,                     ///< This airport has a short landing strip, dangerous for fast aircraft.
	};

	AirportFTAClass(
		const AirportMovingData *moving_data,
		const byte *terminals,
		const byte num_cargo_term,
		const byte num_helipads,
		const byte *entry_points,
		const byte *heli_entry_point,
		Flags flags,
		const AirportFTAbuildup *apFA,
		byte delta_z
	);

	~AirportFTAClass();

	/**
	 * Get movement data at a position.
	 * @param position Element number to get movement data about.
	 * @return Pointer to the movement data.
	 */
	const AirportMovingData *MovingData(byte position) const
	{
		assert(position < nofelements);
		return &moving_data[position];
	}

	const AirportMovingData *moving_data; ///< Movement data.
	struct AirportFTA *layout;            ///< state machine for airport
	const byte *terminals;                ///< %Array with the number of terminal groups, followed by the number of terminals in each group.
	const byte num_cargo_term;            ///< Number of cargo terminals on this airport. When 0, they will go to normal terminals.
	const byte num_helipads;              ///< Number of helipads on this airport. When 0 helicopters will go to normal terminals.
	Flags flags;                          ///< Flags for this airport type.
	byte nofelements;                     ///< number of positions the airport consists of
	const byte *entry_points;             ///< when an airplane arrives at this airport, enter it at position entry_point, index depends on direction
	const byte *heli_entry_point;         ///< Seperate entry point for helicopters.
	byte delta_z;                         ///< Z adjustment for helicopter pads
};

DECLARE_ENUM_AS_BIT_SET(AirportFTAClass::Flags)


/** Internal structure used in openttd - Finite sTate mAchine --> FTA */
struct AirportFTA {
	AirportFTA *next;        ///< possible extra movement choices from this position
	uint64 block1;           ///< 64 bit blocks (st->airport.flags), should be enough for the most complex airports
	uint64 block2;           ///< 64 bit blocks (st->airport.flags), should be enough for the most complex airports
	byte position;           ///< the position that an airplane is at
	byte next_position;      ///< next position from this position
	byte heading;            ///< heading (current orders), guiding an airplane to its target on an airport
};

const AirportFTAClass *GetAirport(const byte airport_type);
byte GetVehiclePosOnBuild(TileIndex hangar_tile);

#endif /* AIRPORT_H */
