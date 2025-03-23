#!/bin/bash

# Source common library
. ./lib.sh

APP_NAME="SBaGen+"
SBAGEN_BINARY="dist/sbagen+-macos-universal"
PNG_SOURCE="assets/sbagen+.png"
ICON_NAME="app_icon"
DMG_NAME="SBaGen+.dmg"

section_header "Creating macOS Application Bundle..."

# Check if the binary exists
if [ ! -f "$SBAGEN_BINARY" ]; then
    error "$SBAGEN_BINARY not found. Execute ./macos-build-sbagen+.sh first."
    exit 1
fi

# Check if the PNG file exists
if [ ! -f "$PNG_SOURCE" ]; then
    error "$PNG_SOURCE not found!"
    exit 1
fi

# Create build directory if it doesn't exist
create_dir_if_not_exists "build"

# Create temporary AppleScript with the dialog
info "Creating AppleScript handler..."
cat > build/sbagen.applescript <<EOF
on run
    display dialog "Please open a .sbg file using this application." buttons {"OK"} default button "OK" with title "$APP_NAME" with icon POSIX file ((POSIX path of (path to me)) & "Contents/Resources/app_icon.icns")
end run

on open theFiles
    set filePath to POSIX path of (item 1 of theFiles)
    set appPath to POSIX path of (path to me)
    set sbagenPath to quoted form of (appPath & "Contents/Resources/bin/sbagen+")
    
    tell application "System Events"
        set fileName to name of (POSIX file filePath as alias)
    end tell
    
    set mainChoice to button returned of (display dialog "What would you like to do with '" & fileName & "'?" buttons {"Play", "Edit", "More..."} default button "Play" with title "$APP_NAME" with icon POSIX file ((POSIX path of (path to me)) & "Contents/Resources/app_icon.icns"))
    
    set oldDelimiters to AppleScript's text item delimiters
    set AppleScript's text item delimiters to "/"

    set pathComponents to text items of filePath
    set fileNameBase to item -1 of pathComponents
    set dirPath to ""

    repeat with i from 1 to ((count of pathComponents) - 1)
        set dirPath to dirPath & (item i of pathComponents) & "/"
    end repeat

    set AppleScript's text item delimiters to "."

    set fileNameParts to text items of fileNameBase
    set outputFileName to (item 1 of fileNameParts) & ".wav"
    set outputFilePath to quoted form of (dirPath & outputFileName)

    set AppleScript's text item delimiters to oldDelimiters
    
    if mainChoice is "Play" then
        tell application "Terminal"
            activate
            set terminalWindow to do script (sbagenPath & " " & quoted form of filePath & "; exit")
        end tell
    else if mainChoice is "Edit" then
        tell application "TextEdit"
            activate
            open (POSIX file filePath as alias)
        end tell
    else if mainChoice is "More..." then
        set convertChoice to button returned of (display dialog "Conversion options:" buttons {"Convert to WAV", "Convert to WAV (30 min)", "Cancel"} default button "Convert to WAV" with title "$APP_NAME" with icon POSIX file ((POSIX path of (path to me)) & "Contents/Resources/app_icon.icns"))
        if convertChoice is "Convert to WAV" then
            tell application "Terminal"
                activate
                set terminalWindow to do script (sbagenPath & " -Wo " & outputFilePath & " " & quoted form of filePath & "; exit")
                repeat while busy of terminalWindow is true
                    delay 0.5
                end repeat
                tell terminalWindow to close
            end tell
        else if convertChoice is "Convert to WAV (30 min)" then
            tell application "Terminal"
                activate
                set terminalWindow to do script (sbagenPath & " -L 00:30:00 -Wo " & outputFilePath & " " & quoted form of filePath & "; exit")
                repeat while busy of terminalWindow is true
                    delay 0.5
                end repeat
                tell terminalWindow to close
            end tell
        end if
    end if
end open
EOF

# Compile the AppleScript into an .app
info "Compiling AppleScript into application bundle..."
osacompile -o "build/$APP_NAME.app" build/sbagen.applescript > /dev/null

if [ $? -ne 0 ]; then
    error "Failed to compile AppleScript into application bundle!"
    exit 1
fi

# Copy the binary
info "Copying binary to application bundle..."
create_dir_if_not_exists "build/$APP_NAME.app/Contents/Resources/bin"
cp "$SBAGEN_BINARY" "build/$APP_NAME.app/Contents/Resources/bin/sbagen+"
chmod +x "build/$APP_NAME.app/Contents/Resources/bin/sbagen+"

