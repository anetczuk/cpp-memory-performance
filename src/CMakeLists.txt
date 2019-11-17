##
##
##

cmake_minimum_required(VERSION 3.1.1)


if( "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}" )
    message( FATAL_ERROR "Building in source is forbidden. Change output directory.")
endif()


project( memory-performance )


if( NOT CMAKE_BUILD_TYPE )
    ##set( CMAKE_BUILD_TYPE "Debug")
    set( CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set( CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path prefix" FORCE )
endif()


##
## set default compilation standard
##
if (CMAKE_VERSION VERSION_LESS "3.1")
    ## handled by gcc and clang
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c14" )
else()
    ## supported by CMake 3.1 and above
    set( CMAKE_C_STANDARD 99 )
    set( CMAKE_C_STANDARD_REQUIRED ON )
    set( CMAKE_CXX_STANDARD 14 )
    set( CMAKE_CXX_STANDARD_REQUIRED ON )
endif()


##
## compiler flags, gcc does not handle "appended" values
##
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-long-long" )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra" )
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long" )


##
## option for code coverage
##
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    option(CODE_COVERAGE "Code coverage (for GCC)" OFF )
    if ( CODE_COVERAGE )
        ## to check compilation parameters run "make VERBOSE=1"
        ## to test is program is compiled properly call "strings test.exe | grep gcda"
        
        set( COVERAGE_COMPILER_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage" )
        set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}" )
    endif()
    
    option(GPROF_ANALYSIS "GProf analysis (for GCC)" OFF )
    if ( GPROF_ANALYSIS )
        set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg" )
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    option(CODE_COVERAGE "Code coverage (for GCC)" OFF )
    if ( CODE_COVERAGE )
        ## to check compilation parameters run "make VERBOSE=1"
        ## to test is program is compiled properly call "strings test.exe | grep gcda"
        
        set( COVERAGE_COMPILER_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage" )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}" )
    endif()
    
    option(GPROF_ANALYSIS "GProf analysis (for GCC)" OFF )
    if ( GPROF_ANALYSIS )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg" )
    endif()
endif()


##
## option for clang sanitizers
##
if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    ## other sanitizers (memory, leak) does not work properly on clang 3.5
    option( ADDRESS_SANITIZER "Address sanitizer (for clang)" OFF )
    if( ADDRESS_SANITIZER )
        set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    ## other sanitizers (memory, leak) does not work properly on clang 3.5
    option( ADDRESS_SANITIZER "Address sanitizer (for clang)" OFF )
    if( ADDRESS_SANITIZER )
        set( CMAKE_CXX_COMPILER_ID "${CMAKE_CXX_COMPILER_ID} -fsanitize=address -fno-omit-frame-pointer")
    endif()
endif()


##
## enable CTest utility
##
enable_testing()



## ================= macros =================


function(generate_file sourceFile targetFile)
    set( GENERATOR_FILE "${sourceFile}" )
    configure_file( ${sourceFile} ${targetFile} @ONLY )
endfunction()


macro(current_dir_name output_var)
    get_filename_component( ${output_var} ${CMAKE_CURRENT_SOURCE_DIR} NAME)
endmacro()


function(add_subdirectories_recursive)
    ## returns files in alphabetic order
    set( subdirs_list )
    file(GLOB_RECURSE cmake_files */CMakeLists.txt )
    foreach(item ${cmake_files})
        get_filename_component( dir_path ${item} DIRECTORY)
        
        set( is_found -1 )
        foreach(subdir ${subdirs_list})
            string(FIND "${dir_path}" "${subdir}" "is_found" )
            if ( ${is_found} GREATER "-1" )
                ## found
                break()
            endif()
        endforeach()
        
        if ( ${is_found} LESS "0" )
            ## new path
            list(APPEND subdirs_list ${dir_path} )
        endif()
    endforeach()
    
    ## message("found dirs: ${subdirs_list}")
    foreach(item ${subdirs_list})
        add_subdirectory( ${item} )
    endforeach()
endfunction()


function(build_execs_list project_name )
    file(GLOB cpp_files *.cpp)
    build_execs_list( "${project_name}" "${cpp_files}" )
endfunction()


function(build_execs_list project_name cpp_files)
    foreach(single_cpp ${cpp_files})
        get_filename_component( cpp_name ${single_cpp} NAME_WE)
        if("${cpp_name}" MATCHES "^test*")
            continue()
        endif()
        set( TARGET_NAME ${project_name}-${cpp_name} )
        add_executable( ${TARGET_NAME} ${single_cpp} )
        target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )
        set_target_properties( ${TARGET_NAME} PROPERTIES OUTPUT_NAME ${cpp_name})
    endforeach()
endfunction()


function(build_tests project_name ext_libs)
    if( NOT CMAKE_TESTING_ENABLED )
        return()
    endif()
    
    file(GLOB cpp_files test_*.cpp)
    
    foreach(single_cpp ${cpp_files})
        get_filename_component( cpp_name ${single_cpp} NAME_WE)
        set( TARGET_NAME ${project_name}-${cpp_name} )
        add_executable( ${TARGET_NAME} ${single_cpp} )
        #add_dependencies( ${TARGET_NAME} gtest )
        target_link_libraries( ${TARGET_NAME} ${ext_libs} )
        set_target_properties( ${TARGET_NAME} PROPERTIES OUTPUT_NAME ${cpp_name})
        add_test(
            NAME ${TARGET_NAME} 
            COMMAND ${cpp_name}
        )
    endforeach()
endfunction()


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
