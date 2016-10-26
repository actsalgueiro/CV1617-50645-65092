/***********************************************************************************
Name:           chessboard.cpp
Revision:
Author:         Paulo Dias
Comments:       ChessBoard Tracking


images
Revision:
Libraries:
***********************************************************************************/
#include <iostream>
#include <vector>

// OpenCV Includes
#include <stdio.h>
#include <opencv2/core/core.hpp>        
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

// Function FindAndDisplayChessboard
// find corners in a cheesboard with board_w x board_h dimensions
// Display the corners in image and return number of detected corners
int FindAndDisplayChessboard(Mat image,int board_w,int board_h, std::vector<Point2f> *corners)
{
  int board_size = board_w * board_h;
  CvSize board_sz = cvSize(board_w,board_h);

  Mat grey_image;


  cvtColor(image, grey_image, CV_BGR2GRAY);

  TermCriteria criteria = TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.001 );
  
  // find chessboard corners
  bool found = findChessboardCorners(grey_image, board_sz, *corners,0);
  vector<Point2f> &c = *corners;

  /// Write them down
  cout << "\nBefore: " << endl;
  for( int i = 0; i < c.size(); i++ )
     { cout<<" -- Corner ["<<i<<"]  ("<<c[i].x<<","<<c[i].y<<")"<<endl; }

  // Refined corners 
  cornerSubPix(grey_image, *corners, Size(5,5), Size(-1,-1), criteria);
  /// Write them down
  cout << "After: " << endl;
  for( int i = 0; i < c.size(); i++ )
     { cout<<" -- Refined Corner ["<<i<<"]  ("<<c[i].x<<","<<c[i].y<<")"<<endl; }
  
  // Draw results
  if (true)
  {
  	drawChessboardCorners(image, board_sz, Mat(*corners), found);
  	imshow("Calibration",image);
  	printf("\n Number of corners: %lu",corners->size());
  	waitKey(0); 
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
  std::vector<std::vector<Point3f> > object_points;
  std::vector<std::vector<Point2f> > image_points;

  // Corners detected in each image
  std::vector<Point2f> corners;
  
  int corner_count;

  Mat image;
  int i;

  int sucesses = 0;

  Mat intrinsic = Mat(3, 3, CV_32FC1);
  Mat distCoeffs;
  std::vector<Mat> rvecs;
  std::vector<Mat> tvecs;

  // chessboard coordinates
  std::vector<Point3f> obj;
  for(int j=0;j<board_sz;j++)
    obj.push_back(Point3f(float(j/board_w), float(j%board_w), 0.0));
  
  for (i=0;i<n_boards;i++)
  {
    // read image 
    sprintf(filename,"..//images//05//left%02d.jpg",i+1);
    printf("\nReading %s",filename);
    image = imread(filename, CV_LOAD_IMAGE_COLOR);
    
    
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
    // Calibrate the camera
    calibrateCamera(object_points, image_points, Size(board_w, board_h), intrinsic, distCoeffs, rvecs,tvecs);

    /*
    // Shows results after calibrating the camera
    cout << endl << "Instrinsics = " << endl << "" << intrinsic << endl << endl;
    cout << endl << "Distortion = " << endl << "" << distCoeffs   << endl << endl;

    cout << endl << endl << "Translations = ";

    for(i = 0; i < n_boards; i++)
      cout << endl << tvecs.at(i);

    cout << endl << endl << "Rotations = ";
    for(i = 0; i < n_boards; i++)
      cout << endl << rvecs.at(i);
      */
  }
  return 0;
}
