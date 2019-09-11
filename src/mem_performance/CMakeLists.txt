#
#
#


current_dir_name( CURR_DIR_NAME )


## add instruction set
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfma -mavx2")


find_package(Threads REQUIRED)

set( EXT_LIBS Threads::Threads )

build_cpps( ${CURR_DIR_NAME} )


## copy plot script
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/plot_all.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/plot_config_single.gnu DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/plot_config_multi.gnu DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/plot_vector_mt.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/plot_vector_mp.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/plot_cllist_mt.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/plot_cllist_mp.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )


function(generate_benchmark_script data_type plot_title)
    set( GENERATOR_FILE "${CMAKE_CURRENT_SOURCE_DIR}/plot_single.sh.in" )
    set( DATA_TYPE ${data_type} )
    set( PLOT_TITLE ${plot_title} )
    configure_file( ${GENERATOR_FILE} "plot_${data_type}.sh" @ONLY )
endfunction()

generate_benchmark_script( vector_single "std::vector access times" )
generate_benchmark_script( array "raw array access times" )
generate_benchmark_script( cllist "linked list access times" )
generate_benchmark_script( cllist_small "linked list access times" )


## tests
build_tests( ${CURR_DIR_NAME} "${GTEST_LIBRARIES}" )
