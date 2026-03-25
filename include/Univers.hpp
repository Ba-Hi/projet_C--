#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include <vector>
#include "Particule.hpp"
#include "Vector.hpp"

class Univers {
private:
    int dimension;
    int n_particules;
    std::vector<Particule> particuleList;

public:
    Univers(int dim, int reserveCount = 0);

    // getters
    int getDimension() const;
    int getNombreParticules() const;
    const std::vector<Particule>& getParticules() const;

    // setters
    void setDimension(int dim);
    void setNombreParticules(int n);
    void setParticules(const std::vector<Particule>& particules);

    void ajouterParticule(const Particule& p);
    void modifierVitesseUniforme(const Vector& v);

    void afficherUnivers() const;

    void avancerParticules(double tEnd, double dt);

    std::vector<Vector> calculerForces() const;
};

#endif