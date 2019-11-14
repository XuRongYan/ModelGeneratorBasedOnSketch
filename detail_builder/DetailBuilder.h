//
// Created by 徐溶延 on 2019/11/6.
//

#ifndef MODELGENERATORBASEDONSKETCH_DETAILBUILDER_H
#define MODELGENERATORBASEDONSKETCH_DETAILBUILDER_H

#include <pmp/algorithms/TriangleKdTree.h>
#include "ExponentialMap.h"
#include "RegionGrowth.h"
#include "../barycenter/BarycenterCoordinate.h"
#include "../triangulation/TriangulationUtil.h"
#include "../io/MeshIO.h"

using namespace pmp;
using namespace std;

class DetailBuilder {
private:
    SurfaceMesh originalMesh, expedMesh, refinedMesh;
    Region region;
    vector<BarycenterCoordinate> barycenterCoordinates;
    vector<int> faceId;
    map<int, int> faceIdMap;
    map<int, int> originalBoundMap;
    vector<pmp::Point> newPoints;
    vector<pmp::Vertex> newVertex;
    int originalN{};
    pmp::Point polarAxis;
public:
    DetailBuilder() = default;

    DetailBuilder(SurfaceMesh &originalMesh) : originalMesh(originalMesh) {
        originalN = originalMesh.n_vertices();
    }

    void build() {
        remesh();
    }


    static pmp::Face faceOfPoint(const pmp::Point &p, const SurfaceMesh &mesh) {
        TriangleKdTree kdTree(mesh);
        auto nearest = kdTree.nearest(p);
        return nearest.face;
    }

private:
    void remesh() {
        pmp::Point choosedP(160.392, -85.8431, 10.732);
        Vertex v = PmpUtils::findVertexByPosition(originalMesh, choosedP);
        regionGrowth(v);
        expMap();
        triangulate();
        getBarycenterCoordinates();
        getNewPoints();
        deleteOldPointsAndFaces();
        addNewPointsAndFaces();
    }

    void regionGrowth(const pmp::Vertex &v) {
        int maxLevel = 2;
        RegionGrowth growth(2);
        region = growth.generateRegion(originalMesh, v);
        int n = region.points.size();
        for (size_t i = n - region.boundaryNum; i < n; i++) {
            cout << region.points[i] << endl;
        }
        logDebug() << "region vertices num =" << region.vertices.size();
    }

    void expMap() {
        ExponentialMap exponentialMap(originalMesh, region);
        exponentialMap.expMap();
        exponentialMap.buildMesh(expedMesh);
        MeshIO::save_mesh(expedMesh, "region2D.obj");
        faceIdMap = exponentialMap.getFaceIdMap();
        polarAxis = exponentialMap.getPolarAxises().at(0);
    }

    void triangulate() {
        vector<pmp::Point> details;
        drawCircle(7, 20, details);
        refinedMesh = xry_mesh::refineMesh(expedMesh, details);
        for (auto v : refinedMesh.vertices()) {
            if (!refinedMesh.is_boundary(v)) continue;
            pmp::Scalar MAX = INT_MAX;
            size_t index = 0;
            pmp::Point p = refinedMesh.position(v);
            int n = region.points.size();
            for (size_t i = n - region.boundaryNum; i < n; i++) {
                assert(expedMesh.is_boundary(pmp::Vertex(i)));
                pmp::Vertex expV = pmp::Vertex(i);
                if (pmp::norm(p - expedMesh.position(expV)) < MAX) {
                    MAX = pmp::norm(p - expedMesh.position(expV));
                    index = i;
                }
            }
            originalBoundMap[v.idx()] = region.originalVerticesMap[index];
        }
        MeshIO::save_mesh(refinedMesh, "refinedRegion2D.obj");
    }

    void getBarycenterCoordinates() {
        barycenterCoordinates.resize(refinedMesh.n_vertices());
        faceId.resize(refinedMesh.n_vertices());
        for (auto v : refinedMesh.vertices()) {
            if (refinedMesh.is_boundary(v)) continue;
            pmp::Point p = refinedMesh.position(v);
            pmp::Face f = faceOfPoint(p, expedMesh);
            barycenterCoordinates[v.idx()] = BarycenterCoordinate(p, expedMesh, f);
            faceId[v.idx()] = f.idx();
        }
    }

    void getNewPoints() {
        for (size_t i = 0; i < barycenterCoordinates.size(); i++) {
            pmp::Face f = pmp::Face(faceId[i]);
            vector<pmp::Vertex> vecV;
            for (auto v : expedMesh.vertices(f)) {
                vecV.push_back(v);
            }
            pmp::Point A = originalMesh.position(pmp::Vertex(region.originalVerticesMap[vecV[0].idx()]));
            pmp::Point B = originalMesh.position(pmp::Vertex(region.originalVerticesMap[vecV[1].idx()]));
            pmp::Point C = originalMesh.position(pmp::Vertex(region.originalVerticesMap[vecV[2].idx()]));
            newPoints.push_back(PmpUtils::getPointByBarycenterCoor(A, B, C, barycenterCoordinates[i]));
        }
    }


    void addNewPointsAndFaces() {
        for (auto p : newPoints) {
            pmp::Vertex v = originalMesh.add_vertex(p);
            newVertex.push_back(v);
        }
        for (auto f : refinedMesh.faces()) {
            vector<pmp::Vertex> vecV;
            for (auto v : refinedMesh.vertices(f)) {
                if (refinedMesh.is_boundary(v)) {
                    assert(originalBoundMap.find(v.idx()) != originalBoundMap.end());
                    int idx = originalBoundMap[v.idx()];
                    pmp::Vertex originalV = pmp::Vertex(idx);
                    assert(originalMesh.is_valid(originalV));
                    vecV.push_back(originalV);
                } else {
                    vecV.push_back(newVertex[v.idx()]);
                }
            }
            pmp::Face face = originalMesh.add_triangle(vecV[0], vecV[1], vecV[2]);
        }
        originalMesh.garbage_collection();
        MeshIO::save_mesh(originalMesh, "refined_3d_mesh.obj");
    }

    void deleteOldPointsAndFaces() {
        int count = 0, i = 0;
        for (auto id : region.faceIds) {
            originalMesh.delete_face(pmp::Face(id));
        }
        MeshIO::save_mesh(originalMesh, "deleted_mesh.obj");
    }

    void drawCircle(pmp::Scalar r, size_t n, vector<pmp::Point> &details) {
        details.clear();
        pmp::Scalar step = 2 * pi / n;
        vector<ExponentialCoordinate> expCoordinates;
        for (size_t i = 0; i < n; i++) {
            expCoordinates.emplace_back(Eigen::Vector2f(i * step, r));
        }
        ExponentialMap::exp2xyz(details, polarAxis, expCoordinates);
    }
};


#endif //MODELGENERATORBASEDONSKETCH_DETAILBUILDER_H
