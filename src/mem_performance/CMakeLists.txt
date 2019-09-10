#
#
#


current_dir_name( CURR_DIR_NAME )


## add instruction set
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfma -mavx2")


build_cpps( ${CURR_DIR_NAME} )


## copy plot script
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/plot_all.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/plot_config.gnu DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )


function(generate_benchmark_script data_type plot_title)
    set( GENERATOR_FILE "${CMAKE_CURRENT_SOURCE_DIR}/plot_single.sh.in" )
    set( DATA_TYPE ${data_type} )
    set( PLOT_TITLE ${plot_title} )
    configure_file( ${GENERATOR_FILE} "plot_${data_type}.sh" @ONLY )
endfunction()

generate_benchmark_script( vector "std::vector access times" )
generate_benchmark_script( array "raw array access times" )
generate_benchmark_script( cllist "linked list access times" )
generate_benchmark_script( cllist_small "linked list access times" )


## tests
build_tests( ${CURR_DIR_NAME} "${GTEST_LIBRARIES}" )
