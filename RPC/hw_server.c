#include <rpc/rpc.h>
#include <string.h>

// Interface gerada pelo RPCGen a partir da IDL (hw.x) especificada
#include "hw.h"

char buffer[5000];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *send_1_svc(char **a, struct svc_req *req) {
	static int res = 1;

     	strcat(buffer, *a);

	printf("---- Message received:\n%s\n", *a);
	printf("---- Buffer:\n%s\n", buffer);

	return (&res);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
char **receive_1_svc(void *a, struct svc_req *req) {
     static char *p;

     p = buffer;
     
     return (&p);
}
