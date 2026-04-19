#include "gtest/gtest.h"
#include "Univers_tp4.hpp"
#include "Particule.hpp"
#include "Vector.hpp"
#include <cmath>

// ── Initialisation des cellules ──────────────────────────────

TEST(UniversTp4Test, NombreCellules2D) {
    // Lx=10, Ly=10, r_cut=2.5 → floor(10/2.5)=4 → 4*4=16 cellules
    Univers_tp4 u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.0);
    u.initialiserCellules();
    EXPECT_EQ(u.getNbCellules(), 16);
}

TEST(UniversTp4Test, VoisinesInterieure2D) {
    // Cellule intérieure (1,1) doit avoir 9 voisines (elle-même incluse)
    Univers_tp4 u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.0);
    u.initialiserCellules();
    EXPECT_EQ(u.getCellule(1, 1, 0).getVoisines().size(), 9);
}

TEST(UniversTp4Test, VoisinesCoin2D) {
    // Cellule coin (0,0) doit avoir 4 voisines
    Univers_tp4 u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.0);
    u.initialiserCellules();
    EXPECT_EQ(u.getCellule(0, 0, 0).getVoisines().size(), 4);
}

// ── mettreAJourCellules ──────────────────────────────────────

TEST(UniversTp4Test, ParticuleDansBonneCellule) {
    Univers_tp4 u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(1.0, 1.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(3.0, 3.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    EXPECT_EQ(u.getCellule(0, 0, 0).getParticuleList().size(), 1);
    EXPECT_EQ(u.getCellule(1, 1, 0).getParticuleList().size(), 1);
}

TEST(UniversTp4Test, MiseAJourApresDeplacement) {
    Univers_tp4 u(2, Vector(10.0, 10.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(1.0, 1.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.initialiserCellules();
    // Déplacer la particule vers la cellule (1,0)
    u.getParticules()[0].setPosition(Vector(3.0, 1.0, 0.0));
    u.mettreAJourCellules();
    EXPECT_EQ(u.getCellule(0, 0, 0).getParticuleList().size(), 0);
    EXPECT_EQ(u.getCellule(1, 0, 0).getParticuleList().size(), 1);
}

// ── Forces de Lennard-Jones ──────────────────────────────────

TEST(UniversTp4Test, ForceNulleAuMinimum) {
    // r = 2^(1/6)*sigma → force nulle (minimum du potentiel LJ)
    double r_star = std::pow(2.0, 1.0/6.0);
    Univers_tp4 u(2, Vector(20.0, 20.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,        10.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0+r_star, 10.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    u.calculerForces();
    EXPECT_NEAR(u.getParticules()[0].getForce().x(), 0.0, 1e-6);
}

TEST(UniversTp4Test, ForceRepulsiveProche) {
    // r < r* → répulsion : force sur p0 dans le sens -x
    Univers_tp4 u(2, Vector(20.0, 20.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,       10.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + 0.9, 10.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    u.calculerForces();
    EXPECT_LT(u.getParticules()[0].getForce().x(), 0.0);
}

TEST(UniversTp4Test, ForceAttractiveLoin) {
    // r* < r < r_cut → attraction : force sur p0 dans le sens +x
    Univers_tp4 u(2, Vector(20.0, 20.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,       10.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + 1.5, 10.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    u.calculerForces();
    EXPECT_GT(u.getParticules()[0].getForce().x(), 0.0);
}

TEST(UniversTp4Test, ForceNulleAuDela_rcut) {
    // r > r_cut → force nulle
    Univers_tp4 u(2, Vector(20.0, 20.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,       10.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + 3.0, 10.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    u.calculerForces();
    EXPECT_NEAR(u.getParticules()[0].getForce().x(), 0.0, 1e-10);
}

TEST(UniversTp4Test, Newton3emeLoi) {
    // F_ij + F_ji = 0
    Univers_tp4 u(2, Vector(20.0, 20.0, 0.0), 2.5, 1.0, 1.0);
    u.ajouterParticule(Particule(Vector(10.0,       10.0, 0.0), Vector(0,0,0), 1.0, 0, 0, Vector(0,0,0)));
    u.ajouterParticule(Particule(Vector(10.0 + 1.5, 10.0, 0.0), Vector(0,0,0), 1.0, 1, 0, Vector(0,0,0)));
    u.initialiserCellules();
    u.calculerForces();
    double fx0 = u.getParticules()[0].getForce().x();
    double fx1 = u.getParticules()[1].getForce().x();
    EXPECT_NEAR(fx0 + fx1, 0.0, 1e-10);
}