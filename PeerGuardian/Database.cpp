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
#include "Database.h"
#include <wx/ffile.h>
#include <wx/regex.h>
#include <wx/tokenzr.h>

extern wxString wxGetPath( const wxString& file );

Database::Database( MainFrame * frame ) : 
  m_frame(frame),
  m_bOk(false)
{
  Reload();
}

Database::~Database()
{
  for ( register int i = 0; i < ips.Count(); i++ )
    delete ips.Item(i);

  ips.clear();
}

Database_BlockItem Database::ShouldBeBlocked( const IP& ip )
{
  for ( register int i = 0; i < ips.Count(); i++ )
  {
    if ( ips.Item(i)->IsIPInRange(ip) )
      return Database_BlockItem(true, ips.Item(i)->GetName());
  }

  return Database_BlockItem();
}

ULONG Database::AmountOfIPs()
{
  ULONG aantal(0);
  for ( register int i = 0; i < ips.Count(); i++ )
    aantal += ips.Item(i)->AmountInRange();

  return aantal;
}

void Database::Reload( void )
{
  // delete all entries ...
  for ( register int i = 0; i < ips.Count(); i++ )
    delete ips.Item(i);

  ips.clear();

  // reread them...

  // IPs file:
  wxString IPfile( wxGetPath(DATABASE_FILE) );

  // don't do anything here ...
  if ( !wxFileExists( IPfile ) )
  {
    wxMessageBox( _("The PeerGuardian database cannot be located!"), _("Error"), wxICON_ERROR );
    return;
  }

  wxFFile ff( IPfile, wxT("r") );

  wxChar * buf = new wxChar[1024];
  wxRegEx regex( wxT("(.*):(.*)-(.*)") );
  IPRange * new_range;
  wxStringTokenizer tkz;
  wxString tk1, tk2, tk3;

  ULONG len;

  while ( !ff.Eof() )
  {
    len = wxStrlen( wxFgets( buf, 1024, ff.fp() ) ); if ( len > 0 ) len --;
    if ( buf[len] == '\n' )      buf[len--] = '\0';
    if ( buf[len] == '\r' )      buf[len--] = '\0';

    if ( buf[0] == '\0' )
      continue;

    tkz.SetString( buf, wxT(":-") );
    if ( tkz.CountTokens() == 3 )
    {
      tk1 = tkz.GetNextToken();
      tk2 = tkz.GetNextToken();
      tk3 = tkz.GetNextToken();

      new_range = new IPRange( tk1, tk2, tk3 );

      ips.Add( new_range );
    }
    else
    {
      // try with regex... It might just work!
      wxRegEx regex( wxT("(.*):(.*)-(.*)") );
      if ( regex.Matches(buf) )
      {
        new_range = new IPRange( regex.GetMatch(buf,1),
                                 regex.GetMatch(buf,2),
                                 regex.GetMatch(buf,3) );

        ips.Add( new_range );
      }
      else
      {
        wxLogDebug( _("Invalid string: '%s'"), buf );
      }
    }
  }

  ff.Close();

  delete [] buf;

  m_frame->ChangeRanges( AmountOfRanges() );
  m_frame->ChangeAddresses( AmountOfIPs() );

  m_bOk = true;
}