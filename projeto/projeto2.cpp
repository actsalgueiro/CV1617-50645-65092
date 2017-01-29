#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iterator>
#include <math.h>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

/// MODIFY THIS VALUES TO FIT THE GRID

// IHC Tabela 1

int POS_ANS = 4;
int ROW = 12;
int COL = 15;

// IHC Tabela 2
/*
int POS_ANS = 2;
int ROW = 9;
int COL = 6;
*/
// P3
/*
int POS_ANS = 2;
int ROW = 21;
int COL = 3;
*/
// Correct answers array
vector<int> correctAnswers;
//int correctAnswers[] = {2,3,1,4,4,3,2,1,1,2,3,4,4,3,2,1,1,2,3,4,4,3,2,1,1,2,3,4,4,3,2,1};


int thresh = 10;
Scalar red = Scalar(0,0,255);
Scalar green = Scalar(0,255,0);
int rectW;

Mat grid;
Mat canny_output;
Mat element;

// Print file format of the solution file
void printSolutionFileFormat(){
	cout << "Accepted solution file formats are: \n";
	cout << "<number> <answer>\n";
	cout << "or\n";
	cout << "<answer>\n\n";
}

// Reads the file with the solution
void readSolution(string fileName){
	string line;
	ifstream myfile(fileName.c_str());
	if (myfile.is_open()){
		while(getline (myfile,line)){
			string s;
			// Check if it is in the format <number> <answer>
			if(line.length() > 2){
				s = line.substr(line.find(" ") + 1);
			}else{	// Format <answer>
				s = line;
			}	
			const char *cstr = s.c_str();
			char c = cstr[0];
			// Convert to lower case
			if(isupper(c))
				c = tolower(c);
			
			// Check if it is true or false answers
			if(POS_ANS == 2 && (c == 'f' || c == 'v' || c == 't')){
				if(c == 'f')
					correctAnswers.push_back(2);
				else
					correctAnswers.push_back(1);
					
			}else{
				correctAnswers.push_back((c - 'a') + 1);
			}
		}
		myfile.close();
	}else{
		cout << "Unable to open file";
	}
}

// Get the information about the table 
void getInitializationInfo(){
	cout << "Insert the number of columns of the table: ";
	cin >> COL;
	cout << "Insert the number of rows of the table: ";
	cin >> ROW;
	cout << "Insert the number of possible answers for each question: ";
	cin >> POS_ANS;
	printSolutionFileFormat();
	string filename;
	cout << "Insert the name of the file with the solution: ";
	cin >> filename;
	readSolution(filename);
}


// Draws a rectangle around the entire table for the case where the table is missing a corner
Mat drawMissingCorners(Mat m){

    Mat horizontal;
    adaptiveThreshold(m, horizontal, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 5, 2);
    Mat vertical = horizontal.clone();

    
    // Horizontal axis
    int horizontalsize = horizontal.cols / 30;
    Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize,1));
    
    // Apply morphology operations
    erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    
    // Vertical axis
    int verticalsize = vertical.rows / 30;
    Mat verticalStructure = getStructuringElement(MORPH_RECT, Size( 1,verticalsize));
    
    // Apply morphology operations
    erode(vertical, vertical, verticalStructure, Point(-1, -1));
    dilate(vertical, vertical, verticalStructure, Point(-1, -1));
    
    /*
     * Retirado de http://answers.opencv.org/question/63847/how-to-extract-tables-from-an-image/
     * */
    // create a mask which includes the tables
    Mat mask = horizontal + vertical;
    
    Mat joints;
    bitwise_and(horizontal, vertical, joints);
    
    vector<Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Mat> rois;

	// Draw the rectangle around the entire table
    for (size_t i = 0; i < contours.size(); i++)
    {

        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );

        rectangle( m, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 0), 3, 8, 0 );
    }
    
    return m;
}

Mat preprocess(Mat m){
	
	m = drawMissingCorners(m);
	
	GaussianBlur(m, m, Size(11,11), 0);
    adaptiveThreshold(m, m, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 5, 2);

    int dilation_size = 4;

    element = getStructuringElement( MORPH_CROSS,
                                     Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                     Point( dilation_size, dilation_size ) );

    dilate(m, m, element);

    return m;
}

bool contour_topbot ( const vector<Point>& a, const vector<Point> & b ) {
	Rect ra(boundingRect(a));
    Rect rb(boundingRect(b));
    // scale factor for y should be larger than img.width
    return ( (ra.x + ra.y*rectW) < (rb.x + rb.y*rectW) );
}

