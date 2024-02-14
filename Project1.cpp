#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int Matching_img(int mode, Mat query, Mat image);
int main(){
    Mat query_img, db_img;
    vector<String> image;
    string user_enter;
    string q_path = "/query_image/";
    string db_path = "/DBs/*.jpg" ;
    int mode, goodmatch_cnt;
    glob(db_path, image, true);
    int gm_list[image.size()];
    int max, max_index;

    cin >> user_enter;
    q_path += user_enter;
    query_img = imread(q_path);

    if(query_img.empty()){
        cout << "no such file";
    } 
    else{
        for(int i = 0; i<image.size(); i++){
            db_img = imread(image[i]);
            goodmatch_cnt = Matching_img(0, query_img, db_img);
            cout<< "Image number " << i+1 << " Matching: " << goodmatch_cnt <<endl;
            gm_list[i] = goodmatch_cnt;
        }
        for(int i = 0; i<image.size(); i++){
            if(i == 0){
                max = gm_list[i];
                max_index = i;
            }
            else if(max < gm_list[i]){
                max = gm_list[i];
                max_index = i;
            }
        }
        db_img = imread(image[max_index]);
        Matching_img(1, query_img, db_img);
        cout << max_index;
    }
    return 0;
}

int Matching_img(int mode, Mat query, Mat image){
    Mat descriptors1, descriptors2;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2;
    vector< vector< DMatch> > matches; //DMatch is descriptor match vector< DMatch > goodMatches;
    vector< DMatch > goodMatches;
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches;
    
    int i, k; 
    float nndr;

    resize(query, query, Size(640, 480)); 
    resize(image, image, Size(640, 480));

    if (query.empty() || image.empty()) return -1;

    //Compute ORB Features
    orbF->detectAndCompute(query, noArray(), keypoints1, descriptors1); 
    orbF->detectAndCompute(image, noArray(), keypoints2, descriptors2);
    
    //KNN Matching(k-nearest neighbor matching)
    //Find best and second-best matches
    k = 2;
    matcher.knnMatch(descriptors1, descriptors2, matches, k);

    // Find out the best match is definitely better than the second-best match 
    nndr = 0.56f;
    for (i = 0; i < matches.size(); i++) {
        if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance) {
            goodMatches.push_back(matches[i][0]);
        }
    }
    if(mode == 1){
        //Draws the found matches of keypoints from two images.
        drawMatches(query, keypoints1, image, keypoints2, goodMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
        imshow("imgMatches", imgMatches);
        imshow("query", query);
        waitKey(0);
    }

    return goodMatches.size();
}