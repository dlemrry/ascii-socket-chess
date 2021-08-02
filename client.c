#include "client.h"



int login = 0;

void main(int argc, char*argv[]) {
        int option = 0;

        system("clear");

        if (argc != 3) {
                printf("Usage : %s <IP> <Port>\n", argv[0]);
                exit(1);
        }

        mysql_init(&mysql);     // Initialize mysql

        if (!mysql_real_connect(&mysql, "database-1.c05l4wrasdry.ap-northeast-2.rds.amazonaws.com", "admin", "dglee9595", "chess", 3306, (char *)NULL, 0)) {
                printf("%s\n", mysql_error(&mysql));
                exit(1);
        }
        printf("mysql connected\n");
        strcpy(server, argv[1]);
        serverPort = atoi(argv[2]);

        while (1) {
                printf("\n\n\n");
                printf("Option\n\n");
                printf("1. Log in\n");
                printf("2. Register\n");
                printf("3. Log out\n");
                printf("4. My Information\n");
                printf("5. Game\n");
                printf("6. Exit\n");

                while (1) {
                        printf("Please select option\n");

                        scanf("%d", &option);

                        if (option <= 0 || option > 6)
                                printf("Invalid option.\n Please select again.\n");
                        else
                                break;
                }

                switch (option) {
                        case 1:
                                logIn();
                                break;

                        case 2:
                                regist();
                                break;

                        case 3:
                                logOut();
                                strcpy(ID, "");
                                login = 0;
                                break;

                        case 4:
                                myInformation();
                                break;

                        case 5:
                                game();
                                break;

                        case 6:
                                system("clear");
                                printf("You select exiting this program option.\n");
                                exit(1);
                                break;
                }
        }
}

void logIn() {
        char inputID[ID_SIZE];
        char inputPassword[PWD_SIZE];
        int i = 0;

        system("clear");

        if (login != 0) {
                printf("You already logged in.\n");
                return;
        }

        printf("You have selected the log in option.\n");
        printf("Please enter your ID.\n");
        scanf("%s", inputID);

        sprintf(query, "SELECT password, latest, login FROM user WHERE id = '%s'", inputID);

        if (mysql_query(&mysql, query)) {
                system("clear");
                printf("%s\n", mysql_error(&mysql));
                return;
        }

        res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);
        fields = mysql_num_rows(res);

        if (fields == 0) {
                printf("There is no user who have matched ID that you entered.\n");
                return;
        } else if (atoi(row[2]) == 1) {
                printf("%s is already logged in.\n", inputID);
                return;
        }

        printf("Please enter your password.\n");

        scanf("%s", inputPassword);

        if (!strcmp(inputPassword, row[0])) {
                system("clear");
                printf("Wellcome.\n");
                printf("your latest log in : %s", row[1]);
                strcpy(ID, inputID);
                login = 1;

                mysql_free_result(res);

                sprintf(query, "UPDATE user SET latest = now(), login = true WHERE id = '%s'", inputID);

                if (mysql_query(&mysql, query)) {
                        system("clear");
                        printf("%s\n", mysql_error(&mysql));
                        return;
                }
        } else {
                system("clear");
                printf("Password that you entered isn't matched.\n");
                return;
        }
}

void regist() {
        char inputID[ID_SIZE];
        char inputPassword[PWD_SIZE];
        int i = 0;

        system("clear");

        if (login == 1) {
                printf("You already logged in.\n");
                return;
        }

        printf("You have selected the sing up option.\n");
        printf("Please enter your ID.\n");
        scanf("%s", inputID);

        sprintf(query, "SELECT * FROM user WHERE id = '%s'", inputID);

        if (mysql_query(&mysql, query)) {
                system("clear");
                printf("%s\n", mysql_error(&mysql));
                return;
        }

        res = mysql_store_result(&mysql);
        fields = mysql_num_rows(res);

        if (fields == 1) {
                system("clear");
                printf("%s is already exist.\n", inputID);
                printf("%d\n", fields);
                return;
        }

        mysql_free_result(res);

        printf("Please enter your password.\n");
        scanf("%s", inputPassword);

        sprintf(query, "INSERT INTO user VALUES ('%s', '%s', 0, 0 , 0, now(), now(), false)", inputID, inputPassword);

        if (mysql_query(&mysql, query)) {
                system("clear");
                printf("%s\n", mysql_error(&mysql));
                return;
        }

        system("clear");
        printf("You successfully registered to our chess program.\n");
}

