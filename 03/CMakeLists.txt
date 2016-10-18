add_executable( CannyEdgeDetector CannyEdgeDetector.cpp )
target_link_libraries( CannyEdgeDetector ${OpenCV_LIBS} )

add_executable( Sobel Sobel.cpp )
target_link_libraries( Sobel ${OpenCV_LIBS} )

add_executable( Scharr Scharr.cpp )
target_link_libraries( Scharr ${OpenCV_LIBS} )

add_executable( Laplacian Laplacian.cpp )
target_link_libraries( Laplacian ${OpenCV_LIBS} )

add_executable( HarrisCorner HarrisCorner.cpp )
target_link_libraries( HarrisCorner ${OpenCV_LIBS} )

#add_executable( FindingContours FindingContours.cpp )
#target_link_libraries( FindingContours ${OpenCV_LIBS} )

