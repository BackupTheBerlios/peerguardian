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
#include <wx/taskbar.h>
#include <wx/datetime.h>
#include "AutoUpdate.h"
#include "Blocker.h"
#ifdef __WINDOWS__
  #include <wx/msw/regconf.h>
#endif

extern bool IsStarting;
extern wxString wxGetExecutablePath();


#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
  #include "pg.xpm"
  #include "PG_Logo.xpm"
#endif

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  // 1st menu
  EVT_MENU    ( MAINFRAME_MENU__STATUS_WINDOW,  MainFrame::OnMenu_StatusWindow  )
  EVT_MENU    ( MAINFRAME_MENU__VIEW_LOG,       MainFrame::OnMenu_ViewLog       )

  // 2nd menu
  EVT_MENU    ( MAINFRAME_MENU__OPTIONS,        MainFrame::OnMenu_Option        )

  // 3th menu
  EVT_MENU    ( MAINFRAME_MENU__UPDATES,        MainFrame::OnMenu_Website       )
  EVT_MENU    ( MAINFRAME_MENU__PG_HOME,        MainFrame::OnMenu_Website       )
  EVT_MENU    ( MAINFRAME_MENU__PG_MIRROR,      MainFrame::OnMenu_Website       )
  EVT_MENU    ( MAINFRAME_MENU__METH_FORUM,     MainFrame::OnMenu_Website       )
  EVT_MENU    ( MAINFRAME_MENU__PG_FORUM,       MainFrame::OnMenu_Website       )
  EVT_MENU    ( MAINFRAME_MENU__PAYPAL,         MainFrame::OnMenu_Website       )

  // others
  EVT_CHECKBOX( wxID_ANY,                       MainFrame::OnCheckBoxClicked    )
  EVT_SIZE    (                                 MainFrame::OnSize               )
  EVT_ICONIZE (                                 MainFrame::OnIconize            )
END_EVENT_TABLE()


// frame constructor
MainFrame::MainFrame(const wxString& title) : 
  wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(421, 397) ),
  lv( NULL ),
  text( NULL ),
  Options( NULL ),
  StatusWindow( NULL ),
  taskbar( NULL ),
  connlog( wxGetPath( CONNECTION_LOG ), wxT("a") )
{
  // set the frame icon
  SetIcon(wxICON(PeerGuardian));

  CreateMenuBar();
  CreateStatusWindow();

  // Add default text
  AddDefaultText();

  // Generate the task-icon
  CreateTaskBarIcon();
}

MainFrame::~MainFrame( )
{
  DeleteTaskBarIcon();
}

void MainFrame::CreateMenuBar( )
{
  wxMenuBar * m_pMenuBar = new wxMenuBar();
  wxMenu * m_Menu, *m_SubMenu;

  m_Menu = new wxMenu();
    m_Menu->Append( MAINFRAME_MENU__STATUS_WINDOW,    _("&Status Window") );
    m_Menu->Append( MAINFRAME_MENU__CURRENT_BLOCKLIST,_("&Current Blocklist") );
    m_Menu->AppendSeparator();
    m_Menu->Append( MAINFRAME_MENU__VIEW_LOG,         _("&View Log") );
  m_pMenuBar->Append( m_Menu,                         _("&View") );

  m_Menu = new wxMenu();
    m_Menu->Append( MAINFRAME_MENU__OPTIONS,          _("&Options") );
    m_Menu->Append( MAINFRAME_MENU__BLOCKLIST,        _("&Blocklist") );
    m_Menu->AppendSeparator();
    m_Menu->Append( MAINFRAME_MENU__IPTOOLS,          _("&IP Tools") );
  m_pMenuBar->Append( m_Menu,                         _("&Tools") );

  m_Menu = new wxMenu();
    m_Menu->Append( MAINFRAME_MENU__UPDATES,          _("&Check for blocklist updates") );
    m_Menu->AppendSeparator();
    m_SubMenu = new wxMenu();
      m_SubMenu->Append( MAINFRAME_MENU__PG_HOME,     _("&PeerGuardian Homepage (http://methlabs.org)") );
      m_SubMenu->Append( MAINFRAME_MENU__PG_MIRROR,   _("P&eerGuardian Mirror (http://xs.tech.nu)") );
      m_SubMenu->AppendSeparator();
      m_SubMenu->Append( MAINFRAME_MENU__METH_FORUM,  _("&Methlabs.org Forums") );
      m_SubMenu->Append( MAINFRAME_MENU__PG_FORUM,    _("Pee&rGuardian.net Forums") );
    m_Menu->Append( wxID_ANY,                         _("Websites/Forums"), m_SubMenu );
    m_Menu->AppendSeparator();
    m_SubMenu = new wxMenu();
      m_SubMenu->Append( MAINFRAME_MENU__PAYPAL,      _("&PayPal") );
    m_Menu->Append( wxID_ANY,                         _("Donations"), m_SubMenu );
  m_pMenuBar->Append( m_Menu,                         _("&Help") );

  SetMenuBar(m_pMenuBar); 
}

