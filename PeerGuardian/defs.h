/*
'==================================================================================
'
'   PeerGuardian++ (IP Blocker to help protect P2P users)
'
'   Copyright 2004 - Steven 'KaReL' Van Ingelgem
'
'    This program is free software; you can redistribute it and/or modify
'    it under the terms of the GNU General Public License as published by
'    the Free Software Foundation; either version 2 of the License, or
'    (at your option) any later version.
'
'    This program is distributed in the hope that it will be useful,
'    but WITHOUT ANY WARRANTY; without even the implied warranty of
'    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
'    GNU General Public License for more details.
'
'    You should have received a copy of the GNU General Public License
'    along with this program; if not, write to the Free Software
'    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
'
'
' karel@linux.be
' ICQ: 35217584
'
'=================================================================================
*/






























































#ifndef __DEF_H
#define __DEF_H

enum
{
  MAINFRAME_MENU__STATUS_WINDOW = wxID_HIGHEST + 1,
  MAINFRAME_MENU__CURRENT_BLOCKLIST,
  MAINFRAME_MENU__VIEW_LOG,

  MAINFRAME_MENU__OPTIONS,
  MAINFRAME_MENU__BLOCKLIST,
  MAINFRAME_MENU__IPTOOLS,

  MAINFRAME_MENU__UPDATES,
  MAINFRAME_MENU__PG_HOME,
  MAINFRAME_MENU__PG_MIRROR,
  MAINFRAME_MENU__METH_FORUM,
  MAINFRAME_MENU__PG_FORUM,
  MAINFRAME_MENU__PAYPAL,

  OPTIONS__BLOCK_BANNED_IP,
  OPTIONS__CONNECTION_LOG,
  OPTIONS__LIVE_WARNINGS,
  OPTIONS__INCLUDE_PORT_NUMBERS,
  OPTIONS__PRIORITY,
  OPTIONS__ALLOW_CONNECTION_TO_PORT_80,
  OPTIONS__SHOW_CHECKED_IPS,

  OPTIONS__LAUNCH_MINIMIZED,
  OPTIONS__HIDE_ON_LAUNCH,
  OPTIONS__AUTO_UPDATE,
  OPTIONS__LAUNCH_WITH_WINDOWS,

  OPTIONS__CONFIRM_SHUTDOWN,
  OPTIONS__PASSWORD_PROTECT_SHUTDOWN,
  OPTIONS__PASSWORD_FOR_SHUTDOWN,

  SOCKET_ID,
};

#define VERSION           wxT("PeerGuardian++ v2.00 pr2")
#define DATABASE_FILE     wxT("guardian.p2p")
#define CONNECTION_LOG    wxT("connlog.txt")

#endif