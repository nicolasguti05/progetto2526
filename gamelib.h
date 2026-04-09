#ifndef GAMELIB_H
#define GAMELIB_H

#define RED     "\e[0;31m"
#define GREEN   "\e[0;32m"
#define RESET   "\e[0m"

void imposta_gioco();
void gioca();
void termina_gioco();
void crediti();

enum Tipo_zona {
    bosco, scuola, laboratorio, caverna, strada, giardino, supermercato, centrale_elettrica, deposito_abbandonato, stazione_polizia
};

enum Tipo_nemico {
    nessun_nemico, billi, democane, demotorzone
};

enum Tipo_oggetto {
    nessun_oggetto, bicicletta, maglietta_fuocoinferno, bussola, schitarrata_metallica
};

// struttura del giocatore
struct Giocatore {
    char nome[25];
    int mondo;
    int attacco_pischico;
    int difesa_pischica;
    int fortuna;
    struct Zona_mondoreale* pos_mondoreale;
    struct Zona_soprasotto* pos_soprasotto;
    enum Tipo_oggetto zaino[3];
};

// struttura delle zone
struct Zona_mondoreale {
    enum Tipo_zona tipo;
    enum Tipo_nemico nemico;
    enum Tipo_oggetto oggetto;
    struct Zona_mondoreale* avanti;
    struct Zona_mondoreale* indietro;
    struct Zona_soprasotto* link_soprasotto;
};

struct Zona_soprasotto {
    enum Tipo_zona tipo;
    enum Tipo_nemico nemico;
    struct Zona_soprasotto* avanti;
    struct Zona_soprasotto* indietro;
    struct Zona_mondoreale* link_mondoreale;
};




#endif
