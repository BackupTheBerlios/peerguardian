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
#include "AutoUpdate.h"
#include <wininet.h>
#pragma comment (lib, "wininet.lib")
#include <wx/regex.h>

wxString DownloadURL( const wxString& server, const wxString& path );

void* UpdateThread::Entry( )
{
  if ( m_bIsAutoUpdate )
  {
    m_frame->SetToolTip( _("Auto-Updating blocklist") );
  }
  else
  {
    m_frame->SetToolTip( _("Updating blocklist") );
    m_frame->AddToLog( _("Updating blocklist") );
  }

  //  1030
  //  http://homepage.ntlworld.com/tim.leonard1/pgupdate.htm?04-FEB-04
  wxString tmp = DownloadURL( wxT("homepage.ntlworld.com"), wxT("/tim.leonard1/pglistver.txt") );
//  wxTextInputStream( tmp );

  wxRegEx regex( wxT("([0-9]*)\r\n(.*)") );
  wxString  url;
  long      ver(-1);
  bool      bNeedReload;
  if ( regex.Matches(tmp) )
  {
    regex.GetMatch(tmp, 1).ToLong( &ver );
    url    = regex.GetMatch(tmp, 2); url.Trim();
  }

  bNeedReload = false;
  if ( ver == -1 )
  {
    if ( m_bIsAutoUpdate )
      ShowError( _("Unable to autoupdate!") );
    else
      ShowError( _("Unable to update!") );
  }
  else if ( !wxFileExists(wxGetPath(DATABASE_FILE)) || ver < DefaultConfig->GetDbVersion() )
  {
    if ( !m_bIsAutoUpdate )
      m_frame->AddToLog( _("New version detected... Download started...") );

    tmp = DownloadURL( wxT("homepage.ntlworld.com"), wxT("/tim.leonard1/guarding.p2p") );

    if ( !tmp.empty() )
    {
      wxFile fp( wxGetPath(DATABASE_FILE), wxFile::write );
      fp.Write( tmp.mb_str(), tmp.Length() );
      fp.Close();

      if ( !m_bIsAutoUpdate )
      {
        m_frame->AddToLog( _("Update saved... Reloading IP database...") );
      }
      bNeedReload = true;
    }
    else if ( m_bIsAutoUpdate )
    {
      bNeedReload = true;
    }
  }
  DefaultConfig->SetDbVersion( ver );

  if ( IPDatabase == NULL )
  {
    IPDatabase = new Database( m_frame );
    if ( !IPDatabase->IsOk() )
    {
      ShowError( _("Invalid database-file!") );
      m_frame->Destroy( );
      return NULL;
    }
    m_frame->AddToLog( wxString::Format( _("Successfully loaded %lu IPs from %lu profiles."), IPDatabase->AmountOfIPs(), IPDatabase->AmountOfRanges() ) );
  }
  else if ( bNeedReload )
  {
    IPDatabase->Reload();
    m_frame->AddToLog( wxString::Format( _("Successfully loaded %lu IPs from %lu profiles."), IPDatabase->AmountOfIPs(), IPDatabase->AmountOfRanges() ) );
  }
  else
  {
    m_frame->AddToLog( _("You already have the latest version!") );
  }

  m_frame->SetToolTip( VERSION );
  return NULL;
}

#define AGENT             wxT("HTTP/1.1")
#define USER_AGENT        wxT("Mozilla/4.0 (compatible; MSIE 5.0; Windows NT; DigExt; DTS Agent;)")
#define DEFAULT_HEADERS   wxT("Accept-Language: nl-be\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\nAccept: */*\r\n")

wxString DownloadURL( const wxString& server, const wxString& path )
{
  HINTERNET hInternetOpen = 0;
  HINTERNET hInternetConnect = 0;
  HINTERNET hHttpOpenRequest = 0;

  char sReadBuffer[2048] = "";
  DWORD dwLengthSizeBuffer = sizeof(sReadBuffer);

  long lRetVal(0), bRet(0), bDoLoop(1);
  ULONG lNumberOfBytesRead(0);
  wxString tmp, sBuffer;

  if (!InternetGetConnectedState(0, 0))
    return wxT("");

  hInternetOpen = InternetOpen(USER_AGENT, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if( hInternetOpen )
  {

    hInternetConnect = InternetConnect(hInternetOpen, server, INTERNET_DEFAULT_HTTP_PORT, NULL, AGENT, INTERNET_SERVICE_HTTP, 0, 0);
    if ( hInternetConnect )
    {

      hHttpOpenRequest = HttpOpenRequest(hInternetConnect, wxT("GET"), path, AGENT, NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, 0);
      if ( hHttpOpenRequest )
      {
        long tmpVar;
        tmpVar = 10000; lRetVal = InternetSetOption(hHttpOpenRequest, INTERNET_OPTION_CONNECT_TIMEOUT, &tmpVar, 4);
        tmpVar = 15000; lRetVal = InternetSetOption(hHttpOpenRequest, INTERNET_OPTION_RECEIVE_TIMEOUT, &tmpVar, 4);
        tmpVar = 20000; lRetVal = InternetSetOption(hHttpOpenRequest, INTERNET_OPTION_SEND_TIMEOUT, &tmpVar, 4);

        bRet = HttpAddRequestHeaders(hHttpOpenRequest, DEFAULT_HEADERS, wxStrlen(DEFAULT_HEADERS), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
        bRet = HttpSendRequest(hHttpOpenRequest, NULL, 0, 0, 0);

        while ( bDoLoop ) {
          sReadBuffer[0] = 0;

          bDoLoop = InternetReadFile(hHttpOpenRequest, sReadBuffer, dwLengthSizeBuffer, &lNumberOfBytesRead);

          if ( lNumberOfBytesRead > 0 )
          {
            tmp = wxString(sReadBuffer, wxConvLibc);
            tmp.Truncate(lNumberOfBytesRead);

            sBuffer += tmp;
          }
          else
          {
            bDoLoop = 0;
          }
        }

        bRet = InternetCloseHandle(hHttpOpenRequest);
      } // if ( hHttpOpenRequest )

      bRet = InternetCloseHandle(hInternetConnect);
    } // if ( hInternetConnect )

    bRet = InternetCloseHandle(hInternetOpen);
  } // if( hInternetOpen )


  return sBuffer;
}