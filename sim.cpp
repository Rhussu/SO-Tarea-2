//Compilación en Linux/WSL: g++ sim.cpp -o sim -std=c++11
/*Ejecución: ./sim Nmarcos tamañomarco [--verbose] traza.txt (traza es un archivo con direcciones de memoria)
    Ej_Modo1: ./sim 3 4096 traza.txt
    Ej_Modo2: ./sim 3 4096 --verbose traza.txt
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

// Estructura para un marco físico
struct Marco {
    int npv = -1;       // Página virtual asignada (-1 si libre)
    bool usado = false; // Bit usado para algoritmo Reloj
};

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Uso: ./sim Nmarcos tamañomarco [--verbose] traza.txt" << endl;
        return 1;
    }

    int Nmarcos = stoi(argv[1]);
    int PAGE_SIZE = stoi(argv[2]);
    bool verbose = false;
    string trace_file;

    if (argc == 5) {
        string opt = argv[3];
        if (opt == "--verbose") verbose = true;
        trace_file = argv[4];
    } else {
        trace_file = argv[3];
    }

    int b = log2(PAGE_SIZE); // número de bits del offset
    int MASK = PAGE_SIZE - 1;

    // Marcos físicos
    vector<Marco> marcos(Nmarcos);

    // Tabla de páginas: npv -> índice de marco
    unordered_map<int, int> tabla_paginas;

    int puntero_reloj = 0;
    int total_referencias = 0;
    int fallos_pagina = 0;

    // Abrir archivo de traza
    ifstream infile(trace_file);
    if (!infile.is_open()) {
        cerr << "Error al abrir archivo " << trace_file << endl;
        return 1;
    }

    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue;

        // Leer DV (decimal o hexadecimal)
        int DV;
        if (line.find("0x") == 0 || line.find("0X") == 0) {
            DV = stoi(line, nullptr, 16);
        } else {
            DV = stoi(line);
        }

        int offset = DV & MASK;
        int npv = DV >> b;

        total_referencias++;
        bool hit = false;
        int marco_usado;

        // Ver si npv está en la tabla de páginas
        if (tabla_paginas.find(npv) != tabla_paginas.end()) {
            hit = true;
            marco_usado = tabla_paginas[npv];
        } else {
            fallos_pagina++;

            // Buscar marco libre
            bool marco_libre = false;
            for (int i = 0; i < Nmarcos; ++i) {
                if (marcos[i].npv == -1) {
                    marco_usado = i;
                    marcos[i].npv = npv;
                    marcos[i].usado = true;
                    tabla_paginas[npv] = i;
                    marco_libre = true;
                    break;
                }
            }

            // Si no hay marco libre, usar algoritmo Reloj
            if (!marco_libre) {
                while (true) {
                    if (!marcos[puntero_reloj].usado) {
                        // Reemplazar página
                        int npv_vieja = marcos[puntero_reloj].npv;
                        tabla_paginas.erase(npv_vieja);

                        marcos[puntero_reloj].npv = npv;
                        marcos[puntero_reloj].usado = true;
                        tabla_paginas[npv] = puntero_reloj;
                        marco_usado = puntero_reloj;

                        puntero_reloj = (puntero_reloj + 1) % Nmarcos;
                        break;
                    } else {
                        marcos[puntero_reloj].usado = false;
                        puntero_reloj = (puntero_reloj + 1) % Nmarcos;
                    }
                }
            }
        }

        int DF = (marco_usado << b) | offset;

        if (hit) {
            marcos[marco_usado].usado = true; // actualizar bit de uso
        }

        if (verbose) {
            cout << "DV: " << DV
                 << " npv: " << npv
                 << " offset: " << offset
                 << " " << (hit ? "HIT" : "FALLO")
                 << " marco: " << marco_usado
                 << " DF: " << DF
                 << endl;
        }
    }

    // Resultados finales
    cout << "Totales:" << endl;
    cout << "  Referencias: " << total_referencias << endl;
    cout << "  Fallos de página: " << fallos_pagina << endl;
    cout << "  Tasa de fallos: " << (double)fallos_pagina / total_referencias << endl;

    infile.close();
    return 0;
}
