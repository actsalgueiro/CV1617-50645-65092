# Exec1
add_executable( ex11_houghlines ex11_houghlines.cpp )
target_link_libraries( ex11_houghlines ${OpenCV_LIBS} )

# Exec2
add_executable( ex11_houghcircles ex11_houghcircles.cpp )
target_link_libraries( ex11_houghcircles ${OpenCV_LIBS} )

# Exec3
add_executable( SURF_FlannMatcher SURF_FlannMatcher.cpp )
target_link_libraries( SURF_FlannMatcher ${OpenCV_LIBS} )


