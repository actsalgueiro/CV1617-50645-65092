#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

#include <pcl/visualization/cloud_viewer.h>

int  main (int argc, char** argv)
{
  
  // Filt_office1
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud1 (new pcl::PointCloud<pcl::PointXYZRGB>);
  if (pcl::io::loadPCDFile<pcl::PointXYZRGB> ("..//images//07//filt_office1.pcd", *cloud1) == -1) //* load the file
  {
    PCL_ERROR ("Couldn't read file filt_office1.pcd \n");
    return (-1);
  }


  // Filt_office2
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud2 (new pcl::PointCloud<pcl::PointXYZRGB>);
  if (pcl::io::loadPCDFile<pcl::PointXYZRGB> ("..//images//07//filt_office2.pcd", *cloud2) == -1) //* load the file
  {
    PCL_ERROR ("Couldn't read file filt_office2.pcd \n");
    return (-1);
  }
  
  pcl::visualization::CloudViewer viewer("Simple Cloud Viewer");
  viewer.showCloud(cloud1);
  viewer.showCloud(cloud2);
  
  while (!viewer.wasStopped())
    {
    }
 
  return (0);
}