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
#include "blocker.h"
#include <time.h>

extern PMIB_TCPTABLE GetTCPTable( );
extern TCHAR* translateAddress(unsigned long addr);
extern void BlockConnection( PMIB_TCPROW row );


#define DEFAULT_CHECK_TIME                250
#define DEFAULT_TIME_BETWEEN_SAME_SHOWS   15


Blocker::Blocker( MainFrame * frame ) : m_bExiting(false)
{
  if ( m_bAlreadyLoaded )
  {
    ShowError( _("BlockerThread has been loaded twice!") );
    throw( _("BlockerThread has been loaded twice!") );
    return;
  }

  m_frame = frame;

  _tzset();
}


void Blocker::OnExit( )
{
  m_bExiting = true;
}

Blocker::~Blocker( )
{
  m_bExiting = true;

  for ( ii = m_list.begin(); ii != m_list.end(); ii++ )
    delete ii->second;

  m_list.clear();
}

void * Blocker::Entry( )
{
  while ( !m_bExiting )
  {
    Notify();

    if ( TestDestroy() )
      break;

    wxThread::Sleep(DEFAULT_CHECK_TIME);
    Yield();
  }

  return NULL;
}

void Blocker::Notify( )
{
  static UINT amount_of_loops(0);
  if ( !DefaultConfig->GetBannedIPs() ) // Do not block IPs --> Do nothing (quit)
    return;
  else if ( IPDatabase == NULL || !IPDatabase->IsOk() )
    return;

  time( &ltime );

  CheckTCPTable( );

  if ( TestDestroy() )
    return;

  if ( amount_of_loops > (60 * (1000/DEFAULT_CHECK_TIME)) ) // clear m_list every minute!
  {
    BlockItem * new_item;

    for ( ii = m_list.begin(); ii != m_list.end(); ii++ )
    {
      new_item = ii->second;
      if ( new_item->time_checked < ltime - 120 ) // clear everything not used in the last minute (helps reducing the amount of mem used)
      {
        delete ii->second;
        m_list.erase(ii);
      }
    }
    amount_of_loops = 0;
  }

  amount_of_loops++;
}

void Blocker::CheckTCPRow( PMIB_TCPROW row )
{
  if ( row->dwRemoteAddr == NULL ) // we should NEVER be here!
    return;

  if ( TestDestroy() )
    return;

  // static so they don't need to be recreated everytime (more performance)
  static USHORT remotePort/*, localPort*/;
  static wxString RemoteAddr/*, LocalAddr*/;
  // LocalAddr  = resolveAddress(row->dwLocalAddr, FALSE);
  register TCHAR * tmpRemAddr;


	remotePort = ntohs((unsigned short)(row->dwRemotePort & 0xFFFF));

  tmpRemAddr = translateAddress(row->dwRemoteAddr);
  RemoteAddr = tmpRemAddr;
  delete [] tmpRemAddr;

  if ( DefaultConfig->GetIncludePortNumbers() )
  {
    RemoteAddr.Append( wxT(":") );
    wxChar buf[6];
    memset( buf, 0, sizeof(wxChar)*6 );
    _ltot( remotePort, buf, 10 );
    RemoteAddr.Append( buf );
  }

  if ( remotePort == 80 && DefaultConfig->GetAllowPortEighty() ) // we do allow all connections outwards to port 80 ... so quit right away...
  {
    RemoteAddr.Prepend( _("Not checked (port 80): ") );

    if ( DefaultConfig->GetConnectionLog() )
      m_frame->AddToConnectionLog( RemoteAddr );

    if ( DefaultConfig->GetShowCheckedIPs() )
      m_frame->AddToLog( RemoteAddr );

    return;
  }

  if ( DefaultConfig->GetConnectionLog() )
    m_frame->AddToConnectionLog( _("Checking: ") + RemoteAddr );

  if ( DefaultConfig->GetShowCheckedIPs() )
    m_frame->AddToLog( _("Checking: ") + RemoteAddr );

  ii = m_list.find( row->dwRemoteAddr );
  BlockItem * new_item;
  DWORD longIP;
  static wxString bah;
  register wxString& company_name = bah;

  if ( ii == m_list.end() )
  { // not in the list -> add it...
    new_item = new BlockItem();
    longIP = row->dwRemoteAddr;

    Database_BlockItem item = IPDatabase->ShouldBeBlocked( RemoteAddr );
    company_name = item.m_company;

    new_item->block_status  = item.m_blocked?BlockItem::blocked:BlockItem::not_blocked;
    new_item->Company       = item.m_company;
//    new_item->next_showtime = ltime - 1;

    m_list[ longIP ] = new_item;
  }
  else
  { // Check if I need to block it ...
    new_item = ii->second;
    longIP = ii->first;
    if ( new_item->block_status == BlockItem::blocked )
      company_name = new_item->Company;
  }
  // update the time...
  new_item->time_checked = ltime;

  if ( new_item->block_status == BlockItem::blocked )
  {
    BlockConnection( row );

    if ( DefaultConfig->GetConnectionLog() )
      m_frame->AddToConnectionLog( _("Rejected: ") + RemoteAddr  );

    if ( DefaultConfig->GetLiveWarnings() )
    {
      // **TODO** Show window with warning...
    }

    Blocker::AddBlockedIP( longIP );
//    if ( new_item->next_showtime < ltime )
    {
      m_frame->AddToLog( _("Rejected: ") + RemoteAddr + wxT("  ") + company_name );
//      new_item->next_showtime = ltime + DEFAULT_TIME_BETWEEN_SAME_SHOWS;
    }
  }
}

void Blocker::CheckTCPTable( )
{
  register DWORD i(0);

  PMIB_TCPTABLE tcpTable = GetTCPTable();

  for ( i = 0; i < tcpTable->dwNumEntries; i++ )
  {
    if ( tcpTable->table[i].dwRemoteAddr == NULL ) // why do we check this?
      continue;

    CheckTCPRow( &(tcpTable->table[i]) );

    if ( TestDestroy() )
      break;
  }

	free(tcpTable);
}

/*

  Static functions and so on...

 */

blocked_amounts Blocker::m_amounts;
bool Blocker::m_bAlreadyLoaded = false;
MainFrame *Blocker::m_frame = NULL;

void Blocker::AddBlockedIP( DWORD dwLongIP )
{
  blocked_amounts_ii ii = m_amounts.find( dwLongIP );
  if ( ii == m_amounts.end() )
  {
    m_amounts[ dwLongIP ] = 1;
    m_frame->ChangeBlocked( AmountOfBlocks() );
  }
  else
  {
    m_amounts[ dwLongIP ] = ii->second + 1;
  }
}

DWORD Blocker::AmountOfBlocks( )
{
  return m_amounts.size();
}