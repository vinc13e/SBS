#include"msmat.h"
#include <iostream>

using namespace cv;
using namespace std;


MSMat::MSMat(int w, int h, vector<cv::Mat> imgs, vector<double> r_w, vector<double> g_w, vector<double> b_w){
    assert((imgs.size() == r_w.size() || r_w.size()==0) && r_w.size() == g_w.size() && g_w.size() == b_w.size());
    assert(w > 0 && h >0);
    if(r_w.size()==0){
        auto n_planes = imgs.size();
        auto i = n_planes;
        vector<double> tmp;
        while(i--){
            r_weights.push_back(1.0F/n_planes);
            g_weights.push_back(1.0F/n_planes);
            b_weights.push_back(1.0F/n_planes);
        }
    }
    else{
        r_weights.swap(r_w);
        g_weights.swap(g_w);
        b_weights.swap(b_w);
    }

    rows = h;
    cols = w;
    for( auto img : imgs){
        cv::Mat tmp, tmp2;
        img.convertTo(tmp, CV_64FC1, 1.0/255); //normalizes to [0,1];
        cv::resize(tmp, tmp2, cv::Size(w,h));
        planes.push_back(tmp2.clone());
    }

}


std::vector<cv::Mat> MSMat::getPlanes() const
{
    return planes;
}

int MSMat::getRows() const
{
    return rows;
}

int MSMat::getCols() const
{
    return cols;
}

void MSMat::normalize_weights(){
    auto r_acc = std::accumulate(r_weights.begin(), r_weights.end(), 0.0F); // 0.0F defines initial value and return type (0, 0.0f, 0.0F ...)
    auto g_acc = std::accumulate(g_weights.begin(), g_weights.end(), 0.0F);
    auto b_acc = std::accumulate(b_weights.begin(), b_weights.end(), 0.0F);

    std::transform(r_weights.begin(), r_weights.end(), r_weights.begin(), [r_acc](double item){ return item/r_acc;});
    std::transform(g_weights.begin(), g_weights.end(), g_weights.begin(), [g_acc](double item){ return item/g_acc;});
    std::transform(b_weights.begin(), b_weights.end(), b_weights.begin(), [b_acc](double item){ return item/b_acc;});

}

cv::Mat MSMat::toMat(int type){ //TODO type
    normalize_weights();
    cv::Mat R = cv::Mat::zeros(rows, cols, CV_64FC1);
    cv::Mat G = cv::Mat::zeros(rows, cols, CV_64FC1);
    cv::Mat B = cv::Mat::zeros(rows, cols, CV_64FC1);
    int i =0;
    for(auto plane : planes){
        //cv::imshow("plane", plane); cv::waitKey(0);
        R += plane * r_weights.at(i);
        G += plane * g_weights.at(i);
        B += plane * b_weights.at(i);
        i++;
    }


    std::vector<cv::Mat> array_to_merge;
    array_to_merge.push_back(B);
    array_to_merge.push_back(G);
    array_to_merge.push_back(R);
    cv::Mat color(rows, cols, type);

    cv::merge(array_to_merge, color);
    color.convertTo(color, type);
    return color.clone();

}

cv::Mat MSMat::mergeRGBNIR(Mat rgb, Mat nir, vector<double> bw, vector<double> gw, vector<double> rw){
    vector<Mat> inPlanes(4);
    cv::split(rgb, inPlanes);
    inPlanes.push_back(nir);
    MSMat tmp(rgb.cols, rgb.rows, inPlanes, rw, gw, bw);
    return tmp.toMat();
}


int MSMat::getNPlanes(){
    return planes.size();
}
