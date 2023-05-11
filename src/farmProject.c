/*
    Created by Jacopo Cioni
    FarmProject - SOL
*/

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>

#include"../includes/utils.h"
#include"../includes/linkedlist.h"
#include"../includes/isregular.h"
#include"../includes/farm_collector.h"
#include"../includes/farm_master_worker.h"
#include"../includes/threadutils.h"
#include"../includes/connection.h"

volatile sig_atomic_t force = 0;

static void interrupt(int signum) {
    force = 1;
}

// Funzione eseguita dal signalhandler thread
static void *sigHandler(void *args) {
    // Passo il set di segnali che si trova nell'argomento
    sigset_t *set = (sigset_t *)args;
    int sig;

    // Uso la sigwait per attendere i segnali, NOTA: Devono essere mascherati
    for(;;) {
        int r = sigwait(set, &sig);
        if(r != 0) {
            errno = r;
            perror("sigwait");
            return NULL;
        }
        // Faccio lo switch su sig
        switch(sig) {
            case SIGUSR1: {
                int fd_farm = clientSocket();
                int length = 6;
                int n;
                if((n = writen(fd_farm, &length, sizeof(int))) == -1) {
                    fprintf(stderr, "Errore writen master.\n");
                    return NULL;
                }
                if((n = writen(fd_farm, "SIGNAL", length*sizeof(char))) == -1) {
                    fprintf(stderr, "Errore writen master.\n");
                    return NULL;
                }
                close(fd_farm);
                continue;
            }
            case SIGINT: {
                fprintf(stdout, "Ricevuto SIGINT.\n");
                interrupt(sig);
                continue;
            }
            case SIGTERM: {
                fprintf(stdout, "Ricevuto SIGTERM.\n");
                interrupt(sig);
                continue;
            }
            case SIGQUIT: {
                fprintf(stdout, "Ricevuto SIGQUIT.\n");
                interrupt(sig);
                continue;
            }
            case SIGHUP: {
                fprintf(stdout, "Ricevuto SIGHUP.\n");
                interrupt(sig);
                continue;
            }
            default: ;
        }
    }
    return NULL;
}

