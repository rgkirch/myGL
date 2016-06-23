#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] )
{
    VideoCapture cap(argv[1]);
    if( !cap.isOpened() )
    {
        cout << "Could not initialize capturing...\n";
        return 0;
    }

    namedWindow( "frame", 1);
    Mat frame;
    for(;;)
    {
        if(waitKey(1) == 27) break;
        cap >> frame;
        if( frame.empty() )
            break;

        imshow("frame", frame);
    }

    return 0;
}
