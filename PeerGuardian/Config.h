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






























































#ifndef __CONFIG_H
#define __CONFIG_H

#include <wx/fileconf.h>


#define ADD_BOOL(name)                                              \
public:                                                             \
  bool Get##name ( ) const { return m_b##name ; };                  \
  void Set##name ( bool mVal )                                      \
  {                                                                 \
    m_b##name = mVal;                                               \
    wxString str( mVal?wxT("1"):wxT("0") );                         \
    config->Write( wxString(wxT("/Options/")) + wxT(#name), str );  \
  };                                                                \
                                                                    \
private:                                                            \
  bool m_b##name


#define ADD_STRING(name)                                            \
public:                                                             \
  const wxString& Get##name ( ) const { return m_sz##name ; };      \
  void Set##name ( const wxString& mVal )                           \
  {                                                                 \
    m_sz##name = mVal;                                              \
    config->Write( wxString(wxT("/Options/")) + wxT(#name), mVal ); \
  };                                                                \
                                                                    \
private:                                                            \
  wxString m_sz##name


#define ADD_LONG(name)                                              \
public:                                                             \
  long Get##name ( ) const { return m_l##name ; };                  \
  void Set##name ( long mVal )                                      \
  {                                                                 \
    m_l##name = mVal;                                               \
    config->Write( wxString(wxT("/Options/")) + wxT(#name), mVal ); \
  };                                                                \
                                                                    \
private:                                                            \
  long m_l##name

class Config
{
public:
  Config( );
  ~Config( );

  ADD_BOOL(BannedIPs);
  ADD_BOOL(ConnectionLog);
  ADD_BOOL(LiveWarnings);
  ADD_BOOL(IncludePortNumbers);
  ADD_BOOL(Priority); // false = normal, true = high
  ADD_BOOL(AllowPortEighty);
  ADD_BOOL(ShowCheckedIPs);
  ADD_BOOL(LaunchMinimized);
  ADD_BOOL(HideOnLaunch);
  ADD_BOOL(AutoUpdate);
  ADD_BOOL(AutoStart);
  ADD_BOOL(ConfirmShutdown);
  ADD_BOOL(PassShutdown);
  ADD_STRING(PassToShutDown);
  ADD_LONG(DbVersion);


private:
  wxFileConfig * config;

  DECLARE_NO_COPY_CLASS(Config)
};

#endif // __CONFIG_H