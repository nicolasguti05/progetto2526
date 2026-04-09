#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gamelib.h"

// puntatori alle teste delle liste doppiamente concatenate
static struct Zona_mondoreale* prima_zona_mondoreale = NULL;
static struct Zona_soprasotto* prima_zona_soprasotto = NULL;

// Array di puntatori per gestire fino a 4 giocatori
static struct Giocatore* giocatori[4] = {NULL, NULL, NULL, NULL};

static int mappa_pronta = 0; // diventa 1 quando la mappa è chiusa
static int numero_giocatori = 0;

static void pulisci_schermo();

// Funzioni per imposta_gioco()
// crea giocatori
static void inizializza_giocatori();
static int chiedi_numero_giocatori();
static void crea_giocatori(int indice);
static void applica_bonus(int indice, int *undici_scelto);
// menu per la mappa
static void menu_game_master();
static void genera_mappa();
static void inserisci_zona();
static void cancella_zona();
static void stampa_mappa();
static void stampa_zona_specifica();
static void chiudi_mappa();
static enum Tipo_zona ottieni_tipo_zona_casuale();
static enum Tipo_oggetto ottieni_tipo_oggetto_casuale();
static enum Tipo_nemico ottieni_tipo_nemico_casuale(int mondo, int e_stanza_boss);
static void svuota_gioco();
static void dealloca_giocatori();
static void dealloca_mappa_mondoreale();
static void dealloca_mappa_soprasotto();
static const char* get_nome_zona(enum Tipo_zona t);
static const char* get_nome_nemico(enum Tipo_nemico n);
static const char* get_nome_oggetto(enum Tipo_oggetto o);
static struct Zona_mondoreale* trova_zona_per_indice(int posizione);
static void stampa_dettagli_nodi(struct Zona_mondoreale* zona_mr, int posizione);



void imposta_gioco() {
    pulisci_schermo();

    svuota_gioco();

    printf("--- IMPOSTAZIONI ---\n");
    inizializza_giocatori();

    menu_game_master();
}

static void pulisci_schermo() {
    sleep(2);
    system("clear");
}

static void svuota_gioco() {
    dealloca_giocatori();
    dealloca_mappa_mondoreale();
    dealloca_mappa_soprasotto();

    mappa_pronta = 0; // Resetta lo stato del gioco
}

static void menu_game_master() {
    int scelta_mappa = 0;

    do {
        pulisci_schermo();
        printf("\n--- MENU MAPPA ---\n");
        printf("1) Genera mappa casuale (15 zone)\n");
        printf("2) Inserisci una nuova zona\n");
        printf("3) Cancella una zona\n");
        printf("4) Stampa mappa\n");
        printf("5) Stampa una zona specifica");
        printf("6) Chiudi mappa e termina impostazioni\n");
        printf("Scegli un'opzione: ");

        if (scanf("%d", &scelta_mappa) != 1) {
            printf(RED"ERRORE: Inserire un numero valido.\n"RESET);
            while (getchar() != '\n');
            continue;
        }

        switch (scelta_mappa) {
            case 1:
                genera_mappa();
                printf("Premere INVIO per continuare...\n");
                while (getchar() != '\n');
                getchar();
                break;
            case 2:
                inserisci_zona();
                break;
            case 3:
                cancella_zona();
                break;
            case 4:
                stampa_mappa();
                break;
            case 5:
                stampa_zona_specifica();
                break;
            case 6: 
                chiudi_mappa();
                break;
            default:
                printf("Opzione non valida! Riprova.\n");
                sleep(2);                                   // pulisci_schermo (da inserire)
        }
    }
    while (mappa_pronta == 0);
}


static void inizializza_giocatori() {
    numero_giocatori = chiedi_numero_giocatori();
    int undici_scelto = 0;

    for (int i = 0; i < numero_giocatori; i++) {
        crea_giocatori(i);
        applica_bonus(i, &undici_scelto);
    }
}

