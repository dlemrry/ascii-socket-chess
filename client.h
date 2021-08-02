#pragma once

#ifndef _CLIENT_H
#define _CLIENT_H


#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/times.h>
#include <limits.h>
#include <sys/wait.h>
#include <pthread.h>
#include <ctype.h>

#define BUF_SIZE 100
#define NAME_SIZE 20
#define IP_SIZE 16
#define QUERY_SIZE 200
#define ID_SIZE 30
#define PWD_SIZE 30
#define ROOM_NAME_SIZE 30

MYSQL mysql;
MYSQL_RES* res;
MYSQL_ROW row;


char msg[BUF_SIZE];


//client.c
int fields;
int cnt;
char query[QUERY_SIZE];
char ID[ID_SIZE];
char server[IP_SIZE];
int serverPort;

char server[IP_SIZE];
int serverPort;
void regist();
void logIn();
void logOut();
void myInformation();
void game();
void checkRoom();
void createRoom();
void enterPrivateRoom();
void error_handling(char *msg);




//chess_clnt.c
int whowon;
char chess_ID[NAME_SIZE];

int chess_clnt(char *ip , char * gamePort, char *id, int play); // char *ip , char *gamePort, char *id, char *play, (char *)NULL)

void * send_msg(void * arg);
void * recv_msg(void * arg);

int chessmain2(void * arg);
int chessmain(void * arg);
int Kcantmove2(int a, int b,char chess[2][8][8]);
int Kcantmove(int a, int b,char chess[2][8][8]);
int takeback(char ent1, int i1, int j1, int k1, int l1, int cond, char chess[2][8][8]);
int takeback2(char ent1, int i1, int j1, int k1, int l1, int cond, char chess[2][8][8]);
int compute(char ent1, int i1, int j1, int k1, int l1, int cond, char chess[2][8][8]);
void printing(char chp[2][8][8]);
void printing2(char chp[2][8][8]);
int  IsInCheck(int a, int b,char chess[2][8][8]);
int  IsInCheck2(int a, int b, char chess[2][8][8]);

#endif