#pragma once

#ifndef _SERVER_H
#define _SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <limits.h>

#define BUF_SIZE 100
#define MAX_CLNT 256


pthread_mutex_t mutx;


//server.c
void * handle_clnt(void * arg);
void error_handling(char *msg);


//chess_serv.c
int chess_serv(char *gamePort);

void * chess_handle_clnt(void * arg);
void chess_send_msg(char * msg, int len);


#endif