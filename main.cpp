#include <iostream>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/bgsegm.hpp>

namespace sbs{
    enum {ABSDIFF, MOG, MOG2, GMG};
}

using namespace cv;
using namespace std;
using namespace sbs;

void run_absdiff(VideoCapture & cam);
void run_mog(VideoCapture & cam, int algorithm);


cv::String commands =
        "{help h usage   ? |         | Print this message         }"
        "{@inputData     i |         | Where to find input video  }"
        "{algorithm      a | 3       | Algorithm to be used, 0:ABSFIDD, 1:MOG, 2:MOG2, 3:GMG }";



int main(int argc, char *argv[])
{
    cv::CommandLineParser parser(argc,argv,commands);

    if (parser.has("help") || argc < 2)
    {
        parser.printMessage();
        return 0;
    }

    std::string inFile = parser.get<string>(0);
    int algorithm      = parser.get<int>("algorithm");
    cout << "algorithm: " << algorithm << endl;

    VideoCapture cam;

    auto webCam  = false;
    auto webCamI = 0;
    if(!strncmp(inFile.c_str(), "/dev/video", strlen("/dev/video"))){ // works for video0 to video9, video10+ fails!
        webCam=true;
        webCamI = std::atoi(inFile.c_str()+strlen(inFile.c_str())-1);
    }

    webCam ? cam.open(webCamI) : cam.open(inFile);
    if( !cam.isOpened() ) {
        cout << "Failed to open file " << inFile << endl;
        return 0;
    }

    switch(algorithm){
    case ABSDIFF:
        run_absdiff(cam);
        break;
    case MOG :
        run_mog(cam, MOG);
        break;
    case MOG2 :
        run_mog(cam, MOG2);
        break;
    case GMG :
        run_mog(cam, GMG);
        break;
    default :
        break;
    }

    return 0;
}


void run_absdiff(VideoCapture & cam){
    Mat imgA, imgB, imgC;
    cam >> imgA;
    if(!imgA.data){
        return;
    }
    cv::cvtColor(imgA,imgA, CV_BGR2GRAY);
    while(true) {
        cam >> imgB;
        if(!imgB.data){
            return;
        }
        cv::cvtColor(imgB,imgB, CV_BGR2GRAY);

        cv::absdiff(imgB,imgA,imgC);
        cv::threshold(imgC,imgC,10,255,CV_THRESH_BINARY);

        imshow("imgA",imgA);
        imshow("imgB",imgB);
        imshow("imgC",imgC);

        imgA = imgB.clone();

        char key = cv::waitKey(100);
        if(key == 'q' || key == 27){
            return;
        }
    }
}

void run_mog(VideoCapture & cam, int algorithm){

    Mat img;
    Mat fgMaskMOG; //generated fg mask
    Ptr <BackgroundSubtractor> pMOG; //MOG Background subtractor

    if(algorithm == MOG){
        pMOG = bgsegm::createBackgroundSubtractorMOG();
    }
    if(algorithm == MOG2){
        pMOG = createBackgroundSubtractorMOG2();
    }
    if(algorithm == GMG){
        pMOG = bgsegm::createBackgroundSubtractorGMG(100, 0.8);
    }

    while(true){

        cam >> img;
        pMOG->apply(img, fgMaskMOG);

        imshow("Foreground Mask",fgMaskMOG);

        char key = cv::waitKey(100);
        if(key == 'q' || key == 27){
            return;
        }

    }
}

