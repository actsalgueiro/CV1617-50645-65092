#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
/*
int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat grayImg;

    namedWindow("original",1);
    namedWindow("RGB histogram",2);
    namedWindow("Gray histogram", 2);


    for(;;)
    {
        Mat frame;

        cap >> frame; // get a new frame from camera
        cvtColor(frame, grayImg, CV_BGR2GRAY);


        // Adapted from http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
        // Separate the image in 3 places ( B, G and R )
        vector<Mat> bgr_planes;
        split(frame, bgr_planes );

        // Establish the number of bins
        int histSize = 256;

        // Set the ranges ( for B,G,R) )
        float range[] = { 0, 256 } ;
        const float* histRange = { range };

        bool uniform = true; bool accumulate = false;

        Mat b_hist, g_hist, r_hist;
        Mat grayHist;

        // Compute the histograms:
        calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &grayImg, 1, 0, Mat(), grayHist, 1, &histSize, &histRange, uniform, accumulate );

        // Draw the histograms for B, G and R
        int hist_w = 512; int hist_h = 400;
        int bin_w = cvRound( (double) hist_w/histSize );

        Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
        Mat gHistImage( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );

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
            line( gHistImage, Point( bin_w*(i-1), hist_h - cvRound(grayHist.at<float>(i-1)) ) ,
                                         Point( bin_w*(i), hist_h - cvRound(grayHist.at<float>(i)) ),
                                         Scalar( 158, 158, 158), 2, 8, 0  );
        }

        imshow("original", frame);
        imshow("RGB histogram", histImage);
        imshow("Gray histogram", gHistImage);

        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}*/