static void genera_mappa() {
    dealloca_mappa_mondoreale();
    dealloca_mappa_soprasotto();

    struct Zona_mondoreale* ultima_mr = NULL;
    struct Zona_soprasotto* ultima_ss = NULL;

    // Posizionamento del Demotorzone
    int pos_demotorzone = rand() %15;

    for (int i = 0; i < 15; i++) {
        struct Zona_mondoreale* nuova_mr = (struct Zona_mondoreale*) malloc(sizeof(struct Zona_mondoreale));
        struct Zona_soprasotto* nuova_ss = (struct Zona_soprasotto*) malloc(sizeof(struct Zona_soprasotto));

        if (nuova_mr == NULL || nuova_ss == NULL) {
            printf (RED"Errore: Impossibile allocare memoria per la mappa.\n"RESET);
            exit(1);
        }

        // Il tipo di zona è lo stesso
        enum Tipo_zona tipo_condiviso = ottieni_tipo_zona_casuale();
        nuova_mr->tipo = tipo_condiviso;
        nuova_ss->tipo = tipo_condiviso;

        // Generazione contenuti (il Soprasotto non contiene oggetti)
        nuova_mr->oggetto = ottieni_tipo_oggetto_casuale();

        nuova_mr->nemico = ottieni_tipo_nemico_casuale(0, 0);
        nuova_ss->nemico = ottieni_tipo_nemico_casuale(1, (i == pos_demotorzone));

        // Collegamenti dimensionali
        nuova_mr->link_soprasotto = nuova_ss;
        nuova_ss->link_mondoreale = nuova_mr;

        // Gestione lista doppiamente concatenata
        nuova_mr->avanti = NULL;
        nuova_ss->avanti = NULL;

        if (prima_zona_mondoreale == NULL) {
            nuova_mr->indietro = NULL;
            nuova_ss->indietro = NULL;
            prima_zona_mondoreale = nuova_mr;
            prima_zona_soprasotto = nuova_ss;
        } else {
            nuova_mr->indietro = ultima_mr;
            nuova_ss->indietro = ultima_ss;
            ultima_mr->avanti = nuova_mr;
            ultima_ss->avanti = nuova_ss;
        }

        ultima_mr = nuova_mr;
        ultima_ss = nuova_ss;
    }
    printf (GREEN"\nLa mappa è stata generata con successo! (15 zone create).\n"RESET);
}

static void inserisci_zona() {

}

static void cancella_zona() {

}

static void stampa_mappa() {
    int scelta_mondo;
    printf ("\nQuale zona si vuole stampare?\n");
    printf ("1) Mondo Reale\n");
    printf ("2) Soprasotto\n");
    printf ("Scelta: ");
    scanf ("%d", &scelta_mondo);

    if (scelta_mondo == 1) {
        struct Zona_mondoreale* corrente = prima_zona_mondoreale;
        int i = 1;
        printf ("\n--- MAPPA MONDOREALE ---\n");
        while(corrente != NULL) {
            printf ("Zona %d [%s] - Nemico: %s - Oggetto: %s\n",
            i++, get_nome_zona(corrente->tipo),
            get_nome_nemico(corrente->nemico),
            get_nome_oggetto(corrente->oggetto));
            corrente = corrente->avanti; // Scorrimento lista
        }
    } else if (scelta_mondo == 2) {
        struct Zona_soprasotto* corrente = prima_zona_soprasotto;
        int i = 1;
        printf ("\n--- MAPPA SOPRASOTTO ---\n");
        while(corrente != NULL) {
            printf ("Zona %d [%s] - Nemico: %s\n",
            i++, get_nome_zona(corrente->tipo),
            get_nome_nemico(corrente->nemico));
            corrente = corrente->avanti;
        }
    } else {
        printf ("Scelta non valida.\n");
    }
    printf ("\nPremere INVIO per tornare al menu...\n");
    while (getchar() != '\n');
    getchar();
}

static void stampa_zona_specifica() {

}

