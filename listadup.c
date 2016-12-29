#include <stdlib.h>
#include <stdio.h>
#include "listadup.h"

struct no
{
	int valor;
	struct no *prox;
	struct no *ant;
};

struct listadup
{
	int tam;
	struct no *inicio;
	struct no *fim;
};

typedef struct no *Nodo;

Lista criaLista(void)
{
	Lista p = (Lista) malloc(sizeof(struct listadup));
	p->tam = 0;
	p->inicio = NULL;
	p->fim = NULL;
	return p;
};

int insereLista(Lista p,int pos ,int valor)
{
	//cria um no e coloca seu valor
	Nodo no = (Nodo) malloc(sizeof(struct no));
	no->valor = valor;
	//caso for o primeiro numero a ser inserido
	if(p->tam == 0)
	{
		p->inicio = no;
		p->fim = no;
		no->prox = NULL;
		no->ant = NULL;
		p->tam++;
		return p->tam;
	}
	//se a posicao for a maior que o tamanho = inserir no fim
	if((pos+1) >= p->tam)
	{
		no->prox = NULL;
		Nodo aux = p->fim;
		aux->prox = no;
		no->ant = aux;
		p->fim = no;
		p->tam++;
		return p->tam;
	}
	//inserir no inicio da lista
	if(pos <= 0)
	{
		no->ant = NULL;
		Nodo aux = p->inicio;
		aux->ant = no;
		no->prox = aux;
		p->inicio = no;
		p->tam++;
		return p->tam;
	}
	//caso normal, inserir no meio da lista
	//primeiro tem que colocar o ponteiro aux no lugar que quero inserir
	Nodo aux = p->inicio;
	for(aux=p->inicio; pos != 0; aux=aux->prox)
	{
		pos--;
	}
	//depois de colocar no aux no lugar eh so mudar os ponteiros
	no->ant = aux->ant;
	no->prox = aux;
	aux->ant->prox = no;
	aux->ant = no;
	p->tam++;
	return p->tam;
};

int vaziaLista(Lista p)
{
	return(p->tam == 0);
};

int retiraListaPos(Lista p, int pos)
{
	//printf("============== RETIRAR LISTA POS %d ================\n", pos);
	if(!vaziaLista(p))
	{
		//printf("========== LISTA ==============\n");
		//printaLista(p, stdout);
		//cria um ponteiro auxiliar
		Nodo aux = NULL;
		//caso especial, existe so uma posicao;
		if(p->tam == 1)
		{
			//printf("================= RETIRAR UNICA POSICAO ==============\n");
			aux = p->inicio;
			free(aux);
			p->inicio = NULL;
			p->fim = NULL;
			p->tam--;
			return p->tam;
		};
		//caso especial, tirar a primeira posicao;
		if(pos == 0)
		{
			//printf("================= RETIRAR PRIMEIRA POSICAO ==============\n");
			aux = p->inicio;
			p->inicio = aux->prox;
			aux->prox->ant = aux->ant;
			free(aux);
			p->tam--;
			return p->tam;
		};
		//caso especial, tirar a ultima posicao;
		if((pos+1) >= p->tam)
		{
			//printf("================= RETIRAR ULTIMA POSICAO ==============\n");
			aux = p->fim;
			p->fim = aux->ant;
			aux->ant->prox = aux->prox;
			free(aux);
			p->tam--;
			return p->tam;
		};
		//caso normal, retirar do meio
		//printf("================= RETIRAR POSICAO NORMAL ==============\n");
		aux = p->inicio;
		for(aux=p->inicio; pos != 0; aux=aux->prox)
		{
			pos--;
		};
		aux->ant->prox = aux->prox;
		aux->prox->ant = aux->ant;
		free(aux);
		p->tam--;
		return p->tam;
	}
	else
	{
		return -1;
	}
};

void printaLista(Lista l, FILE* arquivo)
{
	Nodo aux = l->inicio;
	while(aux != NULL)
	{
		fprintf(arquivo, "%d  ", aux->valor);
		aux = aux->prox;
	};
	fprintf(arquivo, "\n");
};

void destroiLista(Lista l)
{
	while(!vaziaLista(l))
	{
		retiraListaPos(l,0);
	}
	free(l);
};

void mudaValorLista(Lista l, int pos, int valor)
{
	if(vaziaLista(l))
	{
		return;
	}
	if(l->tam > pos)
	{
		Nodo aux = l->inicio;
		for(aux = l->inicio; pos != 0; aux = aux->prox)
		{
			pos--;
		};
		aux->valor = valor;
	}
	return;
};

int tamanhoLista(Lista l)
{
	 return l->tam;
}

int existeLista(Lista l, int valor)
{
	Nodo aux = l->inicio;
	while(aux != NULL)
	{
		if(aux->valor == valor)
			return 1;
		aux = aux->prox;
	}
	return 0;
}

int valorPosLista(Lista l, int pos)
{
	Nodo aux = l->inicio;
	for(aux = l->inicio; pos != 0; aux = aux->prox)
	{
		pos--;
	}
	return aux->valor;
}

int buscaPosLista(Lista l, int valor)
{
	Nodo aux;
	int pos = 0;
	for(aux = l->inicio; aux != NULL; aux = aux->prox)
	{
		if(aux->valor == valor)
			return pos;
		pos++;
	}
	return -1;
}