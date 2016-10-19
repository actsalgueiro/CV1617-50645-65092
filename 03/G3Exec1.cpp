/*
 * Exec1.cpp
 *
 *  Created on: 28/09/2016
 *      Author: filipe
 */



#include "opencv2/opencv.hpp"

using namespace cv;
/*
int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges;
    Mat grayImg;
    Mat blackImg;

    namedWindow("edges",1);
    namedWindow("Gray",2);
    namedWindow("Adaptive BW", 3);


    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);
        cvtColor(frame, grayImg, CV_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);

        // Adaptive Threshold Black and White
        adaptiveThreshold(grayImg,blackImg,255, CV_ADAPTIVE_THRESH_GAUSSIAN_C,
        		CV_THRESH_BINARY, 11, 0);

        imshow("edges", edges);
        imshow("Gray", grayImg);
        imshow("Adaptive BW", blackImg);

        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
*/
