#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(){
    Mat frame, canny_left, canny_right, image;
    float rho_left, rho_right, theta_left, theta_right, a_left, a_right, b_left, b_right, x0_left, x0_right, y0_left, y0_right;
    Point p1, p2;
    int fps;
    int delay;
    VideoCapture cap;
    Rect left_rect(200,400,400,200);
    Rect right_rect(600,400,400,200);
    vector<Vec2f> left_lines, right_lines;

    cap.open("/Users/yeseul/Documents/comvi/test/road.mp4");

    fps = cap.get(CAP_PROP_FPS);
    delay = 1000 / fps;
    for(int i = 0; i<fps*20; i++){
        cap >> frame;
        //canny_left
        image = frame(left_rect);
        cvtColor(image, image, COLOR_BGR2GRAY);
        GaussianBlur(image, image , Size(5, 5), 0, 0, BORDER_DEFAULT);
        Canny(image, canny_left, 10, 60, 3);
        HoughLines(canny_left,left_lines, 1, CV_PI / 180, 100);
        namedWindow("Left canny");
        moveWindow("Left canny",200,0);
        

        //canny_right
        image = frame(right_rect);
        cvtColor(image, image, COLOR_BGR2GRAY);
        GaussianBlur(image, image , Size(5, 5), 0, 0, BORDER_DEFAULT);
        Canny(image, canny_right, 10, 60, 3);
        HoughLines(canny_right, right_lines, 1, CV_PI / 180, 100);
        namedWindow("Right canny");
        moveWindow("Right canny", 600,0);
        
        //filtering
        rho_left = 0;
        rho_right = 0;
        theta_left = 0;
        theta_right = 0;
        int temp = 0;

        //left
        for (int i = 0; i < left_lines.size(); i++) {
            float angle = left_lines[i][1] * 180 / CV_PI ;
            if(angle <= 60 && angle >= 30){
                rho_left += left_lines[i][0];
                theta_left += left_lines[i][1];
                temp += 1;
            }
        }
        if(temp != 0){
            rho_left /= temp;
            theta_left /= temp;

            a_left = cos(theta_left);
            b_left = sin(theta_left);
            x0_left = a_left * rho_left;
            y0_left = b_left * rho_left;

            p1 = Point(cvRound(x0_left + 1000 * (-b_left))+200, cvRound(y0_left + 1000 * a_left)+400);
            p2 = Point(cvRound(x0_left - 1000 * (-b_left))+200, cvRound(y0_left - 1000 * a_left)+400);
            line(frame, p1, p2, Scalar(0, 0, 255), 3, 8);
        }
        //right
        temp = 0;
        for (int i = 0; i < right_lines.size(); i++) {
            float angle = right_lines[i][1] * 180 / CV_PI;
            if(angle <= 150 && angle >= 120){
                rho_right += right_lines[i][0];
                theta_right += right_lines[i][1];
                temp += 1;
            }
        }
        if(temp != 0){
            rho_right /= temp;
            theta_right /= temp;
            a_right = cos(theta_right);
            b_right = sin(theta_right);
            x0_right = a_right * rho_right;
            y0_right = b_right * rho_right;

            p1 = Point(cvRound(x0_right + 1000 * (-b_right))+600, cvRound(y0_right + 1000 * a_right)+400);
            p2 = Point(cvRound(x0_right - 1000 * (-b_right))+600, cvRound(y0_right - 1000 * a_right)+400);
            line(frame, p1, p2, Scalar(0, 0, 255), 3, 8); 
        }      

        imshow("Right canny", canny_right);
        imshow("Left canny", canny_left);
        imshow("Frame", frame);
        waitKey(delay);
    }

}