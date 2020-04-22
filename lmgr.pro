QT          += core gui widgets network help

TARGET      = lmgr

SOURCES     += mainwindow.cpp about.cpp flash.cpp help.cpp onlineupd.cpp
HEADERS     += mainwindow.h   about.h   flash.h   help.h   onlineupd.h helpbrowser.h

FORMS       += res/ui/mainwindow.ui res/ui/about.ui res/ui/flash.ui res/ui/help.ui res/ui/onlineupd.ui
RESOURCES   += res/lmgr.qrc

TRANSLATIONS+= lng/lmgr_de.ts

# Installer Prerequisites:
# LIN = linuxdeployqt -> https://github.com/probonopd/linuxdeployqt/releases
# WIN = 7z -> https://www.7-zip.org/download.html
# ALL = adb/fastboot -> https://developer.android.com/studio/releases/platform-tools

FILE = $$cat(mainwindow.h, lines)
LINE = $$find(FILE, "APPVERS")
VERS = $$section(LINE, " ", 2)
VERS = $$replace(VERS, "\"", "")
VERSION = $$VERS
VERS = $$replace(VERS, "\.", "")

unix:!macx {
QMAKE_LFLAGS += -Wl,-rpath=.

INSTALLDIR = /tmp/lmgr.AppDir

target.path = $$INSTALLDIR/usr/bin

translations.path  = $$INSTALLDIR/usr/bin/lng
translations.files = lng/lmgr_de.qm $$[QT_INSTALL_TRANSLATIONS]/qtbase_de.qm

help.path  = $$INSTALLDIR/usr/bin/hlp
help.files = hlp/dst/*.qch hlp/dst/*.qhc

android.path  = $$INSTALLDIR/usr/bin
android.files = $$(PLATFORMTOOLS)/adb $$(PLATFORMTOOLS)/fastboot

lin.path  = $$INSTALLDIR
lin.extra = cp ins/lin/lmgr.desktop $$INSTALLDIR/lmgr.desktop &&\
            cp res/ico/app.png $$INSTALLDIR/lmgr.png &&\
            mkdir -p $$INSTALLDIR/usr/share/metainfo && cp ins/lin/de.lazyt.lmgr.appdata.xml $$INSTALLDIR/usr/share/metainfo/ &&\
            ln -s lmgr.png $$INSTALLDIR/.DirIcon &&\
            export VERSION=$$VERS &&\
            linuxdeployqt-continuous-x86_64.AppImage $$INSTALLDIR/lmgr.desktop -appimage -no-copy-copyright-files -no-translations &&\
            mv Logo_Manager-*-x86_64.AppImage lmgr-"$$VERS".AppImage

INSTALLS += target translations help android lin
}

win32 {
RC_ICONS += res/ico/app.ico

INSTALLDIR = $$(TMP)/lmgr.7zip

target.path = $$INSTALLDIR

translations.path  = $$INSTALLDIR/lng
translations.files = lng/lmgr_de.qm $$[QT_INSTALL_TRANSLATIONS]/qtbase_de.qm

help.path  = $$INSTALLDIR/hlp
help.files = hlp/dst/*.qch hlp/dst/*.qhc

libraries.path  = $$INSTALLDIR
libraries.files = $$[QT_INSTALL_LIBEXECS]/Qt5Core.dll $$[QT_INSTALL_LIBEXECS]/Qt5Gui.dll $$[QT_INSTALL_LIBEXECS]/Qt5Network.dll $$[QT_INSTALL_LIBEXECS]/Qt5Help.dll $$[QT_INSTALL_LIBEXECS]/Qt5Sql.dll $$[QT_INSTALL_LIBEXECS]/Qt5Widgets.dll $$[QT_INSTALL_LIBEXECS]/libgcc_s_seh-1.dll $$[QT_INSTALL_LIBEXECS]/libstdc++-6.dll $$[QT_INSTALL_LIBEXECS]/libwinpthread-1.dll $$[QT_INSTALL_PREFIX]/../../Tools/OpenSSL/Win_x64/bin/libcrypto-1_1-x64.dll $$[QT_INSTALL_PREFIX]/../../Tools/OpenSSL/Win_x64/bin/libssl-1_1-x64.dll

plugins1.path  = $$INSTALLDIR/platforms
plugins1.files = $$[QT_INSTALL_PLUGINS]/platforms/qwindows.dll
plugins2.path  = $$INSTALLDIR/styles
plugins2.files = $$[QT_INSTALL_PLUGINS]/styles/qwindowsvistastyle.dll
plugins3.path  = $$INSTALLDIR/imageformats
plugins3.files = $$[QT_INSTALL_PLUGINS]/imageformats/qjpeg.dll
plugins4.path  = $$INSTALLDIR/sqldrivers
plugins4.files = $$[QT_INSTALL_PLUGINS]/sqldrivers/qsqlite.dll

android.path  = $$INSTALLDIR
android.files = $$(PLATFORMTOOLS)/adb.exe $$(PLATFORMTOOLS)/fastboot.exe $$(PLATFORMTOOLS)/AdbWinApi.dll $$(PLATFORMTOOLS)/AdbWinUsbApi.dll

win.path  = $$INSTALLDIR
win.extra = move $$INSTALLDIR\lmgr.exe $$INSTALLDIR\run.exe &&\
            7z a -mx9 $$(TMP)\lmgr.7z $$INSTALLDIR\* &&\
            copy /b ins\win\7zs2.sfx + $$(TMP)\lmgr.7z lmgr-"$$VERS".exe

INSTALLS += target translations help libraries plugins1 plugins2 plugins3 plugins4 android win
}

macx {
ICON += res/ico/app.icns

QMAKE_INFO_PLIST = ins/mac/Info.plist

INSTALLDIR = $$(TMPDIR)

target.path = $$INSTALLDIR

translations.path  = $$INSTALLDIR/lmgr.app/Contents/MacOS/lng
translations.files = lng/lmgr_de.qm $$[QT_INSTALL_TRANSLATIONS]/qtbase_de.qm

help.path  = $$INSTALLDIR/lmgr.app/Contents/MacOS/hlp
help.files = hlp/dst/*.qch hlp/dst/*.qhc

android.path  = $$INSTALLDIR/lmgr.app/Contents/MacOS
android.files = $$(PLATFORMTOOLS)/adb $$(PLATFORMTOOLS)/fastboot

mac.path  = $$INSTALLDIR/lmgr.app
mac.extra = cd $$INSTALLDIR &&\
            $$[QT_INSTALL_BINS]/macdeployqt lmgr.app -dmg &&\
            mv $$INSTALLDIR/lmgr.dmg $$PWD/lmgr-"$$VERS".dmg

INSTALLS += target translations help android mac
}
