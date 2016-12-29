/*=======================================================================*/
/*           Trabalho de AED II - Indice remissivo com árvores           */
/* Nome: Arthur Alexsander Martins Teodoro                               */
/* MAtrícula: 0022427         Data: 23/11/2016                           */
/*=======================================================================*/

/*=======================================================================*/
/*                        BIBLIOTECAS USADAS                             */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "listadup.h"

/*=======================================================================*/
/*                         ESTRUTURA CRIADA                              */
/*=======================================================================*/
typedef struct
{
  int id;
  int quant;
} arg;

/*=======================================================================*/
/*                   DEFINIÇÕES E VARIÁVEIS GLOBAIS                      */
/*=======================================================================*/
#define quantidadeThread 6
enum func{Walter, Pinkman, Xavier, Jean, Girafales, Florinda}funcionario;

/*=======================================================================*/
/*                         VARIAVEIS GLOBAIS                             */
/*=======================================================================*/
int esperando[quantidadeThread];
int quantidadeEstacionamento = 0;
int deadlock = 0;
int ativas = 0;
Lista fila;
pthread_mutex_t mutexAtivas = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexDeadlock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t vaga = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waiting[quantidadeThread] = PTHREAD_COND_INITIALIZER; 

/*=======================================================================*/
/*GERA NOME - FUNCAO QUE RETORNA O NOME CORESPONDENTE AO ID              */
/*IN: IDENTIFICADOR DA THREAD   OUT: NOME DA THREAD                      */
/*=======================================================================*/
char* geraNome(int id)
{
  switch(id)
  {
    case 0:
      return "Walter";
    case 1:
      return "Pinkman";
    case 2:
      return "Xavier";
    case 3:
      return "Jean";
    case 4:
      return "Girafales";
    case 5:
      return "Florinda";
  }
  return NULL;
}

/*=======================================================================*/
/*QUEM CHEGOU PRIMEIRO - FUNCAO QUE RETORNA O PRIMEIRO DO GRUPO          */
/*IN: IDENTIFICADOR DA THREAD   OUT: GRUPO DE TRABALHADORES              */
/*=======================================================================*/
int quemChegouPrimeiro(int grupo)
{
  int i = 0;
  int cont = 0;
  for(i = 0; cont != 6; i = (i+1)%6)
  {
    /*grupo 1 = Walter e Pinkman*/
    if(grupo == 1)
    {
      if(valorPosLista(fila, i) == Walter)
        return Walter;
      if(valorPosLista(fila, i) == Pinkman)
        return Pinkman;
    }
    /*grupo 2 = Xavier e Jean*/
    else if(grupo == 2)
    {
      if(valorPosLista(fila, i) == Xavier)
        return Xavier;
      if(valorPosLista(fila, i) == Jean)
        return Jean;
    }
    /*grupo 3 = Girafales e Florinda*/
    else if(grupo == 3)
    {
      if(valorPosLista(fila, i) == Girafales)
        return Girafales;
      if(valorPosLista(fila, i) == Florinda)
        return Florinda;
    }
    cont++;
  }
  return -1;
}

