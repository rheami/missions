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
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include "coordonnee.h"

using namespace std;

class Carte {
public:

    void ajouterLieu(string const &nomlieu, const Coordonnee &coordonnee);

    void ajouterRoute(const string &nom, const list<string> &noms);

    double calculerTrajet(const string &origine, const list<string> &destination,
            std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const;

    double calculerChemin(const string &origine, const string &destination,
            std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const;

private:

    struct Lieu { // sommet
        string nomlieu;

        Coordonnee coordonnee;

        vector<int> aretes;    // aretes (sommets des aretes sortantes)
        vector<string> routes; // nom des routes des aretes (meme indice)
        // mutable bool visite; pas utilisé ! // todo retirer
    };

    map<string, unsigned int> indices;
    vector<Lieu> lieux; // sommets du graphe

    void ajouterArete(const int io, const int id, string const &nomRoute);

    friend istream &operator>>(istream &is, Carte &carte);

    friend ostream &operator<<(ostream &os, Carte &carte);

    double distanceDirecte(int const indiceorigine, int const indicedestination);

    void DijkstraAlgorithm(int const iOrigine, vector<double> &distancesmin, vector<int> &parents) const;

    void DijkstraChemin(int indexLieu, vector<int> const &parents, list<string> &out_cheminnoeuds, list<string> &out_cheminroutes) const;


};

#endif