void logOut() {
        system("clear");

        if (login == 0) {
                printf("You doesn't log in.\n");
                return;
        }

        sprintf(query, "UPDATE user SET login = false WHERE id = '%s'", ID);

        if (mysql_query(&mysql, query)) {
                printf("%s\n", mysql_error(&mysql));
                return;
        }

        strcpy(ID, "");
        login = 0;

        printf("You successfully log out. Thank you.\n");
}

void myInformation() {
        int i = 0;
        float rate = 0;
        int win = 0, draw = 0, lose = 0;
        int total = 0;

        if (login == 0) {
                system("clear");
                printf("You doesn't log in.\n");
                return;
        }

        system("clear");

        sprintf(query, "SELECT * FROM user WHERE id = '%s'", ID);

        if (mysql_query(&mysql, query)) {
                printf("%s\n", mysql_error(&mysql));
                return;
        }

        res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);

        printf("-------------------\n");
        printf("User Information.\n");
        printf("ID : %s\n", row[0]);
        printf("Created : %s\n", row[5]);

        win = atoi(row[2]);
        draw = atoi(row[3]);
        lose = atoi(row[4]);

        total = win + draw + lose;

        if (total == 0)
                rate = 0;
        else
                rate = (float) win / (float) total;

        printf("Total Game : %d\t Win Rate : %.2f\n", total, rate);
        printf("Win : %d\t", win);
        printf("Draw : %d\t", draw);
        printf("Lose : %d\n", lose);

        mysql_free_result(res);
}

void game() {
        int option;

        if (login == 0) {
                system("clear");
                printf("You have to log in.\n");
                return;
        }

        while (1) {
                printf("\n\n\n");
                printf("Option.\n");
                printf("1. Check open waiting game room.\n");
                printf("2. Create waiting game room.\n");
                printf("3. Enter private waiting game room.\n");
                printf("4. Exit.\n");

                while (1) {
                        printf("Please select option.\n");

                        scanf("%d", &option);

                        if (option <= 0 || option > 4)
                                printf("Invalid option.\n Please select again.\n");
                        else
                                break;
                }

                switch (option) {
                        case 1:
                                checkRoom();
                                break;

                        case 2:
                                createRoom();
                                break;

                        case 3:
                                enterPrivateRoom();
                                break;

                        case 4:
                                system("clear");
                                return;
                                break;
                }
        }
}

void checkRoom() {
        char **port;
        int i = 0;
        pid_t pid;
        int status;
        int option;
        //char player[2];

        system("clear");

        printf("Open waiting game room list.\n");

        if (mysql_query(&mysql, "SELECT port, p1, name FROM room WHERE private = false AND usercnt = 1")) {
                system("clear");
                printf("%s\n", mysql_error(&mysql));
                return;
        }

        res = mysql_store_result(&mysql);
        fields = mysql_num_rows(res);

        printf("fields : %d\n", fields);

        if (fields == 0) {
                system("clear");
                printf("There is no waiting room.\n");
                return;
        }

        port = (char **) malloc(sizeof(char *) * fields);
        for (i=0; i<fields; i++)
                port[i] = (char *) malloc(sizeof(char) * ROOM_NAME_SIZE);

        i = 0;

        while ((row = mysql_fetch_row(res))) {
                printf("------------------------------\n");
                printf("%d. %s\n", i+1, row[2]);
                printf("Host : %s\n", row[1]);

                strcpy(port[i], row[0]);

                i++;
                printf("------------------------------\n");
        }

        printf("If you want to join to the open waiting room, please enter the room number.\n");
        printf("If you want to exit this function, please enter 0.\n");

        scanf("%d", &option);

        mysql_free_result(res);

        if (option == 0) {
                system("clear");
                return;
        } else {
                sprintf(query, "UPDATE room SET usercnt = usercnt+1, p2 = '%s', r1 = false, r2 = false WHERE port = %s", ID, port[option-1]);

                if (mysql_query(&mysql, query)) {
                        system("clear");
                        printf("%s\n", mysql_error(&mysql));
                        return;
                }

                chess_clnt(server, port[option-1], ID, 2);
        }
}

