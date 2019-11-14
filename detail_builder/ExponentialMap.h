//
// Created by 徐溶延 on 2019/10/29.
//

#ifndef MODELGENERATORBASEDONSKETCH_EXPONENTIALMAP_H
#define MODELGENERATORBASEDONSKETCH_EXPONENTIALMAP_H

#include <utility>
#include <vector>
#include <map>
#include <Eigen/Dense>
#include <pmp/SurfaceMesh.h>
#include "RegionGrowth.h"
#include "../utils/PmpUtils.h"
#include "../utils/EigenUtils.h"
#include "../utils/log_utils/Logger.h"
#include "ExponentialCoordinate.h"

using namespace std;
using namespace pmp;
using namespace Eigen;

class ExponentialMap {
private:
    const int INF = INT_MAX;
    SurfaceMesh mesh;
    Region region;
    map<int, int> faceIdMap;
    vector<pmp::Scalar> d;
    vector<bool> visit;
    vector<ExponentialCoordinate> expCoordinates;
    vector<pmp::Point> polarAxises;
    vector<int> pre;
    bool calculated;
    bool mapped;

public:
    ExponentialMap(const SurfaceMesh &mesh, Region region) : mesh(mesh), region(std::move(region)) {
        init();
    }

    vector<ExponentialCoordinate> expMap() {
        if (!calculated) {
            dijkstra();
            calculated = true;
        }
        return expCoordinates;
    }

    void buildMesh(SurfaceMesh &mesh) {
        if (!calculated) {
            logError() << "haven't calculated exponential map";
            return;
        }
        vector<pmp::Point> points;
        exp2xyz(points, polarAxises[0], expCoordinates);
        Eigen::Matrix3Xi faces = EigenUtils::mapMatrix3Xi(region.faces);
        PmpUtils::buildMesh(mesh, points, faces);
        buildFaceIdMap(mesh);

    }

    const map<int, int> &getFaceIdMap() const {
        if (!mapped) {
            logError() << "haven't mapped";
        }
        return faceIdMap;
    }

    bool isMapped() const {
        return mapped;
    }

    static pmp::Scalar angle(const pmp::Point &p1, const pmp::Point &p2) {
        pmp::Scalar result = pmp::angle(p1, p2);
        if (angleLargeThanPi(p1, p2)) {
            result = 2 * pi - result;
        }
        return result;
    }

    void printPath() {
        if (!calculated) {
            cout << "haven't calculated" << endl;
            return;
        }
        for (int i = 7; i < region.points.size(); i++) {
            dfs(0, i);
            cout << endl;
        }
    }

    /**
    * 空间中的向量p绕轴A旋转theta度
    * @param p
    * @param A
    * @return
    */
    static pmp::Point rotate3D(const pmp::Point &p, const pmp::Point &A, pmp::Scalar theta) {
        //TODO 精度不是很高
        pmp::Point normalA = pmp::normalize(A);
        return p * cos(theta) + pmp::cross(normalA, p) * sin(theta) + normalA * pmp::dot(normalA, p) * (1 - cos(theta));
    }


    static ExponentialCoordinate xyz2ExpCoor(const pmp::Point &polarAxis, const pmp::Point &v) {
        pmp::Scalar r = pmp::norm(v);
        pmp::Scalar theta = angle(polarAxis, v);
        return {Vector2f(theta, r)};
    }

    static ExponentialCoordinate xy2ExpCoor(pmp::Scalar x, pmp::Scalar y) {
        return {Eigen::Vector2f(atan2(y, x), sqrt(x * x + y * y))};
    }

    static pair<pmp::Scalar, pmp::Scalar> inverseExpMap(const ExponentialCoordinate &coordinate, const pmp::Point &polarAxis) {
        pmp::Scalar x = coordinate.r() * cos(coordinate.theta());
        pmp::Scalar y = coordinate.r() * sin(coordinate.theta());
        return make_pair(x, y);
    }

    static void exp2xyz(vector<pmp::Point> &points, const pmp::Point &polarAxis, const vector<ExponentialCoordinate> &expCoordinates) {
        points.clear();
        for (const auto &coordinate : expCoordinates) {
            auto xy = inverseExpMap(coordinate, polarAxis);
            points.emplace_back(xy.first, xy.second, 0);
        }
    }

    const vector<pmp::Point> &getPolarAxises() const {
        return polarAxises;
    }

private:
    void init() {
        d.resize(region.points.size());
        visit.resize(region.points.size());
        fill(d.begin(), d.end(), INF);
        d[0] = 0;
        expCoordinates.resize(region.points.size());
        expCoordinates[0] = {Eigen::Vector2f(0, 0)};
        for (int i = 0; i < region.points.size(); i++) pre.push_back(i);
        findPolarAxises();
        calculated = false;
        mapped = false;
    }

