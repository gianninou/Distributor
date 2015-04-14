#include "utils.h"


void* xmalloc(int nb){
	void* res = malloc(nb);
	if(!res){
		perror("Erreur malloc");
		exit(1);
	}
	return res;
}