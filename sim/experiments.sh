#!/bin/bash

# Compilar el simulador
echo "Compilando sim.c..."
gcc sim.c -o sim -lm

# Archivo de salida CSV
OUTPUT="resultados.csv"
echo "Traza,TamañoMarco,Nmarcos,Referencias,Fallos,TasaFallos" > $OUTPUT

# Función para ejecutar simulador y extraer métricas
run_sim() {
    local trace=$1
    local page_size=$2
    local nmarcos=$3

    echo "Ejecutando ./sim $nmarcos $page_size $trace ..."
    result=$(./sim $nmarcos $page_size $trace)

    # Extraer métricas directamente de las líneas Totales
    referencias=$(echo "$result" | grep "Referencias:" | awk '{print $2}')
    fallos=$(echo "$result" | grep "Fallos de pagina:" | awk '{print $4}')
    tasa=$(echo "$result" | grep "Tasa de fallos:" | awk '{print $4}')

    # Guardar en CSV
    echo "$trace,$page_size,$nmarcos,$referencias,$fallos,$tasa" >> $OUTPUT
}

# Experimentos con trace1.txt (tamaño de marco = 8)
for n in 8 16 32; do
    run_sim traces/trace1.txt 8 $n
done

# Experimentos con trace2.txt (tamaño de marco = 4096)
for n in 8 16 32; do
    run_sim traces/trace2.txt 4096 $n
done

echo "Resultados guardados en $OUTPUT"