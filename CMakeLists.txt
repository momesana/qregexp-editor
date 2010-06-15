project(QRegExp-Editor)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})


add_subdirectory(src)
