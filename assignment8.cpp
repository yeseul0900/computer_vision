#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
struct MouseParams{
    Mat output_img;
    vector<Point2f> in, out;
};
static void onMouse(int event, int x, int y, int, void* param){
    MouseParams* mp= (MouseParams*)param;
    Mat output_img = mp -> output_img;
    if (event == EVENT_LBUTTONDOWN)
    {
        mp->out.push_back(Point2f(x, y));
    }
    if (event == EVENT_RBUTTONDOWN){
        mp->out.clear();
        imshow("Background", output_img);
    }
}

int main(){
    VideoCapture inv, outv;
    MouseParams mp;

    inv.open("contest.mp4");
    outv.open("Timesquare.mp4");

    Mat input, output, input_resize, result, mask;

    inv >> input;
    outv >> output;
    resize(input, input_resize, Size(output.size().width, output.size().height));
    mp.output_img = output;
    mp.in.push_back(Point2f(0, 0));
    mp.in.push_back(Point2f(input_resize.size().width, 0));
    mp.in.push_back(Point2f(input_resize.size().width, input_resize.size().height));
    mp.in.push_back(Point2f(0, input_resize.size().height));

    namedWindow("Background");
    setMouseCallback("Background", onMouse, (void*)&mp);
    while(1){
        imshow("input", input);
        if(mp.out.size()<4){
            result = mp.output_img.clone();
            for (size_t i= 0; i< mp.out.size(); i++){
                circle(result, mp.out[i], 3, Scalar(0, 0, 255), 5);
            }
            imshow("Background", result);
        }
        else if(mp.out.size() == 4){
            Mat homo_mat = getPerspectiveTransform(mp.in, mp.out);
            warpPerspective(input_resize, result, homo_mat, Size(output.size().width, output.size().height));

            vector<Point> points;
            for (const auto& pt : mp.out) {
                points.push_back(Point(static_cast<int>(pt.x), static_cast<int>(pt.y)));
            }
            vector<vector<Point>> contours{ points };
            fillPoly(output, contours, Scalar(0,0,0));
            threshold(result, mask, 1, 255, THRESH_BINARY);
            result.copyTo(output, mask);
            imshow("Background", output);
        }
        inv >> input;
        outv >> output;
        mp.output_img = output;

        if(input.empty() || output.empty()) break;
        resize(input, input_resize, Size(output.size().width, output.size().height));

        waitKey(33);
    }
}

