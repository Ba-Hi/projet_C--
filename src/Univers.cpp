#include "Univers.hpp"
#include <iostream>

Univers::Univers(int dim, int reserveCount)
    : dimension(dim), n_particules(0) {
    particuleList.reserve(reserveCount);
}

int Univers::getDimension() const {
    return dimension;
}

int Univers::getNombreParticules() const {
    return n_particules;
}

const std::vector<Particule>& Univers::getParticules() const {
    return particuleList;
}

void Univers::setDimension(int dim) {
    dimension = dim;
}

void Univers::setNombreParticules(int n) {
    n_particules = n;
}

void Univers::setParticules(const std::vector<Particule>& particules) {
    particuleList = particules;
}

void Univers::ajouterParticule(const Particule& p) {
    particuleList.push_back(p);
    n_particules = particuleList.size();
}

void Univers::modifierVitesseUniforme(const Vector& v) {
    for (auto& p : particuleList) {
        p.setVitesse(v);
    }
}

void Univers::afficherUnivers() const {
    std::cout << "Univers (dim=" << dimension << ", n=" << n_particules << ")\n";
    for (size_t i = 0; i < particuleList.size(); ++i) {
        const Vector& pos = particuleList[i].getPosition();
        std::cout << " particle[" << i << "] = (" << pos.x() << ", " << pos.y();
        if (dimension == 3) std::cout << ", " << pos.z();
        std::cout << ")\n";
    }
}

std::vector<Vector> Univers::calculerForces() const {
    // réutilise la fonction globale pour ne pas dupliquer la logique
    return initialiser(const_cast<std::vector<Particule>&>(particuleList));
}

void Univers::avancerParticules(double tEnd, double dt) {
    if (particuleList.empty()) return;
    std::vector<Vector> forces = calculerForces();
    std::vector<Vector> forces_old = forces;
    double t = 0.0;

    while (t < tEnd) {
        t += dt;
        // positions
        for (size_t i = 0; i < particuleList.size(); ++i) {
            Particule& p = particuleList[i];
            Vector accel = forces[i] * (1.0 / p.getMasse());
            Vector halfStep = p.getVitesse() + accel * (0.5 * dt);
            Vector newPos = p.getPosition() + halfStep * dt;
            p.setPosition(newPos);
            forces_old[i] = forces[i];
        }
        // recalcul des forces
        forces = calculerForces();
        // vitesse
        for (size_t i = 0; i < particuleList.size(); ++i) {
            Particule& p = particuleList[i];
            Vector newVel = p.getVitesse() + (forces[i] + forces_old[i]) * (0.5 / p.getMasse());
            p.setVitesse(newVel);
        }
        // affichage pour suivi
        /*const Vector& pos0 = particuleList[0].getPosition();
        std::cout << "t=" << t << " pos0=(" << pos0.x() << "," << pos0.y() << ")\n";*/
    }
}

