#ifndef UNIVERS_TP4_HPP
#define UNIVERS_TP4_HPP

#include <vector>
#include "Cellule.hpp"
#include "Vector.hpp"


class Univers_tp4 {
public:
    int dimension;
    Vector l_d;
    double r_cut;
    Vector n_cd;
    std::vector<Cellule> celluleList;

public:
    Univers_tp4(const int& dimension, const Vector& l_d, const double& r_cut);
    void initialiser();
};

#endif