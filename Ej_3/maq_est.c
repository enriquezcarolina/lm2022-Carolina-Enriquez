


#include <stdlib.h>
#include <stdio.h>
#include <iostream>


uint16_t sgn[] =
{
  0, 51, 102, 153, 204, 255, 306, 357, 408, 459, 510, 561, 612, 663, 714, 765, 816, 867, 918, 969, 1020, 1071, 1122, 1173, 1224, 1275, 1326, 1377, 1428, 1479, 1530, 1581, 1632, 1683, 1734, 1785, 1836, 1887, 1938, 1989, 2040, 2091, 2142, 2193, 2244, 2295, 2346, 2397, 2448, 2499, 2550, 2601, 2652, 2703, 2754, 2805, 2856, 2907, 2958, 3009, 3060, 3111, 3162, 3213, 3264,
};

enum
{
    ACUMULAR, PROMEDIAR

};

static void Acumular();
static void Promediar();

static uint32_t llamados = 0;
static uint32_t Acumulador = 0;
uint8_t N = 5;

int main()
{
    uint8_t estado = ACUMULAR; // por default arranco con el acumulador

    printf("--------inicio de programa------- \n");

    while (1)
    {
      
        switch (estado)
        {

        case ACUMULAR:
            Acumular();
            if (llamados >= N)
            {
                
                estado = PROMEDIAR;

            }
            else
            {
                estado = ACUMULAR;
            }
            break;

        case PROMEDIAR:
            Promediar();
            estado = ACUMULAR;
            break;


        }
        
    }

    printf("------ Fin del programa------");

    return 0;
}

static void Acumular()
{

    Acumulador = Acumulador + sgn[llamados];
    llamados++;
    return;
}

static void Promediar()
{

    float Prom = Acumulador / N;
    printf("--------- PROMEDIO ----------");
    
    printf("%f\n", Prom);
    
    Acumulador = 0;
    llamados = 0;
   
    printf("--------Elija un Distinto N------- \n");
    scanf("%d", &N);
    if (N > 64)
    {
        N = 0; // como se encuentra fuera del largo seteo un valor por default 
    }

    return;
}
