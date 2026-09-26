#include <stdio.h>
#include <math.h>

// ---------------------------------------------------------
// FUNCIONES DE LA RED
// ---------------------------------------------------------

// Regla de propagacion
double fn_propagacion(double entrada, double peso, double Net)
{
    return Net + (entrada * peso);
}

// Activacion de las neuronas ocultas
double fn_tanh(double Net)
{
    return tanh(Net);
}

// Activacion de la neurona de salida
double fn_sigmoide(double Net)
{
    return 1.0 / (1.0 + exp(-Net));
}


// ---------------------------------------------------------
// MAIN
// ---------------------------------------------------------

int main()
{
    // -----------------------------------------------------
    // PATRONES XOR
    // -----------------------------------------------------

    double X[4][2] ={{0, 1},
                     {1, 0},
                     {0, 0},
                     {1, 1}};

    double d[4] = {1,
                   1,
                   0,
                   0};


    // -----------------------------------------------------
    // PARAMETROS DE ENTRENAMIENTO
    // -----------------------------------------------------

    double alfa = 0.1;
    double tolerancia = 0.01;

    int epoca = 0;
    int max_epocas = 10000;

    bool entrenada = false;


    // -----------------------------------------------------
    // PESOS CAPA ENTRADA -> CAPA OCULTA
    //
    //       x1      x2
    // h1   w[0][0] w[0][1]
    // h2   w[1][0] w[1][1]
    // -----------------------------------------------------

    double w[2][2] ={{ 0.3, -0.2 },
                     { 0.4,  0.1 }};


    // Sesgos de las neuronas ocultas
    double b[2] = {0.0,
                   0.0};

    // -----------------------------------------------------
    // PESOS CAPA OCULTA -> SALIDA
    //
    // h1 -> v[0]
    // h2 -> v[1]
    // -----------------------------------------------------

    double v[2] ={0.2,
                 -0.3};

    // Sesgo de la neurona de salida
    double c = 0.1;


    // Variables auxiliares
    double z[2];       // Net de neuronas ocultas
    double h[2];       // Salidas ocultas

    double NetSalida;
    double y;

    double deltaSalida;
    double deltaOculta[2];


    printf("ENTRENAMIENTO XOR CON BACKPROPAGATION\n");
    printf("------------------------------------\n\n");

    printf("alfa = %.4f\n", alfa);
    printf("tolerancia = %.4f\n\n", tolerancia);

    // =====================================================
    // ENTRENAMIENTO
    // =====================================================
    while (entrenada == false && epoca < max_epocas)
    {
        // -------------------------------------------------
        // RECORRER TODOS LOS PATRONES
        // -------------------------------------------------
        for (int p = 0; p < 4; p++)
        {
            // =============================================
            // 1. FORWARD
            // CAPA DE ENTRADA -> CAPA OCULTA
            // =============================================
            for (int j = 0; j < 2; j++){
                z[j] = 0;
                for (int i = 0; i < 2; i++){
                    z[j] = fn_propagacion(X[p][i], w[j][i], z[j]);
                }

                // Agregar sesgo
                z[j] = z[j] + b[j];

                // Funcion de activacion
                h[j] = fn_tanh(z[j]);
            }


            // =============================================
            // 2. FORWARD
            // CAPA OCULTA -> CAPA DE SALIDA
            // =============================================
            NetSalida = 0;
            for (int j = 0; j < 2; j++){
                NetSalida = fn_propagacion(h[j],v[j],NetSalida);
            }

            // Agregar sesgo
            NetSalida = NetSalida + c;

            // Funcion de activacion
            y = fn_sigmoide(NetSalida);

            // =============================================
            // 3. BACKPROPAGATION
            // DELTA DE LA CAPA DE SALIDA
            // =============================================
            deltaSalida = (y - d[p]) * y * (1.0 - y);

            // =============================================
            // 4. BACKPROPAGATION
            // DELTAS DE LA CAPA OCULTA
            //
            // IMPORTANTE:
            // Se calculan ANTES de modificar v[]
            // =============================================
            for (int j = 0; j < 2; j++){
                deltaOculta[j] = v[j] * deltaSalida * (1.0 - h[j] * h[j]);
            }

            // =============================================
            // 5. ACTUALIZAR PESOS
            // CAPA OCULTA -> SALIDA
            // =============================================
            for (int j = 0; j < 2; j++){
                v[j] = v[j] - alfa * deltaSalida * h[j];
            }

            // Actualizar sesgo de salida
            c = c - alfa * deltaSalida;

            // =============================================
            // 6. ACTUALIZAR PESOS
            // ENTRADA -> CAPA OCULTA
            // =============================================

            for (int j = 0; j < 2; j++){
                for (int i = 0; i < 2; i++){
                    w[j][i] = w[j][i] - alfa * deltaOculta[j] * X[p][i];
                }
                // Actualizar sesgo
                b[j] = b[j] - alfa * deltaOculta[j];
            }

        } // termina recorrido de patrones

        // =================================================
        // 7. CALCULAR ERROR DE LA EPOCA
        // SIN MODIFICAR LOS PESOS
        // =================================================
        double error = 0;

        for (int p = 0; p < 4; p++){
            // ---------- capa oculta ----------
            for (int j = 0; j < 2; j++){
                z[j] = 0;
                for (int i = 0; i < 2; i++){
                    z[j] = fn_propagacion(X[p][i],w[j][i],z[j]);
                }
                z[j] = z[j] + b[j];
                h[j] = fn_tanh(z[j]);
            }

            // ---------- capa salida ----------
            NetSalida = 0;
            for (int j = 0; j < 2; j++){
                NetSalida = fn_propagacion(h[j], v[j], NetSalida);
            }

            NetSalida = NetSalida + c;

            y = fn_sigmoide(NetSalida);

            // ---------- error ----------
            error = error + pow(d[p] - y, 2);
        }

        // Error cuadratico medio
        double MSE = error / 4.0;
        epoca++;

        // Mostrar cada 100 epocas
        if (epoca % 100 == 0){
            printf("Epoca %5i   MSE = %.8f\n", epoca, MSE);
        }


        // Criterio de paro
        if (MSE <= tolerancia){
            entrenada = true;
            printf("\nLISTO\n");
            printf("Epoca = %i\n", epoca);
            printf("MSE   = %.8f\n\n", MSE);
        }

    } // termina while


    // =====================================================
    // PRUEBA DE LA RED
    // =====================================================

    printf("\n");
    printf("PRUEBA DE LA RED\n");
    printf("------------------------------\n");
    printf("x1 x2   esperado   salida\n");
    printf("------------------------------\n");


    for (int p = 0; p < 4; p++){

        // CAPA OCULTA
        for (int j = 0; j < 2; j++){
            z[j] = 0;
            for (int i = 0; i < 2; i++){
                z[j] = fn_propagacion(X[p][i], w[j][i], z[j]);
            }

            z[j] = z[j] + b[j];

            h[j] = fn_tanh(z[j]);
        }


        // CAPA DE SALIDA
        NetSalida = 0;
        for (int j = 0; j < 2; j++){
            NetSalida = fn_propagacion(h[j], v[j], NetSalida);
        }

        NetSalida = NetSalida + c;

        y = fn_sigmoide(NetSalida);

        printf("%.0f  %.0f      %.0f       %.6f", X[p][0],  X[p][1], d[p], y);

        // Clasificacion
        if (y >= 0.5)
            printf(" -> 1\n");
        else
            printf(" -> 0\n");
    }


    // =====================================================
    // MOSTRAR PESOS FINALES
    // =====================================================
    printf("\n\nPESOS FINALES\n");
    printf("------------------------------\n");
    printf("\nEntrada -> Oculta\n");
    printf("w11 = %f   w12 = %f\n", w[0][0], w[0][1]);
    printf("w21 = %f   w22 = %f\n", w[1][0], w[1][1]);

    printf("\nSesgos ocultos\n");
    printf("b1 = %f   b2 = %f\n", b[0], b[1]);

    printf("\nOculta -> Salida\n");
    printf("v1 = %f   v2 = %f\n", v[0], v[1]);

    printf("c = %f\n", c);

    return 0;
}