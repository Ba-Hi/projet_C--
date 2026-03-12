#ifndef CELLULE_HPP
#define CELLULE_HPP


#include "Particule.hpp"
#include <vector>

class Cellule {
public :
    std::vector<Particule> particuleList;

public :
    Cellule(const std::vector<Particule> particuleListInitiale);
};


#endif