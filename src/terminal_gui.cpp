/* $Id: terminal_gui.cpp 27030 2014-10-21 19:16:47Z Vetteman06 $ */

/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file terminal_gui.cpp Handling of terminals gui. */

#include "stdafx.h"
#include "window_gui.h"
#include "gui.h"
#include "textbuf_gui.h"
#include "viewport_func.h"
#include "strings_func.h"
#include "command_func.h"
#include "company_func.h"
#include "company_base.h"
#include "window_func.h"
#include "station_base.h"
#include "widgets/terminal_widget.h"
#include "widgets/graph_widget.h"
#include "graph_gui.h"
#include "company_gui.h"

#include "table/strings.h"

#include "safeguards.h"

/** GUI for accessing terminals. */
struct TerminalWindow : Window {
	
	uint64 fullload;  ///< Which terminals are restricted against Full Load Orders.
	StationID sID;    ///< Used to set the Caption.
	uint64 terminal;  ///< Terminal selected.
	CompanyID TerminalCompany; ///< Terminal Owner
	const Station * st;    ///< Selected Station.
	
	/**
	 * Construct the window.
	 * @param desc The description of the window.
	 * @param window_number The window number, in this case the tileIndex of the terminal.
	 */
	TerminalWindow(WindowDesc * desc, TileIndex tile) : Window(desc)
	{
		const BaseStation * bst = BaseStation::GetByTile(tile);
		const Station * stbst = Station::From(bst);
		
		st = stbst;
		fullload = st->airport.fullload;
		sID = st->index;
		terminal = st->airport.GetTerminalNum(tile);
		
		
		this->CreateNestedTree();
		//if (this->vt == VEH_TRAIN) {
		//this->GetWidget<NWidgetCore>(WID_W_SHOW_VEHICLES)->SetDataTip(STR_TRAIN, STR_STATION_VIEW_SCHEDULED_TRAINS_TOOLTIP);
		//	this->GetWidget<NWidgetCore>(WID_T_CENTER_VIEW)->tool_tip = STR_WAYPOINT_VIEW_CENTER_TOOLTIP;
		//this->GetWidget<NWidgetCore>(WID_W_RENAME)->tool_tip = STR_WAYPOINT_VIEW_CHANGE_WAYPOINT_NAME;
		//}
		//this->GetWidget<NWidgetStacked>(WID_T_SHOW_RENAME)->SetDisplayedPlane(type == VEH_AIRCRAFT ? SZSP_NONE : 0);
		//this->GetWidget<NWidgetCore>(WID_T_FULLLOAD)->widget_data = STR_PLANE;
		//this->GetWidget<NWidgetCore>(WID_T_FULLLOAD)->tool_tip = STR_DEPOT_VEHICLE_ORDER_LIST_TRAIN_TOOLTIP;
		this->FinishInitNested(tile);
		
		this->flags |= WF_DISABLE_VP_SCROLL;
		
		NWidgetViewport * nvp = this->GetWidget<NWidgetViewport>(WID_T_VIEWPORT);
		nvp->InitializeViewport(this, tile, ZOOM_LVL_VIEWPORT);
		
		TerminalCompany = GetTileOwner(tile);
		//this->LowerWidget(this->TerminalCompany + WID_T_COMPANY_FIRST);
		
		this->owner = st->owner;
		
		this->OnInvalidateData();
	}
	
	~TerminalWindow()
	{
		//DeleteWindowById(WC_TERMINAL_VIEW, this->window_number, false);
	}
	
	virtual void SetStringParameters(int widget) const
	{
		if (widget != WID_T_CAPTION) return;
		SetDParam(0, VEH_AIRCRAFT);
		SetDParam(1, sID);
		//SetDParam(2, terminal);
	}
	
