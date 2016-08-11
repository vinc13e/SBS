#include <iostream>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/bgsegm.hpp>

#include"msmat.h"
#include"poolingbs.h"

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
        "{algorithm      a | 2       | Algorithm to be used, 0:ABSFIDD, 1:MOG, (default) 2:MOG2, 3:GMG }";



int main_(int argc, char *argv[])
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

        imshow("original",img);
        imshow("Foreground Mask",fgMaskMOG);

        char key = cv::waitKey(100);
        if(key == 'q' || key == 27){
            return;
        }

    }
}




int main__(){
    // cv::Mat in = imread("/home/vmachado/Pictures/italy.jpg");//
    cv::Mat inRGB = imread("/home/vmachado/Desktop/thermaltestimages/img_00000.bmp");
    cv::Mat inNIR = imread("/home/vmachado/Desktop/thermaltestimages/img_00001.bmp", CV_LOAD_IMAGE_GRAYSCALE);

    cv::imshow("inRGB", inRGB);
    cv::imshow("inNIR", inNIR);
    cv::waitKey(0);

    vector<Mat> inChannels(4);
    cv::split(inRGB, inChannels);
    inChannels.push_back(inNIR);

    cv::imshow("B",inChannels.at(0)); cv::waitKey(0);

    std::vector<double> b_w{.5, .0, .0, .5};
    std::vector<double> g_w{.0, .5, .0, .5};
    std::vector<double> r_w{.0, .0, .5, .5};

    MSMat msimg(640, 480, inChannels, r_w, g_w, b_w);
    auto ret = msimg.toMat();

    cv::imshow("ret", ret); cv::waitKey(0);
}

/*
cv::Mat poolingBS(MSMat image){
    int nplanes = image.getNPlanes();

    vector<Mat> fgMasksMOG; //generated fg masks
    vector<Ptr<BackgroundSubtractor>> pMogs;

    for(int i=0; i< nplanes; i++){
        pMogs.push_back(bgsegm::createBackgroundSubtractorMOG());
    }

}
*/

int main_fusion(){

    //////////
    //    std::vector<Mat> planes;
    //    cv::imreadmulti("/home/vmachado/Desktop/thermaltestimages/img000097.tif", planes);
    //    cout << planes.size() << endl;
    //    for(auto img : planes){
    //        cv::imshow("plane", img); cv::waitKey(0);
    //    }
    //    int rows = planes.at(0).rows;
    //    int cols = planes.at(0).cols;
    //    MSMat ms(cols, rows, planes);
    //    auto rgb = ms.toMat();
    //    cv::imshow("fusioned", rgb); cv::waitKey(0);

    //    cout << rgb.at<Point3d>(324,34);

    //    return 0;



    // "Bench"

    Mat fgMaskMOG; //generated fg mask
    Ptr <BackgroundSubtractor> pMOG;
    pMOG = bgsegm::createBackgroundSubtractorGMG(100, 0.5);

    for(int i=0; i< 2107; i+=2){
        char pRGB[1024];
        char pNIR[1024];
        sprintf(pRGB, "/home/vmachado/Desktop/thermaltestimages/1b/img_%05d.bmp", i);
        sprintf(pNIR, "/home/vmachado/Desktop/thermaltestimages/1a/img_%05d.bmp", i+1);

        //        cout << pRGB << endl;
        //        cout << pNIR << endl;

        Mat inRGB = imread(pRGB);
        Mat inNIR = imread(pNIR, CV_LOAD_IMAGE_GRAYSCALE);

        MSMat a;
        auto img = a.mergeRGBNIR(inRGB, inNIR);
        img.convertTo(img, CV_8UC3, 255.5);
        // cv::imshow("mixed", ret); cv::waitKey(0);

        pMOG->apply(inNIR, fgMaskMOG);

        imshow("nir",inNIR);
        imshow("rgb",inRGB);
        imshow("f",img);
        imshow("Foreground Mask",fgMaskMOG);

        char key = cv::waitKey(1);
        if(key == 'q' || key == 27){
            break;
        }
    }
}




int main() {
    //Pooling . . .

    std::vector<Mat> planes;

    //TODO -- do with a seq of images (tiff) vis2 bourgonha ...

    cv::imreadmulti("/home/vmachado/Desktop/thermaltestimages/img000097.tif", planes);

    MSMat ms(planes.at(0).cols, planes.at(0).rows, planes);


    PoolingBS pbs = PoolingBS(7);
    cv::Mat out;

    pbs.initialize();
    pbs.apply(ms, out);
}
