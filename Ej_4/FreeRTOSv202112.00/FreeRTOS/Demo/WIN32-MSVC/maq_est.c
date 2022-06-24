


#include <stdlib.h>
#include <stdio.h>
#include <iostream>



uint16_t sgn[] =
{
    #include "sgn.h" 

};

enum
{
    ACUMULAR, PROMEDIAR

};

static void Acumular();
static void Promediar();
static void maquina_estado();

static uint32_t llamados = 0;
static uint32_t Acumulador = 0;
uint8_t N = 5;

static void maquina_estado()
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
