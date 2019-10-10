//
// Created by 徐溶延 on 2019/9/8.
//

#ifndef MODELGENERATORBASEDONSKETCH_CONTOURFACTORY_H
#define MODELGENERATORBASEDONSKETCH_CONTOURFACTORY_H

#include <utility>

#include "ContourImageStrategy.h"
#include "ContourBaseStrategy.h"
#include "../utils/PmpException.h"


using namespace std;

class ContourFactory {
public:
    const static int IMAGE_STRATEGY = 0;
private:
    ContourBaseStrategy *strategy;
    ContourLoader loader;
public:
    ContourFactory(ContourLoader loader, int type) : loader(std::move(loader)) {
        switch (type) {
            case IMAGE_STRATEGY:
                strategy = new ContourImageStrategy();
                break;
            default:
                throw TypeNotFoundException();
        }
    }

    virtual ~ContourFactory() {
        //delete strategy;
    }

    ContourCollection loadContours() {
        return strategy->loadContour(loader);
    }
};


#endif //MODELGENERATORBASEDONSKETCH_CONTOURFACTORY_H
