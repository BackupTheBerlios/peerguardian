# Microsoft Developer Studio Project File - Name="PeerGuardian" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PeerGuardian - Win32 Unicode Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PeerGuardian.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PeerGuardian.mak" CFG="PeerGuardian - Win32 Unicode Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PeerGuardian - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "PeerGuardian - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PeerGuardian - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswu"
# PROP BASE Intermediate_Dir "vc_mswu\PeerGuardian"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswu"
# PROP Intermediate_Dir "vc_mswu\PeerGuardian"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O1 /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswu" /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "_WINDOWS" /Fd"vc_mswu\PeerGuardian.pdb" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "C:\Program Files\Coding\wxWindows\lib\vc_lib\mswu" /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "_WINDOWS" /D "UNICODE" /FR /Yu"stdwx.h" /Fd"vc_mswu\PeerGuardian.pdb" /FD /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswu" /i "." /i ".\..\..\samples" /d "__WXMSW__" /d "_UNICODE" /d "_WINDOWS"
# ADD RSC /l 0x405 /d "__WXMSW__" /d "_UNICODE" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25u_adv.lib wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /subsystem:windows /machine:I386 /libpath:".\..\..\lib\vc_lib"
# ADD LINK32 wxmsw25u_adv.lib wxmsw25u_core.lib wxbase25u.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib IPHlpApi.Lib /nologo /subsystem:windows /machine:I386 /libpath:".\..\..\lib\vc_lib"
# Begin Custom Build
InputDir=.\vc_mswu
ProjDir=.
InputPath=.\vc_mswu\PeerGuardian.exe
SOURCE="$(InputPath)"

"test" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	upx --best $(InputPath) 
	if not exist $(InputDir)\src mkdir $(InputDir)\src 
	if exist $(InputDir)\src\GNU_functions.cpp del $(InputDir)\src\GNU_functions.cpp 
	copy $(ProjDir)\GNU_functions.cpp $(InputDir)\src 
	cd $(InputDir) 
	if exist src\PeerGuardian.zip del src\PeerGuardian.zip 
	C:\Progra~1\WinRAR\winrar.exe a -afzip -ibck -m5 -o+ -r PeerGuardian.zip src\*.* PeerGuardian.exe LICENSE.txt README.txt 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "PeerGuardian - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswud"
# PROP BASE Intermediate_Dir "vc_mswud\PeerGuardian"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswud"
# PROP Intermediate_Dir "vc_mswud\PeerGuardian"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswud" /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /Fd"vc_mswud\PeerGuardian.pdb" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "C:\Program Files\Coding\wxWidgets\lib\vc_lib\mswud" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "UNICODE" /FR /Yu"stdwx.h" /Fd"vc_mswud\PeerGuardian.pdb" /FD /GZ /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswud" /i "." /i ".\..\..\samples" /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_UNICODE" /d "_WINDOWS"
# ADD RSC /l 0x405 /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswud" /i "." /i ".\..\..\samples" /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_UNICODE" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25ud_adv.lib wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\lib\vc_lib"
# ADD LINK32 wxmsw25ud_adv.lib wxmsw25ud_core.lib wxbase25ud.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib IPHlpApi.Lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\lib\vc_lib"

!ENDIF 

# Begin Target

# Name "PeerGuardian - Win32 Unicode Release"
# Name "PeerGuardian - Win32 Unicode Debug"
# Begin Group "Frame"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrame.h
# End Source File
# End Group
# Begin Group "App"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\App.cpp
# End Source File
# Begin Source File

SOURCE=.\App.h
# End Source File
# End Group
# Begin Group "Precompiler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\defs.h
# End Source File
# Begin Source File

SOURCE=.\stdwx.cpp
# ADD CPP /Yc"stdwx.h"
# End Source File
# Begin Source File

SOURCE=.\stdwx.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\resources\PeerGuardian.rc
# End Source File
# Begin Source File

SOURCE=.\resources\pg.ico
# End Source File
# End Group
# Begin Group "Config"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Config.cpp
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# End Group
# Begin Group "IP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IP.cpp
# End Source File
# Begin Source File

SOURCE=.\IP.h
# End Source File
# End Group
# Begin Group "Database"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Database.cpp
# End Source File
# Begin Source File

SOURCE=.\Database.h
# End Source File
# End Group
# Begin Group "Blocker"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Blocker.cpp

!IF  "$(CFG)" == "PeerGuardian - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "PeerGuardian - Win32 Unicode Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Blocker.h
# End Source File
# End Group
# Begin Group "AutoUpdate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutoUpdate.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoUpdate.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GNU_functions.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Target
# End Project
