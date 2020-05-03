/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file airport.cpp Functions related to airports. */

#include "stdafx.h"
#include "station_base.h"
#include "table/strings.h"
#include "table/airport_movement.h"
#include "table/airporttile_ids.h"

#include "safeguards.h"


/**
 * Define a generic airport.
 * @param name Suffix of the names of the airport data.
 * @param terminals The terminals.
 * @param num_cargo_term Number of cargo terminals. (Uses Helipads)
 * @param num_helipads Number of heli pads.
 * @param flags Information about the class of FTA.
 * @param delta_z Height of the airport above the land.
 */
#define AIRPORT_GENERIC(name, terminals, num_cargo_term, num_helipads, flags, delta_z) \
	static AirportFTAClass _airportfta_ ## name(_airport_moving_data_ ## name, terminals, num_cargo_term, \
			num_helipads, _airport_entries_ ## name, _airport_helientry_ ## name, flags, _airport_fta_ ## name, delta_z);

/**
 * Define an airport.
 * @param name Suffix of the names of the airport data.
 * @param num_cargo_term Number of cargo terminals. (Uses Helipads)
 * @param num_helipads Number of heli pads.
 * @param short_strip Airport has a short land/take-off strip.
 */
#define AIRPORT(name, num_cargo_term, num_helipads, short_strip) \
	AIRPORT_GENERIC(name, _airport_terminal_ ## name, num_cargo_term, num_helipads, AirportFTAClass::ALL | (short_strip ? AirportFTAClass::SHORT_STRIP : (AirportFTAClass::Flags)0), 0)

/**
 * Define a heliport.
 * @param name Suffix of the names of the helipad data.
 * @param num_cargo_term Number of cargo terminals. (Uses Helipads)
 * @param num_helipads Number of heli pads.
 * @param delta_z Height of the airport above the land.
 */
#define HELIPORT(name, num_cargo_term, num_helipads, delta_z) \
	AIRPORT_GENERIC(name, NULL, num_cargo_term, num_helipads, AirportFTAClass::HELICOPTERS, delta_z)

AIRPORT(country, 0, 0, true)
AIRPORT(city, 0, 0, false)
HELIPORT(heliport, 0, 1, 60)
AIRPORT(metropolitan, 0, 0, false)
AIRPORT(international, 2, 2, false)
AIRPORT(commuter, 2, 2, true)
HELIPORT(helidepot, 0, 1, 0)
AIRPORT(intercontinental, 2, 2, false)
HELIPORT(helistation, 0, 3, 0)
HELIPORT(oilrig, 0, 1, 54)
AIRPORT(terminus1, 3, 3, false)
AIRPORT(terminus2, 6, 6, false)
AIRPORT(terminus3, 9, 9, false)
AIRPORT(terminus4, 12, 12, false)
AIRPORT(sanfran, 4, 2, false)
AIRPORT(leipzig, 4, 2, false)
AIRPORT(cargos, 4, 4, false)
AIRPORT(cargol, 8, 8, false)
AIRPORT(cargox, 12, 12, false)
AIRPORT(commuterl, 2, 2, false)
AIRPORT(windy, 1, 1, false)
AIRPORT(indianapolis, 6, 2, false)
AIRPORT(schiphol, 4, 2, false)
AIRPORT_GENERIC(dummy, NULL, 0, 0, AirportFTAClass::ALL, 0)

#undef HELIPORT
#undef AIRPORT
#undef AIRPORT_GENERIC

#include "table/airport_defaults.h"


static uint16 AirportGetNofElements(const AirportFTAbuildup *apFA);
static AirportFTA *AirportBuildAutomata(uint nofelements, const AirportFTAbuildup *apFA);


/**
 * Rotate the airport moving data to another rotation.
 * @param orig Pointer to the moving data to rotate.
 * @param rotation How to rotate the moving data.
 * @param num_tiles_x Number of tiles in x direction.
 * @param num_tiles_y Number of tiles in y direction.
 * @return The rotated moving data.
 */
AirportMovingData RotateAirportMovingData(const AirportMovingData *orig, Direction rotation, uint num_tiles_x, uint num_tiles_y)
{
	AirportMovingData amd;
	amd.flag = orig->flag;
	amd.direction = ChangeDir(orig->direction, (DirDiff)rotation);
	switch (rotation) {
		case DIR_N:
			amd.x = orig->x;
			amd.y = orig->y;
			break;

		case DIR_E:
			amd.x = orig->y;
			amd.y = num_tiles_y * TILE_SIZE - orig->x - 1;
			break;

		case DIR_S:
			amd.x = num_tiles_x * TILE_SIZE - orig->x - 1;
			amd.y = num_tiles_y * TILE_SIZE - orig->y - 1;
			break;

		case DIR_W:
			amd.x = num_tiles_x * TILE_SIZE - orig->y - 1;
			amd.y = orig->x;
			break;

		default: NOT_REACHED();
	}
	return amd;
}

AirportFTAClass::AirportFTAClass(
	const AirportMovingData *moving_data_,
	const byte *terminals_,
	const byte num_cargo_term_,
	const byte num_helipads_,
	const byte *entry_points_,
	const byte *heli_entry_point_,
	Flags flags_,
	const AirportFTAbuildup *apFA,
	byte delta_z_
) :
	moving_data(moving_data_),
	terminals(terminals_),
	num_cargo_term(num_cargo_term_),
	num_helipads(num_helipads_),
	flags(flags_),
	nofelements(AirportGetNofElements(apFA)),
	heli_entry_point(heli_entry_point_),
	entry_points(entry_points_),
	delta_z(delta_z_)
{
	/* Build the state machine itself */
	this->layout = AirportBuildAutomata(this->nofelements, apFA);
}

AirportFTAClass::~AirportFTAClass()
{
	for (uint i = 0; i < nofelements; i++) {
		AirportFTA *current = layout[i].next;
		while (current != nullptr) {
			AirportFTA *next = current->next;
			free(current);
			current = next;
		}
	}
	free(layout);
}

/**
 * Get the number of elements of a source Airport state automata
 * Since it is actually just a big array of AirportFTA types, we only
 * know one element from the other by differing 'position' identifiers
 */
static uint16 AirportGetNofElements(const AirportFTAbuildup *apFA)
{
	uint16 nofelements = 0;
	int temp = apFA[0].position;

	for (uint i = 0; i < MAX_ELEMENTS; i++) {
		if (temp != apFA[i].position) {
			nofelements++;
			temp = apFA[i].position;
		}
		if (apFA[i].position == MAX_ELEMENTS) break;
	}
	return nofelements;
}

/**
 * Construct the FTA given a description.
 * @param nofelements The number of elements in the FTA.
 * @param apFA The description of the FTA.
 * @return The FTA describing the airport.
 */
static AirportFTA *AirportBuildAutomata(uint nofelements, const AirportFTAbuildup *apFA)
{
	AirportFTA *FAutomata = MallocT<AirportFTA>(nofelements);
	uint16 internalcounter = 0;

	for (uint i = 0; i < nofelements; i++) {
		AirportFTA *current = &FAutomata[i];
		current->position      = apFA[internalcounter].position;
		current->heading       = apFA[internalcounter].heading;
		current->block1	       = apFA[internalcounter].block1;
		current->block2        = apFA[internalcounter].block2;
		current->next_position = apFA[internalcounter].next;

		/* outgoing nodes from the same position, create linked list */
		while (current->position == apFA[internalcounter + 1].position) {
			AirportFTA *newNode = MallocT<AirportFTA>(1);

			newNode->position      = apFA[internalcounter + 1].position;
			newNode->heading       = apFA[internalcounter + 1].heading;
			newNode->block1        = apFA[internalcounter + 1].block1;
			newNode->block2        = apFA[internalcounter + 1].block2;
			newNode->next_position = apFA[internalcounter + 1].next;
			/* create link */
			current->next = newNode;
			current = current->next;
			internalcounter++;
		}
		current->next = nullptr;
		internalcounter++;
	}
	return FAutomata;
}

/**
 * Get the finite state machine of an airport type.
 * @param airport_type %Airport type to query FTA from. @see AirportTypes
 * @return Finite state machine of the airport.
 */
const AirportFTAClass *GetAirport(const byte airport_type)
{
	if (airport_type == AT_DUMMY) return &_airportfta_dummy;
	return AirportSpec::Get(airport_type)->fsm;
}

/**
 * Get the vehicle position when an aircraft is build at the given tile
 * @param hangar_tile The tile on which the vehicle is build
 * @return The position (index in airport node array) where the aircraft ends up
 */
byte GetVehiclePosOnBuild(TileIndex hangar_tile)
{
	const Station *st = Station::GetByTile(hangar_tile);
	const AirportFTAClass *apc = st->airport.GetFTA();
	/* When we click on hangar we know the tile it is on. By that we know
	 * its position in the array of depots the airport has.....we can search
	 * layout for #th position of depot. Since layout must start with a listing
	 * of all depots, it is simple */
	for (uint i = 0;; i++) {
		if (st->airport.GetHangarTile(i) == hangar_tile) {
			assert(apc->layout[i].heading == HANGAR);
			return apc->layout[i].position;
		}
	}
	NOT_REACHED();
}
