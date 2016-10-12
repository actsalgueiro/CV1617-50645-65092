add_executable( videoCapture videoCapture.cpp )
target_link_libraries( videoCapture ${OpenCV_LIBS} )

add_executable( SkinDetection SkinDetection.cpp )
target_link_libraries( SkinDetection ${OpenCV_LIBS} )

add_executable( Filters Filters.cpp )
target_link_libraries( Filters ${OpenCV_LIBS} )

add_executable( RGB_Histogram RGB_Histogram.cpp )
target_link_libraries( RGB_Histogram ${OpenCV_LIBS} )

add_executable( HistogramCompare HistogramCompare.cpp )
target_link_libraries( HistogramCompare ${OpenCV_LIBS} )