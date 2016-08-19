#include"spectralangle.h"
#include <iostream>
#include <math.h>

using namespace std;

template<typename T>
T normVec(std::vector<T> vec){
    T acc = 0.0F;
    for(auto elem : vec) acc += elem*elem;
    return sqrt(acc);
}

void SpectralAngle::initialize(MSMat back){
    background = back;

}


void SpectralAngle::apply(MSMat img, Mat &fgMask, double threshold){
    int nplanes = background.getNPlanes();
    assert(img.getNPlanes() == nplanes);


    int cols = img.getCols();
    int rows = img.getRows();

    fgMask = cv::Mat::ones(cv::Size(cols, rows), CV_8UC1);
    auto backPlanes = background.getPlanes();
    auto imgPlanes = img.getPlanes();

    //Todo use threads

    for(auto i = 0; i< cols; i+=2){
        for(auto j=0; j< rows; j+=2){
            std::vector<double> I, Mu;
            for(auto k=0; k< nplanes; k++){
                I.push_back(imgPlanes.at(k).at<double>(j,i));
                Mu.push_back(backPlanes.at(k).at<double>(j,i));
            }

//            https://en.wikipedia.org/wiki/Cosine_similarity

            auto dotProd = std::inner_product(I.begin(), I.end(), Mu.begin(), 0.0);


            auto normI  = normVec(I);
            auto normMu = normVec(Mu);
            auto distance = acos(dotProd/(normI*normMu)) / M_PI;
            auto similarity = 1-distance;
//            cout << i << "X" << j << " -> " << similarity << endl;

            fgMask.at<uchar>(j,i) = (similarity < threshold) ? 255 : 0;

        }
    }
}

