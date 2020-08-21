#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        //std::cout << line << std::endl;
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            verts_.push_back(v);
        } 
        else if (!line.compare(0, 2, "f ")) {
            std::vector<std::vector<int>> f;
            int itrash, idx, vertTextureIdx;
            iss >> trash;
            while (iss >> idx >> trash >> vertTextureIdx >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                vertTextureIdx--;
                f.push_back(std::vector<int>{idx, vertTextureIdx});
            }
            //std::cout << f[0][0] << " " << f[0][1] << " " << f[1][0] << " " << f[1][1] << " " << f[2][0] << " " << f[2][1] << std::endl;
            faces_.push_back(f);
        }
        else if (!line.compare(0, 3, "vt ")) {
            //std::cout << line << std::endl;
            float u, v;
            iss >> trash >> trash;
            iss >> u >> v;
            //std::cout << u << v << std::endl;
            Vec2f textureC(u, v);
            //std::cout << "u:" << u << " " << "v:" << v << std::endl;
            textures_.push_back(textureC);
        }
    }
    std::cerr << "# v# " << verts_.size() << " vt# " << textures_.size() << " f# "  << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<std::vector<int>> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec2f Model::text(int i) {
    return textures_[i];
}
