#include <stdio.h>
#include <conio.h>
#include <math.h>

#define NUM_PATRONES 21
#define NUM_NEURONAS 7
#define NUM_ENTRADAS 64
#define MAX_EPOCAS 10000

float fn_propagacion(int entrada, float peso, float Net){
   return Net + (entrada * peso);
}

float fn_activacion(float Net){
   return tanh(Net);
}

float fn_salida(float fnet){
   return fnet;
}

float fn_delta(float Y, int d, int entrada, float peso, float alfa){
   float delta = alfa * entrada * (d - Y) * (1 - Y * Y);
   return peso + delta;
}

int main(){

   // =========================================================
   // PARAMETROS DE LA RED
   // =========================================================
   float alfa = 0.05;
   float tolerancia = 0.01;
   float peso_inicial = 0.0;

   char letras[NUM_NEURONAS] = {'A', 'B', 'C', 'D', 'E', 'J', 'K'};

   float w[NUM_NEURONAS][NUM_ENTRADAS];
   int X[NUM_PATRONES][NUM_ENTRADAS];
   int d[NUM_PATRONES][NUM_NEURONAS];
   float Y[NUM_PATRONES][NUM_NEURONAS];

   float Net = 0;
   int f = 0;
   int aux = 0;
   int epoca = 0;

   bool entrenada = false;

   // =========================================================
   // MATRIZ DE SALIDAS DESEADAS
   // =========================================================
   int h = 0;

   for(int i = 0; i < NUM_PATRONES; i++){
      for(int j = 0; j < NUM_NEURONAS; j++){
         if(h == j)
            d[i][j] = 1;
         else
            d[i][j] = -1;
      }

      h++;

      if(h % NUM_NEURONAS == 0)
         h = 0;
   }

   // =========================================================
   // LEER PATRONES DE ENTRENAMIENTO
   // =========================================================
   FILE *archivo = fopen("../Letras/TRAIN.DAT", "r");

   if(archivo == NULL){
      printf("Error al abrir TRAIN.DAT\n");
      return 1;
   }

   char linea[10];

   while(fgets(linea, 10, archivo) != NULL){
      for(int i = 0; i < 7; i++){
         if(linea[i] == '#' || linea[i] == '@')
            X[f][aux] = 1;
         else if(linea[i] == '.' || linea[i] == 'O')
            X[f][aux] = -1;

         aux++;
      }

      if(aux >= 62){
         X[f][63] = 1; // El ultimo valor es el bias
         f++;
         aux = 0;
      }
   }

   fclose(archivo);

   // =========================================================
   // INICIALIZAR PESOS
   // =========================================================
   for(int i = 0; i < NUM_NEURONAS; i++){
      for(int j = 0; j < NUM_ENTRADAS; j++){
         w[i][j] = peso_inicial;
      }
   }

   // =========================================================
   // MOSTRAR PARAMETROS
   // =========================================================
   printf("\n========================================\n");
   printf("       ENTRENAMIENTO REGLA DELTA\n");
   printf("========================================\n\n");

   printf("alfa         = %.4f\n", alfa);
   printf("tolerancia   = %.4f\n", tolerancia);
   printf("peso inicial = %.4f\n", peso_inicial);
   printf("max. epocas  = %d\n\n", MAX_EPOCAS);

   printf("Epoca     Error          Patrones correctos\n");
   printf("------------------------------------------------\n");

   // =========================================================
   // ENTRENAMIENTO
   // =========================================================
   while(entrenada == false && epoca < MAX_EPOCAS){

      // -------------------------------------------------------
      // PASO 1. PROPAGAR Y ACTUALIZAR PESOS
      // -------------------------------------------------------
      for(int h = 0; h < NUM_PATRONES; h++){
         for(int i = 0; i < NUM_NEURONAS; i++){

            Net = 0;

            for(int j = 0; j < NUM_ENTRADAS; j++){
               Net = fn_propagacion(X[h][j], w[i][j], Net);
            }

            Y[h][i] = fn_salida(fn_activacion(Net));

            // APRENDIZAJE DELTA
            for(int j = 0; j < NUM_ENTRADAS; j++){
               w[i][j] = fn_delta(Y[h][i], d[h][i], X[h][j], w[i][j], alfa);
            }
         }
      }

      // -------------------------------------------------------
      // PASO 2. PROPAGAR NUEVAMENTE SIN MODIFICAR PESOS
      // -------------------------------------------------------
      for(int h = 0; h < NUM_PATRONES; h++){
         for(int i = 0; i < NUM_NEURONAS; i++){

            Net = 0;

            for(int j = 0; j < NUM_ENTRADAS; j++){
               Net = fn_propagacion(X[h][j], w[i][j], Net);
            }

            Y[h][i] = fn_salida(fn_activacion(Net));
         }
      }

      // -------------------------------------------------------
      // PASO 3. CALCULAR ERROR POR PATRON
      // -------------------------------------------------------
      float E[NUM_PATRONES];

      for(int i = 0; i < NUM_PATRONES; i++){
         E[i] = 0;

         for(int j = 0; j < NUM_NEURONAS; j++){
            E[i] += 0.5 * pow(d[i][j] - Y[i][j], 2);
         }
      }

      // -------------------------------------------------------
      // PASO 4. ERROR GLOBAL Y PATRONES DENTRO DE TOLERANCIA
      // -------------------------------------------------------
      float errorEpoca = 0;
      int correctos = 0;

      for(int i = 0; i < NUM_PATRONES; i++){
         errorEpoca += E[i];

         if(E[i] <= tolerancia)
            correctos++;
      }

      epoca++;

      // Imprimir resultado de la epoca
      printf("%5d     %10.6f        %2d/%d\n", epoca, errorEpoca, correctos, NUM_PATRONES);

      // -------------------------------------------------------
      // CRITERIO DE PARO
      // -------------------------------------------------------
      if(correctos == NUM_PATRONES){
         entrenada = true;
      }
   }

   // =========================================================
   // RESULTADO DEL ENTRENAMIENTO
   // =========================================================
   printf("\n========================================\n");

   if(entrenada){
      printf("*** RED ENTRENADA ***\n");
      printf("Convergencia alcanzada en la epoca %d\n", epoca);
   }
   else{
      printf("*** MAXIMO DE EPOCAS ALCANZADO ***\n");
      printf("La red no alcanzo la tolerancia para todos los patrones.\n");
   }

   printf("========================================\n");

   // =========================================================
   // PESOS FINALES
   // =========================================================
   printf("\n\n========================================\n");
   printf("            PESOS FINALES\n");
   printf("========================================\n");

   for(int i = 0; i < NUM_NEURONAS; i++){

      printf("\nNeurona %c:\n", letras[i]);

      for(int j = 0; j < NUM_ENTRADAS; j++){

         printf("%8.4f ", w[i][j]);

         if((j + 1) % 8 == 0)
            printf("\n");
      }
   }

   // =========================================================
   // TEST
   // =========================================================
   printf("\n\nPulse una tecla para realizar TEST...\n");
   getch();

   FILE *archivo2 = fopen("../Letras/TEST.DAT", "r");

   if(archivo2 == NULL){
      printf("Error al abrir TEST.DAT\n");
      return 1;
   }

   char linea2[10];

   aux = 0;
   f = 0;

   // =========================================================
   // LEER PATRONES DE TEST
   // =========================================================
   while(fgets(linea2, 10, archivo2) != NULL){

      for(int i = 0; i < 7; i++){

         if(linea2[i] == '#' || linea2[i] == '@')
            X[f][aux] = 1;
         else if(linea2[i] == '.' || linea2[i] == 'O')
            X[f][aux] = -1;

         aux++;
      }

      if(aux >= 62){
         X[f][63] = 1;
         f++;
         aux = 0;
      }
   }

   fclose(archivo2);

   // =========================================================
   // PROPAGAR PATRONES DE TEST
   // =========================================================
   for(int h = 0; h < NUM_PATRONES; h++){

      for(int i = 0; i < NUM_NEURONAS; i++){

         Net = 0;

         for(int j = 0; j < NUM_ENTRADAS; j++){
            Net = fn_propagacion(X[h][j], w[i][j], Net);
         }

         Y[h][i] = fn_salida(fn_activacion(Net));
      }
   }

   // =========================================================
   // CALCULAR ERROR DE TEST
   // =========================================================
   float E[NUM_PATRONES];

   for(int i = 0; i < NUM_PATRONES; i++){

      E[i] = 0;

      for(int j = 0; j < NUM_NEURONAS; j++){
         E[i] += 0.5 * pow(d[i][j] - Y[i][j], 2);
      }
   }

   // =========================================================
   // IMPRIMIR RESULTADOS
   // =========================================================
   printf("\n\n========================================\n");
   printf("         LETRAS RECONOCIDAS TEST\n");
   printf("========================================\n\n");

   int k = 0;
   float errorTotal = 0;

   for(int i = 0; i < NUM_PATRONES; i++){

      printf("\n");

      for(int j = 0; j < NUM_NEURONAS; j++){
         printf("%7.3f  ", Y[i][j]);
      }

      printf(" %c  (%.6f)", letras[k], E[i]);

      if(E[i] > tolerancia)
         printf(" *");

      errorTotal += E[i];

      k++;

      if(k % NUM_NEURONAS == 0){
         k = 0;
         printf("\n--------\n");
      }
   }

   printf("\n\nError total TEST = %.6f\n", errorTotal);

   return 0;
}