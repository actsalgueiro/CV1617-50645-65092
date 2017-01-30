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

/*int POS_ANS = 4;
int ROW = 12;
int COL = 15;*/

// IHC Tabela 2

int POS_ANS = 2;
int ROW = 9;
int COL = 6;


// P3

/*int POS_ANS = 2;
int ROW = 21;
int COL = 3;*/

// Correct answers array
int nQuestion = 32;
vector<int> correctAnswers (nQuestion, 0);


int thresh = 10;
Scalar red = Scalar(0,0,255);
Scalar green = Scalar(0,255,0);
Scalar yellow = Scalar(0,255,255);
int rectW;
int resolution;
string filename;
string imgPath;

int nmec = 50645;
double cotR = 0.3;
double cotW = 0.1;
double finalG = 0;


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
		int i = 0;
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
					correctAnswers.at(i) = 2;
				else
					correctAnswers.at(i) = 1;
					
			}else{
				correctAnswers.at(i) = (c - 'a') + 1;
			}
			i++;
		}
		myfile.close();
	}else{
		cout << "Unable to open file";
	}
}

/*void readConfig(string fileName)
{
    string line;
    ifstream myfile(fileName.c_str());
    if (myfile.is_open()){
        int i = 0;
        while(getline (myfile,line)){
            COL = atoi(line.substr(line.find("number of columns of the table: ") + 1).c_str());
            ROW = atoi(line.substr(line.find("number of rows of the table: ") + 1).c_str());
            nQuestion = atoi(line.substr(line.find("number of questions: ") + 1).c_str());
            POS_ANS = atoi(line.substr(line.find("number of possible answers for each question: ") + 1).c_str());
            cotR = atof(line.substr(line.find("value of each right answer: ") + 1).c_str());
            cotW = atof(line.substr(line.find("value of each wrong answer: ") + 1).c_str());

            printf("col: %d\n", COL);
        }
        myfile.close();
    }else{
        cout << "Unable to open file";
    }
}*/

