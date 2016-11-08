/***********************************************************************************
Name:           imageRectification.cpp
Revision:
Date:           05-10-2013
Author:         Paulo Dias
Comments:       ChessBoard Tracking


images
Revision:
Libraries:
Notes:          Code generated with Visual Studio 2013 Intel OpenCV 2.4.8 libraries 
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

// Function FindAndDisplayChessboard
// find corners in a cheesboard with board_w x board_h dimensions
// Display the corners in image and return number of detected corners
int FindAndDisplayChessboard(cv::Mat image,int board_w,int board_h, std::vector<cv::Point2f> *corners)
{
  int board_size = board_w * board_h;
  CvSize board_sz = cvSize(board_w,board_h);

  cv::Mat grey_image;

  cv::cvtColor(image, grey_image, CV_BGR2GRAY);
  
  // find chessboard corners
  bool found = cv::findChessboardCorners(grey_image, board_sz, *corners,0);
  
  // Draw results
  if (true)
  {
	cv::drawChessboardCorners(image, board_sz, cv::Mat(*corners), found);
	cv::imshow("Calibration",image);
	cv::waitKey(0); 
  }
  return corners->size();
}

int main(int argc, char **argv)
{
  // ChessBoard Properties
  int n_boards = 13; //Number of images
  int board_w = 9;
  int board_h = 6;

  int board_sz = board_w * board_h;

  char filename[200];

  // Chessboard coordinates and image pixels
  std::vector<std::vector<cv::Point3f> > object_points;
  std::vector<std::vector<cv::Point2f> > image_pointsL;
  std::vector<std::vector<cv::Point2f> > image_pointsR;

  // Variables for calibration
  Mat intrinsic1 = Mat(3, 3, CV_32FC1);
  Mat intrinsic2 = Mat(3, 3, CV_32FC1);
  Mat distCoeffs1;
  Mat distCoeffs2;
  Mat R, E, T, F;

  // Parameters for rectification
  Mat map1x;
  Mat map1y;
  Mat map2x;
  Mat map2y;

  Mat R1;
  Mat R2;

  Mat P1;
  Mat P2;
  Mat Q;

  // Corners detected in each image
  std::vector<cv::Point2f> corners;
  
  int corner_count;

  cv::Mat image;
  int i;

  int sucesses = 0;
	 

  // Read calibration information
  FileStorage fs("../stereoParams.xml", FileStorage::READ);

  if(!fs.isOpened()){
    cerr << "Failed to open stereoParams.xml" << endl;
    return 1;
  }

  fs["cameraMatrix_1"] >> intrinsic1;
  fs["distCoeffs_1"] >> distCoeffs1;
  fs["cameraMatrix_2"] >> intrinsic2;
  fs["distCoeffs_2"] >> distCoeffs2;
  fs["rotation"] >> R;
  fs["translation"] >> T;
  fs["essential"] >> E;
  fs["fundamental"] >> F;


  // chessboard coordinates
  std::vector<cv::Point3f> obj;
  for(int j=0;j<board_sz;j++)
    obj.push_back(cv::Point3f(float(j/board_w), float(j%board_w), 0.0));
  
  // IMAGE RECTIFICATION
  // Transforms both image planes into the same plane
  stereoRectify(intrinsic1, distCoeffs1, intrinsic2, distCoeffs2, Size(board_w,board_h), R, T, R1, R2, P1, P2, Q);

  // Computes the transformation between the original and the rectified image
  initUndistortRectifyMap(intrinsic1, distCoeffs1, R1, P1, Size(board_w,board_h),
   CV_32FC1, // Type of the first output map that can be CV_32FC1 or CV_16SC2
    map1x, map2x);

  printf("\n Not working - Undistorted Image not showing! \n");
  for (i=0;i<n_boards;i++)
  {
    Mat undistortedImage;

    // read left image 
    sprintf(filename,"..//images//06//left%02d.jpg",i+1);
    printf("\nReading %s",filename);
    image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
    
    if(!image.data) 
    {
      printf("\nCould not load image file: %s\n",filename);
      getchar();
      return 0;
    }
    
    // IMAGE RECTIFICATION
    // Apply the transformation between the two images
    remap(image, undistortedImage, map1x, map2x, INTER_LINEAR);

    imshow("Left Image (Original)", image);
    imshow("Undistorted", undistortedImage);
    printf("\nPress key for the next image\n");
    cv::waitKey(0); 
  }

  return 0;
}
