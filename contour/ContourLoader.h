//
// Created by 徐溶延 on 2019/9/8.
//

#ifndef MODELGENERATORBASEDONSKETCH_CONTOURLOADER_H
#define MODELGENERATORBASEDONSKETCH_CONTOURLOADER_H

#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const static int CANNY = 0;
const static int THRESHOLD = 1;

class ContourLoader {
private:
    string fileName;
    int detectionOperator;
    size_t interval;
    double threshold;
    bool draw;
    string saveImgPath;
public:
    ContourLoader() {}

    class Builder {
    private:
        string fileName;
        int detectionOperator;
        size_t interval;
        double threshold;
        bool draw;
        string saveImgPath;

    public:
        Builder() {
            this->threshold = 100;
            this->draw = false;
            this->saveImgPath = "contour.jpg";
            this->interval = 3;
        }

        Builder &setFileName(const string fileName) {
            this->fileName = fileName;
            return *this;
        }


        Builder &setOperator(int detectionOperator) {
            this->detectionOperator = detectionOperator;
            return *this;
        }

        Builder &setThreshold(double threshold) {
            this->threshold = threshold;
            return *this;
        }


        Builder &setDraw(bool draw) {
            Builder::draw = draw;
            return *this;
        }

        Builder &setInterval(size_t interval) {
            Builder::interval = interval;
            return *this;
        }


        Builder &setSaveImgPath(const string &saveImgPath) {
            Builder::saveImgPath = saveImgPath;
            return *this;
        }

        double getThreshold() const {
            return threshold;
        }

        const string &getFileName() const {
            return fileName;
        }


        int getDetectionOperator() const {
            return detectionOperator;
        }

        bool isDraw() const {
            return draw;
        }

        const string &getSaveImgPath() const {
            return saveImgPath;
        }

        size_t getInterval() const {
            return interval;
        }

        ContourLoader build() {
            return ContourLoader(*this);
        }
    };

    explicit ContourLoader(const Builder& builder) {
        this->fileName = builder.getFileName();
        this->detectionOperator = builder.getDetectionOperator();
        this->threshold = builder.getThreshold();
        this->draw = builder.isDraw();
        this->saveImgPath = builder.getSaveImgPath();
        this->interval = builder.getInterval();
    }


    const string &getFileName() const {
        return fileName;
    }

    void setFileName(const string &fileName) {
        ContourLoader::fileName = fileName;
    }


    int getDetectionOperator() const {
        return detectionOperator;
    }

    void setDetectionOperator(int detectionOperator) {
        ContourLoader::detectionOperator = detectionOperator;
    }

    double getThreshold() const {
        return threshold;
    }

    void setThreshold(double threshold) {
        ContourLoader::threshold = threshold;
    }

    bool isDraw() const {
        return draw;
    }

    void setDraw(bool draw) {
        ContourLoader::draw = draw;
    }

    const string &getSaveImgPath() const {
        return saveImgPath;
    }

    void setSaveImgPath(const string &saveImgPath) {
        ContourLoader::saveImgPath = saveImgPath;
    }

    size_t getInterval() const {
        return interval;
    }

    void setInterval(size_t interval) {
        ContourLoader::interval = interval;
    }

};


#endif //MODELGENERATORBASEDONSKETCH_CONTOURLOADER_H
