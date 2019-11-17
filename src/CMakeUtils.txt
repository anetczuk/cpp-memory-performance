##
## various functions and macros
##

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
