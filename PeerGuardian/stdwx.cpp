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

Config    * DefaultConfig;
Database  * IPDatabase;

wxString ShowError( )
{
  /* from msdn */
  LPTSTR lpMsgBuf = NULL;
  FormatMessage( 
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    lpMsgBuf,
    0,
    NULL 
  );
  wxString errMsg = lpMsgBuf;

  LocalFree( lpMsgBuf );

  wxMessageBox( errMsg, _("Error"), wxICON_ERROR );

  return errMsg;
}

void ShowError( const wxString& errMsg )
{
  wxMessageBox( errMsg, _("Error"), wxICON_ERROR );
}

void ViewHTMLFile(const wxString& url)
{
#ifdef __WXMSW__
  HKEY hKey;
  char szCmdName[1024];
  DWORD dwType, dw = sizeof(szCmdName);
  LONG lRes;
  lRes = RegOpenKeyA(HKEY_CLASSES_ROOT, "htmlfile\\shell\\open\\command", &hKey);

  if(lRes == ERROR_SUCCESS && RegQueryValueExA(hKey,(LPSTR)NULL, NULL, &dwType, (LPBYTE)szCmdName, &dw) == ERROR_SUCCESS)
  {
    strcat(szCmdName, (const char*) url.mb_str());
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFOA siStartInfo;
    memset(&siStartInfo, 0, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    CreateProcessA(NULL, szCmdName, NULL, NULL, FALSE, NULL, NULL, NULL, &siStartInfo, &piProcInfo );
  }
  if(lRes == ERROR_SUCCESS)
    RegCloseKey(hKey);
#else
  wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(wxT("html"));
  if ( !ft )
  {
    wxLogError(_T("Impossible to determine the file type for extension html. Please edit your MIME types."));
    return ;
  }

  wxString cmd;
  bool ok = ft->GetOpenCommand(&cmd,
  wxFileType::MessageParameters(url, _T("")));
  delete ft;

  if (!ok)
  {
    // TODO: some kind of configuration dialog here.
    wxMessageBox(_("Could not determine the command for running the browser."),
    _("Browsing problem"), wxOK|wxICON_EXCLAMATION);
    return ;
  }

  ok = (wxExecute(cmd, FALSE) != 0);
#endif
}

wxString wxGetExecutablePath()
{
    static bool found = false;
    static wxString path;

    if (found)
        return path;
    else
    {
#ifdef __WXMSW__

        wxChar buf[512];
        *buf = '\0';
        GetModuleFileName(NULL, buf, 511);
        path = buf;

#elif defined(__WXMAC__)

        ProcessInfoRec processinfo;
        ProcessSerialNumber procno ;
        FSSpec fsSpec;

        procno.highLongOfPSN = NULL ;
        procno.lowLongOfPSN = kCurrentProcess ;
        processinfo.processInfoLength = sizeof(ProcessInfoRec);
        processinfo.processName = NULL;
        processinfo.processAppSpec = &fsSpec;

        GetProcessInformation( &procno , &processinfo ) ;
        path = wxMacFSSpec2MacFilename(&fsSpec);
#else
        wxString argv0 = wxTheApp->argv[0];

        if (wxIsAbsolutePath(argv0))
            path = argv0;
        else
        {
            wxPathList pathlist;
            pathlist.AddEnvList(wxT("PATH"));
            path = pathlist.FindAbsoluteValidPath(argv0);
        }

        wxFileName filename(path);
        filename.Normalize();
        path = filename.GetFullPath();
#endif
        found = true;
        return path;
    }
}

wxString wxGetPath( const wxString& file )
{
  return wxGetExecutablePath().BeforeLast( wxFILE_SEP_PATH ) + wxFILE_SEP_PATH + file;
}
