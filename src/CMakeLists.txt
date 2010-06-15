include_directories(
   ${CMAKE_CURRENT_BINARY_DIR}
)

set(qregexp-editor_SRCS
   node.cpp
   regexpmodel.cpp
   treeitem.cpp
   mainwindow.cpp
   escapedpatterndialog.cpp
   aboutdialog.cpp
   main.cpp
)

set(qregexp-editor_UIs
   ui/aboutdialog.ui
   ui/escapedpatterndialog.ui
   ui/mainwindow.ui
)

set(qregexp-editor_MOC_HDRS
   aboutdialog.h
   escapedpatterndialog.h
   mainwindow.h
   regexpmodel.h
)

qt4_wrap_ui(qregexp-editor_SRCS ${qregexp-editor_UIs})
qt4_wrap_cpp(qregexp-editor_SRCS ${qregexp-editor_MOC_HDRS})

add_executable(qregexp-editor ${qregexp-editor_SRCS})

target_link_libraries(qregexp-editor ${QT_LIBRARIES})

install(TARGETS qregexp-editor DESTINATION bin)
