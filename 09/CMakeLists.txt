CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

# IMPORTANTE PARA REMOVER O ERRO PCLConfigMake.cmake
set(PCL_DIR "/usr/local/pcl-1.8.0/share/pcl-1.8")

PROJECT( view )
FIND_PACKAGE( OpenCV REQUIRED )
FIND_PACKAGE( PCL 1.2 REQUIRED )

# Exec 1
#ADD_EXECUTABLE( mapDisparity mapDisparity.cpp )
# Exec 2
#ADD_EXECUTABLE( 3DReconstruction 3DReconstruction.cpp )
# Exec 3
#ADD_EXECUTABLE( viewCloud viewcloud.cpp )

include_directories( ${PCL_INCLUDE_DIRS} )
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

#TARGET_LINK_LIBRARIES( mapDisparity ${OpenCV_LIBS} )
#TARGET_LINK_LIBRARIES( 3DReconstruction ${OpenCV_LIBS} )
#TARGET_LINK_LIBRARIES( viewCloud ${OpenCV_LIBS} )
#TARGET_LINK_LIBRARIES( viewCloud ${PCL_LIBRARIES} )
#TARGET_LINK_LIBRARIES( PCD_3D_Format ${PCL_LIBRARIES} )
#TARGET_LINK_LIBRARIES( ICP_alignment ${PCL_LIBRARIES} )

#TARGET_LINK_LIBRARIES( viewOpenni ${OpenCV_LIBS} ${PCL_LIBRARIES} )
#TARGET_LINK_LIBRARIES( viewFreenect ${OpenCV_LIBS} ${PCL_LIBRARIES} freenect)