void MainFrame::CreateStatusWindow()
{
  if ( Options )
  {
    Options->Destroy();
    Options = NULL;
  }

  wxASSERT( StatusWindow == NULL );

  // Main painting thingie...
  StatusWindow = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxSUNKEN_BORDER );

  // Bitmap
  wxStaticBitmap * bmp = new wxStaticBitmap( StatusWindow, wxID_ANY, wxBITMAP(PG_LOGO), wxDefaultPosition, wxSize(-1,48), wxSUNKEN_BORDER );
  // listview
  lv = new wxListView( StatusWindow, wxID_ANY, wxDefaultPosition, wxSize(292, 48), wxLC_REPORT|wxLC_NO_HEADER );
  // text-box...
  text = new wxTextCtrl( StatusWindow, wxID_ANY, m_szStatusText, wxDefaultPosition, wxSize(396,277), wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH|wxTE_WORDWRAP );

  // auto-magical resizing
  wxBoxSizer * box = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer * box2 = new wxBoxSizer( wxHORIZONTAL );

    box2->Add( bmp, 0, wxLEFT|wxTOP, 4 );
    box2->Add( lv, 1, wxEXPAND|wxALL, 4 );

  box->Add( box2, 0, wxEXPAND );
  box->Add( text, 1, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 4 );

  // Setup our listview
  SetupListView();

  StatusWindow->SetSizer( box );
  box->SetSizeHints( StatusWindow );
}

class MyStaticText : public wxStaticText
{
public:
    MyStaticText(MainFrame * upper_frame,
                 wxWindow *parent,
                 wxWindowID id,
                 const wxString& label,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxStaticTextNameStr) :
         wxStaticText(parent, id, label, pos, size, style, name),
         m_parent( upper_frame )
    { }
protected:
  void OnLeftClick( wxMouseEvent& WXUNUSED(event) )
  {
    if ( m_parent )
      m_parent->SwapPriority();
  }

private:
  MainFrame *m_parent;

  DECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(MyStaticText, wxStaticText)
  EVT_LEFT_UP( MyStaticText::OnLeftClick )
END_EVENT_TABLE();


void MainFrame::CreateOptionsWindow()
{
  if ( StatusWindow )
  {
    // save our text...
    m_szStatusText = text->GetValue();
    StatusWindow->Destroy();
    StatusWindow = NULL;
  }

  wxASSERT( Options == NULL );

  Options = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxSUNKEN_BORDER );

  wxFont f;
  f.SetWeight(wxBOLD);

  wxStaticBox *statbox;
  wxCheckBox *chk;

  // Part 1
  statbox = new wxStaticBox( Options, wxID_ANY, _("Blocking/Logging Options"), wxPoint(4, 4), wxSize(401, 117) );
  statbox->SetFont( f );
  
  chk = new wxCheckBox( Options, OPTIONS__BLOCK_BANNED_IP, _("Block Banned IPs."), wxPoint(12, 20),  wxSize(124, 21), wxCHK_2STATE|wxSUNKEN_BORDER );
  chk->SetValue( DefaultConfig->GetBannedIPs() );
  chk = new wxCheckBox( Options, OPTIONS__CONNECTION_LOG,  _("Connection Log"),    wxPoint(142, 20), wxSize(124, 21), wxCHK_2STATE|wxSUNKEN_BORDER );
  chk->SetValue( DefaultConfig->GetConnectionLog() );
  chk = new wxCheckBox( Options, OPTIONS__LIVE_WARNINGS,   _("Live Warnings"),     wxPoint(272, 20), wxSize(124, 21), wxCHK_2STATE|wxSUNKEN_BORDER );
  chk->SetValue( DefaultConfig->GetLiveWarnings() );

