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
#include "Mainframe.h"
#include "App.h"
#ifdef _DEBUG
  #include <crtdbg.h>
#endif

IMPLEMENT_APP(MyApp)

#include "Blocker.h"
#include "AutoUpdate.h"

Blocker * MainBlocker;

bool IsStarting(false);

bool MyApp::OnInit()
{
  IsStarting = true;
  DefaultConfig = new Config();

  MainFrame *frame = new MainFrame( VERSION );


  bool m_IsUpdating(false);
  if ( DefaultConfig->GetAutoUpdate() || !wxFileExists( wxGetPath(DATABASE_FILE) ) )
  {
    UpdateThread * m_update_thread = new UpdateThread( frame );
    if ( m_update_thread->Create() == wxTHREAD_NO_ERROR )
    {
      m_update_thread->Run();
      m_IsUpdating = true;
    }
    else
      ShowError( _("Couldn't autoupdate the program!") );
  }

  // init after creating our main frame
  MainBlocker = new Blocker( frame );
  if ( MainBlocker->Create() == wxTHREAD_NO_ERROR )
  {
    MainBlocker->SetPriority( DefaultConfig->GetPriority()?WXTHREAD_MAX_PRIORITY:WXTHREAD_DEFAULT_PRIORITY );
    MainBlocker->Run();
  }
  else
  {
    ShowError( _("Couldn't start the main BlockThread") );
    return false;
  }

  SetTopWindow( frame );

  if ( !m_IsUpdating )
  {
    IPDatabase = new Database( frame );
    if ( !IPDatabase->IsOk() )
    {
      frame->Destroy( );
      return true;
    }
  }
  else
  {
    IPDatabase = NULL;
  }

  frame->AddToLog();

  if ( !m_IsUpdating )
  {
    frame->AddToLog( wxString::Format( _("Successfully loaded %lu IPs from %lu profiles."), IPDatabase->AmountOfIPs(), IPDatabase->AmountOfRanges() ) );
  }

  if ( !DefaultConfig->GetHideOnLaunch() )
  {
    frame->Show(true);
    if ( DefaultConfig->GetLaunchMinimized() )
      frame->Iconize( true );
  }

  IsStarting = false;
  return true;
}

int MyApp::OnExit()
{
  if ( IPDatabase ) 
    delete IPDatabase;

  delete DefaultConfig;
  delete MainBlocker;

  return 0;
}