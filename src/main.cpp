#include <fmt/core.h>
#include <opencv2/opencv.hpp>
#include <ZXing/ReadBarcode.h>

int main()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        fmt::print("Error: Could not open camera\n");
        return -1;
    }

    cv::Mat frame, grayframe;
    while (true){
        cap>>frame;
        if(frame.empty()) break;
        cv::cvtColor(frame, grayframe, cv::COLOR_BGR2GRAY);

        cv::imshow("Video", grayframe);

        ZXing::ImageView image(
            grayframe.data,
            grayframe.cols,
            grayframe.rows,
            ZXing::ImageFormat::Lum,
            grayframe.step
        );

        auto results = ZXing::ReadBarcode(image);
        if (results.isValid()){
            fmt::print(results.text());
            break;
        }

        if(cv::waitKey(10) == 27) break; 
    }
    cap.release();
    return 0;
}