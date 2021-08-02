#include "client.h"

char chess_name[NAME_SIZE] = "[DEFAULT]";

char restorecolor=' ';
char restorekind=' ';
char restorecolor2=' ';
char restorekind2=' ';


int chess_clnt(char *ip , char * gamePort, char *id, int play)
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;

	mysql_init(&mysql);                                    //sql �߰��κ�

	if (!mysql_real_connect(&mysql, "database-1.c05l4wrasdry.ap-northeast-2.rds.amazonaws.com", "admin", "dglee9595", "chess", 3306, (char *)NULL, 0)) {                                    //sql �߰��κ�
		printf("%s\n", mysql_error(&mysql));                                    //sql �߰��κ�
		exit(1);                                    //sql �߰��κ�
	}                                    //sql �߰��κ�

	sprintf(chess_name, "[%s]", id);
	sprintf(chess_ID, "%s", id);                                    //sql �߰��κ�


	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(atoi(gamePort));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");


	//int a = atoi(play);
	//int a=play;
	if (play == 1) {
		chessmain((void*)&sock);
		printf("player %d won the game", whowon);
	}
	else if (play == 2) {
		chessmain2((void*)&sock);
		printf("player %d won the game", whowon);
	}

	//pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	//pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	//pthread_join(snd_thread, &thread_return);
	//pthread_join(rcv_thread, &thread_return);
	close(sock);
	return 0;
}

void * send_msg(void * arg)   // send thread main
{
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	while (1)
	{
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			close(sock);
			exit(0);
		}
		sprintf(name_msg, "%s %s", chess_name, msg);
		write(sock, name_msg, strlen(name_msg));
	}
	return NULL;
}

void * recv_msg(void * arg)   // read thread main
{
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	int str_len;
	while (1)
	{
		str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1);
		if (str_len == -1)
			return (void*)-1;
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
	}
	return NULL;
}




//////////////////////////////////////////////








int chessmain(void * arg)
{
	char chess[2][8][8] = {
						{
						{'R','N','B','Q','K','B','N','R'},
						{'P','P','P','P','P','P','P','P'},
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{' ',' ',' ',' ',' ',' ',' ',' '},  //chess[0],  ���� ����
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{'P','P','P','P','P','P','P','P'},
						{'R','N','B','Q','K','B','N','R'},
					  },

						{
						{'1','1','1','1','1','1','1','1'},
						{'1','1','1','1','1','1','1','1'},
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{' ',' ',' ',' ',' ',' ',' ',' '},  //chess[1], ���� ������
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{'2','2','2','2','2','2','2','2'},
						{'2','2','2','2','2','2','2','2'},
					  }
	};


	const char *ind[8][8] =
	{
	{"A8","B8","C8","D8","E8","F8","G8","H8"},
	{"A7","B7","C7","D7","E7","F7","G7","H7"},
	{"A6","B6","C6","D6","E6","F6","G6","H6"},
	{"A5","B5","C5","D5","E5","F5","G5","H5"},
	{"A4","B4","C4","D4","E4","F4","G4","H4"},
	{"A3","B3","C3","D3","E3","F3","G3","H3"},
	{"A2","B2","C2","D2","E2","F2","G2","H2"},
	{"A1","B1","C1","D1","E1","F1","G1","H1"},
	};

	int checker = 0;   // �ϼ�
	int run = 108;

	while (run > 0)
	{

		printing(chess);
		char ent = '0';


		//  int player �� 1p 2p ���� ������ ����

	   /***************************************TAKING THE INPUT**********************************************/
		if (checker % 2 == 0)
		{
			printf("PLAYER 1 TYPE IN YOUR MOVE: \n");   //                "Player 1 is playing...."


			int whitesp = 0;
			int n = 0;
			int ck = 0;
			int ik = 0;
			int fk = 0;
			char d = '\0';
			char lin1[100];
			char lin2[100];
			char initv[100], finlv[100], help[100];

			int par = 0;
			for (par = 0; par < 100; par++)
			{
				lin1[par] = '\0';
				initv[par] = '\0';
				help[par] = '\0';
				finlv[par] = '\0';
			}

			int o, p;
			for (o = 0; o < 8; o++) {
				for (p = 0; p < 8; p++) {
					if (chess[0][o][p] == 'K' && chess[1][o][p] == '2')  //���� ŷ ��ġ ã��
						break;
				}
				if (chess[0][o][p] == 'K' && chess[1][o][p] == '2')  //���� ŷ ��ġ ã��
					break;
			}


			if (IsInCheck(o, p,chess) == 1) {
				printf("you are in check \n");
				if (Kcantmove(o, p,chess) == 1) {
					printf("King cant move \n");
				}

			}


			getchar();
			scanf(" %[^\n]s", lin2);    //        scanf �� �ڽ��� ������Ʈ�ϰ�   send_msg �� �ٸ� Ŭ���̾�Ʈ�� ����� ����.
			int sock = *((int*)arg);
			send(sock, lin2, 100, 0);
			recv(sock, lin1, 100, 0);
			//send(sock, "00",100, 0);

			char *qq = "quit";
			if (!strcmp(lin1, qq)) {
				whowon = 2;
				sprintf(query, "UPDATE user SET lose = lose+1 WHERE id = '%s'", chess_ID);                // mysql query

				if (mysql_query(&mysql, query)) {
					printf("%s\n", mysql_error(&mysql));
					exit(1);
				}
				break;
			}


			int low = 0;
			int len = strlen(lin1) - 1;

			for (low; low <= len; low++)
			{
				lin1[low] = toupper(lin1[low]);
			}

			for (n = 0; n <= len; n++)
			{
				if (lin1[n] == ' ' && n == 0)
					continue;

				if (lin1[n] == ' ' && ((lin1[n - 1] != ' ') && n != 0)) {
					whitesp++;
				}

				if (whitesp == 0 && (n == 0 || lin1[n - 1] == ' ') && lin1[n] != ' ')
					d = lin1[n];

				if (whitesp == 0 && lin1[n] != ' ') {
					help[ck] = lin1[n];          //Ư�������϶� help ������� �ν�
					ck++;
				}

				if (whitesp == 1 && lin1[n] != ' ') {
					initv[ik] = lin1[n];                                     //1 a2 a3   ���� 1�� ent, a2�� initv, a3�� finlv
					ik++;
				}

				if (whitesp == 2 && lin1[n] != ' ') {
					finlv[fk] = lin1[n];
					fk++;
				}

			}

			/*
				  printf("\n");                            The comments have been left
				  printf("%c \n", d);                      as such because people who are new
				  printf("%s \n", lin1 );                  to the source code can uncomment the
				  printf("%sa\n", help);                   statements and understand the significance
				  printf("%s \n", initv);                  and working of the variables.
				  printf("%s \n", finlv);
			*/



			/*************************************CALCULATING THE INDICES******************************************/
			//char *quit = "QUIT";
			int dep = 30;
			//dep = strcmp(help, quit);
			/*  printf("dep%d",dep) */;

			char *display = "HELP";
			int ped = 40;
			ped = strcmp(help, display);

			if (ped == 0)
			{
				printf("\n\n\n\n\n");
				printf("\t1 = PAWN\n\t2 = KNIGHT\n\t4 = BISHOP\n\t5 = ROOK\n\t7 = QUEEN\n\t9 = KING\n\n\n\n\n");

				printf("PRESS ENTER TO ESCAPE HELP:");
				char esc;

				if ((esc = getchar()) == 'h' || (esc = getchar()) == 'H')
					continue;

			}

			//if (dep == 0)
			//{
			//	printf("ENTER Y TO QUIT:");
			//	char y;

			//	if ((y = getchar()) == 'y' || (y = getchar()) == 'Y')
			//		goto END;
			//}


			ent = d;
			int error = 0;
			int i, j, k, l = 0;
			int check;

			for (i = 0; i <= 7; i++)
			{
				check = 100;

				for (j = 0; j <= 7; j++)
				{
					check = strcmp(ind[i][j], initv);          //ü���� Ž���� ã�� ���̸� Ž�� ����. i�� j�� �ش� ��ġ ���� ��.  ���⼭���� �ش� i,j ��ġ������ �ؿ� ��� Ȯ��.
					if (check == 0)
						break;
				};

				if (check == 0)
					break;
			}


			if (check != 0)
			{
				error++;
				printf("THERE IS NO INITIAL POSITION AS \"%s\"\n", initv);/*************ERROR ERROR*************/
			}


			for (k = 0; k <= 7; k++)
			{
				check = 100;

				for (l = 0; l <= 7; l++)
				{
					check = strcmp(ind[k][l], finlv);                //ü���� Ž���� �̵��� ���ϴ� ��ġ�� Ž�� ����. i�� j�� �ش� ��ġ ���� ��.  ���⼭���� �ش� k,i ��ġ������ �ؿ� ��� Ȯ��.
					if (check == 0)
						break;
				};

				if (check == 0)
					break;
			}

			if (check != 0)
			{
				error++;
				printf("THERE IS NO FINAL POSITION AS \"%s\"\n", finlv);/*************ERROR ERROR*************/
			}

			char king = chess[0][k][l];          //��ǥ�� �� ��ġ

			if (checker % 2 != 0 && chess[1][i][j] == '2')  // ����� ���� �������� 2 �̸�
			{
				error = 1;
				printf("PLAYER 2 YOU CANNOT ACCESS ARMY OF PLAYER 1\n");
			}


			else if (checker % 2 == 0 && chess[1][i][j] == '1')
			{
				error = 1;
				printf("PLAYER 1 YOU CANNOT ACCESS ARMY OF PLAYER 2\n");///////////////////////////////////
			}


			/*  system("clear");*/


			int c, v;
			for (c = 0; c < 8; c++) {
				for (v = 0; v < 8; v++) {
					if (chess[0][c][v] == 'K' && chess[1][c][v] == '2')  //���� ŷ ��ġ ã��
						break;
				}
				if (chess[0][c][v] == 'K' && chess[1][c][v] == '2')  //���� ŷ ��ġ ã��
					break;
			}
			printf("%d %d \n", c, v);
			if (IsInCheck(c, v,chess) == 1) {

				printf("player1 is in check\n");


			}



			if (error == 0)
			{
				error = compute(ent, i, j, k, l, 0, chess);           // ��ɾ��� compute ����
			}


			if (IsInCheck(c, v,chess) == 1) {
				takeback(ent, i, j, k, l, 0, chess);
				error = 1;
				printf(" You are in check\n");
			}



			if (error == 0)
			{
				checker++;
				/*  printf("checker:%d  error a:%d\n",checker,error);*/
			}

			else if (error != 0)
			{
				/*  printf("checker:%d  error b:%d\n",checker,error);*/
			}



			if (king == 'K' && error == 0 && checker % 2 == 0)
			{
				printf("PLAYER 2 HAS WON THE GAME\n");
				whowon = 2;
			
				sprintf(query, "UPDATE user SET lose = lose+1 WHERE id = '%s'", chess_ID);                // mysql query

				if (mysql_query(&mysql, query)) {
					printf("%s\n", mysql_error(&mysql));
					exit(1);
				}

				break;
			}

			if (king == 'K' && error == 0 && checker % 2 != 0)
			{
				printf("PLAYER 1 HAS WON THE GAME\n");
				whowon = 1;

				sprintf(query, "UPDATE user SET win = win+1 where id = '%s'", chess_ID);                // mysql query

				if (mysql_query(&mysql, query)) {
					printf("%s\n", mysql_error(&mysql));
					exit(1);
				}
			

				break;
			}

			printing(chess);


			/*  printf("checker:%d  error c:%d\n",checker,error);*/

		}









		////////////////////////////////////////////////////////////////////////////////////





		else if (checker % 2 != 0)
		{
			printf("PLAYER 2 MOVED: ");


			int whitesp = 0;
			int n = 0;
			int ck = 0;
			int ik = 0;
			int fk = 0;
			char d = '\0';
			char lin1[100];
			char initv[100], finlv[100], help[100];

			int par = 0;
			for (par = 0; par < 100; par++)
			{
				lin1[par] = '\0';
				initv[par] = '\0';
				help[par] = '\0';
				finlv[par] = '\0';
			}

			//scanf(" %[^\n]s", lin1);                         // scanf�� ���� ��� recv_msg��  �÷��̾�2 move ������Ʈ
			int sock = *((int*)arg);
			recv(sock, lin1, 100, 0);

			char *qq = "quit";
			if (!strcmp(lin1, qq)) {
				whowon = 1;
				sprintf(query, "UPDATE user SET win = win+1 where id = '%s'", chess_ID);                // mysql query

				if (mysql_query(&mysql, query)) {
					printf("%s\n", mysql_error(&mysql));
					exit(1);
				}
				break;
			}

			printf("[%s] \n", lin1);




			int low = 0;
			int len = strlen(lin1) - 1;

			for (low; low <= len; low++)
			{
				lin1[low] = toupper(lin1[low]);
			}

			for (n = 0; n <= len; n++)
			{
				if (lin1[n] == ' ' && n == 0)
					continue;

				if (lin1[n] == ' ' && ((lin1[n - 1] != ' ') && n != 0)) {
					whitesp++;
				}

				if (whitesp == 0 && (n == 0 || lin1[n - 1] == ' ') && lin1[n] != ' ')



					d = lin1[n];


				if (whitesp == 0 && lin1[n] != ' ') {
					help[ck] = lin1[n];          //Ư�������϶� help ������� �ν�
					ck++;
				}

				if (whitesp == 1 && lin1[n] != ' ') {


					if (lin1[n] == 'A')
						lin1[n] = 'H';
					else if (lin1[n] == 'B')
						lin1[n] = 'G';
					else if (lin1[n] == 'C')
						lin1[n] = 'F';
					else if (lin1[n] == 'D')
						lin1[n] = 'E';
					else if (lin1[n] == 'E')                               // ��ġ ����
						lin1[n] = 'D';
					else if (lin1[n] == 'F')
						lin1[n] = 'C';
					else if (lin1[n] == 'G')
						lin1[n] = 'B';
					else if (lin1[n] == 'H')
						lin1[n] = 'A';
					else if (lin1[n] == '1')
						lin1[n] = '8';
					else if (lin1[n] == '2')
						lin1[n] = '7';
					else if (lin1[n] == '3')
						lin1[n] = '6';
					else if (lin1[n] == '4')
						lin1[n] = '5';
					else if (lin1[n] == '5')
						lin1[n] = '4';
					else if (lin1[n] == '6')
						lin1[n] = '3';
					else if (lin1[n] == '7')
						lin1[n] = '2';
					else if (lin1[n] == '8')
						lin1[n] = '1';




					initv[ik] = lin1[n];                                     //1 a2 a3   ���� 1�� ent, a2�� initv, a3�� finlv
					ik++;
				}

				if (whitesp == 2 && lin1[n] != ' ') {

					if (lin1[n] == 'A')
						lin1[n] = 'H';
					else if (lin1[n] == 'B')
						lin1[n] = 'G';
					else if (lin1[n] == 'C')
						lin1[n] = 'F';
					else if (lin1[n] == 'D')
						lin1[n] = 'E';
					else if (lin1[n] == 'E')                               // ��ġ ����
						lin1[n] = 'D';
					else if (lin1[n] == 'F')
						lin1[n] = 'C';
					else if (lin1[n] == 'G')
						lin1[n] = 'B';
					else if (lin1[n] == 'H')
						lin1[n] = 'A';
					else if (lin1[n] == '1')
						lin1[n] = '8';
					else if (lin1[n] == '2')
						lin1[n] = '7';
					else if (lin1[n] == '3')
						lin1[n] = '6';
					else if (lin1[n] == '4')
						lin1[n] = '5';
					else if (lin1[n] == '5')
						lin1[n] = '4';
					else if (lin1[n] == '6')
						lin1[n] = '3';
					else if (lin1[n] == '7')
						lin1[n] = '2';
					else if (lin1[n] == '8')
						lin1[n] = '1';

					finlv[fk] = lin1[n];
					fk++;
				}

			}
			//printf("[%d %s %s] \n", d, initv, finlv);
				/*
				  printf("\n");                            The comments have been left
				  printf("%c \n", d);                      as such because people who are new
				  printf("%s \n", lin1 );                  to the source code can uncomment the
				  printf("%sa\n", help);                   statements and understand the significance
				  printf("%s \n", initv);                  and working of the variables.
				  printf("%s \n", finlv);
			*/



			/*************************************CALCULATING THE INDICES******************************************/
			//char *quit = "QUIT";
			int dep = 30;
			//dep = strcmp(help, quit);
			/*  printf("dep%d",dep) */;

			char *display = "HELP";
			int ped = 40;
			ped = strcmp(help, display);

			if (ped == 0)
			{
				printf("\n\n\n\n\n");
				printf("\t1 = PAWN\n\t2 = KNIGHT\n\t4 = BISHOP\n\t5 = ROOK\n\t7 = QUEEN\n\t9 = KING\n\n\n\n\n");

				printf("PRESS ENTER TO ESCAPE HELP:");
				char esc;

				if ((esc = getchar()) == 'h' || (esc = getchar()) == 'H')
					continue;

			}

			//if (dep == 0)
			//{
			//	printf("ENTER Y TO QUIT:");
			//	char y;

			//	if ((y = getchar()) == 'y' || (y = getchar()) == 'Y')
			//		goto END;
			//}


			ent = d;
			int error = 0;
			int i, j, k, l = 0;
			int check;

			for (i = 0; i <= 7; i++)
			{
				check = 100;

				for (j = 0; j <= 7; j++)
				{
					check = strcmp(ind[i][j], initv);          //ü���� Ž���� ã�� ���̸� Ž�� ����. i�� j�� �ش� ��ġ ���� ��.  ���⼭���� �ش� i,j ��ġ������ �ؿ� ��� Ȯ��.
					if (check == 0)
						break;
				};

				if (check == 0)
					break;
			}


			if (check != 0)
			{
				error++;
				printf("THERE IS NO INITIAL POSITION AS \"%s\"\n", initv);/*************ERROR ERROR*************/
			}


			for (k = 0; k <= 7; k++)
			{
				check = 100;

				for (l = 0; l <= 7; l++)
				{
					check = strcmp(ind[k][l], finlv);                //ü���� Ž���� �̵��� ���ϴ� ��ġ�� Ž�� ����. i�� j�� �ش� ��ġ ���� ��.  ���⼭���� �ش� k,i ��ġ������ �ؿ� ��� Ȯ��.
					if (check == 0)
						break;
				};

				if (check == 0)
					break;
			}

			if (check != 0)
			{
				error++;
				printf("THERE IS NO FINAL POSITION AS \"%s\"\n", finlv);/*************ERROR ERROR*************/
			}

			char king = chess[0][k][l];          //��ǥ�� �� ��ġ

			if (checker % 2 != 0 && chess[1][i][j] == '2')  // ����� ���� �������� 2 �̸�
			{
				error = 1;
				printf("PLAYER 2 YOU CANNOT ACCESS ARMY OF PLAYER 1\n");
			}

			else if (checker % 2 == 0 && chess[1][i][j] == '1')
			{
				error = 1;
				printf("PLAYER 1 YOU CANNOT ACCESS ARMY OF PLAYER 2\n");
			}



			int c, v;
			for (c = 0; c < 8; c++) {
				for (v = 0; v < 8; v++) {
					if (chess[0][c][v] == 'K' && chess[1][c][v] == '1')  //����� ŷ ��ġ ã��
						break;
				}
				if (chess[0][c][v] == 'K' && chess[1][c][v] == '1')  //����� ŷ ��ġ ã��
					break;
			}
			printf("%d %d \n", c, v);
			if (IsInCheck2(c, v, chess) == 1) {

				printf("player2 is in check\n");


			}





			/*  system("clear");*/

			if (error == 0)
			{
				error = compute(ent, i, j, k, l, 0, chess);           // ��ɾ��� compute ����
			}

			if (IsInCheck2(c, v, chess) == 1) {
				takeback(ent, i, j, k, l, 0, chess);
				error = 1;
				//printf(" You are in check\n");
			}



			if (error == 0)
			{
				checker++;
				/*  printf("checker:%d  error a:%d\n",checker,error);*/
			}

			else if (error != 0)
			{
				/*  printf("checker:%d  error b:%d\n",checker,error);*/
			}

			if (king == 'K' && error == 0 && checker % 2 == 0)                   //üũ����Ʈ????  ŷ�� ����???
			{
				printf("PLAYER 2 HAS WON THE GAME\n");
				whowon = 2;
				sprintf(query, "UPDATE user SET lose = lose+1 WHERE id = '%s'", chess_ID);                // mysql query

				if (mysql_query(&mysql, query)) {
					printf("%s\n", mysql_error(&mysql));
					exit(1);
				}


				break;
			}

			if (king == 'K' && error == 0 && checker % 2 != 0)
			{
				printf("PLAYER 1 HAS WON THE GAME\n");
				whowon = 1;


				sprintf(query, "UPDATE user SET win = win+1 where id = '%s'", chess_ID);                // mysql query

				if (mysql_query(&mysql, query)) {
					printf("%s\n", mysql_error(&mysql));
					exit(1);
				}


				break;
			}

			/*  printf("checker:%d  error c:%d\n",checker,error);*/

		}

	}

END:
	return whowon;
}















