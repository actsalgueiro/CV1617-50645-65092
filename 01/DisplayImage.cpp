#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

void onMouse( int event, int x, int y, int, void*);

Mat image = imread("../images/me.png", 1);

int maxH=0, minH=255, maxS=0, minS=255, maxV=0, minV=255;

int main(int argc, char** argv )
{
	/*if ( argc != 2 )
	{
		printf("usage: DisplayImage.out <Image_Path>\n");
		return -1;
	}*/

	//Mat image;
	//image = imread( argv[1], 1 );

	if ( !image.data )
	{
		printf("No image data \n");
		return -1;
	}
	cvtColor(image, image, CV_BGR2HSV);
	namedWindow("Display Image", WINDOW_AUTOSIZE );
	setMouseCallback( "Display Image", onMouse, 0 );
	imshow("Display Image", image);

	waitKey(0);

	return 0;
}

void onMouse( int event, int x, int y, int, void*)
{
    if( event != EVENT_LBUTTONDOWN )
            return;

    //Point pt = Point(x,y);
    //std::cout<<"x="<<pt.x<<"\t y="<<pt.y<<"\t value="<<thresh.at<uchar>(x,y)<<"\n";

    Vec3b hsvCol = image.at<Vec3b>(y,x);
//    std::cout << "H: "<<hsvCol.val[2]<<" S: "<<hsvCol.val[1]<<" V: "<<hsvCol.val[0] << std::endl;
    std::cout << "HSV: "<< (int)hsvCol[2] << std::endl;

    if((int)hsvCol[2] < minH)
    {
    	minH = (int)hsvCol[2];
    }else {
    	if ((int)hsvCol[2] > maxH)
    		maxH = (int)hsvCol[2];
    }

    if((int)hsvCol[1] < minS)
    {
    	minS = (int)hsvCol[1];
    }else {
    	if ((int)hsvCol[1] > maxS)
    		maxS = (int)hsvCol[1];
    }

    if((int)hsvCol[0] < minV)
    {
    	minV = (int)hsvCol[0];
    }else {
    	if ((int)hsvCol[0] > maxV)
    		maxV = (int)hsvCol[0];
    }

    std::cout << "MAX" << std::endl;
	std::cout << "H:" << maxH << " S:" << maxS << " v:" << maxV << std::endl; 
	std::cout << "MIN" << std::endl;
	std::cout << "H:" << minH << " S:" << minS << " v:" << minV << std::endl; 
	std::cout << std::endl;

}
