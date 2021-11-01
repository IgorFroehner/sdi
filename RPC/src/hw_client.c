#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>		// sleep
#include <termios.h>  
#include <pthread.h>
#include <semaphore.h>
#include <rpc/rpc.h>
#include <time.h>

#include "../out/hw.h" 	// Interface gerada pelo RPCGen a partir da IDL (hw.x) especificada
#include "utils/utils.h"

#define RESET "\x1b[0m"
#define ECHOFLAGS (ICANON | ECHO | ECHOE)  

sem_t typing;
size_t username_max_len = 128, message_len = 1024;
char *username, *message;
bool running = true;
char *user_color;
int buffer_position = 0;

int set_disp_mode(int echo) {  
	struct termios oldtio, newtio;  
	tcgetattr(0, &oldtio);
    newtio = oldtio;
	if(echo) newtio.c_lflag |= ECHOFLAGS;
	else newtio.c_lflag &= ~ECHOFLAGS;  
	tcsetattr(0, TCSANOW, &newtio);  
	return 0;  
}

void *listener_thread(void *vargp) {
	CLIENT *cl = (CLIENT*) vargp;

	system("clear");
	printf("Welcome %s!\n\n", username);
	
	while (running) {
		while (true) {
			char **res = receive_1(&buffer_position, cl);
			if (strcmp(*res, "") == 0) break;
			printf("\r%*s", (int) strlen(message), " ");
			printf("\r%s", *res);
			printf("%s", message);
			fflush(stdout);
			buffer_position++;
		}
		sleep(1);
	}
	pthread_exit(NULL);
}

void *speaker_thread(void *vargp) {
	CLIENT *cl = (CLIENT*) vargp;

	message = (char*) malloc(message_len * sizeof(char));

	char c, *pos = message;
	while (c = getchar()) {
		if (32 <= c && c <= 126) {
			*pos++ = c;
			*pos = '\0';
			printf("%c", c);
		} else if (c == '\b' || c == 8 || c == 127) {
			printf("\b \b");
			*(--pos) = '\0';
		} else if (c == '\n') {
			message = trim(message);
			if (strcmp(message, "") == 0) continue;
			char *dup = strdup(message);
			snprintf(message, message_len, "%s%s%s: %s\n", user_color, username, RESET, dup);
			int *res = send_1(&message, cl);

			message[0] = '\0';
			pos = message;
		}
	}
	pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
	// Verificação dos parâmetros oriundos da console	
	if (argc != 2) {
		fprintf(stderr, "ERRO: ./client <hostname>\n");
		exit(1);
	}

	// Estrutura RPC de comunicação
	// Conexão com servidor RPC
	CLIENT* cl = clnt_create(argv[1], PROG, VERS, "tcp");
	if (cl == NULL) {
		clnt_pcreateerror(argv[1]);
		exit(1);
	}

	// read Username
	username = malloc(username_max_len * sizeof(char));
	printf("Type your username:\n");
	getline(&username, &username_max_len, stdin);
	username = trim(username);

	// give a color to username
	char *colors[] = {"\x1b[32m", "\x1b[31m", "\x1b[33m", "\x1b[34m", "\x1b[35m", "\x1b[36m"};
	int n_colors = 6;
	srand(time(NULL));
	int rand_color = rand() % n_colors;
	user_color = malloc(128 * sizeof(char));
	strcpy(user_color, colors[rand_color]);

	// use non-bufered non-echoed console
	set_disp_mode(0);

	// inicio das threads
	pthread_t listener, speaker;
	pthread_create(&listener, NULL, listener_thread, (void*) cl);
	pthread_create(&speaker, NULL, speaker_thread, (void*) cl);

	pthread_join(listener, NULL);
	pthread_join(speaker, NULL);

	// revert console
	set_disp_mode(1);

	return 0;
}
