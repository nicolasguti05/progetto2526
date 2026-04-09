#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main() {
    // Inizializzazione numeri casuali
    unsigned int seed = (unsigned int)time(NULL);
    srand(seed);

    int scelta = 0;

    do {
        printf ("\n--- COSESTRANE MENU' ---\n");
        printf ("1) Imposta gioco \n");
        printf ("2) Gioca \n");
        printf ("3) Termina gioco \n");
        printf ("4) Visualizza crediti \n");
        printf ("Scegli un'opzione: ");

        if (scanf("%d", &scelta) != 1) {
            printf ("\nErrore: Inserire un numero valido! \n");
            while (getchar() != '\n'); //pulisce il buffer di input
            continue;
        }

        switch (scelta) {
            case 1:
                imposta_gioco();
                break;
            
            case 2:
                gioca();
                break;
            
            case 3:
                termina_gioco();
                break;

            case 4:
                crediti();
                break;

            default:
                printf(RED"Comando sbagliato, riprova. \n"RESET);
        }
    } while (scelta != 3);

    return 0;
}
