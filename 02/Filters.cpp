#include "opencv2/opencv.hpp"

using namespace cv;


int main(int, char**)
{
     // open the default camera
	VideoCapture cap(0);
    if(!cap.isOpened())  // check if we succeeded
        return -1; 

    namedWindow("original",1);
    namedWindow("blur",1);
    namedWindow("median blur",1);
    namedWindow("gaussian blur",1);
    for(;;)
    {
        Mat frame, blurF, medBlurF, gaussBlurF;
        cap >> frame; // get a new frame from camera

        blur(frame, blurF, Size(9,9));
        medianBlur(frame, medBlurF, 9);
        GaussianBlur(frame, gaussBlurF, Size(9,9), 0);

        resize(frame, frame, Size(400,300));
        resize(blurF, blurF, Size(400,300));
        resize(medBlurF, medBlurF, Size(400,300));
        resize(gaussBlurF, gaussBlurF, Size(400,300));
        imshow("original", frame);
        imshow("blur", blurF);
        imshow("median blur", medBlurF);
        imshow("gaussian blur", gaussBlurF);

        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}