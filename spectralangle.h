#ifndef SPECTRALANGLE_H
#define SPECTRALANGLE_H


#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/bgsegm.hpp>

#include "msmat.h"

class SpectralAngle{
private:
    MSMat background;

public:
    SpectralAngle(){}
    void initialize(MSMat back);
//    void setBackground(MSMat back); // updatebackground ?¿
    void apply(MSMat img, Mat &fgMask, double threshold=0.97);

};

#endif // SPECTRALANGLE_H
