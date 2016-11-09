/***********************************************************************************
Name:           mapDisparity.cpp
Revision:
Date:           18-09-2012
Author:         Paulo Dias
Comments:       ChessBoard Tracking

images
Revision:
Libraries:
Notes:          Code generated with Visual Studio 2008 Intel OpenCV libraries
***********************************************************************************/
#include <iostream>
#include <vector>

#include <stdio.h>

// OpenCV Includes
#include <opencv2/core/core.hpp>        
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void mouseHandlerL(int event, int x, int y, int flags, void* param)
 {


   switch(event){
     case CV_EVENT_LBUTTONDOWN:
       cv::Mat* image = (cv::Mat*) param;
       std::cout << "pixel coordinates (Left)= " << x << "," << y << std::endl;
       std::vector<cv::Point2f> point;
       cv::line(*image,cvPoint(0,y),cvPoint(image->size().width,y),cvScalar(255), 1, 8, 0);
       //cv::imshow("Rectify_right",*image);
      break;
   }
 }


void mouseHandlerR(int event, int x, int y, int flags, void* param)
 {
   switch(event){
   case CV_EVENT_LBUTTONDOWN:
     cv::Mat* image = (cv::Mat*) param;
     std::cout << "pixel coordinates (Left)= " << x << "," << y << std::endl;
     std::vector<cv::Point2f> point;
     cv::line(*image,cvPoint(0,y),cvPoint(image->size().width,y),cvScalar(255), 1, 8, 0);
     //cv::imshow("Rectify_left",*image);
    break;
   }
 }


int main(int argc, char **argv)
{
  // Read camera parameters
  ///////////////////////
  // Calibrate stereo rig
  //Create first Intrinsic Camera Matrix and Distortion Matrix
  cv::Mat intrinsics1;
  cv::Mat distortion1;

  //Create second Intrinsic Camera Matrix and Distortion Matrix
  cv::Mat intrinsics2;
  cv::Mat distortion2;

  // Calibrate Stereo Rig
  cv::Mat rotation;
  cv::Mat translation;
  cv::Mat essential;
  cv::Mat fundamental;

  // 1- Variable definition
  // the preset has to do with the system configuration (basic, fisheye, etc.)
  // ndisparities is the size of disparity range,
  // in which the optimal disparity at each pixel is searched for.
  // SADWindowSize is the size of averaging window used to match pixel blocks
  // (larger values mean better robustness to noise, but yield blurry disparity maps)
  int ndisparities = 16*5;
  int SADWindowSize = 21;
  cv::Mat imgDisparity8U;
  cv::Mat imgDisparity16S;

  //-- 2. Call the constructor for StereoBM
  cv::Ptr<cv::StereoBM> sbm;
  sbm = cv::StereoBM::create( ndisparities, SADWindowSize );

  // reading parameters
  cv::FileStorage fs("..//stereoParams.xml", cv::FileStorage::READ);
  if (!fs.isOpened() )
     {
       std::cout << "Failed to open stereoParams.xml" << std::endl;
       return 1;
     }

  fs["cameraMatrix_1"] >>  intrinsics1;
  fs["cameraMatrix_2"] >>  intrinsics2;
  fs["distCoeffs_1"] >>  distortion1;
  fs["distCoeffs_2"] >>  distortion2;
  fs["rotation"] >>  rotation;
  fs["translation"] >>  translation;
  fs["essential"] >>  essential;
  fs["fundamental"] >>  fundamental;

  fs.release();

  char filename[200];

  sprintf(filename,"..//images//06//left%02d.jpg",1);
  cv::Mat  imagel = cv::imread(filename);
  if(!imagel.data)
  {
    printf("\nCould not load image file: %s\n",filename);
    return 0;
  }

  sprintf(filename,"..//images//06//right%02d.jpg",1);
  cv::Mat  imager = cv::imread(filename);
  if(!imager.data)
  {
    printf("\nCould not load image file: %s\n",filename);
    return 0;
  }

  cv::Mat map1x;
  cv::Mat map1y;
  cv::Mat map2x;
  cv::Mat map2y;

  cv::Mat R1;
  cv::Mat R2;

  cv::Mat P1;
  cv::Mat P2;
  cv::Mat Q;

  //std::cout << "\nStereoRectifyMap";
  cv::stereoRectify(intrinsics1, distortion1, intrinsics2, distortion2, imagel.size(), rotation, translation,R1, R2, P1, P2, Q,0);

  //std::cout << "\ncvInitUndistortRectifyMap";
  cv::initUndistortRectifyMap(intrinsics1, distortion1, R1, P1, imagel.size(), CV_32FC1, map1x, map1y);
  cv::initUndistortRectifyMap(intrinsics2, distortion2, R2, P2, imagel.size(), CV_32FC1, map2x,map2y);

  //std::cout << "\nRemap images";
  cv::Mat  gray_imagel;
  cv::Mat  remap_imgl;
  cv::cvtColor(imagel, gray_imagel , CV_RGB2GRAY);
  cv::remap(gray_imagel, remap_imgl, map1x, map1y,cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());

  cv::Mat  gray_imager;
  cv::Mat  remap_imgr;
  cv::cvtColor(imager, gray_imager , CV_RGB2GRAY);
  cv::remap(gray_imager, remap_imgr, map2x, map2y,cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());

  for (int i=0;i<remap_imgl.size().height;i=i+25)
  {
    cv::line(remap_imgl,cvPoint(0,i),cvPoint(remap_imgl.size().width,i),cvScalar(255), 1, 8, 0);
    cv::line(remap_imgr,cvPoint(0,i),cvPoint(remap_imgr.size().width,i),cvScalar(255), 1, 8, 0);
  }

  //cv::imshow("Rectify_left",remap_imgl);
  //cv::imshow("Rectify_right",remap_imgr);

  cv::setMouseCallback( "Rectify_left", mouseHandlerL,&remap_imgr);
  cv::setMouseCallback( "Rectify_right", mouseHandlerR,&remap_imgl);

 //-- 3. Calculate the disparity image
  sbm->compute( remap_imgl, remap_imgr, imgDisparity16S );//-- Check its extreme values
  double minVal; double maxVal;
  cv::minMaxLoc( imgDisparity16S, &minVal, &maxVal );
  printf("Min disp: %f Max value: %f \n", minVal, maxVal);
  
  //-- 4. Display it as a CV_8UC1 image
  //Display disparity as a CV_8UC1 image
  // the disparity will be 16-bit signed (fixed-point) or
  //32-bit floating-point image of the same size as left.
  imgDisparity16S.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));
  //namedWindow( "disparity", cv::WINDOW_NORMAL );
  //cv::imshow( "disparity", imgDisparity8U );

  Mat Image3D;
  reprojectImageTo3D(imgDisparity8U, Image3D, Q);

    FileStorage fw("../3dCoordinates.xml", FileStorage::WRITE);

  
  if(!fw.isOpened()){
    cout << "Failed to open 3dCoordinates.xml" << endl;
  }else
  cout << endl << "Writing 3d coordinates" << endl;

  // Write to file
  fw << "coordinates" << Image3D;

  fw.release();

  cv::waitKey(-1);    

  return 0;
  }
