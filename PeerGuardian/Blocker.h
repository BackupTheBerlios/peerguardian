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






























































#ifndef __BLOCKER_H
#define __BLOCKER_H

#pragma warning( disable: 4100 )
#pragma warning( disable: 4786 )
#include <map>
#include <IPHlpApi.h>
#include "MainFrame.h"

struct BlockItem;
typedef std::map<DWORD, BlockItem*>             blocklist;
typedef std::map<DWORD, BlockItem*>::iterator   blocklist_ii;
typedef std::map<DWORD, DWORD>                  blocked_amounts;
typedef std::map<DWORD, DWORD>::iterator        blocked_amounts_ii;

struct BlockItem
{
  enum
  {
    not_checked,
    blocked,
    not_blocked
  } block_status;
  time_t     time_checked;
//  time_t     next_showtime;
  wxString   Company;
//  wxString  resolved_ip;
};

class Blocker : public wxThread
{
public:
  Blocker( MainFrame * frame );
  ~Blocker();

  void * Entry();
  void OnExit();

  static DWORD AmountOfBlocks();
  static void AddBlockedIP( DWORD dwLongIP );

private:
  void Notify( );

  void CheckTCPTable( );
  void CheckTCPRow( PMIB_TCPROW row );

  void BlockThisConnection( PMIB_TCPROW row );

  time_t        ltime;

  blocklist     m_list;
  blocklist_ii  ii;

  bool          m_bExiting;


  static blocked_amounts  m_amounts;
  static MainFrame        *m_frame;
  static bool             m_bAlreadyLoaded;

  DECLARE_NO_COPY_CLASS( Blocker );
};
#endif // __BLOCKER_H