#include"poolingbs.h"
#include <iostream>

using namespace std;


int PoolingBS::getNplanes() const
{
    return nplanes;
}

void PoolingBS::setNplanes(int value)
{
    nplanes = value;
}

void PoolingBS::initialize(){
    cout << "initialize" << nplanes << endl;
    if(nplanes <=0)
        throw std::runtime_error("invalid number of planes");
    else{
        for(int i=0; i< nplanes; i++){
            pMogs.push_back(bgsegm::createBackgroundSubtractorMOG());
        }
    }
}


void PoolingBS::apply(MSMat img, Mat& fgMask, int threshold){
    assert(img.getNPlanes() == nplanes);
    vector<Mat> fgMasksMOG(nplanes); //generated fg masks
    auto planes = img.getPlanes();
    for(int i=0; i<nplanes;i++){
        Mat tmp;
        planes[i].convertTo(tmp, CV_8U, 255.5);
        pMogs[i]->apply(tmp, fgMasksMOG[i]);
        cv::imshow("plane", tmp);
        cv::imshow("fgMasksMOG", fgMasksMOG[i]);
        cv::waitKey(0);


    }
    fgMask = cv::Mat::zeros(cv::Size(fgMasksMOG[0].rows, fgMasksMOG[0].cols), CV_8U);
    for(auto i = 0; i< fgMask.cols; i++){
        for(auto j=0; j< fgMask.rows; j++){
            int pixelMaskCount=0;
            for(auto k = 0; k< nplanes; k++){
                pixelMaskCount+= fgMasksMOG[k].at<int>(i,j);
            }
            fgMask.at<int>(i,j) = (pixelMaskCount >= threshold) ? 1 : 0;
        }
    }
}
