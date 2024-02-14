#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(){

    Mat image = imread("lena.png", 0);
    int h = image.rows;
    int w = image.cols;
    //cout<< h <<'/'<< w << endl;
    Mat gamma_img = Mat_<uchar>(w, h);;
    //cout<<gamma_img.size().height<<'/'<<gamma_img.size().width;
    Mat gamma_img_2;
    MatIterator_<uchar> it, end;
    float gamma_v = 10;
    unsigned char pix[256];


    for(int i = 0; i<256; i++){
        pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma_v) * 255.0f);    
    }

    //rotate
    for(int i = 0; i<image.rows; i++){
        for(int j = 0; j<image.cols; j++){
            gamma_img.at<uchar>(image.cols-1-j,i) = image.at<uchar>(i,j);
        }
    }
    
    gamma_img_2 = gamma_img.clone();
    for (int j = 0; j<gamma_img_2.rows; j++){
        for (int i = 0; i< gamma_img_2.cols; i++){
            if (gamma_img_2.at<uchar>(j,i)<127){
                gamma_img_2.at<uchar>(j,i) = 255 - gamma_img.at<uchar>(j,i);
            }
            else{
                gamma_img_2.at<uchar>(j,i) = pix[gamma_img.at<uchar>(j,i)];
            }
        }  
    }  
    //cout << image.size().height << image.size().width ;
    imshow("gray image", image);   
    imshow("result", gamma_img_2);
    
    waitKey(0);
    return 0;
}