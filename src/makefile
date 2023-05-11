# Inserisco a coppie tutti i .c/.h
# MASTER WORKER
master_c = ./client/farm_master_worker.c
master_h = ../includes/farm_master_worker.h
# POOL
pool_c = ./client/pool.c
pool_h = ../includes/pool.h
# COLLECTOR
collector_c = ./server/farm_collector.c
collector_h = ../includes/farm_collector.h
# CONNECTION
connection_c = ./core/connection.c
connection_h = ../includes/connection.h
# ISREGULAR
isregular_c = ./core/isregular.c
isregular_h = ../includes/isregular.h
# LINKED LIST
linkedlist_c = ./core/linkedlist.c
linkedlist_h = ../includes/linkedlist.h
# TASK QUEUE
task_queue_c = ./core/task_queue.c
task_queue_h = ../includes/task_queue.h
# UTILS
utils_c = ./core/utils.c
utils_h = ../includes/utils.h
threadutils_h = ../includes/threadutils.h

# COMPILAZIONE
CC = gcc
CFLAGS = -g -Wall -pedantic -std=c99 -lm 

.PHONY: clear

farm	: farmProject.c $(master_c) $(master_h) $(collector_c) $(collector_h) $(pool_c) $(pool_h) $(connection_c) $(connection_h) $(isregular_c) $(isregular_h) $(linkedlist_c) $(linkedlist_h) $(task_queue_c) $(task_queue_h) $(utils_c) $(utils_h) $(threadutils_h)
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

generafile	: generafile.c
	$(CC) $(CFLAGS) $^ -o $@

test:	
	make
	./test.sh

clear:	
	rm -r testdir
	rm file*
	rm expected.txt