# Generate the icon from the PNG file
section_header "Generating application icon..."
create_dir_if_not_exists "build/tmp_iconset"
sips -z 16 16 "$PNG_SOURCE" --out "build/tmp_iconset/icon_16x16.png" > /dev/null
sips -z 32 32 "$PNG_SOURCE" --out "build/tmp_iconset/icon_16x16@2x.png" > /dev/null
sips -z 32 32 "$PNG_SOURCE" --out "build/tmp_iconset/icon_32x32.png" > /dev/null
sips -z 64 64 "$PNG_SOURCE" --out "build/tmp_iconset/icon_32x32@2x.png" > /dev/null
sips -z 128 128 "$PNG_SOURCE" --out "build/tmp_iconset/icon_128x128.png" > /dev/null
sips -z 256 256 "$PNG_SOURCE" --out "build/tmp_iconset/icon_128x128@2x.png" > /dev/null
sips -z 256 256 "$PNG_SOURCE" --out "build/tmp_iconset/icon_256x256.png" > /dev/null
sips -z 512 512 "$PNG_SOURCE" --out "build/tmp_iconset/icon_512x512.png" > /dev/null
sips -z 1024 1024 "$PNG_SOURCE" --out "build/tmp_iconset/icon_512x512@2x.png" > /dev/null

# Convert the .iconset to .icns
mv build/tmp_iconset "build/$ICON_NAME.iconset"
iconutil -c icns "build/$ICON_NAME.iconset"

# Move the .icns to Resources
mv "build/$ICON_NAME.icns" "build/$APP_NAME.app/Contents/Resources/"

# Add file association and icon reference to Info.plist
info "Configuring application bundle Info.plist..."
cat > "build/$APP_NAME.app/Contents/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>droplet</string>
    <key>CFBundleIdentifier</key>
    <string>com.sbagen.plus</string>
    <key>CFBundleName</key>
    <string>$APP_NAME</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleVersion</key>
    <string>1.0</string>
    <key>CFBundleIconFile</key>
    <string>app_icon</string>
    <key>CFBundleDocumentTypes</key>
    <array>
        <dict>
            <key>CFBundleTypeName</key>
            <string>SBG File</string>
            <key>CFBundleTypeExtensions</key>
            <array>
                <string>sbg</string>
            </array>
            <key>CFBundleTypeRole</key>
            <string>Editor</string>
            <key>LSItemContentTypes</key>
            <array>
                <string>com.sbagen.plus.sbg</string>
            </array>
            <key>CFBundleTypeOSTypes</key>
            <array>
                <string>sbg</string>
            </array>
            <key>LSHandlerRank</key>
            <string>Owner</string>
            <key>LSIsAppleDefaultForType</key>
            <true/>
            <key>CFBundleTypeIconFile</key>
            <string>app_icon</string>
            <key>CFBundleTypeMIMETypes</key>
            <array>
                <string>application/x-sbagen-plus</string>
            </array>
        </dict>
    </array>
    <key>UTImportedTypeDeclarations</key>
    <array>
        <dict>
            <key>UTTypeIdentifier</key>
            <string>com.sbagen.plus.sbg</string>
            <key>UTTypeDescription</key>
            <string>SBaGen+ Sequence File</string>
            <key>UTTypeConformsTo</key>
            <array>
                <string>public.data</string>
            </array>
            <key>UTTypeTagSpecification</key>
            <dict>
                <key>public.filename-extension</key>
                <array>
                    <string>sbg</string>
                </array>
                <key>public.mime-type</key>
                <array>
                    <string>application/x-sbagen-plus</string>
                </array>
            </dict>
        </dict>
    </array>
    <key>UTExportedTypeDeclarations</key>
    <array>
        <dict>
            <key>UTTypeIdentifier</key>
            <string>com.sbagen.plus.sbg</string>
            <key>UTTypeDescription</key>
            <string>SBaGen+ Sequence File</string>
            <key>UTTypeConformsTo</key>
            <array>
                <string>public.data</string>
            </array>
            <key>UTTypeTagSpecification</key>
            <dict>
                <key>public.filename-extension</key>
                <array>
                    <string>sbg</string>
                </array>
                <key>public.mime-type</key>
                <array>
                    <string>application/x-sbagen-plus</string>
                </array>
            </dict>
        </dict>
    </array>
    <key>NSAppleEventsUsageDescription</key>
    <string>This application needs to control the Terminal to process .sbg files.</string>
</dict>
</plist>
EOF

# Create an entitlements file to allow Apple Events
cat > build/entitlements.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>com.apple.security.automation.apple-events</key>
    <true/>
</dict>
</plist>
EOF

# Sign the application with ad-hoc signing
codesign --force --deep --sign - "build/$APP_NAME.app" --entitlements build/entitlements.plist > /dev/null 2>&1

# Create a temporary folder for the DMG content
section_header "Creating DMG installer..."
create_dir_if_not_exists "build/dmg"
mv "build/$APP_NAME.app" build/dmg/

# Create the DMG
hdiutil create -volname "$APP_NAME" -srcfolder build/dmg -ov -format UDZO "dist/$DMG_NAME" > /dev/null 2>&1

# Remove temporary files
info "Cleaning up temporary files..."
rm -rf build

success "Application bundle created and packaged in dist/$DMG_NAME successfully!"