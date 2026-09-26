#include <stdio.h>
#include <conio.h>
#include <math.h>

// ============================================================
// PARAMETROS DE LA RED
// ============================================================

#define NUM_PATRONES 21
#define NUM_CLASES   7
#define NUM_ENTRADAS 64
#define MAX_EPOCAS   10000

float alfa = 0.5;
float peso_inicial = 1.5;


// ============================================================
// FUNCIONES DE LA RED
// ============================================================

// Propagacion
float fn_propagacion(int entrada, float peso, float Net){
    return Net + (entrada * peso);
}


// Funcion de activacion bipolar
int fn_activacion(float Net){
    if (Net >= 0)
        return 1;
    else
        return -1;
}


// Funcion de salida
int fn_salida(int fnet){
    return fnet;
}


// Regla de aprendizaje supervisada
float fn_hebb(int Y, int d, int entrada, float peso, float alfa){
    float dif = d - Y;

    return peso + (alfa * entrada * dif);
}


// ============================================================
// PROGRAMA PRINCIPAL
// ============================================================

int main(){
    FILE *archivo;

    archivo = fopen("../Letras/TRAIN.DAT", "r");

    if (archivo == NULL){
        printf("Error al abrir TRAIN.DAT\n");
        return 1;
    }

    char linea[10];

    char letras[NUM_CLASES] = {'A', 'B', 'C', 'D', 'E', 'J', 'K'};


    float w[NUM_CLASES][NUM_ENTRADAS];

    int X[NUM_PATRONES][NUM_ENTRADAS];

    int d[NUM_PATRONES][NUM_CLASES];

    int Y[NUM_PATRONES][NUM_CLASES];


    int f = 0;
    int aux = 0;


    // ========================================================
    // 1. GENERAR MATRIZ DE SALIDAS DESEADAS
    // ========================================================

    int h = 0;
    for (int i = 0; i < NUM_PATRONES; i++){
         for (int j = 0; j < NUM_CLASES; j++){
            if (h == j)
                d[i][j] = 1;
            else
                d[i][j] = -1;
        }
        h++;
        if (h % NUM_CLASES == 0)
            h = 0;
    }


    // ========================================================
    // 2. CARGAR PATRONES DE ENTRENAMIENTO
    // ========================================================
    while (fgets(linea, 10, archivo) != NULL){
        for (int i = 0; i < 7; i++){
            if (linea[i] == '#' || linea[i] == '@')
                X[f][aux] = 1;

            else if (linea[i] == '.' || linea[i] == 'O')
                X[f][aux] = -1;
            aux++;
        }

        if (aux >= 62){
            // Bias
            X[f][63] = 1;

            f++;
            aux = 0;
        }
    }

    fclose(archivo);

    // ========================================================
    // 3. INICIALIZAR PESOS
    // ========================================================

    for (int i = 0; i < NUM_CLASES; i++){
        for (int j = 0; j < NUM_ENTRADAS; j++){
            w[i][j] = peso_inicial;
        }
    }


    // ========================================================
    // 4. MOSTRAR CONFIGURACION
    // ========================================================
    printf("\n");
    printf("============================================\n");
    printf("       ENTRENAMIENTO DE LA RED NEURONAL\n");
    printf("============================================\n\n");

    printf("Numero de patrones : %d\n", NUM_PATRONES);
    printf("Numero de entradas : %d\n", NUM_ENTRADAS);
    printf("Numero de salidas  : %d\n", NUM_CLASES);

    printf("\nParametros de aprendizaje\n");
    printf("--------------------------\n");

    printf("Alfa             : %.4f\n", alfa);
    printf("Peso inicial     : %.4f\n", peso_inicial);
    printf("Maximo de epocas : %d\n\n", MAX_EPOCAS);


    // ========================================================
    // 5. ENTRENAMIENTO
    // ========================================================

    bool entrenada = false;

    int epoca = 1;

    printf("Epoca\tError\t\tErrores\n");
    printf("----------------------------------------\n");

    while (!entrenada && epoca <= MAX_EPOCAS){
        float errorEpoca = 0.0;

        int errores = 0;

        // Recorrer patrones
        for (int h = 0; h < NUM_PATRONES; h++){

            // Recorrer neuronas de salida
            for (int i = 0; i < NUM_CLASES; i++){
                float Net = 0.0;
                // --------------------------------------------
                // PROPAGACION
                // --------------------------------------------
                for (int j = 0; j < NUM_ENTRADAS; j++){
                    Net = fn_propagacion(X[h][j], w[i][j], Net);
                }

                // --------------------------------------------
                // FUNCION DE ACTIVACION
                // --------------------------------------------
                Y[h][i] = fn_salida(fn_activacion(Net));

                // --------------------------------------------
                // CALCULAR ERROR
                // --------------------------------------------
                float diferencia = d[h][i] - Y[h][i];


                errorEpoca +=  0.5 * pow(diferencia, 2);

                // --------------------------------------------
                // ACTUALIZAR PESOS
                // --------------------------------------------
                if (Y[h][i] != d[h][i]){
                    errores++;

                    for (int j = 0; j < NUM_ENTRADAS; j++){
                        w[i][j] = fn_hebb(Y[h][i], d[h][i], X[h][j], w[i][j], alfa);
                    }
                }
            }
        }

        // ====================================================
        // RESULTADO DE LA EPOCA
        // ====================================================

        printf("%3d\t%8.4f\t%d\n", epoca, errorEpoca, errores);

        // Si no hubo errores, la red aprendio
        if (errores == 0){
            entrenada = true;
        }

        epoca++;
    }


    // ========================================================
    // 6. RESULTADO DEL ENTRENAMIENTO
    // ========================================================

    printf("\n============================================\n");
    if (entrenada){
        printf("ENTRENAMIENTO FINALIZADO CORRECTAMENTE\n");
        printf("La red convergio en la epoca %d\n", epoca - 1);
    }
    else{
        printf("SE ALCANZO EL MAXIMO DE EPOCAS\n");
        printf("La red no alcanzo error cero.\n");
    }
    printf("============================================\n");


    // ========================================================
    // 7. IMPRIMIR PESOS FINALES
    // ========================================================

    printf("\n\n");
    printf("============================================\n");
    printf("              PESOS FINALES\n");
    printf("============================================\n");

    for (int i = 0; i < NUM_CLASES; i++){
        printf("\nNeurona %c\n", letras[i]);
        printf("--------------------------------------------\n");
        for (int j = 0; j < NUM_ENTRADAS; j++){
            printf("w[%02d] = %8.3f  ", j, w[i][j]);

            if ((j + 1) % 4 == 0)
                printf("\n");
        }
        printf("\n");
    }


    // ========================================================
    // 8. TEST
    // ========================================================

    printf("\nPulse una tecla para realizar TEST...\n");
    getch();


    FILE *archivo2;
    archivo2 = fopen("../Letras/TEST.DAT", "r");


    if (archivo2 == NULL){
        printf("Error al abrir TEST.DAT\n");
        return 1;
    }


    char linea2[10];
    aux = 0;
    f = 0;


    // ========================================================
    // CARGAR PATRONES TEST
    // ========================================================

    while (fgets(linea2, 10, archivo2) != NULL){
        for (int i = 0; i < 7; i++){
            if (linea2[i] == '#' || linea2[i] == '@')
                X[f][aux] = 1;
            else if (linea2[i] == '.' || linea2[i] == 'O')
                X[f][aux] = -1;
            aux++;
        }

        if (aux >= 62){
            X[f][63] = 1;
            f++;
            aux = 0;
        }
    }

    fclose(archivo2);
    // ========================================================
    // PROPAGAR PATRONES DE TEST
    // ========================================================

    for (int h = 0; h < NUM_PATRONES; h++){
        for (int i = 0; i < NUM_CLASES; i++){
            float Net = 0.0;

            for (int j = 0; j < NUM_ENTRADAS; j++){
                Net =
                    fn_propagacion(X[h][j],w[i][j],Net);
            }

            Y[h][i] = fn_salida(fn_activacion(Net));
        }
    }


    // ========================================================
    // 9. RESULTADOS DEL TEST
    // ========================================================

    printf("\n\n");
    printf("============================================\n");
    printf("          RESULTADOS DEL TEST\n");
    printf("============================================\n\n");

    printf("Patron\tSalida\t\t\tEsperada\tError\n");
    printf("------------------------------------------------------------\n");


    int k = 0;
    float errorTotalTest = 0.0;

    for (int i = 0; i < NUM_PATRONES; i++){
        float error = 0.0;
        printf("%2d\t", i + 1);

        for (int j = 0; j < NUM_CLASES; j++){

            // Para mostrar 0/1
            if (Y[i][j] < 0)
                printf("0 ");
            else
                printf("1 ");

            error += pow(d[i][j] - Y[i][j], 2);
        }

        error *= 0.5;
        errorTotalTest += error;

        printf("\t%c\t\t%.4f",letras[k],error);

        printf("\n");

        k++;

        if (k % NUM_CLASES == 0){
            k = 0;
            printf("------------------------------------------------------------\n");
        }
    }


    // ========================================================
    // 10. ERROR TOTAL TEST
    // ========================================================
    printf("\nError total TEST = %.4f\n",errorTotalTest);
    printf("\nFin del programa.\n");

    return 0;
}