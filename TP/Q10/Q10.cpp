#include <iostream>
#include <fstream>
#include <random>
#include <string>

using namespace std;

// Structure pour les données du problème P1
struct P1Data {
    int num_objects;
    int max_energy;
    vector<int> profits;
    vector<int> energies;
};

// Structure pour les données du problème P2
struct P2Data {
    int num_cities;
    vector<string> city_names;
    vector<vector<int>> distances;
};

// Fonction pour générer aléatoirement les données du problème P1
P1Data generateP1Data(int num_objects, int max_energy) {
    P1Data data;
    data.num_objects = num_objects;
    data.max_energy = max_energy;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib_p(1, 10); // Profits entre 1 et 10
    std::uniform_int_distribution<> distrib_e(1, max_energy); // Energies entre 1 et max_energy

    for (int i = 0; i < num_objects; i++) {
        data.profits.push_back(distrib_p(gen));
        data.energies.push_back(distrib_e(gen));
    }

    return data;
}

// Fonction pour écrire les données du problème P1 dans un fichier
void writeP1File(const P1Data& data, const string& filename) {
    ofstream file(filename);
    file << data.max_energy << " " << data.num_objects << endl;
    for (int i = 0; i < data.num_objects; i++) {
        file << i + 1 << " " << data.profits[i] << " " << data.energies[i] << endl;
    }
    file.close();
}

// Fonction pour générer aléatoirement les données du problème P2
P2Data generateP2Data(int num_cities) {
    P2Data data;
    data.num_cities = num_cities;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib_d(1, 100); // Distances entre 1 et 100

    // Générer les noms des villes
    for (int i = 0; i < num_cities; i++) {
        data.city_names.push_back("Ville" + to_string(i + 1));
    }

    // Générer les distances entre les villes (matrice carrée symétrique)
    for (int i = 0; i < num_cities; i++) {
        vector<int> row;
        for (int j = 0; j < num_cities; j++) {
            if (i == j) {
                row.push_back(0);
            }
            else if (j > i) { // On ne génère qu'une moitié de la matrice (la partie supérieure)
                row.push_back(distrib_d(gen));
            }
            else {
                row.push_back(data.distances[j][i]);
            }
        }
        data.distances.push_back(row);
    }

    return data;
}

// Fonction pour écrire les données du problème P2 dans un fichier
void writeP2File(const P2Data& data, const string& filename) {
    ofstream file(filename);
    file << data.num_cities << endl;
    for (const auto& city : data.city_names) {
        file << city << endl;
    }
    for (const auto& row : data.distances) {
        for (const auto& distance :row) {
            file << distance << " ";
        }
        file << endl;
    }
    file.close();
}

int main() {
    // Générer les données du problème P1 et les écrire dans un fichier
    P1Data p1_data = generateP1Data(10, 20);
    writeP1File(p1_data, "../Q10/P1_data.txt");

    // Générer les données du problème P2 et les écrire dans un fichier
    P2Data p2_data = generateP2Data(5);
    writeP2File(p2_data, "../Q10/P2_data.txt");

    std::cout << "Les fichiers ont ete ecrits avec succes";

    return 0;
}
