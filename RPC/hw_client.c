#include <stdio.h>
#include <rpc/rpc.h>
#include <string.h>

// Interface gerada pelo RPCGen a partir da IDL (hw.x) especificada
#include "hw.h"

int main (int argc, char *argv[]) {
	// Estrutura RPC de comunicação
	CLIENT *cl;

	// Parâmetros das funçcões
	char        *message = (char *) malloc(1024*sizeof(char));

	// Verificação dos parâmetros oriundos da console	
	if (argc != 2) {
		printf("ERRO: ./client <hostname>\n");
		exit(1);
	}

	// Conexão com servidor RPC
	cl = clnt_create(argv[1], PROG, VERS, "tcp");
	if (cl == NULL) {
		clnt_pcreateerror(argv[1]);
		exit(1);
	}

	char aux;
	char *pos = message;
	while (aux = getchar()) {
		if (aux == EOF) {
			*pos = '\0';
			break;
		} 
		*pos++ = aux;
	}


	int *res = send_1(&message, cl);
	if (res == NULL) {
		clnt_perror(cl, argv[1]);
	    exit(1);
	}

	printf("\n%s\n", *receive_1(NULL, cl));

	// // Chamadas das funções remotas
	// printf ("Chamando func0 (sem parâmetros)\n");
	// ret_f0 = func0_1(NULL, cl);
	// if (ret_f0 == NULL) {
	//     clnt_perror(cl,argv[1]);
	//     exit(1);
	// }
	// printf ("Retorno func0 (%s)\n", *ret_f0);

	// printf ("Chamando func1 (%s)\n", par_f1);
	// ret_f1 = func1_1(&par_f1, cl);
	// if (ret_f1 == NULL) {
	//     clnt_perror(cl,argv[1]);
	//     exit(1);
	// }
    //     printf ("Retorno func1 (%d)\n", *ret_f1);

    //     printf ("Chamando func2 (%d)\n", par_f2);
	// ret_f2 = func2_1(&par_f2, cl);
	// if (ret_f2 == NULL) {
	//     clnt_perror(cl,argv[1]);
	//     exit(1);
	// }
    //     printf ("Retorno func2 (%d)\n", *ret_f2);

    //     printf ("Chamando func3 (%d/%d)\n", par_f3.arg1, par_f3.arg2);
    //     ret_f3 = func3_1(&par_f3, cl);
	// if (ret_f3 == NULL) {
	//     clnt_perror(cl,argv[1]);
	//     exit(1);
	// }
    //     printf ("Retorno func3 (%d)\n", *ret_f3);


	return 0;
}
