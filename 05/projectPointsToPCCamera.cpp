/***********************************************************************************
Name:           projectPointsToPCCamera.cpp
Revision:
Author:         Paulo Dias
Comments:       ChessBoard Tracking


images
Revision:
Libraries:
***********************************************************************************/
#include <iostream>
#include <vector>
#include <string>

// OpenCV Includes
#include <stdio.h>
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
  //int board_size = board_w * board_h;
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
	printf("\n Number of corners: %lu",corners->size());
	//cv::waitKey(0); 
  }
  return corners->size();
}

// Same as the method above but does not display the corners
int FindChessboard(cv::Mat image,int board_w,int board_h, std::vector<cv::Point2f> *corners)
{
  //int board_size = board_w * board_h;
  CvSize board_sz = cvSize(board_w,board_h);

  cv::Mat grey_image;

  cv::cvtColor(image, grey_image, CV_BGR2GRAY);
  
  // find chessboard corners
  bool found = cv::findChessboardCorners(grey_image, board_sz, *corners,0);
  
  return corners->size();
}


int main(int argc, char **argv)
{
  // ChessBoard Properties
  int n_boards = 10; //Number of images
  int board_w = 9;
  int board_h = 6;

  int board_sz = board_w * board_h;

  char filename[200];

  // Chessboard coordinates and image pixels
  std::vector<std::vector<cv::Point3f> > object_points;
  std::vector<std::vector<cv::Point2f> > image_points;

  // Corners detected in each image
  std::vector<cv::Point2f> corners;
  
  int corner_count;

  cv::Mat image;
  int i;

  int sucesses = 0;
	 
  // Variables for calibration
  Mat intrinsic = Mat(3, 3, CV_32FC1);
  Mat distCoeffs;

  
  cv::VideoCapture cap(0);


  std::vector<cv::Point3f> obj;
  for(int j=0;j<board_sz;j++)
    obj.push_back(cv::Point3f(float(j/board_w), float(j%board_w), 0.0));
  



  // Access Video
  if(!cap.isOpened()){
    std::cout << "Cannot open the video file"  << endl;
    getchar();
    return -1;
  }
  
  // Get 10 images for calibration
  for(int i = 0; i < 10; i++){
    for(;;){
      cap >> image; // get a new image from camera
      imshow("cam",image);
      if(cv::waitKey(30) >= 0) break;
    }
    if(!image.data) 
    {
      printf("\nCould not load image file: %s\n",filename);
      getchar();
      return 0;
    }

    // find and display corners
    corner_count = FindAndDisplayChessboard(image,board_w,board_h,&corners);
      
    if (corner_count == board_w * board_h)
      {
        image_points.push_back(corners);
      object_points.push_back(obj);
        sucesses++;
      }
  }

  // Calibrate the camera


  // Write the params to file
  FileStorage fs("../CamParams.xml", FileStorage::WRITE);
  fs << "cameraMatrix" << intrinsic << "distCoeffs" << distCoeffs;
  fs.release();

  // New points to project
  vector<Point3f> pointsToProject;
  pointsToProject.push_back(Point3f(0,0,0));
  pointsToProject.push_back(Point3f(0,0,1));
  pointsToProject.push_back(Point3f(0,1,0));
  pointsToProject.push_back(Point3f(0,1,1));
  pointsToProject.push_back(Point3f(1,0,0));  // 4
  pointsToProject.push_back(Point3f(1,0,1));
  pointsToProject.push_back(Point3f(1,1,0));
  pointsToProject.push_back(Point3f(1,1,1));
  
  // Project Points to the camera
  for(;;){

        cap >> image; // get a new image from camera

  std::vector<std::vector<cv::Point3f> > object_points;
  std::vector<std::vector<cv::Point2f> > image_points;

    // find and display corners
    corner_count = FindChessboard(image,board_w,board_h,&corners);
      
    if (corner_count == board_w * board_h)
      {
        image_points.push_back(corners);
      object_points.push_back(obj);
      }

    vector<Mat> rvecs;
    vector<Mat> tvecs;
    
    // Try to obtain another image in case one fails
    if(image_points.size() == 0)
      continue;

    calibrateCamera(object_points, image_points, Size(board_w, board_h), intrinsic, distCoeffs, rvecs,tvecs);


    // Get the coordinates for the points projected into the image
    vector<Point2f> projectedPoints;
    projectPoints(pointsToProject, rvecs.at(0), tvecs.at(0), intrinsic, distCoeffs, projectedPoints);
    
    // Draw the line in the image
    line(image, projectedPoints.at(0), projectedPoints.at(1), Scalar(255,0,0), 2, 0);
    line(image, projectedPoints.at(0), projectedPoints.at(2), Scalar(255,0,0), 2, 0);
    line(image, projectedPoints.at(0), projectedPoints.at(4), Scalar(255,0,0), 2, 0);
    line(image, projectedPoints.at(4), projectedPoints.at(6), Scalar(255,0,0), 2, 0);
    line(image, projectedPoints.at(4), projectedPoints.at(5), Scalar(255,0,0), 2, 0);
    line(image, projectedPoints.at(2), projectedPoints.at(6), Scalar(255,0,0), 2, 0);
    line(image, projectedPoints.at(2), projectedPoints.at(3), Scalar(255,0,0), 2, 0);
    line(image, projectedPoints.at(6), projectedPoints.at(7), Scalar(255,0,0), 2, 0);
    line(image, projectedPoints.at(1), projectedPoints.at(3), Scalar(255,0,0), 2, 0);
    line(image, projectedPoints.at(1), projectedPoints.at(5), Scalar(255,0,0), 2, 0);
    line(image, projectedPoints.at(7), projectedPoints.at(3), Scalar(255,0,0), 2, 0);
    line(image, projectedPoints.at(7), projectedPoints.at(5), Scalar(255,0,0), 2, 0);

    // Showing on the same window as the other ones
    imshow("Calibration", image);
    
    if(waitKey(30) >= 0) break;

  }

  return 0;
}
