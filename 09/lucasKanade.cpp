#include "opencv2/opencv.hpp"

#include <iostream>
#include <vector>

#include <stdio.h>

// OpenCV Includes
#include <opencv2/core/core.hpp>        
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;


int main(int, char**)
{
    // open the default camera
    VideoCapture cap(0);
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    // Obtain first image and set up two feature vectors
    cv::Mat image_prev, image_next;
    std::vector<cv::Point2f> features_prev, features_next;
    vector<cv::Point2f> points_prev, points_next;
    int max_count = 100;
    double qlevel = 0.3;
    double minDist = 7;

    cap >> image_next;

    cv::goodFeaturesToTrack(image_next, // the image 
      features_next,   // the output detected features
      max_count,  // the maximum number of features 
      qlevel,     // quality level
      minDist     // min distance between two features
    );

    points_prev = features_next;

    // Tracker is initialised and initial features are stored in features_next
    // Now iterate through rest of images
    for(;;)
    {
        Mat frame;
        Mat status;
        Mat err;


        image_prev = image_next.clone();

        features_prev = features_next;
        
        cap >> image_next;  // Get next image

        // Obtain initial set of features


        // Find position of feature in new image
        cv::calcOpticalFlowPyrLK(
          image_prev, image_next, // 2 consecutive images
          points_prev, // input point positions in first im
          points_next, // output point positions in the 2nd
          status,    // tracking success
          err      // tracking error
        );
    
        imshow("normal", frame);
            
        if(waitKey(30) >= 0) break;
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}