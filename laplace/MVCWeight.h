//
// Created by 徐溶延 on 2019/9/5.
//

#ifndef MODELGENERATORBASEDONSKETCH_MVCWEIGHT_H
#define MODELGENERATORBASEDONSKETCH_MVCWEIGHT_H

#include <utility>
#include <cmath>
#include <pmp/algorithms/DifferentialGeometry.h>
#include "../utils/PmpUtils.h"
#include "LaplaceWeight.h"

using namespace pmp;
using namespace std;

class MVCWeight : public LaplaceWeight {
public:
    MVCWeight(const SurfaceMesh &mesh) : LaplaceWeight(mesh) {}

    ~MVCWeight() override = default;

    pmp::Scalar getWeight(const Vertex v0, const Vertex neighborV) override {
        return getMvcWeight(v0, neighborV);
    }

private:
    pmp::Scalar getMvcWeight(const Vertex v0, const Vertex neighborV) {
        Edge e = getMesh().find_edge(v0, neighborV);
        if(getMesh().is_boundary(e)) return 0;
        pair<pmp::Scalar, pmp::Scalar> alphas = getAlphas(v0, neighborV);
        pmp::Scalar distaceV_NeighborV = getDistance(v0, neighborV);
        return (tan(alphas.first / 2) + tan(alphas.second / 2)) / distaceV_NeighborV;
    }

    pair<pmp::Scalar, pmp::Scalar> getAlphas(const Vertex v0, const Vertex neighborV) {
        pair<Vertex, Vertex> verticesOppositeEdge = PmpUtils::findVerticesOppositeEdge(getMesh(), v0, neighborV);

        return getAlphas(v0, neighborV, verticesOppositeEdge);
    }

    pair<pmp::Scalar, pmp::Scalar>
    getAlphas(const Vertex v0, const Vertex neighborV, const pair<Vertex, Vertex> verticesOppositeEdge) {
        pmp::Point pV0 = getPosition(v0);
        pmp::Point pNeiV = getPosition(neighborV);
        pmp::Point pLeft = getPosition(verticesOppositeEdge.first);
        pmp::Point pRight = getPosition(verticesOppositeEdge.second);
        pmp::Scalar alpha1 = angle(pNeiV - pV0, pLeft - pV0);
        pmp::Scalar alpha2 = angle(pNeiV - pV0, pRight - pV0);
        return make_pair(alpha1, alpha2);
    }

    pmp::Scalar getDistance(const Vertex v0, const Vertex neightborV) {
        pmp::Point pV0 = PmpUtils::vertex2Point(getMesh(), v0);
        pmp::Point pNeighborV = PmpUtils::vertex2Point(getMesh(), neightborV);
        return distance(pV0, pNeighborV);
    }
};


#endif //MODELGENERATORBASEDONSKETCH_MVCWEIGHT_H
