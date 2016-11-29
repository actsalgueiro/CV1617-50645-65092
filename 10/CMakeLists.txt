# Exec1
add_executable( templateMatching templateMatching.cpp )
target_link_libraries( templateMatching ${OpenCV_LIBS} )

# Exec2 face detection
# python face_detection.py

# Exec2 face recognition
# Falta o ficheiro csv que nao foi possivel obter
add_executable( face_recognition face_recognition.cpp )
target_link_libraries( face_recognition ${OpenCV_LIBS} )

