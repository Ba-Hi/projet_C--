#include "Univers.hpp"
#include <iostream>
#include <filesystem>
#include "ExportCsv.hpp"

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

Univers& Univers::setDimension(int dim) {
    dimension = dim;
    return *this;
}

Univers& Univers::setNombreParticules(int n) {
    n_particules = n;
    return *this;
}

Univers& Univers::setParticules(const std::vector<Particule>& particules) {
    particuleList = particules;
    return *this;
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

 std::ofstream csvFile("output.csv");
 // header: t x0 y0 x1 y1 ...
 csvFile << "t";
 for (size_t i = 0; i < particuleList.size(); ++i)
 csvFile << " x" << i << " y" << i;
 csvFile << "\n";

 std::vector<Vector> forces = calculerForces();
 double t = 0.0;
 while (t < tEnd) {
 t += dt;
 std::vector<Vector> forces_old = forces;
 for (size_t i = 0; i < particuleList.size(); ++i) {
 Particule& p = particuleList[i];
 Vector accel = forces[i] * (1.0 / p.getMasse());
 Vector newPos = p.getPosition() + p.getVitesse() * dt
 + accel * (0.5 * dt * dt);
 p.setPosition(newPos);
 }
 forces = calculerForces();
 for (size_t i = 0; i < particuleList.size(); ++i) {
 Particule& p = particuleList[i];
 Vector newVel = p.getVitesse()
 + (forces[i] + forces_old[i]) * (0.5 * dt / p.getMasse());
 p.setVitesse(newVel);
 }
 saveCSV(particuleList, csvFile, t);
 }
}

