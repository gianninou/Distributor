#include "clientExec.h"


ClientExec* newClientExec(){
	ClientExec* cli = (ClientExec*)xmalloc(sizeof(ClientExec));
	cli->prog = "python";
	cli->arg1 = "./algo/friedman.py";
	cli->arg2 = (char*)xmalloc(sizeof(char)*20);
	return cli;
}


void deleteClienExec(ClientExec* cli){
	/*if(cli->prog){
		free(cli->prog);	
	}
	if(cli->arg1){
		free(cli->arg1);
	}
	if(cli->arg2){
		free(cli->arg2);
	}*/
	free(cli);
}


char* execClientExec(ClientExec* cli, int nb){
	char* result = (char*)malloc(sizeof(char)*MAX_LENGTH);
	memset(result,0,MAX_LENGTH);
	memset(result,0,MAX_LENGTH);
	pid_t pid;
	int fd[2];
	pipe(fd);
	if(!(pid=fork())){
		/* fils */
		close(fd[0]);
		dup2(fd[1],1);
		sprintf(cli->arg2,"%d",nb);
		execlp(cli->prog,cli->prog,cli->arg1,cli->arg2,NULL);
		perror("Erreur d'execution");
		exit(0);
	}
	close(fd[1]);

	/* TODO check return */
	//sleep(1);
	waitpid(pid,NULL,0);

	int r = read(fd[0],result,MAX_LENGTH-1);
	/*printf("RESULTAT %d %d : '%s'\n", nb, r, result);*/
	if(r){
		return result;
	}else{
		free(result);
		return NULL;
	}
}



