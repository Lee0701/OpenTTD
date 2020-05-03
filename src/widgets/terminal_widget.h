/* $Id: terminal_widget.h 23600 2011-12-19 20:46:17Z Vetteman06 $ */

/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file terminal_widget.h Types related to the terminal widgets. */

#ifndef WIDGETS_TERMINAL_WIDGET_H
 #define WIDGETS_TERMINAL_WIDGET_H

/** Widgets of the #TerminalWindow class. */
enum TerminalWidgets {
	WID_T_CAPTION,       ///< Caption of window.
	WID_T_VIEWPORT,      ///< The viewport on this terminal.
	WID_T_CENTER_VIEW,   ///< Center the main view on this terminal.
	WID_T_FULLLOAD,		 ///< Full Load Button
	//WID_T_RENAME,        ///< Rename this waypoint.
	//WID_T_SHOW_VEHICLES, ///< Show the vehicles visiting this waypoint.
	
	WID_T_COMPANY_FIRST, ///< First company.
	WID_T_COMPANY_LAST = WID_T_COMPANY_FIRST + MAX_COMPANIES, ///< Last company.
};

#endif /* WIDGETS_TERMINAL_WIDGET_H */
