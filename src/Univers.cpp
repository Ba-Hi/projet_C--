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

std::vector<Particule>& Univers::getParticules() {
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
    n_particules = particuleList.size();
    return *this;
}

void Univers::ajouterParticule(const Particule& p) {
    particuleList.push_back(p);
    n_particules = particuleList.size();
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
        mettreAJourCellules();
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