bool contour_leftright ( const vector<Point>& a, const vector<Point> & b ) {
	Rect ra(boundingRect(a));
    Rect rb(boundingRect(b));
    return ( (ra.x) < (rb.x) );
}

bool compareContourAreas ( vector<Point> contour1, vector<Point> contour2 ) {
    double i = fabs( contourArea(Mat(contour1)) );
    double j = fabs( contourArea(Mat(contour2)) );
    return ( i < j );
}


int main(int, char** argv)
{
	
	getInitializationInfo();
	
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
    
    grid = gray.clone();
    int gridArr[ROW][COL] = {0};

    /// Pre-process the grid
    grid = preprocess(grid);

    int max=-1; 
    Point maxPt; 
    /// Find biggest BLOB (outside grid)
    for(int y=0;y<grid.size().height;y++) 
    { 
    	uchar *row = grid.ptr(y); 
    	for(int x=0;x<grid.size().width;x++) 
    	{ 
    		if(row[x]>=128) { 
    			int area = floodFill(grid, Point(x,y), CV_RGB(0,0,64)); 
    			if(area>max) { 
    				maxPt = Point(x,y); 
    				max = area; 
    			} 
    		} 
    	} 
    }
    /// Fill the biggest BLOB with white
    floodFill(grid, maxPt, CV_RGB(255,255,255));
    /// Hide all other BLOBS by filling them with black
    for(int y=0;y<grid.size().height;y++) 
    { 
    	uchar *row = grid.ptr(y); 
    	for(int x=0;x<grid.size().width;x++) 
    	{ 
    		if(row[x]==64 && x!=maxPt.x && y!=maxPt.y) { 
    			int area = floodFill(grid, Point(x,y), CV_RGB(0,0,0)); 
    		} 
    	} 
    }

    //erode(grid, grid, element);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<vector<Point> > contours_poly(1);

    /// get the grid contour
    findContours( grid, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    double epsilon = 0.02*arcLength(contours[0], true);
    /// find a polygon that fits the grid
    approxPolyDP( Mat(contours[0]), contours_poly[0], epsilon, true );
    /// get the rectangle align the grid
    Rect boundrect;
    boundrect = boundingRect(contours[0]);
    rectW = boundrect.width;

	
    /// prepare for the perspective transform
    vector<Point2f> quad_pts;
    vector<Point2f> squre_pts;
    quad_pts.push_back(Point2f(contours_poly[0][0].x,contours_poly[0][0].y));
    quad_pts.push_back(Point2f(contours_poly[0][1].x,contours_poly[0][1].y));
    quad_pts.push_back(Point2f(contours_poly[0][3].x,contours_poly[0][3].y));
    quad_pts.push_back(Point2f(contours_poly[0][2].x,contours_poly[0][2].y));
    squre_pts.push_back(Point2f(boundrect.x,boundrect.y));
    squre_pts.push_back(Point2f(boundrect.x,boundrect.y+boundrect.height));
    squre_pts.push_back(Point2f(boundrect.x+boundrect.width,boundrect.y));
    squre_pts.push_back(Point2f(boundrect.x+boundrect.width,boundrect.y+boundrect.height));

    Mat transmtx = getPerspectiveTransform(quad_pts,squre_pts);

    Mat roi = gray.clone();
    
    roi = preprocess(roi);

    warpPerspective(roi, roi, transmtx, grid.size());

    roi = roi(boundrect);

    Mat transformed = Mat::zeros(src.rows, src.cols, CV_8UC3);
    warpPerspective(src, transformed, transmtx, src.size());
    transformed = transformed(boundrect);

    //erode(roi,roi,element);


    /*
	 * EXTRACTING THE LINES
	 */
    // Create the images that will use to extract the horizontal and vertical lines
    Mat horizontal = roi.clone();
    Mat vertical = roi.clone();

    int scale = 10;

    // Specify size on horizontal axis
    int horizontalsize = horizontal.cols / scale;

    // Create structure element for extracting horizontal lines through morphology operations
    Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize,1));

    // Apply morphology operations
    erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));

    // Specify size on vertical axis
    int verticalsize = vertical.rows / scale;

    // Create structure element for extracting vertical lines through morphology operations
    Mat verticalStructure = getStructuringElement(MORPH_RECT, Size( 1,verticalsize));

    // Apply morphology operations
    erode(vertical, vertical, verticalStructure, Point(-1, -1));
    dilate(vertical, vertical, verticalStructure, Point(-1, -1));
	dilate(vertical, vertical, verticalStructure, Point(-1, -1)); // expand vertical lines

	// Show extracted vertical lines
    namedWindow("vertical", CV_WINDOW_NORMAL);
    imshow("vertical", vertical);
    // Show extracted horizontal lines
    namedWindow("horizontal", CV_WINDOW_NORMAL);
    imshow("horizontal", horizontal);
	
    Mat mask = horizontal + vertical;
	/*namedWindow("mask", CV_WINDOW_NORMAL);
    imshow("mask", mask);*/

    /// Find the intersections
    Mat joints;
    bitwise_and(horizontal, vertical, joints);

    vector<vector<Point> > contoursC;
    vector<Vec4i> hierarchyC;

    // dilate the intersections for finding the contours easily
    /*int dilation_size = 1;
    element = getStructuringElement( MORPH_CROSS,
                                     Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                     Point( dilation_size, dilation_size ) );
    dilate(joints, joints, element);

    printf("TESTE\n");*/

    findContours( joints, contoursC, hierarchyC, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    printf("num contours: %d\n", contoursC.size());

    if(contoursC.size() > ((COL+1)*(ROW+1)) )
    {
    	sort(contoursC.begin(), contoursC.end(), compareContourAreas );
    	contoursC.erase(contoursC.begin(), contoursC.begin() + (contoursC.size() - (COL+1)*(ROW+1)) );
    }

    printf("num contours: %d\n", contoursC.size());

    // sort the contours(intersections) from top to bottom, then from left to right
	sort(contoursC.begin(), contoursC.end(), contour_topbot);
	for(int i = 0; i < (ROW+1)*(COL+1); i += COL+1)
		sort(contoursC.begin()+i, contoursC.begin()+i+COL+1, contour_leftright);

	// drawing the corners
  	for( int i = 0; i< contoursC.size(); i++ )
    {
       	drawContours( transformed, contoursC, i, red, 1, 8, hierarchyC, 0, Point() );
		/*namedWindow("transformed", CV_WINDOW_NORMAL);
		imshow("transformed", transformed);
		waitKey(9999);*/
    }

    Mat detectROI = transformed.clone();
    cvtColor(detectROI, detectROI, CV_BGR2GRAY);
    //detectROI = preprocess(detectROI);
    GaussianBlur(detectROI, detectROI, Size(11,11), 0);
    adaptiveThreshold(detectROI, detectROI, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 7, 2);
    int dilation_size = 4;
    element = getStructuringElement( MORPH_CROSS,
                                     Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                     Point( dilation_size, dilation_size ) );
    dilate(detectROI,detectROI, element);
    /// create a rectangle for each cell
    vector<Rect> cells;
    for(int i = 0; i < ROW; i++ )
    {
    	for(int j = 0; j < COL; j++ )
    	{
    		/// x = current corner x | y = current corner x | width = absdist(curr corner - corner right | height = absdist = curr corner - corner down)
    		int rectH = norm(contoursC[i*(COL+1)+j][0].y-contoursC[(i+1)*(COL+1)+j][0].y);
    		int rectW = norm(contoursC[i*(COL+1)+j][0].x-contoursC[i*(COL+1)+j+1][0].x);
    		int rectX = contoursC[i*(COL+1)+j][0].x;
    		int rectY = contoursC[i*(COL+1)+j][0].y;
    		Rect c = Rect(rectX, rectY, rectW, rectH);

    		/// Scaling the rectangle so it minimizes the lines detection
    		int rwidth  = c.width;
    		int rheight = c.height;

    		c.width = round((c.width * 90) / 100.0f);
		    c.height = round((c.height * 90) / 100.0f);        
		    c.x += (rwidth - c.width) / 2;
		    c.y += (rheight - c.height) / 2;

    		Mat m1 = detectROI(c);
    		/// sum the pixels of the rect and divide by area to get ratio
    		double s1 = sum(m1)[0]/c.area();
    		/// update the cell matrix with cell value

    		int select = 0;
    		
    		// ignore first row (a,b,c,d) and the cells with the number of the question 
    		if((i == 0) || (j%(POS_ANS+1) == 0) )
    			select = 99;
    		else{
    			// threshold for a cross
    			if(s1>57 & s1<132)
    				select = 1;
	    		else{
	    			// threshold for a filled square
	    			if(s1>=132)
	    				select = 77;
	    		}
    		}

    		gridArr[i][j] = select;
    		/// save the rectangle that represents the cell
    		cells.push_back(c);
    	}
    }

    /// print all cells borders
    for(int i = 0; i < ROW*COL; i++)
    	rectangle(transformed, cells[i], red, 2, 8, 0);


    namedWindow("joints", CV_WINDOW_NORMAL);
    imshow("joints", joints);
    


    //Mat transformedGrid = Mat::zeros(grid.rows, grid.cols, CV_8UC3);
    
    //drawContours( src, contours, 0 , green, 8, 8, hierarchy, 0, Point() );
    //rectangle(src,boundrect,red,8,8,0);
    //rectangle(transformed,boundrect,red,8,8,0);

    /*vector<vector<Point> > roiContours;
    vector<Vec4i> roiHierarchy;
    Mat roi_canny_output;

    Canny( roi, roi_canny_output, 100, 200, 3 );		
	findContours( roi_canny_output, roiContours, roiHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    //Mat drawing = Mat::zeros( grid.size(), CV_8UC3 );
  	for( int i = 0; i< roiContours.size(); i++ )
    {
       drawContours( transformed, roiContours, i, red, 6, 8, roiHierarchy, 0, Point() );
    }*/

    /*vector<Vec4i> lines;
    Mat linesMat = roi.clone();
    HoughLinesP( linesMat, lines, 1, CV_PI/180, 80, 100, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        line( roi, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), red, 8, 8 );
    }*/

    namedWindow("1", CV_WINDOW_NORMAL);
    imshow("1", grid);

    namedWindow("2", CV_WINDOW_NORMAL);
    imshow("2", src);

    namedWindow("transformed", CV_WINDOW_NORMAL);
    imshow("transformed", transformed);

    namedWindow("ROI", CV_WINDOW_NORMAL);
    imshow("ROI", detectROI);
    
   
    /// grid matrix with filled ratio
    for(int j = 0; j < COL; ++j)
    	printf("|-%s-", "---");
   	printf("|\n");
    for(int i = 0; i < ROW; ++i)
    {
    	for(int j = 0; j < COL; ++j)
    		printf("| %3d ", gridArr[i][j]);
    	printf("|\n");
    }
    for(int j = 0; j < COL; ++j)
    	printf("|-%s-", "---");
   	printf("|\n");
   	
   	
   	
   	
   	// Exam sheet correction
   	/*
     * Correct = 1
     * Not answered = 0
     * Incorrect = -1
     * */
    
    // Get student answers
    int ansNRows = ROW - 1;
    int ansNCols = COL/POS_ANS;
    int answers[ansNRows][ansNCols] = {0};
    int ansrow = 0;
    
    // One of the elements is not being initialized with zero
	for(int l = 0; l < ansNCols; l++){
		for(int k = 0; k < ansNRows; k++){
			answers[k][l] = 0;
		}
	}
	for(int i = 1; i < ROW; ++i){
		int anscol = 0;
		for(int j = 1; j < COL; ++j){
			// For multiple questions in the same row
			if(gridArr[i][j] == 99){
				anscol++;
				continue;;
			}
			
			// Look for an answer
			if(gridArr[i][j] == 1){
				// Check for multiple answers to the same question
				if(answers[ansrow][anscol] != 0){
					answers[ansrow][anscol] = -1;
				}else{
					// Answer equals the position of the correct answer
					if((j % (POS_ANS + 1)) == correctAnswers[(anscol * ansNRows) + ansrow]){
						answers[ansrow][anscol] = 1;
					}else{
						answers[ansrow][anscol] = -1;
					}
				}
			}
			
		}
		// Increase the index for the row of the answers array
		ansrow++;
	}
	
	
   	vector<int> correction;
	for(int l = 0; l < ansNCols; l++){
		for(int k = 0; k < ansNRows; k++){
			correction.push_back(answers[k][l]);
		}
	}
	
	// Write correction to file
	ofstream myfile;
	myfile.open ("correction.txt");
	for(int i = 0; i < correction.size(); i++){
		myfile << (i + 1) << " - " << correction.at(i) << "\n";
	}
	myfile.close();
   	
   	// Print the results
	for(int i = 0; i < correction.size(); i++){
		if(correction.at(i) == 1){
			printf("%3d - Correct\n", (i+1));
		}else if(correction.at(i) == 0){
			printf("%3d - Not answered\n", (i+1));
		}else{
			printf("%3d - Incorrect\n", (i+1));
		}
	}


    waitKey(0);
    return 0;
}
