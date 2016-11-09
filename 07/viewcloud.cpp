#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

#include <pcl/visualization/cloud_viewer.h>


#include <vector>

#include <stdio.h>

// OpenCV Includes
#include <opencv2/core/core.hpp>        
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int  main (int argc, char** argv)
{
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

  (*cloud).width    = 50;
  (*cloud).height   = 50;
  (*cloud).is_dense = false;
  (*cloud).points.resize ((*cloud).width * (*cloud).height);


  FileStorage fs("..//3dCoordinates.xml", cv::FileStorage::READ);
  if (!fs.isOpened() )
     {
       std::cout << "Failed to open 3dCoordinates.xml" << std::endl;
       return 1;
     }

  Mat pos3D;
  fs["coordinates"] >>  pos3D;

  int p=0;
  for (int i=0;i< (*cloud).height; i++)
    for (int j=0;j< (*cloud).width; j++)
    {
      (*cloud).points[p].x = pos3D.at<cv::Vec3f>(i,j)[0];
      (*cloud).points[p].y = pos3D.at<cv::Vec3f>(i,j)[1];
      (*cloud).points[p].z = pos3D.at<cv::Vec3f>(i,j)[2];
      p++;
    }

 /*
  for (size_t i = 0; i < (*cloud).points.size (); ++i)
  {
    (*cloud).points[i].x = 10 * rand () / (RAND_MAX + 1.0f);
    (*cloud).points[i].y = 10 * rand () / (RAND_MAX + 1.0f);
    (*cloud).points[i].z = 10 * rand () / (RAND_MAX + 1.0f);
  }
*/
  
  pcl::visualization::CloudViewer viewer("Simple Cloud Viewer");
  viewer.showCloud(cloud);
  
  while (!viewer.wasStopped())
    {
    }
 
  return (0);
}