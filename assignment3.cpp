#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(){
    Mat moon = imread("/Users/yeseul/Documents/comvi/test/Moon.jpeg", 0);
    Mat salt = imread("/Users/yeseul/Documents/comvi/test/saltnpepper.png", 0);

    // 1. moon shapening 
    Mat filter, moon_filtered;
    int m_width = moon.cols;
    int m_height = moon.rows;
    Rect left_rect(0, 0, m_width/2, m_height);
    Mat moon_copy = moon.clone();
    GaussianBlur(moon, moon_copy , Size(5, 5), 0, 0, BORDER_DEFAULT);
    subtract(moon, moon_copy, filter);
    add(moon, filter, moon_filtered);
    moon(left_rect).copyTo(moon_filtered(left_rect));
    imshow("moon_filtered", moon_filtered);
    imshow("moon", moon);

    
    //2. saltnpepper median filter
    Mat salt_filtered;
    int s_width = salt.cols;
    int s_height = salt.rows;
    Rect right_rect(s_width/2, 0, s_width/2, s_height);
    Mat salt_copy = salt.clone();
    medianBlur(salt ,salt_filtered, 9);
    salt(right_rect).copyTo(salt_filtered(right_rect));
    imshow("saltnpepeper", salt);
    imshow("saltnpepper_filtered", salt_filtered);

    waitKey(0);
    return 0;
}
