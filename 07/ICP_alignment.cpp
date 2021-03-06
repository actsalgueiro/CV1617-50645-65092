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
  
  
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr aligned_cloud (new pcl::PointCloud<pcl::PointXYZRGB>);
  pcl::IterativeClosestPoint<pcl::PointXYZRGB, pcl::PointXYZRGB> icp;
  
  icp.setTransformationEpsilon (1e-6);
  icp.setMaxCorrespondenceDistance (0.25);
  icp.setMaximumIterations (50);
  icp.setInputCloud (cloud2);
  icp.setInputTarget (cloud1);
  icp.align (*aligned_cloud);

  pcl::visualization::CloudViewer viewer("Simple Cloud Viewer");
  viewer.showCloud(cloud1);
  viewer.showCloud(cloud2);
  viewer.showCloud(aligned_cloud);
  
  while (!viewer.wasStopped())
    {
    }
 
  return (0);
}