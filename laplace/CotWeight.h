//
// Created by 徐溶延 on 2019/9/5.
//

#ifndef MODELGENERATORBASEDONSKETCH_COTWEIGHT_H
#define MODELGENERATORBASEDONSKETCH_COTWEIGHT_H


#include "LaplaceWeight.h"
#include <pmp/algorithms/DifferentialGeometry.h>

class CotWeight : public LaplaceWeight {
public:
    CotWeight(const SurfaceMesh &mesh) : LaplaceWeight(mesh) {}

    ~CotWeight() override = default;

    pmp::Scalar getWeight(const Vertex v0, const Vertex neighborV) override {
        return getCotWeight(v0, neighborV);
    }
private:


    pmp::Scalar getCotWeight(const Vertex v0, const Vertex neighborV) {
        Edge e = getMesh().find_edge(v0, neighborV);
        return 0.5 * cotan_weight(getMesh(), e);
    }
};


#endif //MODELGENERATORBASEDONSKETCH_COTWEIGHT_H
