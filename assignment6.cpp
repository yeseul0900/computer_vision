#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
void print_image(Mat frame, int mode);
int main(){
    VideoCapture cap;
    Mat frame, print_frame;
    int fps, delay, mode;
    cap.open("video.mp4");
    char keycode ;
    fps = cap.get(CAP_PROP_FPS);
    delay = 1000/fps;

    while(1){
        cap >> frame;
        if(frame.empty()) break; 
        print_frame = frame.clone();
        int key = waitKey(delay);
        if(key == 'r' || key == 'R') mode = 0; // reset
        else if(key == 'n' || key == 'N') mode = 1; // negative
        else if(key == 'g' || key == 'G') mode = 2; // gamma transformation
        else if(key == 'h' || key == 'H') mode = 3; // histogram equalization
        else if(key == 's' || key == 'S') mode = 4; // color slicing
        else if(key == 'c' || key == 'C') mode = 5; // color conversion
        else if(key == 'a' || key == 'A') mode = 6; // average filtering
        else if(key == 'u' || key == 'U') mode = 7; // unsharp masking
        else if(key == 'w' || key == 'W') mode = 8; // white balancing
        else if(frame.empty() || key == 27) break;
        print_image(frame, mode);
    }
    return 0;
}

void print_image(Mat frame, int mode){
    Mat print_frame;
    Mat temp_f;
    Mat frame_channels[3];
    print_frame = frame.clone();

    if(mode == 1){ // negative 
        cvtColor(print_frame, print_frame, COLOR_BGR2HSV);
        temp_f = print_frame.clone();
        for(int i = 0; i < frame.rows ; i ++){
            for(int j = 0; j < frame.cols ; j++){
                print_frame.at<Vec3b>(i,j)[2]= 255 - temp_f.at<Vec3b>(i,j)[2];
            }
        }
        cvtColor(print_frame, print_frame, COLOR_HSV2BGR);
    }
    else if(mode == 2){ // gamma transformation
        float gamma = 2.5;
        unsigned char pix[256];
        temp_f = print_frame.clone();
        cvtColor(temp_f, temp_f, COLOR_BGR2HSV);
        cvtColor(print_frame, print_frame, COLOR_BGR2HSV);
        for(int i = 0; i < 256; i++){
            pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
        }
        for(int i = 0; i < frame.rows; i++){
            for(int j = 0; j < frame.cols; j++){
                print_frame.at<Vec3b>(i,j)[2]= pix[temp_f.at<Vec3b>(i,j)[2]];
            }
        }
        cvtColor(print_frame, print_frame, COLOR_HSV2BGR);
    }
    else if(mode == 3){ // histogram equalization
        cvtColor(print_frame, print_frame, COLOR_BGR2HSV);
        split(print_frame, frame_channels);
        equalizeHist(frame_channels[2], frame_channels[2]);
        merge(frame_channels,3,print_frame);
        cvtColor(print_frame, print_frame, COLOR_HSV2BGR);
    }
    else if(mode == 4){ // color slicing
        uchar* h;
        uchar* s;
        cvtColor(print_frame, print_frame, COLOR_BGR2HSV);
        split(print_frame, frame_channels);
        for(int i = 0; i < print_frame.rows; i++){
            h = frame_channels[0].ptr<uchar>(i);
            s = frame_channels[1].ptr<uchar>(i);
            for(int j = 0; j < print_frame.cols; j++){
                if (h[j] > 9 && h[j] < 23) s[j] = s[j];
                else s[j] = 0;
            }
        }
        merge(frame_channels,3, print_frame);
        cvtColor(print_frame, print_frame, COLOR_HSV2BGR);
    }
    else if(mode == 5){ // color conversion
        uchar* h;
        uchar* s;
        cvtColor(print_frame, print_frame, COLOR_BGR2HSV);
        split(print_frame, frame_channels);
        for(int i = 0; i < print_frame.rows; i++){
            h = frame_channels[0].ptr<uchar>(i);
            s = frame_channels[1].ptr<uchar>(i);
            for(int j = 0; j < print_frame.cols; j++){
                if(h[j] > 129) h[j] = h[j] - 129;
                else h[j] += 50;
            }
        }
        merge(frame_channels,3, print_frame);
        cvtColor(print_frame, print_frame, COLOR_HSV2BGR);
    }
    else if(mode == 6){ // average filtering
        cvtColor(print_frame, print_frame, COLOR_BGR2HSV);
        split(print_frame, frame_channels);
        blur(frame_channels[2],frame_channels[2],Size(9,9));
        merge(frame_channels,3, print_frame);
        cvtColor(print_frame, print_frame, COLOR_HSV2BGR);
    }
    else if(mode == 7){ // unsharp masking
        cvtColor(print_frame, print_frame, COLOR_BGR2HSV);
        split(print_frame, frame_channels);

        blur(frame_channels[2],temp_f,Size(9,9));
        temp_f = frame_channels[2] - temp_f;
        frame_channels[2] = frame_channels[2] + temp_f;

        merge(frame_channels,3, print_frame);
        cvtColor(print_frame, print_frame, COLOR_HSV2BGR);
    }
    else if(mode == 8){ // white balancing
        int sum , temp;
        double avg;
        split(print_frame, frame_channels);
        for(int c = 0; c < print_frame.channels(); c++){
            sum = 0;
            avg = 0.0f;
            for(int i = 0; i < print_frame.rows; i++){
                for(int j = 0; j < print_frame.cols; j++){
                    sum += frame_channels[c].at<uchar>(i,j);
                }
            }

            avg = sum / (print_frame.rows * print_frame.cols);

            for(int i = 0; i < print_frame.rows; i++){
                for(int j = 0; j < print_frame.cols; j++){
                    temp = (128/avg) * frame_channels[c].at<uchar>(i,j);
                    if(temp > 255) frame_channels[c].at<uchar>(i,j) = 255;
                    else frame_channels[c].at<uchar>(i,j) = temp;
                }
            }
        }
        merge(frame_channels,3, print_frame);
    }
    imshow("video", print_frame);
}