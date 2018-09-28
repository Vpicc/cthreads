
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
	if (!has_init_cthreads) {
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
	insertFilaPrioridades(newThread);
	priorityYield();
	return newThread->tid;
}

int csetprio(int tid, int prio) {
	if (!has_init_cthreads) {
		has_init_cthreads = 1;
		initMainThread();
	}
	if (prio == FPRIO_PRIORITY_HIGH || prio == FPRIO_PRIORITY_MEDIUM || prio == FPRIO_PRIORITY_LOW) {
		setRunningThreadPriority(prio);
		priorityYield();
		return 0;
	} else { // Se nao e uma das prioridades estipuladas, retorna com erro
		return -1;
	}
}

int cyield(void) {
	if (!has_init_cthreads) {
		has_init_cthreads = 1;
		initMainThread();
	}
	return prepareYield();
}

int cjoin(int tid) {
	blockedForThread(tid);
	return -1;
}

int csem_init(csem_t *sem, int count) {
	if(sem == NULL)
		sem = malloc(sizeof(csem_t*));
	
	sem->count=count;
	sem->fila= malloc(sizeof(csem_t*));
	return CreateFila2(sem->fila);
}

int cwait(csem_t *sem) {
	sem->count= sem->count - 1;
	if(sem->count < 0){
		int *retorna = malloc(sizeof(int));
		*retorna = 1;
		TCB_t *thread = blockThread();
		if(AppendFila2(sem->fila, thread)!= 0)
			return ERROR;

		getcontext(&(thread->context));
		if(*retorna == 1){
			*retorna = 0;
			chooseAndRunReadyThread();
		}
		free(retorna);
	}
	return SUCCESS;
}

int csignal(csem_t *sem) {
	sem->count = sem->count + 1;
	if(sem->count < 1){

		if(FirstFila2(sem->fila) != 0)
			return ERROR;

		TCB_t *thread=(TCB_t*)GetAtIteratorFila2(sem->fila);
		unblockThread(thread->tid);
		DeleteAtIteratorFila2(sem->fila);
	}
	return SUCCESS;
}

int cidentify (char *name, int size) {
	strncpy (name, "Victor de Almeida Piccoli Ferreira - 221192\nLucas Cardoso Tavares - 264411\nSamuel Rudnicki - 214871", size);
	return 0;
}


