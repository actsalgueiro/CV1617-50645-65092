#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

/**
 * @function main
 */
int main(int, char**)
{
    

    VideoCapture cap(0);
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    for(;;)
    {

      Mat src, dst, gray;

      cap >> src;

      cvtColor(src, gray, CV_BGR2GRAY);

      /// Separate the image in 3 places ( B, G and R )
      vector<Mat> bgr_planes;
      split( src, bgr_planes );

      vector<Mat> gray_planes;
      split( gray, gray_planes );

      /// Establish the number of bins
      int histSize = 256;

      /// Set the ranges ( for B,G,R) )
      float range[] = { 0, 256 } ;
      const float* histRange = { range };

      bool uniform = true; bool accumulate = false;

      Mat b_hist, g_hist, r_hist, gray_hist;

      /// Compute the histograms:
      calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
      calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
      calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

      calcHist( &gray_planes[0], 1, 0, Mat(), gray_hist, 1, &histSize, &histRange, uniform, accumulate );

      // Draw the histograms for B, G and R
      int hist_w = 512; int hist_h = 400;
      int bin_w = cvRound( (double) hist_w/histSize );

      Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
      Mat histImageGray( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

      /// Normalize the result to [ 0, histImage.rows ]
      normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
      normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
      normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

      normalize(gray_hist, gray_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

      /// Draw for each channel
      for( int i = 1; i < histSize; i++ )
      {
          line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                           Scalar( 255, 0, 0), 2, 8, 0  );
          line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                           Scalar( 0, 255, 0), 2, 8, 0  );
          line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                           Scalar( 0, 0, 255), 2, 8, 0  );
      }

      /// Draw for each channel
      for( int i = 1; i < histSize; i++ )
      {
          line( histImageGray, Point( bin_w*(i-1), hist_h - cvRound(gray_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(gray_hist.at<float>(i)) ),
                           Scalar( 255, 255, 255), 2, 8, 0  );
      }

      /// Display
      namedWindow("calcHist RGB", CV_WINDOW_AUTOSIZE );
      namedWindow("calcHist GRAY", CV_WINDOW_AUTOSIZE );
      namedWindow("Original", 1);
      namedWindow("BW", 1);
      resize(src, src, Size(400,300));
      resize(gray, gray, Size(400,300));
      imshow("Original", src);
      imshow("BW", gray);      
      imshow("calcHist RGB", histImage );
      imshow("calcHist GRAY", histImageGray );

      if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}