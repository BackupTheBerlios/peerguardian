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
#include "IP.h"
#include <wx/regex.h>
#include <wx/tokenzr.h>

IP nul_ip( wxT("0.0.0.0") );
IP top_ip( wxT("255.255.255.255") );


IP::IP( const IP& ip ) :
  a(ip.a), b(ip.b),
  c(ip.c), d(ip.d),
  m_bOk( ip.m_bOk )
{
}

IP::IP( const wxString& szIP ) :
  a(0), b(0),
  c(0), d(0),
  m_bOk(false)
{
  wxStringTokenizer tkz( szIP, wxT(".") );
  if ( tkz.CountTokens() != 4 )
    return;

  long lConvert;
  wxString szConvert;

  tkz.GetNextToken().ToLong( &lConvert ); a = static_cast<int>(lConvert);
  tkz.GetNextToken().ToLong( &lConvert ); b = static_cast<int>(lConvert);
  tkz.GetNextToken().ToLong( &lConvert ); c = static_cast<int>(lConvert);
  tkz.GetNextToken().ToLong( &lConvert ); d = static_cast<int>(lConvert);

  // Now check if they are all >= 0 && <= 255
  m_bOk = ( a >= 0 && a <= 255 )
          &&
          ( b >= 0 && b <= 255 )
          &&
          ( c >= 0 && c <= 255 )
          &&
          ( d >= 0 && d <= 255 );
}

IP::~IP( )
{
}

// see if provided 'ip' > then the internal IP
bool IP::operator <( const IP& ip ) const
{
  if ( ip.a > a )
    return true;
  else if ( ip.a < a )
    return false;

  if ( ip.b > b )
    return true;
  else if ( ip.b < b )
    return false;

  if ( ip.c > c )
    return true;
  else if ( ip.c < c )
    return false;

  if ( ip.d > d )
    return true;
  else
    return false; // ip.d <= d
}

bool IP::operator ==( const IP& ip ) const
{
  return ip.a == a &&
         ip.b == b &&
         ip.c == c &&
         ip.d == d;
}

IP& IP::operator ++()
{
  if ( *this == top_ip )
    return *this;

  d++;
  if ( d > 255 )
  {
    d = 0;
    c++;
  }

  if ( c > 255 )
  {
    c = 0;
    b++;
  }

  if ( b > 255 )
  {
    b = 0;
    a++;
  }

  return *this;
}

IP& IP::operator --()
{
  if ( *this == nul_ip )
    return *this;

  d--;
  if ( d < 0 )
  {
    d = 255;
    c--;
  }

  if ( c < 0 )
  {
    c = 255;
    b--;
  }

  if ( b < 0 )
  {
    b = 255;
    a--;
  }

  return *this;
}

IP IP::operator -( const IP& ip ) const
{
  IP tmp(*this);

  tmp.a -= ip.GetA();
  tmp.b -= ip.GetB();
  tmp.c -= ip.GetC();
  tmp.d -= ip.GetD();

  if ( tmp.d < 0 )
  {
    tmp.c--;
    tmp.d += 256;
  }

  if ( tmp.c < 0 )
  {
    tmp.b--;
    tmp.c += 256;
  }

  if ( tmp.b < 0 )
  {
    tmp.a--;
    tmp.b += 256;
  }

  if ( tmp.a < 0 )
    return nul_ip;

  return tmp;
}

IP IP::operator +( const IP& ip ) const
{
  IP tmp(*this);

  tmp.a += ip.GetA();
  tmp.b += ip.GetB();
  tmp.c += ip.GetC();
  tmp.d += ip.GetD();

  if ( tmp.d > 255 )
  {
    tmp.c++;
    tmp.d -= 256;
  }

  if ( tmp.c > 255 )
  {
    tmp.b++;
    tmp.c -= 256;
  }

  if ( tmp.b > 255 )
  {
    tmp.a++;
    tmp.b -= 256;
  }

  if ( tmp.a > 255 )
    return top_ip;

  return tmp;
}