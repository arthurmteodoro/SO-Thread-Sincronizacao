all:
  gcc -Wall -c listadup.c -o listadup.o
  gcc -pthread -Wall listadup.o main.c -o main.out
  
run:
  ./main.out 1
