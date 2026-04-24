#include <fmt/core.h>
#include <opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <ZXing/ReadBarcode.h>
#include <include/osqrlib.hpp>

void preprocess(cv::Mat& img);

int main()
{
    fmt::print("----------------------Welcome to LazyQR----------------------\n");
    fmt::print("When your camera turns on, hold up a qr code to it to open it!\n");
    cv::VideoCapture cap(0, cv::CAP_DSHOW); 
    if (!cap.isOpened()) {
        fmt::print("Error: Could not open camera\n");
        return -1;
    }
    ZXing::ReaderOptions options;
    options.setFormats(ZXing::BarcodeFormat::QRCode);

    cv::Mat frame;
    std::string linkname;
    while (cap.read(frame)){  
        cv::imshow("Video", frame);
        preprocess(frame);

        auto image = ZXing::ImageView(frame.data, frame.cols, frame.rows, ZXing::ImageFormat::Lum);
        auto result = ZXing::ReadBarcode(image);

        if (result.isValid()){
            linkname = result.text();
            break;
        }
        if(cv::waitKey(10) == 27) break; 
    }
    cap.release();
    cv::destroyAllWindows();

    if (!linkname.empty()){
        fmt::print("------------------------------------------------------------------\n");
        fmt::print("The link that has been detected is: " + linkname + '\n');
        fmt::print("Enter '1' to open! Enter anything else to close without opening link...\n>>> ");

        int input;
        std::cin >> input;
        if (input == 1){
            openurl(linkname);
        }
    }

    return 0;
}

void preprocess(cv::Mat& frame){
    cv::Mat processed;
    cv::cvtColor(frame, processed, cv::COLOR_BGR2GRAY);
    cv::resize(processed, processed, cv::Size(640, 480));  
    cv::GaussianBlur(processed, processed, cv::Size(3, 3), 0);
    frame = processed;
}