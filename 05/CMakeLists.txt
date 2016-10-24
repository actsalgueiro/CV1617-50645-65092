# Exec1
add_executable( chessboard chessboard.cpp )
target_link_libraries( chessboard ${OpenCV_LIBS} )

# Exec2
add_executable( projectPoints projectPoints.cpp )
target_link_libraries( projectPoints ${OpenCV_LIBS} )

# Exec3
add_executable( projectPointsToPCCamera projectPointsToPCCamera.cpp )
target_link_libraries( projectPointsToPCCamera ${OpenCV_LIBS} )

# Exec4
add_executable( externalCameraCalibration externalCameraCalibration.cpp )
target_link_libraries( externalCameraCalibration ${OpenCV_LIBS} )

