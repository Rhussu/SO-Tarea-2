# Tarea 2 de Sistemas Operativos



El informe de la tarea se encuentra en el repositorio como [*Informe.pdf*](https://github.com/Rhussu/SO-Tarea-2/blob/main/Informe.pdf)


> ⚠️ **Advertencia**: Todo esta pensado para ser compilado en **Linux**




## 🧱 Parte I, Barrera Reutilizable



Se implementa una barrera que coordina N hebras, obligándolas a esperar hasta que todas lleguen al mismo punto.

La barrera es reutilizable, o sea, soporta múltiples etapas (fase 1, fase 2, etc.).



### 📦 Archivos

```

/barrera/
  barrera.h
  barrera.c
  main.c
```



### ⚙️ Compilación



```

gcc -pthread main.c barrera.c -o barrera

```



### ▶️ Uso

```

./barrera [N_hebras] [E_etapas]

```



- N_hebras → Número de hebras (por defecto 5).



- E_etapas → Número de puntos de sincronización (por defecto 4).



### 📌 Ejemplo



```

./barrera 6 3

```



## 🧠 Parte II, Simulador de Memoria Virtual

Este módulo implementa un simulador de memoria virtual con paginación simple y reemplazo de páginas mediante el algoritmo Reloj. El simulador lee una traza de direcciones virtuales, realiza la traducción a direcciones físicas usando un conjunto limitado de marcos, y reporta el número de fallos de página y la tasa de fallos. También permite ejecutar en modo detallado (--verbose) para mostrar paso a paso la traducción de cada dirección.

### 📦 Archivos
```
/sim/
  sim.c
  traza.txt
  experiments.sh
  traces/
    trace1.txt
    trace2.txt
```

### ⚙️ Compilación
```
gcc sim.c -o sim -lm
```

### ▶️ Uso
```
./sim Nmarcos tamanioMarco [--verbose] traza.txt
```

- **Nmarcos** → Número de marcos físicos disponibles

- **tamanioMarco** → Tamaño del marco en bytes (ej: 8, 4096, etc.)

- **--verbose** (opcional) → Activa modo detallado, mostrando traducción de cada referencia

- **traza.txt** → Archivo con direcciones virtuales (en decimal o hexadecimal)

### 📌 Ejemplo:
```
./sim 3 4096 trace2.txt
./sim 3 4096 --verbose trace1.txt
```
### 🖨️ Output

Si no se usa --verbose:

- Número total de referencias

- Número de fallos de página

- Tasa de fallos

Si se usa --verbose: para cada referencia, imprime:

- Dirección virtual (DV)

- Número de página virtual (nvp)

- Offset

- Si fue HIT o FALLO

- Marco físico usado

- Dirección física resultante (DF)

### 🧪 Experimentos incluidos

Se incluye un script (experiments.sh) que ejecuta simulaciones usando los archivos trace1.txt y trace2.txt, variando los parámetros de:

- Número de marcos físicos (8, 16, 32)

- Tamaño de marco (por ejemplo, 8 bytes para trace1.txt, 4096 bytes para trace2.txt)

Esto permite observar cómo cambia la tasa de fallos de página con distintas configuraciones de memoria física y tamaño de marco.

> **Para ejecutar experiments.sh**:
> ```
> bash experiments.sh
> ```
