#include "server.h"



static int port;

int main(int argc, char *argv[]) {

        printf("main\n");
        int serv_sock, clnt_sock;
        struct sockaddr_in serv_adr, clnt_adr;
        int clnt_adr_sz;
        printf("started\n");
        pthread_t t_id;
        printf("pthread\n");
        if (argc != 2) {
                printf("Usage : %s <port>\n", argv[0]);
                exit(1);
        }
        printf("thread init\n");
        pthread_mutex_init(&mutx, NULL);
        printf("thread mutex init...\n");
        serv_sock = socket(PF_INET, SOCK_STREAM, 0);
        printf("serv_socket init...\n");
        memset(&serv_adr, 0, sizeof(serv_adr));
        printf("memset...\n");
        serv_adr.sin_family = AF_INET;
        printf("serv family...\n");
        serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("serv addr...\n");
        serv_adr.sin_port = htons(atoi(argv[1]));
            //printf("serv port error\n");
            printf("sin port = %d\n", serv_adr.sin_port);
        printf("serv port...\n");

        port = atoi(argv[1]);

        if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
                error_handling("bind() error\n");
        printf("binding...\n");
        printf("listening...");
        if (listen(serv_sock, 5) == -1)
                error_handling("listen() error\n");
        printf("listening...");

        while (1) {
                clnt_adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &clnt_adr_sz);

                pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
                pthread_detach(t_id);
                printf("Connected client IP in main program: %s\n", inet_ntoa(clnt_adr.sin_addr));
        }

        close(serv_sock);
        return 0;
}

void * handle_clnt(void * arg) {
        printf("handle_clnt\n");
        int clnt_sock = *((int*)arg);
        int str_len = 0, i;
        char msg[BUF_SIZE];
        char stringPort[10];
        pid_t pid;

        if ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0) {
                printf("create message : %s\n",msg);
                pthread_mutex_lock(&mutx);

                port += 1;
                printf("port : %d\n",port);
                sprintf(stringPort, "%d", port);

                pid = fork();

                pthread_mutex_unlock(&mutx);

                if (pid == 0) {
                        printf("chat_serv start.\n");
                        chess_serv(stringPort);
                }

                write(clnt_sock, stringPort, 10);

                close(clnt_sock);
                return NULL;
        }
}

void error_handling(char *msg) {
        //printf("eror_handling");
        fputs(msg, stderr);
        fputc('\n', stderr);
        exit(1);
}

