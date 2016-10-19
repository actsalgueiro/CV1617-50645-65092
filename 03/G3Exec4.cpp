

#include "opencv2/opencv.hpp"

using namespace cv;
/*
int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat normalBlur;
    Mat medianBlure;
    Mat gaussianBlure;
    namedWindow("original",2);
    namedWindow("blur",1);
    namedWindow("median blur", 1);
    namedWindow("gaussian blur", 1);


    for(;;)
    {
        Mat frame;

        cap >> frame; // get a new frame from camera

        Size tamanho = Size(11,11);

        // normal blur
        blur(frame, normalBlur, tamanho, Point(-1,-1), BORDER_DEFAULT);

        medianBlur(frame, medianBlure, 11);

        GaussianBlur(frame, gaussianBlure, tamanho, 0, 0, BORDER_DEFAULT);

        imshow("original", frame);
        imshow("blur", normalBlur);
        imshow("median blur", medianBlure);
        imshow("gaussian blur", gaussianBlure);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}*/
