# Exec1
add_executable( chessboardStereo chessboardStereo.cpp )
target_link_libraries( chessboardStereo ${OpenCV_LIBS} )

# Exec2
add_executable( stereoCalibration stereoCalibration.cpp )
target_link_libraries( stereoCalibration ${OpenCV_LIBS} )

# Exec3
add_executable( lensDistortion lensDistortion.cpp )
target_link_libraries( lensDistortion ${OpenCV_LIBS} )

# Exec5 usando o reconstruct da aula 07
add_executable( imageRectification imageRectification_reconstruct.cpp )
target_link_libraries( imageRectification ${OpenCV_LIBS} )
