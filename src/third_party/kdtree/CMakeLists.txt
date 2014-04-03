
include_directories( . )

file( GLOB kdtree_src *.c ) 
file( GLOB kdtree_hdr *.h ) 
add_library( kdtree STATIC ${kdtree_hdr} ${kdtree_src} )

if(ENABLE_SOLUTION_FOLDERS)
   set_target_properties(kdtree PROPERTIES FOLDER "third_party")
endif(ENABLE_SOLUTION_FOLDERS)

