
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
    int c;

    namedWindow("Scharr",1);


    for(;;)
    {


        Mat frame;
        cap >> frame; // get a new frame from camera
        GaussianBlur(frame, src, Size(3,3), 0, 0, BORDER_DEFAULT );

        /// Convert it to gray
        cvtColor( src, src_gray, CV_BGR2GRAY );

        /// Generate grad_x and grad_y
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;

        /// Gradient X
        Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
        //Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( grad_x, abs_grad_x );

        /// Gradient Y
        Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
        //Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( grad_y, abs_grad_y );

        /// Total Gradient (approximate)
        addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

        imshow("Scharr", grad );

        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
