include_directories(
   ${CMAKE_CURRENT_BINARY_DIR}
)


file(GLOB HEADER_FILES *.h *.hpp)
file(GLOB CPP_FILES *.cpp)
file(GLOB UI_FILES *.ui)
set(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${CPP_FILES} ${UI_FILES} ${HEADER_FILES} PARENT_SCOPE)

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

set(qregexp-editor_RCCS
   ../application.qrc
)

# generate rules for building source files from the resources
qt4_add_resources(qregexp-editor_RCC_SRCS ${qregexp-editor_RCCS})

qt4_wrap_ui(qregexp-editor_SRCS ${qregexp-editor_UIs})
qt4_wrap_cpp(qregexp-editor_SRCS ${qregexp-editor_MOC_HDRS})

add_executable(qregexp-editor ${qregexp-editor_SRCS} ${qregexp-editor_RCC_SRCS})

target_link_libraries(qregexp-editor ${QT_LIBRARIES})

install(TARGETS qregexp-editor DESTINATION bin)
