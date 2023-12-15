#include "CornerDeterctor_FAST.h"


int main() 
{
    cv::Mat image = cv::imread("image1.jpeg", cv::IMREAD_GRAYSCALE);

    if (image.empty())
    {
        std::cerr << "Failed to load the image." << std::endl;
        return -1;
    }

    int threshold = 20;
    int n = 12;

    std::vector<Corner> corners = detectCorners(image, threshold, n);

    cv::Mat imageWithCorners = image.clone();
    cv::cvtColor(imageWithCorners, imageWithCorners, cv::COLOR_GRAY2BGR);

    for (const auto& corner : corners)
    {
        cv::circle(imageWithCorners, corner.point, 3, cv::Scalar(0, 255, 0), -1);
    }
    
    cv::imshow("FAST Corner Detector", imageWithCorners);
    cv::waitKey(0);

    return 0;
}