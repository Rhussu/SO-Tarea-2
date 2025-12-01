//Compilación en Linux/WSL: gcc sim.c -o sim -lm
/*Ejecución: ./sim Nmarcos tamañomarco [--verbose] traza.txt (traza es un archivo con direcciones de memoria)
    Ej_Modo1: ./sim 3 4096 traza.txt
    Ej_Modo2: ./sim 3 4096 --verbose traza.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Estructura para un marco físico
typedef struct {
    int npv;     // número de página virtual asignada
    int usado;   // bit usado (0 o 1)
} Marco;

// Estructura para la tabla de páginas (simple arreglo)
typedef struct {
    int npv;
    int marco;
} EntradaTabla;

int buscar_tabla(EntradaTabla *tabla, int n, int npv) {
    for (int i = 0; i < n; i++) {
        if (tabla[i].npv == npv) return tabla[i].marco;
    }
    return -1;
}

void insertar_tabla(EntradaTabla *tabla, int *n, int npv, int marco) {
    tabla[*n].npv = npv;
    tabla[*n].marco = marco;
    (*n)++;
}

void borrar_tabla(EntradaTabla *tabla, int *n, int npv) {
    for (int i = 0; i < *n; i++) {
        if (tabla[i].npv == npv) {
            tabla[i] = tabla[*n - 1];  // reemplazar por el último
            (*n)--;
            return;
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("Uso: ./sim Nmarcos tamanioMarco [--verbose] traza.txt\n");
        return 1;
    }

    int Nmarcos = atoi(argv[1]);
    int PAGE_SIZE = atoi(argv[2]);
    int verbose = 0;
    char *trace_file;

    if (argc == 5) {
        if (strcmp(argv[3], "--verbose") == 0) verbose = 1;
        trace_file = argv[4];
    } else {
        trace_file = argv[3];
    }

    int b = log2(PAGE_SIZE);
    int MASK = PAGE_SIZE - 1;

    // Crear marcos
    Marco *marcos = malloc(sizeof(Marco) * Nmarcos);
    for (int i = 0; i < Nmarcos; i++) {
        marcos[i].npv = -1;
        marcos[i].usado = 0;
    }

    // Tabla de páginas simple
    EntradaTabla tabla[10000];
    int tabla_size = 0;

    int puntero_reloj = 0;
    int total_referencias = 0;
    int fallos_pagina = 0;

    FILE *f = fopen(trace_file, "r");
    if (!f) {
        printf("Error al abrir archivo %s\n", trace_file);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n') continue;

        int DV;
        if (strncmp(line, "0x", 2) == 0 || strncmp(line, "0X", 2) == 0) {
            DV = strtol(line, NULL, 16);
        } else {
            DV = atoi(line);
        }

        int offset = DV & MASK;
        int npv = DV >> b;

        total_referencias++;
        int marco_usado;

        int marco_existente = buscar_tabla(tabla, tabla_size, npv);

        int hit = (marco_existente != -1);

        if (hit) {
            marco_usado = marco_existente;
        } else {
            fallos_pagina++;

            int marco_libre = -1;
            for (int i = 0; i < Nmarcos; i++) {
                if (marcos[i].npv == -1) {
                    marco_libre = i;
                    break;
                }
            }

            if (marco_libre != -1) {
                marco_usado = marco_libre;
                marcos[marco_usado].npv = npv;
                marcos[marco_usado].usado = 1;
                insertar_tabla(tabla, &tabla_size, npv, marco_usado);
            } else {
                while (1) {
                    if (marcos[puntero_reloj].usado == 0) {
                        int vieja = marcos[puntero_reloj].npv;
                        borrar_tabla(tabla, &tabla_size, vieja);

                        marcos[puntero_reloj].npv = npv;
                        marcos[puntero_reloj].usado = 1;
                        insertar_tabla(tabla, &tabla_size, npv, puntero_reloj);

                        marco_usado = puntero_reloj;
                        puntero_reloj = (puntero_reloj + 1) % Nmarcos;
                        break;
                    } else {
                        marcos[puntero_reloj].usado = 0;
                        puntero_reloj = (puntero_reloj + 1) % Nmarcos;
                    }
                }
            }
        }

        int DF = (marco_usado << b) | offset;

        if (hit) {
            marcos[marco_usado].usado = 1;
        }

        if (verbose) {
            printf("DV: %d npv: %d offset: %d %s marco: %d DF: %d\n",
                   DV, npv, offset,
                   hit ? "HIT" : "FALLO",
                   marco_usado,
                   DF);
        }
    }

    printf("Totales:\n");
    printf("  Referencias: %d\n", total_referencias);
    printf("  Fallos de pagina: %d\n", fallos_pagina);
    printf("  Tasa de fallos: %.6f\n",
           (double) fallos_pagina / total_referencias);

    fclose(f);
    free(marcos);

    return 0;
}
