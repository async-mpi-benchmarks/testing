#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#define nb_point 100
#define longueur 800
#define largeur 800

typedef struct 
{

  int x;
  int y;
  
} point;

void draw(int colour, SDL_Renderer *renderer, point *position)
{
	for(int i = 0; i < nb_point; i++)
	{
		SDL_SetRenderDrawColor(renderer, colour, colour, colour, 255);
		SDL_RenderDrawPoint(renderer, position[i].x, position[i].y);
	}
}

void placement(point *position)
{
	for(int i = 0; i < nb_point; i++)
	{
		position[i].x = rand() % largeur;
		position[i].y = rand() % longueur;
	}
}

int main(int argc, char** argv)
{

	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		printf("erreur lors de l'initialisation de SDL");
		return 1;
	}

	SDL_Window *window;
	SDL_Renderer *renderer;

	SDL_CreateWindowAndRenderer(800, 800, SDL_WINDOW_OPENGL, &window, &renderer);

	SDL_Event close_event;

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);


  srand(getpid());

  point *position0 = malloc(nb_point * sizeof(point));
  point *position1 = malloc(nb_point * sizeof(point));

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

  if(!rank)
  {
    int test = 1;

    draw(255, renderer, position0);

    while(test)
    {
    	SDL_PollEvent(&close_event);

    	if(close_event.type == SDL_QUIT)
        break;

      draw(0, renderer, position0);

	    placement(position0);

      MPI_Send(position0, nb_point, MPItype_point, 1, 123, MPI_COMM_WORLD);

	    draw(255, renderer, position0);

	    SDL_RenderPresent(renderer);
     	SDL_Delay(10);

	    MPI_Recv(position1, nb_point, MPItype_point, 1, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    for(int i = 0; i < nb_point; i++)
	    {

	      for(int j = 0; j < nb_point; j++)
	      {
		      if(position0[i].x == position1[j].x && position0[i].y == position1[j].y)
		      {
		      	printf("rank %d position0[i].x =%d position1[j].x=%d position0[i].y =%d position1[j].y=%d\n",rank,position0[i].x,position1[j].x,position0[i].y,position1[j].y);
		        test = 0;
		      }
	      }
	    }
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }


  if(rank)
  {
    int test = 1;

    draw(255, renderer, position1);

    while(test)
    {
    	SDL_PollEvent(&close_event);

    	if(close_event.type == SDL_QUIT)
        break;

      draw(0, renderer, position1);

	    placement(position1);

	    MPI_Send(position1, nb_point, MPItype_point, 0, 123, MPI_COMM_WORLD);

	    draw(255, renderer, position1);

	    SDL_RenderPresent(renderer);
      SDL_Delay(10);

	    MPI_Recv(position0, nb_point, MPItype_point, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	    for(int i = 0; i < nb_point; i++)
	    {
	      for(int j = 0; j < nb_point; j ++)
	      {
		      if(position1[i].x == position0[j].x && position1[i].y == position0[j].y)
		      {
		      	printf("rank %d position1[i].x =%d position0[j].x=%d position1[i].y =%d position0[j].y=%d\n",rank,position1[i].x,position0[j].x,position1[i].y,position0[j].y);
		        test = 0;
		      }
	      }
	    }
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }

  free(position1);
  free(position0);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  MPI_Type_free(&MPItype_point);
  MPI_Finalize();
}