int chessmain2(void * arg)
{
	char chess[2][8][8] = {
						{
						{'R','N','B','K','Q','B','N','R'},
						{'P','P','P','P','P','P','P','P'},
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{' ',' ',' ',' ',' ',' ',' ',' '},  //chess[0],  ���� ����
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{'P','P','P','P','P','P','P','P'},
						{'R','N','B','K','Q','B','N','R'},
					  },

						{
						{'1','1','1','1','1','1','1','1'},
						{'1','1','1','1','1','1','1','1'},
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{' ',' ',' ',' ',' ',' ',' ',' '},  //chess[1], ���� ������
						{' ',' ',' ',' ',' ',' ',' ',' '},
						{'2','2','2','2','2','2','2','2'},
						{'2','2','2','2','2','2','2','2'},
					  }
	};


	const char *ind[8][8] =
	{
	{"A8","B8","C8","D8","E8","F8","G8","H8"},
	{"A7","B7","C7","D7","E7","F7","G7","H7"},
	{"A6","B6","C6","D6","E6","F6","G6","H6"},
	{"A5","B5","C5","D5","E5","F5","G5","H5"},
	{"A4","B4","C4","D4","E4","F4","G4","H4"},
	{"A3","B3","C3","D3","E3","F3","G3","H3"},
	{"A2","B2","C2","D2","E2","F2","G2","H2"},
	{"A1","B1","C1","D1","E1","F1","G1","H1"},
	};

	int checker = 1;   // �ϼ�
	int run = 108;
	while (run > 0)
	{

		printing2(chess);
		char ent = '0';


		//  int player �� 1p 2p ���� ������ ����

	   /***************************************TAKING THE INPUT**********************************************/

		//player1 type move
		if (checker % 2 == 0)
		{
			printf("PLAYER 2 TYPE IN YOUR MOVE: \n");   //                "Player 1 is playing...."


			int whitesp = 0;
			int n = 0;
			int ck = 0;
			int ik = 0;
			int fk = 0;
			char d = '\0';
			char lin1[100];
			char lin2[100];
			char initv[100], finlv[100], help[100];

			int par = 0;
			for (par = 0; par < 100; par++)
			{
				lin1[par] = '\0';
				initv[par] = '\0';
				help[par] = '\0';
				finlv[par] = '\0';
			}


			int o, p;
			for (o = 0; o < 8; o++) {
				for (p = 0; p< 8; p++) {
					if (chess[0][o][p] == 'K' && chess[1][o][p] == '2')  //���� ŷ ��ġ ã��
						break;
				}
				if (chess[0][o][p] == 'K' && chess[1][o][p] == '2')  //���� ŷ ��ġ ã��
					break;
			}


			if (IsInCheck(o, p,chess) == 1) {
				printf("you are in check \n");
				if (Kcantmove(o, p,chess) == 1) {
					printf("King cant move \n");
				}

			}



			scanf(" %[^\n]s", lin2);    //        scanf �� �ڽ��� ������Ʈ�ϰ�   send_msg �� �ٸ� Ŭ���̾�Ʈ�� ����� ����.
			int sock = *((int*)arg);
			send(sock, lin2, 100, 0);
			recv(sock, lin1, 100, 0);
			//send(sock, "00", 100, 0);

			char *qq = "quit";
			if (!strcmp(lin1, qq)) {
				whowon = 1;
				// ���� �м� �߰�
				sprintf(query, "UPDATE user SET lose = lose+1 WHERE id = '%s'", chess_ID);                // mysql query

				if (mysql_query(&mysql, query)) {
					printf("%s\n", mysql_error(&mysql));
					exit(1);
				}
				break;
			}

			int low = 0;
			int len = strlen(lin1) - 1;

			for (low; low <= len; low++)
			{
				lin1[low] = toupper(lin1[low]);
			}

			for (n = 0; n <= len; n++)
			{
				if (lin1[n] == ' ' && n == 0)
					continue;

				if (lin1[n] == ' ' && ((lin1[n - 1] != ' ') && n != 0)) {
					whitesp++;
				}

				if (whitesp == 0 && (n == 0 || lin1[n - 1] == ' ') && lin1[n] != ' ')
					d = lin1[n];

				if (whitesp == 0 && lin1[n] != ' ') {
					help[ck] = lin1[n];          //Ư�������϶� help ������� �ν�
					ck++;
				}

				if (whitesp == 1 && lin1[n] != ' ') {
					initv[ik] = lin1[n];                                     //1 a2 a3   ���� 1�� ent, a2�� initv, a3�� finlv
					ik++;
				}

				if (whitesp == 2 && lin1[n] != ' ') {
					finlv[fk] = lin1[n];
					fk++;
				}

			}

			/*
				  printf("\n");                            The comments have been left
				  printf("%c \n", d);                      as such because people who are new
				  printf("%s \n", lin1 );                  to the source code can uncomment the
				  printf("%sa\n", help);                   statements and understand the significance
				  printf("%s \n", initv);                  and working of the variables.
				  printf("%s \n", finlv);
			*/



			/*************************************CALCULATING THE INDICES******************************************/
			//char *quit = "QUIT";
			int dep = 30;
			//dep = strcmp(help, quit);
			/*  printf("dep%d",dep) */;

			char *display = "HELP";
			int ped = 40;
			ped = strcmp(help, display);

			if (ped == 0)
			{
				printf("\n\n\n\n\n");
				printf("\t1 = PAWN\n\t2 = KNIGHT\n\t4 = BISHOP\n\t5 = ROOK\n\t7 = QUEEN\n\t9 = KING\n\n\n\n\n");

				printf("PRESS ENTER TO ESCAPE HELP:");
				char esc;

				if ((esc = getchar()) == 'h' || (esc = getchar()) == 'H')
					continue;

			}

			//if (dep == 0)
			//{
			//	printf("ENTER Y TO QUIT:");
			//	char y;

			//	if ((y = getchar()) == 'y' || (y = getchar()) == 'Y')
			//		goto END;
			//}


			ent = d;
			int error = 0;
			int i, j, k, l = 0;
			int check;

			for (i = 0; i <= 7; i++)
			{
				check = 100;

				for (j = 0; j <= 7; j++)
				{
					check = strcmp(ind[i][j], initv);          //ü���� Ž���� ã�� ���̸� Ž�� ����. i�� j�� �ش� ��ġ ���� ��.  ���⼭���� �ش� i,j ��ġ������ �ؿ� ��� Ȯ��.
					if (check == 0)
						break;
				};

				if (check == 0)
					break;
			}


			if (check != 0)
			{
				error++;
				printf("THERE IS NO INITIAL POSITION AS \"%s\"\n", initv);/*************ERROR ERROR*************/
			}


			for (k = 0; k <= 7; k++)
			{
				check = 100;

				for (l = 0; l <= 7; l++)
				{
					check = strcmp(ind[k][l], finlv);                //ü���� Ž���� �̵��� ���ϴ� ��ġ�� Ž�� ����. i�� j�� �ش� ��ġ ���� ��.  ���⼭���� �ش� k,i ��ġ������ �ؿ� ��� Ȯ��.
					if (check == 0)
						break;
				};

				if (check == 0)
					break;
			}

			if (check != 0)
			{
				error++;
				printf("THERE IS NO FINAL POSITION AS \"%s\"\n", finlv);/*************ERROR ERROR*************/
			}

			char king = chess[0][k][l];          //��ǥ�� �� ��ġ

			if (checker % 2 != 0 && chess[1][i][j] == '2')  // ����� ���� �������� 2 �̸�
			{
				error = 1;
				printf("PLAYER 2 YOU CANNOT ACCESS ARMY OF PLAYER 1\n");
			}


			else if (checker % 2 == 0 && chess[1][i][j] == '1')
			{
				error = 1;
				printf("PLAYER 1 YOU CANNOT ACCESS ARMY OF PLAYER 2\n");///////////////////////////////////
			}


			/*  system("clear");*/

			int c, v;
			for (c = 0; c < 8; c++) {
				for (v = 0; v < 8; v++) {
					if (chess[0][c][v] == 'K' && chess[1][c][v] == '2')  //���� ŷ ��ġ ã��
						break;
				}
				if (chess[0][c][v] == 'K' && chess[1][c][v] == '2')  //���� ŷ ��ġ ã��
					break;
			}
			printf("%d %d \n", c, v);
			if (IsInCheck(c, v,chess) == 1) {

				printf("player2 is in check\n");


			}


			if (error == 0)
			{
				error = compute(ent, i, j, k, l, 0, chess);           // ��ɾ��� compute ����
			}


			if (IsInCheck(c, v,chess) == 1) {
				takeback2(ent, i, j, k, l, 0, chess);
				error = 1;
				printf(" You are in check\n");
			}


			if (error == 0)
			{
				checker++;
				/*  printf("checker:%d  error a:%d\n",checker,error);*/
			}

			else if (error != 0)
			{
				/*  printf("checker:%d  error b:%d\n",checker,error);*/
			}

			if (king == 'K' && error == 0 && checker % 2 == 0)                   //üũ����Ʈ????  ŷ�� ����???
			{
				printf("PLAYER 1 HAS WON THE GAME\n");
				whowon = 1;

				// ���� �м� �߰�
				sprintf(query, "UPDATE user SET lose = lose+1 WHERE id = '%s'", chess_ID);                // mysql query

				if (mysql_query(&mysql, query)) {
					printf("%s\n", mysql_error(&mysql));
					exit(1);
				}



				break;
			}

			if (king == 'K' && error == 0 && checker % 2 != 0)
			{
				printf("PLAYER 2 HAS WON THE GAME\n");
				whowon = 2;

				//�� �߰�
				sprintf(query, "UPDATE user SET win = win+1 where id = '%s'", chess_ID);                // mysql query

				if (mysql_query(&mysql, query)) {
					printf("%s\n", mysql_error(&mysql));
					exit(1);
				}


				break;
			}
			printing2(chess);

			/*  printf("checker:%d  error c:%d\n",checker,error);*/

		}













		//player2 type move
		else if (checker % 2 != 0)
		{
			printf("PLAYER 1 MOVED: ");


			int whitesp = 0;
			int n = 0;
			int ck = 0;
			int ik = 0;
			int fk = 0;
			char d = '\0';
			char lin1[100];
			char initv[100], finlv[100], help[100];

			int par = 0;
			for (par = 0; par < 100; par++)
			{
				lin1[par] = '\0';
				initv[par] = '\0';
				help[par] = '\0';
				finlv[par] = '\0';
			}

			//scanf(" %[^\n]s", lin1);                         // scanf�� ���� ��� recv_msg��  �÷��̾�2 move ������Ʈ
			int sock = *((int*)arg);
			recv(sock, lin1, 100, 0);

			char *qq = "quit";
			if (!strcmp(lin1, qq)) {
				whowon = 2;
				//�� �߰�
				sprintf(query, "UPDATE user SET win = win+1 where id = '%s'", chess_ID);                // mysql query

				if (mysql_query(&mysql, query)) {
					printf("%s\n", mysql_error(&mysql));
					exit(1);
				}

				break;
			}

			printf("[%s] \n", lin1);

			int low = 0;
			int len = strlen(lin1) - 1;

			for (low; low <= len; low++)
			{
				lin1[low] = toupper(lin1[low]);
			}

			for (n = 0; n <= len; n++)
			{
				if (lin1[n] == ' ' && n == 0)
					continue;

				if (lin1[n] == ' ' && ((lin1[n - 1] != ' ') && n != 0)) {
					whitesp++;
				}

				if (whitesp == 0 && (n == 0 || lin1[n - 1] == ' ') && lin1[n] != ' ')



					d = lin1[n];


				if (whitesp == 0 && lin1[n] != ' ') {
					help[ck] = lin1[n];          //Ư�������϶� help ������� �ν�
					ck++;
				}

				if (whitesp == 1 && lin1[n] != ' ') {


					if (lin1[n] == 'A')
						lin1[n] = 'H';
					else if (lin1[n] == 'B')
						lin1[n] = 'G';
					else if (lin1[n] == 'C')
						lin1[n] = 'F';
					else if (lin1[n] == 'D')
						lin1[n] = 'E';
					else if (lin1[n] == 'E')                               // ��ġ ����
						lin1[n] = 'D';
					else if (lin1[n] == 'F')
						lin1[n] = 'C';
					else if (lin1[n] == 'G')
						lin1[n] = 'B';
					else if (lin1[n] == 'H')
						lin1[n] = 'A';
					else if (lin1[n] == '1')
						lin1[n] = '8';
					else if (lin1[n] == '2')
						lin1[n] = '7';
					else if (lin1[n] == '3')
						lin1[n] = '6';
					else if (lin1[n] == '4')
						lin1[n] = '5';
					else if (lin1[n] == '5')
						lin1[n] = '4';
					else if (lin1[n] == '6')
						lin1[n] = '3';
					else if (lin1[n] == '7')
						lin1[n] = '2';
					else if (lin1[n] == '8')
						lin1[n] = '1';



					initv[ik] = lin1[n];                                     //1 a2 a3   ���� 1�� ent, a2�� initv, a3�� finlv
					ik++;
				}

				if (whitesp == 2 && lin1[n] != ' ') {


					if (lin1[n] == 'A')
						lin1[n] = 'H';
					else if (lin1[n] == 'B')
						lin1[n] = 'G';
					else if (lin1[n] == 'C')
						lin1[n] = 'F';
					else if (lin1[n] == 'D')
						lin1[n] = 'E';
					else if (lin1[n] == 'E')                               // ��ġ ����
						lin1[n] = 'D';
					else if (lin1[n] == 'F')
						lin1[n] = 'C';
					else if (lin1[n] == 'G')
						lin1[n] = 'B';
					else if (lin1[n] == 'H')
						lin1[n] = 'A';
					else if (lin1[n] == '1')
						lin1[n] = '8';
					else if (lin1[n] == '2')
						lin1[n] = '7';
					else if (lin1[n] == '3')
						lin1[n] = '6';
					else if (lin1[n] == '4')
						lin1[n] = '5';
					else if (lin1[n] == '5')
						lin1[n] = '4';
					else if (lin1[n] == '6')
						lin1[n] = '3';
					else if (lin1[n] == '7')
						lin1[n] = '2';
					else if (lin1[n] == '8')
						lin1[n] = '1';



					finlv[fk] = lin1[n];
					fk++;
				}

			}

			//printf("[%d %s %s] \n", d, initv, finlv);

			/*
				  printf("\n");                            The comments have been left
				  printf("%c \n", d);                      as such because people who are new
				  printf("%s \n", lin1 );                  to the source code can uncomment the
				  printf("%sa\n", help);                   statements and understand the significance
				  printf("%s \n", initv);                  and working of the variables.
				  printf("%s \n", finlv);
			*/



			/*************************************CALCULATING THE INDICES******************************************/
			//char *quit = "QUIT";
			int dep = 30;
			//dep = strcmp(help, quit);
			/*  printf("dep%d",dep) */;

			char *display = "HELP";
			int ped = 40;
			ped = strcmp(help, display);

			if (ped == 0)
			{
				printf("\n\n\n\n\n");
				printf("\t1 = PAWN\n\t2 = KNIGHT\n\t4 = BISHOP\n\t5 = ROOK\n\t7 = QUEEN\n\t9 = KING\n\n\n\n\n");

				printf("PRESS ENTER TO ESCAPE HELP:");
				char esc;

				if ((esc = getchar()) == 'h' || (esc = getchar()) == 'H')
					continue;

			}

			//if (dep == 0)
			//{
			//	printf("ENTER Y TO QUIT:");
			//	char y;

			//	if ((y = getchar()) == 'y' || (y = getchar()) == 'Y')
			//		goto END;
			//}


			ent = d;
			int error = 0;
			int i, j, k, l = 0;
			int check;

			for (i = 0; i <= 7; i++)
			{
				check = 100;

				for (j = 0; j <= 7; j++)
				{
					check = strcmp(ind[i][j], initv);          //ü���� Ž���� ã�� ���̸� Ž�� ����. i�� j�� �ش� ��ġ ���� ��.  ���⼭���� �ش� i,j ��ġ������ �ؿ� ��� Ȯ��.
					if (check == 0)
						break;
				};

				if (check == 0)
					break;
			}


			if (check != 0)
			{
				error++;
				printf("THERE IS NO INITIAL POSITION AS \"%s\"\n", initv);/*************ERROR ERROR*************/
			}


			for (k = 0; k <= 7; k++)
			{
				check = 100;

				for (l = 0; l <= 7; l++)
				{
					check = strcmp(ind[k][l], finlv);                //ü���� Ž���� �̵��� ���ϴ� ��ġ�� Ž�� ����. i�� j�� �ش� ��ġ ���� ��.  ���⼭���� �ش� k,i ��ġ������ �ؿ� ��� Ȯ��.
					if (check == 0)
						break;
				};

				if (check == 0)
					break;
			}

			if (check != 0)
			{
				error++;
				printf("THERE IS NO FINAL POSITION AS \"%s\"\n", finlv);/*************ERROR ERROR*************/
			}

			char king = chess[0][k][l];          //��ǥ�� �� ��ġ

			if (checker % 2 != 0 && chess[1][i][j] == '2')  // ����� ���� �������� 2 �̸�
			{
				error = 1;
				printf("PLAYER 2 YOU CANNOT ACCESS ARMY OF PLAYER 1\n");
			}

			else if (checker % 2 == 0 && chess[1][i][j] == '1')
			{
				error = 1;
				printf("PLAYER 1 YOU CANNOT ACCESS ARMY OF PLAYER 2\n");
			}


			/*  system("clear");*/


			int c, v;
			for (c = 0; c < 8; c++) {
				for (v = 0; v < 8; v++) {
					if (chess[0][c][v] == 'K' && chess[1][c][v] == '1')  //����� ŷ ��ġ ã��
						break;
				}
				if (chess[0][c][v] == 'K' && chess[1][c][v] == '1')  //����� ŷ ��ġ ã��
					break;
			}
			printf("%d %d \n", c, v);
			if (IsInCheck2(c, v, chess) == 1) {

				//printf("player1 is in check\n");


			}



			if (error == 0)
			{
				error = compute(ent, i, j, k, l, 0, chess);           // ��ɾ��� compute ����
			}

			
			if (IsInCheck2(c, v, chess) == 1) {
				takeback2(ent, i, j, k, l, 0, chess);
				error = 1;
				//printf(" You are in check\n");
			}




			if (error == 0)
			{
				checker++;
				/*  printf("checker:%d  error a:%d\n",checker,error);*/
			}

			else if (error != 0)
			{
				/*  printf("checker:%d  error b:%d\n",checker,error);*/
			}

			if (king == 'K' && error == 0 && checker % 2 == 0)                   //üũ����Ʈ????  ŷ�� ����???
			{
				printf("PLAYER 1 HAS WON THE GAME\n");
				whowon = 1;

				// ���� �м� �߰�
				// ���� �м� �߰�
				sprintf(query, "UPDATE user SET lose = lose+1 WHERE id = '%s'", chess_ID);                // mysql query

				if (mysql_query(&mysql, query)) {
					printf("%s\n", mysql_error(&mysql));
					exit(1);
				}

				break;
			}

			if (king == 'K' && error == 0 && checker % 2 != 0)
			{
				printf("PLAYER 2 HAS WON THE GAME\n");
				whowon = 2;
			
				//�� �߰�
				sprintf(query, "UPDATE user SET win = win+1 where id = '%s'", chess_ID);                // mysql query

				if (mysql_query(&mysql, query)) {
					printf("%s\n", mysql_error(&mysql));
					exit(1);
				}

				break;
			}

			/*  printf("checker:%d  error c:%d\n",checker,error);*/

		}

	}
END:
	return whowon;
}




















