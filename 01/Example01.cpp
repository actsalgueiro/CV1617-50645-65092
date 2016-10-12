#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv )
{
	if ( argc != 2 )
	{
		printf("usage: DisplayImage.out <Image_Path>\n");
		return -1;
	}

	Mat image, gray;
	image = imread( argv[1], 1 );

	if ( !image.data )
	{
		printf("No image data \n");
		return -1;
	}

	cvtColor(image, gray, COLOR_RGB2GRAY);

	for(int r = 0 ; r < image.rows ; r++)
		for(int c = 0 ; c < image.cols ; c++)
		{
			Vec3b pix = image.at<Vec3b>(r, c);
			if(pix[2] > 127) pix[2] = 0;
			image.at<Vec3b>(r, c) = pix;
		}
	
	imshow("Display Image", image);
	imshow("Gray Image", gray);

	waitKey(0);

	return 0;
}
