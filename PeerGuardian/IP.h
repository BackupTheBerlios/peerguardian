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






























































#ifndef __IP_H
#define __IP_H

class IP
{
public:
  IP( const wxString& ip );
  IP( const IP& ip );
  ~IP( );

  inline bool IsOK() const { return m_bOk; }

  inline int  GetA() const { return a; }
  inline int  GetB() const { return b; }
  inline int  GetC() const { return c; }
  inline int  GetD() const { return d; }

  inline bool operator > ( const IP& ip ) const { return !operator <(ip) && operator !=(ip); }
  inline bool operator !=( const IP& ip ) const { return !operator ==(ip); }
  bool operator < ( const IP& ip ) const;
  bool operator ==( const IP& ip ) const;

  IP& operator ++();
  IP& operator --();

  inline IP& operator ++(int) { return operator ++(); }
  inline IP& operator --(int) { return operator --(); }

  IP operator -( const IP& ip ) const;
  IP operator +( const IP& ip ) const;

  operator wxString() { wxString tmp; tmp << a << wxT(".") << b << wxT(".") << c << wxT(".") << d; return tmp; }

private:
  int a,b,c,d;
  bool  m_bOk;
};

class IPRange
{
public:
  IPRange( const IP& ip1, const IP& ip2 ) : m_ip1(ip1), m_ip2(ip2) { };
  IPRange( const wxString name, const IP& ip1, const IP& ip2 ) : m_name(name), m_ip1(ip1), m_ip2(ip2) { };

  bool IsIPInRange( const IP& ip )
  {
    if ( !ip.IsOK() ||
         !m_ip1.IsOK() ||
         !m_ip2.IsOK() )
      return false; // If one of our IP's is false --> quit!

    if ( ip == m_ip1 || ip == m_ip2 )
      return true;

    return ip > m_ip1 && ip < m_ip2;
  }

  ULONG AmountInRange()
  {
    if ( m_ip2 < m_ip1 )
      return 0;
    else if ( m_ip1 == m_ip2 )
      return 1;

    IP tmp( m_ip2 - m_ip1 );

    if ( !tmp.IsOK() )
      return 0;

    return
      (tmp.GetD())                  + 
      (tmp.GetC() * 256)            + 
      (tmp.GetB() * 256 * 256)      +
      (tmp.GetA() * 256 * 256 * 256);
  }

  const IP& GetIP1() const { return m_ip1; }
  const IP& GetIP2() const { return m_ip2; }
  const wxString& GetName() const { return m_name; }

private:
  IP m_ip1,
    m_ip2;
  wxString m_name;
};

#endif // __IP_H