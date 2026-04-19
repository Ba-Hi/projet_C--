#ifndef UNIVERS_TP4_HPP
#define UNIVERS_TP4_HPP

#include <vector>
#include "Cellule.hpp"
#include "Vector.hpp"



class Univers_tp4 {
private:
    int dimension;
    Vector l_d;
    double r_cut;
    Vector n_cd;
    std::vector<Particule> particuleList;
    std::vector<Cellule> celluleList;

public:
    Univers_tp4(const int& dimension, const Vector& l_d, const double& r_cut);
    void initialiserCellules();
    std::vector<Vector> calculerForces();
    void calculerForces(double epsilon, double sigma);
    void mettreAJourCellules(); 
    void ajouterParticule(const Particule& p);
    void avancerParticules(double tEnd, double dt);
private:
    int indice1D(int i, int j, int k);

};

#endif