    /**
     * 以每个点为极点的极坐标系以其第一个邻居的位置作为极轴
     */
    void findPolarAxises() {
        polarAxises.resize(region.vertices.size());
        size_t i = 0;
        for (auto v : region.vertices) {
            for (auto neiV : mesh.vertices(v)) {
                pmp::Normal n = PmpUtils::getVertexNormal(mesh, v);
                pmp::Point po = mesh.position(v);
                pmp::Point pv = mesh.position(neiV);
                polarAxises[i++] = PmpUtils::projectToPlane(n, pv - po);
                break;
            }
        }
    }

    void dijkstra() {
        for (size_t i = 0; i < d.size(); i++) {
            int u = findMinD();
            if (u == -1) return;
            visit[u] = true;
            optimize(u);
        }
    }

    void dfs(int s, int d) {
        if (s == d) {
            cout << s << " ";
            return;
        }
        dfs(s, pre[d]);
        cout << d << " ";
    }

    int findMinD() {
        int u = -1;
        pmp::Scalar MIN = INF;
        for (size_t j = 0; j < d.size(); j++) {
            if (!visit[j] && d[j] < MIN) {
                u = j;
                MIN = d[j];
            }
        }
        return u;
    }

    void optimize(int u) {
        for (size_t v = 0; v < d.size(); v++) {
            if (!visit[v] && existEdge(u, v) && d[u] + distance(u, v) < d[v]) {
                updateDistance(u, v);
                updateExpCoordinate(u, v);
                pre[v] = u;
            }
        }
    }

    bool existEdge(size_t u, size_t v) {
        return mesh.is_valid(mesh.find_edge(region.vertices[u], region.vertices[v]));
    }

    pmp::Scalar distance(size_t u, size_t v) {
        return pmp::distance(region.points[u], region.points[v]);
    }

    void updateDistance(int u, int v) {
        d[v] = d[u] + distance(u, v);
    }

    void updateExpCoordinate(int u, int v) {
        //两个法向量的夹角
        pmp::Scalar theta = getAngle(0, u);
        //logDebug() << "theta_n =" << theta;
        pmp::Point p = polarAxises[u];
        pmp::Normal n1 = PmpUtils::getVertexNormal(mesh, region.vertices[0]);
        pmp::Normal n2 = PmpUtils::getVertexNormal(mesh, region.vertices[u]);
        pmp::Point A = pmp::cross(n2, n1);
        pmp::Point rotatedP = rotate3D(p, A, theta);
        pmp::Scalar theta_pr = angle(pmp::normalize(polarAxises[0]), pmp::normalize(rotatedP));
        pmp::Scalar rotatePDotN1 = pmp::dot(rotatedP, n1);
        //logDebug() << "dot :" << v << "value =" << rotatePDotN1 << "theta_n =" << theta;
        //logDebug() << "theta_pr" << theta_pr;
        expCoordinates[v] = polarVectorPlus(expCoordinates[u], rotate2D(theta_pr, tmpExpCoor(u, v)));
    }

    ExponentialCoordinate polarVectorPlus(const ExponentialCoordinate &c1, const ExponentialCoordinate &c2) {
        auto xyz1 = inverseExpMap(c1, polarAxises[0]);
        auto xyz2 = inverseExpMap(c2, polarAxises[0]);
        pair<pmp::Scalar, pmp::Scalar> xyz3 = make_pair(xyz1.first + xyz2.first, xyz1.second + xyz2.second);
        return xy2ExpCoor(xyz3.first, xyz3.second);
    }

    static ExponentialCoordinate rotate2D(pmp::Scalar angle, const ExponentialCoordinate &coordinate) {
        return {Eigen::Vector2f(coordinate.theta() + angle, coordinate.r())};
    }

    pmp::Scalar getAngle(size_t u, size_t v) {
        pmp::Normal n1 = PmpUtils::getVertexNormal(mesh, region.vertices[u]);
        pmp::Normal n2 = PmpUtils::getVertexNormal(mesh, region.vertices[v]);
        return pmp::angle(n1, n2);
    }

    /**
     * 求以u点为极点时v点的指数坐标
     * @param u
     * @param v
     * @return
     */
    ExponentialCoordinate tmpExpCoor(size_t u, size_t v) {
        pmp::Point Puv = region.points[v] - region.points[u];
        pmp::Normal nu = PmpUtils::getVertexNormal(mesh, region.vertices[u]);
        pmp::Point vOnPlane = PmpUtils::projectToPlane(nu, Puv);
        return xyz2ExpCoor(polarAxises[u], vOnPlane);
    }


    void buildFaceIdMap(const SurfaceMesh &mesh) {
        for (auto f : mesh.faces()) {
            faceIdMap[f.idx()] = region.faceIds[f.idx()];
        }
        mapped = true;
    }

    static bool angleLargeThanPi(const pmp::Point &point1, const pmp::Point &point2) {
        float temp = point1[0] * point2[1] - point2[0] * point1[1];
        return (temp < 0);
    }
};


#endif //MODELGENERATORBASEDONSKETCH_EXPONENTIALMAP_H