int compute(char ent1, int i1, int j1, int k1, int l1, int cond, char chess[2][8][8])
/*cond means condition for stopping printf statements
 *when cond ==1 do not print  */
{
	int q = 0;
	switch (ent1)
	{
	case 'P':/************************************* RULES FOR PAWN************************************************/
		switch (chess[0][i1][j1])
		{
		case'P': /*CHECKS WHETHER PAWN IS AVAILABLE OR NOT*/
		{
			if (chess[1][i1][j1] == '1')/*FOR UPPER PLAYER*/
			{
				if ((chess[1][k1][l1] == '1' && l1 == j1) || (i1 == 1 && chess[1][2][j1] == '1' && j1 == l1))
				{
					q = 1;
					if (cond == 0)
						printf("PLAYER 2 YOU CAN NOT OVERRUN YOUR ARMY\n");/************ERROR ERROR***************/
				}

				else if (chess[1][2][j1] == '2')
				{
					q = 1;
					if (cond == 0)
						printf("PLAYER 2 PAWN CAN'T JUMP OVER THE ENEMY\n");/************ERROR ERROR***************/
				}


				else
				{
					if (i1 == 1 && (k1 == i1 + 1 || k1 == i1 + 2) && l1 == j1)/*WHEN THE PAWN IS AT ITS INITIAL POSITION*/
					{
						if (chess[1][k1][l1] == ' ')
						{
							chess[0][k1][l1] = chess[0][i1][j1];
							chess[1][k1][l1] = chess[1][i1][j1];
							chess[0][i1][j1] = chess[1][i1][j1] = ' ';
						}

						else
						{
							q = 1;
							if (cond == 0)
								printf("ILLEGAL MOVE\n");/************ERROR ERROR***************/

						}

					}


					else if (k1 == i1 + 1 && l1 == j1)/*WHEN PAWN IS ANYWHERE ELSE*/
					{
						if (chess[1][k1][l1] == ' ')
						{
							chess[0][k1][l1] = chess[0][i1][j1];
							chess[1][k1][l1] = chess[1][i1][j1];
							chess[0][i1][j1] = chess[1][i1][j1] = ' ';
						}

						else
						{
							q = 1;
							if (cond == 0)
								printf("ILLEGAL MOVE\n");/************ERROR ERROR***************/
						}

					}

					else if (k1 == i1 + 1 && (l1 == j1 - 1 || l1 == j1 + 1))/*WHEN PAWN IS ANYWHERE ELSE*/
					{
						if (chess[1][k1][l1] == '2')
						{
							restorecolor = chess[1][k1][l1];
							restorekind = chess[0][k1][l1];
							restorecolor2 = chess[1][k1][l1];
							restorekind2 = chess[0][k1][l1];
							chess[0][k1][l1] = chess[0][i1][j1];
							chess[1][k1][l1] = chess[1][i1][j1];
							chess[0][i1][j1] = chess[1][i1][j1] = ' ';
						}

						else
						{
							q = 1;
							if (cond == 0)
								printf("ILLEGAL MOVE\n");/************ERROR ERROR***************/

						}
					}


					else
					{
						q = 1;
						if (cond == 0)
							printf("PLAYER 2 ILLEGAL MOVE\n");/************ERROR ERROR***************/

					}

				}

			}

			else if (chess[1][i1][j1] == '2')/*FOR LOWER PLAYER*/
			{
				if ((chess[1][k1][l1] == '2' && l1 == j1) || (i1 == 6 && chess[1][5][j1] == '2' && j1 == l1))
				{
					q = 1;
					if (cond == 0)
						printf("PLAYER 1 YOU CAN NOT OVERRUN YOUR ARMY\n");/************ERROR ERROR***************/
				}

				else if (chess[1][5][j1] == '1')
				{
					q = 1;
					if (cond == 0)
						printf("PLAYER 1 PAWN CAN'T JUMP OVER THE ENEMY\n");/************ERROR ERROR***************/
				}


				else
				{
					if (i1 == 6 && (k1 == i1 - 1 || k1 == i1 - 2) && l1 == j1)/*WHEN THE PAWN IS AT ITS INITIAL POSITION*/
					{
						if (chess[1][k1][l1] == ' ')
						{
							chess[0][k1][l1] = chess[0][i1][j1];
							chess[1][k1][l1] = chess[1][i1][j1];
							chess[0][i1][j1] = chess[1][i1][j1] = ' ';
						}

						else
						{
							q = 1;
							if (cond == 0)
								printf("ILLEGAL MOVE\n");/************ERROR ERROR***************/

						}

					}

					else if (k1 == i1 - 1 && l1 == j1)/*WHEN PAWN IS ANYWHERE ELSE*/
					{
						if (chess[1][k1][l1] == ' ')
						{
							chess[0][k1][l1] = chess[0][i1][j1];
							chess[1][k1][l1] = chess[1][i1][j1];
							chess[0][i1][j1] = chess[1][i1][j1] = ' ';
						}

						else
						{
							q = 1;
							if (cond == 0)
								printf("ILLEGAL MOVE\n");/************ERROR ERROR***************/
						}

					}

					else if (k1 == i1 - 1 && (l1 == j1 - 1 || l1 == j1 + 1))/*WHEN PAWN IS ANYWHERE ELSE*/
					{
						if (chess[1][k1][l1] == '1')
						{
							restorecolor = chess[1][k1][l1];
							restorekind = chess[0][k1][l1];
							restorecolor2 = chess[1][k1][l1];
							restorekind2 = chess[0][k1][l1];
							chess[0][k1][l1] = chess[0][i1][j1];
							chess[1][k1][l1] = chess[1][i1][j1];
							chess[0][i1][j1] = chess[1][i1][j1] = ' ';
						}

						else
						{
							q = 1;
							if (cond == 0)
								printf("ILLEGAL MOVE\n");/************ERROR ERROR***************/
						}
					}


					else
					{
						q = 1;
						if (cond == 0)
							printf("YOUR MOVE IS ILLEGAL\n");/************ERROR ERROR***************/
					}

				}
			}

			break; }/*BREAK OF CASE 1*/



		default:
		{  q = 1;
		if (cond == 0)
			printf("PAWN IS NOT AT THE SPECIFIED POSITION\n");/************ERROR ERROR***************/
		}

		}break;/**********************************************END OF PAWN***********************************/



	case 'N': /******************************************RULES FOR KNIGHT*********************************/
		switch (chess[0][i1][j1])
		{
		case 'N':/***CHECKS WHETHER KNIGHT IS THERE OR NOT******/
		{
			if ((k1 == i1 + 2 || k1 == i1 - 2) && (k1 >= 0 && k1 <= 7) && (l1 == j1 + 1 || l1 == j1 - 1) && (l1 >= 0 && l1 <= 7))
			{/*******FOR VERTICAL MOVEMENT*************/
				if (chess[1][i1][j1] == chess[1][k1][l1])
				{
					q = 1;
					if (cond == 0)
						printf("YOU CANNOT OVERRUN YOUR ARMY\n"); /************ERROR ERROR***************/
				}


				else
				{
					restorecolor = chess[1][k1][l1];
					restorekind = chess[0][k1][l1];
					restorecolor2 = chess[1][k1][l1];
					restorekind2 = chess[0][k1][l1];
					chess[0][k1][l1] = chess[0][i1][j1];
					chess[1][k1][l1] = chess[1][i1][j1];
					chess[0][i1][j1] = chess[1][i1][j1] = ' ';

				}

			}


			else if ((k1 == i1 + 1 || k1 == i1 - 1) && (k1 >= 0 && k1 <= 7) && (l1 == j1 + 2 || l1 == j1 - 2) && (l1 >= 0 && l1 <= 7))
			{/*********FOR HORIZONTAL MOVEMENT***********/
				if (chess[1][i1][j1] == chess[1][k1][l1])
				{
					q = 1;
					if (cond == 0)
						printf("YOU CANNOT OVERRUN YOUR ARMY\n");/************ERROR ERROR***************/
				}


				else
				{
					restorecolor = chess[1][k1][l1];
					restorekind = chess[0][k1][l1];
					restorecolor2 = chess[1][k1][l1];
					restorekind2 = chess[0][k1][l1];
					chess[0][k1][l1] = chess[0][i1][j1];
					chess[1][k1][l1] = chess[1][i1][j1];
					chess[0][i1][j1] = chess[1][i1][j1] = ' ';

				}

			}

			else
			{
				q = 1;
				if (cond == 0)
					printf("YOUR KNIGHT MOVE IS ILLEGAL\n");/************ERROR ERROR***************/
			}

			break; }/*BREAK OF CASE 2*/


		default:
		{
			q = 1;
			if (cond == 0)
				printf("KNIGHT IS NOT AT THE SPECIFIED POSITION\n");/************ERROR ERROR***************/
		}

		}break;/***********************************END OF KNIGHT********************************************/



	case'3':/***************************************RULE OF NO-ENTITY 3 ****************************************/
	{
		q = 1;
		if (cond == 0)
			printf("THERE IS NO ENTITY 3\n");/************ERROR ERROR***************/

	}break;/***********************************END OF NO-ENTITY 3 ****************************************/



	case'B':/****************************************RULES OF BISHOP****************************************/
		switch (chess[0][i1][j1])
		{
		case 'B': /***CHECKS WHETHER BISHOP IS THERE OR NOT******/
		{    int bis;
		bis = k1 - i1;
		/*   printf("%d\n",bis);*/

		if (l1 == bis + j1 || l1 == (-1)*bis + j1)
		{
			int var = bis - 1;


			if (bis < 0) {
				var = (-1)*bis - 1;
			}

			for (var; var > 0; var--)
			{
				if (bis > 0 && l1 > j1)
				{
					if (chess[1][i1 + var][j1 + var] == ' ')
						continue;

					else
					{
						q = 1;
						if (cond == 0)
							printf("BISHOP CAN'T JUMP\n");/*******ERROR ERROR*********/
						break;
					}
				}

				else if (bis > 0 && l1 < j1)
				{
					if (chess[1][i1 + var][j1 - var] == ' ')
						continue;

					else
					{
						q = 1;
						if (cond == 0)
							printf("BISHOP CAN'T JUMP\n");/*******ERROR ERROR*********/
						break;
					}
				}

				else if (bis < 0 && l1 > j1)
				{
					if (chess[1][i1 - var][j1 + var] == ' ')
						continue;

					else
					{
						q = 1;
						if (cond == 0)
							printf("BISHOP CAN'T JUMP\n");/*******ERROR ERROR*********/
						break;

					}
				}

				if (bis < 0 && l1 < j1)
				{
					if (chess[1][i1 - var][j1 - var] == ' ')
						continue;

					else
					{
						q = 1;
						if (cond == 0)
							printf("BISHOP CAN'T JUMP\n");/*******ERROR ERROR*********/
						break;

					}
				}

			}


			if (chess[1][i1][j1] == chess[1][k1][l1])
			{
				q = 1;
				var = 1;
				if (cond == 0)
					printf("YOU CANNOT OVERRUN YOUR ARMY\n"); /************ERROR ERROR***************/
			}


			if (var == 0)
			{
				restorecolor = chess[1][k1][l1];
				restorekind = chess[0][k1][l1];
				restorecolor2 = chess[1][k1][l1];
				restorekind2 = chess[0][k1][l1];
				chess[0][k1][l1] = chess[0][i1][j1];
				chess[1][k1][l1] = chess[1][i1][j1];
				chess[0][i1][j1] = chess[1][i1][j1] = ' ';

			}

		}

		else
		{
			q = 1;
			if (cond == 0)
				printf("INVALID MOVE OF BISHOP\n"); /************ERROR ERROR***************/
		}





		break; }/***END OF CASE 4 ***********/

		default:
		{   q = 1;
		if (cond == 0)
			printf("BISHOP IS NOT AT THE SPECIFIED POSITION\n"); /************ERROR ERROR***************/
		}


		}break; /*********************************END OF BISHOP**************************************/



	case 'R': /******************************************RULES FOR ROOK*********************************/
		switch (chess[0][i1][j1])
		{
		case 'R':/***CHECKS WHETHER ROOK IS THERE OR NOT******/
		{
			if (l1 == j1)
			{/*******FOR VERTICAL MOVEMENT*************/
				int rip;
				rip = k1 - i1;
				int vap = rip - 1;

				if (rip < 0)
				{
					vap = (-1)*rip - 1;
				}

				for (vap; vap > 0; vap--)
				{
					if (rip > 0)
					{
						if (chess[1][i1 + vap][j1] == ' ')
							continue;

						else
						{
							q = 1;
							if (cond == 0)
								printf("ROOK CAN'T JUMP\n");/*******ERROR ERROR*********/
							break;
						}
					}

					else if (rip < 0)
					{
						if (chess[1][i1 - vap][j1] == ' ')
							continue;

						else
						{
							q = 1;
							if (cond == 0)
								printf("ROOK CAN'T JUMP\n");/*******ERROR ERROR*********/
							break;
						}
					}

				}

				if (chess[1][i1][j1] == chess[1][k1][l1])
				{
					q = 1;
					vap = 1;
					if (cond == 0)
						printf("YOU CANNOT OVERRUN YOUR ARMY\n");/************ERROR ERROR***************/
				}


				if (vap == 0)
				{
					restorecolor = chess[1][k1][l1];
					restorekind = chess[0][k1][l1];
					restorecolor2 = chess[1][k1][l1];
					restorekind2 = chess[0][k1][l1];
					chess[0][k1][l1] = chess[0][i1][j1];
					chess[1][k1][l1] = chess[1][i1][j1];
					chess[0][i1][j1] = chess[1][i1][j1] = ' ';

				}

			}

			else if (k1 == i1)
			{/******FOR HORIZONTAL MOVEMENT*************/
				int rhip;
				rhip = l1 - j1;
				int vhap = rhip - 1;

				if (rhip < 0)
				{
					vhap = (-1)*rhip - 1;
				}

				for (vhap; vhap > 0; vhap--)
				{
					if (rhip > 0)
					{
						if (chess[1][i1][j1 + vhap] == ' ')
							continue;

						else
						{
							q = 1;
							if (cond == 0)
								printf("ROOK CAN'T JUMP\n");/*******ERROR ERROR*********/
							break;
						}
					}

					else if (rhip < 0)
					{
						if (chess[1][i1][j1 - vhap] == ' ')
							continue;

						else
						{
							q = 1;
							if (cond == 0)
								printf("ROOK CAN'T JUMP\n");/*******ERROR ERROR*********/
							break;
						}
					}


				}

				if (chess[1][i1][j1] == chess[1][k1][l1])
				{
					q = 1;
					vhap = 1;
					if (cond == 0)
						printf("YOU CANNOT OVERRUN YOUR ARMY\n"); /************ERROR ERROR***************/
				}


				if (vhap == 0)
				{
					restorecolor = chess[1][k1][l1];
					restorekind = chess[0][k1][l1];
					restorecolor2 = chess[1][k1][l1];
					restorekind2 = chess[0][k1][l1];
					chess[0][k1][l1] = chess[0][i1][j1];
					chess[1][k1][l1] = chess[1][i1][j1];
					chess[0][i1][j1] = chess[1][i1][j1] = ' ';

				}

			}

			else
			{
				q = 1;
				if (cond == 0)
					printf("INVALID MOVE FOR ROOK\n"); /*******ERROR ERROR*********/

			}


			break; }/***END OF CASE 5 ***********/


		default:
		{
			q = 1;
			if (cond == 0)
				printf("ROOK IS NOT AT THE SPECIFIED POSITION\n");/***********ERROR ERROR**************/
		}

		}break; /*******************************END OF ROOK**************************************/




	case'6':/***************************************RULE OF NO-ENTITY 6 ****************************************/
	{
		q = 1;
		if (cond == 0)
			printf("THERE IS NO ENTITY 6\n");/************ERROR ERROR***************/

	}break;/***********************************END OF NO-ENTITY 6 ****************************************/





	case'Q':/***************************************RULE OF QUEEN ****************************************/
		switch (chess[0][i1][j1])/***CHECKS WHETHER QUEEN IS THERE OR NOT******/
		{
		case'Q':
		{

			if (l1 == j1)
			{/*******FOR VERTICAL MOVEMENT*************/
				int rip;
				rip = k1 - i1;
				int vap = rip - 1;

				if (rip < 0)
				{
					vap = (-1)*rip - 1;
				}

				for (vap; vap > 0; vap--)
				{
					if (rip > 0)
					{
						if (chess[1][i1 + vap][j1] == ' ')
							continue;

						else
						{
							q = 1;
							if (cond == 0)
								printf("QUEEN CAN'T JUMP\n");/*******ERROR ERROR*********/
							break;
						}
					}

					else if (rip < 0)
					{
						if (chess[1][i1 - vap][j1] == ' ')
							continue;

						else
						{
							q = 1;
							if (cond == 0)
								printf("QUEEN CAN'T JUMP\n");/*******ERROR ERROR*********/
							break;
						}
					}

				}

				if (chess[1][i1][j1] == chess[1][k1][l1])
				{
					q = 1;
					vap = 1;
					if (cond == 0)
						printf("YOU CANNOT OVERRUN YOUR ARMY\n");/************ERROR ERROR***************/
				}


				if (vap == 0)
				{
					restorecolor = chess[1][k1][l1];
					restorekind = chess[0][k1][l1];
					restorecolor2 = chess[1][k1][l1];
					restorekind2 = chess[0][k1][l1];
					chess[0][k1][l1] = chess[0][i1][j1];
					chess[1][k1][l1] = chess[1][i1][j1];
					chess[0][i1][j1] = chess[1][i1][j1] = ' ';

				}

			}

			else if (k1 == i1)
			{/******FOR HORIZONTAL MOVEMENT*************/
				int rhip;
				rhip = l1 - j1;
				int vhap = rhip - 1;

				if (rhip < 0)
				{
					vhap = (-1)*rhip - 1;
				}

				for (vhap; vhap > 0; vhap--)
				{
					if (rhip > 0)
					{
						if (chess[1][i1][j1 + vhap] == ' ')
							continue;

						else
						{
							q = 1;
							if (cond == 0)
								printf("QUEEN CAN'T JUMP\n");/*******ERROR ERROR*********/
							break;
						}
					}

					else if (rhip < 0)
					{
						if (chess[1][i1][j1 - vhap] == ' ')
							continue;

						else
						{
							q = 1;
							if (cond == 0)
								printf("QUEEN CAN'T JUMP\n");/*******ERROR ERROR*********/
							break;
						}
					}


				}

				if (chess[1][i1][j1] == chess[1][k1][l1])
				{
					q = 1;
					vhap = 1;
					if (cond == 0)
						printf("YOU CANNOT OVERRUN YOUR ARMY\n"); /************ERROR ERROR***************/
				}


				if (vhap == 0)
				{
					restorecolor = chess[1][k1][l1];
					restorekind = chess[0][k1][l1];
					restorecolor2 = chess[1][k1][l1];
					restorekind2 = chess[0][k1][l1];
					chess[0][k1][l1] = chess[0][i1][j1];
					chess[1][k1][l1] = chess[1][i1][j1];
					chess[0][i1][j1] = chess[1][i1][j1] = ' ';

				}

			}


			else
			{
				int bis;
				bis = k1 - i1;
				/*printf("%d\n",bis);*/

				if (l1 == bis + j1 || l1 == (-1)*bis + j1)
				{
					int var = bis - 1;


					if (bis < 0) {
						var = (-1)*bis - 1;
					}

					for (var; var > 0; var--)
					{
						if (bis > 0 && l1 > j1)
						{
							if (chess[1][i1 + var][j1 + var] == ' ')
								continue;

							else
							{
								q = 1;
								if (cond == 0)
									printf("QUEEN CAN'T JUMP\n");/*******ERROR ERROR*********/
								break;
							}
						}

						else if (bis > 0 && l1 < j1)
						{
							if (chess[1][i1 + var][j1 - var] == ' ')
								continue;

							else
							{
								q = 1;
								if (cond == 0)
									printf("QUEEN CAN'T JUMP\n");/*******ERROR ERROR*********/
								break;
							}
						}

						else if (bis < 0 && l1 > j1)
						{
							if (chess[1][i1 - var][j1 + var] == ' ')
								continue;

							else
							{
								q = 1;
								if (cond == 0)
									printf("QUEEN CAN'T JUMP\n");/*******ERROR ERROR*********/
								break;

							}
						}

						if (bis < 0 && l1 < j1)
						{
							if (chess[1][i1 - var][j1 - var] == ' ')
								continue;

							else
							{
								q = 1;
								if (cond == 0)
									printf("QUEEN CAN'T JUMP\n");/*******ERROR ERROR*********/
								break;

							}
						}

					}


					if (chess[1][i1][j1] == chess[1][k1][l1])
					{
						q = 1;
						var = 1;
						if (cond == 0)
							printf("YOU CANNOT OVERRUN YOUR ARMY\n"); /************ERROR ERROR***************/
					}


					if (var == 0)
					{
						restorecolor = chess[1][k1][l1];
						restorekind = chess[0][k1][l1];
						restorecolor2 = chess[1][k1][l1];
						restorekind2 = chess[0][k1][l1];
						chess[0][k1][l1] = chess[0][i1][j1];
						chess[1][k1][l1] = chess[1][i1][j1];
						chess[0][i1][j1] = chess[1][i1][j1] = ' ';

					}

				}

				else
				{
					q = 1;
					if (cond == 0)
						printf("INVALID MOVE FOR QUEEN\n");/*******ERROR ERROR*********/
				}

			}


			break; }/********END OF CASE 7 ************/



		default:
		{
			q = 1;
			if (cond == 0)
				printf("QUEEN IS NOT AT THE SPECIFIED POSITION\n");/***********ERROR ERROR**************/
		}

		}break;/***********************************END OF QUEEN ****************************************/




	case'K':/***************************************RULE OF KING ****************************************/
		switch (chess[0][i1][j1])/***CHECKS WHETHER QUEEN IS THERE OR NOT******/
		{
		case'K':
		{

			if ((k1 == i1 + 1 || k1 == i1 - 1) && (l1 == j1 || l1 == j1 - 1 || l1 == j1 + 1))
			{

				if (chess[1][i1][j1] == chess[1][k1][l1])
				{
					q = 1;
					if (cond == 0)
						printf("YOU CANNOT OVERRUN YOUR ARMY\n"); /************ERROR ERROR***************/
				}


				else
				{
					restorecolor = chess[1][k1][l1];
					restorekind = chess[0][k1][l1];
					restorecolor2 = chess[1][k1][l1];
					restorekind2 = chess[0][k1][l1];
					chess[0][k1][l1] = chess[0][i1][j1];
					chess[1][k1][l1] = chess[1][i1][j1];
					chess[0][i1][j1] = chess[1][i1][j1] = ' ';

				}


			}

			else if ((k1 == i1) && (l1 == j1 - 1 || l1 == j1 + 1))
			{

				if (chess[1][i1][j1] == chess[1][k1][l1])
				{
					q = 1;
					if (cond == 0)
						printf("YOU CANNOT OVERRUN YOUR ARMY\n"); /************ERROR ERROR***************/
				}


				else
				{
					restorecolor = chess[1][k1][l1];
					restorekind = chess[0][k1][l1];
					restorecolor2 = chess[1][k1][l1];
					restorekind2 = chess[0][k1][l1];
					chess[0][k1][l1] = chess[0][i1][j1];
					chess[1][k1][l1] = chess[1][i1][j1];
					chess[0][i1][j1] = chess[1][i1][j1] = ' ';

				}


			}


			else
			{
				q = 1;
				if (cond == 0)
					printf("ILLEGAL MOVE FOR KING\n"); /************ERROR ERROR***************/
			}



			break; }/********END OF CASE 9 ************/


		default:
		{
			q = 1;
			if (cond == 0)
				printf("KING IS NOT AT THE SPECIFIED POSITION\n");/***********ERROR ERROR**************/
		}


		}break;/***********************************END OF KING **************************************/


	default: {
		if (cond == 0)
			printf("OTHER THAN THE ALLOWED NUMBERS , CHARACTERS NOT ALLOWED AS ENTITY\nLOOK INTO HELP\n");/************ERROR ERROR***************/
	}

	}/*****************END OF SWITCH(ENTITY)************************/

	return q;

}/*END OF COMPUTE*/

