#include <stdio.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/cdata.h"
#include "../include/cthread.h"
#include "../include/scheduler.h"
#include <stdlib.h>

//gcc -o SemaphoreText semaphoreText.c ../src/thread.c ../src/scheduler.c ../src/lib.c ../bin/support.o -Wall -ggdb && ./semaphoreTest

/*
Semaforo
O func cria threads com secao critica. O programa deve exercutar a secao critica da thread1 ate dar yield, entao executar outra thread
conforme o escalonada. Ao encontrar o cwait da thread3 deve retornar a thread1
*/

int func1();
int func2();
int func3();
int func4();
int func5();
int func6();

int thread1;
int thread2;
int thread3;
int thread4;
int thread5;
int thread6;

csem_t sem;


int main(){
  char * names = malloc(sizeof(int)*200);
  cidentify(names,200);
  printf("Nomes: \n%s", names);
  printf("Wait sem Iniciar: Retorno %d\n",cwait(&sem));
  printf("Signal sem Iniciar: Retorno %d\n",csignal(&sem));

  if(!csem_init(&sem, 1))
    printf("iniciou semaforo \n");
  thread1= ccreate((void*)&func1, 0 , FPRIO_PRIORITY_LOW);
  thread2= ccreate((void*)&func2, 0, FPRIO_PRIORITY_HIGH);
  thread3= ccreate((void*)&func3, 0, FPRIO_PRIORITY_LOW);
  thread4= ccreate((void*)&func4, 0, FPRIO_PRIORITY_MEDIUM);
  thread5= ccreate((void*)&func5, 0, FPRIO_PRIORITY_MEDIUM);
  cjoin(thread6);

  return 0;
}

int func1(){
  printf("funcao 1\n");
  cwait(&sem);
  cyield();
  printf("SC da func1\n");
  csignal(&sem);
  printf("recurso LIBERADO, func1 count:%d\n",sem.count);
  return 0;
}

int func2(){
  printf("funcao 2\n");
  cwait(&sem);
  printf("SC da func2\n");
  csignal(&sem);
  printf("recurso LIBERADO, func2 count:%d\n",sem.count);
  return 0;
}

int func3(){
  printf("funcao 3\n");
  cwait(&sem);
  thread6= ccreate((void*)&func6, 0, FPRIO_PRIORITY_LOW);
  cyield();
  printf("SC da func3\n");
  csignal(&sem);
  printf("recurso LIBERADO, func3 count:%d\n",sem.count);
  return 0;
}
int func4(){
  printf("funcao 4\n");
  cwait(&sem);
  printf("SC da func4\n");
  csignal(&sem);

  printf("recurso LIBERADO, func4 count:%d\n",sem.count);
  return 0;
}
int func5(){
  printf("funcao 5\n");
  cwait(&sem);
  printf("SC da func5\n");
  csignal(&sem);

  printf("recurso LIBERADO, func5 count:%d\n",sem.count);
  return 0;
}
int func6(){
  printf("funcao 6\n");
  cwait(&sem);
  printf("SC da func6\n");
  csignal(&sem);

  printf("recurso LIBERADO, func6 count:%d\n",sem.count);
  return 0;
}