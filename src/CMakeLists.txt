##
##
##

cmake_minimum_required(VERSION 3.1.1)


if( "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}" )
    message( FATAL_ERROR "Building in source is forbidden. Change output directory.")
endif()


project( memory-performance )


set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )


include( CMakeCompileFlags )

include( CMakeCoverage )

include( CMakeSanitizer )

include( CMakeUtils )


##
## enable CTest utility
##
enable_testing()



## ================= build =================


option( DISABLE_MT_BENCHMARKS "Disable multithread benchmarks" OFF )
if ( DISABLE_MT_BENCHMARKS )
    add_definitions(-DDISABLE_MT_BENCHMARKS)
endif()


set( LOOP_UNROLL_DEFAULT OFF )
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    ## default value for GCC
    set( LOOP_UNROLL_DEFAULT ON )
endif()
option( LOOP_UNROLL "Loop unroll" ${LOOP_UNROLL_DEFAULT} )
if ( LOOP_UNROLL )
    add_definitions( -DLOOP_UNROLL )
endif()


set( EXTERNAL_LIBS "${CMAKE_SOURCE_DIR}/../libs" )


include(ExternalProject)

include_directories( . )
include_directories( SYSTEM ${EXTERNAL_LIBS} )


if( CMAKE_TESTING_ENABLED )
    ##
    ## Google Test advises to build it inside project's cmake list to have the same compiler flags.
    ##
    
    ExternalProject_Add( googletest
                         SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libs/googletest-release-1.8.1/googletest
                         PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
                         INSTALL_COMMAND "" 
                         )
                         
    ExternalProject_Get_Property(googletest source_dir binary_dir)
    set( gtest_source_dir ${source_dir} )
    include_directories( SYSTEM ${gtest_source_dir}/include )
                         
    ExternalProject_Get_Property(googletest binary_dir)
    set( gtest_binary_dir ${binary_dir} )
    
    add_library(gtest UNKNOWN IMPORTED)
    set_target_properties( gtest PROPERTIES IMPORTED_LOCATION ${gtest_binary_dir}/libgtest.a )
    add_dependencies(gtest googletest)
    
    add_library(gtest_main UNKNOWN IMPORTED)
    set_target_properties( gtest_main PROPERTIES IMPORTED_LOCATION ${gtest_binary_dir}/libgtest_main.a )
    add_dependencies(gtest_main googletest)
    
    set( GTEST_LIBRARIES gtest gtest_main pthread )
    
    include_directories( test_utils )
endif()


add_subdirectories_recursive()
