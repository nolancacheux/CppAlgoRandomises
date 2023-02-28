#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <locale.h>
using namespace std;

// Structure de donn�es pour les donn�es du probl�me P1
struct P1Data {
    int capacity; // Capacit� du sac � dos
    int num_objects; // Nombre d'objets disponibles
    std::vector<std::tuple<int, int, int>> objects; // (nom/index, consommation, b�n�fice)
};

// Structure de donn�es pour les donn�es du probl�me P2
struct P2Data {
    int num_cities; // Nombre de villes
    std::vector<std::string> city_names; // Noms des villes
    std::vector<std::vector<int>> distances; // Matrice des distances
};

// Fonction pour lire les donn�es du probl�me P1 � partir d'un fichier
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
        cerr << "Le fichier P1 ne peut pas �tre ouvert" << endl;
        exit(1);
    }
}

// Fonction pour lire les donn�es du probl�me P2 � partir d'un fichier
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
        cerr << "Le fichier P2 ne peut pas �tre ouvert" << endl;
        exit(1);
    }
}

// Fonction pour afficher les donn�es du probl�me P1
void printP1Data(P1Data data) {
    std::cout << "Capacit� du sac � dos : " << data.capacity << std::endl;
    std::cout << "Nombre d'objets disponibles : " << data.num_objects << std::endl;
    for (int i = 0; i < data.num_objects; i++) {
        int index, consumption, profit;
        std::tie(index, consumption, profit) = data.objects[i];
        std::cout << "Objet " << index << " : consommation = " << consumption << ", b�n�fice = " << profit << std::endl;
    }
}

// Fonction pour afficher les donn�es du probl�me P2
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
    // V�rifier si les noms de fichiers ont �t� fournis en tant que param�tres
    if (argc != 3) {
        cerr << "Deux noms de fichiers doivent �tre fournis en tant que param�tres." << endl;
        return 1;
    }
    
    // Lecture des fichiers d'entr�e
    P1Data p1_data = readP1File(argv[1]);
    P2Data p2_data = readP2File(argv[2]);
    
    // Affichage des donn�es lues
    std::cout << "Donn�es du probl�me P1 :" << std::endl;
    printP1Data(p1_data);
    std::cout << std::endl;
    std::cout << "Donn�es du probl�me P2 :" << std::endl;
    printP2Data(p2_data);
    
    return 0;
}

