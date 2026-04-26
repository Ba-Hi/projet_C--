#include "Univers.hpp"
#include <iostream>
#include <filesystem>
#include "ExportVTK.hpp"

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

    std::vector<Vector> forces = calculerForces();

    double t = 0.0;
    int step = 0;
    const int save_every = 10;

    std::vector<int>    vtk_steps;
    std::vector<double> vtk_times;

    while (t < tEnd) {
        t += dt;
        step++;
        std::vector<Vector> forces_old = forces;

        for (size_t i = 0; i < particuleList.size(); ++i) {
            Particule& p = particuleList[i];
            Vector accel = forces[i] * (1.0 / p.getMasse());
            p.setPosition(p.getPosition() + p.getVitesse()*dt + accel*(0.5*dt*dt));
        }

        mettreAJourCellules();
        forces = calculerForces();

        for (size_t i = 0; i < particuleList.size(); ++i) {
            Particule& p = particuleList[i];
            p.setVitesse(p.getVitesse()
                + (forces[i] + forces_old[i]) * (0.5*dt / p.getMasse()));
        }

        if (step % save_every == 0) {
            saveVTK(particuleList, step, t);
            vtk_steps.push_back(step);
            vtk_times.push_back(t);

            // double pct = 100.0 * t / tEnd;
            // std::cout << "t = " << t << " / " << tEnd
            //           << "  (" << pct << "%)\n" << std::flush;

            double fmax = 0.0, vmax = 0.0;
            for (const auto& p : particuleList) {
                fmax = std::max(fmax, p.getForce().norm());
                vmax = std::max(vmax, p.getVitesse().norm());
            }
            // std::cout << "  fmax=" << fmax << "  vmax=" << vmax << "\n";

            if (fmax > 1e6) {
                std::cerr << "EXPLOSION à t=" << t << "\n";
                savePVD(vtk_steps, vtk_times);
                return;
            }
        }
    }

    savePVD(vtk_steps, vtk_times); 
    std::cout << "Simulation terminée. Résultats dans output_XXXX.vtp\n";
}