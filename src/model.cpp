#include "model.h"
#include <fstream>
#include <string>
#include <sstream>

// parses wevafront fileformat
Model::Model(const char *filename)
{

  std::ifstream in;

  in.open(filename, std::ios::in);

  // check if open was succesful
  if (in.fail())
  {
    std::cerr << "Can't open file " << filename << "\n";
    in.close();
    return;
  }

  std::string line;
  while (!in.eof())
  {
    std::getline(in, line);
    std::istringstream iss(line.c_str());
    char trash;

    // line contains vertex information
    if (!line.compare(0, 2, "v "))
    {
      iss >> trash;
      Vec3f v;
      iss >> v.x >> v.y >> v.z;
      verts_.push_back(v);
    }

    else if (!line.compare(0, 2, "f "))
    {
      std::vector<int> f;
      int itrash, idx;
      std::vector<int> v;

      iss >> trash;
      while (iss >> idx >> trash >> itrash >> trash >> itrash)
      {
        // indexes in wavefront ff start from 1
        f.push_back(idx - 1);
      }
      faces_.push_back(f);
    }
  }
  std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

int Model::nverts(){
  return (int)verts_.size();
}

int Model::nfaces(){
  return (int)faces_.size();
}

Vec3f Model::vert(int i){
  if(i < 0 || i >= nverts()) throw std::out_of_range("Index out of bounds");
  return verts_[i];
}

std::vector<int> Model::face(int idx){
  if(idx < 0 || idx >= nfaces()) throw std::out_of_range("Index out of bounds");
  return faces_[idx];
}



