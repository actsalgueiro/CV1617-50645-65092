#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <getopt.h>

using namespace cv;
using namespace std;

void print_usage()
{
	printf("Usage: DisplayAndSaveImage [-v] -i inputFile -o outputFile\n");
}

int main(int argc, char** argv )
{
	int option = 0;
	string inputFileName, outputFileName;
	while ((option = getopt(argc, argv,"vi:o:")) != -1) {
		switch (option)
		{
		case 'i':
			inputFileName = optarg;
			break;
		case 'o':
			outputFileName = optarg;
			break;
		case 'v':
			print_usage();
			exit(EXIT_FAILURE);
			break;
		default:
			print_usage();
			exit(EXIT_FAILURE);
		}
	}

	if (inputFileName.length() == 0 || outputFileName.length() == 0)
	{
		print_usage();
		exit(EXIT_FAILURE);
	}

	Mat image;
	image = imread(inputFileName, 1 );

	if ( !image.data )
	{
		printf("No image data \n");
		return -1;
	}
	namedWindow("Display Image", WINDOW_AUTOSIZE );
	imshow("Display Image", image);

	imwrite(outputFileName, image);
	waitKey(0);
	return 0;
}
