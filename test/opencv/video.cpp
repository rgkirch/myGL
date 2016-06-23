#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char* argv[])
{
    try
    {
        if(argc > 1)
        {
            VideoCapture cap(argv[1]); // open the default camera
            if(!cap.isOpened())  // check if we succeeded
                return -1;

            Mat edges, blurred;
            namedWindow("edges", 1);
            namedWindow("blurred", 1);
            for(;;)
            {
                Mat frame;
                cap >> frame; // get a new frame from camera
                GaussianBlur(edges, blurred, Size(7,7), 1.5, 1.5);
                cvtColor(frame, edges, CV_BGR2GRAY);
                GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
                Canny(edges, edges, 0, 30, 3);
                imshow("edges", edges);
                imshow("blurred", edges);
                if(waitKey(30) == 27) break;
            }
            // the camera will be deinitialized automatically in VideoCapture destructor
        } else {
            std::cout << "needs an argument" << std::endl;
        }
        return 0;
    }
    catch( cv::Exception& e )
    {
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
    }
}
