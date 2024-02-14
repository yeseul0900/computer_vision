#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
int get_threshold_value(Mat image);
int main(){
    Mat finger_print = imread("finger_print.png");
    Mat adaptive_1 = imread("adaptive_1.jpg");
    Mat adaptive = imread("adaptive.png");
    cvtColor(finger_print, finger_print, CV_BGR2GRAY);
    cvtColor(adaptive_1, adaptive_1, CV_BGR2GRAY);
    cvtColor(adaptive, adaptive, CV_BGR2GRAY);
    Mat thresh;
    int thresh_T;

    //finger_print
    thresh_T = get_threshold_value(finger_print);

    threshold(finger_print, thresh, thresh_T, 255, THRESH_BINARY);
    imshow("finger_print", thresh);

    //adaptive_1
    adaptiveThreshold(adaptive_1, thresh, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 85, 15);
    imshow("adaptive_1", thresh);

    //adaptive
    adaptiveThreshold(adaptive, thresh, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 10);
    imshow("adaptive", thresh);
    waitKey(0);
    return 0;
} 


int get_threshold_value(Mat image){
    int thresh_T, low_cnt, high_cnt, low_sum, high_sum, i, j, th;

    thresh_T= 200;
    th= 10;
    low_cnt= high_cnt= low_sum= high_sum= 0;

    while (1) {
        for (j = 0; j < image.rows; j++) {
            for (i = 0; i < image.cols; i++) {
                if (image.at<uchar>(j, i) < thresh_T) {
                    low_sum += image.at<uchar>(j, i);
                    low_cnt++;
                }
                else {
                    high_sum += image.at<uchar>(j, i);
                    high_cnt++;
                }
            }
        }
        if (abs(thresh_T - (low_sum / low_cnt + high_sum / high_cnt) / 2.0f) < th) {
            break;
        }
        else {
            thresh_T = (low_sum / low_cnt + high_sum / high_cnt) / 2.0f;
            cout << "threshold value:" << thresh_T << endl;
            low_cnt = high_cnt = low_sum = high_sum = 0;
        }
    }
    return thresh_T;
}