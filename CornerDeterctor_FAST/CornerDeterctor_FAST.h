#pragma once
#include <opencv2/opencv.hpp>
#include "CornerDeterctor_FAST.h"
#include <vector>


struct Corner
{
    cv::Point2i point;
    int score;
};

int calculateIntensity(const cv::Mat& image, int x, int y)
{
    return static_cast<int>(image.at<uchar>(y, x));
}

bool isCorner(const cv::Mat& image, int x, int y, int threshold, int n)
{
    int centerIntensity = calculateIntensity(image, x, y);
    int brighterCount = 0;
    int darkerCount = 0;

    const int offsets[16][2] = 
    {
        {3, 0}, {3, -1}, {2, -2}, {1, -3},
        {0, -3}, {-1, -3}, {-2, -2}, {-3, -1},
        {-3, 0}, {-3, 1}, {-2, 2}, {-1, 3},
        {0, 3}, {1, 3}, {2, 2}, {3, 1}
    };

    for (int i = 0; i < 16; ++i)
    {
        int offsetX = x + offsets[i][0];
        int offsetY = y + offsets[i][1];

        int intensity = calculateIntensity(image, offsetX, offsetY);

        if (intensity > centerIntensity + threshold)
        {
            ++brighterCount;
        }
        else if (intensity < centerIntensity - threshold)
        {
            ++darkerCount;
        }

        if (brighterCount >= n || darkerCount >= n)
        {
            return true;  // Pixel is corner
        }
    }

    return false;
}

std::vector<Corner> detectCorners(const cv::Mat& image, int threshold, int n)
{
    std::vector<Corner> corners;

    for (int y = 3; y < image.rows - 3; ++y)
    {
        for (int x = 3; x < image.cols - 3; ++x)
        {
            //  high-speed test
            if (std::abs(calculateIntensity(image, x, y + 1) - calculateIntensity(image, x, y)) >= threshold ||
                std::abs(calculateIntensity(image, x - 1, y) - calculateIntensity(image, x, y)) >= threshold)
            {
                continue;  // did not pass the high-speed test
            }

            // Checking if a pixel is a corner pixel
            if (isCorner(image, x, y, threshold, n))
            {
                Corner corner;
                corner.point = cv::Point2i(x, y);
                corner.score = calculateIntensity(image, x, y);
                corners.push_back(corner);
            }
        }
    }

    return corners;
}