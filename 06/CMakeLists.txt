# Exec1
add_executable( chessboardStereo chessboardStereo.cpp )
target_link_libraries( chessboardStereo ${OpenCV_LIBS} )

# Exec2
add_executable( stereoCalibration stereoCalibration.cpp )
target_link_libraries( stereoCalibration ${OpenCV_LIBS} )
