
#include "opencv2/opencv.hpp"

using namespace cv;

Mat src, src_gray;
int thresh = 200;
int max_thresh = 255;

char* source_window = "Source image";
char* corners_window = "Corners detected";

void cornerHarris_demo( int, void* );

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;



    for(;;)
    {

        createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
        Mat frame;
        cap >> src; // get a new frame from camera
        //GaussianBlur(frame, src, Size(3,3), 0, 0, BORDER_DEFAULT );
        cvtColor( src, src_gray, CV_BGR2GRAY );

        /// Showing the result
        cornerHarris_demo( 0, 0 );

        if(waitKey(30) >= 0) break;
        imshow("Source", src);
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

void cornerHarris_demo( int, void* )
{

  Mat dst, dst_norm, dst_norm_scaled;
  dst = Mat::zeros( src.size(), CV_32FC1 );

  /// Detector parameters
  int blockSize = 2;
  int apertureSize = 3;
  double k = 0.04;

  /// Detecting corners
  cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

  /// Normalizing
  normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
  convertScaleAbs( dst_norm, dst_norm_scaled );

  /// Drawing a circle around corners
  for( int j = 0; j < dst_norm.rows ; j++ )
     { for( int i = 0; i < dst_norm.cols; i++ )
          {
            if( (int) dst_norm.at<float>(j,i) > thresh )
              {
               circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
              }
          }
     }
  /// Showing the result
  namedWindow( corners_window, CV_WINDOW_AUTOSIZE );
  imshow( corners_window, dst_norm_scaled );
}
