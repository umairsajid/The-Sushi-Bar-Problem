#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define SITS 5

typedef sem_t semaphore;
int sitstaken = 0; // Numero silla estan sienod utilizadas

semaphore customersQueque;
semaphore mutex; //Proteger variable de acceso global sitstaken.

void down(sem_t * s);
void up(sem_t * s);
void * customer(void * arg);
void diner(int id);

int main(int argc, char* argv[])
{
	int n=0; // Cantidad de clientes.
	int *ids;
	pthread_t *cust;
	int i;

	sitstaken = 0;

	printf("argc: %d\n",argc);

	if(argc>1)
    {
    	printf("Entre por el if\n");
        n = atoi(argv[1]);
    }

    while(n<=0)
        scanf("%d",&n);

    printf("Creating %d customers\n",n);

    //Inicia el semaforo customersQueque en el valor de SITS
    sem_init(&customersQueque,0,SITS);
    sem_init(&mutex,0,1);

    //Reservar espacio dinámico a los vectores
    cust = (pthread_t *)malloc(n*sizeof(pthread_t));
    ids = (int *)malloc(n*sizeof(int));

    for( i=0;i<n;i++)
    {
    	//Referencia al hilo que se esta llamando
    	ids[i]=i;
    	//Inicia el hilo i-esimo del cliente y le asigna un identificador
    	pthread_create(&cust[i], 0, customer, &ids[i]); 

    }
    //Esperar a que los hilos terminen.
    for(i=0;i<n;i++)
    {
        pthread_join(cust[i],0);
    }
}
void * customer(void * arg)
{
	int id = *(int *) arg;
	printf("[%d] I want to diner...\n", id);
	//Pide entrar al bar.
	down(&customersQueque);
		down(&mutex);
			sitstaken = sitstaken +1;
		up(&mutex);

		
		diner(id);
		down(&mutex);
			sitstaken = sitstaken -1;
			printf("[%d]I'm leaving \n",id );
		up(&mutex);
		if(sitstaken==0) // Si ya salieron todos.
			up(&customersQueque);



}
void diner(int id)
{
	printf("[%d] I'm having dinner\n",id );
	sleep(rand()%8);
}
void down(sem_t * s)
{
	sem_wait(s); 
}
void up(sem_t * s)
{
	sem_post(s);
}
