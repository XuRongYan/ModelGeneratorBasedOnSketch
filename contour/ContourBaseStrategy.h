//
// Created by 徐溶延 on 2019/9/7.
//

#ifndef MODELGENERATORBASEDONSKETCH_CONTOURBASESTRATEGY_H
#define MODELGENERATORBASEDONSKETCH_CONTOURBASESTRATEGY_H

#include <vector>
#include "Contour.h"
#include "ContourLoader.h"
#include "ContourCollection.h"

using namespace std;

class ContourBaseStrategy {
protected:
    Mat srcImg;
    Mat grayImg;
    Mat contourImg;
    Mat drawImg;
    ContourLoader loader;
    vector<vector<cv::Point> > vecContourPoint;
    vector<Contour> vecContours;
public:

    virtual ~ContourBaseStrategy() = default;

    virtual ContourCollection
    loadContour(ContourLoader loader) = 0;
};




#endif //MODELGENERATORBASEDONSKETCH_CONTOURBASESTRATEGY_H