  chk = new wxCheckBox( Options, OPTIONS__INCLUDE_PORT_NUMBERS, _("Include Port Numbers On Connection Log"), wxPoint(12, 44), wxSize(248, 21), wxCHK_2STATE|wxSUNKEN_BORDER );
  chk->SetValue( DefaultConfig->GetIncludePortNumbers() );
  new MyStaticText( this, Options, OPTIONS__PRIORITY, _("Priority:"),  wxPoint(272, 47), wxSize(124, 21) /*, wxSUNKEN_BORDER*/ );
  if ( !DefaultConfig->GetPriority() )
    priority = new MyStaticText( this, Options, OPTIONS__PRIORITY, _("NORMAL"), wxPoint(315, 47) ); // of _("HIGH");
  else
    priority = new MyStaticText( this, Options, OPTIONS__PRIORITY, _("HIGH"), wxPoint(315, 47) ); // of _("HIGH");
  priority->SetFont( f );
  priority->SetSize(wxSize(124, 21));

  chk = new wxCheckBox( Options, OPTIONS__ALLOW_CONNECTION_TO_PORT_80, _("Allow All Connections To Destination Port 80"), wxPoint(12, 68), wxSize(383, 21), wxCHK_2STATE|wxSUNKEN_BORDER );
  chk->SetValue( DefaultConfig->GetAllowPortEighty() );

  chk = new wxCheckBox( Options, OPTIONS__SHOW_CHECKED_IPS, _("Show IPs Checked In Status Window"), wxPoint(12, 92), wxSize(383, 21), wxCHK_2STATE|wxSUNKEN_BORDER );
  chk->SetValue( DefaultConfig->GetShowCheckedIPs() );

  // Part 2
  statbox = new wxStaticBox( Options, wxID_ANY, _("Launch Options"), wxPoint(4, 124), wxSize(401, 69) );
  statbox->SetFont( f );

  chk = new wxCheckBox( Options, OPTIONS__LAUNCH_MINIMIZED, _("Launch Minimized"), wxPoint(12, 140), wxSize(124, 21), wxCHK_2STATE|wxSUNKEN_BORDER );
  chk->SetValue( DefaultConfig->GetLaunchMinimized() );
  chk = new wxCheckBox( Options, OPTIONS__HIDE_ON_LAUNCH, _("Hide On Launch"), wxPoint(142, 140), wxSize(124, 21), wxCHK_2STATE|wxSUNKEN_BORDER );
  chk->SetValue( DefaultConfig->GetHideOnLaunch() );
  chk = new wxCheckBox( Options, OPTIONS__AUTO_UPDATE, _("Auto-Update"), wxPoint(272, 140), wxSize(124, 21), wxCHK_2STATE|wxSUNKEN_BORDER );
  chk->SetValue( DefaultConfig->GetAutoUpdate() );

  chk = new wxCheckBox( Options, OPTIONS__LAUNCH_WITH_WINDOWS, _("Launch PeerGuardian Automatically When Windows Starts"), wxPoint(12, 164), wxSize(383, 21), wxCHK_2STATE|wxSUNKEN_BORDER );
  chk->SetValue( DefaultConfig->GetAutoStart() );


  // Part 3
  statbox = new wxStaticBox( Options, wxID_ANY, _("Shutdown Options"), wxPoint(4, 198), wxSize(401, 69) );
  statbox->SetFont( f );

