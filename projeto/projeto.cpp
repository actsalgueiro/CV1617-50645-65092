#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
/*
 * Retirado de http://docs.opencv.org/master/d1/dee/tutorial_moprh_lines_detection.html
 * */
int main(int, char** argv)
{
    // Load the image
    Mat src = imread(argv[1]);
    
    // Check if image is loaded fine
    if(!src.data)
        cerr << "Problem loading image!!!" << endl;
    
    // Transform source image to gray if it is not
    Mat gray;
    if (src.channels() == 3)
    {
        cvtColor(src, gray, CV_BGR2GRAY);
    }
    else
    {
        gray = src;
    }
    
    // Show gray image
    imshow("gray", gray);
    
    // Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
    Mat bw;
    adaptiveThreshold(~gray, bw, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
    
    // Show binary image
    //imshow("binary", bw);
    
    // Create the images that will use to extract the horizontal and vertical lines
    Mat horizontal = bw.clone();
    Mat vertical = bw.clone();
    
    // Specify size on horizontal axis
    int horizontalsize = horizontal.cols / 30;
    // Create structure element for extracting horizontal lines through morphology operations
    Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize,1));
    // Apply morphology operations
    erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    // Show extracted horizontal lines
    //imshow("horizontal", horizontal);
    
    // Specify size on vertical axis
    int verticalsize = vertical.rows / 30;
    // Create structure element for extracting vertical lines through morphology operations
    Mat verticalStructure = getStructuringElement(MORPH_RECT, Size( 1,verticalsize));
    // Apply morphology operations
    erode(vertical, vertical, verticalStructure, Point(-1, -1));
    dilate(vertical, vertical, verticalStructure, Point(-1, -1));
    // Show extracted vertical lines
    //imshow("vertical", vertical);
    
    /*
     * Retirado de http://answers.opencv.org/question/63847/how-to-extract-tables-from-an-image/
     * */
    // create a mask which includes the tables
    Mat mask = horizontal + vertical;
    imshow("mask", mask);
    
    // find the joints between the lines of the tables, we will use this information in order to descriminate tables from pictures (tables will contain more than 4 joints while a picture only 4 (i.e. at the corners))
    Mat joints;
    bitwise_and(horizontal, vertical, joints);
    //imshow("joints", joints);
    
    
    // Find external contours from the mask, which most probably will belong to tables or to images
    // Extract the tables
    vector<Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Mat> rois;

    for (size_t i = 0; i < contours.size(); i++)
    {
        // find the area of each contour
        double area = contourArea(contours[i]);

       // filter individual lines of blobs that might exist and they do not represent a table
        //if(area < 100) // value is randomly chosen, you will need to find that by yourself with trial and error procedure
        //    continue;

        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );

        // find the number of joints that each table has
        Mat roi = joints(boundRect[i]);

        vector<vector<Point> > joints_contours;
        findContours(roi, joints_contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

        // if the number is not more than 5 then most likely it not a table
        if(joints_contours.size() <= 4)
            continue;

        rois.push_back(src(boundRect[i]).clone());
	// CV_FILLED em vez de 2
        drawContours( src, contours, i, Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point() );
        rectangle( src, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 1, 8, 0 );
    }
    
    
    imshow("src", src);
    
    
    for(size_t i = 0; i < rois.size(); ++i)
    {
		imshow("roi", rois[i]);
		   
		/* Ainda nao esta funcional esta parte     
        // Extract the tables
		vector<Vec4i> hierarchy2;
		std::vector<std::vector<cv::Point> > contours2;
		cv::findContours(mask, contours2, hierarchy2, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		
		vector<vector<Point> > contours_poly( contours.size() );
		vector<Rect> boundRect( contours.size() );
		vector<Mat> rois2;

		for (size_t i = 0; i < contours.size(); i++)
		{
			Mat image = rois[i];
			// find the area of each contour
			double area = contourArea(contours[i]);

		   // filter individual lines of blobs that might exist and they do not represent a table
			//if(area < 100) // value is randomly chosen, you will need to find that by yourself with trial and error procedure
			//    continue;

			approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
			boundRect[i] = boundingRect( Mat(contours_poly[i]) );

			// find the number of joints that each table has
			Mat roi = joints(boundRect[i]);

			vector<vector<Point> > joints_contours;
			findContours(roi, joints_contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

			// if the number is not more than 5 then most likely it not a table
			if(joints_contours.size() <= 3)
				continue;

			rois2.push_back(image(boundRect[i]).clone());
	
			drawContours( src, contours, i, Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point() );
			rectangle( src, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 1, 8, 0 );
		}
		
		for(size_t i = 0; i < rois2.size(); ++i)
		{
			imshow("roi2", rois2[i]); 
			 waitKey();
		}
		     */

        waitKey();
    }
    
    waitKey(0);
    return 0;
}
