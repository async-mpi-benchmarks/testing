#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#define longueur 800 //definition de la largeur et de la longeur de 
#define largeur 800	 // l'affichage graphique

//structure d'un point
typedef struct 
{

  int x;
  int y;
  
} point;

//fonction qui permet le calcul des cycles cpu
unsigned long long rdtsc()
{
	unsigned long long rdtsc;

	__asm__ volatile( "rdtsc;\n" : "=A" (rdtsc));

	return rdtsc;
}

//fonction qui permet de dessiner un point sur l'ecran en lui donnant la couleur, l'emplacement
//et le nombre de point a afficher
void draw(int colour, SDL_Renderer *renderer, point *position, int nb_point)
{
	for(int i = 0; i < nb_point; i++)
	{
		SDL_SetRenderDrawColor(renderer, colour, colour, colour, 255);
		SDL_RenderDrawPoint(renderer, position[i].x, position[i].y);
	}
}

//permet de changer l'emplacement de points, en indicant les points a changer et leur nombre
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
  MPI_Init(&argc, &argv);	//initialisation de l'utilisation des appel MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); //création du rank (chaque processus pocède son propre rank)
  MPI_Comm_size(MPI_COMM_WORLD, &size);	//permet de réuperer le nombre de processus sur le meme comm
  MPI_Request req = MPI_REQUEST_NULL; //création de la requete qui est utilise dans les envois/receptions bloquant

  //initialisation de la "graine" pour l'aleatoire avec l'utilisation du pid du programme
  srand(getpid());

  //création d'un point test pour créer le type point, ce type est necessaire pour l'envoi de la
  //structure point en MPI
  point init;

  int count = 2;

  //nom du nouveau type
  MPI_Datatype MPItype_point;

  //indication du nombre de variable différent dans la struct et leu type qui leur correspond
  MPI_Datatype old_types[count];
  old_types[0] = MPI_INT;
  old_types[1] = MPI_INT;

  //le nombre de variable qu'il y a dans chaque type precédemment citer (différent de 1 si on fait des tableau)
  int array_of_blocklengths[count];
  array_of_blocklengths[0] = 1;
  array_of_blocklengths[1] = 1;

  MPI_Aint array_of_displaysments[count];
  MPI_Aint base;

  //indication de la taille mémoire utilisé par chaque variable pour indiquer la taille mémoire du type
  //ainsi que le debut et la fin de la memoire attribue a chaque information dans la struct
	MPI_Get_address(&init, &base);
	MPI_Get_address(&init.x, &array_of_displaysments[0]);
	MPI_Get_address(&init.y, &array_of_displaysments[1]);
	array_of_displaysments[0] = MPI_Aint_diff(array_of_displaysments[0], base);
	array_of_displaysments[1] = MPI_Aint_diff(array_of_displaysments[1], base);

	//création du nouveau type avec toutes les information créé précedemment
	MPI_Type_create_struct(count, array_of_blocklengths, array_of_displaysments, old_types, &MPItype_point);
	MPI_Type_commit(&MPItype_point);

	//ouverture des fichier pour écrire les résultats
	FILE *f = fopen("result0.dat", "w");
	FILE *f2 = fopen("result1.dat", "w");

	//vérification que les deux fichiers ont bien été ouvert
  if(f == NULL || f2 == NULL)
  {
   	printf("erreur lors de l'ouverture du fichier dans le rank %d",rank);
    return 1;
  }

	//boucle qui permet d'effecute le programme plusieurs fois avec un nombre de point différent
	//(utilisé pour la mesure de performance avec des envoi de plus en plus grand)
	for(int nb_point = 1; nb_point <= 4096; nb_point *= 2)
	{
		printf("nb_point %d\n",nb_point);

		//initiamisation de SDL pour l'affichage
		if(SDL_Init(SDL_INIT_VIDEO) == -1)
		{
			printf("erreur lors de l'initialisation de SDL");
			return 1;
		}

		unsigned long long result1 = 0;
		unsigned long long result2 = 0;
		unsigned long long result3 = 0;

		//création des données utilisé dans sdl
		SDL_Window *window;
		SDL_Renderer *renderer;
		SDL_CreateWindowAndRenderer(800, 800, SDL_WINDOW_OPENGL, &window, &renderer);

		//création d'un event pour fermer la fenetre avec la croix (en haut a droite)
		SDL_Event close_event;

		//allocation de lamémoire pour nos tableaux de points
		point *position0 = malloc(nb_point * sizeof(point));
	  point *position1 = malloc(nb_point * sizeof(point));

	  //programme a effectuer pour le processus 0 
	  if(!rank)
	  {
	  	unsigned long long rdtsc1, rdtsc2, rdtsc3, rdtsc4;
	  	//placement des points et affichage de ces dernier
	  	placement(position0, nb_point);
	    draw(255, renderer, position0, nb_point);

	    //boucle qui permet de faire 100 fois les envoi et reception pour une mesure de performance plus précise
	    for(int i = 0; i < 100; i++)
	    {
	    	//vérification qui permet la fermeture de la fenetre avec la croix
	    	SDL_PollEvent(&close_event);
	    	if(close_event.type == SDL_QUIT)
	        break;

	      rdtsc1 = rdtsc();
	      //envoi non bloquant avec rdtsc avant et apres
		    MPI_Isend(position0, nb_point, MPItype_point, 1, 123, MPI_COMM_WORLD, &req);

		    rdtsc2 = rdtsc();

		    //calcul entre l'envoi et l'attente utilisé pour la mesure de performance
		    draw(0, renderer, position0, nb_point);
		    placement(position0, nb_point);
		    draw(255, renderer, position0, nb_point);
		    SDL_RenderPresent(renderer);
		    SDL_Delay(10);

		    rdtsc3 = rdtsc();
		    //fonction qui permet d'attendre que l'envoi soit terminé rdtsc avant et apres pour la mesure de performance
		    MPI_Wait(&req, MPI_STATUS_IGNORE);

		    rdtsc4 = rdtsc();

		    //réinitialisation de la requete a null pour les prochaines utilisations
		    req = MPI_REQUEST_NULL;

		    //réception bloquante (on ne fait pas de rdtsc car nous ne mesurons que les envoi pour le moment)
		    MPI_Recv(position1, nb_point, MPItype_point, 1, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		    //soustraction qui permet d'avoir la mesure rdtsc des appels
		    result1 += rdtsc2 - rdtsc1;
		    result2 += rdtsc3 - rdtsc2;
		    result3 += rdtsc4 - rdtsc3;

	    }
	    //fonction qui permet que chaque processus s'attende avant de continuer
	    MPI_Barrier(MPI_COMM_WORLD);

	    //affichage des rdtsc et des tailles d'envoi dans un fichier
	    fprintf(f, "isend %llu calcul %llu wait %llu  total %llu nb_val %d\n",result1/100, result2/100, result3/100, (result1 +result3)/100, nb_point);
	  }

	  //programme a effectuer pour le processus 1
	  if(rank)
	  {
	  	unsigned long long rdtsc1, rdtsc2, rdtsc3, rdtsc4;

	  	//placement des points et affichage de ces dernier
	  	placement(position1, nb_point);
	    draw(255, renderer, position1, nb_point);

	    //boucle qui permet de faire 100 fois les envoi et reception pour une mesure de performance plus précise
	    for(int i = 0; i < 100; i++)
	    {
	    	SDL_PollEvent(&close_event);

	    	if(close_event.type == SDL_QUIT)
	        break;

	      rdtsc1 = rdtsc();
	      //envoi bloquant
		    MPI_Send(position1, nb_point, MPItype_point, 0, 123, MPI_COMM_WORLD);

		    rdtsc2 = rdtsc();

		    //les memes calcul que dans l'autre processus mais les calculs ne se font pas en meme temps que l'envoi
		    draw(0, renderer, position1, nb_point);

		    placement(position1, nb_point);

		    draw(255, renderer, position1, nb_point);
		    
		    SDL_RenderPresent(renderer);

		    SDL_Delay(10);

		    rdtsc3 = rdtsc();

		    //reception bloquante
		    MPI_Recv(position0, nb_point, MPItype_point, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    
		    result1 += rdtsc2 - rdtsc1;
		    result2 += rdtsc3 - rdtsc2;

	    }

	    MPI_Barrier(MPI_COMM_WORLD);
	    fprintf(f2, "send %llu calcul %llu nb_val %d\n",result1/100, result2/100, nb_point);
	  }

	  //liberation de la mémoire des tableau de points
	  free(position1);
	  free(position0);

	  //libearation de la memoire allouer a l'affichage
	  SDL_DestroyRenderer(renderer);
	  SDL_DestroyWindow(window);
	  SDL_Quit();
	}
	//fermeture des fichiers
	fclose(f);
	fclose(f2);

	//liberation de la memoire du type créé et de la memoire allouer a MPI
  MPI_Type_free(&MPItype_point);
  MPI_Finalize();
}