  chk = new wxCheckBox( Options, OPTIONS__CONFIRM_SHUTDOWN, _("Confirmation Pop-up Window When Shutting Down"), wxPoint(12, 212), wxSize(383, 21), wxCHK_2STATE|wxSUNKEN_BORDER );
  chk->SetValue( DefaultConfig->GetConfirmShutdown() );
  chk = new wxCheckBox( Options, OPTIONS__PASSWORD_PROTECT_SHUTDOWN, wxString(_("Password Protected Shutdown")) + wxT(" ---------->"), wxPoint(12, 236), wxSize(248, 21), wxCHK_2STATE|wxSUNKEN_BORDER );
  chk->SetValue( DefaultConfig->GetPassShutdown() );

  password = new wxTextCtrl( Options, OPTIONS__PASSWORD_FOR_SHUTDOWN, wxEmptyString, wxPoint(272, 236), wxSize(124, 21), wxTE_PASSWORD );
  password->SetValue( DefaultConfig->GetPassToShutDown() );
  password->Enable( DefaultConfig->GetPassShutdown() );

//  SetSizer( NULL );
}

void MainFrame::OnMenu_Option( wxCommandEvent& WXUNUSED(event) )
{
  CreateOptionsWindow();
  SendSizeEvent();
}

void MainFrame::OnMenu_StatusWindow( wxCommandEvent& WXUNUSED(event) )
{
  CreateStatusWindow();
  SendSizeEvent();
}

void MainFrame::OnIconize( wxIconizeEvent& event )
{
  if ( event.Iconized() )
    Show(FALSE);
}

void MainFrame::OnSize( wxSizeEvent& event )
{

  static int default_width (421),
             default_height(397);
  static wxSize size(default_width, default_height);

  wxSize newSize( event.GetSize() );

  if ( newSize.GetWidth() < default_width )
    newSize.SetWidth( default_width );

  if ( newSize.GetHeight() < default_height )
    newSize.SetHeight( default_height );

  if ( newSize != event.GetSize() )
    SetSize( newSize );
  else // Let the sizers take control!
    event.Skip();
}

void MainFrame::SwapPriority( void )
{
  if ( priority->GetLabel() == _("NORMAL") ) // swap to high...
  {
    priority->SetLabel( _("HIGH") );
    DefaultConfig->SetPriority( true );
  }
  else
  {
    priority->SetLabel( _("NORMAL") );
    DefaultConfig->SetPriority( false );
  }
}

void MainFrame::AddToLog( const wxString& text_to_add, bool show_time )
{
  static wxString tmp;
  if ( show_time )
    tmp = wxDateTime::Now().Format( wxT("%Y-%m-%d %H:%M:%S: ") ) + text_to_add;
  else
    tmp = text_to_add;

  if ( StatusWindow )
    text->AppendText( tmp + wxT("\n") );
  else
    m_szStatusText.Append( tmp + wxT("\n") );
}

void MainFrame::AddToConnectionLog( const wxString& text_to_add, bool show_time )
{
  if ( !DefaultConfig->GetConnectionLog() )
    return; // Don't save in non-connectionlog mode

  static wxString tmp;
  if ( show_time )
    connlog.Write( wxDateTime::Now().Format( wxT("%Y-%m-%d %H:%M:%S: ") ) + text_to_add + wxT("\r\n") );
  else
    connlog.Write( text_to_add + wxT("\r\n") );
}

void MainFrame::AddDefaultText( void )
{
  AddToLog( VERSION );
  AddToLog( wxT("http://xs.tech.nu") );
  AddToLog( wxT("http://methlabs.org") );
  AddToLog( wxT("http://peerguardian.net") );
  AddToLog();
  AddToLog( _("UDP support for XP soon.") );
  AddToLog( _("Check Tools Menu for Blocklist Manager") );
}

void MainFrame::ChangeRanges( ULONG amount_of_ranges )
{
  wxString tmp; tmp << amount_of_ranges;

  wxFont font;
//  font.SetWeight( wxBOLD );

  wxListItem item;
  item.SetAlign(wxLIST_FORMAT_RIGHT);
  item.SetText( tmp );
  item.SetFont( font );
  item.SetColumn( 1 );
  item.SetId( 0 );

  lv->SetItem(item);
}

