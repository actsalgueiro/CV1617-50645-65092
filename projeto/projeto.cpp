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
    //imshow("gray", gray);
    
    // Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
    Mat bw;
    adaptiveThreshold(~gray, bw, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
    
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
        
        drawContours( src, contours, i, Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point() );
        rectangle( src, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 1, 8, 0 );
    }
    
    
    imshow("src", src);
    
    
    for(size_t i = 0; i < rois.size(); ++i)
    {
		imshow("roi", rois[i]);
		Mat table = rois[i];
	
		
		// Transform source image to gray if it is not
		cvtColor(table, table, CV_BGR2GRAY);
		
		// Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
		Mat bwtable;
		adaptiveThreshold(~table, bwtable, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
		//Mat wbtable;
		//adaptiveThreshold(~table, wbtable, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 15, -2);
		
		// Create the images that will use to extract the horizontal and vertical lines
		Mat horizontaltable = bwtable.clone();
		Mat verticaltable = bwtable.clone();
		
		// Specify size on horizontal axis
		int horizontalsizetable = horizontaltable.cols / 30;
		
		// Create structure element for extracting horizontal lines through morphology operations
		Mat horizontalStructuretable = getStructuringElement(MORPH_RECT, Size(horizontalsizetable,1));
		// Apply morphology operations
		erode(horizontaltable, horizontaltable, horizontalStructuretable, Point(-1, -1));
		dilate(horizontaltable, horizontaltable, horizontalStructuretable, Point(-1, -1));
		
		// Specify size on vertical axis
		int verticalsizetable = verticaltable.rows / 30;
		// Create structure element for extracting vertical lines through morphology operations
		Mat verticalStructuretable = getStructuringElement(MORPH_RECT, Size( 1,verticalsizetable));
		// Apply morphology operations
		erode(verticaltable, verticaltable, verticalStructuretable, Point(-1, -1));
		dilate(verticaltable, verticaltable, verticalStructuretable, Point(-1, -1));
		
		// Create the mask for the table
		Mat mask2 = horizontaltable + verticaltable;
		
		// Remove the table lines from the answer sheet
		Mat answers = bwtable - mask2;
		
		medianBlur(answers, answers, 3);
		imshow("answersbw", answers);
		
		
		
		// Get each rectangle from the table
		vector<vector<Point> > contourstable;
		vector<Vec4i> hierarchytable;

		Canny( table, table, 100, 200, 3 );
		
		findContours( table, contourstable, hierarchytable, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		Mat drawing = Mat::zeros( table.size(), CV_8UC3 );
		
		vector<bool> sheet;
		
		// Check each rectangle
		for( int i = 0; i< contourstable.size(); i++ )
		{
			
			cv::Rect brect = cv::boundingRect(contourstable[i]);
			// O problema de P3 pode estar aqui na detecao de retangulos com areas com um tamanho maior que 1000 mas que nao sao celulas
			if (brect.area() < 1000)
				continue;
				
			rectangle(drawing, brect, CV_RGB(0,0,255), 3);
			
			// Setup the coordinates of the rectangle
			int y = brect.y;
			int xmax = brect.x + brect.width;
			int ymax = y + brect.height;
		
			bool foundWhitePixel = false;
			// Check each pixel
			for(; y < ymax; y++){
				for(int x = brect.x; x < xmax; x++){
					
					if(answers.at<uchar>(y, x) == 255){
						sheet.push_back(true);
						foundWhitePixel = true;
						break;
					}
				}
				if(foundWhitePixel)
					break;
			}
			// If no white pixel has been found in this rectangle then add it has false
			if(!foundWhitePixel)
				sheet.push_back(false);
		}
		
		
		//int nlinhas;
		int ncolunas;
		/*cout << "Indique o numero de linhas: ";
		cin >> nlinhas;*/
		
		cout << endl << "Indique o numero de colunas: ";
		cin >> ncolunas;
	
		int k = 0;
		// Pop the last element that does not exist on the table but was added because the cycle runs one time more than the table size
		sheet.pop_back();
		while(sheet.size() > 0){
			if(k%ncolunas==0)
				cout << endl;
			cout << sheet.back() << " ";
			sheet.pop_back();
			k++;
			
		}
		
		cout << endl;

		waitKey(0);
		imshow( "Contours", drawing);
    }

    waitKey(0);
    return 0;
}
