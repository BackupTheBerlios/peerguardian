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






























































#include "stdwx.h"
#include "Config.h"

#define READ_BOOL(name)                                               \
  if ( config->Read(wxString(wxT("/Options/")) + wxT(#name), &tmp) )  \
    m_b##name = (tmp != wxT("0"));                                    \
  else                                                                \
    m_b##name = false; // default set all == false


#define READ_STRING(name)                                             \
  if ( config->Read(wxString(wxT("/Options/")) + wxT(#name), &tmp) )  \
    m_sz##name = tmp;                                                 \
  else                                                                \
    m_sz##name = wxEmptyString;


#define READ_LONG(name)                                                            \
  m_l##name = config->Read(wxString(wxT("/Options/")) + wxT(#name), m_l##name );


extern wxString wxGetPath( const wxString& file );

Config::Config( ) : 
  config(NULL)
{
  bool m_bInitDefaults(false);
  wxString tmp( wxGetPath( wxT("PeerGuardian.ini") ) );

  m_bInitDefaults = !wxFileExists(tmp);

  config = new wxFileConfig( wxT("PeerGuardian"), wxT("KaReL"), tmp );


  READ_BOOL(BannedIPs);
  READ_BOOL(ConnectionLog);
  READ_BOOL(LiveWarnings);
  READ_BOOL(IncludePortNumbers);
  READ_BOOL(Priority); // true = normal, false = high
  READ_BOOL(AllowPortEighty);
  READ_BOOL(ShowCheckedIPs);
  READ_BOOL(LaunchMinimized);
  READ_BOOL(HideOnLaunch);
  READ_BOOL(AutoUpdate);
  READ_BOOL(AutoStart);
  READ_BOOL(ConfirmShutdown);
  READ_BOOL(PassShutdown);
  READ_STRING(PassToShutDown);
  READ_LONG(DbVersion);

  if ( m_bInitDefaults )
  {
    SetBannedIPs(true);
    SetAllowPortEighty(true);
    SetHideOnLaunch(true);
    SetLaunchMinimized(true);
    SetAutoUpdate(true);
    SetAutoStart(true);
    SetDbVersion(0);
  }
}

Config::~Config( )
{
  config->Flush();

  delete config;
}