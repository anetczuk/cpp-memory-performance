#
#
#

if( NOT CMAKE_TESTING_ENABLED )
    return()
endif()


current_dir_name( CURR_DIR_NAME )


build_cpps( ${CURR_DIR_NAME} )


## tests
build_tests( ${CURR_DIR_NAME} )
