#ifndef PARTICULE_HPP
#define PARTICULE_HPP

#include "Vector.hpp"
#include <vector>

class Particule
{
private:
    // position, vitesse et force passent en Vector 2D (z fixé à 0)
    Vector position;
    Vector vitesse;
    Vector force;
    double masse;
    int identifiant;
    int categorie;
public:
    Particule(const Vector& position, const Vector& vitesse, double masse,
              int identifiant, int categorie, const Vector& force);

    // accesseurs en référence pour faciliter la modification
    Particule& setPosition(const Vector& newPosition);
    const Vector& getPosition() const;

    Particule& setVitesse(const Vector& newVitesse);
    const Vector& getVitesse() const;

    Particule& setMasse(double newMasse);
    double getMasse() const;

    Particule& setForce(const Vector& newForce);
    const Vector& getForce() const;

    Particule& setType(int newType);
    int getType() const;
};

// fonctions utilitaires pour l'algorithme
std::vector<Vector> initialiser(std::vector<Particule>& particleList);
void algo_stromer_verlet(std::vector<Particule>& particleList,
                         std::vector<Vector> forces_fold,
                         double tEnd,
                         double dt);

#endif