static void chiudi_mappa() {
    int contatore_zone = 0;
    int contatore_demotorzone = 0;

    struct Zona_soprasotto* corrente = prima_zona_soprasotto;

    while (corrente != NULL) {
        contatore_zone++;
        if (corrente->nemico == demotorzone) {
            contatore_demotorzone++;
        }
        corrente = corrente->avanti;
    }

    // Verifica dei requisiti
    if (contatore_zone < 15) {
        printf("La mappa deve avere minimo 15 zone (attuali: %d).\n", contatore_zone);
        mappa_pronta = 0;
    }
    else if (contatore_demotorzone != 1) {
        printf("Deve esserci esattamente 1 Demotorzone (attuali: %d).\n", contatore_demotorzone);
        mappa_pronta = 0;
    }
    else {
        printf("Mappa validata con successo. %d zone confermate.\n", contatore_zone);
        mappa_pronta = 1;
    }
    pulisci_schermo();
}

// Funzioni per inizializza_giocatori()
// Chiede il numero di giocatori
static int chiedi_numero_giocatori() {
    int num;

    do {
        printf ("Quanti eroi sfideranno il Soprasotto? (1-4) : \n");
        if (scanf("%d", &num) != 1 || num < 1 || num > 4) {
            printf ("ERRORE: Inserire un numero tra 1 e 4.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    } while (1);
    return num;
}

// Alloca memoria e imposta le statistiche base
static void crea_giocatori(int i) {
    giocatori[i] = (struct Giocatore*) malloc(sizeof(struct Giocatore));
    if (giocatori[i] == NULL) {
        printf (RED"ERRORE: Allocazione memoria fallita!\n"RESET);
        exit(1);
    }

    printf ("\nNome del giocatore %d: ", i+1);
    scanf ("%s", giocatori[i]->nome);

    // Lancio dadi per le abilità
    giocatori[i]->attacco_pischico = (rand() %20) + 1;
    giocatori[i]->difesa_pischica = (rand() %20) + 1;
    giocatori[i]->fortuna = (rand() %20) + 1;
    giocatori[i]->mondo = 0; // Inizia nel mondo reale

    // Zaino vuoto all'inizio
    for (int j = 0; j < 3; j++) {
        giocatori[i]->zaino[j] = nessun_oggetto;
    }
}

// Gestisce le scelte dei giocatori per i bonus
static void applica_bonus(int i, int *undici_scelto) {
    int scelta;
    printf ("Scegli un bonus per %s (Att:%d Dif:%d Fort:%d):\n", giocatori[i]->nome, giocatori[i]->attacco_pischico, giocatori[i]->difesa_pischica, giocatori[i]->fortuna);
    printf ("1) +3 Attacco / -3 Difesa\n");
    printf ("2) -3 Attacco / +3 Difesa\n");
    if (!(*undici_scelto)) {
        printf ("3) Diventa 'UndiciVirgolaCinque' (+4 Attacco / +4 Difesa / -7 Fortuna)\n");
    }
    printf("4) Nessuna scelta\n");
    printf("Scelta: ");

    scanf("%d", &scelta);

    if (scelta == 1) {
        giocatori[i]->attacco_pischico += 3;
        giocatori[i]->difesa_pischica -= 3;
    } else if (scelta == 2) {
        giocatori[i]->attacco_pischico -= 3;
        giocatori[i]->difesa_pischica += 3;
    } else if (scelta == 3 && !(*undici_scelto)) {
        giocatori[i]->attacco_pischico += 3;
        giocatori[i]->difesa_pischica -= 3;
        giocatori[i]->fortuna -= 7;
        strcpy(giocatori[i]->nome, "UndiciVirgolaCinque");
        *undici_scelto = 1;
    }
}

// Funzioni per svuota_mappa()
// Svuota l'array dei giocatori
static void dealloca_giocatori() {
    for (int i = 0; i < 4; i++) {
        if (giocatori[i] != NULL) {
            free(giocatori[i]);
            giocatori[i] = NULL;
        }
    }
}

// Svuota la lista del Mondo Reale
static void dealloca_mappa_mondoreale() {
    struct Zona_mondoreale* corr = prima_zona_mondoreale;
    while (corr != NULL) {
        struct Zona_mondoreale* temp = corr;
        corr = corr->avanti;
        free(temp);
    }
    prima_zona_mondoreale = NULL;
}

// Svuota la lista del Soprasotto
static void dealloca_mappa_soprasotto() {
    struct Zona_soprasotto* corr = prima_zona_soprasotto;
    while (corr != NULL) {
        struct Zona_soprasotto* temp = corr;
        corr = corr->avanti;
        free(temp);
    }
    prima_zona_soprasotto = NULL;
}

static enum Tipo_zona ottieni_tipo_zona_casuale() {
    return (enum Tipo_zona)(rand() % 10);
}

static enum Tipo_oggetto ottieni_tipo_oggetto_casuale() {
    int rand_ogg = rand() % 100;
    if (rand_ogg < 40) return nessun_oggetto;
    if (rand_ogg < 55) return bicicletta;
    if (rand_ogg < 70) return maglietta_fuocoinferno;
    if (rand_ogg < 85) return bussola;
    return schitarrata_metallica;
}

// Genera un nemico rispettando le regole dei mondi
// Mondo : 0 = Mondo reale, 1 = Soprasotto
static enum Tipo_nemico ottieni_tipo_nemico_casuale(int mondo, int e_stanza_boss) {
    // Il demotorzone è solo nel Soprasotto e solo in una stanza
    if (mondo == 1 && e_stanza_boss) {
        return demotorzone;
    }

    int rand_nem = rand() % 100;

    if (mondo == 0) {
        if (rand_nem < 50) return nessun_nemico;
        if (rand_nem < 80) return billi;
        return democane;
    } else {
        if (rand_nem < 40) return nessun_nemico;
        return democane;
    }
}

static const char* get_nome_zona(enum Tipo_zona t) {
    switch(t) {
        case bosco: return "Bosco";
        case laboratorio: return "Laboratorio";
        case scuola: return "Scuola";
        case caverna: return "Caverna";
        case strada: return "Strada";
        case giardino: return "Giardino";
        case supermercato: return "Supermercato";
        case centrale_elettrica: return "Centrale elettrica";
        case deposito_abbandonato: return "Deposito abbandonato";
        case stazione_polizia: return "Stazione polizia";
        default: return "Sconosciuta";
    }
}

static const char* get_nome_nemico(enum Tipo_nemico n) {
    switch(n) {
        case nessun_nemico: return "Nessuno";
        case billi: return "Billi";
        case democane: return "Democane";
        case demotorzone: return "Demotorzone";
        default: return "Sconosciuto";
    }
}

static const char* get_nome_oggetto(enum Tipo_zona o) {
    switch(o) {
        case nessun_oggetto: return "Nessuno";
        case bicicletta: return "Bicicletta";
        case maglietta_fuocoinferno: return "Maglietta Fuocoinferno";
        case bussola: return "Bussola";
        case schitarrata_metallica: return "Schitarrata Metallica";
        default: return "Sconosciuto";
    }
}

// Scorre la lista e restituisce il puntatore alla zona trovata
// Se la posizione non esiste restituisce NULL
static struct Zona_mondoreale* trova_zona_per_indice(int posizione) {
    struct Zona_mondoreale* corrente = prima_zona_mondoreale;
    int indice_corrente = 1;

    while (corrente != NULL && indice_corrente < posizione) {
        corrente = corrente->avanti;
        indice_corrente++;
    }
    return corrente;
}

// Prende in input il puntatore alla zona e la stampa
static void stampa_dettagli_nodi(struct Zona_mondoreale* zona_mr, int posizione) {
    struct Zona_soprasotto* gemella_ss = zona_mr->link_soprasotto;

    printf ("[MONDO REALE]\n");
    printf ("- Tipo zona: %s\n", get_nome_zona(zona_mr->tipo));
    printf ("- Nemico: %s\n", get_nome_nemico(zona_mr->nemico));
    printf ("- Oggetto: %s\n", get_nome_oggetto(zona_mr->oggetto));

    printf ("\n[SOPRASOTTO]\n");
    if (gemella_ss != NULL) {
        printf ("- Tipo zona: %s\n", get_nome_zona(gemella_ss->tipo));
        printf ("- Nemico: %s\n", get_nome_nemico(gemella_ss->nemico));
    } else {
        printf (RED"- Errore Critico: collegamento dimensionale mancante!\n"RESET);
    }
}
