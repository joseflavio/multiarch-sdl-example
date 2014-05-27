#ifndef MODELLOADER_HPP
#define MODELLOADER_HPP

#include <vector>
#include <deque>

#include <sstream>
#include <iostream>

#include <entrypoint/CommonDomain.hpp>

class ModelLoader {

public:
    static bool loadFile(
            Model3& oModel, const std::string& iFilename, const Dimension& iFactor) {

        std::ifstream infile(iFilename.c_str());
        if (!infile.is_open()) {
            return false;
        }

        const std::string kVERTEX = "v";
        const std::string kFACE = "f";
        const std::string kNORMAL = "vn";

        bool aFirstRun = true;
        Vertex aModelMin, aModelMax;

        std::string line;
        int i = 0;
        while ( getline (infile, line) ) {
            std::string aType;
            std::stringstream aLineStream(line);
            aLineStream >> aType;

            if( aType == kVERTEX ) {
                Dimension aVx, aVy, aVz;
                aLineStream >> aVx >> aVy >> aVz;
                Vertex v = { (aVx*iFactor), (aVy*iFactor), (aVz*iFactor) };
                oModel.vertexes.push_back(v);

                if(aFirstRun) {
                    aFirstRun = false;
                    aModelMin = v;
                    aModelMax = v;
                }
                else {
                    if(v.x < aModelMin.x) {
                        aModelMin.x = v.x;
                    }
                    if(v.y < aModelMin.y) {
                        aModelMin.y = v.y;
                    }
                    if(v.z < aModelMin.z) {
                        aModelMin.z = v.z;
                    }
                    if(v.x > aModelMax.x) {
                        aModelMax.x = v.x;
                    }
                    if(v.y > aModelMax.y) {
                        aModelMax.y = v.y;
                    }
                    if(v.z > aModelMax.z) {
                        aModelMax.z = v.z;
                    }
                }
            }
            else if( aType == kFACE ) {

                std::string f1;
                std::string f2;
                std::string f3;
                aLineStream >> f1;
                aLineStream >> f2;
                aLineStream >> f3;

                std::stringstream ssf1(f1);
                std::stringstream ssf2(f2);
                std::stringstream ssf3(f3);

                std::string fv1str;
                std::getline(ssf1, fv1str, '/');
                if(!aLineStream) {
                    std::cerr << "Problem reading. line="<<line<<"\n";
                    return false;
                }

                std::string fv2str;
                std::getline(ssf2, fv2str, '/');
                if(!aLineStream) {
                    std::cerr << "Problem reading. line="<<line<<"\n";
                    return false;
                }

                std::string fv3str;
                std::getline(ssf3, fv3str, '/');
                if(!aLineStream) {
                    std::cerr << "Problem reading. line="<<line<<"\n";
                    return false;
                }

                size_t fvi1, fvi2, fvi3;
                if(!TypeUtil::parseInt(fvi1, fv1str)) {
                    std::cerr << "Problem parsing. f1="<<f1<<", line="<<line<<"\n";
                    return false;
                }
                if(!TypeUtil::parseInt(fvi2, fv2str)) {
                    std::cerr << "Problem parsing. f2="<<f1<<", line="<<line<<"\n";
                    return false;
                }
                if(!TypeUtil::parseInt(fvi3, fv3str)) {
                    std::cerr << "Problem parsing. f3="<<f1<<", line="<<line<<"\n";
                    return false;
                }
                Face3 b = { fvi1-1, fvi2-1, fvi3-1 };
                oModel.faces.push_back(b);

            }
            else if( aType == kNORMAL ) {

            }
            else {
                std::cout << i << " - " << aType << "\n";
            }
            ++i;
        }
        infile.close();

        oModel.modelMin = aModelMin;
        oModel.modelMax = aModelMax;

        Face3::Face3Vector_t::const_iterator cIt = oModel.faces.begin();
        for(; cIt !=oModel.faces.end(); ++cIt) {
            const Face3& aFace = (*cIt);
            const Vertex& v1 = oModel.getFace3Vertex1(aFace);
            const Vertex& v2 = oModel.getFace3Vertex2(aFace);
            const Vertex& v3 = oModel.getFace3Vertex3(aFace);
            oModel.facesCenters.push_back(VertexUtil::calculateCenter(v1, v2, v3));
        }

        return true;
    }

};

#endif // MODELLOADER_HPP
