//
// Created by 徐溶延 on 2019/10/28.
//

#ifndef MODELGENERATORBASEDONSKETCH_REGIONGROWTH_H
#define MODELGENERATORBASEDONSKETCH_REGIONGROWTH_H

#include <vector>
#include <queue>
#include <map>
#include <unordered_set>
#include <pmp/SurfaceMesh.h>
#include "../utils/PmpUtils.h"
#include "../utils/EigenUtils.h"
using namespace std;
using namespace pmp;

//TODO 将生成区域信息转化为mesh

struct Region {
    vector<pmp::Vertex> vertices;
    vector<pmp::Point> points;
    vector<int> faces;
    vector<int> faceIds;
    map<int, int> originalVerticesMap;
    size_t boundaryNum{};
};

class RegionGrowth {
public:
    vector<bool> isVisit;
    int maxLevel;
    map<int, int> id;
public:
    explicit RegionGrowth(int maxLevel) : maxLevel(maxLevel) {}

    Region generateRegion(const SurfaceMesh &mesh, const Vertex &seed) {
        int level = 0;
        Region region = Region();
        queue<Vertex> q;
        q.push(seed);
        isVisit.resize(mesh.n_vertices());
        int num = 1;
        int counter = 0;
        region.vertices.push_back(seed);
        region.points.push_back(mesh.position(seed));
        region.originalVerticesMap[region.points.size() - 1] = seed.idx();
        id[seed.idx()] = 0;
        isVisit[seed.idx()] = true;
        while (!q.empty()) {
            Vertex vs = q.front();
            q.pop();
            for (auto v : mesh.vertices(vs)) {
                if (!isVisit[v.idx()]) {
                    q.push(v);
                    region.vertices.push_back(v);
                    id[v.idx()] = region.vertices.size() - 1;
                    region.points.push_back(mesh.position(v));
                    region.originalVerticesMap[region.points.size() - 1] = v.idx();
                    assert(mesh.position(v) == region.points[region.points.size() - 1]);
                    isVisit[v.idx()] = true;
                }
            }
            for (auto f : mesh.faces(vs)) {
                for (auto v : mesh.vertices(f)) {
                    assert(id.find(v.idx()) != id.end());
                    region.faces.push_back(id[v.idx()]);
                }
                region.faceIds.push_back(f.idx());
            }
            counter++;
            if (counter == num) {
                level++;
                cout << counter << endl;
                counter = 0;
                num = q.size();
                region.boundaryNum = num;
            }
            if (level == maxLevel) break;
        }
        return region;
    }

    static void buildMesh(SurfaceMesh &mesh, const Region& region) {
        Eigen::Matrix3Xi faces = EigenUtils::mapMatrix3Xi(region.faces);
        PmpUtils::buildMesh(mesh, region.points, faces);
    }
};


#endif //MODELGENERATORBASEDONSKETCH_REGIONGROWTH_H
