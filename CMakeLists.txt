project(QRegExp-Editor)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})

set(SHARE_PATH ${CMAKE_INSTALL_PREFIX}/share/qregexp-editor)

# must be placed before any add_subdirectory
set(FILES_TO_TRANSLATE)

add_subdirectory(src)

################ translation stuff ################

file(GLOB TRANSLATIONS_FILES translations/*.ts)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts
files(WARNING: make clean will delete the source .ts files! Danger!)")

if (UPDATE_TRANSLATIONS)
   qt4_create_translation(QM_FILES ${FILES_TO_TRANSLATE} ${TRANSLATIONS_FILES})
else (UPDATE_TRANSLATIONS)
   qt4_add_translation(QM_FILES ${TRANSLATIONS_FILES})
endif (UPDATE_TRANSLATIONS)

add_custom_target(translations DEPENDS ${QM_FILES})

install(FILES ${QM_FILES} DESTINATION ${SHARE_PATH}/translations)
