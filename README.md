# **Logo Manager**
[![](https://img.shields.io/github/license/lazyt/lmgr.svg?color=blue)](https://github.com/LazyT/lmgr/blob/master/LICENSE)
[![](https://img.shields.io/badge/platform-win%20%7C%20lin%20%7C%20mac-blue.svg)](https://github.com/LazyT/lmgr#download)
[![](https://img.shields.io/badge/paypal-buy%20me%20a%20beer-red.svg)](https://paypal.me/LazyT)
[![](https://img.shields.io/github/downloads/lazyt/lmgr/total.svg?color=orange)](https://github.com/LazyT/lmgr/releases)
[![](https://img.shields.io/github/last-commit/lazyt/lmgr/master.svg?color=yellow)](https://github.com/LazyT/lmgr/commits/master)
[![](https://img.shields.io/github/release-date/lazyt/lmgr.svg?color=brightgreen)](https://github.com/LazyT/lmgr/releases/latest)
[![](https://img.shields.io/github/release/lazyt/lmgr.svg?color=brightgreen)](https://github.com/LazyT/lmgr/releases/latest)

## Description

Logo Manager is an easy-to-use tool for viewing, editing, dumping and flashing splash screens (logo.img) of certain Xiaomi smartphones.

It is based on Qt and can therefore be used on various operating systems such as Windows, Linux and MacOS.

All necessary files are included, so no installation is required.

The current version supports the following features:

* view, edit, dump (root) and flash (unlocked bootloader) logo images
* multi language (English and German)
* cross platform (same look & feel on Windows, Linux, MacOS)

## Screenshots

![main window](https://raw.github.com/LazyT/lmgr/gh-pages/screenshots/mainwindow.png)

## Download

Download the latest version for your operating system. All 3 files (exe, dmg, AppImage) contain the binary, translation, help and required libraries to run lmgr on the target platform without installing anything.

* [Windows (exe)](https://github.com/LazyT/lmgr/releases)

This is an [7zip](https://www.7-zip.org) self extracting archive. It will be automatically extracted to "%temp%\7zxxxxxxxx" and after that the "lmgr.exe" is started. You can copy this directory or extract the file with 7z if you want the content.

Note: some virus scanners classify lmgr as malicious. This is due to the use of [7zsfx](https://www.7-zip.org/a/lzma1900.7z). If you don't trust me unzip the exe and scan that directory, no warnings should appear now.

* [MacOS (dmg)](https://github.com/LazyT/lmgr/releases)

This is an Apple disc image. You can mount and run or copy the application.

* [Linux (AppImage)](https://github.com/LazyT/lmgr/releases)

This is an [AppImage](https://appimage.org) package. Don't forget to "chmod +x *.AppImage" after download and then run it. You can use the parameter "--appimage-extract" if you want the content.

For better system integration it's recommended to install [AppImageD](https://github.com/AppImage/appimaged) or [AppImageLauncher](https://github.com/TheAssassin/AppImageLauncher).

## Build from Source

If the release binaries doesn't work for you build it yourself:

1) checkout the source code

		git clone https://github.com/LazyT/lmgr

2) change into the new lmgr directory and generate the Makefile

		cd lmgr && qmake

3) compile the source code

		make

GUI fans just install the [Qt-Environment](http://www.qt.io/download-open-source), open the "lmgr.pro" project file and click on the build button.

## Contact

You can reach me at [xda developers](https://forum.xda-developers.com/k20-pro/themes/app-logo-manager-splash-screens-t4084455).

## Credits

lmgr is based on

* [Qt](http://www.qt.io)

Thanks for this great software!
