#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <locale.h>
using namespace std;

// Structure de données pour les données du problème P1
struct P1Data {
    int capacity; // Capacité du sac à dos
    int num_objects; // Nombre d'objets disponibles
    std::vector<std::tuple<int, int, int>> objects; // (nom/index, consommation, bénéfice)
};

// Structure de données pour les données du problème P2
struct P2Data {
    int num_cities; // Nombre de villes
    std::vector<std::string> city_names; // Noms des villes
    std::vector<std::vector<int>> distances; // Matrice des distances
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

// Fonction pour lire les données du problème P2 à partir d'un fichier
P2Data readP2File(std::string filename) {
    std::ifstream infile(filename);
    P2Data data;
    if (infile.is_open()) {
        infile >> data.num_cities;
        std::string city_name;
        for (int i = 0; i < data.num_cities; i++) {
            infile >> city_name;
            data.city_names.push_back(city_name);
        }
        data.distances.resize(data.num_cities, std::vector<int>(data.num_cities));
        for (int i = 0; i < data.num_cities; i++) {
            for (int j = 0; j < data.num_cities; j++) {
                infile >> data.distances[i][j];
            }
        }
        return data;
    }
    else {
        cerr << "Le fichier P2 ne peut pas être ouvert" << endl;
        exit(1);
    }
}

// Fonction pour afficher les données du problème P1
void printP1Data(P1Data data) {
    std::cout << "Capacité du sac à dos : " << data.capacity << std::endl;
    std::cout << "Nombre d'objets disponibles : " << data.num_objects << std::endl;
    for (int i = 0; i < data.num_objects; i++) {
        int index, consumption, profit;
        std::tie(index, consumption, profit) = data.objects[i];
        std::cout << "Objet " << index << " : consommation = " << consumption << ", bénéfice = " << profit << std::endl;
    }
}

// Fonction pour afficher les données du problème P2
void printP2Data(P2Data data) {
    std::cout << "Nombre de villes : " << data.num_cities << std::endl;
    std::cout << "Noms des villes :" << std::endl;
    for (int i = 0; i < data.num_cities; i++) {
        std::cout << data.city_names[i] << std::endl;
    }
    std::cout << "Matrice des distances :" << std::endl;
    for (const auto& row : data.distances) {
        for (const auto& distance : row) {
            std::cout << distance << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}




int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "FR_fr");
    // Vérifier si les noms de fichiers ont été fournis en tant que paramètres
    if (argc != 3) {
        cerr << "Deux noms de fichiers doivent être fournis en tant que paramètres." << endl;
        return 1;
    }
    
    // Lecture des fichiers d'entrée
    P1Data p1_data = readP1File(argv[1]);
    P2Data p2_data = readP2File(argv[2]);
    
    // Affichage des données lues
    std::cout << "Données du problème P1 :" << std::endl;
    printP1Data(p1_data);
    std::cout << std::endl;
    std::cout << "Données du problème P2 :" << std::endl;
    printP2Data(p2_data);
    
    return 0;
}

