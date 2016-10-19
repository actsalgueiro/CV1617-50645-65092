/*
 * LoadSaveImage.cpp
 *
 *  Created on: 24/09/2016
 *      Author: filipe
 */

#include <cv.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace cv;
/*
// From http://docs.opencv.org/2.4/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html
Mat& ScanImageIterator(Mat& I)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);

    const int channels = I.channels();
    switch(channels)
    {
    case 1:
        {
            MatIterator_<uchar> it, end;
            //for( it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
                //*it = table[*it];
            break;
        }
    case 3:
        {
            MatIterator_<Vec3b> it, end;
            for( it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
            {
            	/* Image copy
                (*it)[0] = table[(*it)[0]];
                (*it)[1] = table[(*it)[1]];
                (*it)[2] = table[(*it)[2]];*/
            /*}
        }
    }

    return I;
}




int main( int argc, char** argv ){
 char* imageName = argv[1];

 Mat image;
 image = imread( imageName, 1 );

 if( argc != 2 || !image.data )
 {
   printf( " No image data \n " );
   return -1;
 }

 Mat gray_image = ScanImageIterator(image);
 cvtColor( gray_image, gray_image, CV_BGR2GRAY );

 imwrite( "../../images/Gray_Image.jpg", gray_image );

 namedWindow( imageName, CV_WINDOW_AUTOSIZE );
 namedWindow( "Gray image", CV_WINDOW_AUTOSIZE );

 imshow( imageName, image );
 imshow( "Gray image", gray_image );

 waitKey(0);

 return 0;
}
*/
