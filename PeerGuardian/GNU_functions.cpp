/*
'==================================================================================
'
'   PeerGuardian (IP Blocker to help protect P2P users)
'
'   Copyright 2003, 2004 - T. A. Leonard (Method/methlabs.org)
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
' tim.leonard1@ntlworld.com
' ICQ: 48546660
' http://methlabs.org http://xs.tech.nu
'
'=================================================================================




  *** Steven 'KaReL' Van Ingelgem ( @ 11.02.2004 ) ***
  The functions inside this file are taken from Tim 'method' Leonard his opensource
    PeerGuardian-tool.

  Therefor I am obliged to make those open-source too...
  *** Steven 'KaReL' Van Ingelgem ( @ 11.02.2004 ) ***

*/
#include <windows.h>
#include "iphlpapi/IPHlpApi.h"
#include <tchar.h>

#pragma comment( lib, "iphlpapi/IPHlpApi.Lib" )

#define MAX_TCP_ENTRIES       1000

PMIB_TCPTABLE GetTCPTable( )
{
  // MAX <MAX_TCP_ENTRIES> table-entries...
	DWORD tcpTableSize = sizeof(MIB_TCPTABLE) * MAX_TCP_ENTRIES;
	MIB_TCPTABLE *tcpTable = (MIB_TCPTABLE *)malloc(tcpTableSize);
	tcpTable->dwNumEntries = 0;

	GetTcpTable(tcpTable, &tcpTableSize, TRUE);

  return tcpTable;
}

void BlockConnection( PMIB_TCPROW row )
{
  row->dwState = MIB_TCP_STATE_DELETE_TCB;
  SetTcpEntry( row );
}

/*


  Source taken from:
  
    http://www.codeproject.com/useritems/wnetstat.asp?target=iphlpapi


  Slightly modified to be able to use UNICODE by:

    Steven 'KaReL' Van Ingelgem @ 11.02.2004

 */
#define MAX_IP_LENGTH         16 // 4*3+3+1

TCHAR* translateAddress(unsigned long addr)
{
  TCHAR * buf = new TCHAR[MAX_IP_LENGTH];

	_sntprintf(buf, MAX_IP_LENGTH, _T("%hi.%hi.%hi.%hi"), 
						 	((unsigned char *)&addr)[0],
						 	((unsigned char *)&addr)[1],
						 	((unsigned char *)&addr)[2],
						 	((unsigned char *)&addr)[3]);

  return buf;
}
