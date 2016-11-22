CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT( view )
FIND_PACKAGE( OpenCV REQUIRED )

# Exec 1
# lucasKanade.py
# Exec 2
ADD_EXECUTABLE( background_separation background_separation.cpp )
# Exec 3
ADD_EXECUTABLE( object_tracker object_tracker.cpp )

TARGET_LINK_LIBRARIES( background_separation ${OpenCV_LIBS} )
TARGET_LINK_LIBRARIES( object_tracker ${OpenCV_LIBS} )

