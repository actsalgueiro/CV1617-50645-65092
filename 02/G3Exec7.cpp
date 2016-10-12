#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

/*
 *
 * Falta a parte de melhorar para procurar qual as duas imagens mais semelhantes
 *
 */

/*
int main( int argc, char** argv ){
	Mat image1;
	Mat image2;
	image1 = imread( argv[1], 1 );
	image2 = imread(argv[2], 1);

	if( argc != 3 || !image1.data || !image2.data )
	{
	printf( " No image data \n " );
	printf(" Input two image file names");
	return -1;
	}

    Mat grayImg;

    namedWindow("Imagem 1",1);
    namedWindow("Imagem 2",2);



	// Adapted from http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
	// Separate the image in 3 places ( B, G and R )
	vector<Mat> bgr_planes1;
	split(image1, bgr_planes1 );
	vector<Mat> bgr_planes2;
	split(image2, bgr_planes2 );

	// Establish the number of bins
	int histSize = 256;

	// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 } ;
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist1, g_hist1, r_hist1;
	Mat b_hist2, g_hist2, r_hist2;

	// Compute the histograms:
	calcHist( &bgr_planes1[0], 1, 0, Mat(), b_hist1, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes1[1], 1, 0, Mat(), g_hist1, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes1[2], 1, 0, Mat(), r_hist1, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes2[0], 1, 0, Mat(), b_hist2, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes2[1], 1, 0, Mat(), g_hist2, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes2[2], 1, 0, Mat(), r_hist2, 1, &histSize, &histRange, uniform, accumulate );

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage1( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
	Mat histImage2( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist1, b_hist1, 0, histImage1.rows, NORM_MINMAX, -1, Mat() );
	normalize(g_hist1, g_hist1, 0, histImage1.rows, NORM_MINMAX, -1, Mat() );
	normalize(r_hist1, r_hist1, 0, histImage1.rows, NORM_MINMAX, -1, Mat() );
	normalize(b_hist2, b_hist2, 0, histImage2.rows, NORM_MINMAX, -1, Mat() );
	normalize(g_hist2, g_hist2, 0, histImage2.rows, NORM_MINMAX, -1, Mat() );
	normalize(r_hist2, r_hist2, 0, histImage2.rows, NORM_MINMAX, -1, Mat() );
/*
	/// Draw for each channel
	for( int i = 1; i < histSize; i++ )
	{
		line( histImage1, Point( bin_w*(i-1), hist_h - cvRound(b_hist1.at<float>(i-1)) ) ,
						 Point( bin_w*(i), hist_h - cvRound(b_hist1.at<float>(i)) ),
						 Scalar( 255, 0, 0), 2, 8, 0  );
		line( histImage1, Point( bin_w*(i-1), hist_h - cvRound(g_hist1.at<float>(i-1)) ) ,
						 Point( bin_w*(i), hist_h - cvRound(g_hist1.at<float>(i)) ),
						 Scalar( 0, 255, 0), 2, 8, 0  );
		line( histImage1, Point( bin_w*(i-1), hist_h - cvRound(r_hist1.at<float>(i-1)) ) ,
						 Point( bin_w*(i), hist_h - cvRound(r_hist1.at<float>(i)) ),
						 Scalar( 0, 0, 255), 2, 8, 0  );
		line( histImage2, Point( bin_w*(i-1), hist_h - cvRound(b_hist2.at<float>(i-1)) ) ,
						 Point( bin_w*(i), hist_h - cvRound(b_hist2.at<float>(i)) ),
						 Scalar( 255, 0, 0), 2, 8, 0  );
		line( histImage2, Point( bin_w*(i-1), hist_h - cvRound(g_hist2.at<float>(i-1)) ) ,
						 Point( bin_w*(i), hist_h - cvRound(g_hist2.at<float>(i)) ),
						 Scalar( 0, 255, 0), 2, 8, 0  );
		line( histImage2, Point( bin_w*(i-1), hist_h - cvRound(r_hist2.at<float>(i-1)) ) ,
						 Point( bin_w*(i), hist_h - cvRound(r_hist2.at<float>(i)) ),
						 Scalar( 0, 0, 255), 2, 8, 0  );	}*/
/*
	/// Apply the histogram comparison methods
	for( int i = 0; i < 4; i++ )
	{
		int compare_method = i;
		double blue = compareHist( b_hist1, b_hist2, compare_method );
		double green = compareHist( g_hist1, g_hist2, compare_method );
		double red = compareHist( r_hist1, r_hist2, compare_method );

		printf( " Method [%d] Red, Green, Blue: %f, %f, %f \n", i, red, green, blue);
	}

	imshow("Imagem 1", image1);
	imshow("Imagem 2", image2);

	waitKey();
    return 0;
}
*/

