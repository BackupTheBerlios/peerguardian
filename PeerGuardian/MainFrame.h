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






























































#ifndef __MAINFRAME_H
#define __MAINFRAME_H

class MyStaticText;
class MyTaskBarIcon;

class MainFrame : public wxFrame
{
public:
  MainFrame(const wxString& title);
  virtual ~MainFrame( void );

  void AddToLog( const wxString& text_to_add = wxT(""), bool show_time = true );
  void AddToConnectionLog( const wxString& text_to_add = wxT(""), bool show_time = true );

  void ChangeRanges( ULONG amount_of_ranges );
  void ChangeAddresses( ULONG amount_of_addresses );
  void ChangeBlocked( ULONG amount_of_blocks );

  void SetToolTip( const wxString& tip );

protected: // events
  void OnMenu_Option      ( wxCommandEvent& WXUNUSED(event) );
  void OnMenu_StatusWindow( wxCommandEvent& WXUNUSED(event) );
  void OnMenu_ViewLog     ( wxCommandEvent& WXUNUSED(event) );
  void OnMenu_Website     ( wxCommandEvent&          event  );
  void OnSize             ( wxSizeEvent&          event     );
  void OnIconize          ( wxIconizeEvent&       event     );
  void OnCheckBoxClicked  ( wxCommandEvent&       event     );

  friend class MyStaticText;
  void SwapPriority( void );

private:
  void CreateMenuBar( void );
  void CreateStatusWindow( void );
  void CreateOptionsWindow( void );
  void CreateTaskBarIcon( void );
  void DeleteTaskBarIcon( void );

  /* status window */
  wxListView    * lv;
  void SetupListView( void );
  wxString      m_szStatusText;

  wxTextCtrl    * text;
  void AddDefaultText( void );


  /* options window */
  wxStaticText  * priority;
  wxTextCtrl    * password;

  /* The panels */
  wxPanel       * StatusWindow;
  wxPanel       * Options;

  /* The taskbar icon */
  MyTaskBarIcon * taskbar;

  /* Connection Log */
  wxFFile       connlog;

  DECLARE_EVENT_TABLE()
};

#endif // __MAINFRAME_H