void printing(char chp[2][8][8])
{
	int a, b;
	a = b = 0;
	int  len, brd, buffer;   /*len = length & brd = breadth*/
	brd = len = 8;
	printf("\n\n\n\n\n");

	printf("                          \x1b[31m PLAYER 2 Black\x1b[0m                         \n");
	//printf("           1      2      3      4      5      6      7      8    \n");
	buffer = len;
	printf("       --");

	for (len; len > 0; len--) {
		printf("-------");
	}

	printf("\n");
	len = buffer;

	int i;
	//char alpha = 'A';
	int numbers = 8;
	int vert = 0;
	for (brd; brd > 0; brd--)
	{
		a = 8 - brd;
		i = 3;
		if (i == 3)
		{
			len = buffer;
			int hor = 0;

			printf("       |");
			for (len; len > 0; len--)
			{
				if ((hor + vert) % 2 == 0)
				{
					printf("       ");
				}

				else if ((hor + vert) % 2 != 0)
				{
					printf("\u2588\u2588\u2588\u2588\u2588\u2588\u2588");     /* the characters inside the print statement*/
				}                                                           /* are unicodes used for printing the chess grid*/
				hor++;
			}

			printf("|");
			printf("\n");
			i--;
		}

		if (i == 2)
		{
			len = buffer;
			int hor = 0;
			printf("     %d |", numbers);

			for (len; len > 0; len--)
			{
				b = 8 - len;
				if ((hor + vert) % 2 == 0)
				{
					if (chp[1][a][b] == '1')
					{
						printf("   \x1b[31m%c\x1b[0m   ", chp[0][a][b]);                   /*unicode*/
					}

					if (chp[1][a][b] == ' ')
					{
						printf("       ");
					}

					if (chp[1][a][b] == '2')
					{
						printf("   \x1b[32m%c\x1b[0m   ", chp[0][a][b]);                   /*unicode*/
					}
				}

				else if ((hor + vert) % 2 != 0)
				{
					if (chp[1][a][b] == '1')
					{
						printf("\u2588\u2588\u258C\x1b[31m%c\x1b[0m\u2590\u2588\u2588", chp[0][a][b]);
					}

					if (chp[1][a][b] == '2')
					{
						printf("\u2588\u2588\u258C\x1b[32m%c\x1b[0m\u2590\u2588\u2588", chp[0][a][b]);
					}

					if (chp[1][a][b] == ' ')
					{
						printf("\u2588\u2588\u2588\u2588\u2588\u2588\u2588", chp[0][a][b]);
					}

				}
				hor++;

			}

			printf("|");
			printf("\n");
			i--;
		}

		if (i == 1)
		{
			len = buffer;
			printf("       |");
			int hor = 0;

			for (len; len > 0; len--)
			{
				if ((hor + vert) % 2 == 0)
				{
					printf("       ");
				}

				else if ((hor + vert) % 2 != 0)
				{
					printf("\u2588\u2588\u2588\u2588\u2588\u2588\u2588");
				}
				hor++;

			}

			printf("|");
			printf("\n");
			i--;
		}

		numbers--;
		vert++;
	}


	len = buffer;
	printf("       --");

	for (len; len > 0; len--) {
		printf("-------");
	}
	printf("\n");

	printf("           A      B      C      D      E      F      G      H    \n");
	printf("                          \x1b[32mPLAYER 1 White\x1b[0m                         \n\n");
	printf("       '1'=>PAWN        '2'=>KNIGHT          '4'=>BISHOP\n       '5'=>ROOK        '7'=>QUEEN           '9'=>KING");
	printf("\n\n");


}


