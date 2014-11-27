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

class Carte{
  public:
    // À compléter (vous pouvez tout changer).
    void ajouterLieu(const string& nom, const Coordonnee& c);
    void ajouterRoute(const string& nom, const list<string>& noms);

    double calculerTrajet(const string& origine, const list<string>& destination, 
                          std::list<string>& out_cheminnoeuds, std::list<string>& out_cheminroutes) const;
    double calculerChemin(const string& origine, const string& destination,
                             std::list<string>& out_cheminnoeuds, std::list<string>& out_cheminroutes) const;
  private:
    // À compléter.
    struct Lieu { // lieux = sommets du graphe
        string nomlieu;
        Lieu(const string& nomlieu_, const Coordonnee& c_) :nomlieu(nomlieu_), coor(c_){}
        Coordonnee coor;
        set<unsigned int> voisins; // lieux voisin directement lié
        mutable bool visite;
    };
    map<string, unsigned int> indices;
    vector<Lieu> lieux;

    void ajouterAreteNonOrientee(const int io, const int id);
    friend istream& operator >> (istream& is, Carte& carte);
    friend ostream& operator << (ostream& os, Carte& carte);
};

#endif

