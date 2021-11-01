#include <rpc/rpc.h>
#include <string.h>

// Interface gerada pelo RPCGen a partir da IDL (hw.x) especificada
#include "../out/hw.h"

void censor(char*, int, int);
char* censor_palindrome(char*);

int buffer_position = 0;
char *buffer[4096];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *send_1_svc(char **a, struct svc_req *req) {
	static int res = 1;

	printf("Message received: %d %s", buffer_position, *a);

	// if the message has a palidrome it is considered dangerous
    *a = censor_palindrome(*a);
    buffer[buffer_position++] = strdup(*a);

	return (&res);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
char **receive_1_svc(int *idx, struct svc_req *req) {
    static char *p;

    // printf("Message requested: %d %d\n", *idx, buffer_position);
    if (*idx >= buffer_position) p = "";
    else p = buffer[*idx];
    return (&p);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void censor(char* s, int l, int r) {
	for (int i = l; i <= r; i++) s[i] = '*';
}

char* censor_palindrome(char *s) {
    char *novo = malloc((5+strlen(s)) * sizeof(char));
    strcpy(novo, s);

    int str_len = strlen(s);
    int min_censor_sz = 4;
    for (int i = 0; i < str_len; i++) {
	    int sz, l = i, r = i;
        for (sz = 0; 0 <= i-sz && i+sz < str_len && s[i-sz] == s[i+sz]; sz++);
	    if (2*sz-1 > r-l+1) l = i-sz+1, r = i+sz-1;

	    for (sz = 0; 0 <= i-sz-1 && i+sz < str_len && s[i-sz-1] == s[i+sz]; sz++);
	    if (2*sz > r-l+1) l = i-sz, r = i+sz-1;
	
	    if (r-l+1 >= min_censor_sz) censor(novo, l, r);
    }
    return novo;
}