void printing2(char chp[2][8][8])
{
	int a, b;
	a = b = 0;
	int  len, brd, buffer;   /*len = length & brd = breadth*/
	brd = len = 8;
	printf("\n\n\n\n\n");

	printf("                          \x1b[32m PLAYER 1 White\x1b[0m                         \n");
	//printf("           1      2      3      4      5      6      7      8    \n");
	buffer = len;
	printf("       --");

	for (len; len > 0; len--) {
		printf("-------");
	}

	printf("\n");
	len = buffer;

	int i;
	//char alpha = 'A';
	int numbers = 8;
	int vert = 0;
	for (brd; brd > 0; brd--)
	{
		a = 8 - brd;
		i = 3;
		if (i == 3)
		{
			len = buffer;
			int hor = 0;

			printf("       |");
			for (len; len > 0; len--)
			{
				if ((hor + vert) % 2 == 0)
				{
					printf("       ");
				}

				else if ((hor + vert) % 2 != 0)
				{
					printf("\u2588\u2588\u2588\u2588\u2588\u2588\u2588");     /* the characters inside the print statement*/
				}                                                           /* are unicodes used for printing the chess grid*/
				hor++;
			}

			printf("|");
			printf("\n");
			i--;
		}

		if (i == 2)
		{
			len = buffer;
			int hor = 0;
			printf("     %d |", numbers);

			for (len; len > 0; len--)
			{
				b = 8 - len;
				if ((hor + vert) % 2 == 0)
				{
					if (chp[1][a][b] == '1')
					{
						printf("   \x1b[32m%c\x1b[0m   ", chp[0][a][b]);                   /*unicode*/
					}

					if (chp[1][a][b] == ' ')
					{
						printf("       ");
					}

					if (chp[1][a][b] == '2')
					{
						printf("   \x1b[31m%c\x1b[0m   ", chp[0][a][b]);                   /*unicode*/
					}
				}

				else if ((hor + vert) % 2 != 0)
				{
					if (chp[1][a][b] == '1')
					{
						printf("\u2588\u2588\u258C\x1b[32m%c\x1b[0m\u2590\u2588\u2588", chp[0][a][b]);
					}

					if (chp[1][a][b] == '2')
					{
						printf("\u2588\u2588\u258C\x1b[31m%c\x1b[0m\u2590\u2588\u2588", chp[0][a][b]);
					}

					if (chp[1][a][b] == ' ')
					{
						printf("\u2588\u2588\u2588\u2588\u2588\u2588\u2588", chp[0][a][b]);
					}

				}
				hor++;

			}

			printf("|");
			printf("\n");
			i--;
		}

		if (i == 1)
		{
			len = buffer;
			printf("       |");
			int hor = 0;

			for (len; len > 0; len--)
			{
				if ((hor + vert) % 2 == 0)
				{
					printf("       ");
				}

				else if ((hor + vert) % 2 != 0)
				{
					printf("\u2588\u2588\u2588\u2588\u2588\u2588\u2588");
				}
				hor++;

			}

			printf("|");
			printf("\n");
			i--;
		}

		numbers--;
		vert++;
	}


	len = buffer;
	printf("       --");

	for (len; len > 0; len--) {
		printf("-------");
	}
	printf("\n");

	printf("           A      B      C      D      E      F      G      H    \n");
	printf("                          \x1b[31mPLAYER 2 Black\x1b[0m                         \n\n");
	printf("       '1'=>PAWN        '2'=>KNIGHT          '4'=>BISHOP\n       '5'=>ROOK        '7'=>QUEEN           '9'=>KING");
	printf("\n\n");


}


