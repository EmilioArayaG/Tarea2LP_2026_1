Tarea 2 Lenguajes de Programacion
Nombre: Emilio Alfonso Araya Guzman  
Rol: 202473561-6

---

Intrucciones para compilacion:
- Todos los archivos ('.c', '.h' y 'Makefile') deben estar en la misma carpeta.
- Compilador GCC con estándar C11.
- Sistema Linux o WSL (Ubuntu). (Al menos fue probado en este)

Pasos para compilar y ejecutar
1. Abra la terminal en la carpeta del proyecto
2. Compile con: 'make'
3. Ejecute el juego con: './juego'
4. Para limpiar archivos temporales: 'make clean'

En caso de no compilar
- Verifique estar en Linux/WSL
- Confirme que GCC está instalado: 'gcc --version'
- Compile manualmente: 'gcc -std=c11 -Wall -o juego main.c tablero.c piezas.c armas.c' (ultima opcion, ojala no llegar a usar esto)

---

Arma Especial: "Misil en Cruz" 
¿Cómo funciona?
El misil avanza en línea recta por el tablero hasta golpear una pieza enemiga o el borde del mapa. Al impactar, genera una explosión en forma de cruz que daña:
- La casilla central (impacto)
- Las 4 casillas adyacentes (arriba, abajo, izquierda, derecha)
- Daño: 2 puntos de vida a cada enemigo

Implementación técnica
En 'armas.c', la función 'especial()' utiliza un bucle que simula el movimiento del proyectil hasta detectar una colisión. Luego aplica daño a las 5 casillas de la explosión y libera la memoria de enemigos destruidos.

---

Consideraciones Importantes

1. Funciones internas (static void): Se utilizaron funciones privadas para modularizar el codigo sin alterar los encabezados requeridos por el pdf, es decir asi no alteramos el original. Esto mantiene el código ordenado.

2. Pantalla clara: El juego usa 'system("clear")' para refrescar la pantalla. Por esto es imprescindible ejecutarlo en Linux o WSL.

---
