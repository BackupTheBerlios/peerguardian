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






























































#ifndef __STDWX_H__INCLUDED__
#define __STDWX_H__INCLUDED__

#pragma warning( disable: 4100 )
#pragma warning( disable: 4018 )

#include <wx/wxprec.h>
#include <wx/listctrl.h> // wxListView
#include <wx/ffile.h>    // wxFFile

// includes
#include "defs.h"
#include "config.h"
#include "Database.h"
#include "App.h"

// Variables
extern Config     * DefaultConfig;
extern Database   * IPDatabase;

// functions
extern MyApp& wxGetApp();
extern void ShowError( const wxString& errMsg );
extern wxString wxGetPath( const wxString& file );

#endif // __STDWX_H__INCLUDED__
