#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int nrp = 10, n=10000; //numar procese, 10k numere

int verifyIfPrime(int n) { //verica daca un numar e prim 

    for(int i = 2; i<=n/2; i++)
        if(n%i ==0) return 0;
    return 1;
}

void childProc(int readPipe, int writePipe)
{
    int v[1000] = {};
    int i=1,j;
    int nr = 0;
    /*citim din pipe numarul procesului */
    read(readPipe, &i, sizeof(int));

    /*inceputul partii si sf partii */
    for(j=i*(n/nrp); j< (i+1)*(n/nrp); j++)
        if(j < n)
            {
                if(verifyIfPrime(j)==1)
                {
                v[nr] =j;
                nr++;
                }
            }

    /*trimitem in pipe cate numere prime sunt in vector*/
    write(writePipe, &nr, 4); 
    /*trimitem in pipe vectorul cu numere prime*/
    write(writePipe, &v, 4000);
    exit(1);
}

void parentProc(int readPipe, int writePipe)
{
    int j;
    int val[1000] ={};
    int nr;
    /*se scrie pt. fiecare proces numarul sau in conducta*/
    for(j=0; j < nrp; j++)
        /*scriem in pipe numarul procesului */
        write(writePipe, &j, sizeof(int));

        for(j=0; j< nrp; j++)
        {

            /* citim numarul de elemente*/
            read(readPipe, &nr, 4);
          /*citim din pipe vectorul cu nr prime pt fiecare proces*/
            read(readPipe, val, 4000);

            /*suma*/
            for(int i =1; i< nr; i++){
                printf("%d ", val[i]);
            }
            wait(NULL); /*eliberare resurse */
        }
    }
    int main()
    {
        int i;
        int j;
        int firstPipe[2]; /*0-citit 1-scris*/
        int secondPipe[2];
        int pid=1;

        if (pipe(firstPipe))
        {
            printf("error creare conducta!\n");
            exit(-1);
        }
        if (pipe(secondPipe))
        {
            printf("error creare conducta!\n");
            exit(-1);
        }
        for(j=0; j< nrp; j++)
        {
            pid = fork();
            if (pid)

            {

                if (pid == -1)

                {

                    printf("error creare proc. copil.\n");
                    exit(-1);
                }
            }
            else
            {
                childProc(firstPipe[0], secondPipe[1]);

            }
        }
        parentProc(secondPipe[0],firstPipe[1]);
    }
