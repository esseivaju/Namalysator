# Changelog

## 1.1 - 12.06.2020

### Added

* Updated from qt4 to qt5. This enables support of new [image formats](https://doc.qt.io/qt-5/qtimageformats-index.html) (3cde3d2). 
* Added the option to export errors to CSV files. One file is created for each sheet that was present in the ods file (dfb8bfa, 2a3b82e).

### Changed

* Block-cutter has been upadted to support other image formats by using Qt to read image istead of libtiff (bb246b8).
* Fixed bugs causing crash when some expected files / directory were missing (66c6e5e, 414c400).
* Updated boost filesystem version (1eca855).
