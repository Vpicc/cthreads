
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/thread.h"
#include "../include/cdata.h"
#include "../include/scheduler.h"
#include "../include/cthread.h"

int has_init_cthreads = 0;//flag



int ccreate (void* (*start)(void*), void *arg, int prio) {
/*inicializa a main thread caso n tenha sido inicializada ainda*/
	if(!has_init_cthreads){
		has_init_cthreads = 1;
		initMainThread();
	}
	if(start == NULL)
		return ERROR;
	/*Criação do contexto*/	
	ucontext_t *newThreadContext = malloc(sizeof(ucontext_t));
	createContext(newThreadContext, start);

	/*Criação do TCB*/
	TCB_t *newThread = malloc(sizeof(TCB_t));
	newThread = createThread(*newThreadContext, prio);

	return insertFilaPrioridades(newThread);
}

int csetprio(int tid, int prio) {
	return -1;
}

int cyield(void) {
	return -1;
}

int cjoin(int tid) {
	return -1;
}

int csem_init(csem_t *sem, int count) {
	return -1;
}

int cwait(csem_t *sem) {
	return -1;
}

int csignal(csem_t *sem) {
	return -1;
}

int cidentify (char *name, int size) {
	strncpy (name, "Victor de Almeida Piccoli Ferreira - 221192\nLucas Cardoso Tavares - 264411\nSamuel Rudnicki - 214871", size);
	return 0;
}


