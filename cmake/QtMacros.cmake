# This file contains wrappers for some Qt4 and Qt5 macros

#############################################
#
#       Some macros for building Qt4/5 files
#
#############################################

macro (QTX_GENERATE_MOC _files)
   if (QT4_FOUND)
      qt4_generate_moc(${_files} "${ARGN}")
   elseif (Qt5Core_FOUND)
      qt5_generate_moc(${_files} "${ARGN}")
   endif()
endmacro()


macro (QTX_WRAP_CPP _files)
   if (QT4_FOUND)
      qt4_wrap_cpp(${_files} "${ARGN}")
   elseif (Qt5Core_FOUND)
      qt5_wrap_cpp(${_files} "${ARGN}")
   endif()
endmacro()


macro (QTX_WRAP_UI _files)
   if (QT4_FOUND)
      qt4_wrap_ui(${_files} "${ARGN}")
   elseif (Qt5Core_FOUND)
      qt5_wrap_ui(${_files} "${ARGN}")
   endif()
endmacro()


macro (QTX_ADD_RESOURCES _files)
   if (QT4_FOUND)
      qt4_add_resources(${_files} "${ARGN}")
   elseif (Qt5Core_FOUND)
      qt5_add_resources(${_files} "${ARGN}")
   endif()
endmacro()

function(QTX_CREATE_TRANSLATION files)
   if (QT4_FOUND)
      qt4_create_translation(_files)
   elseif (Qt5Core_FOUND)
      qt5_create_translation(_files)
   endif()
endfunction()

function(QTX_ADD_TRANSLATION files)
   if (QT4_FOUND)
      qt4_add_translation(_files)
   elseif (Qt5Core_FOUND)
      qt5_add_translation(_files)
   endif()
endfunction()
