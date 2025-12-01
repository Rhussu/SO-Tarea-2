# \# Tarea 2 de Sistemas Operativos

# 

# El informe de la tarea se encuentra en el repositorio como \[\*Informe.pdf\*](https://github.com/Rhussu/Shell/blob/main/Informe.pdf)

# 

# 

# \## 🧱 Parte I — Barrera Reutilizable

# 

# Se implementa una barrera que coordina N hebras, obligándolas a esperar hasta que todas lleguen al mismo punto.

# La barrera es reutilizable, o sea, soporta múltiples etapas (fase 1, fase 2, etc.).

# 

# \### 📦 Archivos

# ```

# /barrera/

# &nbsp;   barrier.h

# &nbsp;   barrier.c

# &nbsp;   main.c

# ```

# 

# \### ⚙️ Compilación

# 

# ```

# gcc -pthread main.c barrier.c -o barrera

# ```

# 

# \### ▶️ Uso

# ```

# ./barrera \[N\_hebras] \[E\_etapas]

# ```

# 

# \- N\_hebras → número de hebras (por defecto 5).

# 

# \- E\_etapas → número de puntos de sincronización (por defecto 4).

# 

# \### 📌 Ejemplo

# 

# ```

# ./barrera 6 3

# ```

# 

# \## 🧠 Parte II — Simulador de Memoria Virtual (Reloj)

# 

# Se implementa un simulador que:

# 

# lee una traza de direcciones virtuales desde un archivo,

# 

# aplica paginación simple,

# 

# usa reemplazo Clock cuando no quedan marcos libres,

# 

# reporta fallos de página y tasa de fallos.

# 

# \### 📦 Archivos

# ```

# /sim/

# &nbsp;   sim.c

# &nbsp;   experiments.sh

# &nbsp;   traza.txt

# ```

# 

# \### ⚙️ Compilación y ejecución

# Primero hay que darle permisos de ejecución:

# ```

# chmod +x experiments.sh

# ```

# 

# y luego se ejecuta con:

# ```

# ./experiments.sh

# ```

