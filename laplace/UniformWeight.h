//
// Created by 徐溶延 on 2019/9/5.
//

#ifndef MODELGENERATORBASEDONSKETCH_UNIFORMWEIGHT_H
#define MODELGENERATORBASEDONSKETCH_UNIFORMWEIGHT_H


#include "LaplaceWeight.h"

class UniformWeight : public LaplaceWeight{
public:
    UniformWeight(const SurfaceMesh &mesh) : LaplaceWeight(mesh) {}

    ~UniformWeight() override = default;

    pmp::Scalar getWeight(const Vertex v0, const Vertex neighborV) override {
        return getUniformWeight(v0, neighborV);
    }

private:
    pmp::Scalar getUniformWeight(const Vertex v0, const Vertex neighborV) {
        return 1;
    }
};


#endif //MODELGENERATORBASEDONSKETCH_UNIFORMWEIGHT_H