int main (int argc, char *argv[]) {

    // Numero di threads
    long num_threads;
    // Lunghezza della coda
    long q_length;
    // Tempo di ritardo
    long r_time;
    // Nome della directory
    char *dir_name = NULL;
    
    int opt;
    int n_counter = 0, q_counter = 0, d_counter = 0, t_counter = 0;

    // Gestione dei segnali, come visto ad esercitazione
    // Setto la maschera per i segnali che mi interessano
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGUSR1);

    // Registro la maschera e blocco
    if(pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
        fprintf(stderr, "FATAL ERROR: signal.\n");
        exit(EXIT_FAILURE);
    }
    // Ignoro SIGPIPE per evitare di essere terminato da una scrittura su un socket.
    struct sigaction s;
    memset(&s, 0, sizeof(s));
    s.sa_handler = SIG_IGN;
    // Passo all'handler che ignorerà SIGPIPE
    if(sigaction(SIGPIPE, &s, NULL) == -1) {
        perror("sigaction sigpipe");
        exit(EXIT_FAILURE);
    }

    // Creo il thread per la gestione dei segnali
    pthread_t signalhandler_thread;
    createThread(&signalhandler_thread, NULL, sigHandler, &mask);


    // I due punti dopo l'option dicono al getopt che ci sono argomenti
    while ((opt = getopt(argc, argv, ":n:q:t:d:")) != -1) {
        // Gestione dei parametri
        switch(opt) {
            case 'n': {
                if(n_counter > 0) {
                    fprintf(stderr, "FATAL ERROR: L'opzione '-n' è stata chiamata più volte.\n");
                    fprintf(stderr, "Utilizzare: farmProject -n <n_threads> -q <q_length> -t <r_time> -d <dir_name>\n");
                    exit(EXIT_FAILURE);
                } else {
                    // Controllo che optarg sia un valore, in caso contrario non viene modificato num_threads e alla fine prenderà il valore di default.
                    if(isNumber(optarg, &num_threads) != 0) {
                        fprintf(stderr, "L'argomento di '-n' non è valido.\n");
                        fprintf(stderr, "Utilizzo valori di default per '-n'.\n");
                        // Valore di default per num_threads
                        num_threads = DEFAULT_NUM_THREADS;
                    }
                    // In ogni caso aumento il contatore dei tentativi, sia nel caso di inserimento corretto, sia nel caso di inserimento di default
                    n_counter++;
                }
            } break;
            case 'q': {
                if(q_counter > 0) {
                    fprintf(stderr, "FATAL ERROR: L'opzione '-q' è stata chiamata più volte.\n");
                    fprintf(stderr, "Utilizzare: farmProject -n <n_threads> -q <q_length> -t <r_time> -d <dir_name>\n");
                    exit(EXIT_FAILURE);
                } else {
                    // Controllo che optarg sia un valore, in caso contrario non viene modificato q_length e alla fine prenderà il valore di default.
                    if(isNumber(optarg, &q_length) != 0) {
                        fprintf(stderr, "L'argomento di '-q' non è valido.\n");
                        fprintf(stderr, "Utilizzo valori di default per '-q'.\n");
                        // Valore di defualt per q_length
                        q_length = DEFAULT_QUEUE_LENGTH;
                    }
                    // In ogni caso aumento il contatore dei tentativi, sia nel caso di inserimento corretto, sia nel caso di inserimento di default
                    q_counter++;
                }
            } break;
            case 't': {
                if(t_counter > 0) {
                    fprintf(stderr, "FATAL ERROR: L'opzione '-t' è stata chiamata più volte.\n");
                    fprintf(stderr, "Utilizzare: farmProject -n <n_threads> -q <q_length> -t <r_time> -d <dir_name>\n");
                    exit(EXIT_FAILURE);
                } else {
                    // Controllo che optarg sia un valore, in caso contrario non viene modificato r_time e alla fine prenderà il valore di default.
                    if(isNumber(optarg, &r_time) != 0) {
                        fprintf(stderr, "L'argomento di '-t' non è valido.\n");
                        fprintf(stderr, "Utilizzo valori di default per '-t'.\n");
                        // Valore di default per r_time
                        r_time = DEFAULT_DELAY_TIME;
                    }
                    // In ogni caso aumento il contatore dei tentativi, sia nel caso di inserimento corretto, sia nel caso di inserimento di default
                    t_counter++;
                }
            } break;
            case 'd': {
                if(d_counter > 0) {
                    fprintf(stderr, "FATAL ERROR: L'opzione '-d' è stata chiamata più volte.\n");
                    fprintf(stderr, "Utilizzare: farmProject -n <n_threads> -q <q_length> -t <r_time> -d <dir_name>\n");
                    exit(EXIT_FAILURE);
                } else {
                    if(strlen(optarg) > MAX_ARGV_LENGTH) {
                        fprintf(stderr, "L'argomento di '-d' contiene troppi caratteri.\n");
                        fprintf(stderr, "Utilizzo valori di default per '-d'.\n");
                        // Valore di default per dir_name
                        dir_name = NULL;
                    } else {
                        dir_name = optarg;
                    }
                    // In ogni caso aumento il contatore dei tentativi, sia nel caso di inserimento corretto, sia nel caso di inserimento di default
                    d_counter++;
                }
            } break;
            case ':': {
                fprintf(stderr, "FATAL ERROR: L'opzione '-%c' richiede un argomento.\n", optopt);
                exit(EXIT_FAILURE);
            } break;
            case '?': {
                fprintf(stderr, "L'opzione '-%c' non è riconosciuta.\n", optopt);
            } break;
            default:;
        }
    }

    // Se avvio il programma senza argomenti in input, setto tutti i valori di default.
    if(n_counter == 0) {
        fprintf(stdout, "Utilizzo valore di default per '-n'.\n");
        num_threads = DEFAULT_NUM_THREADS;
    }
    if(q_counter == 0) {
        fprintf(stdout, "Utilizzo valore di default per '-q'.\n");
        q_length = DEFAULT_QUEUE_LENGTH;
    }
    if(t_counter == 0) {
        fprintf(stdout, "Utilizzo valore di default per '-t'.\n");
        r_time = DEFAULT_DELAY_TIME;
    }
    if(d_counter == 0) {
        fprintf(stdout, "Utilizzo valore di default per '-d'.\n");
    }

    // Gestione della lista degli argomenti
    Node_t *lista = NULL;
    // Comincio a scorrere gli argomenti non riconosciuti da getopt
    // Il primo valore servirà a creare la lista
    // I successivi valori verranno inseriti alla fine della lista

    int i = optind;
    while(i<argc) {
        if((strlen(argv[i])+1) > MAX_ARGV_LENGTH) {
            fprintf(stderr, "Errore: argomento formato da troppi caratteri. Ignorato.\n");
        } else {
            // printf("Test.\n");
            if (is_regular_file(argv[i]) == 0) {
                lista = postInsert(lista, argv[i]);
            } else {
                fprintf(stderr, "Errore: File non regolare/stat fallita. Ignorato.\n");
            }
        }
        i++;
    }

    // TEST: --> OK
    // printList(lista);

    /*
        Debug argomenti:
        printf("-n: %ld, -q: %ld, -t: %ld, -d: %s", num_threads, q_length, r_time, dir_name);
        printf("\n");
        printList(head);
    */
    if (dir_name != NULL) {
        // Controllo che dir sia una cartella regolare
        if(is_regular_folder(dir_name) == 0) {
            naviga_cartella(dir_name, lista);
        } else {
            fprintf(stderr, "Errore: cartella non regolare. Ignorato.\n");
        }
    }

        // TEST: --> OK
        // printf("-n: %ld, -q: %ld, -t: %ld, -d: %s", num_threads, q_length, r_time, dir_name);
        // printf("\n");
        // printList(lista);

        // printf("FINE TEST.\n");
    
    
    pid_t pid = fork();
    // Controllo se sono padre, figlio o se c'è stato un errore
    if(pid == 0) {
        // Sono il figlio
        // Processo collector
        int child = farm_collector();
        if(child != 0) {
            fprintf(stderr, "FATAL ERROR: processo collector.\n");
            exit(EXIT_FAILURE);
        }
    } else if(pid > 0) {
        // Sono il padre
        // Processo Master_Worker
        int parent = farm_master(num_threads, q_length, r_time, lista);
        if(parent != 0) {
            fprintf(stderr, "FATAL ERROR: processo master.\n");
            exit(EXIT_FAILURE);
        }
        if(waitpid(pid, NULL, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        // dopo che la cancellazione di un thread è terminata, la join ottiene PTHREAD_CANCELED
        // Fare la join è l'unico modo per sapere se la cancellazione è terminata.
        pthread_cancel(signalhandler_thread);
        JOIN(signalhandler_thread);
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    
    return 0;
}
