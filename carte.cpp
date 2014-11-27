/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2014
 *  Squelette pour le TP3
*/

#include "carte.h"
#include <limits>

void Carte::ajouterLieu(const string& nomlieu, const Coordonnee& c){
    // À compléter.
}

void Carte::ajouterRoute(const string& nomroute, const list<string>& route){
    // À compléter.
    // Exemple de ligne dans une carte : «Jeanne-Mance : a b c d ; »
    // Donc, route est une séquence (liste) : <a, b, c, d, e>.
    // Il faut alors ajouter les segments de route (arêtes/arcs) : (a,b), (b,c), (c,d), (d,e).
    // Il ne faut pas ajouter (e,d), (d,c), (c,b), (b,a).
    // Les sens uniques doivent être considérées.
}

double Carte::calculerTrajet(const string& nomorigine, const list<string>& nomsdestinations, 
                             std::list<string>& out_cheminnoeuds, std::list<string>& out_cheminroutes) const
{
   // À compléter. La version actuelle génère un trajet valide, mais généralement non optimal pour plusieurs destinations.
   string position = nomorigine;
   double total = 0;
   for(list<string>::const_reverse_iterator iter=nomsdestinations.rbegin();iter!=nomsdestinations.rend();++iter){
       total += calculerChemin(*iter, position, out_cheminnoeuds, out_cheminroutes);
       position = *iter;
   }
   total += calculerChemin(nomorigine, position, out_cheminnoeuds, out_cheminroutes);
   return total;
}

double Carte::calculerChemin(const string& nomorigine, const string& nomdestination,
                             std::list<string>& out_cheminnoeuds, std::list<string>& out_cheminroutes) const
{
    // À compléter.
    return numeric_limits<double>::infinity();
}

/* Lire une carte. */
istream& operator >> (istream& is, Carte& carte)
{
    // Lire les lieux
    while(is){
        string nomlieu;
        is >> nomlieu;
        if(nomlieu == "---") break;
        Coordonnee coor;
        is >> coor;       
        carte.ajouterLieu(nomlieu, coor);
    }

    // Lire les routes
    while(is){
        string nomroute;
        is >> nomroute;
        if(nomroute == "---" || nomroute=="" || !is) break;
        
        char deuxpoints;
        is >> deuxpoints;
        assert(deuxpoints == ':');
        
        std::list<std::string> listenomslieux;

        string nomlieu;        
        while(is){
            is>>nomlieu;
            if(nomlieu==";") break;
            assert(nomlieu!=":");
            assert(nomlieu.find(";")==string::npos);
            listenomslieux.push_back(nomlieu);
        }
        
        assert(nomlieu==";");
        carte.ajouterRoute(nomroute, listenomslieux);
    }
    
    return is;
}

