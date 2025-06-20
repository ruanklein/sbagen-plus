#define MyAppName "SBaGen+"
#define MyAppVersion "1.5.5"
#define MyAppPublisher "Ruan Klein"
#define MyAppURL "https://github.com/ruanklein/sbagen-plus"
#define MyAppExeName "sbagen+.exe"
#define MyAppIcon "assets\sbagen+.ico"
#define MyAppAssocName "SBaGen+ Sequence File"
#define MyAppAssocExt ".sbg"
#define MyAppAssocKey StringChange(MyAppAssocName, " ", "") + MyAppAssocExt
#define MyAppUserDocsDir "{userdocs}\SBaGen+"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
AppId={{F4E7F182-5437-4F54-A4FE-B8F2891AE7E2}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=dist
OutputBaseFilename=sbagen+-windows-setup
Compression=lzma2/fast
LZMAUseSeparateProcess=yes
LZMANumBlockThreads=1
SolidCompression=yes
ArchitecturesAllowed=x86 x64 arm64
MinVersion=6.1.7601
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=commandline
SetupIconFile={#MyAppIcon}
UninstallDisplayIcon={app}\{#MyAppExeName}
WizardStyle=modern
LicenseFile=COPYING.txt
DisableWelcomePage=no
ChangesAssociations=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Messages]
; Override some default messages to ensure they're in English
BeveledLabel=Installation
ButtonBack=< &Back
ButtonCancel=Cancel
ButtonFinish=&Finish
ButtonInstall=&Install
ButtonNext=&Next >
ClickNext=Click Next to continue, or Cancel to exit Setup.
WizardLicense=License Agreement
LicenseLabel={#MyAppName} license agreement.
LicenseAccepted=I &accept the agreement
LicenseNotAccepted=I &do not accept the agreement

[CustomMessages]
NoticeCaption=Important Notice
NoticeDescription=Please read this important notice before continuing:

[Tasks]
Name: "associatewithfiles"; Description: "Associate .sbg files with {#MyAppName}"; GroupDescription: "File associations:";
Name: "addtopath"; Description: "Add {#MyAppName} to PATH environment variable"; GroupDescription: "System integration:"; Flags: unchecked

[Files]
; Include both 32-bit and 64-bit versions
Source: "dist\sbagen+-win32.exe"; DestDir: "{app}"; DestName: "sbagen+-win32.exe"; Flags: ignoreversion
Source: "dist\sbagen+-win64.exe"; DestDir: "{app}"; DestName: "sbagen+-win64.exe"; Flags: ignoreversion
; Documentation
Source: "COPYING.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "NOTICE.txt"; DestDir: "{app}"; Flags: ignoreversion dontcopy
Source: "docs\*"; DestDir: "{app}\docs"; Flags: ignoreversion recursesubdirs createallsubdirs
; Example files
Source: "examples\*"; DestDir: "{app}\examples"; Flags: ignoreversion recursesubdirs createallsubdirs
; Scripts directory
Source: "scripts\*"; DestDir: "{app}\scripts"; Flags: ignoreversion recursesubdirs createallsubdirs
; Changelog
Source: "ChangeLog.txt"; DestDir: "{app}"; Flags: ignoreversion
; USAGE.txt
Source: "build\USAGE.txt"; DestDir: "{app}"; Flags: ignoreversion
; RESEARCH.txt
Source: "build\RESEARCH.txt"; DestDir: "{app}"; Flags: ignoreversion
; Documentation files in user's Documents folder
Source: "docs\*"; DestDir: "{#MyAppUserDocsDir}\Documentation"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "examples\*"; DestDir: "{#MyAppUserDocsDir}\Examples"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "COPYING.txt"; DestDir: "{#MyAppUserDocsDir}"; DestName: "License.txt"; Flags: ignoreversion
Source: "NOTICE.txt"; DestDir: "{#MyAppUserDocsDir}"; DestName: "Notice.txt"; Flags: ignoreversion
Source: "build\USAGE.txt"; DestDir: "{#MyAppUserDocsDir}"; DestName: "Usage.txt"; Flags: ignoreversion
Source: "build\RESEARCH.txt"; DestDir: "{#MyAppUserDocsDir}"; DestName: "Research.txt"; Flags: ignoreversion

[Dirs]
Name: "{#MyAppUserDocsDir}"; Flags: uninsalwaysuninstall

[Icons]
; Program shortcuts
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

; Desktop shortcut to Documents folder
Name: "{autodesktop}\SBaGen+ Files"; Filename: "{#MyAppUserDocsDir}"

[Registry]
; File association for .sbg files (User Level)
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocExt}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue; Tasks: associatewithfiles
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocKey}"; ValueType: string; ValueName: ""; ValueData: "{#MyAppAssocName}"; Flags: uninsdeletekey; Tasks: associatewithfiles
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocKey}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\{#MyAppExeName},0"; Tasks: associatewithfiles
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1"""; Tasks: associatewithfiles

; Context menu for .sbg files - Edit option
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\edit"; ValueType: string; ValueName: ""; ValueData: "Edit sequence file"; Flags: uninsdeletekey; Tasks: associatewithfiles
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\edit"; ValueType: string; ValueName: "Icon"; ValueData: """{app}\{#MyAppExeName}"""; Flags: uninsdeletekey; Tasks: associatewithfiles
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\edit\command"; ValueType: string; ValueName: ""; ValueData: "notepad.exe ""%1"""; Flags: uninsdeletekey; Tasks: associatewithfiles

; Context menu for .sbg files - Write to WAV option
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\writetoWAV"; ValueType: string; ValueName: ""; ValueData: "Write file to WAV"; Flags: uninsdeletekey; Tasks: associatewithfiles
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\writetoWAV"; ValueType: string; ValueName: "Icon"; ValueData: """{app}\{#MyAppExeName}"""; Flags: uninsdeletekey; Tasks: associatewithfiles
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\writetoWAV\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" -Wo ""output.wav"" ""%1"""; Flags: uninsdeletekey; Tasks: associatewithfiles

; Context menu for .sbg files - Write to WAV option (30 minutes)
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\writetoWAV30"; ValueType: string; ValueName: ""; ValueData: "Write file to WAV (30 minutes)"; Flags: uninsdeletekey; Tasks: associatewithfiles
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\writetoWAV30"; ValueType: string; ValueName: "Icon"; ValueData: """{app}\{#MyAppExeName}"""; Flags: uninsdeletekey; Tasks: associatewithfiles
Root: HKCU; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\writetoWAV30\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" -L 00:30:00 -Wo ""output.wav"" ""%1"""; Flags: uninsdeletekey; Tasks: associatewithfiles

; Force Windows to refresh shell icons
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\{#MyAppAssocExt}"; ValueType: none; ValueName: ""; Flags: deletekey; Tasks: associatewithfiles

; Add installation directory to user PATH
Root: HKCU; Subkey: "Environment"; ValueType: expandsz; ValueName: "PATH"; ValueData: "{olddata};{app}"; Check: NeedsAddPath('{app}'); Tasks: addtopath

[Run]
Filename: "{win}\explorer.exe"; Parameters: """{#MyAppUserDocsDir}"""; Description: "Open SBaGen+ folder"; Flags: postinstall nowait skipifsilent shellexec

[Code]
var
  NoticePage: TOutputMsgMemoWizardPage;

function NeedsAddPath(Param: string): boolean;
var
  OrigPath: string;
begin
  if RegQueryStringValue(HKEY_CURRENT_USER, 'Environment', 'PATH', OrigPath) then
  begin
    { look for the path with leading and trailing semicolon }
    { Pos() returns 0 if not found }
    Result := Pos(';' + Param + ';', ';' + OrigPath + ';') = 0;
  end
  else
  begin
    Result := True;
  end;
end;

procedure RemovePath(Path: string);
var
  Paths: string;
  P: Integer;
begin
  if RegQueryStringValue(HKEY_CURRENT_USER, 'Environment', 'PATH', Paths) then
  begin
    { Remove path from the string }
    P := Pos(';' + Path + ';', ';' + Paths + ';');
    if P > 0 then
    begin
      Delete(Paths, P - 1, Length(Path) + 1);
      RegWriteStringValue(HKEY_CURRENT_USER, 'Environment', 'PATH', Paths);
    end;
  end;
end;

procedure InitializeWizard;
var
  NoticeLines: TStringList;
  NoticeText: AnsiString;
begin
  { Create the notice page }
  NoticePage := CreateOutputMsgMemoPage(wpWelcome,
    ExpandConstant('{cm:NoticeCaption}'),
    ExpandConstant('{cm:NoticeDescription}'),
    '', '');

  { Load and display NOTICE.txt content }
  NoticeLines := TStringList.Create;
  try
    ExtractTemporaryFile('NOTICE.txt');
    NoticeLines.LoadFromFile(ExpandConstant('{tmp}\NOTICE.txt'));
    NoticeText := NoticeLines.Text;
    NoticePage.RichEditViewer.Lines.Text := NoticeText;
  finally
    NoticeLines.Free;
  end;
end;

function GetUninstallString(): String;
var
  sUnInstPath: String;
  sUnInstallString: String;
begin
  sUnInstPath := ExpandConstant('Software\Microsoft\Windows\CurrentVersion\Uninstall\{#emit SetupSetting("AppId")}_is1');
  sUnInstallString := '';
  if not RegQueryStringValue(HKLM, sUnInstPath, 'UninstallString', sUnInstallString) then
    RegQueryStringValue(HKCU, sUnInstPath, 'UninstallString', sUnInstallString);
  Result := sUnInstallString;
end;

function IsUpgrade(): Boolean;
begin
  Result := (GetUninstallString() <> '');
end;

function InitializeSetup(): Boolean;
var
  V: Integer;
  iResultCode: Integer;
  sUnInstallString: String;
begin
  Result := True;
  
  if IsUpgrade() then
  begin
    V := MsgBox('A previous version of ' + '{#MyAppName}' + ' was detected. Would you like to uninstall it before continuing?', mbInformation, MB_YESNO);
    if V = IDYES then
    begin
      sUnInstallString := GetUninstallString();
      sUnInstallString := RemoveQuotes(sUnInstallString);
      Exec(ExpandConstant(sUnInstallString), '/SILENT /NORESTART /SUPPRESSMSGBOXES','', SW_HIDE, ewWaitUntilTerminated, iResultCode);
      Result := True;
    end
    else
      Result := False;
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  SourceFile: String;
  DestFile: String;
begin
  if CurStep = ssPostInstall then
  begin
    { Check system architecture and copy appropriate executable }
    if IsWin64 then
    begin
      SourceFile := ExpandConstant('{app}\sbagen+-win64.exe');
      DestFile := ExpandConstant('{app}\sbagen+.exe');
    end
    else
    begin
      SourceFile := ExpandConstant('{app}\sbagen+-win32.exe');
      DestFile := ExpandConstant('{app}\sbagen+.exe');
    end;
    
    { Copy the appropriate executable }
    if FileCopy(SourceFile, DestFile, False) then
    begin
      { Delete the original files }
      DeleteFile(ExpandConstant('{app}\sbagen+-win32.exe'));
      DeleteFile(ExpandConstant('{app}\sbagen+-win64.exe'));
    end;
    
    { Notify system about PATH changes }
    if WizardIsTaskSelected('addtopath') then
    begin
      { Broadcast WM_SETTINGCHANGE message }
      MsgBox('SBaGen+ has been added to the PATH environment variable. ' +
             'You may need to restart running applications for them to ' +
             'recognize the change.', mbInformation, MB_OK);
    end;
  end;
end;

{ Handle uninstallation - Remove dynamically created files }
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usPostUninstall then
  begin
    { Remove the dynamically created sbagen+.exe file }
    DeleteFile(ExpandConstant('{app}\sbagen+.exe'));
  end;
end; 