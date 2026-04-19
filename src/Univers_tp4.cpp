#include "Univers_tp4.hpp"
#include <iostream>
#include <cmath>
#include "ExportCsv.hpp"



Univers_tp4::Univers_tp4(const int& dimension, const Vector& l_d, const double& r_cut)
            : 
            dimension(dimension),
            l_d(l_d),
            r_cut(r_cut),
            epsilon(1.0),
            sigma(1.0),
            nx((int)(l_d.x() / r_cut)),
            ny((int)(l_d.y() / r_cut)),
            nz(dimension == 3 ? (int)(l_d.z() / r_cut) : 1),
            particuleList(),
            celluleList(){
    if (dimension > 3 || dimension < 1) {
        throw std::invalid_argument("La dimension est soit 1D, 2D, ou 3D.");
    }
}

Univers_tp4::Univers_tp4(const int& dimension, const Vector& l_d, const double& r_cut, double epsilon, double sigma)
            : 
            dimension(dimension),
            l_d(l_d),
            r_cut(r_cut),
            epsilon(epsilon),
            sigma(sigma),
            nx((int)(l_d.x() / r_cut)),
            ny((int)(l_d.y() / r_cut)),
            nz(dimension == 3 ? (int)(l_d.z() / r_cut) : 1),
            particuleList(),
            celluleList(){
    if (dimension > 3 || dimension < 1) {
        throw std::invalid_argument("La dimension est soit 1D, 2D, ou 3D.");
    }
}

void Univers_tp4::initialiserCellules() {
    celluleList.clear();

    int nbCellules = 0;
    if (dimension == 1) {
        nbCellules = nx;
    }
    else if (dimension == 2) {
        nbCellules = nx * ny;
    }
    else {
        nbCellules = nx * ny * nz;
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
    return i + nx * (j + ny * k);

}

Cellule& Univers_tp4::getCellule(int i, int j, int k) {
    return celluleList[indice1D(i, j, k)];
}

size_t Univers_tp4::getNbCellules() const {
    return celluleList.size();
}

std::vector<Vector> Univers_tp4::calculerForces() {
    calculerForces(epsilon, sigma);
    std::vector<Vector> forces;
    forces.reserve(particuleList.size());
    for (const auto& p : particuleList) {
        forces.push_back(p.getForce());
    }
    return forces;
}

void Univers_tp4::calculerForces(double epsilon, double sigma) {
    for (Particule& p : particuleList)
        p.setForce(Vector(0.0, 0.0, 0.0));

    for (int i = 0; i < nx; i++)
    for (int j = 0; j < ny; j++)
    for (int k = 0; k < nz; k++) {
        Cellule& ci = celluleList[indice1D(i,j,k)];

        // On ne parcourt que les voisines "à droite" pour éviter double calcul
        for (Cellule* cj : ci.getVoisines()) {
            // Éviter de compter deux fois la même paire
            if (cj < &ci) continue;

            for (Particule* pi_ptr : ci.getParticuleList()) {
                Particule& pi = *pi_ptr;
                double distance_pi_centre_cj = (pi.getPosition() - cj->centreCellule()).norm();
                if (distance_pi_centre_cj > r_cut)
                    continue; // on ignore cette cellule voisine, elle est trop loin
            for (Particule* pj_ptr : cj->getParticuleList()) {
                Particule& pj = *pj_ptr;
                if (&pi == &pj) continue;

                Vector rij = pi.getPosition() - pj.getPosition();
                double dist = rij.norm();
                if (dist == 0.0) continue;

                double s_r6  = pow(sigma / dist, 6);
                double coeff = (24.0 * epsilon / (dist * dist))
                               * s_r6 * (1.0 - 2.0 * s_r6);
                Vector fij   = rij * (-coeff);  // F sur i due à j

                pi.setForce(pi.getForce() + fij);
                pj.setForce(pj.getForce() - fij);
            }
            }
        }
    }
    mettreAJourCellules();
}
    

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

        i = std::max(0, std::min(i, nx - 1));
        j = std::max(0, std::min(j, ny - 1));
        k = std::max(0, std::min(k, nz - 1));

        celluleList[indice1D(i, j, k)].ajouterParticule(&p);
    }
}

void Univers_tp4::avancerParticules(double tEnd, double dt) {
    if (particuleList.empty()) return;

    std::ofstream csvFile("output_tp4.csv");
    csvFile << "t";
    for (size_t i = 0; i < particuleList.size(); ++i)
        csvFile << " x" << i << " y" << i;
    csvFile << "\n";

    // Initialisation : calcul des forces au t=0
    calculerForces(epsilon, sigma);
    std::vector<Vector> forces(particuleList.size(), Vector(0, 0, 0));
    for (size_t i = 0; i < particuleList.size(); i++)
        forces[i] = particuleList[i].getForce();

    double t = 0.0;
    while (t < tEnd) {
        t += dt;
        std::vector<Vector> forces_old = forces;

        // Mise à jour des positions (Störmer-Verlet)
        for (size_t i = 0; i < particuleList.size(); ++i) {
            Particule& p = particuleList[i];
            Vector accel  = forces[i] * (1.0 / p.getMasse());
            Vector newPos = p.getPosition() + p.getVitesse()*dt + accel*(0.5*dt*dt);
            p.setPosition(newPos);
        }

        // Réassigner les particules aux cellules
        mettreAJourCellules();

        // Nouveaux forces
        calculerForces(epsilon, sigma);
        for (size_t i = 0; i < particuleList.size(); i++)
            forces[i] = particuleList[i].getForce();

        // Mise à jour des vitesses
        for (size_t i = 0; i < particuleList.size(); ++i) {
            Particule& p = particuleList[i];
            Vector newVel = p.getVitesse()
                          + (forces[i] + forces_old[i]) * (0.5*dt / p.getMasse());
            p.setVitesse(newVel);
        }
        saveCSV(particuleList, csvFile, t);
    }
}