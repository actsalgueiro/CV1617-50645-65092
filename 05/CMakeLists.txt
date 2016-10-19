add_executable( chessboard chessboard.cpp )
target_link_libraries( chessboard ${OpenCV_LIBS} )

add_executable( projectPoints projectPoints.cpp )
target_link_libraries( projectPoints ${OpenCV_LIBS} )

