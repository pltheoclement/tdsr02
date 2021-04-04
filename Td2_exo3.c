#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

struct sigaction act_pere, act_fils;
pid_t pid_fils, pid_pere;
int it_fils = 0 , it_pere = 0;

void captfils()
{
	rectvert(5);
    it_fils++;
	printf("\n FILS %d : signal %d recu \n",getpid(), it_fils);
	if (it_fils == 3) {
		printf("\n FILS : fin du fils car trois signaux sont deja reçu. Le pid du mon pere est : %d\n",getppid());
		detruitrec(); /* detruire la fenetre graphique */
        exit(0);
	}
}

/*-------------------------------------------------*/
void captpere()
{
	it_pere++;
	printf("\n PERE %d : signal %d recu \n", getpid(),it_pere);
	if (it_pere == 3) {
		printf("\n PERE : fin du pere car trois signals sont deja reçu \n ");
		wait();
        exit(0);
	}
}

int main() {
    pid_fils = fork();
    if (pid_fils==-1) {perror("echec du fork"); exit(0);}
    if (pid_fils==0) { /* fils */
        int i=0;
        pid_pere=getppid();
        initrec();
	    act_fils.sa_handler=captfils;
	    sigaction(SIGINT,&act_fils,NULL);
        while(i != -1){
		    i = attendreclic();
		    if (i == 0) {
		    	printf("\n Le fils envoit un signal SIGINT au PERE\n"); kill(getppid(), SIGINT);
		    }
	    }
        printf("--fin du fils car clic sur fin--\n");
    } /* fin fils */

    else { /* pere */
        int n;
	    act_pere.sa_handler=captpere;
	    sigaction(SIGINT,&act_pere,NULL);
	    printf("\n PERE: \n");
	    while(1){
		    n=sleep(10); printf("\n Temps restant = %d \n",n);
	    }
    }
	return 0;
}



