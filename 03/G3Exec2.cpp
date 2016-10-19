

#include "opencv2/opencv.hpp"

using namespace cv;
/*
int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat grayImg;
    Mat blackImg;
    Mat thresholdImg;
    Mat otsuImg;

    // Threshold Local
    namedWindow("Adaptive BW", 1);
    // Threshold Global
    namedWindow("Threshold BW", 1);
    namedWindow("Threshold Otsu", 2);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, grayImg, CV_BGR2GRAY);


        // Adaptive Threshold Black and White
        adaptiveThreshold(grayImg,blackImg,255, CV_ADAPTIVE_THRESH_GAUSSIAN_C,
        		CV_THRESH_BINARY, 11, 0);

        // Threshold
        threshold(grayImg, otsuImg, THRESH_OTSU, 255, THRESH_BINARY);
        threshold(grayImg, thresholdImg, 127, 255, THRESH_BINARY);

        imshow("Adaptive BW", blackImg);
        imshow("Threshold BW", thresholdImg);
        imshow("Threshold Otsu", otsuImg);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

*/
