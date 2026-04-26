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
            conditionX(Univers::ConditionLimite::REFLEXION),
            conditionY(Univers::ConditionLimite::REFLEXION),
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
            conditionX(Univers::ConditionLimite::REFLEXION),
            conditionY(Univers::ConditionLimite::REFLEXION),
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
    calculerForces(epsilon, sigma);
    std::vector<Vector> forces;
    forces.reserve(particuleList.size());
    for (const auto& p : particuleList) {
        forces.push_back(p.getForce());
    }
    return forces;
}

void UniversLJ::calculerForces(double epsilon, double sigma) {
    const double r_cut2 = r_cut * r_cut;
    const double sigma2 = sigma * sigma;
    const double sigma6 = sigma2 * sigma2 * sigma2;
    const double min_r2   = 0.05 * sigma2; // évite la singularité pour des contacts très proches

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
                double dist2 = rij.norm2();
                if (dist2 == 0.0 || dist2 > r_cut2) continue;
                if (dist2 < min_r2) dist2 = min_r2;

                double invDist2 = 1.0 / dist2;
                double s_r6 = sigma6 * invDist2 * invDist2 * invDist2;
                double coeff = (24.0 * epsilon * invDist2) * s_r6 * (1.0 - 2.0 * s_r6);
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
                
                if (d > r_cut * (1.0 + std::sqrt(2.0) / 2.0)) continue;

                for (Particule* pj_ptr : cj->getParticuleList()) {
                    Particule& pj = *pj_ptr;
                    if (&pi == &pj) continue;

                    Vector rij = pi.getPosition() - pj.getPosition();
                    double dist2 = rij.norm2();
                    if (dist2 == 0.0 || dist2 > r_cut2) continue;
                    if (dist2 < min_r2) dist2 = min_r2;

                    double invDist2 = 1.0 / dist2;
                    double s_r6  = sigma6 * invDist2 * invDist2 * invDist2;
                    double coeff = (24.0 * epsilon * invDist2) * s_r6 * (1.0 - 2.0 * s_r6);
                    Vector fij   = rij * (-coeff);

                    pi.setForce(pi.getForce() + fij);
                    pj.setForce(pj.getForce() - fij);
                }
            }
        }
    }
}


void UniversLJ::setConditionsLimites(Univers::ConditionLimite cx, Univers::ConditionLimite cy) {
    conditionX = cx;
    conditionY = cy;
}

void UniversLJ::mettreAJourCellules() {
    appliquerConditionsLimites(conditionX, conditionY);

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

void UniversLJ::appliquerConditionsLimites(Univers::ConditionLimite cx, Univers::ConditionLimite cy) {
    auto appliquerAxe = [&](double &coord, double limit, ConditionLimite condition, double &velocity) {
        bool absorb = false;

        if (condition == Univers::ConditionLimite::REFLEXION) {
            while (coord < 0.0 || coord >= limit) {
                if (coord < 0.0) {
                    coord = -coord;
                    velocity = -velocity;
                } else {
                    coord = 2.0 * limit - coord;
                    velocity = -velocity;
                }
            }
        } else if (condition == Univers::ConditionLimite::PERIODIQUE) {
            if (limit > 0.0) {
                coord = std::fmod(coord, limit);
                if (coord < 0.0) coord += limit;
            }
        } else if (condition == Univers::ConditionLimite::ABSORPTION) {
            if (coord < 0.0 || coord >= limit) {
                absorb = true;
            }
        }

        return absorb;
    };

    std::vector<Particule> survivantes;
    survivantes.reserve(particuleList.size());

    for (Particule& p : particuleList) {
        Vector pos = p.getPosition();
        Vector vel = p.getVitesse();
        bool absorb = false;

        double x = pos.x();
        double y = pos.y();
        double z = pos.z();
        double vx = vel.x();
        double vy = vel.y();
        double vz = vel.z();

        absorb = appliquerAxe(x, l_d.x(), cx, vx) || appliquerAxe(y, l_d.y(), cy, vy);
        if (dimension == 3) {
            absorb = absorb || appliquerAxe(z, l_d.z(), cy, vz);
        }

        if (!absorb) {
            p.setPosition(Vector(x, y, z));
            p.setVitesse(Vector(vx, vy, vz));
            survivantes.push_back(p);
        }
    }

    if (cx == Univers::ConditionLimite::ABSORPTION || cy == Univers::ConditionLimite::ABSORPTION) {
        particuleList = survivantes;
        n_particules = particuleList.size();
    }
}


double UniversLJ::energieCinetique() const {
    double ec = 0.0;
    for (const auto& p : particuleList)
        ec += 0.5 * p.getMasse() * p.getVitesse().norm2();
    return ec;
}

double UniversLJ::energiePotentielle() const {
    double ep = 0.0;
    for (size_t i = 0; i < particuleList.size(); ++i) {
        for (size_t j = i+1; j < particuleList.size(); ++j) {
            double dist = (particuleList[i].getPosition()
                         - particuleList[j].getPosition()).norm();
            if (dist == 0.0 || dist > r_cut) continue;
            double s_r6 = pow(sigma / dist, 6);
            ep += 4.0 * epsilon * s_r6 * (s_r6 - 1.0);
        }
    }
    return ep;
}