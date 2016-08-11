#ifndef POOLINGBS_H
#define POOLINGBS_H


#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/bgsegm.hpp>

#include "msmat.h"

class PoolingBS{
private:
    //TODO algorithm
    int nplanes;

    //vector<Mat> fgMasksMOG; //generated fg masks
    vector<Ptr<BackgroundSubtractor>> pMogs;
public:
    PoolingBS(){nplanes=-1;}
    PoolingBS(int np){ nplanes=np;}
    int getNplanes() const;
    void setNplanes(int value);
    void initialize(); //todo pass algorithm
    void apply(MSMat img, Mat &fgMask, int threshold=1);

};
#endif // POOLINGBS_H
