

#include "opencv2/opencv.hpp"

using namespace cv;
/*
int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    // Based on http://www.pyimagesearch.com/2014/08/18/skin-detection-step-step-example-using-python-opencv/
    Mat skinMask;
    Mat converted;
    namedWindow("original",1);
    namedWindow("skin",1);



    // Valores para obter a mascara da cor de pele
    Vec3b lowerV = Vec3b(0,48,80);
    Vec3b upperV = Vec3b(20,255,255);

    for(;;)
    {
        Mat frame;
        Mat skinImg;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, converted, CV_BGR2HSV);
        inRange(converted,lowerV, upperV, skinMask);

        bitwise_and(frame, frame, skinImg, skinMask);
        imshow("original", frame);
        imshow("skin", skinImg);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}*/
