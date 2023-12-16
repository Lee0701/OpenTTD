/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file dropdown_type.h Types related to the drop down widget. */

#ifndef WIDGETS_DROPDOWN_TYPE_H
#define WIDGETS_DROPDOWN_TYPE_H

#include "../window_type.h"
#include "../gfx_func.h"
#include "table/strings.h"
#include <vector>

enum DropDownSyncFocus {
	DDSF_NONE = 0,
	DDSF_RECV_FOCUS = 1,
	DDSF_LOST_FOCUS = 2,
	DDSF_ALL = DDSF_RECV_FOCUS | DDSF_LOST_FOCUS,
};

/**
 * Base list item class from which others are derived. If placed in a list it
 * will appear as a horizontal line in the menu.
 */
class DropDownListItem {
public:
	int result;  ///< Result code to return to window on selection
	bool masked; ///< Masked and unselectable item

	DropDownListItem(int result, bool masked) : result(result), masked(masked) {}
	virtual ~DropDownListItem() = default;

	virtual bool Selectable() const { return false; }
	virtual uint Height() const { return FONT_HEIGHT_NORMAL; }
	virtual uint Width() const { return 0; }
	virtual void Draw(const Rect &r, bool sel, Colours bg_colour) const;
};

/**
 * Common string list item.
 */
class DropDownListStringItem : public DropDownListItem {
public:
	std::string string; ///< String of item
	TextColour colour_flags = TC_BEGIN;

	DropDownListStringItem(StringID string, int result, bool masked);
	DropDownListStringItem(const std::string &string, int result, bool masked);

	bool Selectable() const override { return !this->String().empty(); }
	uint Width() const override;
	void Draw(const Rect &r, bool sel, Colours bg_colour) const override;
	const std::string &String() const { return this->string; }
	void SetColourFlags(TextColour colour_flags) { this->colour_flags = colour_flags; }

	static bool NatSortFunc(std::unique_ptr<const DropDownListItem> const &first, std::unique_ptr<const DropDownListItem> const &second);
};

/**
 * List item with icon and string.
 */
class DropDownListIconItem : public DropDownListStringItem {
	SpriteID sprite;
	PaletteID pal;
	Dimension dim;
	uint sprite_y;
public:
	DropDownListIconItem(SpriteID sprite, PaletteID pal, StringID string, int result, bool masked);

	uint Height() const override;
	uint Width() const override;
	void Draw(const Rect &r, bool sel, Colours bg_colour) const override;
	void SetDimension(Dimension d);
};

/**
 * A drop down list is a collection of drop down list items.
 */
typedef std::vector<std::unique_ptr<const DropDownListItem>> DropDownList;

void ShowDropDownListAt(Window *w, DropDownList &&list, int selected, int button, Rect wi_rect, Colours wi_colour, bool instant_close = false, DropDownSyncFocus sync_parent_focus = DDSF_NONE);

void ShowDropDownList(Window *w, DropDownList &&list, int selected, int button, uint width = 0, bool instant_close = false, DropDownSyncFocus sync_parent_focus = DDSF_NONE);

Dimension GetDropDownListDimension(const DropDownList &list);

#endif /* WIDGETS_DROPDOWN_TYPE_H */
