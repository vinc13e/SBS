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
            //pMogs.push_back(createBackgroundSubtractorMOG2());
            // pMogs.push_back(bgsegm::createBackgroundSubtractorGMG(20, 0.7));
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
//        cv::imshow("plane", tmp);
//        cv::imshow("fgMasksMOG", fgMasksMOG[i]);
//        cv::waitKey(0);


    }
    fgMask = cv::Mat::ones(cv::Size(fgMasksMOG[0].cols, fgMasksMOG[0].rows), CV_8UC1);
    for(auto i = 0; i< fgMask.cols; i++){
        for(auto j=0; j< fgMask.rows; j++){
            int pixelMaskCount=0;
            for(auto k = 0; k< nplanes; k++){
                if((int) fgMasksMOG[k].at<uchar>(j, i) > 0) pixelMaskCount++;
            }
            fgMask.at<uchar>(j, i) = (pixelMaskCount >= threshold) ? 255 : 0;
        }
    }
}