void createRoom() {
        int sock;
        struct sockaddr_in serv_addr;
        char roomName[30];
        int isPrivate;
        pid_t pid;
        int status;
        char port[10];
        char password[30];
        char msg[23] = "Give me a port number.";
        char isCreate;
        //char player[2];
        //int player=0;

        system("clear");

        printf("Please enter the room name.\n");
        scanf("%s", roomName);

        printf("Do you want to set this room private?\n");
        printf("Please enter 0(No) or 1(Yes).\n");
        scanf("%d", &isPrivate);

        sock = socket(PF_INET, SOCK_STREAM, 0);
        if(sock==-1)
                printf("socket error...\n");
        printf("socket set...\n");

        memset(&serv_addr, 0, sizeof(serv_addr));
        printf("memset...\n");
        serv_addr.sin_family = AF_INET;
        printf("serv_addr.family...\n");
        serv_addr.sin_addr.s_addr = inet_addr(server);
        printf("serv.addr.addr...\n");
        serv_addr.sin_port = htons(serverPort);
        printf("serv_addr.port...\n");

        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
                error_handling("connect() error");

        if (send(sock, msg, sizeof(msg), 0) == -1)
                error_handling("send() error");

        if (recv(sock, port, sizeof(port), 0) == -1)
                error_handling("recv() error");
        printf("sock connected\n");
        close(sock);

        if (isPrivate == 0) {
                                sprintf(query, "INSERT INTO room VALUES (%s, '%s', NULL, '%s', false, false, false, NULL, 1)", port, ID, roomName);

        } else if (isPrivate == 1) {
                printf("Please enter the room password.\n");
                scanf("%s", password);

                sprintf(query, "INSERT INTO room VALUES (%s, '%s', NULL, '%s', false, false, true, '%s', 1)", port, ID, roomName, password);
        } else {
                //system("clear");
                printf("You entered wrong option.\n");
                return;
        }
        printf("room created...\n");
        if (mysql_query(&mysql, query)) {
                //system("clear");
                printf("i am error\n");
                printf("%s\n", mysql_error(&mysql));
                return;
        }
        printf("query set...\n");
        //player=1;
        //sprintf(player, "1");
        printf("forking...\n");

        chess_clnt(server, port, ID, 1);
}

void enterPrivateRoom() {
        char roomName[ROOM_NAME_SIZE];
        char password[PWD_SIZE];
        char port[10];

        pid_t pid;
        int status;

        //char player[2];
        //int player=0;

        system("clear");

        printf("Please enter room name.\n");

        scanf("%s", roomName);

        sprintf(query, "SELECT port, password FROM room WHERE private = true AND name = '%s'", roomName);

        if (mysql_query(&mysql, query)) {
                system("clear");
                printf("%s\n", mysql_error(&mysql));
                return;
        }

        res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);
        fields = mysql_num_rows(res);

        if (fields == 0) {
                system("clear");
                printf("There is no waiting room matching information you entered.\n");
                return;
        }

        printf("Please enter the room password.\n");

        scanf("%s", password);

        if (strcmp(password, row[1]) != 0) {
                system("clear");
                printf("Password isn't matched.\n");
                return;
        }

        strcpy(port, row[0]);

        mysql_free_result(res);

        sprintf(query, "UPDATE room SET usercnt = usercnt + 1, p2 = '%s', r1 = false, r2 = false WHERE private = true AND name = '%s'", ID, roomName);
        //player join to private room

        if (mysql_query(&mysql, query)) {
                printf("%s\n", mysql_error(&mysql));
                exit(1);
        }

        chess_clnt(server, port, ID, 2);
}

void error_handling(char *msg) {
        fputs(msg, stderr);
        fputc('\n', stderr);
        exit(1);
}