// Get the information about the table 
void getInitializationInfo(){
    cout << "Insert the image path: ";
    cin >> imgPath;
    cout << "Insert the student number: ";
    cin >> nmec;
	cout << "Insert the number of columns of the table: ";
	cin >> COL;
	cout << "Insert the number of rows of the table: ";
	cin >> ROW;
    cout << "Insert the number of questions: ";
    cin >> nQuestion;
	cout << "Insert the number of possible answers for each question: ";
	cin >> POS_ANS;
    cout << "Insert the value of each right answer: ";
    cin >> cotR;
    cout << "Insert the value of each wrong answer: ";
    cin >> cotW;
	printSolutionFileFormat();
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

Mat preprocess(Mat m, int threshold){	
	GaussianBlur(m, m, Size(11,11), 0);
    adaptiveThreshold(m, m, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, threshold, 2);

    int dilation_size = 0;

    if(resolution > 2000*4000)
    	dilation_size = 4;
    else if(resolution > 1500*3500)
    	dilation_size = 3;
    else if(resolution > 1000*1000)
    	dilation_size = 2;
    else
    	dilation_size = 1;

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
	//readConfig("../projeto/config.txt");
	getInitializationInfo();
    
	//filename = "../projeto/solution2.txt";
	
	// Read the solution from the file
	//readSolution(filename);

    for (;;)
    {
        // Load the image
        Mat src = imread(imgPath);
        
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

        resolution = src.size().height * src.size().width;

        printf("Resolution W: %d  |  H: %d\n", src.size().width, src.size().height);
        
        grid = gray.clone();
        vector<vector<int> > gridArr( ROW, vector<int>(COL));

        /// Pre-process the grid
        grid = preprocess(grid, 5);

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
        double dist1 = norm(contours_poly[0][0] - contours_poly[0][1]);
        double dist2 = norm(contours_poly[0][0] - contours_poly[0][3]);

        printf("dist1:%f \n", dist1);
        printf("dist2:%f \n", dist2);

        vector<Point2f> quad_pts;
        vector<Point2f> squre_pts;
        
        squre_pts.push_back(Point2f(boundrect.x,boundrect.y));
        squre_pts.push_back(Point2f(boundrect.x,boundrect.y+boundrect.height));
        squre_pts.push_back(Point2f(boundrect.x+boundrect.width,boundrect.y));
        squre_pts.push_back(Point2f(boundrect.x+boundrect.width,boundrect.y+boundrect.height));

        if(ROW < COL)
        {
        	if(dist1 > dist2)
    	    {
    			quad_pts.push_back(Point2f(contours_poly[0][1].x,contours_poly[0][1].y));
    		    quad_pts.push_back(Point2f(contours_poly[0][2].x,contours_poly[0][2].y));
    		    quad_pts.push_back(Point2f(contours_poly[0][0].x,contours_poly[0][0].y));
    		    quad_pts.push_back(Point2f(contours_poly[0][3].x,contours_poly[0][3].y));
    	    }else{
    	    	quad_pts.push_back(Point2f(contours_poly[0][0].x,contours_poly[0][0].y));
    		    quad_pts.push_back(Point2f(contours_poly[0][1].x,contours_poly[0][1].y));
    		    quad_pts.push_back(Point2f(contours_poly[0][3].x,contours_poly[0][3].y));
    		    quad_pts.push_back(Point2f(contours_poly[0][2].x,contours_poly[0][2].y));
    	    }
        }else{
        	if(dist1 < dist2)
    	    {
    			quad_pts.push_back(Point2f(contours_poly[0][1].x,contours_poly[0][1].y));
    		    quad_pts.push_back(Point2f(contours_poly[0][2].x,contours_poly[0][2].y));
    		    quad_pts.push_back(Point2f(contours_poly[0][0].x,contours_poly[0][0].y));
    		    quad_pts.push_back(Point2f(contours_poly[0][3].x,contours_poly[0][3].y));
    	    }else{
    	    	quad_pts.push_back(Point2f(contours_poly[0][0].x,contours_poly[0][0].y));
    		    quad_pts.push_back(Point2f(contours_poly[0][1].x,contours_poly[0][1].y));
    		    quad_pts.push_back(Point2f(contours_poly[0][3].x,contours_poly[0][3].y));
    		    quad_pts.push_back(Point2f(contours_poly[0][2].x,contours_poly[0][2].y));
    	    }
        }

        Mat transmtx = getPerspectiveTransform(quad_pts,squre_pts);

        Mat roi = gray.clone();
        Mat finalRes;
        
        roi = preprocess(roi, 5);

        warpPerspective(roi, roi, transmtx, grid.size());

        roi = roi(boundrect);

        Mat transformed = Mat::zeros(src.rows, src.cols, CV_8UC3);
        warpPerspective(src, transformed, transmtx, src.size());
        warpPerspective(src, finalRes, transmtx, src.size());
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
        /*namedWindow("vertical", CV_WINDOW_NORMAL);
        imshow("vertical", vertical);*/
        // Show extracted horizontal lines
        /*namedWindow("horizontal", CV_WINDOW_NORMAL);
        imshow("horizontal", horizontal);*/
    	
        Mat mask = horizontal + vertical;
    	/*namedWindow("mask", CV_WINDOW_NORMAL);
        imshow("mask", mask);*/

        /// Find the intersections
        Mat joints;
        bitwise_and(horizontal, vertical, joints);

        vector<vector<Point> > contoursC;
        vector<Vec4i> hierarchyC;

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
      	/*for( int i = 0; i< contoursC.size(); i++ )
        {
           	drawContours( transformed, contoursC, i, red, 1, 8, hierarchyC, 0, Point() );
        }*/

        Mat detectROI = transformed.clone();
        cvtColor(detectROI, detectROI, CV_BGR2GRAY);
        detectROI = preprocess(detectROI, 7);
        
        /// create a rectangle for each cell
        vector<Rect> cells;
        vector<Rect> studentAnswer;
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
        		Rect tmp = Rect(rectX, rectY, rectW, rectH);

        		/// Scaling the rectangle so it minimizes the lines detection
        		int rwidth  = c.width;
        		int rheight = c.height;

        		c.width = round((c.width * 80) / 100.0f);
    		    c.height = round((c.height * 80) / 100.0f);        
    		    c.x += (rwidth - c.width) / 2;
    		    c.y += (rheight - c.height) / 2;

    		    c.x += detectROI.size().width*0.003;
    		    c.y += detectROI.size().height*0.003;
    		    tmp.x += detectROI.size().width*0.003;
    		    tmp.y += detectROI.size().height*0.003;

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
        			if(s1>30 & s1<104)
        			{
        				select = 1;
        				studentAnswer.push_back(c);
        			}
    	    		else{
    	    			// threshold for a filled square
    	    			if(s1>=104)
    	    				select = 77;
    	    		}
        		}

        		gridArr[i][j] = select;
        		/// save the rectangle that represents the cell
        		cells.push_back(c);
        	}
        }


        /*namedWindow("joints", CV_WINDOW_NORMAL);
        imshow("joints", joints);*/
       
        // TABLE WITH DETECTED ANSWERS
    	/*printf("\n\n Table: \n");
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
       	printf("|\n\n\n");*/
       	
       	// Exam sheet correction
       	/*
         * Correct = 1
         * Not answered = 0
         * Incorrect = -1
         * */
        
        // Get student answers
        int ansNRows = ROW - 1;
        int ansNCols = COL/POS_ANS;
        vector<vector<int> > answers( ROW, vector<int>(COL));
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
    	
    	// Transform the array into a vector
       	vector<int> correction;
    	for(int l = 0; l < ansNCols; l++){
    		for(int k = 0; k < ansNRows; k++){
    			correction.push_back(answers[k][l]);
    		}
    	}

        int countCol = 1;
        int count = 0;
        // function to draw the correct answers rectangles (yellow)
        for(int i = 1; i < ROW; i++)
        {
            for(int j = 0; j < (POS_ANS+1); j++ )
            {
                if(j%(POS_ANS+1) == 0)
                    continue;
                if(j == correctAnswers[count])
                {
                    rectangle(transformed, cells[i*COL + (countCol-1)*(POS_ANS+1) + j ], yellow, 6, 8, 0);
                    count++;
                    break;
                }

            }

            if((i == (ROW-1)) & (countCol < COL/POS_ANS))
            {
                i = 0;
                countCol++;
            }
        }

        // counters for right and wrong answers
    	int cnt = 0;
        int rightA = 0;
        int wrongA = 0;
    	for(int i = 0; i< correction.size(); i++)
    	{
    		if (correction[i] == 1)
    		{
                // draw the correct answer rectangle
    			rectangle(transformed, studentAnswer[cnt], green, 6, 8, 0);
    			cnt++;
                finalG += cotR;
                rightA++;

    		}
    		if (correction[i] == -1)
    		{
                // draw the wrong answer rectangle
    			rectangle(transformed, studentAnswer[cnt], red, 6, 8, 0);
    			cnt++;
                finalG -= cotW;
                wrongA++;
                }
    	}
    	
    	// Write correction to file
    	/*ofstream myfile;
    	myfile.open ("correction.txt");
    	for(int i = 0; i < correction.size(); i++){
    		myfile << (i + 1) << " - " << correction.at(i) << "\n";
    	}
    	myfile.close();*/
       	
       	// Print the results
       	/*printf(" Correction: \n");
    	for(int i = 0; i < correction.size(); i++){
    		if(correction.at(i) == 1){
    			printf("%3d - Correct\n", (i+1));
    		}else if(correction.at(i) == 0){
    			printf("%3d - Not answered\n", (i+1));
    		}else{
    			printf("%3d - Incorrect\n", (i+1));
    		}
    	}*/

        // add footer to original img cropped by ROI
        Mat showResult = Mat::zeros(transformed.rows + 400, transformed.cols, CV_8UC3);
        showResult = Scalar(255,255,255);

        transformed.copyTo(showResult(Rect(0,0,transformed.cols, transformed.rows)));

        int startY = transformed.size().height;

        char strNMec[32];
        char strRAns[32];
        char strWAns[32];
        char strGrade[32];

        sprintf(strNMec, "Num MEC: %d", nmec);
        sprintf(strRAns, "Respostas certas: %d", rightA);
        sprintf(strWAns, "Respostas erradas: %d", wrongA);
        sprintf(strGrade, "Nota: %3.1f", finalG);

        // Bottom text added to footer
        putText(showResult, strNMec, Point(0 + 60,startY + 80), FONT_HERSHEY_PLAIN, 3.5, Scalar(0,0,0), 1, CV_AA);
        putText(showResult, strRAns, Point(0 + 60,startY + 160), FONT_HERSHEY_PLAIN, 3.5, Scalar(0,0,0), 1, CV_AA);
        putText(showResult, strWAns, Point(0 + 60,startY + 240), FONT_HERSHEY_PLAIN, 3.5, Scalar(0,0,0), 1, CV_AA);
        putText(showResult, strGrade, Point(0 + 60,startY + 320), FONT_HERSHEY_PLAIN, 3.5, Scalar(0,0,0), 1, CV_AA);


        // SHOW WINDOWS
        namedWindow("Grid", CV_WINDOW_NORMAL);
        imshow("Grid", grid);

        namedWindow("Original", CV_WINDOW_NORMAL);
        imshow("Original", src);

        /*namedWindow("transformed", CV_WINDOW_NORMAL);
        imshow("transformed", transformed);*/

        namedWindow("ROI", CV_WINDOW_NORMAL);
        imshow("ROI", detectROI);
        
        namedWindow("Result", CV_WINDOW_NORMAL);
        imshow("Result", showResult);


        // SAVE IMAGE
        char tmpPath[32];
        sprintf(tmpPath, "../Results/%d.jpg", nmec);
        imwrite( tmpPath, showResult );

        printf("Image saved on %s\n", tmpPath);

        waitKey(0);

        char choice;
        cout << "Do you want to correct another using the same Grid? (Y/N) ";
        cin >> choice;

        if(choice == 'Y' | choice == 'y')
        {
            cout << "Insert the student number: ";
            cin >> nmec;
            cout << "Insert the image path: ";
            cin >> imgPath;
        }else{
            printf("Goodbye!\n");
            break;
        }
    }
    return 0;
}
