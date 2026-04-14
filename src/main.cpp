#include <fmt/core.h>
#include <opencv2/opencv.hpp>
#include <ZXing/ReadBarcode.h>

void preprocess(cv::Mat& img);

int main()
{
    cv::VideoCapture cap(0, cv::CAP_DSHOW); 
    if (!cap.isOpened()) {
        fmt::print("Error: Could not open camera\n");
        return -1;
    }
    cv::Mat frame;
    while (cap.read(frame)){  
        preprocess(frame);
        cv::imshow("Video", frame);

        auto image = ZXing::ImageView(frame.data, frame.cols, frame.rows, ZXing::ImageFormat::Lum);
        auto result = ZXing::ReadBarcode(image);
        if (result.isValid()){
            fmt::print("Result: {}\n", result.text());
            fmt::print("Format: {}\n", ZXing::ToString(result.format()));
            break;
        }

        if(cv::waitKey(10) == 27) break; 
    }
    cap.release();
    return 0;
}

void preprocess(cv::Mat& frame){
    cv::Mat processed;
    cv::cvtColor(frame, processed, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(processed, processed, cv::Size(3, 3), 0);
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
    clahe->apply(processed, processed);
    cv::adaptiveThreshold(processed, processed, 255, 
                          cv::ADAPTIVE_THRESH_GAUSSIAN_C, 
                          cv::THRESH_BINARY, 11, 2);
    cv::morphologyEx(processed, processed, cv::MORPH_CLOSE, 
                    cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)));
    frame = processed;
}