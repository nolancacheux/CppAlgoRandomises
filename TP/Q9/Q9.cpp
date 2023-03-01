#include <algorithm> // Pour la fonction de tri std::sort()
#include <limits> // Pour la valeur numérique maximale numeric_limits<int>::max()
#include <vector> // Pour utiliser des vecteurs
#include <tuple> // Pour stocker les données de P1 dans un tuple
#include <string> // Pour stocker les noms de villes
#include <iostream> // Pour afficher les résultats intermédiaires
#include <fstream>
using namespace std;

// Structure de données pour les toutes les solutions trouvées au problème P1
struct SolutionsP1Data {
    vector<int> benef; 
    vector<int> consommation;
    vector<tuple<int, int, int>> objects; // (nom/index, consommation, bénéfice)
};

// Structure de données pour les données du problème P2
struct SolutionsP2Data {
    int num_cities; // Nombre de villes
    vector<string> city_names; // Noms des villes
    vector<vector<int>> distances; // Matrice des distances
};


// Fonction pour lire les données du problème P1 à partir d'un fichier
P1Data readP1File(std::string filename) {
    std::ifstream infile(filename);
    P1Data data;
    if (infile.is_open()) {
        infile >> data.capacity >> data.num_objects;
        int index, consumption, profit;
        for (int i = 0; i < data.num_objects; i++) {
            infile >> index >> consumption >> profit;
            data.objects.push_back(std::make_tuple(index, consumption, profit));
        }
        return data;
    }
    else {
        cerr << "Le fichier P1 ne peut pas être ouvert" << endl;
        exit(1);
    }
}