/*=======================================================================*/
/*PROXIMO - FUNCAO QUE RETORNA O PROXIMO A USAR A VAGA                   */
/*IN: IDENTIFICADOR DA THREAD   OUT: IDENTIFICADOR DA PROXIMA THREAD     */
/*=======================================================================*/
int proximo(int id)
{
  /*caso der um deadlock, avisa que deu deadlock e vai embora*/
  if( (esperando[Walter] || esperando[Pinkman]) && (esperando[Xavier] || esperando[Jean]) && 
      (esperando[Girafales] || esperando[Florinda]) )
  {
    pthread_mutex_lock(&mutexDeadlock);
    deadlock = 1;
    pthread_mutex_unlock(&mutexDeadlock);
    return -1;
  }

  /*caso nao deu deadlock, procura o proximo segundo meu id*/
  pthread_mutex_lock(&mutexDeadlock);
  deadlock = 0;
  pthread_mutex_unlock(&mutexDeadlock);

  if(id == Girafales)
  {

    if(esperando[Xavier] && esperando[Jean])
      return quemChegouPrimeiro(2);

    if(esperando[Xavier])
      return Xavier;

    if(esperando[Jean])
      return Jean;

    if(esperando[Walter] && esperando[Pinkman])
      return quemChegouPrimeiro(1);

    if(esperando[Walter])
      return Walter;

    if(esperando[Pinkman])
      return Pinkman;

    if(esperando[Florinda])
      return Florinda;
  }

  if(id == Florinda)
  {

    if(esperando[Xavier] && esperando[Jean])
      return quemChegouPrimeiro(2);

    if(esperando[Xavier])
      return Xavier;

    if(esperando[Jean])
      return Jean;

    if(esperando[Walter] && esperando[Pinkman])
      return quemChegouPrimeiro(1);

    if(esperando[Walter])
      return Walter;

    if(esperando[Pinkman])
      return Pinkman;

    if(esperando[Girafales])
      return Girafales;
  }

  if(id == Xavier)
  {
    if(esperando[Walter] && esperando[Pinkman])
      return quemChegouPrimeiro(1);

    if(esperando[Walter])
      return Walter;

    if(esperando[Pinkman])
      return Pinkman;

    if(esperando[Girafales] && esperando[Florinda])
      return quemChegouPrimeiro(3);

    if(esperando[Girafales])
      return Girafales;

    if(esperando[Florinda])
      return Florinda;

    if(esperando[Jean])
      return Jean;
  }

  if(id == Jean)
  {
    if(esperando[Walter] && esperando[Pinkman])
      return quemChegouPrimeiro(1);

    if(esperando[Walter])
      return Walter;

    if(esperando[Pinkman])
      return Pinkman;

    if(esperando[Girafales] && esperando[Florinda])
      return quemChegouPrimeiro(3);

    if(esperando[Girafales])
      return Girafales;

    if(esperando[Florinda])
      return Florinda;

    if(esperando[Xavier])
      return Xavier;
  }

  if(id == Walter)
  {
    if(esperando[Girafales] && esperando[Florinda])
      return quemChegouPrimeiro(3);

    if(esperando[Girafales])
      return Girafales;

    if(esperando[Florinda])
      return Florinda;

    if(esperando[Xavier] && esperando[Jean])
      return quemChegouPrimeiro(2);

    if(esperando[Xavier])
      return Xavier;

    if(esperando[Jean])
      return Jean;

    if(esperando[Pinkman])
      return Pinkman;
  }

  if(id == Pinkman)
  {
    if(esperando[Girafales] && esperando[Florinda])
      return quemChegouPrimeiro(3);

    if(esperando[Girafales])
      return Girafales;

    if(esperando[Florinda])
      return Florinda;

    if(esperando[Xavier] && esperando[Jean])
      return quemChegouPrimeiro(2);

    if(esperando[Xavier])
      return Xavier;

    if(esperando[Jean])
      return Jean;

    if(esperando[Walter])
      return Walter;
  }

  return -1;

}

/*=======================================================================*/
/*ESPERAR VAGA - FUNCAO QUE FAZ ESPERAR PELA VAGA                        */
/*IN: IDENTIFICADOR DA THREAD   OUT: VOID                                */
/*=======================================================================*/
void esperarVaga(int id)
{ 
  pthread_mutex_lock(&lock);

  /*marca intenção e se coloca na fila*/
  esperando[id] = 1;
  insereLista(fila, tamanhoLista(fila), id);

  quantidadeEstacionamento++;
  if(quantidadeEstacionamento == 1)/*unico carro no estacionamento*/
    pthread_cond_signal(&waiting[id]);
  else
    pthread_cond_wait(&waiting[id], &lock);

  pthread_mutex_unlock(&lock);
}

/*=======================================================================*/
/*LIBERAR VAGA - FUNCAO QUE FAZ LIBERAR A VAGA                           */
/*IN: IDENTIFICADOR DA THREAD   OUT: VOID                                */
/*=======================================================================*/
void liberarVaga(int id)
{ 
  pthread_mutex_lock(&lock);
  
  /*retira intenção e se tira da fila*/
  esperando[id] = 0;
  quantidadeEstacionamento--;

  retiraListaPos(fila, buscaPosLista(fila, id));

  if(quantidadeEstacionamento != 0)
  {
    int prox = proximo(id);
    
    pthread_cond_signal(&waiting[prox]);
  }

  pthread_mutex_unlock(&lock);
}

