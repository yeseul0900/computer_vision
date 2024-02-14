#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
Mat make_panorama(Mat query, Mat image);

int main(){
    Mat img_1, img_2, img_3, img_4, result1, result2, panorama_img;
    img_1 = imread("pano1.JPG");
    img_2 = imread("pano2.JPG"); 
    img_3 = imread("pano3.JPG");  
    img_4 = imread("pano4.JPG"); 

    result1 = make_panorama(img_1, img_2);
    result2 = make_panorama(result1, img_3);
    panorama_img = make_panorama(result2, img_4);
    imshow("panorama_img", panorama_img);
    waitKey(0);
    return 0;
}

Mat make_panorama(Mat left_img, Mat right_img){

    Mat panorama_img, descriptors1, descriptors2, mask ;
    Ptr<ORB> orbF = ORB::create(1000);
    vector <KeyPoint> keypoints1, keypoints2;
    vector< vector< DMatch> > matches;
    vector< DMatch > goodMatches;
    BFMatcher matcher(NORM_HAMMING);
    int i, k;
    float nndr;

    orbF->detectAndCompute(left_img, noArray(), keypoints1, descriptors1);
    orbF->detectAndCompute(right_img, noArray(), keypoints2, descriptors2);
    
    k = 2;
    matcher.knnMatch(descriptors1, descriptors2, matches, 2);

    nndr = 0.6f;
    for(i = 0; i < matches.size(); i ++){
        if(matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance){
            goodMatches.push_back(matches[i][0]);
        }
    }
    vector<Point2f> ptr1, ptr2;
    for(i = 0; i<goodMatches.size(); i++){
        ptr1.push_back(keypoints1[goodMatches[i].queryIdx].pt);
        ptr2.push_back(keypoints2[goodMatches[i].trainIdx].pt);
    }

    Mat H = findHomography(ptr2, ptr1, RANSAC);
    //cout<<H<<endl;
    warpPerspective(right_img, panorama_img, H, Size(left_img.cols*1.3, left_img.rows*1.1));
    Mat pano_roi(panorama_img, Rect(0,0,left_img.cols, left_img.rows));
    threshold(left_img, mask, 1, 255, THRESH_BINARY);
    left_img.copyTo(pano_roi, mask);
    return panorama_img;
}