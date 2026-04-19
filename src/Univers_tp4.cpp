#include "Univers_tp4.hpp"
#include <iostream>
#include <cmath>
#include "ExportCsv.hpp"



Univers_tp4::Univers_tp4(const int& dimension, const Vector& l_d, const double& r_cut)
            : 
            dimension(dimension),
            l_d(l_d),
            r_cut(r_cut),
            n_cd((1 / r_cut) * l_d),
            particuleList(particuleList),
            celluleList(){
    if (dimension > 3 || dimension < 1) {
        throw std::invalid_argument("La dimension est soit 1D, 2D, ou 3D.");
    }
}

void Univers_tp4::initialiserCellules() {
    celluleList.clear();

    int nx = (int)n_cd.x();
    int ny = (int)n_cd.y();
    int nz = (int)n_cd.z();

    int nbCellules = 0;
    if (dimension == 1) {
        nbCellules = n_cd.x();
    }
    else if (dimension == 2) {
        nbCellules = n_cd.x() * n_cd.y();
    }
    else {
        nbCellules = n_cd.x() * n_cd.y() * n_cd.z();
    }

    celluleList.reserve(nbCellules);

    // Création des cellules :
    for (int i=0; i<nx; i++){
        for (int j=0; j<ny; j++){
            for (int k=0; k<nz; k++){
                celluleList.emplace_back(r_cut, i, j, k);
            }
        }
    }

    // Création des voisins :
    for (int i=0; i<nx; i++){
        for (int j=0; j<ny; j++){
            for (int k=0; k<nz; k++){
                Cellule& c = celluleList[indice1D(i, j, k)];
                for (int dk = -1; dk <= 1; dk++)
                    for (int dj = -1; dj <= 1; dj++)
                        for (int di = -1; di <= 1; di++) {
                            int ni = i + di, nj = j + dj, nk = k + dk;
                            if (ni >= 0 && ni < nx &&
                                nj >= 0 && nj < ny &&
                                nk >= 0 && nk < nz) c.ajouterVoisine(&celluleList[indice1D(ni, nj, nk)]);
               }
            }
        }
    }

    mettreAJourCellules();
}

int Univers_tp4::indice1D(int i, int j, int k){
    return i + n_cd.x() * (j + n_cd.y() * k);

}



std::vector<Vector> Univers_tp4::calculerForces(double epsilon, double sigma) {
    std::vector<Vector> forces(particuleList.size());
    
    for (Cellule& cellCourante : celluleList) {
        for (Particule& p : cellCourante.getParticuleList()) {
            Vector forceTotale(0.0, 0.0, 0.0);
            for (Cellule* celluleVoisine : cellCourante.getVoisines()) {
                if ((p.getPosition() - celluleVoisine->getPositionCellule()).norm() > r_cut) {
                    continue; // Ignorer les cellules trop éloignées
                }
                for (Particule& p2 : celluleVoisine->getParticuleList()) {
                    if (&p == &p2) continue;

                    double dist = (p.getPosition() - p2.getPosition()).norm();
                    forceTotale += (1/ pow(dist, 2)) * pow(sigma/dist, 6) * (1 - 2 * pow(sigma/dist, 6)) * (p2.getPosition() - p.getPosition());

                }
            }
            p.setForce(24 * epsilon * forceTotale);
        }
}}
    

void Univers_tp4::ajouterParticule(const Particule& p) {
    particuleList.push_back(p);
}

void Univers_tp4::mettreAJourCellules() {
    for (Cellule& c : celluleList)
        c.viderParticules();

    for (Particule& p : particuleList) {
        Vector pos = p.getPosition();
        int i = (int)(pos.x() / r_cut);
        int j = (int)(pos.y() / r_cut);
        int k = (dimension == 3) ? (int)(pos.z() / r_cut) : 0;

        i = std::max(0, std::min(i, (int)n_cd.x() - 1));
        j = std::max(0, std::min(j, (int)n_cd.y() - 1));
        k = std::max(0, std::min(k, (int)n_cd.z() - 1));

        celluleList[indice1D(i, j, k)].ajouterParticule(p);
    }
}

void Univers_tp4::avancerParticules(double tEnd, double dt) {
    if (particuleList.empty()) return;

    std::ofstream csvFile("output_tp4.csv");
    // header: t x0 y0 x1 y1 ...
    csvFile << "t";

    for (size_t i = 0; i < particuleList.size(); ++i)
    csvFile << " x" << i << " y" << i; {}
    csvFile << "\n";

    std::vector<Vector> forces = ;
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