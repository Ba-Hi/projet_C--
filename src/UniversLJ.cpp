#include "UniversLJ.hpp"
#include <iostream>
#include <cmath>
#include "ExportCsv.hpp"



UniversLJ::UniversLJ(const int& dimension, const Vector& l_d, const double& r_cut)
            : 
            Univers(dimension),
            l_d(l_d),
            r_cut(r_cut),
            epsilon(1.0),
            sigma(1.0),
            nx((int)(l_d.x() / r_cut)),
            ny((int)(l_d.y() / r_cut)),
            nz(dimension == 3 ? (int)(l_d.z() / r_cut) : 1),
            celluleList(){
    if (dimension > 3 || dimension < 1) {
        throw std::invalid_argument("La dimension est soit 1D, 2D, ou 3D.");
    }
}

UniversLJ::UniversLJ(const int& dimension, const Vector& l_d, const double& r_cut, double epsilon, double sigma)
            : 
            Univers(dimension),
            l_d(l_d),
            r_cut(r_cut),
            epsilon(epsilon),
            sigma(sigma),
            nx((int)(l_d.x() / r_cut)),
            ny((int)(l_d.y() / r_cut)),
            nz(dimension == 3 ? (int)(l_d.z() / r_cut) : 1),
            celluleList(){
    if (dimension > 3 || dimension < 1) {
        throw std::invalid_argument("La dimension est soit 1D, 2D, ou 3D.");
    }
}

void UniversLJ::initialiserCellules() {
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

int UniversLJ::indice1D(int i, int j, int k){
    return i + nx * (j + ny * k);

}

Cellule& UniversLJ::getCellule(int i, int j, int k) {
    return celluleList[indice1D(i, j, k)];
}

size_t UniversLJ::getNbCellules() const {
    return celluleList.size();
}

std::vector<Vector> UniversLJ::calculerForces() {
    size_t n = particuleList.size();
    std::vector<Vector> forces(n, Vector(0.0, 0.0, 0.0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i+1; j < n; ++j) {
            const Vector& pi = particuleList[i].getPosition();
            const Vector& pj = particuleList[j].getPosition();
            Vector diff = pj - pi;
            double dist = diff.norm();
            if (dist == 0.0 || dist > r_cut) continue;

            double r2inv = 1.0 / (dist * dist);
            double s_r6 = r2inv * r2inv * r2inv * (sigma * sigma * sigma * sigma * sigma * sigma);
            double coeff = (24.0 * epsilon / (dist * dist))
                         * s_r6 * (1.0 - 2.0 * s_r6);
            Vector forceIJ = diff * (-coeff);
            forces[i] = forces[i] + forceIJ;
            forces[j] = forces[j] - forceIJ;
        }
    }
    return forces;
}


void UniversLJ::calculerForces(double epsilon, double sigma) {
    for (Particule& p : particuleList)
        p.setForce(Vector(0.0, 0.0, 0.0));

    for (int i = 0; i < nx; i++)
    for (int j = 0; j < ny; j++)
    for (int k = 0; k < nz; k++) {

        Cellule& ci = celluleList[indice1D(i, j, k)];

        // Interactions intra-cellule
        const auto& parts = ci.getParticuleList();
        for (size_t a = 0; a < parts.size(); ++a) {
            for (size_t b = a + 1; b < parts.size(); ++b) {
                Particule& pi = *parts[a];
                Particule& pj = *parts[b];

                Vector rij  = pi.getPosition() - pj.getPosition();
                double dist = rij.norm();
                if (dist == 0.0 || dist > r_cut) continue;

                double r2inv = 1.0 / (dist * dist);
                double s_r6 = r2inv * r2inv * r2inv * (sigma * sigma * sigma * sigma * sigma * sigma);
                double coeff = (24.0 * epsilon / (dist * dist))
                             * s_r6 * (1.0 - 2.0 * s_r6);
                Vector fij   = rij * (-coeff);

                pi.setForce(pi.getForce() + fij);
                pj.setForce(pj.getForce() - fij);

            }
        }


        // Interactions avec les cellules voisines
        for (Cellule* cj : ci.getVoisines()) {
            if (cj == &ci) continue;
            if (cj < &ci) continue; // évite double comptage

            for (Particule* pi_ptr : ci.getParticuleList()) {
                Particule& pi = *pi_ptr;

                // Distance entre la particule pi et le centre de la cellule voisine cj
                double d = (pi.getPosition() - cj->centreCellule()).norm();
                if (d > r_cut * std::sqrt(2.0) ) continue; // cj trop loin pour pi → on ignore

                for (Particule* pj_ptr : cj->getParticuleList()) {
                    Particule& pj = *pj_ptr;
                    if (&pi == &pj) continue;

                    Vector rij = pi.getPosition() - pj.getPosition();
                    double dist = rij.norm();
                    if (dist == 0.0 || dist > r_cut) continue;
                    double s_r6  = pow(sigma / dist, 6);
                    double coeff = (24.0 * epsilon / (dist * dist))
                                 * s_r6 * (1.0 - 2.0 * s_r6);
                    Vector fij   = rij * (-coeff);

                    pi.setForce(pi.getForce() + fij);
                    pj.setForce(pj.getForce() - fij);
                }
            }
        }
    }
}


void UniversLJ::mettreAJourCellules() {
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

void UniversLJ::avancerParticules(double tEnd, double dt) {
    if (particuleList.empty()) return;

    std::ofstream csvFile("output_tp4.csv");
    csvFile << "t";
    for (size_t i = 0; i < particuleList.size(); ++i)
        csvFile << " x" << i << " y" << i;
    csvFile << "\n";

    // Forces initiales à t=0
    calculerForces();

    std::vector<Vector> forces(particuleList.size(), Vector(0, 0, 0));
    for (size_t i = 0; i < particuleList.size(); i++)
        forces[i] = particuleList[i].getForce();

    double t = 0.0;
    int step = 0;
    const int save_every = 200;

    while (t < tEnd) {
        t += dt;
        step++;
        std::vector<Vector> forces_old = forces;

        for (size_t i = 0; i < particuleList.size(); ++i) {
            Particule& p = particuleList[i];
            Vector accel  = forces[i] * (1.0 / p.getMasse());
            Vector newPos = p.getPosition() + p.getVitesse()*dt + accel*(0.5*dt*dt);
            p.setPosition(newPos);
        }

        mettreAJourCellules();

        calculerForces();
        for (size_t i = 0; i < particuleList.size(); i++)
            forces[i] = particuleList[i].getForce();

        // Mise à jour des vitesses
        for (size_t i = 0; i < particuleList.size(); ++i) {
            Particule& p = particuleList[i];
            Vector newVel = p.getVitesse()
                          + (forces[i] + forces_old[i]) * (0.5*dt / p.getMasse());
            p.setVitesse(newVel);
        }

        if (step % save_every == 0) {
            saveCSV(particuleList, csvFile, t);
            std::cout << "t = " << t << " / " << tEnd
                      << "  (" << (100.0*t/tEnd) << "%)\n" << std::flush;

            double f_max = 0.0;
            double pos_max = 0.0;
            for (const auto& p : particuleList) {
                double f_norm = p.getForce().norm();
                if (f_norm > f_max) f_max = f_norm;
                double pos_norm = p.getPosition().norm();
                if (pos_norm > pos_max) pos_max = pos_norm;
            }
            std::cout << "t = " << t << "  fmax=" << f_max
                      << "  posmax=" << pos_max << "\n" << std::flush;

        }
    }
}