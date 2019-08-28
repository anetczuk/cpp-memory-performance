#
#
#

cmake_minimum_required(VERSION 3.0.2)


if( "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}" )
    message( FATAL_ERROR "Building in source is forbidden. Change output directory.")
endif()


project( optimization-exercises )


if( NOT CMAKE_BUILD_TYPE )
    ##set( CMAKE_BUILD_TYPE "Debug")
    set( CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set( CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path prefix" FORCE )
endif()


## set default compilation standard
if (CMAKE_VERSION VERSION_LESS "3.1")
    ## handled by gcc and clang
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99" )
else()
    ## supported by CMake 3.1 and above
    set( CMAKE_C_STANDARD 99 )
    set( CMAKE_C_STANDARD_REQUIRED ON )
endif()

## compiler flags, gcc does not handle "appended" values
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-long-long" )


## option for code coverage
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    option(CODE_COVERAGE "Code coverage (for GCC)" OFF )
    if ( CODE_COVERAGE )
        ## to check compilation parameters run "make VERBOSE=1"
        ## to test is program is compiled properly call "strings test.exe | grep gcda"
        
        set( COVERAGE_COMPILER_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage" )
        set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}" )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}" )
    endif()
    
    option(GPROF_ANALYSIS "GProf analysis (for GCC)" OFF )
    if ( GPROF_ANALYSIS )
        set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg" )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg" )
    endif()
else()
    message( STATUS "Code coverage unavailable for your compiler: '${CMAKE_C_COMPILER_ID}'" )
endif()


## option for clang sanitizers
if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    ## other sanitizers (memory, leak) does not work properly on clang 3.5
    option( ADDRESS_SANITIZER "Address sanitizer (for clang)" OFF )
    if( ADDRESS_SANITIZER )
        set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    endif()
endif()


## enable CTest utility
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


function(build_cpps project_name)
    file(GLOB cpp_files *.cpp)
    
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


function(build_tests project_name)
    if( NOT CMAKE_TESTING_ENABLED )
        return()
    endif()
    
    file(GLOB cpp_files test_*.cpp)
    
    foreach(single_cpp ${cpp_files})
        get_filename_component( cpp_name ${single_cpp} NAME_WE)
        set( TARGET_NAME ${project_name}-${cpp_name} )
        add_executable( ${TARGET_NAME} ${single_cpp} )
        target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )
        set_target_properties( ${TARGET_NAME} PROPERTIES OUTPUT_NAME ${cpp_name})
        add_test(
            NAME ${TARGET_NAME} 
            COMMAND ${cpp_name}
        )
    endforeach()
endfunction()


## ================= build =================


include_directories( . )

if( CMAKE_TESTING_ENABLED )
    include_directories( test_utils )
endif()


add_subdirectories_recursive()
