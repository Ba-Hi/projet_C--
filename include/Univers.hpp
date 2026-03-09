#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include <vector>
#include "Particule.hpp"
#include "Vector.hpp"

class Univers {
public:
    int dimension;
    int n_particules;
    std::vector<Particule> particuleList;

public:
    Univers(int dim, int reserveCount = 0);

    void ajouterParticule(const Particule& p);
    void modifierVitesseUniforme(const Vector& v);

    void afficherUnivers() const;

    void avancerParticules(double tEnd, double dt);

    std::vector<Vector> calculerForces() const;
};

#endif