	virtual void OnClick(Point pt, int widget, int click_count)
	{
		switch (widget) {
			case WID_T_CENTER_VIEW: // scroll to location
				if (_ctrl_pressed) {
					ShowExtraViewPortWindow(this->window_number);
				} else {
					ScrollMainWindowToTile(this->window_number);
				}
				break;
			
			case WID_T_FULLLOAD: // Change Full Load restriction.
				DoCommandP(this->window_number, this->sID, 0, CMD_TERMINAL_CHANGE_FULLLOAD, NULL, NULL);
				this->InvalidateData();
				InvalidateWindowClassesData(WC_AIRPORT_CONTROL);
				break;
			
				//case WID_W_SHOW_VEHICLES: // show list of vehicles having this waypoint in their orders
				//	ShowVehicleListWindow(this->wp->owner, this->vt, this->wp->index);
				//	break;
		}
		
		/* Check which button is clicked */
		if (IsInsideMM(widget, WID_T_COMPANY_FIRST, WID_T_COMPANY_LAST + 1)) {
			/* Is it no on disable? */
			if (!this->IsWidgetDisabled(widget)) {
				if (_current_company == this->owner || this->TerminalCompany == OWNER_TOWN || _current_company == this->TerminalCompany) {
					this->RaiseWidget(this->TerminalCompany + WID_T_COMPANY_FIRST);
					this->TerminalCompany = (CompanyID)(widget - WID_T_COMPANY_FIRST);
					this->LowerWidget(this->TerminalCompany + WID_T_COMPANY_FIRST);
						// If Full load restrictions are enabled, disable them when switching owners.
					if (fullload & terminal)
						DoCommandP(this->window_number, this->sID, 0, CMD_TERMINAL_CHANGE_FULLLOAD, NULL, NULL);
						// Actually change the terminals owner.
					DoCommandP(this->window_number, this->TerminalCompany, 0, CMD_TERMINAL_CHANGE_OWNER, NULL, NULL);
					this->SetDirty();
					InvalidateWindowClassesData(WC_AIRPORT_CONTROL);
				}
			}
		}
	}
	
	/**
	 * Some data on this window has become invalid.
	 * @param data Information about the changed data.
	 * @param gui_scope Whether the call is done from GUI scope. You may not do everything when not in GUI scope. See #InvalidateWindowData() for details.
	 */
	virtual void OnInvalidateData(int data = 0, bool gui_scope = true)
	{
		if (!gui_scope) return;
		
		fullload = st->airport.fullload;
		terminal = st->airport.GetTerminalNum(this->window_number);
		
		//this->SetWidgetLoweredState(WID_T_FULLLOAD,fullload & terminal);
		
		if (fullload & terminal)
			this->LowerWidget(WID_T_FULLLOAD);
		else
			this->RaiseWidget(WID_T_FULLLOAD);
		
		/* Disable the companies who are not active.  It skips OWNER_TOWN. */
		for (CompanyID i = COMPANY_FIRST; i < MAX_COMPANIES; i++) {
			this->SetWidgetDisabledState(i + WID_T_COMPANY_FIRST, !Company::IsValidID(i));
			
			// If the current company does not own the station, then block other company selections.
			if (_current_company != this->owner && _current_company != TerminalCompany) {
				this->SetWidgetDisabledState(i + WID_T_COMPANY_FIRST, true);
			}
		}
		
		// If the current comapny is not the station owner and does not have this terminal, then disable the OWNER_TOWN button.
		if (_current_company != this->owner && _current_company != TerminalCompany && TerminalCompany != OWNER_TOWN)
			 this->SetWidgetDisabledState(MAX_COMPANIES + WID_T_COMPANY_FIRST, true);
		else
			this->SetWidgetDisabledState(MAX_COMPANIES + WID_T_COMPANY_FIRST, false);
		
		/* Check if the currently selected company is still active. Exclude OWNER_TOWN. */
		if (this->TerminalCompany != INVALID_COMPANY && this->TerminalCompany != OWNER_TOWN && !Company::IsValidID(this->TerminalCompany)) {
			/* Raise the widget for the previous selection. */
			this->RaiseWidget(this->TerminalCompany + WID_T_COMPANY_FIRST);
			this->TerminalCompany = this->owner;
		}
		
		// If the terminal is owned by OWNER_TOWN, then enable the local company.
		if (this->TerminalCompany == OWNER_TOWN)
			 this->SetWidgetDisabledState(_local_company + WID_T_COMPANY_FIRST, false);

		/* Make sure the widget is lowered */
		this->LowerWidget(this->TerminalCompany + WID_T_COMPANY_FIRST);
		
		ScrollWindowToTile(this->window_number, this, true);
	}
	
	virtual void OnResize()
	{
		if (this->viewport != NULL) {
			NWidgetViewport * nvp = this->GetWidget<NWidgetViewport>(WID_T_VIEWPORT);
			nvp->UpdateViewportCoordinates(this);
			//this->wp->UpdateVirtCoord();
			
			ScrollWindowToTile(this->window_number, this, true); // Re-center viewport.
		}
	}
	
