#
#
#


current_dir_name( CURR_DIR_NAME )


## add instruction set
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfma -mavx2")


#find_package(Threads REQUIRED)
#
#set( EXT_LIBS Threads::Threads )
#
#build_execs( ${CURR_DIR_NAME} )


## tests
build_tests( ${CURR_DIR_NAME} "${GTEST_LIBRARIES}" )
