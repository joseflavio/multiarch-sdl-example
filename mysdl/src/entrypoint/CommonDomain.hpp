#ifndef COMMONDOMAIN_HPP
#define COMMONDOMAIN_HPP

#include <string>

typedef float Dimension;

class Vertex {
public:
    typedef std::vector<Vertex> VertexVector_t;
    Dimension x;
    Dimension y;
    Dimension z;
    const std::string toString() const {
        std::stringstream ss;
        ss << std::fixed << "( " << x << ", " << y << ", " <<z << " )";
        return ss.str();
    }
};

class Face3 {
public:
    typedef std::vector<Face3> Face3Vector_t;
    size_t vi1;
    size_t vi2;
    size_t vi3;
};

class Model3 {

public:
    Vertex::VertexVector_t vertexes;
    Face3::Face3Vector_t faces;
    Vertex::VertexVector_t facesCenters;
    Vertex modelMin;
    Vertex modelMax;

    const Vertex& getFace3Vertex1(const Face3& iFace) const {
        return vertexes.at(iFace.vi1);
    }

    const Vertex& getFace3Vertex2(const Face3& iFace) const {
        return vertexes.at(iFace.vi2);
    }

    const Vertex& getFace3Vertex3(const Face3& iFace) const {
        return vertexes.at(iFace.vi3);
    }

};// class Model3

class VertexUtil {
public:
    static Vertex calculateCenter(const Vertex& iV1, const Vertex& iV2, const Vertex& iV3) {
        Dimension x = (iV1.x + iV2.x + iV3.x) / 3;
        Dimension y = (iV1.y + iV2.y + iV3.y) / 3;
        Dimension z = (iV1.z + iV2.z + iV3.z) / 3;
        Vertex center = { x, y, z };
        return center;
    }
};

class FacesUtil {
public:
    static void calculatePowDistances(std::vector<Dimension>& oPowDistances,
                                      Dimension& oMin,
                                      Dimension& oMax,
                                      const Vertex& iVertex,
                                      const Vertex::VertexVector_t& iVector) {
        bool aFirstRun = true;
        Vertex::VertexVector_t::const_iterator cIt = iVector.begin();
        for(; cIt!=iVector.end(); ++cIt) {
            Dimension xd = (*cIt).x - iVertex.x;
            Dimension yd = (*cIt).y - iVertex.y;
            Dimension zd = (*cIt).z - iVertex.z;
            Dimension currentPdist = (xd*xd) + (yd*yd) + (zd*zd);
            if(aFirstRun) {
                aFirstRun = false;
                oMin = currentPdist;
                oMax = currentPdist;
            }
            else {
                if( currentPdist < oMin ) {
                    oMin = currentPdist;
                }
                if( currentPdist > oMax ) {
                    oMax = currentPdist;
                }
            }
            oPowDistances.push_back(currentPdist);
        }
    }
};

class ContainerUtil {
public:
    template<typename T>
    static std::string toString(const std::deque<T>& iVector) {
        typename std::vector<T>::const_iterator it = iVector.begin();
        std::stringstream ss;
        for(;it!=iVector.end();++it) {
            ss << (*it);
        }
        return ss.str();
    }
};

class TypeUtil {
public:
    template<typename T>
    static bool parseInt(T& oInt, std::string iString) {
        std::stringstream ss(iString);
        ss >> oInt;
        if(!ss) {
            return false;
        }
        return true;
    }
};

#endif // COMMONDOMAIN_HPP