	virtual void OnQueryTextFinished(char* str)
	{
		if (str == NULL) return;
		
		//DoCommandP(0, this->window_number, 0, CMD_RENAME_WAYPOINT | CMD_MSG(STR_ERROR_CAN_T_CHANGE_WAYPOINT_NAME), NULL, str);
	}
	
	virtual void DrawWidget(const Rect & r, int widget) const
	{
		if (IsInsideMM(widget, WID_T_COMPANY_FIRST, WID_T_COMPANY_LAST + 1)) {
			CompanyID cid = (CompanyID)(widget - WID_T_COMPANY_FIRST);
			if (Company::IsValidID(cid) || cid == OWNER_TOWN) {
				int offset = (cid == this->TerminalCompany) ? 1 : 0;
				Dimension sprite_size = GetSpriteSize(SPR_COMPANY_ICON);
				DrawCompanyIcon(cid, (r.left + r.right - sprite_size.width) / 2 + offset, (r.top + r.bottom - sprite_size.height) / 2 + offset);
			}
			return;
		}
	}
};

/** Make a number of rows with buttons for each company for the company selection window. */
NWidgetBase * MakeCompanyButtonRowsGraphGUITerminal(int* biggest_index)
{
	return MakeCompanyButtonRows(biggest_index, WID_T_COMPANY_FIRST, WID_T_COMPANY_LAST, 8, STR_TERMINAL_SELECT_COMPANY_TOOLTIP);
}

/** The widgets of the terminal view. */
static const NWidgetPart _nested_terminal_view_widgets[] = {
NWidget(NWID_HORIZONTAL),
NWidget(WWT_CLOSEBOX, COLOUR_GREY),
NWidget(WWT_CAPTION, COLOUR_GREY, WID_T_CAPTION), SetDataTip(STR_TERMINAL_VIEW_CAPTION, STR_TOOLTIP_WINDOW_TITLE_DRAG_THIS),
NWidget(WWT_SHADEBOX, COLOUR_GREY),
NWidget(WWT_DEFSIZEBOX, COLOUR_GREY),
NWidget(WWT_STICKYBOX, COLOUR_GREY),
EndContainer(),
NWidget(NWID_HORIZONTAL),
NWidget(WWT_TEXTBTN, COLOUR_GREY, WID_T_FULLLOAD), SetMinimalSize(100, 12), SetResize(1, 0), SetFill(1, 0), SetDataTip(STR_TERMINAL_FULLLOAD_BUTTON, STR_TERMINAL_VIEW_FULLLOAD_TOOLTIP),
EndContainer(),
NWidget(WWT_PANEL, COLOUR_GREY),
NWidget(WWT_INSET, COLOUR_GREY), SetPadding(2, 2, 2, 2),
NWidget(NWID_VIEWPORT, COLOUR_GREY, WID_T_VIEWPORT), SetMinimalSize(256, 88), SetPadding(1, 1, 1, 1), SetResize(1, 1),
EndContainer(),
EndContainer(),
NWidget(WWT_PANEL, COLOUR_GREY),
NWidgetFunction(MakeCompanyButtonRowsGraphGUITerminal), SetPadding(0, 1, 1, 2),
EndContainer(),
NWidget(NWID_HORIZONTAL),
NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, WID_T_CENTER_VIEW), SetMinimalSize(100, 12), SetResize(1, 0), SetFill(1, 0), SetDataTip(STR_BUTTON_LOCATION, STR_TERMINAL_VIEW_CENTER_TOOLTIP),
NWidget(WWT_RESIZEBOX, COLOUR_GREY),
EndContainer(),
};

/** The description of the terminal view. */
static WindowDesc _terminal_view_desc(
	WDP_AUTO, "view_terminal", 260, 118,
	WC_TERMINAL_VIEW, WC_NONE,
	0,
	_nested_terminal_view_widgets, lengthof(_nested_terminal_view_widgets)
);

/**
 * Show the window for the given terminal.
 * @param tile The terminal to show the window for.
 */
void ShowTerminalWindow(TileIndex tile)
{
	if (BringWindowToFrontById(WC_TERMINAL_VIEW, tile) != NULL) return;
	
	new TerminalWindow(&_terminal_view_desc, tile);
}
