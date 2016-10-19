
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat src, src_gray;
    Mat grad;
    Mat temp;

    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    int kernel_size = 7;

    namedWindow("Laplacian",1);


    for(;;)
    {


        Mat frame;
        cap >> frame; // get a new frame from camera
        GaussianBlur(frame, src, Size(3,3), 0, 0, BORDER_DEFAULT );

        /// Convert it to gray
        cvtColor( src, src_gray, CV_BGR2GRAY );

        /// Generate grad_x and grad_y
        Mat dst;
        Mat abs_dst;

        
        // Gradient
        Laplacian( src_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( dst, abs_dst );
        
        imshow("Laplacian", abs_dst );

        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
