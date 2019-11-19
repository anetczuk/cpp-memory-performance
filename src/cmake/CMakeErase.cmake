##
## Create "erase" target for make that removes all files except CMakeCache.txt
##



if (UNIX)
    ### implementation of erasing files and directories for Linux
    
    ## add_custom_target( erase bash -c "find . ! -name 'CMakeCache.txt' -print" COMMENT "Erasing all build files" VERBATIM )
    
    add_custom_target( erase bash -c "find . ! -name 'CMakeCache.txt' ! -name '.' ! -name '..' -exec rm -rf {} +" COMMENT "Erasing all build files" VERBATIM )
    
elseif (WIN32)

    message( "make 'erase' not implemented" )
    
endif ()

