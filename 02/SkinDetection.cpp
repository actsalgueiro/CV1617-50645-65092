#include "opencv2/opencv.hpp"

using namespace cv;


int main(int, char**)
{
    // open the default camera
	VideoCapture cap(0);
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Vec3b lowerV = Vec3b(9,52,142);    
    Vec3b upperV = Vec3b(18,142,255);    


//    Vec3b lowerV = Vec3b(0,47,80);    
//    Vec3b upperV = Vec3b(20,255,255);    

    namedWindow("skin",1);
    namedWindow("normal",1);
    for(;;)
    {
        Mat frame;
        Mat conv;
	    Mat skinMask;
	    Mat skin;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, conv, CV_BGR2HSV);
        inRange(conv, lowerV, upperV, skinMask);

        /*
        Mat kernel;

        kernel = getStructuringElement(MORPH_ELLIPSE, Size(4, 4));
		erode(skinMask, skinMask, kernel, Point(-1,-1), 3);
		dilate(skinMask, skinMask, kernel, Point(-1,-1), 3);

		GaussianBlur(skinMask, skinMask, Size(3, 3), 0);
		*/

        bitwise_and(frame, frame, skin, skinMask);
        //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        //Canny(edges, edges, 0, 30, 3);
        imshow("skin", skin);
        imshow("normal", frame);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}