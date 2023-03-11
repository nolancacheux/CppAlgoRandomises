#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    // Ouvrir le fichier en mode lecture
    ifstream fichier("C:/Users/Administrateur/Desktop/Examples-main/out/build/x64-debug/TP/Q8_V2/fichier.txt");

    // Vérifier si le fichier est ouvert correctement
    if (!fichier) {
        cerr << "Impossible d'ouvrir le fichier" << endl;
        return 1;
    }

    string ligne;
    int benefice, energie, resultat_pondere, distance;
    string objets_inclus, chemin;

    // Lire chaque ligne du fichier
    getline(fichier, ligne);
    benefice = stoi(ligne);

    getline(fichier, ligne);
    energie = stoi(ligne);

    getline(fichier, objets_inclus);

    getline(fichier, ligne);
    resultat_pondere = stoi(ligne);

    getline(fichier, chemin);

    getline(fichier, ligne);
    distance = stoi(ligne);

    // Afficher les informations récupérées
    cout << "Bénéfice total = " << benefice << endl;
    cout << "Energie consommée = " << energie << endl;
    cout << "Objets inclus " << objets_inclus << endl;
    cout << "Résultat pondéré = " << resultat_pondere << endl;
    cout << "Chemin : " << chemin << endl;
    cout << "Distance minimale = " << distance << endl;

    // Fermer le fichier
    fichier.close();

    return 0;
}
