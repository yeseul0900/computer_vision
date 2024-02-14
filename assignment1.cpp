#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(){
    Mat frame;
    int fps;
    int delay;
    int frame_total_count;
    int frame_current_count;
    VideoCapture cap;

    if(cap.open("background.mp4") == 0){
        cout << "No such file" << endl;
        waitKey(0);
    }

    fps = cap.get(CAP_PROP_FPS);
    delay = 1000 / fps;
    frame_total_count = cap.get(CAP_PROP_FRAME_COUNT);
    for(int i = 0; i<fps*3; i++){
        cap >> frame;
        frame_current_count = cap.get(CAP_PROP_POS_FRAMES);
        cout << frame_current_count << " / " << frame_total_count << endl;
        imshow("video", frame);
        waitKey(delay);
    }
}