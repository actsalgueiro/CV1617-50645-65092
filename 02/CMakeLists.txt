add_executable( Example01 Example01.cpp )
target_link_libraries( Example01 ${OpenCV_LIBS} )

add_executable( DisplayImage DisplayImage.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )

add_executable( DisplayAndSaveImage DisplayAndSaveImage.cpp )
target_link_libraries( DisplayAndSaveImage ${OpenCV_LIBS} )

add_executable( AddingTwo AddingTwo.cpp )
target_link_libraries( AddingTwo ${OpenCV_LIBS} )

add_executable( Contrast Contrast.cpp )
target_link_libraries( Contrast ${OpenCV_LIBS} )
