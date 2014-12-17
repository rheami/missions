/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2014
 *  Squelette pour le TP3
*/

#ifndef CARTE_HEADER
#define CARTE_HEADER

#include <assert.h>
#include <istream>
#include <list>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "coordonnee.h"

using namespace std;

class Carte {
public:

    void ajouterLieu(string const &nomlieu, const Coordonnee &coordonnee);

    void ajouterRoute(const string &nom, const list <string> &noms);

    double calculerTrajet_1(string const &nomorigine, list <string> const &nomsdestinations, list <string> &out_cheminnoeuds, list <string> &out_cheminroutes) const;

    double calculerChemin(const int origine, const int destination,
            std::list <string> &out_cheminnoeuds, std::list <string> &out_cheminroutes) const;

private:

    struct Arete {
        int indicelieu;
        int indiceroute;
    };

    struct Lieu { // sommet
        string nomlieu;
        Coordonnee coordonnee;
        vector <Arete> aretes;    // aretes (sommets des aretes sortantes)
        vector<int> destinations;
    };

    std::unordered_map<string, unsigned int> indices;
    vector <Lieu> lieux; // sommets du graphe
    vector <string> routes;

    void ajouterArete(const int io, const int id, string const &nomRoute);

    friend istream &operator>>(istream &is, Carte &carte);

    friend ostream &operator<<(ostream &os, Carte &carte);

    inline double distanceEuclidienne(int const iOrigine, int const iDestination) const {
        Coordonnee const &coordonneeD = lieux[iDestination].coordonnee;
        return lieux[iOrigine].coordonnee.distance(coordonneeD);
    }

    double AStarAlgorithm(int const origine, int const destination, unordered_map<int, int> &parents) const;

    void chemin(int courant, unordered_map<int, int> parents, list <string> &out_cheminnoeuds, list <string> &out_cheminroutes) const;


    inline double heuristique(int const iOrigine, int const iDestination) const {
        return distanceEuclidienne(iOrigine, iDestination);
    };

};

#endif

