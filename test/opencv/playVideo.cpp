#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char* argv[]) {
    cv::namedWindow("frame", 1);

    cv::VideoCapture cap(argv[1]);
    if(!cap.isOpened()) {
        throw std::runtime_error("couldn't open video");
    }
    cv::Mat frame;
    cap >> frame;
    if(frame.empty()) {
        throw std::runtime_error("frame empty");
    }

    while(!frame.empty() && cv::waitKey(10) != 27) {
        cv::imshow("frame", frame);
        cap >> frame;
    }
    return 0;
}
