set(PatternFilterTest_SRCS
   ../patternfilter.cpp
   patternfiltertest.cpp)


set(PatternFilterTest_MOC_HDRS
   patternfiltertest.h
)

qtx_wrap_cpp(PatternFilterTest_SRCS ${PatternFilterTest_MOC_HDRS})

add_executable(PatternFilterTest ${PatternFilterTest_SRCS})

if(QT4_FOUND)
   target_link_libraries(PatternFilterTest ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES})
elseif(Qt5Core_FOUND)
   target_link_libraries(PatternFilterTest ${Qt5Test_LIBRARIES})
endif()

add_test(NAME PatternFilterTest COMMAND PatternFilterTest)
