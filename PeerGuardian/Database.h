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






























































#ifndef __DATABASE_H
#define __DATABASE_H

#include "MainFrame.h"
#include "IP.h"

WX_DEFINE_ARRAY_PTR(IPRange*, IP_Vector);

struct Database_BlockItem
{
  Database_BlockItem(bool blocked = NULL, wxString company = wxT("")) : m_blocked(blocked), m_company(company) { };
  Database_BlockItem(const Database_BlockItem& db): m_blocked(db.m_blocked), m_company(db.m_company) { };

  bool        m_blocked;
  wxString    m_company;
};

class Database
{
public:
  Database( MainFrame * frame );
  ~Database();

  Database_BlockItem ShouldBeBlocked( const IP& ip );
  inline bool IsOk() { return m_bOk; };

  inline ULONG AmountOfIPs();
  inline ULONG AmountOfRanges() { return ips.Count(); }

  void Reload( void );

private:
  IP_Vector   ips;
  MainFrame   *m_frame;
  bool        m_bOk;

  DECLARE_NO_COPY_CLASS(Database);
};

#endif //__DATABASE_H