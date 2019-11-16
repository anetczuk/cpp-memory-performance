#
#
#


current_dir_name( CURR_DIR_NAME )


## add instruction set
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfma -mavx2")


find_package(Threads REQUIRED)

set( EXT_LIBS Threads::Threads )


include_directories( . )

file(GLOB_RECURSE cpp_files *.cpp)
build_execs_list( ${CURR_DIR_NAME} "${cpp_files}" )


set( EXPERIMENT_DIR experiment )


## generate cachegrind scripts
file(GLOB exec_files ${CMAKE_CURRENT_BINARY_DIR}/benchmark_*)
foreach(single_exec ${exec_files})
    get_filename_component( exec_name ${single_exec} NAME_WE)
    set( GENERATOR_FILE "${CMAKE_SOURCE_DIR}/cachegrind.sh.in" )
    set( EXECUTABLE "${exec_name}" )
    configure_file( ${GENERATOR_FILE} "cachegrind_${exec_name}.sh" @ONLY )
endforeach()


## copy plot script
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/${EXPERIMENT_DIR}/plot_all.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/${EXPERIMENT_DIR}/plot_config_single.gnu DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/${EXPERIMENT_DIR}/plot_config_multi.gnu DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )


function(generate_benchmark_script script_template data_type plot_title)
    set( GENERATOR_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${EXPERIMENT_DIR}/${script_template}" )
    set( DATA_TYPE ${data_type} )
    set( PLOT_TITLE ${plot_title} )
    configure_file( ${GENERATOR_FILE} "plot_${data_type}.sh" @ONLY )
endfunction()

function(generate_benchmark_script_mp executable data_type plot_title)
    set( GENERATOR_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${EXPERIMENT_DIR}/plot_mp.sh.in" )    
    set( BENCHMARK_TYPE ${executable} )
    set( DATA_TYPE ${data_type} )
    set( PLOT_TITLE ${plot_title} )
    configure_file( ${GENERATOR_FILE} "plot_${data_type}.sh" @ONLY )
endfunction()


generate_benchmark_script( plot_st.sh.in    raw_st "std::vector access times (raw)" )
generate_benchmark_script( plot_st.sh.in  array_st "raw array access times" )
generate_benchmark_script( plot_st.sh.in vector_st "std::vector access times" )
generate_benchmark_script( plot_st.sh.in cllist_st "linked list access times" )
generate_benchmark_script( plot_st.sh.in cllist_st_small "linked list access times" )

generate_benchmark_script( plot_mt.sh.in vector_mt "multithreaded std::vector access times" )
generate_benchmark_script( plot_mt.sh.in cllist_mt "multithreaded linked list access times" )

generate_benchmark_script_mp( vector_st vector_mp "multiprocessed std::vector access times" )
generate_benchmark_script_mp( cllist_st cllist_mp "multiprocessed linked list access times" )


## tests
build_tests( ${CURR_DIR_NAME} "${GTEST_LIBRARIES}" )
