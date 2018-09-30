CC=gcc
LIB_DIR=../lib
INC_DIR=../include
BIN_DIR=../bin
SRC_DIR=../src

all: supportTest schedulerTest threadTest cjoinTest semaphoreTest

supportTest:
	$(CC) -o supportTest supportTest.c $(BIN_DIR)/support.o -Wall -ggdb

schedulerTest:
	$(CC) -o schedulerTest schedulerTest.c -L../lib -lcthread -Wall

threadTest:
	$(CC) -o threadTest threadTest.c -L../lib -lcthread -Wall

cjoinTest:
	$(CC) -o cjoinTest cjoinTest.c -L../lib -lcthread -Wall

semaphoreTest:
	$(CC) -o semaphoreTest semaphoreTest.c -L../lib -lcthread -Wall

clean:
	rm semaphoreTest schedulerTest cjoinTest threadTest supportTest
