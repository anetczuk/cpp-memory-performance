#
#
#


current_dir_name( CURR_DIR_NAME )


## add instruction set
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfma -mavx2")


build_cpps( ${CURR_DIR_NAME} )


## copy plot script
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/plot.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/plot_config.gnu DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )


## tests
build_tests( ${CURR_DIR_NAME} "${GTEST_LIBRARIES}" )
