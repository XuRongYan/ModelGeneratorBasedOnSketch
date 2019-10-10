//
// Created by 徐溶延 on 2019/9/8.
//

#ifndef MODELGENERATORBASEDONSKETCH_CONTOURIMAGESTRATEGY_H
#define MODELGENERATORBASEDONSKETCH_CONTOURIMAGESTRATEGY_H

#include <vector>
#include <Eigen/Core>
#include <opencv2/opencv.hpp>
#include "ContourBaseStrategy.h"
#include "../utils/PmpException.h"
#include "../utils/EigenUtils.h"

using namespace std;
using namespace cv;
using namespace Eigen;

class ContourImageStrategy : public ContourBaseStrategy {
public:
    ~ContourImageStrategy() override = default;

    ContourCollection loadContour(ContourLoader loader) override {
        this->loader = loader;
        return load();
    }

    ContourCollection load() {
        srcImg = loadImgRGB(loader.getFileName());
        convertRGB2GRAY();
        detectContours(loader.getDetectionOperator());
        findContours(grayImg, vecContourPoint, RETR_TREE, CHAIN_APPROX_SIMPLE);
        drawContour();
        convertPoints2Contours();
        return ContourCollection(vecContours);
    }

private:
    Mat loadImgRGB(string fileName) {
        return imread(fileName);
    }

    void convertRGB2GRAY() {
        cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);
    }

    void detectContours(int type) {
        detectInType(grayImg, contourImg, type);
    }

    void detectInType(const Mat &src, Mat &dst, int type) {
        switch (type) {
            case CANNY:
                Canny(src, dst, loader.getThreshold(), 2 * loader.getThreshold());
                break;
            case THRESHOLD:
                threshold(src, dst, loader.getThreshold(), 255, THRESH_BINARY);
                break;
            default:
                throw TypeNotFoundException();
        }
    }

    void drawContour() {
        if (loader.isDraw()) {
            drawImg = Mat::zeros(srcImg.size(), CV_8UC3);
            drawContours(drawImg, vecContourPoint, -1, cv::Scalar(0, 255, 0), 1, 8);
            imwrite(loader.getSaveImgPath(), drawImg);
        }
    }

    void convertPoints2Contours() {
        bool isFirst = true;
        for (size_t i = 1; i < vecContourPoint.size(); i++) {
            vector<cv::Point> points = vecContourPoint[i];
            logDebug() << "contour size =" << points.size();
            vector<pmp::Scalar> contourPoints;
            getOneContourData(points, contourPoints);
            Contour contour(EigenUtils::mapMatrix3Xf(contourPoints), isFirst ? IS_OUTSKIRT : NOT_OUTSKIRT);
            isFirst = false;
            vecContours.push_back(contour);
        }
        assert(vecContours.size() > 0);
        logInfo() << "find" << vecContours.size() << "contours";
    }

    void getOneContourData(const vector<cv::Point> &points, vector<pmp::Scalar> &contourPoints) {
        //此处要把原本均处于第一象限的轮廓平移到以原点为中心
        for (size_t i = points.size() - 1; i >= 0; i -= loader.getInterval()) {
            contourPoints.push_back(points[i].x - 0.5 * grayImg.cols);
            contourPoints.push_back(points[i].y - 0.5 * grayImg.rows);
            contourPoints.push_back(0);
            if (i < loader.getInterval()) break;
        }

    }
};


#endif //MODELGENERATORBASEDONSKETCH_CONTOURIMAGESTRATEGY_H