void MainFrame::ChangeAddresses( ULONG amount_of_ranges )
{
  wxString tmp; tmp << amount_of_ranges;

  wxFont font;
//  font.SetWeight( wxBOLD );

  wxListItem item;
  item.SetAlign(wxLIST_FORMAT_RIGHT);
  item.SetText( tmp );
  item.SetFont( font );
  item.SetColumn( 1 );
  item.SetId( 1 );
  item.SetBackgroundColour( wxColour(240,240,240) );

  lv->SetItem(item);
}

void MainFrame::ChangeBlocked( ULONG amount_of_blocks )
{
  wxString tmp; tmp << amount_of_blocks;

  wxFont font;
//  font.SetWeight( wxBOLD );

  wxListItem item;
  item.SetAlign(wxLIST_FORMAT_RIGHT);
  item.SetText( tmp );
  item.SetFont( font );
  item.SetColumn( 1 );
  item.SetId( 2 );

  lv->SetItem(item);
}

void MainFrame::SetupListView( void )
{
  // Set 2 columns
  lv->InsertColumn(0, wxEmptyString);
  lv->InsertColumn(1, wxEmptyString);

  // Set the whole background...
  lv->SetBackgroundColour( wxColour(224,224,224) );

  // Make every font behave normal...
  wxFont font; font.SetWeight( wxNORMAL );

  wxListItem item;
  item.SetAlign(wxLIST_FORMAT_LEFT);
  item.SetFont( font );
  item.SetColumn( 0 );

  // Add 'Ranges'
  item.SetText( _("Ranges") );
  item.SetId( 0 );

  lv->InsertItem( item );

  // Add 'Addresses'
  item.SetText( _("Addresses") );
  item.SetBackgroundColour( wxColour(240,240,240) );
  item.SetId( 1 );

  lv->InsertItem( item);

  // Add 'Blocked'
  item.SetText( _("Blocked") );
  item.SetBackgroundColour( wxNullColour );
  item.SetId( 2 );

  lv->InsertItem( item);

  lv->SetColumnWidth( 0, 200 );
  lv->SetColumnWidth( 1, 80 );


  if ( IPDatabase && IPDatabase->IsOk() )
  {
    ChangeRanges( IPDatabase->AmountOfRanges() );
    ChangeAddresses( IPDatabase->AmountOfIPs() );
    ChangeBlocked( Blocker::AmountOfBlocks() );
  }
  else
  {
    ChangeRanges( 0 );
    ChangeAddresses( 0 );
    ChangeBlocked( 0 );
  }
  
  SendSizeEvent();
}

void MainFrame::OnCheckBoxClicked( wxCommandEvent& event )
{
  bool bWaarde( event.GetInt()==0?false:true );
  
  switch( event.GetId() )
  {
  case OPTIONS__BLOCK_BANNED_IP:
    DefaultConfig->SetBannedIPs( bWaarde );
    break;

  case OPTIONS__CONNECTION_LOG:
    DefaultConfig->SetConnectionLog( bWaarde );
    break;

  case OPTIONS__LIVE_WARNINGS:
    DefaultConfig->SetLiveWarnings( bWaarde );
    break;

  case OPTIONS__INCLUDE_PORT_NUMBERS:
    DefaultConfig->SetIncludePortNumbers( bWaarde );
    break;

  case OPTIONS__ALLOW_CONNECTION_TO_PORT_80:
    DefaultConfig->SetAllowPortEighty( bWaarde );
    break;

  case OPTIONS__SHOW_CHECKED_IPS:
    DefaultConfig->SetShowCheckedIPs( bWaarde );
    break;

  case OPTIONS__LAUNCH_MINIMIZED:
    DefaultConfig->SetLaunchMinimized( bWaarde );
    break;

  case OPTIONS__HIDE_ON_LAUNCH:
    DefaultConfig->SetHideOnLaunch( bWaarde );
    break;

  case OPTIONS__AUTO_UPDATE:
    DefaultConfig->SetAutoUpdate( bWaarde );
    break;

  case OPTIONS__LAUNCH_WITH_WINDOWS:
    {
#ifdef __WINDOWS__
      wxRegConfig reg( wxT("PeerGuardian"), wxT("KaReL"), wxT("Microsoft\\Windows\\CurrentVersion\\Run"), wxEmptyString, wxCONFIG_USE_LOCAL_FILE );
      if ( bWaarde ) 
      {
        reg.Write( wxT("PeerGuardian"), wxGetExecutablePath() );
      }
      else
      {
        reg.Write( wxT("PeerGuardian"), wxT("") );
        reg.DeleteEntry( wxT("PeerGuardian"), false );
      }

#else
#error Not implemented on this platform!
#endif
      DefaultConfig->SetAutoStart( bWaarde );
    }
    break;

  case OPTIONS__CONFIRM_SHUTDOWN:
    DefaultConfig->SetConfirmShutdown( bWaarde );
    break;

  case OPTIONS__PASSWORD_PROTECT_SHUTDOWN:
    DefaultConfig->SetPassShutdown( bWaarde );
    password->Enable( bWaarde );
    password->SetFocus();
    break;
  }
}

