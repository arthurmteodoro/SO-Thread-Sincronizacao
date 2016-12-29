#ifndef _LISTA_DUP_LA_
#define _LISTA_DUP_LA_

typedef struct listadup *Lista;

extern Lista criaLista(void);
extern int insereLista(Lista p,int pos ,int valor);
extern int vaziaLista(Lista p);
extern int retiraListaPos(Lista p, int pos);
extern void printaLista(Lista l, FILE* arquivo);
extern void destroiLista(Lista l);
extern void mudaValorLista(Lista l, int pos, int valor);
extern int tamanhoLista(Lista l);
extern int existeLista(Lista l, int valor);
extern int valorPosLista(Lista l, int pos);
extern int buscaPosLista(Lista l, int valor);

#endif