#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges;
    Mat frameBW;
    Mat frameBWglob;
    namedWindow("original", 1);
    namedWindow("gray", 1);
    namedWindow("BW", 1);
    namedWindow("BW global", 1);
    //namedWindow("edges",1);
    //namedWindow("bw",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);
        adaptiveThreshold(edges, frameBW, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 0);
        threshold(edges, frameBWglob, 127, 255, THRESH_BINARY);

        resize(frameBWglob, frameBWglob, Size(400,300));
        resize(edges, edges, Size(400,300));
        resize(frameBW, frameBW, Size(400,300));
        resize(frame, frame, Size(400,300));
        
        imshow("BW global", frameBWglob);
        imshow("Gray", edges);
        imshow("BW", frameBW);
        imshow("original", frame);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
