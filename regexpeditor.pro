# #####################################################################
# Automatically generated by qmake (2.01a) Do. Mrz 4 18:08:42 2010
# #####################################################################
TEMPLATE = app
TARGET = qregexp-editor
DEPENDPATH += .
INCLUDEPATH += .
MOC_DIR = build-intermediate/moc
OBJECTS_DIR = build-intermediate/obj
UI_DIR = build-intermediate/ui
CONFIG += debug

# Input
HEADERS += src/mainwindow.h \
    src/regexpmodel.h \
    src/aboutdialog.h \
    src/node.h \
    src/treeitem.h
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/aboutdialog.cpp \
    src/regexpmodel.cpp \
    src/node.cpp \
    src/treeitem.cpp
FORMS += forms/mainwindow.ui \
    forms/aboutdialog.ui
RESOURCES += application.qrc
OTHER_FILES += COPYING \
    COPYING.html \
    ABOUT.html \
    AUTHORS \
    AUTHORS.html
TRANSLATIONS = translations/regexpeditor_de.ts
