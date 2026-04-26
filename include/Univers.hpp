#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include <vector>
#include "Particule.hpp"
#include "Vector.hpp"


/**
 * @brief Classe abstraite représentant un univers contenant un ensemble de particules
 * 
 * Cette classe gère une collection de particules dans un espace de dimension donnée.
 * Elle définit l'interface commune pour différents types d'univers (gravitationnel, Lennard-Jones, etc.).
 */

class Univers
{
protected:
    int dimension; ///< Dimension de l'espace (1D, 2D, 3D)
    std::vector<Particule> particuleList; ///< Liste des particules
    int n_particules; ///< Nombre de particules

public:
    /**
     * @brief Constructeur de l'univers
     * @param dim Dimension de l'espace
     * @param reserveCount Nombre de particules à réserver (optionnel)
     */
    Univers(int dim, int reserveCount = 0);

    /**
     * @brief Destructeur virtuel
     */
    virtual ~Univers() = default;

    /**
     * @brief Ajoute une particule à l'univers
     * @param p La particule à ajouter
     */
    void ajouterParticule(const Particule& p);

    /**
     * @brief Fait évoluer les particules dans le temps avec l'algorithme Störmer-Verlet
     * @param tEnd Temps final de la simulation
     * @param dt Pas de temps
     */
    void avancerParticules(double tEnd, double dt);

    /**
     * @brief Getter de la dimension
     * @return La dimension de l'univers
     */
    int getDimension() const;

    /**
     * @brief Getter du nombre de particules
     * @return Nombre de particules
     */
    int getNombreParticules() const;

    /**
     * @brief Getter de la liste des particules
     * @return Référence à la liste de particules
     */
    const std::vector<Particule>& getParticules() const;

    /**
     * @brief Getter non-const de la liste des particules
     * @return Référence non-const à la liste de particules
     */
    std::vector<Particule>& getParticules();

    /**
     * @brief Calcule les forces appliquées sur chaque particule
     * @return Un vecteur contenant les forces associées à chaque particule
     */
    virtual std::vector<Vector> calculerForces() = 0;

    /**
     * @brief Met à jour les cellules (si applicable)
     * Méthode virtuelle vide par défaut pour les univers sans cellules
     */
    virtual void mettreAJourCellules() {}

    /**
     * @brief Modifie la dimension.
     * @param dim Nouvelle dimension.
     * @return Référence sur l'objet courant.
     */
    Univers& setDimension(int dim);

    /**
     * @brief Modifie le nombre de particule.
     * @param n nombre de particules
     * @return Référence sur l'objet courant.
     */
    Univers& setNombreParticules(int n);

    /**
     * @brief Modifie la liste des particules.
     * @param particules nouvelle liste de particules
     * @return Référence sur l'objet courant.
     */
    Univers& setParticules(const std::vector<Particule>& particules);
};

#endif