int  IsInCheck(int a, int b,char chess[2][8][8])
{
	int             k,
		h,
		y, z,
		row,
		col;



	/* Check attacking of Knight */
	if (chess[0][a - 2][b - 1] == 'N' && chess[1][a - 2][b - 1] == '1')
		return 1;
	if (chess[0][a - 2][b + 1] == 'N' && chess[1][a - 2][b + 1] == '1')
		return 1;
	if (chess[0][a - 1][b - 2] == 'N' && chess[1][a - 1][b - 2] == '1')
		return 1;
	if (chess[0][a - 1][b + 2] == 'N' && chess[1][a - 1][b + 2] == '1')
		return 1;
	if (chess[0][a + 1][b - 2] == 'N' && chess[1][a + 1][b - 2] == '1')
		return 1;
	if (chess[0][a + 1][b + 2] == 'N' && chess[1][a + 1][b + 2] == '1')
		return 1;
	if (chess[0][a + 2][b - 1] == 'N' && chess[1][a + 2][b - 1] == '1')
		return 1;
	if (chess[0][a + 2][b + 1] == 'N' && chess[1][a + 2][b + 1] == '1')
		return 1;



	/* Check horizontal and vertical lines for attacking of Queen, Rook, King */
	/* go down */

	if (chess[1][a + 1][b] == '1' && (chess[0][a + 1][b] == 'K' || chess[0][a + 1][b] == 'Q' || chess[0][a + 1][b] == 'R'))
		return 1;
	else if (chess[0][a + 1][b] == ' '&& chess[1][a + 2][b] == '1' && (chess[0][a + 2][b] == 'K' || chess[0][a + 2][b] == 'Q' || chess[0][a + 2][b] == 'R'))
		return 1;
	else if (chess[0][a + 1][b] == ' '&&chess[0][a + 2][b] == ' '&&chess[1][a + 3][b] == '1' && (chess[0][a + 3][b] == 'K' || chess[0][a + 3][b] == 'Q' || chess[0][a + 3][b] == 'R'))
		return 1;
	else 	if (chess[0][a + 1][b] == ' '&&chess[0][a + 2][b] == ' '&&chess[0][a + 3][b] == ' '&&chess[1][a + 4][b] == '1' && (chess[0][a + 4][b] == 'K' || chess[0][a + 4][b] == 'Q' || chess[0][a + 4][b] == 'R'))
		return 1;
	else if (chess[0][a + 1][b] == ' '&&chess[0][a + 2][b] == ' '&&chess[0][a + 3][b] == ' '&&chess[0][a + 4][b] == ' '&&chess[1][a + 5][b] == '1' && (chess[0][a + 5][b] == 'K' || chess[0][a + 5][b] == 'Q' || chess[0][a + 5][b] == 'R'))
		return 1;
	else if (chess[0][a + 1][b] == ' '&&chess[0][a + 2][b] == ' '&&chess[0][a + 3][b] == ' '&&chess[0][a + 4][b] == ' '&&chess[0][a + 5][b] == ' '&&chess[1][a + 6][b] == '1' && (chess[0][a + 6][b] == 'K' || chess[0][a + 6][b] == 'Q' || chess[0][a + 6][b] == 'R'))
		return 1;
	else 	if (chess[0][a + 1][b] == ' '&&chess[0][a + 2][b] == ' '&&chess[0][a + 3][b] == ' '&&chess[0][a + 4][b] == ' '&&chess[0][a + 5][b] == ' '&&chess[0][a + 6][b] == ' '&&chess[1][a + 7][b] == '1' && (chess[0][a + 7][b] == 'K' || chess[0][a + 7][b] == 'Q' || chess[0][a + 7][b] == 'R'))
		return 1;



	/* go left */


	if (chess[1][a][b - 1] == '1'/*color[b-1] == xside */ && (chess[0][a][b - 1] == 'K' || chess[0][a][b - 1] == 'Q' || chess[0][a][b - 1] == 'R'/*piece[b-1] == KING || piece[b-1] == QUEEN || piece[b-1] == ROOK*/))
		return 1;
	else 	if (chess[0][a][b - 1] == ' '&&chess[1][a][b - 2] == '1'/*color[b-2] == xside */ && (chess[0][a][b - 2] == 'K' || chess[0][a][b - 2] == 'Q' || chess[0][a][b - 2] == 'R'/*piece[b-2] == KING || piece[b-2] == QUEEN || piece[b-2] == ROOK*/))
		return 1;
	else if (chess[0][a][b - 1] == ' '&&chess[0][a][b - 2] == ' '&&chess[1][a][b - 3] == '1'/*color[b-3] == xside */ && (chess[0][a][b - 3] == 'K' || chess[0][a][b - 3] == 'Q' || chess[0][a][b - 3] == 'R'/*piece[b-3] == KING || piece[b-3] == QUEEN || piece[b-3] == ROOK*/))
		return 1;
	else if (chess[0][a][b - 1] == ' '&&chess[0][a][b - 2] == ' '&&chess[0][a][b - 3] == ' '&&chess[1][a][b - 4] == '1'/*color[b-4] == xside */ && (chess[0][a][b - 4] == 'K' || chess[0][a][b - 4] == 'Q' || chess[0][a][b - 4] == 'R'/*piece[b-4] == KING || piece[b-4] == QUEEN || piece[b-4] == ROOK*/))
		return 1;
	else if (chess[0][a][b - 1] == ' '&&chess[0][a][b - 2] == ' '&&chess[0][a][b - 3] == ' '&&chess[0][a][b - 4] == ' '&&chess[1][a][b - 5] == '1'/*color[b-5] == xside */ && (chess[0][a][b - 5] == 'K' || chess[0][a][b - 5] == 'Q' || chess[0][a][b - 5] == 'R'/*piece[b-5] == KING || piece[b-5] == QUEEN || piece[b-5] == ROOK*/))
		return 1;
	else if (chess[0][a][b - 1] == ' '&&chess[0][a][b - 2] == ' '&&chess[0][a][b - 3] == ' '&&chess[0][a][b - 4] == ' '&&chess[0][a][b - 5] == ' '&&chess[1][a][b - 6] == '1'/*color[b-6] == xside */ && (chess[0][a][b - 6] == 'K' || chess[0][a][b - 6] == 'Q' || chess[0][a][b - 6] == 'R'/*piece[b-6] == KING || piece[b-6] == QUEEN || piece[b-6] == ROOK*/))
		return 1;
	else if (chess[0][a][b - 1] == ' '&&chess[0][a][b - 2] == ' '&&chess[0][a][b - 3] == ' '&&chess[0][a][b - 4] == ' '&&chess[0][a][b - 5] == ' '&&chess[0][a][b - 6] == ' '&&chess[1][a][b - 7] == '1'/*color[b-7] == xside */ && (chess[0][a][b - 7] == 'K' || chess[0][a][b - 7] == 'Q' || chess[0][a][b - 7] == 'R'/*piece[b-7] == KING || piece[b-7] == QUEEN || piece[b-7] == ROOK*/))
		return 1;



	/* go right */


	if (chess[1][a][b + 1] == '1'/*color[b+1] == xside */ && (chess[0][a][b + 1] == 'K' || chess[0][a][b + 1] == 'Q' || chess[0][a][b + 1] == 'R'/*piece[b+1] == KING || piece[b+1] == QUEEN || piece[b+1] == ROOK*/))
		return 1;
	else if (chess[0][a][b + 1] == ' '&&chess[1][a][b + 2] == '1'/*color[b+2] == xside */ && (chess[0][a][b + 2] == 'K' || chess[0][a][b + 2] == 'Q' || chess[0][a][b + 2] == 'R'/*piece[b+2] == KING || piece[b+2] == QUEEN || piece[b+2] == ROOK*/))
		return 1;
	else if (chess[0][a][b + 1] == ' '&&chess[0][a][b + 2] == ' '&&chess[1][a][b + 3] == '1'/*color[b+3] == xside */ && (chess[0][a][b + 3] == 'K' || chess[0][a][b + 3] == 'Q' || chess[0][a][b + 3] == 'R'/*piece[b+3] == KING || piece[b+3] == QUEEN || piece[b+3] == ROOK*/))
		return 1;
	else if (chess[0][a][b + 1] == ' '&&chess[0][a][b + 2] == ' '&&chess[0][a][b + 3] == ' '&&chess[1][a][b + 4] == '1'/*color[b+4] == xside */ && (chess[0][a][b + 4] == 'K' || chess[0][a][b + 4] == 'Q' || chess[0][a][b + 4] == 'R'/*piece[b+4] == KING || piece[b+4] == QUEEN || piece[b+4] == ROOK*/))
		return 1;
	else if (chess[0][a][b + 1] == ' '&&chess[0][a][b + 2] == ' '&&chess[0][a][b + 3] == ' '&&chess[0][a][b + 4] == ' '&&chess[1][a][b + 5] == '1'/*color[b+5] == xside */ && (chess[0][a][b + 5] == 'K' || chess[0][a][b + 5] == 'Q' || chess[0][a][b + 5] == 'R'/*piece[b+5] == KING || piece[b+5] == QUEEN || piece[b+5] == ROOK*/))
		return 1;
	else if (chess[0][a][b + 1] == ' '&&chess[0][a][b + 2] == ' '&&chess[0][a][b + 3] == ' '&&chess[0][a][b + 4] == ' '&&chess[0][a][b + 5] == ' '&&chess[1][a][b + 6] == '1'/*color[b+6] == xside */ && (chess[0][a][b + 6] == 'K' || chess[0][a][b + 6] == 'Q' || chess[0][a][b + 6] == 'R'/*piece[b+6] == KING || piece[b+6] == QUEEN || piece[b+6] == ROOK*/))
		return 1;
	else if (chess[0][a][b + 1] == ' '&&chess[0][a][b + 2] == ' '&&chess[0][a][b + 3] == ' '&&chess[0][a][b + 4] == ' '&&chess[0][a][b + 5] == ' '&&chess[0][a][b + 6] == ' '&&chess[1][a][b + 7] == '1'/*color[b+7] == xside */ && (chess[0][a][b + 7] == 'K' || chess[0][a][b + 7] == 'Q' || chess[0][a][b + 7] == 'R'/*piece[b+7] == KING || piece[b+7] == QUEEN || piece[b+7] == ROOK*/))
		return 1;




	/* go up */


	if (chess[1][a - 1][b] == '1'/*color[a-1] == xside */ && (chess[0][a - 1][b] == 'K' || chess[0][a - 1][b] == 'Q' || chess[0][a - 1][b] == 'R'/*piece[a-1] == KING || piece[a-1] == QUEEN || piece[a-1] == ROOK*/))
		return 1;
	else if (chess[0][a - 1][b] == ' '&&chess[1][a - 2][b] == '1'/*color[a-2] == xside */ && (chess[0][a - 2][b] == 'K' || chess[0][a - 2][b] == 'Q' || chess[0][a - 2][b] == 'R'/*piece[a-2] == KING || piece[a-2] == QUEEN || piece[a-2] == ROOK*/))
		return 1;
	else if (chess[0][a - 1][b] == ' '&&chess[0][a - 2][b] == ' '&&chess[1][a - 3][b] == '1'/*color[a-3] == xside */ && (chess[0][a - 3][b] == 'K' || chess[0][a - 3][b] == 'Q' || chess[0][a - 3][b] == 'R'/*piece[a-3] == KING || piece[a-3] == QUEEN || piece[a-3] == ROOK*/))
		return 1;
	else if (chess[0][a - 1][b] == ' '&&chess[0][a - 2][b] == ' '&&chess[0][a - 3][b] == ' '&&chess[1][a - 4][b] == '1'/*color[a-4] == xside */ && (chess[0][a - 4][b] == 'K' || chess[0][a - 4][b] == 'Q' || chess[0][a - 4][b] == 'R'/*piece[a-4] == KING || piece[a-4] == QUEEN || piece[a-4] == ROOK*/))
		return 1;
	else if (chess[0][a - 1][b] == ' '&&chess[0][a - 2][b] == ' '&&chess[0][a - 3][b] == ' '&&chess[0][a - 4][b] == ' '&&chess[1][a - 5][b] == '1'/*color[a-5] == xside */ && (chess[0][a - 5][b] == 'K' || chess[0][a - 5][b] == 'Q' || chess[0][a - 5][b] == 'R'/*piece[a-5] == KING || piece[a-5] == QUEEN || piece[a-5] == ROOK*/))
		return 1;
	else if (chess[0][a - 1][b] == ' '&&chess[0][a - 2][b] == ' '&&chess[0][a - 3][b] == ' '&&chess[0][a - 4][b] == ' '&&chess[0][a - 5][b] == ' '&&chess[1][a - 6][b] == '1'/*color[a-6] == xside */ && (chess[0][a - 6][b] == 'K' || chess[0][a - 6][b] == 'Q' || chess[0][a - 6][b] == 'R'/*piece[a-6] == KING || piece[a-6] == QUEEN || piece[a-6] == ROOK*/))
		return 1;
	else if (chess[0][a - 1][b] == ' '&&chess[0][a - 2][b] == ' '&&chess[0][a - 3][b] == ' '&&chess[0][a - 4][b] == ' '&&chess[0][a - 5][b] == ' '&&chess[0][a - 6][b] == ' '&&chess[1][a - 7][b] == '1'/*color[a-7] == xside */ && (chess[0][a - 7][b] == 'K' || chess[0][a - 7][b] == 'Q' || chess[0][a - 7][b] == 'R'/*piece[a-7] == KING || piece[a-7] == QUEEN || piece[a-7] == ROOK*/))
		return 1;






	/* Check diagonal lines for attacking of Queen, Bishop, King, Pawn */
	/* go right down */


	if (chess[1][a + 1][b + 1] == '1' && (chess[0][a + 1][b + 1] == 'K' || chess[0][a + 1][b + 1] == 'Q' || chess[0][a + 1][b + 1] == 'B' ))
		return 1;
	else if (chess[0][a + 1][b + 1] == ' '&&chess[1][a + 2][b + 2] == '1' && (chess[0][a + 2][b + 2] == 'Q' || chess[0][a + 2][b + 2] == 'B'))
		return 1;
	else if (chess[0][a + 1][b + 1] == ' '&&chess[0][a + 2][b + 2] == ' '&&chess[1][a + 3][b + 3] == '1' && (chess[0][a + 3][b + 3] == 'Q' || chess[0][a + 3][b + 3] == 'B'))
		return 1;
	else if (chess[0][a + 1][b + 1] == ' '&&chess[0][a + 2][b + 2] == ' '&&chess[0][a + 3][b + 3] == ' '&&chess[1][a + 4][b + 4] == '1' && (chess[0][a + 4][b + 4] == 'Q' || chess[0][a + 4][b + 4] == 'B'))
		return 1;
	else if (chess[0][a + 1][b + 1] == ' '&&chess[0][a + 2][b + 2] == ' '&&chess[0][a + 3][b + 3] == ' '&&chess[0][a + 4][b + 4] == ' '&&chess[1][a + 5][b + 5] == '1' && (chess[0][a + 5][b + 5] == 'Q' || chess[0][a + 5][b + 5] == 'B'))
		return 1;
	else if (chess[0][a + 1][b + 1] == ' '&&chess[0][a + 2][b + 2] == ' '&&chess[0][a + 3][b + 3] == ' '&&chess[0][a + 4][b + 4] == ' '&&chess[0][a + 5][b + 5] == ' '&&chess[1][a + 6][b + 6] == '1' && (chess[0][a + 6][b + 6] == 'Q' || chess[0][a + 6][b + 6] == 'B'))
		return 1;
	else if (chess[0][a + 1][b + 1] == ' '&&chess[0][a + 2][b + 2] == ' '&&chess[0][a + 3][b + 3] == ' '&&chess[0][a + 4][b + 4] == ' '&&chess[0][a + 5][b + 5] == ' '&&chess[0][a + 6][b + 6] == ' '&&chess[1][a + 7][b + 7] == '1' && (chess[0][a + 7][b + 7] == 'Q' || chess[0][a + 7][b + 7] == 'B'))
		return 1;




	/* go left down */


	if (chess[1][a + 1][b - 1] == '1' && (chess[0][a + 1][b - 1] == 'K' || chess[0][a + 1][b - 1] == 'Q' || chess[0][a + 1][b - 1] == 'B' ))
		return 1;
	else if (chess[0][a + 1][b - 1] == ' '&&chess[1][a + 2][b - 2] == '1' && (chess[0][a + 2][b - 2] == 'Q' || chess[0][a + 2][b - 2] == 'B'))
		return 1;
	else if (chess[0][a + 1][b - 1] == ' '&&chess[0][a + 2][b - 2] == ' '&&chess[1][a + 3][b - 3] == '1' && (chess[0][a + 3][b - 3] == 'Q' || chess[0][a + 3][b - 3] == 'B'))
		return 1;
	else if (chess[0][a + 1][b - 1] == ' '&&chess[0][a + 2][b - 2] == ' '&&chess[0][a + 3][b - 3] == ' '&&chess[1][a + 4][b - 4] == '1' && (chess[0][a + 4][b - 4] == 'Q' || chess[0][a + 4][b - 4] == 'B'))
		return 1;
	else if (chess[0][a + 1][b - 1] == ' '&&chess[0][a + 2][b - 2] == ' '&&chess[0][a + 3][b - 3] == ' '&&chess[0][a + 4][b - 4] == ' '&&chess[1][a + 5][b - 5] == '1' && (chess[0][a + 5][b - 5] == 'Q' || chess[0][a + 5][b - 5] == 'B'))
		return 1;
	else if (chess[0][a + 1][b - 1] == ' '&&chess[0][a + 2][b - 2] == ' '&&chess[0][a + 3][b - 3] == ' '&&chess[0][a + 4][b - 4] == ' '&&chess[0][a + 5][b - 5] == ' '&&chess[1][a + 6][b - 6] == '1' && (chess[0][a + 6][b - 6] == 'Q' || chess[0][a + 6][b - 6] == 'B'))
		return 1;
	else if (chess[0][a + 1][b - 1] == ' '&&chess[0][a + 2][b - 2] == ' '&&chess[0][a + 3][b - 3] == ' '&&chess[0][a + 4][b - 4] == ' '&&chess[0][a + 5][b - 5] == ' '&&chess[0][a + 6][b - 6] == ' '&&chess[1][a + 7][b - 7] == '1' && (chess[0][a + 7][b - 7] == 'Q' || chess[0][a + 7][b - 7] == 'B'))
		return 1;



	/* go left up */


	if (chess[1][a - 1][b - 1] == '1' && (chess[0][a - 1][b - 1] == 'K' || chess[0][a - 1][b - 1] == 'Q' || chess[0][a - 1][b - 1] == 'B' || chess[0][a + 1][b + 1] == 'P'))
		return 1;
	else if (chess[0][a - 1][b - 1] == ' '&&chess[1][a - 2][b - 2] == '1' && (chess[0][a - 2][b - 2] == 'Q' || chess[0][a - 2][b - 2] == 'B'))
		return 1;
	else if (chess[0][a - 1][b - 1] == ' '&&chess[0][a - 2][b - 2] == ' '&&chess[1][a - 3][b - 3] == '1' && (chess[0][a - 3][b - 3] == 'Q' || chess[0][a - 3][b - 3] == 'B'))
		return 1;
	else if (chess[0][a - 1][b - 1] == ' '&&chess[0][a - 2][b - 2] == ' '&&chess[0][a - 3][b - 3] == ' '&&chess[1][a - 4][b - 4] == '1' && (chess[0][a - 4][b - 4] == 'Q' || chess[0][a - 4][b - 4] == 'B'))
		return 1;
	else if (chess[0][a - 1][b - 1] == ' '&&chess[0][a - 2][b - 2] == ' '&&chess[0][a - 3][b - 3] == ' '&&chess[0][a - 4][b - 4] == ' '&&chess[1][a - 5][b - 5] == '1' && (chess[0][a - 5][b - 5] == 'Q' || chess[0][a - 5][b - 5] == 'B'))
		return 1;
	else if (chess[0][a - 1][b - 1] == ' '&&chess[0][a - 2][b - 2] == ' '&&chess[0][a - 3][b - 3] == ' '&&chess[0][a - 4][b - 4] == ' '&&chess[0][a - 5][b - 5] == ' '&&chess[1][a - 6][b - 6] == '1' && (chess[0][a - 6][b - 6] == 'Q' || chess[0][a - 6][b - 6] == 'B'))
		return 1;
	else if (chess[0][a - 1][b - 1] == ' '&&chess[0][a - 2][b - 2] == ' '&&chess[0][a - 3][b - 3] == ' '&&chess[0][a - 4][b - 4] == ' '&&chess[0][a - 5][b - 5] == ' '&&chess[0][a - 6][b - 6] == ' '&&chess[1][a - 7][b - 7] == '1' && (chess[0][a - 7][b - 7] == 'Q' || chess[0][a - 7][b - 7] == 'B'))
		return 1;



	/* go right up */


	if (chess[1][a - 1][b + 1] == '1' && (chess[0][a - 1][b + 1] == 'K' || chess[0][a - 1][b + 1] == 'Q' || chess[0][a - 1][b + 1] == 'B' || chess[0][a + 1][b + 1] == 'P'))
		return 1;
	else if (chess[0][a - 1][b + 1] == ' '&&chess[1][a - 2][b + 2] == '1' && (chess[0][a - 2][b + 2] == 'Q' || chess[0][a - 2][b + 2] == 'B'))
		return 1;
	else if (chess[0][a - 1][b + 1] == ' '&&chess[0][a - 2][b + 2] == ' '&&chess[1][a - 3][b + 3] == '1' && (chess[0][a - 3][b + 3] == 'Q' || chess[0][a - 3][b + 3] == 'B'))
		return 1;
	else if (chess[0][a - 1][b + 1] == ' '&&chess[0][a - 2][b + 2] == ' '&&chess[0][a - 3][b + 3] == ' '&&chess[1][a - 4][b + 4] == '1' && (chess[0][a - 4][b + 4] == 'Q' || chess[0][a - 4][b + 4] == 'B'))
		return 1;
	else if (chess[0][a - 1][b + 1] == ' '&&chess[0][a - 2][b + 2] == ' '&&chess[0][a - 3][b + 3] == ' '&&chess[0][a - 4][b + 4] == ' '&&chess[1][a - 5][b + 5] == '1' && (chess[0][a - 5][b + 5] == 'Q' || chess[0][a - 5][b + 5] == 'B'))
		return 1;
	else if (chess[0][a - 1][b + 1] == ' '&&chess[0][a - 2][b + 2] == ' '&&chess[0][a - 3][b + 3] == ' '&&chess[0][a - 4][b + 4] == ' '&&chess[0][a - 5][b + 5] == ' '&&chess[1][a - 6][b + 6] == '1' && (chess[0][a - 6][b + 6] == 'Q' || chess[0][a - 6][b + 6] == 'B'))
		return 1;
	else if (chess[0][a - 1][b + 1] == ' '&&chess[0][a - 2][b + 2] == ' '&&chess[0][a - 3][b + 3] == ' '&&chess[0][a - 4][b + 4] == ' '&&chess[0][a - 5][b + 5] == ' '&&chess[0][a - 6][b + 6] == ' '&&chess[1][a - 7][b + 7] == '1' && (chess[0][a - 7][b + 7] == 'Q' || chess[0][a - 7][b + 7] == 'B'))
		return 1;





	return 0;
}
int  IsInCheck2(int a, int b, char chess[2][8][8])
{
	int             k,
		h,
		y, z,
		row,
		col;



	/* Check attacking of Knight */
	if (chess[0][a - 2][b - 1] == 'N' && chess[1][a - 2][b - 1] == '2')
		return 1;
	if (chess[0][a - 2][b + 1] == 'N' && chess[1][a - 2][b + 1] == '2')
		return 1;
	if (chess[0][a - 1][b - 2] == 'N' && chess[1][a - 1][b - 2] == '2')
		return 1;
	if (chess[0][a - 1][b + 2] == 'N' && chess[1][a - 1][b + 2] == '2')
		return 1;
	if (chess[0][a + 1][b - 2] == 'N' && chess[1][a + 1][b - 2] == '2')
		return 1;
	if (chess[0][a + 1][b + 2] == 'N' && chess[1][a + 1][b + 2] == '2')
		return 1;
	if (chess[0][a + 2][b - 1] == 'N' && chess[1][a + 2][b - 1] == '2')
		return 1;
	if (chess[0][a + 2][b + 1] == 'N' && chess[1][a + 2][b + 1] == '2')
		return 1;



	/* Check horizontal and vertical lines for attacking of Queen, Rook, King */
	/* go down */

	if (chess[1][a + 1][b] == '2' && (chess[0][a + 1][b] == 'K' || chess[0][a + 1][b] == 'Q' || chess[0][a + 1][b] == 'R'))
		return 1;
	else if (chess[0][a + 1][b] == ' '&& chess[1][a + 2][b] == '2' && (chess[0][a + 2][b] == 'K' || chess[0][a + 2][b] == 'Q' || chess[0][a + 2][b] == 'R'))
		return 1;
	else if (chess[0][a + 1][b] == ' '&&chess[0][a + 2][b] == ' '&&chess[1][a + 3][b] == '2' && (chess[0][a + 3][b] == 'K' || chess[0][a + 3][b] == 'Q' || chess[0][a + 3][b] == 'R'))
		return 1;
	else 	if (chess[0][a + 1][b] == ' '&&chess[0][a + 2][b] == ' '&&chess[0][a + 3][b] == ' '&&chess[1][a + 4][b] == '2' && (chess[0][a + 4][b] == 'K' || chess[0][a + 4][b] == 'Q' || chess[0][a + 4][b] == 'R'))
		return 1;
	else if (chess[0][a + 1][b] == ' '&&chess[0][a + 2][b] == ' '&&chess[0][a + 3][b] == ' '&&chess[0][a + 4][b] == ' '&&chess[1][a + 5][b] == '2' && (chess[0][a + 5][b] == 'K' || chess[0][a + 5][b] == 'Q' || chess[0][a + 5][b] == 'R'))
		return 1;
	else if (chess[0][a + 1][b] == ' '&&chess[0][a + 2][b] == ' '&&chess[0][a + 3][b] == ' '&&chess[0][a + 4][b] == ' '&&chess[0][a + 5][b] == ' '&&chess[1][a + 6][b] == '2' && (chess[0][a + 6][b] == 'K' || chess[0][a + 6][b] == 'Q' || chess[0][a + 6][b] == 'R'))
		return 1;
	else 	if (chess[0][a + 1][b] == ' '&&chess[0][a + 2][b] == ' '&&chess[0][a + 3][b] == ' '&&chess[0][a + 4][b] == ' '&&chess[0][a + 5][b] == ' '&&chess[0][a + 6][b] == ' '&&chess[1][a + 7][b] == '2' && (chess[0][a + 7][b] == 'K' || chess[0][a + 7][b] == 'Q' || chess[0][a + 7][b] == 'R'))
		return 1;



	/* go left */


	if (chess[1][a][b - 1] == '2'/*color[b-1] == xside */ && (chess[0][a][b - 1] == 'K' || chess[0][a][b - 1] == 'Q' || chess[0][a][b - 1] == 'R'/*piece[b-1] == KING || piece[b-1] == QUEEN || piece[b-1] == ROOK*/))
		return 1;
	else 	if (chess[0][a][b - 1] == ' '&&chess[1][a][b - 2] == '2'/*color[b-2] == xside */ && (chess[0][a][b - 2] == 'K' || chess[0][a][b - 2] == 'Q' || chess[0][a][b - 2] == 'R'/*piece[b-2] == KING || piece[b-2] == QUEEN || piece[b-2] == ROOK*/))
		return 1;
	else if (chess[0][a][b - 1] == ' '&&chess[0][a][b - 2] == ' '&&chess[1][a][b - 3] == '2'/*color[b-3] == xside */ && (chess[0][a][b - 3] == 'K' || chess[0][a][b - 3] == 'Q' || chess[0][a][b - 3] == 'R'/*piece[b-3] == KING || piece[b-3] == QUEEN || piece[b-3] == ROOK*/))
		return 1;
	else if (chess[0][a][b - 1] == ' '&&chess[0][a][b - 2] == ' '&&chess[0][a][b - 3] == ' '&&chess[1][a][b - 4] == '2'/*color[b-4] == xside */ && (chess[0][a][b - 4] == 'K' || chess[0][a][b - 4] == 'Q' || chess[0][a][b - 4] == 'R'/*piece[b-4] == KING || piece[b-4] == QUEEN || piece[b-4] == ROOK*/))
		return 1;
	else if (chess[0][a][b - 1] == ' '&&chess[0][a][b - 2] == ' '&&chess[0][a][b - 3] == ' '&&chess[0][a][b - 4] == ' '&&chess[1][a][b - 5] == '2'/*color[b-5] == xside */ && (chess[0][a][b - 5] == 'K' || chess[0][a][b - 5] == 'Q' || chess[0][a][b - 5] == 'R'/*piece[b-5] == KING || piece[b-5] == QUEEN || piece[b-5] == ROOK*/))
		return 1;
	else if (chess[0][a][b - 1] == ' '&&chess[0][a][b - 2] == ' '&&chess[0][a][b - 3] == ' '&&chess[0][a][b - 4] == ' '&&chess[0][a][b - 5] == ' '&&chess[1][a][b - 6] == '2'/*color[b-6] == xside */ && (chess[0][a][b - 6] == 'K' || chess[0][a][b - 6] == 'Q' || chess[0][a][b - 6] == 'R'/*piece[b-6] == KING || piece[b-6] == QUEEN || piece[b-6] == ROOK*/))
		return 1;
	else if (chess[0][a][b - 1] == ' '&&chess[0][a][b - 2] == ' '&&chess[0][a][b - 3] == ' '&&chess[0][a][b - 4] == ' '&&chess[0][a][b - 5] == ' '&&chess[0][a][b - 6] == ' '&&chess[1][a][b - 7] == '2'/*color[b-7] == xside */ && (chess[0][a][b - 7] == 'K' || chess[0][a][b - 7] == 'Q' || chess[0][a][b - 7] == 'R'/*piece[b-7] == KING || piece[b-7] == QUEEN || piece[b-7] == ROOK*/))
		return 1;



	/* go right */


	if (chess[1][a][b + 1] == '2'/*color[b+1] == xside */ && (chess[0][a][b + 1] == 'K' || chess[0][a][b + 1] == 'Q' || chess[0][a][b + 1] == 'R'/*piece[b+1] == KING || piece[b+1] == QUEEN || piece[b+1] == ROOK*/))
		return 1;
	else if (chess[0][a][b + 1] == ' '&&chess[1][a][b + 2] == '2'/*color[b+2] == xside */ && (chess[0][a][b + 2] == 'K' || chess[0][a][b + 2] == 'Q' || chess[0][a][b + 2] == 'R'/*piece[b+2] == KING || piece[b+2] == QUEEN || piece[b+2] == ROOK*/))
		return 1;
	else if (chess[0][a][b + 1] == ' '&&chess[0][a][b + 2] == ' '&&chess[1][a][b + 3] == '2'/*color[b+3] == xside */ && (chess[0][a][b + 3] == 'K' || chess[0][a][b + 3] == 'Q' || chess[0][a][b + 3] == 'R'/*piece[b+3] == KING || piece[b+3] == QUEEN || piece[b+3] == ROOK*/))
		return 1;
	else if (chess[0][a][b + 1] == ' '&&chess[0][a][b + 2] == ' '&&chess[0][a][b + 3] == ' '&&chess[1][a][b + 4] == '2'/*color[b+4] == xside */ && (chess[0][a][b + 4] == 'K' || chess[0][a][b + 4] == 'Q' || chess[0][a][b + 4] == 'R'/*piece[b+4] == KING || piece[b+4] == QUEEN || piece[b+4] == ROOK*/))
		return 1;
	else if (chess[0][a][b + 1] == ' '&&chess[0][a][b + 2] == ' '&&chess[0][a][b + 3] == ' '&&chess[0][a][b + 4] == ' '&&chess[1][a][b + 5] == '2'/*color[b+5] == xside */ && (chess[0][a][b + 5] == 'K' || chess[0][a][b + 5] == 'Q' || chess[0][a][b + 5] == 'R'/*piece[b+5] == KING || piece[b+5] == QUEEN || piece[b+5] == ROOK*/))
		return 1;
	else if (chess[0][a][b + 1] == ' '&&chess[0][a][b + 2] == ' '&&chess[0][a][b + 3] == ' '&&chess[0][a][b + 4] == ' '&&chess[0][a][b + 5] == ' '&&chess[1][a][b + 6] == '2'/*color[b+6] == xside */ && (chess[0][a][b + 6] == 'K' || chess[0][a][b + 6] == 'Q' || chess[0][a][b + 6] == 'R'/*piece[b+6] == KING || piece[b+6] == QUEEN || piece[b+6] == ROOK*/))
		return 1;
	else if (chess[0][a][b + 1] == ' '&&chess[0][a][b + 2] == ' '&&chess[0][a][b + 3] == ' '&&chess[0][a][b + 4] == ' '&&chess[0][a][b + 5] == ' '&&chess[0][a][b + 6] == ' '&&chess[1][a][b + 7] == '2'/*color[b+7] == xside */ && (chess[0][a][b + 7] == 'K' || chess[0][a][b + 7] == 'Q' || chess[0][a][b + 7] == 'R'/*piece[b+7] == KING || piece[b+7] == QUEEN || piece[b+7] == ROOK*/))
		return 1;




	/* go up */


	if (chess[1][a - 1][b] == '2'/*color[a-1] == xside */ && (chess[0][a - 1][b] == 'K' || chess[0][a - 1][b] == 'Q' || chess[0][a - 1][b] == 'R'/*piece[a-1] == KING || piece[a-1] == QUEEN || piece[a-1] == ROOK*/))
		return 1;
	else if (chess[0][a - 1][b] == ' '&&chess[1][a - 2][b] == '2'/*color[a-2] == xside */ && (chess[0][a - 2][b] == 'K' || chess[0][a - 2][b] == 'Q' || chess[0][a - 2][b] == 'R'/*piece[a-2] == KING || piece[a-2] == QUEEN || piece[a-2] == ROOK*/))
		return 1;
	else if (chess[0][a - 1][b] == ' '&&chess[0][a - 2][b] == ' '&&chess[1][a - 3][b] == '2'/*color[a-3] == xside */ && (chess[0][a - 3][b] == 'K' || chess[0][a - 3][b] == 'Q' || chess[0][a - 3][b] == 'R'/*piece[a-3] == KING || piece[a-3] == QUEEN || piece[a-3] == ROOK*/))
		return 1;
	else if (chess[0][a - 1][b] == ' '&&chess[0][a - 2][b] == ' '&&chess[0][a - 3][b] == ' '&&chess[1][a - 4][b] == '2'/*color[a-4] == xside */ && (chess[0][a - 4][b] == 'K' || chess[0][a - 4][b] == 'Q' || chess[0][a - 4][b] == 'R'/*piece[a-4] == KING || piece[a-4] == QUEEN || piece[a-4] == ROOK*/))
		return 1;
	else if (chess[0][a - 1][b] == ' '&&chess[0][a - 2][b] == ' '&&chess[0][a - 3][b] == ' '&&chess[0][a - 4][b] == ' '&&chess[1][a - 5][b] == '2'/*color[a-5] == xside */ && (chess[0][a - 5][b] == 'K' || chess[0][a - 5][b] == 'Q' || chess[0][a - 5][b] == 'R'/*piece[a-5] == KING || piece[a-5] == QUEEN || piece[a-5] == ROOK*/))
		return 1;
	else if (chess[0][a - 1][b] == ' '&&chess[0][a - 2][b] == ' '&&chess[0][a - 3][b] == ' '&&chess[0][a - 4][b] == ' '&&chess[0][a - 5][b] == ' '&&chess[1][a - 6][b] == '2'/*color[a-6] == xside */ && (chess[0][a - 6][b] == 'K' || chess[0][a - 6][b] == 'Q' || chess[0][a - 6][b] == 'R'/*piece[a-6] == KING || piece[a-6] == QUEEN || piece[a-6] == ROOK*/))
		return 1;
	else if (chess[0][a - 1][b] == ' '&&chess[0][a - 2][b] == ' '&&chess[0][a - 3][b] == ' '&&chess[0][a - 4][b] == ' '&&chess[0][a - 5][b] == ' '&&chess[0][a - 6][b] == ' '&&chess[1][a - 7][b] == '2'/*color[a-7] == xside */ && (chess[0][a - 7][b] == 'K' || chess[0][a - 7][b] == 'Q' || chess[0][a - 7][b] == 'R'/*piece[a-7] == KING || piece[a-7] == QUEEN || piece[a-7] == ROOK*/))
		return 1;






	/* Check diagonal lines for attacking of Queen, Bishop, King, Pawn */
	/* go right down */


	if (chess[1][a + 1][b + 1] == '2' && (chess[0][a + 1][b + 1] == 'K' || chess[0][a + 1][b + 1] == 'Q' || chess[0][a + 1][b + 1] == 'B'))
		return 1;
	else if (chess[0][a + 1][b + 1] == ' '&&chess[1][a + 2][b + 2] == '2' && (chess[0][a + 2][b + 2] == 'Q' || chess[0][a + 2][b + 2] == 'B'))
		return 1;
	else if (chess[0][a + 1][b + 1] == ' '&&chess[0][a + 2][b + 2] == ' '&&chess[1][a + 3][b + 3] == '2' && (chess[0][a + 3][b + 3] == 'Q' || chess[0][a + 3][b + 3] == 'B'))
		return 1;
	else if (chess[0][a + 1][b + 1] == ' '&&chess[0][a + 2][b + 2] == ' '&&chess[0][a + 3][b + 3] == ' '&&chess[1][a + 4][b + 4] == '2' && (chess[0][a + 4][b + 4] == 'Q' || chess[0][a + 4][b + 4] == 'B'))
		return 1;
	else if (chess[0][a + 1][b + 1] == ' '&&chess[0][a + 2][b + 2] == ' '&&chess[0][a + 3][b + 3] == ' '&&chess[0][a + 4][b + 4] == ' '&&chess[1][a + 5][b + 5] == '2' && (chess[0][a + 5][b + 5] == 'Q' || chess[0][a + 5][b + 5] == 'B'))
		return 1;
	else if (chess[0][a + 1][b + 1] == ' '&&chess[0][a + 2][b + 2] == ' '&&chess[0][a + 3][b + 3] == ' '&&chess[0][a + 4][b + 4] == ' '&&chess[0][a + 5][b + 5] == ' '&&chess[1][a + 6][b + 6] == '2' && (chess[0][a + 6][b + 6] == 'Q' || chess[0][a + 6][b + 6] == 'B'))
		return 1;
	else if (chess[0][a + 1][b + 1] == ' '&&chess[0][a + 2][b + 2] == ' '&&chess[0][a + 3][b + 3] == ' '&&chess[0][a + 4][b + 4] == ' '&&chess[0][a + 5][b + 5] == ' '&&chess[0][a + 6][b + 6] == ' '&&chess[1][a + 7][b + 7] == '2' && (chess[0][a + 7][b + 7] == 'Q' || chess[0][a + 7][b + 7] == 'B'))
		return 1;




	/* go left down */


	if (chess[1][a + 1][b - 1] == '2' && (chess[0][a + 1][b - 1] == 'K' || chess[0][a + 1][b - 1] == 'Q' || chess[0][a + 1][b - 1] == 'B'))
		return 1;
	else if (chess[0][a + 1][b - 1] == ' '&&chess[1][a + 2][b - 2] == '2' && (chess[0][a + 2][b - 2] == 'Q' || chess[0][a + 2][b - 2] == 'B'))
		return 1;
	else if (chess[0][a + 1][b - 1] == ' '&&chess[0][a + 2][b - 2] == ' '&&chess[1][a + 3][b - 3] == '2' && (chess[0][a + 3][b - 3] == 'Q' || chess[0][a + 3][b - 3] == 'B'))
		return 1;
	else if (chess[0][a + 1][b - 1] == ' '&&chess[0][a + 2][b - 2] == ' '&&chess[0][a + 3][b - 3] == ' '&&chess[1][a + 4][b - 4] == '2' && (chess[0][a + 4][b - 4] == 'Q' || chess[0][a + 4][b - 4] == 'B'))
		return 1;
	else if (chess[0][a + 1][b - 1] == ' '&&chess[0][a + 2][b - 2] == ' '&&chess[0][a + 3][b - 3] == ' '&&chess[0][a + 4][b - 4] == ' '&&chess[1][a + 5][b - 5] == '2' && (chess[0][a + 5][b - 5] == 'Q' || chess[0][a + 5][b - 5] == 'B'))
		return 1;
	else if (chess[0][a + 1][b - 1] == ' '&&chess[0][a + 2][b - 2] == ' '&&chess[0][a + 3][b - 3] == ' '&&chess[0][a + 4][b - 4] == ' '&&chess[0][a + 5][b - 5] == ' '&&chess[1][a + 6][b - 6] == '2' && (chess[0][a + 6][b - 6] == 'Q' || chess[0][a + 6][b - 6] == 'B'))
		return 1;
	else if (chess[0][a + 1][b - 1] == ' '&&chess[0][a + 2][b - 2] == ' '&&chess[0][a + 3][b - 3] == ' '&&chess[0][a + 4][b - 4] == ' '&&chess[0][a + 5][b - 5] == ' '&&chess[0][a + 6][b - 6] == ' '&&chess[1][a + 7][b - 7] == '2' && (chess[0][a + 7][b - 7] == 'Q' || chess[0][a + 7][b - 7] == 'B'))
		return 1;



	/* go left up */


	if (chess[1][a - 1][b - 1] == '2' && (chess[0][a - 1][b - 1] == 'K' || chess[0][a - 1][b - 1] == 'Q' || chess[0][a - 1][b - 1] == 'B' || chess[0][a + 1][b + 1] == 'P'))
		return 1;
	else if (chess[0][a - 1][b - 1] == ' '&&chess[1][a - 2][b - 2] == '2' && (chess[0][a - 2][b - 2] == 'Q' || chess[0][a - 2][b - 2] == 'B'))
		return 1;
	else if (chess[0][a - 1][b - 1] == ' '&&chess[0][a - 2][b - 2] == ' '&&chess[1][a - 3][b - 3] == '2' && (chess[0][a - 3][b - 3] == 'Q' || chess[0][a - 3][b - 3] == 'B'))
		return 1;
	else if (chess[0][a - 1][b - 1] == ' '&&chess[0][a - 2][b - 2] == ' '&&chess[0][a - 3][b - 3] == ' '&&chess[1][a - 4][b - 4] == '2' && (chess[0][a - 4][b - 4] == 'Q' || chess[0][a - 4][b - 4] == 'B'))
		return 1;
	else if (chess[0][a - 1][b - 1] == ' '&&chess[0][a - 2][b - 2] == ' '&&chess[0][a - 3][b - 3] == ' '&&chess[0][a - 4][b - 4] == ' '&&chess[1][a - 5][b - 5] == '2' && (chess[0][a - 5][b - 5] == 'Q' || chess[0][a - 5][b - 5] == 'B'))
		return 1;
	else if (chess[0][a - 1][b - 1] == ' '&&chess[0][a - 2][b - 2] == ' '&&chess[0][a - 3][b - 3] == ' '&&chess[0][a - 4][b - 4] == ' '&&chess[0][a - 5][b - 5] == ' '&&chess[1][a - 6][b - 6] == '2' && (chess[0][a - 6][b - 6] == 'Q' || chess[0][a - 6][b - 6] == 'B'))
		return 1;
	else if (chess[0][a - 1][b - 1] == ' '&&chess[0][a - 2][b - 2] == ' '&&chess[0][a - 3][b - 3] == ' '&&chess[0][a - 4][b - 4] == ' '&&chess[0][a - 5][b - 5] == ' '&&chess[0][a - 6][b - 6] == ' '&&chess[1][a - 7][b - 7] == '2' && (chess[0][a - 7][b - 7] == 'Q' || chess[0][a - 7][b - 7] == 'B'))
		return 1;



	/* go right up */


	if (chess[1][a - 1][b + 1] == '2' && (chess[0][a - 1][b + 1] == 'K' || chess[0][a - 1][b + 1] == 'Q' || chess[0][a - 1][b + 1] == 'B' || chess[0][a + 1][b + 1] == 'P'))
		return 1;
	else if (chess[0][a - 1][b + 1] == ' '&&chess[1][a - 2][b + 2] == '2' && (chess[0][a - 2][b + 2] == 'Q' || chess[0][a - 2][b + 2] == 'B'))
		return 1;
	else if (chess[0][a - 1][b + 1] == ' '&&chess[0][a - 2][b + 2] == ' '&&chess[1][a - 3][b + 3] == '2' && (chess[0][a - 3][b + 3] == 'Q' || chess[0][a - 3][b + 3] == 'B'))
		return 1;
	else if (chess[0][a - 1][b + 1] == ' '&&chess[0][a - 2][b + 2] == ' '&&chess[0][a - 3][b + 3] == ' '&&chess[1][a - 4][b + 4] == '2' && (chess[0][a - 4][b + 4] == 'Q' || chess[0][a - 4][b + 4] == 'B'))
		return 1;
	else if (chess[0][a - 1][b + 1] == ' '&&chess[0][a - 2][b + 2] == ' '&&chess[0][a - 3][b + 3] == ' '&&chess[0][a - 4][b + 4] == ' '&&chess[1][a - 5][b + 5] == '2' && (chess[0][a - 5][b + 5] == 'Q' || chess[0][a - 5][b + 5] == 'B'))
		return 1;
	else if (chess[0][a - 1][b + 1] == ' '&&chess[0][a - 2][b + 2] == ' '&&chess[0][a - 3][b + 3] == ' '&&chess[0][a - 4][b + 4] == ' '&&chess[0][a - 5][b + 5] == ' '&&chess[1][a - 6][b + 6] == '2' && (chess[0][a - 6][b + 6] == 'Q' || chess[0][a - 6][b + 6] == 'B'))
		return 1;
	else if (chess[0][a - 1][b + 1] == ' '&&chess[0][a - 2][b + 2] == ' '&&chess[0][a - 3][b + 3] == ' '&&chess[0][a - 4][b + 4] == ' '&&chess[0][a - 5][b + 5] == ' '&&chess[0][a - 6][b + 6] == ' '&&chess[1][a - 7][b + 7] == '2' && (chess[0][a - 7][b + 7] == 'Q' || chess[0][a - 7][b + 7] == 'B'))
		return 1;





	return 0;
}
int takeback(char ent1, int i1, int j1, int k1, int l1, int cond, char chess[2][8][8]) {

	chess[0][i1][j1] = chess[0][k1][l1];

	chess[1][i1][j1] = chess[1][k1][l1];

	chess[1][k1][l1] = restorecolor;
	chess[0][k1][l1] = restorekind;

	return 0;
}
int takeback2(char ent1, int i1, int j1, int k1, int l1, int cond, char chess[2][8][8]) {

	chess[0][i1][j1] = chess[0][k1][l1];

	chess[1][i1][j1] = chess[1][k1][l1];

	chess[1][k1][l1] = restorecolor2;
	chess[0][k1][l1] = restorekind2;

	return 0;
}


