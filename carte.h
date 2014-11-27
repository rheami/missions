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

  friend istream& operator >> (istream& is, Carte& carte);
};

#endif