/*=======================================================================*/
/*ESTACIONAR VAGA - FUNCAO QUE FAZ A THREAD ESTACIONAR                   */
/*IN: IDENTIFICADOR DA THREAD   OUT: VOID                                */
/*=======================================================================*/
void estacionarVaga(int id)
{
  printf("%s estaciona para trabalhar\n", geraNome(id));
  sleep(1);
}

/*=======================================================================*/
/*VAI ESTUDAR - FUNCAO QUE FAZ A THREAD IR ESTUDAR                       */
/*IN: IDENTIFICADOR DA THREAD   OUT: VOID                                */
/*=======================================================================*/
void vaiEstudar(int id)
{
  printf("%s vai para casa estudar\n", geraNome(id));
  sleep((rand()%3)+3);
}

/*=======================================================================*/
/*TAREFA - FUNCAO QUE AS THREADS VAO REALIZAR                            */
/*IN: ARGUMENTOS   OUT: PONTEIRO DA FUNCAO                               */
/*=======================================================================*/
void* tarefa(void* args)
{
  /*converte o argumento*/
  arg* Args = (arg*) args;

  pthread_mutex_lock(&mutexAtivas);
  ativas++;
  pthread_mutex_unlock(&mutexAtivas);

  while(Args->quant > 0)
  {
    printf("%s quer usar a vaga\n", geraNome(Args->id));
    
    esperarVaga(Args->id);

    pthread_mutex_lock(&vaga);

    estacionarVaga(Args->id);
  
    liberarVaga(Args->id);

    pthread_mutex_unlock(&vaga);

    vaiEstudar(Args->id);

    Args->quant--;
  }

  pthread_mutex_lock(&mutexAtivas);
  ativas--;
  pthread_mutex_unlock(&mutexAtivas);

}

/*=======================================================================*/
/*DIRETOR - FUNCAO QUE O DIRETOR VAI REALIZAR                            */
/*IN: ARGUMENTO   OUT: PONTEIRO PARA FUNCAO                              */
/*=======================================================================*/
void* diretor(void* args)
{
  int roda = 1;
  int teveDeadlock = 0;

  /*pega a quantidade de thread ativas*/
  pthread_mutex_lock(&mutexAtivas);
  roda = ativas;
  pthread_mutex_unlock(&mutexAtivas);

  while(roda != 0)
  {
    sleep(5);

    pthread_mutex_lock(&mutexDeadlock);
    teveDeadlock = deadlock;
    pthread_mutex_unlock(&mutexDeadlock);

    /*caso deu deadlock, randomiza um ate pegar um que quer*/
    if(teveDeadlock)
    {
      int vai = rand()%6;
      while(esperando[vai] != 1)
      {
        vai = rand()%6;
      }

      pthread_mutex_lock(&mutexDeadlock);
      deadlock = 0;
      pthread_mutex_unlock(&mutexDeadlock);
     
      printf("Diretor detectou um deadlock, liberando %s\n", geraNome(vai));
    
      pthread_cond_signal(&waiting[vai]);
    }

    pthread_mutex_lock(&mutexAtivas);
    roda = ativas;
    pthread_mutex_unlock(&mutexAtivas);
  }

}

/*=======================================================================*/
/*MAIN - FUNCAO PRINCIPAL DO PROGRAMA                                    */
/*IN: ARGUMENTOS   OUT: RETORNO PARA O S.0                               */
/*=======================================================================*/
int main(int argc, char const *argv[])
{
    
  if(argc != 2)
  {
    fprintf(stderr, "ERRO\n");
    exit(1);
  }

  srand(time(NULL));

  fila = criaLista();

  int i = 0;
  pthread_t threads[quantidadeThread];
  pthread_t threadDiretor;
  arg threadArgumentos[quantidadeThread];

  for(i = 0; i < quantidadeThread; i++)
  {
    threadArgumentos[i].id = i;
    threadArgumentos[i].quant = atoi(argv[1]);
    pthread_create(&threads[i], NULL, tarefa, (void*)&threadArgumentos[i]);
  }

  pthread_create(&threadDiretor, NULL, diretor, NULL);

  /*termina as threads*/
  for(i = 0; i < quantidadeThread; i++)
  {
    pthread_join(threads[i], NULL);
  }

  pthread_join(threadDiretor, NULL);

  destroiLista(fila);

  return 0;
}