extern void ViewHTMLFile(const wxString& url);
void MainFrame::OnMenu_Website( wxCommandEvent& event )
{
  switch( event.GetId() )
  {
  case MAINFRAME_MENU__PG_HOME:
    ViewHTMLFile( wxT("http://www.methlabs.org/") );
    break;

  case MAINFRAME_MENU__PG_MIRROR:
    ViewHTMLFile( wxT("http://xs.tech.nu/") );
    break;

  case MAINFRAME_MENU__METH_FORUM:
    ViewHTMLFile( wxT("http://methlabs.org/forums/") );
    break;

  case MAINFRAME_MENU__PG_FORUM:
    ViewHTMLFile( wxT("http://peerguardian.net/forums/index.php?act=idx") );
    break;

  case MAINFRAME_MENU__PAYPAL:
    ViewHTMLFile( wxT("https://www.paypal.com/cgi-bin/webscr?cmd=_xclick&business=tim.leonard1@ntlworld.com&item_name=PeerGuardian&nonote=1&currency_code=USD&tax=0") );
    break;

  case MAINFRAME_MENU__UPDATES:
    UpdateThread * m_update_thread = new UpdateThread( this, false );
    if ( m_update_thread->Create() == wxTHREAD_NO_ERROR )
      m_update_thread->Run();
    else
      ShowError( _("Couldn't update the program!") );
    break;
  }
}

class MyTaskBarIcon : public wxTaskBarIcon
{
public:
  MyTaskBarIcon( MainFrame * frame ) : m_frame(frame) { }
  virtual ~MyTaskBarIcon( ) { }

  void ShowFrame( wxTaskBarIconEvent& WXUNUSED(event) )
  {
    if ( !m_frame->IsShown() )
    {
      m_frame->Show( TRUE );
      m_frame->Iconize( false );
      m_frame->SetFocus( );
    }
  }

/*
  void ShowPopup( wxTaskBarIconEvent& event )
  {
  }
*/

private:
  MainFrame * m_frame;

  DECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(MyTaskBarIcon, wxTaskBarIcon)
  EVT_TASKBAR_LEFT_DCLICK ( MyTaskBarIcon::ShowFrame )
//  EVT_TASKBAR_RIGHT_UP    ( MyTaskBarIcon::ShowPopup )
END_EVENT_TABLE();

void MainFrame::CreateTaskBarIcon( )
{
  taskbar = new MyTaskBarIcon( this );
  SetToolTip( VERSION );
}

void MainFrame::DeleteTaskBarIcon( void )
{
  if ( taskbar )
    delete taskbar;
}

inline void MainFrame::SetToolTip( const wxString& tip )
{
  taskbar->SetIcon( wxICON(PeerGuardian), tip );
}

void MainFrame::OnMenu_ViewLog     ( wxCommandEvent& WXUNUSED(event) )
{
  wxString cmd;

  cmd << wxGetPath(CONNECTION_LOG);
#if defined(__WXMSW__)
  cmd.Prepend( wxT("notepad ") );

  STARTUPINFO sui;
  memset(&sui,0,sizeof(sui));
  PROCESS_INFORMATION pi;
  memset(&pi,0,sizeof(pi));

  CreateProcess(NULL,wxConstCast(cmd.c_str(), wxChar),NULL,NULL,FALSE,0,NULL,NULL,&sui,&pi);
#else
  wxExecute( cmd );
#endif
}
