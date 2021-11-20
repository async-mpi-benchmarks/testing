#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#define longueur 800
#define largeur 800

typedef struct 
{

  int x;
  int y;
  
} point;

unsigned long long rdtsc()
{
	unsigned long long rdtsc;

	__asm__ volatile( "rdtsc;\n" : "=A" (rdtsc));

	return rdtsc;
}

void draw(int colour, SDL_Renderer *renderer, point *position, int nb_point)
{
	for(int i = 0; i < nb_point; i++)
	{
		SDL_SetRenderDrawColor(renderer, colour, colour, colour, 255);
		SDL_RenderDrawPoint(renderer, position[i].x, position[i].y);
	}
}

void placement(point *position, int nb_point)
{
	for(int i = 0; i < nb_point; i++)
	{
		position[i].x = rand() % largeur;
		position[i].y = rand() % longueur;
	}
}

int main(int argc, char** argv)
{
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Request req = MPI_REQUEST_NULL; 


  srand(getpid());

  point init;

  int count = 2;
  MPI_Datatype MPItype_point;

  MPI_Datatype old_types[count];
  old_types[0] = MPI_INT;
  old_types[1] = MPI_INT;

  int array_of_blocklengths[count];
  array_of_blocklengths[0] = 1;
  array_of_blocklengths[1] = 1;

  MPI_Aint array_of_displaysments[count];
  MPI_Aint base;

	MPI_Get_address(&init, &base);
	MPI_Get_address(&init.x, &array_of_displaysments[0]);
	MPI_Get_address(&init.y, &array_of_displaysments[1]);
	array_of_displaysments[0] = MPI_Aint_diff(array_of_displaysments[0], base);
	array_of_displaysments[1] = MPI_Aint_diff(array_of_displaysments[1], base);

	MPI_Type_create_struct(count, array_of_blocklengths, array_of_displaysments, old_types, &MPItype_point);
	MPI_Type_commit(&MPItype_point);

	FILE *f = fopen("result0.dat", "w");
	FILE *f2 = fopen("result1.dat", "w");

  if(f == NULL || f2 == NULL)
  {
   	printf("erreur lors de l'ouverture du fichier dans le rank %d",rank);
    return 1;
  }

for(int nb_point = 1; nb_point <= 4096; nb_point *= 2)
{
	printf("nb_point %d\n",nb_point);

	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		printf("erreur lors de l'initialisation de SDL");
		return 1;
	}

	unsigned long long result1 = 0;
	unsigned long long result2 = 0;
	unsigned long long result3 = 0;

	SDL_Window *window;
	SDL_Renderer *renderer;

	SDL_CreateWindowAndRenderer(800, 800, SDL_WINDOW_OPENGL, &window, &renderer);

	SDL_Event close_event;

	point *position0 = malloc(nb_point * sizeof(point));
  point *position1 = malloc(nb_point * sizeof(point));

  if(!rank)
  {
  	unsigned long long rdtsc1, rdtsc2, rdtsc3, rdtsc4;

    draw(255, renderer, position0, nb_point);

    for(int i = 0; i < 100; i++)
    {
    	SDL_PollEvent(&close_event);

    	if(close_event.type == SDL_QUIT)
        break;

      rdtsc1 = rdtsc();

	    MPI_Isend(position0, nb_point, MPItype_point, 1, 123, MPI_COMM_WORLD, &req);

	    rdtsc2 = rdtsc();

	    draw(0, renderer, position0, nb_point);

	    placement(position0, nb_point);

	    draw(255, renderer, position0, nb_point);

	    SDL_RenderPresent(renderer);

	    SDL_Delay(10);

	    rdtsc3 = rdtsc();
	       
	    MPI_Wait(&req, MPI_STATUS_IGNORE);

	    rdtsc4 = rdtsc();

	    req = MPI_REQUEST_NULL;

	    MPI_Recv(position1, nb_point, MPItype_point, 1, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	    result1 += rdtsc2 - rdtsc1;
	    result2 += rdtsc3 - rdtsc2;
	    result3 += rdtsc4 - rdtsc3;

    }
    MPI_Barrier(MPI_COMM_WORLD);
    fprintf(f, "isend %llu calcul %llu wait %llu  total %llu nb_val %d\n",result1/100, result2/100, result3/100, (result1 +result3)/100, nb_point);
  }


  if(rank)
  {
  	unsigned long long rdtsc1, rdtsc2, rdtsc3, rdtsc4;

    draw(255, renderer, position1, nb_point);

    for(int i = 0; i < 100; i++)
    {
    	SDL_PollEvent(&close_event);

    	if(close_event.type == SDL_QUIT)
        break;

      rdtsc1 = rdtsc();

	    MPI_Send(position1, nb_point, MPItype_point, 0, 123, MPI_COMM_WORLD);

	    rdtsc2 = rdtsc();

	    draw(0, renderer, position1, nb_point);

	    placement(position1, nb_point);

	    draw(255, renderer, position1, nb_point);
	    
	    SDL_RenderPresent(renderer);

	    SDL_Delay(10);

	    rdtsc3 = rdtsc();

	    MPI_Recv(position0, nb_point, MPItype_point, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
	    result1 += rdtsc2 - rdtsc1;
	    result2 += rdtsc3 - rdtsc2;

    }
    MPI_Barrier(MPI_COMM_WORLD);
    fprintf(f2, "send %llu calcul %llu nb_val %d\n",result1/100, result2/100, nb_point);
  }

  free(position1);
  free(position0);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
	fclose(f);

  MPI_Type_free(&MPItype_point);
  MPI_Finalize();
}