int Kcantmove(int a, int b,char chess[2][8][8]) {


	if (a == 7 && b == 0) {
		if ((IsInCheck(a - 1, b,chess) == 1 || chess[1][a - 1][b] == '2') && (IsInCheck(a - 1, b + 1, chess) == 1 || chess[1][a - 1][b + 1] == '2') && (IsInCheck(a, b + 1, chess) == 1 || chess[1][a][b + 1] == '2'))
		
			return 1;
	}

	if (a == 7 && b != 0 && b != 7) {
		if ((IsInCheck(a - 1, b - 1, chess) == 1 || chess[1][a - 1][b - 1] == '2') && (IsInCheck(a - 1, b, chess) == 1 || chess[1][a - 1][b] == '2') && (IsInCheck(a - 1, b + 1, chess) == 1 || chess[1][a - 1][b + 1] == '2') && (IsInCheck(a, b - 1, chess) == 1 || chess[1][a][b - 1] == '2') && (IsInCheck(a, b + 1, chess) == 1 || chess[1][a][b + 1] == '2'))
			
			return 1;

	}
	if (a == 7 && b == 7) {
		if ((IsInCheck(a - 1, b - 1, chess) == 1 || chess[1][a - 1][b - 1] == '2') && (IsInCheck(a - 1, b, chess) == 1 || chess[1][a - 1][b] == '2') && (IsInCheck(a, b - 1, chess) == 1 || chess[1][a][b - 1] == '2'))
			return 1;
	}

	if (a == 0 && b == 0) {
		if ((IsInCheck(a, b + 1, chess) == 1 || chess[1][a][b + 1] == '2') && (IsInCheck(a + 1, b, chess) == 1 || chess[1][a + 1][b] == '2') && (IsInCheck(a + 1, b + 1, chess) == 1 || chess[1][a + 1][b + 1] == '2'))
			return 1;
	}
	if (a == 0 && b != 0 && b != 7) {
		if ((IsInCheck(a, b - 1, chess) == 1 || chess[1][a][b - 1] == '2') && (IsInCheck(a, b + 1, chess) == 1 || chess[1][a][b + 1] == '2') && (IsInCheck(a + 1, b - 1, chess) == 1 || chess[1][a + 1][b - 1] == '2') && (IsInCheck(a + 1, b, chess) == 1 || chess[1][a + 1][b] == '2') && (IsInCheck(a + 1, b + 1, chess) == 1 || chess[1][a + 1][b + 1] == '2'))
			return 1;
	}
	if (a == 0 && b == 7) {
		if ((IsInCheck(a, b - 1, chess) == 1 || chess[1][a][b - 1] == '2') && (IsInCheck(a + 1, b - 1, chess) == 1 || chess[1][a + 1][b - 1] == '2') && (IsInCheck(a + 1, b, chess) == 1 || chess[1][a + 1][b] == '2'))
			return 1;
	}
	if (a != 0 && a != 7 && b == 0) {
		if ((IsInCheck(a - 1, b, chess) == 1 || chess[1][a - 1][b] == '2') && (IsInCheck(a - 1, b + 1, chess) == 1 || chess[1][a - 1][b + 1] == '2') && (IsInCheck(a, b + 1, chess) == 1 || chess[1][a][b + 1] == '2') && (IsInCheck(a + 1, b, chess) == 1 || chess[1][a + 1][b] == '2') && (IsInCheck(a + 1, b + 1, chess) == 1 || chess[1][a + 1][b + 1] == '2'))
			return 1;
	}
	if (a != 0 && a != 7 && b == 7) {
		if ((IsInCheck(a - 1, b - 1, chess) == 1 || chess[1][a - 1][b - 1] == '2') && (IsInCheck(a - 1, b, chess) == 1 || chess[1][a - 1][b] == '2') && (IsInCheck(a, b - 1, chess) == 1 || chess[1][a][b - 1] == '2') && (IsInCheck(a + 1, b - 1, chess) == 1 || chess[1][a + 1][b - 1] == '2') && (IsInCheck(a + 1, b, chess) == 1 || chess[1][a + 1][b] == '2'))
			return 1;
	}


	

	return 0;
}
int Kcantmove2(int a, int b, char chess[2][8][8]) {


	if (a == 7 && b == 0) {
		if ((IsInCheck(a - 1, b, chess) == 1 || chess[1][a - 1][b] == '1') && (IsInCheck(a - 1, b + 1, chess) == 1 || chess[1][a - 1][b + 1] == '1') && (IsInCheck(a, b + 1, chess) == 1 || chess[1][a][b + 1] == '1'))

			return 1;
	}

	if (a == 7 && b != 0 && b != 7) {
		if ((IsInCheck(a - 1, b - 1, chess) == 1 || chess[1][a - 1][b - 1] == '1') && (IsInCheck(a - 1, b, chess) == 1 || chess[1][a - 1][b] == '1') && (IsInCheck(a - 1, b + 1, chess) == 1 || chess[1][a - 1][b + 1] == '1') && (IsInCheck(a, b - 1, chess) == 1 || chess[1][a][b - 1] == '1') && (IsInCheck(a, b + 1, chess) == 1 || chess[1][a][b + 1] == '1'))

			return 1;

	}
	if (a == 7 && b == 7) {
		if ((IsInCheck(a - 1, b - 1, chess) == 1 || chess[1][a - 1][b - 1] == '1') && (IsInCheck(a - 1, b, chess) == 1 || chess[1][a - 1][b] == '1') && (IsInCheck(a, b - 1, chess) == 1 || chess[1][a][b - 1] == '1'))
			return 1;
	}

	if (a == 0 && b == 0) {
		if ((IsInCheck(a, b + 1, chess) == 1 || chess[1][a][b + 1] == '1') && (IsInCheck(a + 1, b, chess) == 1 || chess[1][a + 1][b] == '1') && (IsInCheck(a + 1, b + 1, chess) == 1 || chess[1][a + 1][b + 1] == '1'))
			return 1;
	}
	if (a == 0 && b != 0 && b != 7) {
		if ((IsInCheck(a, b - 1, chess) == 1 || chess[1][a][b - 1] == '1') && (IsInCheck(a, b + 1, chess) == 1 || chess[1][a][b + 1] == '1') && (IsInCheck(a + 1, b - 1, chess) == 1 || chess[1][a + 1][b - 1] == '1') && (IsInCheck(a + 1, b, chess) == 1 || chess[1][a + 1][b] == '1') && (IsInCheck(a + 1, b + 1, chess) == 1 || chess[1][a + 1][b + 1] == '1'))
			return 1;
	}
	if (a == 0 && b == 7) {
		if ((IsInCheck(a, b - 1, chess) == 1 || chess[1][a][b - 1] == '1') && (IsInCheck(a + 1, b - 1, chess) == 1 || chess[1][a + 1][b - 1] == '1') && (IsInCheck(a + 1, b, chess) == 1 || chess[1][a + 1][b] == '1'))
			return 1;
	}
	if (a != 0 && a != 7 && b == 0) {
		if ((IsInCheck(a - 1, b, chess) == 1 || chess[1][a - 1][b] == '1') && (IsInCheck(a - 1, b + 1, chess) == 1 || chess[1][a - 1][b + 1] == '1') && (IsInCheck(a, b + 1, chess) == 1 || chess[1][a][b + 1] == '1') && (IsInCheck(a + 1, b, chess) == 1 || chess[1][a + 1][b] == '1') && (IsInCheck(a + 1, b + 1, chess) == 1 || chess[1][a + 1][b + 1] == '1'))
			return 1;
	}
	if (a != 0 && a != 7 && b == 7) {
		if ((IsInCheck(a - 1, b - 1, chess) == 1 || chess[1][a - 1][b - 1] == '1') && (IsInCheck(a - 1, b, chess) == 1 || chess[1][a - 1][b] == '1') && (IsInCheck(a, b - 1, chess) == 1 || chess[1][a][b - 1] == '1') && (IsInCheck(a + 1, b - 1, chess) == 1 || chess[1][a + 1][b - 1] == '1') && (IsInCheck(a + 1, b, chess) == 1 || chess[1][a + 1][b] == '1'))
			return 1;
	}




	return 0;
}

//gcc �����Ͻ� �ɼ� �߰�
// -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
