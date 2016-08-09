#ifndef MSMAT_H
#define MSMAT_H



#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/bgsegm.hpp>

using namespace cv;
using namespace std;

class MSMat{
private:
    std::vector<cv::Mat> planes;
    std::vector<double> r_weights;
    std::vector<double> g_weights;
    std::vector<double> b_weights;
    int cols;
    int rows;

    //int p_type = CV_32FC1;

    void normalize_weights();

 public:
    MSMat(){}
    MSMat(int w, int h){cols = w; rows= h;}
    MSMat(int w, int h, vector<cv::Mat> imgs, vector<double> r_w, vector<double> g_w, vector<double> b_w);
    cv::Mat toMat(int type=CV_64FC3);
    cv::Mat mergeRGBNIR(Mat rgb, Mat nir, vector<double> bw = {.5,.0,.0,.5}, vector<double> gw = {.0,.5,.0,.5}, vector<double> rw = {.0,.0,.5,.5});
};


#endif // MSMAT_H
