#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(){
    VideoCapture cap;
    Mat frame, result, temp, print_frame, left_roi, right_roi, center_roi, car_roi, car_avg;
    float rho_left, rho_right, rho_center, theta_left, theta_right, theta_center;
    Point p1, p2;
    int fps, delay, left_, right_, center_;
    vector<vector<Point>> contours;
    vector<Vec4i>hierarchy;
    cap.open("Project2_video.mp4");
    fps = cap.get(CAP_PROP_FPS);
    delay = 1000/fps;
    vector<Vec2f> left_lines, right_lines, center_lines;
    int pre_count = 0, count;
    Rect left_rect(60,280,240,200);
    Rect right_rect(440,280,240,200);
    Rect center_rect(300,240,140,200);
    Rect car_rect(230, 300, 200, 100);
        
    while(1){
        cap >> frame;
        int key = waitKey(delay);
        if(frame.empty() || key == 27) break;
        left_roi = frame(left_rect);
        right_roi = frame(right_rect);
        center_roi = frame(center_rect);
        car_roi = frame(car_rect);

        //Start moving

        cvtColor(car_roi, car_roi, COLOR_BGR2GRAY);
        threshold(car_roi, result, 30, 255, THRESH_BINARY_INV);
        findContours(result, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        count = contours.size();


        //lane departure
        //left 
        left_roi = frame(left_rect);
        cvtColor(left_roi, left_roi, COLOR_BGR2GRAY);
        GaussianBlur(left_roi, left_roi , Size(5, 5), 0, 0, BORDER_DEFAULT);
        Canny(left_roi, left_roi, 10, 60, 3);
        HoughLines(left_roi,left_lines, 1, CV_PI / 180, 100);

        //right
        right_roi = frame(right_rect);
        cvtColor(right_roi, right_roi, COLOR_BGR2GRAY);
        GaussianBlur(right_roi, right_roi , Size(5, 5), 0, 0, BORDER_DEFAULT);
        Canny(right_roi, right_roi, 10, 60, 3);
        HoughLines(right_roi, right_lines, 1, CV_PI / 180, 100);

        //center
        center_roi = frame(center_rect);
        cvtColor(center_roi, center_roi, COLOR_BGR2GRAY);
        GaussianBlur(center_roi, center_roi , Size(9, 9), 0, 0, BORDER_DEFAULT);
        Canny(center_roi, center_roi, 10, 60, 3);
        HoughLines(center_roi, center_lines, 3, CV_PI / 180, 100);

        //filtering
        rho_left = 0;
        rho_right = 0;
        rho_center = 0;
        theta_left = 0;
        theta_right = 0;
        theta_center = 0;
        left_ = 0;
        right_ = 0;
        center_ = 0;

        //left
        for (int i = 0; i < left_lines.size(); i++) {
            float angle = left_lines[i][1] * 180 / CV_PI ;
            if(angle <= 60 && angle >= 30){
                rho_left += left_lines[i][0];
                theta_left += left_lines[i][1];
                left_ += 1;
            }
        }
        //right
        for (int i = 0; i < right_lines.size(); i++) {
            float angle = right_lines[i][1] * 180 / CV_PI;
            if(angle <= 150 && angle >= 120){
                rho_right += right_lines[i][0];
                theta_right += right_lines[i][1];
                right_ += 1;
            }
        }
        //center
        for (int i = 0; i < center_lines.size(); i++) {
            float angle = center_lines[i][1] * 180 / CV_PI;
            if(angle <= 190 && angle >= 170){
                rho_center += center_lines[i][0];
                theta_center += center_lines[i][1];
                center_ += 1;
            }
        }
        if(right_ == 0 && left_ == 0 && center_ != 0){
            putText(frame, format("Lane departure!"), Point(50, 80), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 4);
        }
        

        pre_count = count;
        imshow("video", frame);
    }
    return 0;
    
}
