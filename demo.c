//Using libs SDL, glibc
#include <SDL.h>	//SDL version 2.0
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <time.h>

// Numbers bitmap, some routines and and SDL initialization taken from
// https://github.com/flightcrank/pong


// Please follow the coding guidelines described in:
// https://users.ece.cmu.edu/~eno/coding/CCodingStandard.html



/* Do not use magic numbers in the code, define the meaning of the number
   in this section and the use the define across the code. This increases
   readability accross the code
*/


// The constants used to define the size of the window
#define SCREEN_WIDTH 1280	//window height
#define SCREEN_HEIGHT 505	//window width

// The constants used for the movement of the block
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3


// Block size that will be used for drawing on the screen
#define BLOCK_SIZE 50


// Coordinates of the starting player
#define PLAYER_START_X 20
#define PLAYER_START_Y 20

// For the return of the functions
#define SUCCESS 0
#define FAILURE 1

// Defines of the game states of each screen
#define START_SCREEN 0
#define START_GAME 1
#define GAME_OVER 2
#define WINNER 5
#define NEXT_LEVEL 4
#define ARRIBA 0
#define ABAJO 3
#define IZQUIERDA 1
#define DERECHA 2

// Defines for states
#define FALSE 0
#define TRUE 1

// The colors used in the game
#define BLACK 0x000000ff
#define WHITE 0xffffffff

// Timing delays
#define INPUT_DELAY_MS 500

// Time to render a frame in ms = 1000/60
#define FRAME_TIME_MS 16

// Movement diferential
#define MOVEMENT_DELTA 5

//Estado del juego
int state = START_SCREEN;

//function prototypes
//initialise SDL
int init_SDL(int w, int h, int argc, char *args[]);


typedef struct player_t {

	int x; 
	int y;
	int w; 
	int h;
	int vidas;
	int estado;
	int puntos;
	int timer;
	int timer2;
	int score1;
	int score2;

} player_t;

typedef struct game_element_t {
	int x;
	int y;
	int w;
	int h;
	int tipo;
	int timer;
	int explotar;

} game_element_t;

typedef struct enemigo_t {
	int x;
	int y;
	int w;
	int h;
	int tipo;
	int estado;
	int colision;
	int dir;
	int predir;
	int timer;
	int timer2;
	

} enemigo_t;

typedef struct bomba_t {
	int x;
	int y;
	int w;
	int h;
	int timer1; // contador antes de explotar
	int timer2; // contador durante explosion
	int estado; // estado = 1 => Explotando
	int explosion;
	int explosionh0;
	int explosionh1;
	int explosionw0;
	int explosionw1;
	int corte;
	int colision;
	int destx;
	int desty;
	int tipohw;


} bomba_t;

// This is one of the few cases where it makes sense to use magic numbers
// Avoid the use of global variables at maximum
int g_score[] = {0,0}; 
// Avoid the use of global variables, modify the code to avoid its use.
int g_width, g_height;		//used if fullscreen
int map=0;
int start =0;
int level =1;
int cantidad = 6;
int numEn1=0, numEn2=0, numEn3=0, numEn4=0, numEn5=0; 
int checkdir = 0;

int count_E=0;
int bumeran_E =0;
int destx_E=0;
int count_E3=0;
int count_E4=0;
int bumeran_E4 =0; 
int destx_E4=0;
int count_E5=0;
int bumeran_E5 =0; 
int destx_E5=0;
int bumeran_E3 =0;
int destx_E3=0;
int gate=0;
int posgate = 0;
int nextleveltimer = 0;

SDL_Window* window = NULL;	//The window we'll be rendering to
SDL_Renderer *renderer;		//The renderer SDL will use to draw to the screen

//Variables
static player_t J;
static game_element_t muros[200];
static bomba_t bombas[60];
static enemigo_t enemigos[20];



//Puntero a imagenes
	//Pantallas
static SDL_Surface *end;
static SDL_Surface *screen;
static SDL_Surface *title;
static SDL_Surface *winn;
static SDL_Surface *ground;
static SDL_Surface *ground1;
static SDL_Surface *ground3;
static SDL_Surface *ground0;
	//Marco
static SDL_Surface *rtg;
static SDL_Surface *vida;
static SDL_Surface *nivel;
static SDL_Surface *puntos;
static SDL_Surface *numbermap;
static SDL_Surface *bloqueMarcoh1;
static SDL_Surface *bloqueMarcoh2;
static SDL_Surface *bloqueMarcow1;
static SDL_Surface *bloqueMarcow2;
	//Jugador
static SDL_Surface *jugador;
static SDL_Surface *jugadorm;
	//Enemigo
static SDL_Surface *enemigo1;
static SDL_Surface *enemigo2;
static SDL_Surface *enemigo3;
static SDL_Surface *enemigo4;
static SDL_Surface *enemigo5;
static SDL_Surface *enemigomuerto;
	//muros
static SDL_Surface *muro1;
static SDL_Surface *muro2;
static SDL_Surface *muro3;
static SDL_Surface *muro4;
static SDL_Surface *muro5;
static SDL_Surface *puerta;
static SDL_Surface *puerta2;
static SDL_Surface *muro2Exp;
	//bomba
static SDL_Surface *bomba;
static SDL_Surface *llamah1;
static SDL_Surface *llamav1;


//textures
SDL_Texture *screen_texture;

// Always make a header for each one of the use created functions
static void init_level_1();
static void init_level_2();
static void init_level_3();
static void draw_ground1();
static void draw_ground2();
static void draw_ground3();
static void init_enemy(struct enemigo_t *enemigos, int cantidad);
/* Function: init_game
 * ---------------------------------
 * This function initialises the position of the player in the screen
 * (the block that can be moved with the WASD keys), and the block that
 * is part of the map, which is a test for collisions. Please note that
 * in this functions we are passing pointers, not values.
 *
 * Arguments:
 *	player:	Pointer to the player object
 *	map_element: Pointer to the map element object.
 * 
 * Return:
 * 	void.
 */
static void init_game(struct enemigo_t *enemigos, int cantidad) {
	// Here the function is receiving the pointer to the player object
	// it modifies the player object directly
	J.x = 331;
	J.y = 54;
	J.w = 22;
	J.h = 30;
	J.puntos = 0;
	J.estado = 0;
	J.vidas = 3;
	J.timer = 0;
	J.score1 =0;
	J.score2 =0;
	numEn1 = 0;
	checkdir = 0;
	gate=0; 

	count_E=0;
	bumeran_E =0;
	destx_E=0;
	count_E3=0;
	count_E4=0;
	bumeran_E4 =0; 
	destx_E4=0;
	count_E5=0;
	bumeran_E5 =0; 
 	destx_E5=0;
	bumeran_E3 =0;
	destx_E3=0;
 
	srand(time(NULL));

	posgate = rand() % 4;

	for (int i = 0; i < 60; i++)
	{
		bombas[i].estado = 0;
	}
	
	if(map == 1){
	init_level_1();
	draw_ground1();
	}else if(map == 2){
	init_level_2();
	draw_ground2();
	}else if(map == 3){
	init_level_3();
	draw_ground3();
	}
	
	for (int i = 0; i < cantidad; i++){
 
		enemigos[i].x = 0;
		enemigos[i].y = 0;
		enemigos[i].w = 0;
		enemigos[i].h = 0;
		enemigos[i].tipo = 0;
		enemigos[i].colision = 0;
		enemigos[i].dir = 0;
		enemigos[i].predir = 0; 
		enemigos[i].timer = 0;
		enemigos[i].timer2 = 0;
		enemigos[i].estado = 0; 
			
	}

	init_enemy(enemigos,cantidad);

}

static void draw_gate(){

	SDL_Rect src;
	SDL_Rect dest;

	posgate = 0;
	if(posgate == 0){
		muros[60].x = 450;			//Cargar datos al primer muro, para usar como referencia de coordenadas
		muros[60].y = 86;
		muros[60].w = 30;
		muros[60].h = 30; 
	}else if(posgate == 1){
		muros[60].x = 420;			//Cargar datos al primer muro, para usar como referencia de coordenadas
		muros[60].y = 236;
		muros[60].w = 30;
		muros[60].h = 30; 
	}
	else if(posgate == 2){
		muros[60].x = 720;			//Cargar datos al primer muro, para usar como referencia de coordenadas
		muros[60].y = 296;
		muros[60].w = 30;
		muros[60].h = 30; 
	}
	else if(posgate == 3){
		muros[60].x = 870;			//Cargar datos al primer muro, para usar como referencia de coordenadas
		muros[60].y = 326;
		muros[60].w = 30;
		muros[60].h = 30; 
	}
	src.x = 0;
	src.y = 0;
	src.w = 30;
	src.h = 30;

	dest.x = muros[60].x;
	dest.y = muros[60].y;
	dest.w = 30;
	dest.h = 30;
	

	if(gate == 0){  
		muros[60].tipo = 2;  
	}
	else if(gate == 1){ 
		muros[60].tipo = 7;  

		if(J.score1 <= 4){
			SDL_BlitSurface(puerta, &src, screen, &dest);
		}else if(J.score1 > 4){
			SDL_BlitSurface(puerta2, &src, screen, &dest);
		}
	}
			
	 

}

static void marco(){
	muros[100].x = 233;
	muros[100].y = 60;
	muros[100].w = bloqueMarcoh1->w;
	muros[100].h = 420;
	muros[100].tipo = 3;

	muros[101].x = 960;
	muros[101].y = 60;
	muros[101].w = bloqueMarcoh2->w;
	muros[101].h = 420;
	muros[101].tipo = 4;

	muros[102].x = 233;
	muros[102].y = 10;
	muros[102].w = bloqueMarcow1->w;
	muros[102].h = bloqueMarcow1->h;
	muros[102].tipo = 5;

	muros[103].x = 233;
	muros[103].y = 450;
	muros[103].w = bloqueMarcow2->w;
	muros[103].h = bloqueMarcow2->h;
	muros[103].tipo = 6;
	
}

static void draw_num_vidas() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) - src.w - 545; //12 is just padding spacing
	dest.y = 62;
	dest.w = 64;
	dest.h = 64;

	SDL_BlitSurface(vida , &src, screen, &dest);

}

static void draw_img_vidas() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = J.vidas * 64;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) - 542;
	dest.y = 60;
	dest.w = 64;
	dest.h = 64;
	
	

	SDL_BlitSurface(numbermap, &src, screen, &dest);


}

static void draw_space_vidas() {
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 172;
	src.h = 61;

	dest.x = (screen->w / 2) - src.w - 435; 
	dest.y = 64;
	dest.w = 172;
	dest.h = 61;

	SDL_BlitSurface(rtg , &src, screen, &dest);
}

static void draw_num_level() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = 570; //12 is just padding spacing
	dest.y = 435;
	dest.w = 64;
	dest.h = 64;

	SDL_BlitSurface(nivel , &src, screen, &dest);

}

static void draw_img_level() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = map * 64;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = 665;
	dest.y = 443;
	dest.w = 64;
	dest.h = 64;
	
	

	SDL_BlitSurface(numbermap, &src, screen, &dest);


}

static void draw_space_level() {
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 172;
	src.h = 44;

	dest.x = 560; 
	dest.y = 452;
	dest.w = 172;
	dest.h = 50;

	SDL_BlitSurface(rtg , &src, screen, &dest);
}

static void draw_img_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = 43; //12 is just padding spacing
	dest.y = 253;
	dest.w = 64;
	dest.h = 64;

	SDL_BlitSurface(puntos , &src, screen, &dest);

}

static void draw_num1_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = J.score1 * 64;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = 107;
	dest.y = 258;
	dest.w = 64;
	dest.h = 64;
	
	

	SDL_BlitSurface(numbermap, &src, screen, &dest);


}

static void draw_num2_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = 150;
	dest.y = 258;
	dest.w = 64;
	dest.h = 64;
	
	

	SDL_BlitSurface(numbermap, &src, screen, &dest);


}

static void draw_space_score() {
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 172;
	src.h = 61;

	dest.x = (screen->w / 2) - src.w - 435; 
	dest.y = 258;
	dest.w = 172;
	dest.h = 61;

	SDL_BlitSurface(rtg , &src, screen, &dest);
}

int count_M=0;
int srcx = 0;
int bumeran_M=0;
/* Function: draw_menu
 * --------------------------------
 * This function is in charge of drawing the first screen that is
 * presented to the player.
 *
 * Arguments:
 *	none
 *
 * Return:
 *	void.
 */
static void draw_menu() {

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = title->w;
	src.h = title->h;

	dest.x = (screen->w / 2) - (src.w / 2);
	dest.y = (screen->h / 2) - (src.h / 2);
	dest.w = title->w;
	dest.h = title->h;

	SDL_BlitSurface(title, &src, screen, &dest);
}

static void draw_ground0() {

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = ground0->w;
	src.h = ground0->h;

	dest.x = 0;
	dest.y = 0;
	dest.w = ground0->w;
	dest.h = ground0->h;

	SDL_BlitSurface(ground0, &src, screen, &dest);

	for (int i = 0; i < 104; i++){
		dest.x = muros[i].x;
		dest.y = muros[i].y;
		dest.w = muros[i].w;
		dest.h = muros[i].h;

		src.x = 0;
		src.y = 0;
		src.w = muros[i].w;
		src.h = muros[i].h;

		if(muros[i].tipo == 0){
			muros[i].x = 0;
			muros[i].y = 0;
			muros[i].w = 0;
			muros[i].h = 0;
			muros[i].timer = 0;
			muros[i].explotar = 0;
		
		}
		else if(muros[i].tipo == 1){
			SDL_BlitSurface(muro1, &src, screen, &dest);
		}else if(muros[i].tipo == 3){
			SDL_BlitSurface(bloqueMarcoh1, &src, screen, &dest);
		}
		else if(muros[i].tipo == 4){
			SDL_BlitSurface(bloqueMarcoh2, &src, screen, &dest);
		}
		else if(muros[i].tipo == 5){
			SDL_BlitSurface(bloqueMarcow1, &src, screen, &dest);
		}
		else if(muros[i].tipo == 6){
			SDL_BlitSurface(bloqueMarcow2, &src, screen, &dest);
		}
		else if(muros[i].tipo == 2){
			if(muros[i].explotar == 0){
				if(count_M >= 200){ 
				if(srcx <=0){ bumeran_M = 1;}
				if(srcx >= 90){ bumeran_M = 0;}
					if(bumeran_M == 1){srcx = srcx + 30;}
					if(bumeran_M == 0){srcx = 0;}
					count_M =0;
					}
					src.x =  srcx;
					count_M++;
					SDL_BlitSurface(muro2, &src, screen, &dest);
			}
			if(muros[i].explotar == 1){
				if(muros[i].timer > 0){
					muros[i].timer = muros[i].timer - 1; 
					SDL_BlitSurface(muro2Exp, &src, screen, &dest);
				}
				if(muros[i].timer == 0){
					muros[i].tipo = 0;
				}
			}
			
		}
	}
}

static void draw_ground1() {
	draw_ground0();

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 330;
	src.y = 58;
	src.w = 630;
	src.h = 390;

	dest.x = 0;
	dest.y = 0;
	dest.w = 630;
	dest.h = 390;

	SDL_BlitSurface(ground, &dest, screen, &src);
 
	for (int i = 0; i < 104; i++){
		dest.x = muros[i].x;
		dest.y = muros[i].y;
		dest.w = muros[i].w;
		dest.h = muros[i].h;

		src.x = 0;
		src.y = 0;
		src.w = muros[i].w;
		src.h = muros[i].h;

		if(muros[i].tipo == 0){
			muros[i].x = 0;
			muros[i].y = 0;
			muros[i].w = 0;
			muros[i].h = 0;
			muros[i].timer = 0;
			muros[i].explotar = 0;
		
		}
		else if(muros[i].tipo == 1){
			SDL_BlitSurface(muro3, &src, screen, &dest);
		}else if(muros[i].tipo == 3){
			SDL_BlitSurface(bloqueMarcoh1, &src, screen, &dest);
		}
		else if(muros[i].tipo == 4){
			SDL_BlitSurface(bloqueMarcoh2, &src, screen, &dest);
		}
		else if(muros[i].tipo == 5){
			SDL_BlitSurface(bloqueMarcow1, &src, screen, &dest);
		}
		else if(muros[i].tipo == 6){
			SDL_BlitSurface(bloqueMarcow2, &src, screen, &dest);
		}
		else if(muros[i].tipo == 2){
			if(muros[i].explotar == 0){
				/*if(count_M >= 200){ 
				if(srcx <=0){ bumeran_M = 1;}
				if(srcx >= 90){ bumeran_M = 0;}
					if(bumeran_M == 1){srcx = srcx + 30;}
					if(bumeran_M == 0){srcx = 0;}
					count_M =0;
					}
					src.x =  srcx;
					count_M++;*/
					SDL_BlitSurface(muro4, &src, screen, &dest);
			}
			if(muros[i].explotar == 1){
				if(muros[i].timer > 0){
					muros[i].timer = muros[i].timer - 1; 
					SDL_BlitSurface(muro5, &src, screen, &dest);
				}
				if(muros[i].timer == 0){
					muros[i].tipo = 0;
				}
			}
			
		}
	}

	 
}
static void draw_ground2() {
	draw_ground0();

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 330;
	src.y = 58;
	src.w = 630;
	src.h = 390;

	dest.x = 0;
	dest.y = 0;
	dest.w = 630;
	dest.h = 390;

	SDL_BlitSurface(ground, &dest, screen, &src);
 
	for (int i = 0; i < 104; i++){
		dest.x = muros[i].x;
		dest.y = muros[i].y;
		dest.w = muros[i].w;
		dest.h = muros[i].h;

		src.x = 0;
		src.y = 0;
		src.w = muros[i].w;
		src.h = muros[i].h;

		if(muros[i].tipo == 0){
			muros[i].x = 0;
			muros[i].y = 0;
			muros[i].w = 0;
			muros[i].h = 0;
			muros[i].timer = 0;
			muros[i].explotar = 0;
		
		}
		else if(muros[i].tipo == 1){
			SDL_BlitSurface(muro1, &src, screen, &dest);
		}else if(muros[i].tipo == 3){
			SDL_BlitSurface(bloqueMarcoh1, &src, screen, &dest);
		}
		else if(muros[i].tipo == 4){
			SDL_BlitSurface(bloqueMarcoh2, &src, screen, &dest);
		}
		else if(muros[i].tipo == 5){
			SDL_BlitSurface(bloqueMarcow1, &src, screen, &dest);
		}
		else if(muros[i].tipo == 6){
			SDL_BlitSurface(bloqueMarcow2, &src, screen, &dest);
		}
		else if(muros[i].tipo == 2){
			if(muros[i].explotar == 0){
				 
					SDL_BlitSurface(muro4, &src, screen, &dest);
			}
			if(muros[i].explotar == 1){
				if(muros[i].timer > 0){
					muros[i].timer = muros[i].timer - 1; 
					SDL_BlitSurface(muro5, &src, screen, &dest);
				}
				if(muros[i].timer == 0){
					muros[i].tipo = 0;
				}
			}
			
		}
	}


	 
}
static void draw_ground3() {
	draw_ground0();

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 330;
	src.y = 58;
	src.w = 630;
	src.h = 390;

	dest.x = 0;
	dest.y = 0;
	dest.w = 630;
	dest.h = 390;

	SDL_BlitSurface(ground, &dest, screen, &src);

	for (int i = 0; i < 104; i++){
		dest.x = muros[i].x;
		dest.y = muros[i].y;
		dest.w = muros[i].w;
		dest.h = muros[i].h;

		src.x = 0;
		src.y = 0;
		src.w = muros[i].w;
		src.h = muros[i].h;

		if(muros[i].tipo == 0){
			muros[i].x = 0;
			muros[i].y = 0;
			muros[i].w = 0;
			muros[i].h = 0;
			muros[i].timer = 0;
			muros[i].explotar = 0;
		
		}
		else if(muros[i].tipo == 1){
			SDL_BlitSurface(muro1, &src, screen, &dest);
		}else if(muros[i].tipo == 3){
			SDL_BlitSurface(bloqueMarcoh1, &src, screen, &dest);
		}
		else if(muros[i].tipo == 4){
			SDL_BlitSurface(bloqueMarcoh2, &src, screen, &dest);
		}
		else if(muros[i].tipo == 5){
			SDL_BlitSurface(bloqueMarcow1, &src, screen, &dest);
		}
		else if(muros[i].tipo == 6){
			SDL_BlitSurface(bloqueMarcow2, &src, screen, &dest);
		}
		else if(muros[i].tipo == 2){
			if(muros[i].explotar == 0){
				if(count_M >= 200){ 
				if(srcx <=0){ bumeran_M = 1;}
				if(srcx >= 90){ bumeran_M = 0;}
					if(bumeran_M == 1){srcx = srcx + 30;}
					if(bumeran_M == 0){srcx = 0;}
					count_M =0;
					}
					src.x =  srcx;
					count_M++;
					SDL_BlitSurface(muro2, &src, screen, &dest);
			}
			if(muros[i].explotar == 1){
				if(muros[i].timer > 0){
					muros[i].timer = muros[i].timer - 1; 
					SDL_BlitSurface(muro2Exp, &src, screen, &dest);
				}
				if(muros[i].timer == 0){
					muros[i].tipo = 0;
				}
			}
			
		}
	}
}
 

static void init_level_1(){
	cantidad = 6;
	for (int i = 0; i < 200; i++) //Limpiar datos del arreglo de muros, para dibujar el mapa
	{
	muros[i].x = 0;
	muros[i].y = 0;
	muros[i].w = 0;
	muros[i].h = 0;
	muros[i].tipo = 0;
	}
	
	marco();

	muros[0].x = 360;			//Cargar datos al primer muro, para usar como referencia de coordenadas
	muros[0].y = 86;
	muros[0].w = 30;
	muros[0].h = 30;
	muros[0].tipo = 1;

	int columna=0;
	for (int ms = 1; ms < 60; ms++){ //Muros estaticos (ms). cargar datos en el arreglo del 0 al 29
		
		if(columna < 10){
		muros[ms].x = muros[ms-1].x  + 60;
		muros[ms].y = muros[ms-1].y;
		muros[ms].w = 30;
		muros[ms].h = 30;
		muros[ms].tipo = 1;
		columna=columna+1;
		}
		
		if(columna >= 10){
		muros[ms].x = muros[0].x ;
		muros[ms].y = muros[ms -1].y + 60;
		muros[ms].w = 30;
		muros[ms].h = 30;
		muros[ms].tipo = 1;
		columna = 0;
		}
				
				
	}
/*
	for (int mdx= 60; mdx < 80; mdx++){ //Muros dinamicos (md). Recorrer y cargar datos en el arreglo de muros del 30 al 49
	
			muros[mdx].w = 30;
			muros[mdx].h = 30;
			muros[mdx].tipo = 1;

		int numeroAleatorio_mdx = rand() % 690+ 330; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 370 en x para coincidir con el mapa
		for (int i = 0; i < 60; i++){	

			int errorx = abs(numeroAleatorio_mdx - muros[i].x);
			
			if ( (errorx >= 0) && (errorx < 15)){
				muros[mdx].x = muros[i].x;
				muros[mdx].y = muros[i].y;
				
				
				break;

			}else if ( (errorx >= 15) && (errorx < 30) ){
				muros[mdx].x = muros[i].x+30;
				muros[mdx].y = numeroAleatorio_mdx;
				
				
				break;

			}
			
			if ( (i >= 59) && (errorx >= 30) ){//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
					i = rand() % 60;
					muros[mdx].x = muros[i].x;
					muros[mdx].y = muros[i].y;
				}
		}
			
		int numeroAleatorio_mdy = rand() % 330+ 60; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 100 en y	
		for (int j = 0; j < 60; j++){
			int errory = abs(numeroAleatorio_mdy - muros[j].y);
			
			if ( (errory >= 0) && (errory < 15)){
				muros[mdx].x = muros[mdx].x;
				muros[mdx].y = muros[j].y;
				muros[mdx].w = 30;
				muros[mdx].h = 30;
				muros[mdx].tipo = 1;
				
				break;
			}else if ( (errory >= 15) && (errory < 30)){
				muros[mdx].x = muros[mdx].x;
				muros[mdx].y = muros[j].y + 30;
				muros[mdx].w = 30;
				muros[mdx].h = 30;
				muros[mdx].tipo = 1;
				
				break;
			}

			if ( (j >= 59) && (errory >= 30) ){	//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
					j = rand() % 60;
					muros[mdx].x = muros[j].x;
					muros[mdx].y = muros[j].y;
				}
		}
			
	}
*/
	for (int mdx2= 61; mdx2 < 100; mdx2++){ //Muros dinamicos (md). Recorrer y cargar datos en el arreglo de muros del 30 al 49
	
			muros[mdx2].w = 30;
			muros[mdx2].h = 30;
			muros[mdx2].tipo = 2;

		int numeroAleatorio_mdx2 = rand() % 690 + 330; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 370 en x para coincidir con el mapa
		for (int m = 0; m < 61; m++){	

			int errorx2 = abs(numeroAleatorio_mdx2 - muros[m].x);
			
			if ( (errorx2 >= 0) && (errorx2 < 15)){
				muros[mdx2].x = muros[m].x;
				muros[mdx2].y = muros[m].y;
				 	
				
				break;

			}else if ( (errorx2 >= 15) && (errorx2 < 30) ){
				muros[mdx2].x = muros[m].x+30;
				muros[mdx2].y = muros[mdx2].y;
				
				
				break;

			}
			
			if ( (m >= 59) && (errorx2 >= 30) ){//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
				muros[mdx2].x = 0;
				muros[mdx2].y = 0;
				muros[mdx2].w = 0;
				muros[mdx2].h = 0;
				muros[mdx2].tipo = 0;
			}
		}
			
		int numeroAleatorio_mdy = rand() % 330 + 60; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 100 en y	
		for (int n = 0; n < 61; n++){
			int errory2 = abs(numeroAleatorio_mdy - muros[n].y);
			
			if ( (errory2 >= 0) && (errory2 < 15)){
				muros[mdx2].y = muros[n].y;

					for (int borrar1 = 0; borrar1 < 61; borrar1++){
						if((muros[mdx2].x == muros[borrar1].x) && (muros[mdx2].y == muros[borrar1].y)){
							muros[mdx2].x = 0;
							muros[mdx2].y = 0;
							muros[mdx2].w = 0;
							muros[mdx2].h = 0;
							muros[mdx2].tipo = 0;
						}
					}

				
				break;

			}else if ( (errory2 >= 15) && (errory2 < 30)){
				
				muros[mdx2].y = muros[n].y + 30;

					for (int borrar2 = 0; borrar2 < 61; borrar2++){
						if((muros[mdx2].x == muros[borrar2].x) && (muros[mdx2].y == muros[borrar2].y)){
							muros[mdx2].x = 0;
							muros[mdx2].y = 0;
							muros[mdx2].w = 0;
							muros[mdx2].h = 0;
							muros[mdx2].tipo = 0;
						}
					}
			
				
				break;
			}

			if ( (n >= 60) && (errory2 >= 30) ){	//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
						muros[mdx2].x = 0;
						muros[mdx2].y = 0;
						muros[mdx2].w = 0;
						muros[mdx2].h = 0;
				}
				
		}

		if((muros[mdx2].x == muros[60].x) && (muros[mdx2].y == muros[60].y)){
			muros[mdx2].x = 0;			//Cargar datos al primer muro, para usar como referencia de coordenadas
			muros[mdx2].y = 0;
			muros[mdx2].w = 0;
			muros[mdx2].h = 0;
			muros[mdx2].tipo = 0;
		}
		
			
	}
 
	 
}
static void init_level_2(){
	cantidad =10;
	for (int i = 0; i < 200; i++) //Limpiar datos del arreglo de muros, para dibujar el mapa
		{
		muros[i].x = 0;
		muros[i].y = 0;
		muros[i].w = 0;
		muros[i].h = 0;
		muros[i].tipo = 0;
	}
	
	marco();

	muros[0].x = 360;			//Cargar datos al primer muro, para usar como referencia de coordenadas
	muros[0].y = 86;
	muros[0].w = 30;
	muros[0].h = 30;
	muros[0].tipo = 1;

	int columna=0;
	for (int ms = 1; ms < 60; ms++){ //Muros estaticos (ms). cargar datos en el arreglo del 0 al 29
		
		if(columna < 10){
		muros[ms].x = muros[ms-1].x  + 60;
		muros[ms].y = muros[ms-1].y;
		muros[ms].w = 30;
		muros[ms].h = 30;
		muros[ms].tipo = 1;
		columna=columna+1;
		}
		
		if(columna >= 10){
		muros[ms].x = muros[0].x ;
		muros[ms].y = muros[ms -1].y + 60;
		muros[ms].w = 30;
		muros[ms].h = 30;
		muros[ms].tipo = 1;
		columna = 0;
		}
				
				
	}
/*
	for (int mdx= 60; mdx < 80; mdx++){ //Muros dinamicos (md). Recorrer y cargar datos en el arreglo de muros del 30 al 49
	
			muros[mdx].w = 30;
			muros[mdx].h = 30;
			muros[mdx].tipo = 1;

		int numeroAleatorio_mdx = rand() % 690+ 330; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 370 en x para coincidir con el mapa
		for (int i = 0; i < 60; i++){	

			int errorx = abs(numeroAleatorio_mdx - muros[i].x);
			
			if ( (errorx >= 0) && (errorx < 15)){
				muros[mdx].x = muros[i].x;
				muros[mdx].y = muros[i].y;
				
				
				break;

			}else if ( (errorx >= 15) && (errorx < 30) ){
				muros[mdx].x = muros[i].x+30;
				muros[mdx].y = numeroAleatorio_mdx;
				
				
				break;

			}
			
			if ( (i >= 59) && (errorx >= 30) ){//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
					i = rand() % 60;
					muros[mdx].x = muros[i].x;
					muros[mdx].y = muros[i].y;
				}
		}
			
		int numeroAleatorio_mdy = rand() % 330+ 60; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 100 en y	
		for (int j = 0; j < 60; j++){
			int errory = abs(numeroAleatorio_mdy - muros[j].y);
			
			if ( (errory >= 0) && (errory < 15)){
				muros[mdx].x = muros[mdx].x;
				muros[mdx].y = muros[j].y;
				muros[mdx].w = 30;
				muros[mdx].h = 30;
				muros[mdx].tipo = 1;
				
				break;
			}else if ( (errory >= 15) && (errory < 30)){
				muros[mdx].x = muros[mdx].x;
				muros[mdx].y = muros[j].y + 30;
				muros[mdx].w = 30;
				muros[mdx].h = 30;
				muros[mdx].tipo = 1;
				
				break;
			}

			if ( (j >= 59) && (errory >= 30) ){	//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
					j = rand() % 60;
					muros[mdx].x = muros[j].x;
					muros[mdx].y = muros[j].y;
				}
		}
			
	}
*/
	for (int mdx2= 61; mdx2 < 100; mdx2++){ //Muros dinamicos (md). Recorrer y cargar datos en el arreglo de muros del 30 al 49
	
			muros[mdx2].w = 30;
			muros[mdx2].h = 30;
			muros[mdx2].tipo = 2;

		int numeroAleatorio_mdx2 = rand() % 690 + 330; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 370 en x para coincidir con el mapa
		for (int m = 0; m < 61; m++){	

			int errorx2 = abs(numeroAleatorio_mdx2 - muros[m].x);
			
			if ( (errorx2 >= 0) && (errorx2 < 15)){
				muros[mdx2].x = muros[m].x;
				muros[mdx2].y = muros[m].y;
				 	
				
				break;

			}else if ( (errorx2 >= 15) && (errorx2 < 30) ){
				muros[mdx2].x = muros[m].x+30;
				muros[mdx2].y = muros[mdx2].y;
				
				
				break;

			}
			
			if ( (m >= 60) && (errorx2 >= 30) ){//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
				muros[mdx2].x = 0;
				muros[mdx2].y = 0;
				muros[mdx2].w = 0;
				muros[mdx2].h = 0;
				muros[mdx2].tipo = 0;
			}
		}
			
		int numeroAleatorio_mdy = rand() % 330 + 60; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 100 en y	
		for (int n = 0; n < 61; n++){
			int errory2 = abs(numeroAleatorio_mdy - muros[n].y);
			
			if ( (errory2 >= 0) && (errory2 < 15)){
				muros[mdx2].y = muros[n].y;

					for (int borrar1 = 0; borrar1 < 61; borrar1++){
						if((muros[mdx2].x == muros[borrar1].x) && (muros[mdx2].y == muros[borrar1].y)){
							muros[mdx2].x = 0;
							muros[mdx2].y = 0;
							muros[mdx2].w = 0;
							muros[mdx2].h = 0;
							muros[mdx2].tipo = 0;
						}
					}

				
				break;

			}else if ( (errory2 >= 15) && (errory2 < 30)){
				
				muros[mdx2].y = muros[n].y + 30;

					for (int borrar2 = 0; borrar2 < 61; borrar2++){
						if((muros[mdx2].x == muros[borrar2].x) && (muros[mdx2].y == muros[borrar2].y)){
							muros[mdx2].x = 0;
							muros[mdx2].y = 0;
							muros[mdx2].w = 0;
							muros[mdx2].h = 0;
							muros[mdx2].tipo = 0;
						}
					}
			
				
				break;
			}

			if ( (n >= 60) && (errory2 >= 30) ){	//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
						muros[mdx2].x = 0;
						muros[mdx2].y = 0;
						muros[mdx2].w = 0;
						muros[mdx2].h = 0;
				}
				
		}

		
		
			
	}

	 
}
static void init_level_3(){
	
	cantidad =15;
	for (int i = 0; i < 200; i++) //Limpiar datos del arreglo de muros, para dibujar el mapa
	{
	muros[i].x = 0;
	muros[i].y = 0;
	muros[i].w = 0;
	muros[i].h = 0;
	muros[i].tipo = 0;
	}
	
	marco();

	muros[0].x = 360;			//Cargar datos al primer muro, para usar como referencia de coordenadas
	muros[0].y = 86;
	muros[0].w = 30;
	muros[0].h = 30;
	muros[0].tipo = 1;

	int columna=0;
	for (int ms = 1; ms < 60; ms++){ //Muros estaticos (ms). cargar datos en el arreglo del 0 al 29
		
		if(columna < 10){
		muros[ms].x = muros[ms-1].x  + 60;
		muros[ms].y = muros[ms-1].y;
		muros[ms].w = 30;
		muros[ms].h = 30;
		muros[ms].tipo = 1;
		columna=columna+1;
		}
		
		if(columna >= 10){
		muros[ms].x = muros[0].x ;
		muros[ms].y = muros[ms -1].y + 60;
		muros[ms].w = 30;
		muros[ms].h = 30;
		muros[ms].tipo = 1;
		columna = 0;
		}
				
				
	}
/*
	for (int mdx= 60; mdx < 80; mdx++){ //Muros dinamicos (md). Recorrer y cargar datos en el arreglo de muros del 30 al 49
	
			muros[mdx].w = 30;
			muros[mdx].h = 30;
			muros[mdx].tipo = 1;

		int numeroAleatorio_mdx = rand() % 690+ 330; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 370 en x para coincidir con el mapa
		for (int i = 0; i < 60; i++){	

			int errorx = abs(numeroAleatorio_mdx - muros[i].x);
			
			if ( (errorx >= 0) && (errorx < 15)){
				muros[mdx].x = muros[i].x;
				muros[mdx].y = muros[i].y;
				
				
				break;

			}else if ( (errorx >= 15) && (errorx < 30) ){
				muros[mdx].x = muros[i].x+30;
				muros[mdx].y = numeroAleatorio_mdx;
				
				
				break;

			}
			
			if ( (i >= 59) && (errorx >= 30) ){//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
					i = rand() % 60;
					muros[mdx].x = muros[i].x;
					muros[mdx].y = muros[i].y;
				}
		}
			
		int numeroAleatorio_mdy = rand() % 330+ 60; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 100 en y	
		for (int j = 0; j < 60; j++){
			int errory = abs(numeroAleatorio_mdy - muros[j].y);
			
			if ( (errory >= 0) && (errory < 15)){
				muros[mdx].x = muros[mdx].x;
				muros[mdx].y = muros[j].y;
				muros[mdx].w = 30;
				muros[mdx].h = 30;
				muros[mdx].tipo = 1;
				
				break;
			}else if ( (errory >= 15) && (errory < 30)){
				muros[mdx].x = muros[mdx].x;
				muros[mdx].y = muros[j].y + 30;
				muros[mdx].w = 30;
				muros[mdx].h = 30;
				muros[mdx].tipo = 1;
				
				break;
			}

			if ( (j >= 59) && (errory >= 30) ){	//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
					j = rand() % 60;
					muros[mdx].x = muros[j].x;
					muros[mdx].y = muros[j].y;
				}
		}
			
	}
*/
	for (int mdx2= 61; mdx2 < 100; mdx2++){ //Muros dinamicos (md). Recorrer y cargar datos en el arreglo de muros del 30 al 49
	
			muros[mdx2].w = 30;
			muros[mdx2].h = 30;
			muros[mdx2].tipo = 2;

		int numeroAleatorio_mdx2 = rand() % 690 + 330; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 370 en x para coincidir con el mapa
		for (int m = 0; m < 61; m++){	

			int errorx2 = abs(numeroAleatorio_mdx2 - muros[m].x);
			
			if ( (errorx2 >= 0) && (errorx2 < 15)){
				muros[mdx2].x = muros[m].x;
				muros[mdx2].y = muros[m].y;
				 	
				
				break;

			}else if ( (errorx2 >= 15) && (errorx2 < 30) ){
				muros[mdx2].x = muros[m].x+30;
				muros[mdx2].y = muros[mdx2].y;
				
				
				break;

			}
			
			if ( (m >= 60) && (errorx2 >= 30) ){//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
				muros[mdx2].x = 0;
				muros[mdx2].y = 0;
				muros[mdx2].w = 0;
				muros[mdx2].h = 0;
				muros[mdx2].tipo = 0;
			}
		}
			
		int numeroAleatorio_mdy = rand() % 330 + 60; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 100 en y	
		for (int n = 0; n < 61; n++){
			int errory2 = abs(numeroAleatorio_mdy - muros[n].y);
			
			if ( (errory2 >= 0) && (errory2 < 15)){
				muros[mdx2].y = muros[n].y;

					for (int borrar1 = 0; borrar1 < 61; borrar1++){
						if((muros[mdx2].x == muros[borrar1].x) && (muros[mdx2].y == muros[borrar1].y)){
							muros[mdx2].x = 0;
							muros[mdx2].y = 0;
							muros[mdx2].w = 0;
							muros[mdx2].h = 0;
							muros[mdx2].tipo = 0;
						}
					}

				
				break;

			}else if ( (errory2 >= 15) && (errory2 < 30)){
				
				muros[mdx2].y = muros[n].y + 30;

					for (int borrar2 = 0; borrar2 < 61; borrar2++){
						if((muros[mdx2].x == muros[borrar2].x) && (muros[mdx2].y == muros[borrar2].y)){
							muros[mdx2].x = 0;
							muros[mdx2].y = 0;
							muros[mdx2].w = 0;
							muros[mdx2].h = 0;
							muros[mdx2].tipo = 0;
						}
					}
			
				
				break;
			}

			if ( (n >= 60) && (errory2 >= 30) ){	//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
						muros[mdx2].x = 0;
						muros[mdx2].y = 0;
						muros[mdx2].w = 0;
						muros[mdx2].h = 0;
				}
				
		}

		
		
			
	}

	 
}

/* Function: draw_game_over
 * ----------------------------
 * Currently this function is not being used but it will function
 * when the developed game requires a game over screen.
 *
 * Arguments: 
 *	none
 *
 * Return:
 *
 *	void.
 */
static void draw_game_over() { 

	SDL_Rect p1;
	SDL_Rect dest;

	p1.x = 0;
	p1.y = 0;
	p1.w = end->w;
	p1.h = end->h;

	dest.x = 0;
	dest.y = 0;
	dest.w = end->w;
	dest.h = end->h;
	
	SDL_BlitSurface(end, &dest, screen, &p1);
	
}

static void Winner() { 

	SDL_Rect p1;
	SDL_Rect dest;

	p1.x = 0;
	p1.y = 0;
	p1.w = winn->w;
	p1.h = winn->h;

	dest.x = 0;
	dest.y = 0;
	dest.w = winn->w;
	dest.h = winn->h;
	
	SDL_BlitSurface(winn, &dest, screen, &p1);
	
}

static void Nextlevel() { 

	SDL_Rect p1;
	SDL_Rect dest;

	p1.x = 550;
	p1.y = 200;
	p1.w = 64;
	p1.h = 64;

	dest.x = 0;
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;
	
	SDL_BlitSurface(nivel, &dest, screen, &p1);

	p1.x = 650;
	p1.y = 207;
	p1.w = 64;
	p1.h = 71;

	dest.y = 0;
	dest.w = 64;
	dest.h = 64;

	if (map == 0){ 
	
		dest.x = 64; 

		SDL_BlitSurface(numbermap, &dest, screen, &p1);
	}else if (map == 1){ 
	
		dest.x = 128; 

		SDL_BlitSurface(numbermap, &dest, screen, &p1);
	}else if (map == 2){ 
	
		dest.x = 192;
		 
		SDL_BlitSurface(numbermap, &dest, screen, &p1);
	}
	
}

/* Function: draw_muros
 * -------------------------------
 * This functions draws a game element on the screen according to its
 * properties and coordinates. Please note that the element to be 
 * drawn is passed as an reference insted of a value.
 *
 * Arguments:
 *	element: Element to be drawn on the screen.
 *
 * Return:
 *	void.
 */
 
static void draw_muros( int *level, int *map) {

	
	if (*map == 1){ draw_ground1(); }
	else if (*map == 2){  draw_ground2(); }
	else if (*map == 3){ draw_ground3(); }
			 
}

/* Function: check_collision
 * --------------------------
 * This function verifies if there is collision between two game
 * elements.
 *
 * Arguments:
 * 	a: game element to be checked
 *	b: game element to be checked
 *
 * Return:
 *	TRUE if there is collision 
 * 	FALSE if there isn't collision
 */
int colision_jugador_bomba2(player_t a, bomba_t b) {			//Colision con bombas en estado sin explotar

	int izquierda_a, izquierda_b;
	int derecha_a, derecha_b;
	int arriba_a, arriba_b;
	int abajo_a, abajo_b;

	izquierda_a = a.x;
	derecha_a = a.x + a.w;
	arriba_a = a.y;
	abajo_a = a.y + a.h;

	izquierda_b = b.x;
	derecha_b = b.x + b.w;
	arriba_b = b.y;
	abajo_b = b.y + b.h;
	
	if (izquierda_a > derecha_b) {
		return 0;
	}

	if (derecha_a < izquierda_b) {
		return 0;
	}

	if (arriba_a > abajo_b -5) {
		return 0;
	}

	if (abajo_a < arriba_b) {
		return 0;
	}

	if ((abajo_a > arriba_b) && (derecha_a > izquierda_b) && (izquierda_a < derecha_b) && (arriba_a < abajo_b)){
		return 1;
	}

	
}

/* Function: check_collision
 * --------------------------
 * This function verifies if there is collision between two game
 * elements.
 *
 * Arguments:
 * 	a: game element to be checked
 *	b: game element to be checked
 *
 * Return:
 *	TRUE if there is collision 
 * 	FALSE if there isn't collision
 */
int colision_jugador_bomba(player_t a, bomba_t b) {  			//Colision al explotar la bomba

	int izquierda_a, izquierda_b;
	int derecha_a, derecha_b;
	int arriba_a, arriba_b;
	int abajo_a, abajo_b;

	izquierda_a = a.x;
	derecha_a = a.x + a.w;
	arriba_a = a.y;
	abajo_a = a.y + a.h;

	izquierda_b = b.x;
	derecha_b = b.x + b.w;
	arriba_b = b.y;
	abajo_b = b.y + b.h;
	
	if (izquierda_a > derecha_b - 5) {  //Constantes +5, -5, -10 para permitir una mayor aproximacion
		return 0;
	}

	if (derecha_a < izquierda_b + 5 ) {
		return 0;
	}

	if (arriba_a > abajo_b -10) {
		return 0;
	}

	if (abajo_a < arriba_b + 5) {
		return 0;
	}

	if ((abajo_a > arriba_b) || (derecha_a > izquierda_b) || (izquierda_a < derecha_b) || (arriba_a < abajo_b)){
		return 1;
	}

	
}

int colision_bomba_muro(bomba_t b, game_element_t a, int n) {			

	int izquierda_a, izquierda_b;
	int derecha_a, derecha_b;
	int arriba_a, arriba_b;
	int abajo_a, abajo_b;

	izquierda_a = a.x;
	derecha_a = a.x + a.w;
	arriba_a = a.y;
	abajo_a = a.y + a.h;

	izquierda_b = b.x;
	derecha_b = b.x + b.w;
	arriba_b = b.y;
	abajo_b = b.y + b.h;
	
	if (izquierda_a > derecha_b-2) {
		
		return 0;
	}

	if (derecha_a < izquierda_b +2) {
		
		return 0;
	}

	if (arriba_a > abajo_b-5) {
		return 0;
	}

	if (abajo_a < arriba_b+5) {
		return 0;
	}

	
	if ((abajo_a > arriba_b) || (derecha_a > izquierda_b) || (izquierda_a < derecha_b) || (arriba_a < abajo_b)){
		return 1;
	}

	
}

int orden = 0;
static void colision_bomba( int n ) {	//Accion al detectar colision de bomba

	for( int b = 20; b < 60; b++ ){							//En este rango se encuentran las bombas en explsion
		if(bombas[b].estado == 2 ){
			if(J.estado == 0){
				if(colision_jugador_bomba( J, bombas[b]) != 0) {
					
					J.timer = 100;
					J.estado = 1;
					J.vidas = J.vidas - 1;
					
					if(J.vidas == 0){
						state = GAME_OVER;
					}
					
				}
			}
		}
	}	
	
	if((bombas[n].corte == 0) && (bombas[n].tipohw == 0)){	//Colision bomba-muro horizontal
		int movex, movex1, movew, movew1;
		movex = bombas[n].x;
		movex1 = bombas[n].x + bombas[n].w;
		movew = bombas[n].w;
		movew1 = bombas[n].w;

		for(int m = 0; m < 100; m++){ 	 //Limitar la explosion horizontal hacia la izquierda con los muros
			if ((bombas[n].y == muros[m].y) &&  (orden == 0) && (bombas[n].corte == 0)){
				if(((movex + 1) <= (muros[m].x + muros[m].w)) && (movex >= muros[m].x  )){
					bombas[n].x = muros[m].x + muros[m].w;
					bombas[n].destx = 30;
					bombas[n].w =  bombas[n].w - 30;
					orden =1;
					
					if(muros[m].tipo == 2){		//Eliminar muros rompibles
					muros[m].timer = bombas[n].timer2 + 100;
					muros[m].explotar = 1;
					J.score1= J.score1 +1;
					if(m == 60){ gate = 1;}
					}
				}
			}
		}
		for(int m1 = 0; m1 < 100; m1++){ //Limitar la explosion horizontal hacia la derecha con los muros
			if ((bombas[n].y == muros[m1].y) &&  (bombas[n].corte == 0)){
				
				if(((movex1 - 1) >= muros[m1].x) && (movex1 <= (muros[m1].x + muros[m1].w))  && (bombas[n].corte == 0)){
					bombas[n].w = bombas[n].w - 30;
					bombas[n].corte = 1;
						
					if(muros[m1].tipo == 2){		//Eliminar muros rompibles
						muros[m1].timer = bombas[n].timer2 + 100;
						muros[m1].explotar = 1;
						J.score1= J.score1 +1;
						if(m1 == 60){ gate = 1;}
					}
				}
			}
		}
		bombas[n].corte = 1;
		orden = 0;
		if (movex < (muros[100].x + muros[100].w)){ //Limitar la explosion horizontal con el marco izquierdo
			bombas[n].x = muros[100].x + muros[100].w;
			bombas[n].destx = 30;
			bombas[n].w = bombas[n].w - 30;
			bombas[n].corte = 1;
		}
		if (movex1 > (muros[101].x)){				//Limitar la explosion horizontal con el marco derecho
			bombas[n].w = bombas[n].w - 30;
			
			bombas[n].corte = 1;
		}
	}	
	if((bombas[n].corte == 0) && (bombas[n].tipohw == 1)){	//Colision bomba-muro vertical
		int movey, movey1, moveh, moveh1;
		movey = bombas[n].y;
		movey1 = bombas[n].y + bombas[n].h;
		moveh = bombas[n].h;
		moveh1 = bombas[n].h;

		for(int m = 0; m < 100; m++){ 	 //Limitar la explosion vertical hacia arriba con los muros
			if ((bombas[n].x == muros[m].x) &&  (orden == 0) && (bombas[n].corte == 0)){
				if(((movey + 1) <= (muros[m].y + muros[m].h)) && ((movey + 1) >= muros[m].y  )){
					bombas[n].y = muros[m].y + muros[m].h;
					bombas[n].desty = 30;
					bombas[n].h =  bombas[n].h - 30;

					orden =1;
					
					if(muros[m].tipo == 2){		//Eliminar muros rompibles
						muros[m].timer = bombas[n].timer2 + 100;
						muros[m].explotar = 1;
						J.score1= J.score1 +1;
						if(m == 60){ gate = 1;}
					}
				}
			}
		}
		for(int m1 = 0; m1 < 100; m1++){ //Limitar la explosion horizontal hacia abajo con los muros
			if ((bombas[n].x == muros[m1].x) &&  (bombas[n].corte == 0)){
				
				if(((movey1 - 1) >= muros[m1].y) && (movey1 <= (muros[m1].y + muros[m1].h))  && (bombas[n].corte == 0)){
						bombas[n].h = bombas[n].h - 30;
						bombas[n].corte = 1;
						
					if(muros[m1].tipo == 2){		//Eliminar muros rompibles
						muros[m1].timer = bombas[n].timer2 + 100;
						muros[m1].explotar = 1;
						J.score1= J.score1 +1;
						if(m1 == 60){ gate = 1;}
					}
				}
			}
		}
		bombas[n].corte = 1;
		orden = 0;
		if ((movey + 5) < (muros[102].y + muros[102].h)){ //Limitar la explosion horizontal con el marco de arriba
			bombas[n].y = muros[102].y + muros[102].h;
			bombas[n].desty = 30;
			bombas[n].h = bombas[n].h - 30;
			bombas[n].corte = 1;
		}
		if (movey1 > (muros[103].y)){				//Limitar la explosion horizontal con el marco de abajo
			bombas[n].h = bombas[n].h - 30;
			
			bombas[n].corte = 1;
		}
		
	}
	
	
}

static void explosionH(){

	SDL_Rect src;
	SDL_Rect dest;

	for (int v = 40; v < 60; v++){
		if( bombas[v].estado == 0 ){
			for ( int i = 0; i < 20; i++){
				if (bombas[i].estado == 2 ){
					if (bombas[i].timer2 == 99){
						
						bombas[v].x = bombas[i].x - 30;
						bombas[v].y = bombas[i].y;
						bombas[v].h = bombas[i].h;
						bombas[v].w = bombas[i].w + 60;
						bombas[v].estado = bombas[i].estado;
						bombas[v].timer2 = bombas[i].timer2;
		
						bombas[v].tipohw = 0;

						dest.x= bombas[v].destx;
						dest.y= bombas[v].desty;
						
						bombas[i].estado = 0;
					}
				}
			}
		}
	if(bombas[v].estado == 2 ){

		src.x = bombas[v].x;
		src.y = bombas[v].y;
		src.h = bombas[v].h;
		src.w = bombas[v].w;

		dest.x= bombas[v].destx;
		dest.y= bombas[v].desty;
		dest.h = bombas[v].h;
		dest.w = bombas[v].w;
		
		
		colision_bomba(v);
		

		src.x = bombas[v].x;
		src.y = bombas[v].y;
		src.h = bombas[v].h;
		src.w = bombas[v].w;

		dest.x= bombas[v].destx;
		dest.y= bombas[v].desty;
		dest.h = bombas[v].h;
		dest.w = bombas[v].w;

		SDL_BlitSurface(llamah1, &dest, screen, &src);
	}
		bombas[v].timer2 = bombas[v].timer2 - 1;

		if (bombas[v].timer2 == 0){
			bombas[v].estado = 0;
		}
	
	}

}

static void explosionV(){

	SDL_Rect src;
	SDL_Rect dest;

	for (int v = 20; v < 40; v++){				//Cargar datos de bombas que estan explotando
		if( bombas[v].estado == 0 ){
			for ( int i = 0; i < 20; i++){
				if (bombas[i].estado == 2){
					
					if (bombas[i].timer2 == 100){
						bombas[v].x = bombas[i].x;
						bombas[v].y = bombas[i].y - 30;
						bombas[v].h = bombas[i].h + 60;
						bombas[v].w = bombas[i].w;
						bombas[v].estado = bombas[i].estado;
						bombas[v].timer2 = bombas[i].timer2;
						
						bombas[v].tipohw = 1;
						
						dest.x= bombas[v].destx;
						dest.y= bombas[v].desty;

						bombas[i].timer2 = 99;
					}
				}
			}
		}
		if(bombas[v].estado == 2 ){
			src.x = bombas[v].x;
			src.y = bombas[v].y;
			src.h = bombas[v].h;
			src.w = bombas[v].w;

			dest.x= bombas[v].destx;
			dest.y= bombas[v].desty;
			dest.h = bombas[v].h;
			dest.w = bombas[v].w;
			
			
			colision_bomba(v);
			

			src.x = bombas[v].x;
			src.y = bombas[v].y;
			src.h = bombas[v].h;
			src.w = bombas[v].w;

			dest.x= bombas[v].destx;
			dest.y= bombas[v].desty;
			dest.h = bombas[v].h;
			dest.w = bombas[v].w;

			SDL_BlitSurface(llamav1, &dest, screen, &src);
		}
		bombas[v].timer2 = bombas[v].timer2 - 1;

		if (bombas[v].timer2 == 0){
			bombas[v].estado = 0;
		}
			
		
	}
	
}

static void draw_bomba(int bombaE, int *bombaN, int *EN ){
	SDL_Rect src;
	SDL_Rect dest;

	int numMaxB = 10;

	if( bombaE == 1 && bombas[*bombaN].timer1 == 160 && *bombaN < numMaxB){//fijar posicion de las bombas
		
		bombas[*bombaN].x = J.x;
		bombas[*bombaN].y = J.y;
		bombas[*bombaN].h = 30;
		bombas[*bombaN].w = 30;
		bombas[*bombaN].estado = 1;
		bombas[*bombaN].colision = 0;
		
	}	

	for (int i = 0; i < 60; i++){
		
		if(*bombaN != i){				//Evitar que ponga una bomba sobre otra
			if ((bombas[*bombaN].x == bombas[i].x) && (bombas[*bombaN].y == bombas[i].y))
			{
				bombas[*bombaN].estado = 0;
			}
		}
			
		
		
		if(bombas[i].estado == 0){		//Resetear las bombas despues de explotar

			bombas[i].x = 0;
			bombas[i].y = 0;
			bombas[i].h = 0;
			bombas[i].w = 0;
			bombas[i].timer1 = 0;
			bombas[i].timer2 = 0;
			bombas[i].explosionw1 = 0;
			bombas[i].explosionw0 = 0;
			bombas[i].explosionh1 = 0;
			bombas[i].explosionh0 = 0;
			bombas[i].explosion = 0;
			bombas[i].colision = 0;
			bombas[i].corte = 0;
			bombas[i].destx = 0;
			bombas[i].desty = 0;

			src.x = 0;
			src.y = 0;
			src.w = 0;
			src.h = 0;

			dest.y = 0;
			dest.x = 0;
			dest.w = 0;
			dest.h = 0;

		}

		if(bombas[i].estado == 1){		//Animacion de bombas en estado 1 y transicion a estado 2

			src.x = bombas[i].x;
			src.y = bombas[i].y;
			src.w = bombas[i].w;
			src.h = bombas[i].h;

			dest.y = 0;
			dest.w = bombas[i].w;
			dest.h = bombas[i].h;
		
	
			if (bombas[i].timer1 > 120)
			{
				dest.x = 0;
				bombas[i].timer1= bombas[i].timer1 - 1;
			}
			else if (bombas[i].timer1 > 80 && bombas[i].timer1 < 121)
			{
				dest.x = 30;
				bombas[i].timer1= bombas[i].timer1 - 1;
			}
			else if (bombas[i].timer1 > 40 && bombas[i].timer1 < 81)
			{
				dest.x = 0;
				bombas[i].timer1= bombas[i].timer1 - 1;
			}
			else if (bombas[i].timer1 > 0 && bombas[i].timer1 < 41)
			{
				dest.x = 30;
				bombas[i].timer1= bombas[i].timer1 - 1;

					if ((bombas[i].timer1 == 0)){
						dest.h = 0;
						dest.w = 0;
						bombas[i].estado = 2;
					
						bombas[i].timer2 = 100;
					}
			
			}
			for (int m = 0; m < 60; m++){			//Centrar las bombas
				int absx = abs(muros[m].x - bombas[i].x );
				int absy = abs(muros[m].y - bombas[i].y );
				int distanciax = muros[m].x - bombas[i].x;
				int distanciay = muros[m].y - bombas[i].y;

				if (muros[m].tipo == 1){
					if (bombas[i].x < muros[0].x - 30){bombas[i].x = muros[0].x - 30;}
					if (bombas[i].y < muros[0].y - 30 ){bombas[i].y = muros[0].y - 30;}
					if (bombas[i].x > ((muros[0].x)+(muros[0].w*19))){bombas[i].x = ((muros[0].x)+(muros[0].w*19));} //El valor multiplicando define el ancho y largo 
					if (bombas[i].y > ((muros[0].y)+(muros[0].w*11))){bombas[i].y = ((muros[0].y)+(muros[0].w*11));} // del area donde se pondran las bombas
					if (( absx <= 30) && ( absy <= 30 ) && (bombas[i].timer1 == 159))
					{
						if(distanciax < 0){ 
							if( absx < 20){bombas[i].x= muros[m].x;}
							else if( 20<= absx <= 30){bombas[i].x = muros[m].x +30;}
							
							if(distanciay < 0){
								if( absy <15){bombas[i].y = muros[m].y;}
								else if( 15<= absy <= 30){bombas[i].y = muros[m].y +30;}
								break;
							}
							else if(distanciay >= 0){
								if( absy <20){bombas[i].y= muros[m].y;}
								else if( 20 <= absy <= 30){bombas[i].y = muros[m].y -30;}
								break;
							}
							
						}
						else if(distanciax > 0){
							if( absx <10){bombas[i].x= muros[m].x;}
							else if( 10 <= absx <= 30){bombas[i].x = muros[m].x -30;}

							if(distanciay < 0){
								if( absy < 15){bombas[i].y = muros[m].y;}
								else if( 15 <= absy <= 30){bombas[i].y = muros[m].y +30;}
									break;
							}
							else if(distanciay >= 0){
								if( absy < 15){bombas[i].y= muros[m].y;}
								else if( 15 <= absy <= 30){bombas[i].y = muros[m].y -30;}
								break;
							}
						
						}
						break;
						
					}
				}
				

			}
			src.x = bombas[i].x;
			src.y = bombas[i].y;
			SDL_BlitSurface(bomba, &dest, screen, &src);
		}
		
			
	}

	if (*EN > 0 ){						//temporizador para poner bombas 
				*EN = *EN - 1;
				}
}
 
int colision_enemigo_muro(enemigo_t a, game_element_t b) {	//Se verifica en mover_enemigo

	int izquierda_a, izquierda_b;
	int derecha_a, derecha_b;
	int arriba_a, arriba_b;
	int abajo_a, abajo_b;

	izquierda_a = a.x;
	derecha_a = a.x + a.w;
	arriba_a = a.y;
	abajo_a = a.y + a.h;

	izquierda_b = b.x;
	derecha_b = b.x + b.w;
	arriba_b = b.y;
	abajo_b = b.y + b.h;
	
	if (izquierda_a >= derecha_b) {
		return 0;
	}

	if (derecha_a <= izquierda_b) {
		return 0;
	}

	if (arriba_a+5 >= abajo_b ) {
		return 0;
	}

	if (abajo_a-2 <= arriba_b) {
		return 0;
	}

	if ((abajo_a > arriba_b) || (derecha_a > izquierda_b) || (izquierda_a < derecha_b) || (arriba_a < abajo_b)){
		return 1;
	}

	
}

int colision_enemigo_bomba(enemigo_t a, bomba_t b) {	//Se verifica en mover_enemigo

	int izquierda_a, izquierda_b;
	int derecha_a, derecha_b;
	int arriba_a, arriba_b;
	int abajo_a, abajo_b;

	izquierda_a = a.x;
	derecha_a = a.x + a.w;
	arriba_a = a.y;
	abajo_a = a.y + a.h;

	izquierda_b = b.x;
	derecha_b = b.x + b.w;
	arriba_b = b.y;
	abajo_b = b.y + b.h;
	
	if (izquierda_a >= derecha_b) {
		return 0;
	}

	if (derecha_a <= izquierda_b) {
		return 0;
	}

	if (arriba_a >= abajo_b ) {
		return 0;
	}

	if (abajo_a <= arriba_b) {
		return 0;
	}

	if ((abajo_a > arriba_b) || (derecha_a > izquierda_b) || (izquierda_a < derecha_b) || (arriba_a < abajo_b)){
		return 1;
	}

	
}

int d =0;
int colision_jugador_enemigo(player_t a, enemigo_t b);
static void mover_enemigo(struct enemigo_t *enemigos, int cantidad) {
	
	for(int e = 0; e < cantidad; e++){
		if(enemigos[e].estado == 0 ){
			int velocidad = 1;	
			if(enemigos[e].colision == 1){
				d = rand() %4;

				enemigos[e].dir = d;
				
				enemigos[e].colision = 0; 
				checkdir = 1;
			}

			enemigos[e].timer = enemigos[e].timer - 1;
			if(enemigos[e].timer == 0){		//timer para cambiar de direccion sin tener que colisionar
				enemigos[e].colision = 1;
				d = rand() %200;
				enemigos[e].timer = d;
			}

			//Arriba
			if (enemigos[e].dir == 0) {
				enemigos[e].y -= velocidad;
				
				for (int i = 0; i < 104; i++)
				{
					if(colision_enemigo_muro( enemigos[e], muros[i]) != 0) {
					enemigos[e].y =  muros[i].y + ( muros[i].h-5);
					enemigos[e].colision = 1;
					}
				}
				
				for (int b = 0; b < 60; b++)
				{
					if(bombas[b].estado == 1 ){
						if(colision_enemigo_bomba( enemigos[e], bombas[b]) != 0){ 
							enemigos[e].y =  bombas[b].y + ( bombas[b].h+1);	
						}							
					}
					else if(bombas[b].estado == 2){
						if(colision_enemigo_bomba( enemigos[e], bombas[b]) != 0) {
						
							enemigos[e].timer2 = 100;
							enemigos[e].estado = 1;
							J.score1 = J.score1 + 1;
							
								
						}
					}
				}
				
			}
			
			// A moverse a la izquierda
			if (enemigos[e].dir == 1) {
				enemigos[e].x -= velocidad;
				
				for (int i = 0; i < 104; i++)
				{
					if(colision_enemigo_muro( enemigos[e], muros[i]) != 0) {
					enemigos[e].x =  muros[i].x + ( muros[i].w+1);
					enemigos[e].colision = 1;
					}
				}
				for (int b = 0; b < 60; b++){
					if(bombas[b].estado == 1 ){
						if(colision_enemigo_bomba( enemigos[e], bombas[b]) != 0){ 
						enemigos[e].x =  bombas[b].x + ( bombas[b].w+1);	
						}							
					}
					else if(bombas[b].estado == 2){
						if(colision_enemigo_bomba( enemigos[e], bombas[b]) != 0) {
				
						enemigos[e].timer2 = 100;
						enemigos[e].estado = 1;
						J.score1 = J.score1 + 1;
						}
					}
				}

			}
				
			// D moverse a la derecha
			if (enemigos[e].dir == 2) {
				enemigos[e].x += velocidad;
				
				for (int i = 0; i < 104; i++)
					{
						if(colision_enemigo_muro( enemigos[e], muros[i]) != 0) {
						enemigos[e].x =  muros[i].x - ( enemigos[e].w+1);
						enemigos[e].colision = 1;
						}
					}
				for (int b = 0; b < 60; b++){
					if(bombas[b].estado == 1 ){
						if(colision_enemigo_bomba( enemigos[e], bombas[b]) != 0){ 
						enemigos[e].x =  bombas[b].x - ( enemigos[e].w+1);	
						}							
					}
					else if(bombas[b].estado == 2){
						if(colision_enemigo_bomba( enemigos[e], bombas[b]) != 0) {
				
						enemigos[e].timer2 = 100;
						enemigos[e].estado = 1;
						J.score1 = J.score1 + 1;
						}
					}
				}

			}	
			
			// S mmoverse hacia abajo
			if (enemigos[e].dir == 3) {
				enemigos[e].y += velocidad;

				for (int i = 0; i < 104; i++)
				{
					if(colision_enemigo_muro( enemigos[e], muros[i]) != 0) {
					enemigos[e].y =  muros[i].y - ( enemigos[e].h+1);
					enemigos[e].colision = 1;
					}
				}
				for (int b = 0; b < 60; b++){
					if(bombas[b].estado == 1 ){
						if(colision_enemigo_bomba( enemigos[e], bombas[b]) != 0){ 
						enemigos[e].y =  bombas[b].y - ( bombas[b].h-1);	
						}							
					}
					else if(bombas[b].estado == 2){
						if(colision_enemigo_bomba( enemigos[e], bombas[b]) != 0) {
				
						enemigos[e].timer2 = 100;
						enemigos[e].estado = 1;
						J.score1 = J.score1 + 1;
						}
					}
				}
			}
		
			if(J.estado == 0){
				if(colision_jugador_enemigo( J, enemigos[e]) != 0) {
					
					J.timer = 100;
					J.estado = 1;
					J.vidas = J.vidas - 1;
					
					if(J.vidas == 0){
						state = GAME_OVER;
					}
					
				}
			}
		}
	}		
	
	
}

int check=0;
static void init_enemy(struct enemigo_t *enemigos, int cantidad) {

    for (int i = 0; i < cantidad; i++) {
 
		 
		if (check ==1){
			i = i - 1;
			check = 0;
		}
		
		enemigos[i].colision = 0; 
		enemigos[i].timer = 10;
		enemigos[i].timer2 = 0;
		enemigos[i].estado = 0;
		//enemigos[i].tipo = 1;
		enemigos[i].dir = 1;
		enemigos[i].predir = 0;
		
		 
 
		int numeroAleatorio_mdx2 = rand() % 690 + 300; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 370 en x para coincidir con el mapa
		for (int m = 0; m < 100; m++){	
			if(muros[m].tipo != 0){
				int errorx2 = abs(numeroAleatorio_mdx2 - muros[m].x);
			
				if ( (errorx2 >= 0) && (errorx2 < 15)){
					enemigos[i].x = muros[m].x;
					enemigos[i].y = muros[m].y;
					
					break;

				}else if ( (errorx2 >= 15) && (errorx2 < 30) ){
					enemigos[i].x = muros[m].x+30;
					enemigos[i].y = enemigos[i].y;
					
					break;

				}
				
				if ( (m >= 99) && (errorx2 >= 30) ){//En caso de que se realicen todas las comparaciones 
													//y no se cumpla ninguna condicion de distacia, se genera random
					enemigos[i].x = 0;
					enemigos[i].y = 0;
					enemigos[i].w = 0;
					enemigos[i].h = 0;
					enemigos[i].tipo = 0;
					enemigos[i].colision = 0;
					enemigos[i].dir = 0;
					enemigos[i].timer = 0;
					enemigos[i].timer2 = 0;
					enemigos[i].estado = 0;
					enemigos[i].predir = 0;

					check=1;
				}
			}
		}
			
		int numeroAleatorio_mdy = rand() % 330 + 60; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 100 en y	
		for (int n = 0; n < 100; n++){
			if(muros[n].tipo != 0){
				int errory2 = abs(numeroAleatorio_mdy - muros[n].y);
				
				if ( (errory2 >= 0) && (errory2 < 15)){
					enemigos[i].y = muros[n].y;

					break;

				}else if ( (errory2 >= 15) && (errory2 < 30)){
					
					enemigos[i].y = muros[n].y + 30;

					break;
				}
				if ( (n >= 99) && (errory2 >= 30) ){	//En caso de que se realicen todas las comparaciones 
													//y no se cumpla ninguna condicion de distacia, se genera random
					enemigos[i].x = 0;
					enemigos[i].y = 0;
					enemigos[i].w = 0;
					enemigos[i].h = 0;
					enemigos[i].tipo = 0;
					enemigos[i].colision = 0;
					enemigos[i].dir = 0;
					enemigos[i].timer = 0;
					enemigos[i].timer2 = 0;
					enemigos[i].estado = 0;
					enemigos[i].predir = 0;

					check=1;
				}
				for (int borrar2 = 0; borrar2 < 100; borrar2++){
							if((enemigos[i].x == muros[borrar2].x) && (enemigos[i].y == muros[borrar2].y)){
								enemigos[i].x = 0;
								enemigos[i].y = 0;
								enemigos[i].w = 0;
								enemigos[i].h = 0;
								enemigos[i].tipo = 0;
								enemigos[i].colision = 0;
								enemigos[i].dir = 0;
								enemigos[i].timer = 0;
								enemigos[i].timer2 = 0;
								enemigos[i].estado = 0;
								enemigos[i].predir = 0;
						
								check=1;

							}
				}
				if((enemigos[i].x < (muros[100].x + muros[100].w)) || ((enemigos[i].x + enemigos[i].w) > (muros[101].x )) || (enemigos[i].y < (muros[102].y + muros[102].y)) || ((enemigos[i].x + enemigos[i].w) > (muros[101].x ))) {
							enemigos[i].x = 0;
								enemigos[i].y = 0;
								enemigos[i].w = 0;
								enemigos[i].h = 0;
								enemigos[i].tipo = 0;
								enemigos[i].colision = 0;
								enemigos[i].dir = 0;
								enemigos[i].timer = 0;
								enemigos[i].timer2 = 0;
								enemigos[i].estado = 0;
								enemigos[i].predir = 0;
					
					check=1;
				}
			}	
		}

		if (check == 0){ 
			if (map == 1){ 
				if (numEn1 == 0){ enemigos[i].tipo = 1; numEn1 = 1;} 	//1 + rand() % 2;  Asignar tipo de enemigo aleatoriamente a los elementos del arreglo 
				else if (numEn1 == 1){ enemigos[i].tipo = 2; numEn1 = 0;} 
			}else if (map == 2){ 
				if (numEn1 == 0){ enemigos[i].tipo = 2; numEn1 = 1;} 
				else if (numEn1 == 1){ enemigos[i].tipo = 3; numEn1 = 2;} 
				else if (numEn1 == 2){ enemigos[i].tipo = 4; numEn1 = 0;} 
			}else if (map == 3){ 
				if (numEn1 == 0){ enemigos[i].tipo = 3; numEn1 = 1;} 
				else if (numEn1 == 1){ enemigos[i].tipo = 4; numEn1 = 2;} 
				else if (numEn1 == 2){ enemigos[i].tipo = 5; numEn1 = 0;} 
			}
		

			if(enemigos[i].tipo == 1){
				enemigos[i].h = 30;
				enemigos[i].w = 22; 
			}else if(enemigos[i].tipo == 2){
				enemigos[i].h = 28;
				enemigos[i].w = 28; 
			}else if(enemigos[i].tipo == 3){
				enemigos[i].h = 28;
				enemigos[i].w = 28; 
			}else if(enemigos[i].tipo == 4){
				enemigos[i].h = 30;
				enemigos[i].w = 28; 
			}else if(enemigos[i].tipo == 5){
				enemigos[i].h = 30;
				enemigos[i].w = 27; 
			} 
		}

		 
    }
	check = 0;
	numEn1 = 0;
}

/*
static void init_enemy() { //init enemy sin malloc

    for (int i = 0; i < 7; i++) {
		int tipo = rand() % 3;  //Asignar tipo de enemigo aleatoriamente a los elementos del arreglo 
		enemigos[i].h = 30;
		enemigos[i].w = 22;
		enemigos[i].tipo = 1;

		int numeroAleatorio_mdx2 = rand() % 690 + 330; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 370 en x para coincidir con el mapa
		for (int m = 0; m < 80; m++){	

			int errorx2 = abs(numeroAleatorio_mdx2 - muros[m].x);
		
			if ( (errorx2 >= 0) && (errorx2 < 15)){
				enemigos[i].x = muros[m].x;
				enemigos[i].y = muros[m].y;
				
				break;

			}else if ( (errorx2 >= 15) && (errorx2 < 30) ){
				enemigos[i].x = muros[m].x+30;
				enemigos[i].y = enemigos[i].y;
				
				break;

			}
			
			if ( (m >= 79) && (errorx2 >= 30) ){//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
				enemigos[i].x = 0;
				enemigos[i].y = 0;
				enemigos[i].w = 0;
				enemigos[i].h = 0;
				enemigos[i].tipo = 0;
			}
		}
			
			int numeroAleatorio_mdy = rand() % 330 + 60; //numeros random en un rango igual al ancho de el mapa
													//Luego se dezplazan 100 en y	
		for (int n = 0; n < 80; n++){
			int errory2 = abs(numeroAleatorio_mdy - muros[n].y);
			
			if ( (errory2 >= 0) && (errory2 < 15)){
				enemigos[i].y = muros[n].y;

				for (int borrar1 = 0; borrar1 < 80; borrar1++){
					if((enemigos[i].x == muros[borrar1].x) && (enemigos[i].y == muros[borrar1].y)){
						enemigos[i].x = 0;
						enemigos[i].y = 0;
						enemigos[i].w = 0;
						enemigos[i].h = 0;
						enemigos[i].tipo = 0;
					}
				}
				break;

			}else if ( (errory2 >= 15) && (errory2 < 30)){
				
				enemigos[i].y = muros[n].y + 30;

					for (int borrar2 = 0; borrar2 < 80; borrar2++){
						if((enemigos[i].x == muros[borrar2].x) && (enemigos[i].y == muros[borrar2].y)){
							enemigos[i].x = 0;
							enemigos[i].y = 0;
							enemigos[i].w = 0;
							enemigos[i].h = 0;
							enemigos[i].tipo = 0;
						}
					}
			
				
				break;
			}
			if ( (n >= 79) && (errory2 >= 30) ){	//En caso de que se realicen todas las comparaciones 
												//y no se cumpla ninguna condicion de distacia, se genera random
				enemigos[i].x = 0;
				enemigos[i].y = 0;
				enemigos[i].w = 0;
				enemigos[i].h = 0;
				enemigos[i].tipo = 0;

			}
			
		}

		

    }

}
*/
static void draw_enemy(struct enemigo_t *enemigos, int cantidad) { 

	SDL_Rect src;
	SDL_Rect dest;

	

	for (int i = 0; i < cantidad; i++){
 
		src.x = enemigos[i].x;
		src.y = enemigos[i].y;
		src.w = enemigos[i].w;
		src.h = enemigos[i].h;

		dest.w = enemigos[i].w ;
		dest.h = enemigos[i].h; 

		if((enemigos[i].x < (muros[100].x + muros[100].w)) || ((enemigos[i].x + 21) > (muros[101].x )) || ((enemigos[i].y)+15 < (muros[102].y + muros[102].h)) || ((enemigos[i].y + 25) > (muros[103].y ))) {
			enemigos[i].x = 0;
			enemigos[i].y = 0;
			enemigos[i].w = 0;
			enemigos[i].h = 0;
			enemigos[i].tipo = 0;
			enemigos[i].colision = 0;
			enemigos[i].dir = 0;
			enemigos[i].predir = 0; 
			enemigos[i].timer = 0;
			enemigos[i].timer2 = 0;
			enemigos[i].estado = 0; 
			
		}
 
		dest.y = 0; 
		if(enemigos[i].tipo == 0){
			enemigos[i].x = 0;
			enemigos[i].y = 0;
			enemigos[i].w = 0;
			enemigos[i].h = 0;
			enemigos[i].tipo = 0;
			enemigos[i].colision = 0;
			enemigos[i].dir = 0;
			enemigos[i].predir = 0; 
			enemigos[i].timer = 0;
			enemigos[i].timer2 = 0;
			enemigos[i].estado = 0; 

			
		}else if(enemigos[i].tipo != 0){
			/*	
				for (int borrar2 = 0; borrar2 < 100; borrar2++){
					if((enemigos[i].x == muros[borrar2].x) && (enemigos[i].y == muros[borrar2].y)){
						enemigos[i].x = 0;
						enemigos[i].y = 0;
						enemigos[i].w = 0;
						enemigos[i].h = 0;
						enemigos[i].tipo = 0;
						enemigos[i].colision = 0;
						enemigos[i].dir = 0;
						enemigos[i].predir = 0; 
						enemigos[i].timer = 0;
						enemigos[i].timer2 = 0;
						enemigos[i].estado = 0; 
					}
				} 
			*/

			if(map == 1){
				
				if (enemigos[i].tipo == 1){ 

					if(enemigos[i].estado == 0){
						
						if (count_E == 0)
						{
							dest.x = 0;
						}	
						if (destx_E <= 0){ bumeran_E = 0;}  //0 -> posicion inicial en x de primera imagen del arreglo
						if (destx_E >= 66){ bumeran_E = 1;} //66 -> posicion inicial en x de la ultima imagen del arreglo
						if (count_E >= 20)
						{ 
							if(bumeran_E ==0){destx_E = destx_E + 22;}
							if(bumeran_E ==1){destx_E = destx_E - 22;}
							
							count_E = 0;
						}
						count_E=count_E +1;
						
						dest.x = destx_E;

					SDL_BlitSurface(enemigo1, &dest, screen, &src);
					}else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 > 0)){
				
						enemigos[i].timer2 = enemigos[i].timer2 - 1;

							dest.x = 0;
						SDL_BlitSurface(enemigomuerto, &dest, screen, &src);
					
					} else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 == 0)){
						enemigos[i].tipo = 0;
					}
					
				}else if (enemigos[i].tipo == 2){ 
					if(enemigos[i].estado == 0){
					if (count_E3 == 0)
					{
						dest.x = 0;
					}	
					if (destx_E3 <= 0){ bumeran_E3 = 0;}  //0 -> posicion inicial en x de primera imagen del arreglo
					if (destx_E3 >= 28){ bumeran_E3 = 1;} //66 -> posicion inicial en x de la ultima imagen del arreglo
					if (count_E3 >=30)
					{ 
						if(bumeran_E3 ==0){destx_E3 = destx_E3 + 28;}
						if(bumeran_E3 ==1){destx_E3 = destx_E3 - 28;}
						
						count_E3 = 0;
					}
					count_E3=count_E3 +1;
					
					dest.x = destx_E3;

					SDL_BlitSurface(enemigo2, &dest, screen, &src);

					}else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 > 0)){
				
						enemigos[i].timer2 = enemigos[i].timer2 - 1;

							dest.x = 56;
						SDL_BlitSurface(enemigo2, &dest, screen, &src);
					
					} else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 == 0)){
						enemigos[i].tipo = 0;
					}
					
				}
			}
			else if(map == 2){

				if (enemigos[i].tipo == 2){ 

					if(enemigos[i].estado == 0){
					if (count_E3 == 0)
					{
						dest.x = 0;
					}	
					if (destx_E3 <= 0){ bumeran_E3 = 0;}  //0 -> posicion inicial en x de primera imagen del arreglo
					if (destx_E3 >= 28){ bumeran_E3 = 1;} //66 -> posicion inicial en x de la ultima imagen del arreglo
					if (count_E3 >=30)
					{ 
						if(bumeran_E3 ==0){destx_E3 = destx_E3 + 28;}
						if(bumeran_E3 ==1){destx_E3 = destx_E3 - 28;}
						
						count_E3 = 0;
					}
					count_E3=count_E3 +1;
					
					dest.x = destx_E3;

					SDL_BlitSurface(enemigo2, &dest, screen, &src);

					}else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 > 0)){
				
						enemigos[i].timer2 = enemigos[i].timer2 - 1;

							dest.x = 56;
						SDL_BlitSurface(enemigo2, &dest, screen, &src);
					
					} else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 == 0)){
						enemigos[i].tipo = 0;
					}
					
				}else if (enemigos[i].tipo == 3){ 

					if(enemigos[i].estado == 0){
					if (enemigos[i].dir == 0){
						
						dest.x = 84;
						SDL_BlitSurface(enemigo3, &dest, screen, &src);

					}else if(enemigos[i].dir == 1){
				
							dest.x = 28;
						SDL_BlitSurface(enemigo3, &dest, screen, &src);
					
					}else if(enemigos[i].dir == 2){
				
							dest.x = 56;
						SDL_BlitSurface(enemigo3, &dest, screen, &src);
					
					}else if(enemigos[i].dir == 3){
				
							dest.x = 0;
						SDL_BlitSurface(enemigo3, &dest, screen, &src);
					
					}
					}else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 > 0)){
				
						enemigos[i].timer2 = enemigos[i].timer2 - 1;

							dest.x = 112;
						SDL_BlitSurface(enemigo3, &dest, screen, &src);
					
					} else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 == 0)){
						enemigos[i].tipo = 0;
					}
				
				}else if (enemigos[i].tipo == 4){ 

					if(enemigos[i].estado == 0){
						if (enemigos[i].dir == 0){
							
							if ((count_E4 >=15) || (checkdir = 1))
							{ 
								if(bumeran_E4 ==0){destx_E4 = 168; bumeran_E4 = 1;}
								else if(bumeran_E4 ==1){destx_E4= 196; bumeran_E4 = 0;}
								
								count_E4 = 0;
								checkdir = 0;
							}
							count_E4 = count_E4 +1;
							dest.x = destx_E4; 
								
							SDL_BlitSurface(enemigo4, &dest, screen, &src);

						}else if(enemigos[i].dir == 1){
					
								if ((count_E4 >=15) || (checkdir = 1))
							{ 
								if(bumeran_E4 ==0){destx_E4 = 56; bumeran_E4 = 1;}
								else if(bumeran_E4 ==1){destx_E4= 84; bumeran_E4 = 0;}
								
								count_E4 = 0;
								checkdir = 0;
							}
							count_E4 = count_E4 +1;
							dest.x = destx_E4;

							SDL_BlitSurface(enemigo4, &dest, screen, &src);
						
						}else if(enemigos[i].dir == 2){
					
								if ((count_E4 >=15) || (checkdir = 1))
							{ 
								if(bumeran_E4 ==0){destx_E4 = 112; bumeran_E4 = 1;}
								else if(bumeran_E4 ==1){destx_E4= 140; bumeran_E4 = 0;}
								
								count_E4 = 0;
								checkdir = 0;
							}
							count_E4 = count_E4 +1;
							dest.x = destx_E4;

							SDL_BlitSurface(enemigo4, &dest, screen, &src);
						
						}else if(enemigos[i].dir == 3){
					
								if ((count_E4 >=15) || (checkdir = 1))
							{ 
								if(bumeran_E4 ==0){destx_E4 = 0; bumeran_E4 = 1;}
								else if(bumeran_E4 ==1){destx_E4= 28; bumeran_E4 = 0;}
								
								count_E4 = 0;
								checkdir = 0;
							}
							count_E4 = count_E4 +1;
							dest.x = destx_E4;

							SDL_BlitSurface(enemigo4, &dest, screen, &src);
						
						}
					}else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 > 0)){
				
						enemigos[i].timer2 = enemigos[i].timer2 - 1;

							dest.x = 224;
						SDL_BlitSurface(enemigo4, &dest, screen, &src);
					
					} else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 == 0)){
						enemigos[i].tipo = 0;
					}
				
				}
			}
			else if(map == 3){
				
				if (enemigos[i].tipo == 3){ 

					if(enemigos[i].estado == 0){
					if (enemigos[i].dir == 0){
						
						dest.x = 84;
						SDL_BlitSurface(enemigo3, &dest, screen, &src);

					}else if(enemigos[i].dir == 1){
				
							dest.x = 28;
						SDL_BlitSurface(enemigo3, &dest, screen, &src);
					
					}else if(enemigos[i].dir == 2){
				
							dest.x = 56;
						SDL_BlitSurface(enemigo3, &dest, screen, &src);
					
					}else if(enemigos[i].dir == 3){
				
							dest.x = 0;
						SDL_BlitSurface(enemigo3, &dest, screen, &src);
					
					}
					}else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 > 0)){
				
						enemigos[i].timer2 = enemigos[i].timer2 - 1;

							dest.x = 112;
						SDL_BlitSurface(enemigo3, &dest, screen, &src);
					
					} else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 == 0)){
						enemigos[i].tipo = 0;
					}
				
				}else if (enemigos[i].tipo == 4){ 
					if(enemigos[i].estado == 0){
						if (enemigos[i].dir == 0){
							
							if ((count_E4 >=15) || (checkdir = 1))
							{ 
								if(bumeran_E4 ==0){destx_E4 = 168; bumeran_E4 = 1;}
								else if(bumeran_E4 ==1){destx_E4= 196; bumeran_E4 = 0;}
								
								count_E4 = 0;
								checkdir = 0;
							}
							count_E4 = count_E4 +1;
							dest.x = destx_E4; 
								
							SDL_BlitSurface(enemigo4, &dest, screen, &src);

						}else if(enemigos[i].dir == 1){
					
								if ((count_E4 >=15) || (checkdir = 1))
							{ 
								if(bumeran_E4 ==0){destx_E4 = 56; bumeran_E4 = 1;}
								else if(bumeran_E4 ==1){destx_E4= 84; bumeran_E4 = 0;}
								
								count_E4 = 0;
								checkdir = 0;
							}
							count_E4 = count_E4 +1;
							dest.x = destx_E4;

							SDL_BlitSurface(enemigo4, &dest, screen, &src);
						
						}else if(enemigos[i].dir == 2){
					
								if ((count_E4 >=15) || (checkdir = 1))
							{ 
								if(bumeran_E4 ==0){destx_E4 = 112; bumeran_E4 = 1;}
								else if(bumeran_E4 ==1){destx_E4= 140; bumeran_E4 = 0;}
								
								count_E4 = 0;
								checkdir = 0;
							}
							count_E4 = count_E4 +1;
							dest.x = destx_E4;

							SDL_BlitSurface(enemigo4, &dest, screen, &src);
						
						}else if(enemigos[i].dir == 3){
					
								if ((count_E4 >=15) || (checkdir = 1))
							{ 
								if(bumeran_E4 ==0){destx_E4 = 0; bumeran_E4 = 1;}
								else if(bumeran_E4 ==1){destx_E4= 28; bumeran_E4 = 0;}
								
								count_E4 = 0;
								checkdir = 0;
							}
							count_E4 = count_E4 +1;
							dest.x = destx_E4;

							SDL_BlitSurface(enemigo4, &dest, screen, &src);
						
						}
					}else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 > 0)){
				
						enemigos[i].timer2 = enemigos[i].timer2 - 1;

							dest.x = 224;
						SDL_BlitSurface(enemigo4, &dest, screen, &src);
					
					} else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 == 0)){
						enemigos[i].tipo = 0;
					}
				
				}else if (enemigos[i].tipo == 5){ 

				if(enemigos[i].estado == 0){
					if (enemigos[i].dir == 0){
						
						if ((count_E5 >=15) || (checkdir = 1))
						{ 
							if(bumeran_E5 ==0){destx_E5 = 162; bumeran_E5 = 1;}
							else if(bumeran_E5 ==1){destx_E5= 189; bumeran_E5 = 0;}
							
							count_E5 = 0;
							checkdir = 0;
						}
						count_E5 = count_E5 +1;
						dest.x = destx_E5;
							
						SDL_BlitSurface(enemigo5, &dest, screen, &src);

					}else if(enemigos[i].dir == 1){
				
							if ((count_E5 >=15) || (checkdir = 1))
						{ 
							if(bumeran_E5 ==0){destx_E5 = 54; bumeran_E5 = 1;}
							else if(bumeran_E5 ==1){destx_E5= 81; bumeran_E5 = 0;}
							
							count_E5 = 0;
							checkdir = 0;
						}
						count_E5 = count_E5 +1;
						dest.x = destx_E5;

						SDL_BlitSurface(enemigo5, &dest, screen, &src);
					
					}else if(enemigos[i].dir == 2){
				
							if ((count_E5 >=15) || (checkdir = 1))
						{ 
							if(bumeran_E5 ==0){destx_E5 = 108; bumeran_E5 = 1;}
							else if(bumeran_E5 ==1){destx_E5= 135; bumeran_E5 = 0;}
							
							count_E5 = 0;
							checkdir = 0;
						}
						count_E5= count_E5 +1;
						dest.x = destx_E5;

						SDL_BlitSurface(enemigo5, &dest, screen, &src);
					
					}else if(enemigos[i].dir == 3){
				
							if ((count_E5 >=15) || (checkdir = 1))
						{ 
							if(bumeran_E5 ==0){destx_E5 = 0; bumeran_E5 = 1;}
							else if(bumeran_E5 ==1){destx_E5= 27; bumeran_E5 = 0;}
							
							count_E5 = 0;
							checkdir = 0;
						}
						count_E5 = count_E5 +1;
						dest.x = destx_E5;

						SDL_BlitSurface(enemigo5, &dest, screen, &src);
					
					}
				}else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 > 0)){
			
					enemigos[i].timer2 = enemigos[i].timer2 - 1;

						dest.x = 216;
					SDL_BlitSurface(enemigo5, &dest, screen, &src);
				
				} else if((enemigos[i].estado == 1)&&(enemigos[i].timer2 == 0)){
					enemigos[i].tipo = 0;
				}
				
				}
			}
		}
	}
	
}

// The header of this two functions are for the student to complete
// Try changing the coordinates to see the effect that has on the game
// Also, are here the magic numbers justified? Or should be declared
// as constants in the beggining of the file?

int count_P = 0;
static void draw_player(int dir, int predir) {
	
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = J.x;
	src.y = J.y;
	src.w = J.w;
	src.h = J.h;

	
	dest.y = 0;
	dest.w = J.w;
	dest.h = J.h;

	if (J.timer > 1){		//Accion, y reestablecer al jugador cuando muere
		
		J.timer = J.timer - 1;

		if(J.timer > 1 ){
			J.x = J.x;
			J.y = J.y;
			
 
			if (J.timer<20)
			{
				dest.x = 88;
			}else if (J.timer>19 && J.timer<40)
			{
				dest.x = 66;
			}else if (J.timer>39 && J.timer<60)
			{
				dest.x = 44;
			}else if (J.timer > 59 && J.timer <80)
			{
				dest.x = 22;
			}
			else if (J.timer > 79)
			{
				dest.x = 0;
			} 
			SDL_BlitSurface(jugadorm, &dest, screen, &src);
		}
	}

	if (J.timer < 2 ){
		  
		if (J.timer == 1){
			J.x = 331;
			J.y = 54;
			dest.x = 66;
			J.timer = J.timer -1;

			for (int i = 0; i < 60; i++)
			{
				bombas[i].estado = 0;
			}
			SDL_BlitSurface(jugador, &dest, screen, &src);
		} 
		if(J.timer == 0){
		
			if (dir == 0 ){
				if(predir == 1){
					if (count_P<10)
					{
						dest.x = 0;
					}else if (count_P>9 && count_P<20)
					{
						dest.x = 22;
					}else if (count_P>19 && count_P<30)
					{
						dest.x = 0;
					}else if (count_P > 29 && count_P <40)
					{
						dest.x = 44;
					}
					else if (count_P = 40)
					{
						count_P = 0;
					}
				}else if(predir == 0){dest.x = 0;}
			}	 
			if (dir == 1 ){

				if(predir == 1){
					if (count_P<10)
					{
						dest.x = 132;
					}else if (count_P>9 && count_P<20)
					{
						dest.x = 154;
					}else if (count_P>19 && count_P<30)
					{
						dest.x = 132;
					}else if (count_P > 29 && count_P <40)
					{
						dest.x = 176;
					}
					else if (count_P = 40)
					{
						count_P = 0;
					}
				}else if(predir == 0){dest.x = 132;}
					
				}
			if (dir == 2 ){
					
				if(predir == 1){
					if (count_P<10)
					{
						dest.x = 198;
					}else if (count_P>9 && count_P<20)
					{
						dest.x = 220;
					}else if (count_P>19 && count_P<30)
					{
						dest.x = 198;
					}else if (count_P > 29 && count_P <40)
					{
						dest.x = 242;
					}
					else if (count_P = 40)
					{
						count_P = 0;
					}
				}else if(predir == 0){dest.x = 198;}
			}
			if (dir == 3 ){
					
				if(predir == 1){
					if (count_P<10)
					{
						dest.x = 66;
					}else if (count_P>9 && count_P<20)
					{
						dest.x = 88;
					}else if (count_P>19 && count_P<30)
					{
						dest.x = 66;
					}else if (count_P > 29 && count_P <40)
					{
						dest.x = 110;
					}
					else if (count_P = 40)
					{
						count_P = 0;
					}
				}else if(predir == 0){dest.x = 66;}
			}
		
		SDL_BlitSurface(jugador, &dest, screen, &src);
		}
	}
	
}

/* Function: check_collision
 * --------------------------
 * This function verifies if there is collision between two game
 * elements.
 *
 * Arguments:
 * 	a: game element to be checked
 *	b: game element to be checked
 *
 * Return:
 *	TRUE if there is collision 
 * 	FALSE if there isn't collision
 */
int colision_jugador_muro(player_t a, game_element_t b) {

	int izquierda_a, izquierda_b;
	int derecha_a, derecha_b;
	int arriba_a, arriba_b;
	int abajo_a, abajo_b;

	izquierda_a = a.x;
	derecha_a = a.x + a.w;
	arriba_a = a.y;
	abajo_a = a.y + a.h;

	izquierda_b = b.x;
	derecha_b = b.x + b.w;
	arriba_b = b.y;
	abajo_b = b.y + b.h;
	
	if (izquierda_a > derecha_b) {
		return 0;
	}

	if (derecha_a < izquierda_b) {
		return 0;
	}

	if (arriba_a > abajo_b -15) {
		return 0;
	}

	if (abajo_a < arriba_b) {
		return 0;
	}

	if ((abajo_a > arriba_b) || (derecha_a > izquierda_b) || (izquierda_a < derecha_b) || (arriba_a < abajo_b)){
		return 1;
	}

	
}

int colision_jugador_enemigo(player_t a, enemigo_t b) {

	int izquierda_a, izquierda_b;
	int derecha_a, derecha_b;
	int arriba_a, arriba_b;
	int abajo_a, abajo_b;

	izquierda_a = a.x;
	derecha_a = a.x + a.w;
	arriba_a = a.y;
	abajo_a = a.y + a.h;

	izquierda_b = b.x;
	derecha_b = b.x + b.w;
	arriba_b = b.y;
	abajo_b = b.y + b.h;
	
	if (izquierda_a > derecha_b) {
		return 0;
	}

	if (derecha_a < izquierda_b) {
		return 0;
	}

	if (arriba_a > abajo_b -2) {
		return 0;
	}

	if (abajo_a < arriba_b) {
		return 0;
	}

	if ((abajo_a > arriba_b) || (derecha_a > izquierda_b) || (izquierda_a < derecha_b) || (arriba_a < abajo_b)){
		return 1;
	}

	
}

/* Function: move_player
 * ---------------------
 * This function is in charge of relocating the player position
 * across the screen
 *
 * Arguments:
 *	d: direction on which the player is going to be moved.
 *
 * Return:
 *	void.
 */
int vacio = 1; //Para indicar cuando el jugador se aparto completamente de la bomba, y poner a trabajar la colision
static void mover_jugador(int d  ) {

	int velocidad = 2;	
	// W moverse hacia arriba
	if (d == 0) {
		 J.y -= velocidad;
		
		
		for (int i = 0; i < 104; i++)
		{	
			if(i != 60){
				if(colision_jugador_muro( J, muros[i]) != 0) {
					J.y =  muros[i].y + ( muros[i].h-14);
				}
			}else if(i == 60){
				if (gate == 0){
					if(colision_jugador_muro( J, muros[i]) != 0) {
					J.y =  muros[i].y + ( muros[i].h-14);
					}
				}else if (gate != 0){
					if((J.x > muros[60].x) && ((J.x + J.w) < (muros[60].x + muros[60].w)) && (J.y+2 >= muros[60].y) && ((J.y + J.h)-2 <= (muros[60].y + muros[60].h))){
						if(J.score1 > 4){ 
							state = NEXT_LEVEL;  
						}
					}
				}
			}
		}
		for (int b = 0; b < 20; b++)
		{
			if((bombas[b].estado == 1) && (bombas[b].colision == 0) ){
				if(colision_jugador_bomba2( J, bombas[b]) != 0){ //Verifica si el jugador se alejo de la bomba
					vacio = 0;									//Desactiva colision
				}else {
					vacio = 1;	
					bombas[b].colision = 1;					//Activa colision
				}
			}
			if(vacio == 1){
				if(colision_jugador_bomba2( J, bombas[b]) != 0) {
				J.y =  bombas[b].y + ( bombas[b].h-4);
				}
			}
		}
		
	}
	
	// A moverse a la izquierda
	else if (d == 1) {
		 J.x -= velocidad;
		
		for (int i = 0; i < 104; i++)
		{
			if(i != 60){
				if(colision_jugador_muro( J, muros[i]) != 0) {
					J.x =  muros[i].x + ( muros[i].w+1);
				}
			}else if(i == 60){

				if (gate == 0){
					if(colision_jugador_muro( J, muros[i]) != 0) {
					J.x =  muros[i].x + ( muros[i].w+1);
					}
				}else if (gate != 0){
					if((J.x > muros[60].x) && ((J.x + J.w) < (muros[60].x + muros[60].w)) && (J.y+2 >= muros[60].y) && ((J.y + J.h)-2 <= (muros[60].y + muros[60].h))){
						if(J.score1 > 4){ 
							state = NEXT_LEVEL;  
						}
					}
				}
			}
		}
		for (int b = 0; b < 20 ; b++)
		{
			if((bombas[b].estado == 1) && (bombas[b].colision == 0) ){
				if(colision_jugador_bomba2( J, bombas[b]) != 0){ //Verifica si el jugador se alejo de la bomba
					vacio = 0;									//Desactiva colision
				}else {
					vacio = 1;	
					bombas[b].colision = 1;								//Activa colision
				}
			}
			if(vacio == 1){
				if(colision_jugador_bomba2( J, bombas[b]) != 0) {
				J.x =  bombas[b].x + ( bombas[b].w+1);
				}
			}
		}
		


	}
		
	// D moverse a la derecha
	else if (d == 2) {
		 J.x += velocidad;
		
		for (int i = 0; i < 104; i++)
		{
			if(i != 60){
				if(colision_jugador_muro( J, muros[i]) != 0) {
				J.x =  muros[i].x - ( J.w+1);
				}
			}else if(i == 60){

				if (gate == 0){
					if(colision_jugador_muro( J, muros[i]) != 0) {
					J.x =  muros[i].x - ( J.w+1);
					}
				}else if (gate != 0){
					if((J.x > muros[60].x) && ((J.x + J.w) < (muros[60].x + muros[60].w)) && (J.y+2 >= muros[60].y) && ((J.y + J.h)-2 <= (muros[60].y + muros[60].h))){
						if(J.score1 > 4){ 
							state = NEXT_LEVEL;  
						}
					}
				}
			}
		}
		for (int b = 0; b < 20; b++)
		{
			if((bombas[b].estado == 1) && (bombas[b].colision == 0) ){
					if(colision_jugador_bomba2( J, bombas[b]) != 0){ //Verifica si el jugador se alejo de la bomba
						vacio = 0;									//Desactiva colision
					}else {
						vacio = 1;
						bombas[b].colision = 1;										//Activa colision
					}
				}
			if(vacio == 1){
				if(colision_jugador_bomba2( J, bombas[b]) != 0) {
				J.x =  bombas[b].x - ( J.w+1);
				}
			}
		}
		

	}	
	
	// S mmoverse hacia abajo
	else if (d == 3) {
		 J.y += velocidad;

		for (int i = 0; i < 104; i++)
		{
			if(i != 60){
				if(colision_jugador_muro( J, muros[i]) != 0) {
					J.y =  muros[i].y - ( J.h+1);
				}
			}else if(i == 60){

				if (gate == 0){
					if(colision_jugador_muro( J, muros[i]) != 0) {
					J.y =  muros[i].y - ( J.h+1);
					}
				}else if (gate != 0){
					if((J.x > muros[60].x) && ((J.x + J.w) < (muros[60].x + muros[60].w)) && (J.y+2 >= muros[60].y) && ((J.y + J.h)-2 <= (muros[60].y + muros[60].h))){
						if(J.score1 > 4){ 
							state = NEXT_LEVEL;  
						}
					}
				}
			}
		}
		for (int b = 0; b < 20; b++)
		{
			if((bombas[b].estado == 1) && (bombas[b].colision == 0) ){
				if(colision_jugador_bomba2( J, bombas[b]) != 0){ //Verifica si el jugador se alejo de la bomba
					vacio = 0;									//Desactiva colision
				}else {
					vacio = 1;		
					bombas[b].colision = 1;								//Activa colision
				}
			}
			if(vacio == 1){
				if(colision_jugador_bomba2( J, bombas[b]) != 0) {
				J.y =  bombas[b].y - ( J.h+1);
				}
			}
		}
		
	}
			
	
}


// Main function

int main (int argc, char *args[]) {

	

	// For the project the elements of the map should be created
	// dinamically (using malloc) and using linked lists.
	

	//SDL Window setup
	if (init_SDL(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == FAILURE) {
		
		return FAILURE;
	}
	
	SDL_GetWindowSize(window, &g_width, &g_height);
	
	int sleep = 0;
	int quit = FALSE;
	Uint32 next_game_tick = SDL_GetTicks();
	int dir=ABAJO;
	int predir=0;
	int bombaE=0;
	int bombaN=0;
		int EN=0;
	 
		

	 
	//cantidad = 20;
    //struct enemigo_t *enemigos = (struct enemigo_t *)malloc(cantidad * sizeof(struct enemigo_t));// Crear un arreglo dinámico de enteros
	//if (enemigos == NULL) {// Verificar si la asignación de memoria fue exitosa
	//	printf("Error al asignar memoria.");
	//	return 1;
	//}
	init_game(enemigos, cantidad); 
	
	//render loop
	while(quit == FALSE) {
	predir= 0;
	bombaE = 0;
		
	
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
			
		if (keystate[SDL_SCANCODE_ESCAPE]) {
		
			quit = TRUE;
		}
		if(J.timer == 0){
			
			if (keystate[SDL_SCANCODE_S]) {
				
				mover_jugador(ABAJO);
				count_P ++;
				dir=ABAJO;
				predir=1;
			}

			if (keystate[SDL_SCANCODE_W]) {
				
				mover_jugador(ARRIBA);
				count_P ++;
				dir=ARRIBA;
				predir=1;
			}
			
			if (keystate[SDL_SCANCODE_A]) {
				
				mover_jugador(IZQUIERDA);
				count_P ++;
				dir=IZQUIERDA;
				predir=1;
			}

			if (keystate[SDL_SCANCODE_D]) {
				
				mover_jugador(DERECHA);
				count_P ++;
				dir=DERECHA;
				predir=1;
			}
			if (keystate[SDL_SCANCODE_SPACE] && start ==1) {
					
					bombaE=1;
					if ( EN == 0){
						for (int b = 0; b <= 20; b++)
						{
							
							if(bombas[b].estado == 0){
							bombas[b].timer1 = 160;
							bombaN = b;
							
							break;
							}
						
							
						}
						EN =20;
						
					}
					
				}
			if (keystate[SDL_SCANCODE_M]) {
					if (map < 3){ 
					state = NEXT_LEVEL;
					
					}else if (map >= 3){
						map = map;
						state = WINNER;
						
					}
					SDL_Delay(INPUT_DELAY_MS);
				}
			if (keystate[SDL_SCANCODE_N]) {

				if (map > 1){
				map = map - 1;
				init_game(enemigos, cantidad); 
				}else if (map <= 1){
					map = map;
					init_game(enemigos, cantidad); 
				}
				SDL_Delay(INPUT_DELAY_MS);
			}
			if(J.estado == 1){
				predir = 1;
				dir = 3;
				J.estado = 0;
			}
		}
		//draw background
		SDL_RenderClear(renderer);
		SDL_FillRect(screen, NULL, BLACK);
		
		//display main menu
		if (state == START_SCREEN ) {
		
			if (keystate[SDL_SCANCODE_SPACE]) {
				
				state = NEXT_LEVEL;
				SDL_Delay(INPUT_DELAY_MS);
				//start=1;
				init_game(enemigos, cantidad);		//Reiniciar parametros
			}
		
			//draw menu 
			draw_menu();
		
		//display gameover
		}else if (state == GAME_OVER) { 
			if (keystate[SDL_SCANCODE_SPACE]) {
				state = START_SCREEN; 
            	SDL_Delay(INPUT_DELAY_MS);
						
			}
				start = 0;			//Reiniciar para que a la hora de presionar espacio 
				
				draw_game_over();		//para volver a iniciar no porga una bomba
				
				map = 1;
				predir = 1;
				dir = 3;		//Para que la imagen del jugador aparezca de frente
		//display the game
		}else if (state == WINNER) { 
			if (keystate[SDL_SCANCODE_SPACE]) {
				state = START_SCREEN; 
            	SDL_Delay(INPUT_DELAY_MS);
				map = 0;
				predir = 1;
				dir = 3;
			}
				start = 0;			//Reiniciar para que a la hora de presionar espacio 
				
				Winner();		//para volver a iniciar no porga una bomba
				 
		}else if (state == NEXT_LEVEL) { 
			if (keystate[SDL_SCANCODE_SPACE]) {
				if(map < 3) {
					map = map+1;
					state = START_GAME;  
				}else if(map >= 3) {
					state = WINNER;  
				}
				init_game(enemigos, cantidad); 
            	SDL_Delay(INPUT_DELAY_MS);
				start=1;			//Reiniciar para que a la hora de presionar espacio 
				
						
			}
				 
				Nextlevel();		//para volver a iniciar no porga una bomba
				 
				predir = 1;
				dir = 3;		//Para que la imagen del jugador aparezca de frente
		//display the game
		}else if (state == START_GAME) {
			 
			//draw_gate();
			draw_muros(&level, &map );
			draw_bomba(bombaE, &bombaN, &EN);
			explosionV();
			explosionH();
			
			draw_gate();
			mover_enemigo(enemigos, cantidad);
			draw_enemy(enemigos, cantidad); //enemigos, cantidad
			draw_player(dir, predir);
			draw_space_vidas();
			draw_num_vidas();
			draw_img_vidas();
			draw_space_level();
			draw_num_level();
			draw_img_level();
			draw_space_score();
			draw_num1_score();
			draw_num2_score();
			draw_img_score();

			


		}
	
		SDL_UpdateTexture(screen_texture, NULL, screen->pixels, 
						  screen->w * sizeof (Uint32));
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);

		//draw to the display
		SDL_RenderPresent(renderer);
				
		//time it takes to render  frame in milliseconds
		next_game_tick += FRAME_TIME_MS;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {
            				
			SDL_Delay(sleep);
		}
	}

	//free loaded images
	SDL_FreeSurface(screen);
	SDL_FreeSurface(title);
	SDL_FreeSurface(winn);
	SDL_FreeSurface(ground);
	SDL_FreeSurface(ground1);
	SDL_FreeSurface(ground3);
	SDL_FreeSurface(ground0);
	SDL_FreeSurface(numbermap);
	SDL_FreeSurface(rtg);
	SDL_FreeSurface(vida);
	SDL_FreeSurface(nivel);
	SDL_FreeSurface(puntos);
	SDL_FreeSurface(bloqueMarcoh1);
	SDL_FreeSurface(bloqueMarcoh2);
	SDL_FreeSurface(bloqueMarcow1);
	SDL_FreeSurface(bloqueMarcow2);
	SDL_FreeSurface(end);
	SDL_FreeSurface(jugador);
	SDL_FreeSurface(jugadorm);
	SDL_FreeSurface(muro1);
	SDL_FreeSurface(muro2);
	SDL_FreeSurface(muro3);
	SDL_FreeSurface(muro4);
	SDL_FreeSurface(muro5);
	SDL_FreeSurface(puerta);
	SDL_FreeSurface(puerta2);
	SDL_FreeSurface(muro2Exp);
	SDL_FreeSurface(bomba);
	SDL_FreeSurface(llamah1);
	SDL_FreeSurface(llamav1);
	SDL_FreeSurface(enemigo1);
	SDL_FreeSurface(enemigo2);
	SDL_FreeSurface(enemigo3);
	SDL_FreeSurface(enemigo4);
	SDL_FreeSurface(enemigo5);
	SDL_FreeSurface(enemigomuerto);

	//free renderer and all textures used with it
	SDL_DestroyRenderer(renderer);
	
	//Destroy window 
	SDL_DestroyWindow(window);

	//Quit SDL subsystems 
	SDL_Quit(); 
	 
	return SUCCESS;
	
}

/* Function: init_SDL
 * ----------------------------
 * This function initialises the required environment for SDL
 * to work.
 *
 * Arguments:
 *	width: The width of the screen.
 * 	height: The height of the screen.
 *	argc: The same parameter from the main function.
 *	args: The same parameter from the main function.
 *
 * Return:
 *	SUCCESS if no problem reported.
 *	FAILURE if any initialization failed
 */
int init_SDL(int width, int height, int argc, char *args[]) {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		
		return FAILURE;
	} 
	
	int i;
	
	for (i = 0; i < argc; i++) {
		
		//Create window	
		if(strcmp(args[i], "-f")) {
			
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
		
		} else {
		
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window,&renderer);
		}
	}

	if (window == NULL) { 
		
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return FAILURE;
	}

	// Create the screen surface where all the elements will be drawn
	screen = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, 
											SDL_PIXELFORMAT_RGBA32);
	
	if (screen == NULL) {
		
		printf("Could not create the screen surfce! SDL_Error: %s\n", 
			   SDL_GetError());

		return FAILURE;
	}

	// Create the screen texture to render the screen surface to the actual 
	// display
	screen_texture = SDL_CreateTextureFromSurface(renderer, screen);

	if (screen_texture == NULL) {
		
		printf("Could not create the screen_texture! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}

	//Load the title image
	title = SDL_LoadBMP("title.bmp");

	if (title == NULL) {
		
		printf("Could not Load title image! SDL_Error: %s\n", SDL_GetError());

		return FAILURE;
	}
	
	//Load the numbermap image
	numbermap = SDL_LoadBMP("numbermap.bmp");

	if (numbermap == NULL) {
		
		printf("Could not Load numbermap image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the rtg image
	rtg = SDL_LoadBMP("rtg.bmp");

	if (rtg == NULL) {
		
		printf("Could not Load rtg image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	vida = SDL_LoadBMP("vida.bmp");

	if (vida == NULL) {
		
		printf("Could not Load vida image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	nivel = SDL_LoadBMP("nivel.bmp");

	if (nivel == NULL) {
		
		printf("Could not Load nivel image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	puntos = SDL_LoadBMP("puntos.bmp");

	if (puntos == NULL) {
		
		printf("Could not Load puntos image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	

	//Load the bloqueMarcoh1 image
	bloqueMarcoh1 = SDL_LoadBMP("bloqueMarcoh1.bmp");

	if (bloqueMarcoh1 == NULL) {
		
		printf("Could not Load bloqueMarcoh1 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}

	//Load the bloqueMarcoh2 image
	bloqueMarcoh2 = SDL_LoadBMP("bloqueMarcoh2.bmp");

	if (bloqueMarcoh2 == NULL) {
		
		printf("Could not Load bloqueMarcoh2 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the bloqueMarcow1 image
	bloqueMarcow1 = SDL_LoadBMP("bloqueMarcow1.bmp");

	if (bloqueMarcow1 == NULL) {
		
		printf("Could not Load bloqueMarcow1 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the bloqueMarcow2 image
	bloqueMarcow2 = SDL_LoadBMP("bloqueMarcow2.bmp");

	if (bloqueMarcow2 == NULL) {
		
		printf("Could not Load bloqueMarcow2 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	
	//Load the gameover image
	end = SDL_LoadBMP("gameover.bmp");

	if (end == NULL) {
		
		printf("Could not Load title image! SDL_Error: %s\n", SDL_GetError());

		return FAILURE;
	}
	winn = SDL_LoadBMP("winn.bmp");

	if (end == NULL) {
		
		printf("Could not Load winn image! SDL_Error: %s\n", SDL_GetError());

		return FAILURE;
	}

	//Load the ground image
	ground = SDL_LoadBMP("groundT2.bmp");

	if (ground == NULL) {
		
		printf("Could not Load ground image! SDL_Error: %s\n", SDL_GetError());

		return FAILURE;
	}
	//Load the ground1 image
	ground1 = SDL_LoadBMP("groundT1.bmp");

	if (ground1 == NULL) {
		
		printf("Could not Load ground1 image! SDL_Error: %s\n", SDL_GetError());

		return FAILURE;
	}
	//Load the ground3 image
	ground3 = SDL_LoadBMP("groundT3.bmp");

	if (ground3== NULL) {
		
		printf("Could not Load ground3 image! SDL_Error: %s\n", SDL_GetError());

		return FAILURE;
	}
	//Load the ground0 image
	ground0 = SDL_LoadBMP("ground01.bmp");

	if (ground0 == NULL) {
		
		printf("Could not Load ground0 image! SDL_Error: %s\n", SDL_GetError());

		return FAILURE;
	}
	
	//Load the player image
	jugador = SDL_LoadBMP("jugador.bmp");

	if (numbermap == NULL) {
		
		printf("Could not Load numbermap image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the player image
	jugadorm = SDL_LoadBMP("jugadorm.bmp");

	if (numbermap == NULL) {
		
		printf("Could not Load numbermap image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	
	//Load the muro image
	muro1 = SDL_LoadBMP("bloqueL1.bmp");
	if (muro1 == NULL) {
		
		printf("Could not Load muro image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the muro2 image
	muro2 = SDL_LoadBMP("bloque2.bmp");
	if (muro2 == NULL) {
		
		printf("Could not Load muro2 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the muro2Exp image
	muro2Exp = SDL_LoadBMP("bloque2Exp.bmp");
	if (muro2Exp == NULL) {
		
		printf("Could not Load muro2Exp image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the muro2 image
	muro3 = SDL_LoadBMP("bloqueness1.bmp");
	if (muro3 == NULL) {
		
		printf("Could not Load muro3 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the muro2 image
	muro4 = SDL_LoadBMP("bloque2ness.bmp");
	if (muro4 == NULL) {
		
		printf("Could not Load muro4 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the muro2 image
	muro5 = SDL_LoadBMP("bloqueexp2ness.bmp");
	if (muro5 == NULL) {
		
		printf("Could not Load muro5 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the muro2 image
	puerta = SDL_LoadBMP("puerta.bmp");
	if (puerta == NULL) {
		
		printf("Could not Load puerta image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	puerta2 = SDL_LoadBMP("puertaabierta.bmp");
	if (puerta2 == NULL) {
		
		printf("Could not Load puerta2 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}

	//Load the bomba image
	bomba = SDL_LoadBMP("bomba.bmp");
	if (bomba == NULL) {
		
		printf("Could not Load bomba image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the llamah1 image
	llamah1 = SDL_LoadBMP("LlamaH1.bmp");
	if (llamah1 == NULL) {
		
		printf("Could not Load Llamah1 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the llamav1 image
	llamav1 = SDL_LoadBMP("LlamaV1.bmp");
	if (llamav1 == NULL) {
		
		printf("Could not Load llamav1 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}

	//Load the enemigo image
	enemigo1 = SDL_LoadBMP("enemigo1.bmp");
	if (enemigo1 == NULL) {
		
		printf("Could not Load enemigo1 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the enemigo image
	enemigo2 = SDL_LoadBMP("enemigo3.bmp");
	if (enemigo2 == NULL) {
		
		printf("Could not Load enemigo2 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the enemigo image
	enemigo3 = SDL_LoadBMP("et.bmp");
	if (enemigo3 == NULL) {
		
		printf("Could not Load enemigo3 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the enemigomuerto image
	enemigomuerto = SDL_LoadBMP("enemigomuerto.bmp");
	if (enemigomuerto == NULL) {
		
		printf("Could not Load enemigomuerto image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the enemigo image
	enemigo4 = SDL_LoadBMP("rosat.bmp");
	if (enemigo4 == NULL) {
		
		printf("Could not Load enemigo4 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	//Load the enemigo image
	enemigo5 = SDL_LoadBMP("pikat.bmp");
	if (enemigo5 == NULL) {
		
		printf("Could not Load enemigo5 image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	
	// Set the title colourkey. 
	Uint32 colorkey = SDL_MapRGB(title->format, 255, 0, 255);
	SDL_SetColorKey(title, SDL_TRUE, colorkey);
	SDL_SetColorKey(winn, SDL_TRUE, colorkey);
	SDL_SetColorKey(ground, SDL_TRUE, colorkey);
	SDL_SetColorKey(ground1, SDL_TRUE, colorkey);
	SDL_SetColorKey(ground3, SDL_TRUE, colorkey);
	SDL_SetColorKey(ground0, SDL_TRUE, colorkey);
	SDL_SetColorKey(numbermap, SDL_TRUE, colorkey);
	SDL_SetColorKey(rtg, SDL_TRUE, colorkey);
	SDL_SetColorKey(vida, SDL_TRUE, colorkey);
	SDL_SetColorKey(nivel, SDL_TRUE, colorkey);
	SDL_SetColorKey(puntos, SDL_TRUE, colorkey);
	SDL_SetColorKey(bloqueMarcoh1, SDL_TRUE, colorkey);
	SDL_SetColorKey(bloqueMarcoh2, SDL_TRUE, colorkey);
	SDL_SetColorKey(bloqueMarcow1, SDL_TRUE, colorkey);
	SDL_SetColorKey(bloqueMarcow2, SDL_TRUE, colorkey);
	SDL_SetColorKey(jugador, SDL_TRUE, colorkey);
	SDL_SetColorKey(jugadorm, SDL_TRUE, colorkey);
	SDL_SetColorKey(muro1, SDL_TRUE, colorkey);
	SDL_SetColorKey(muro2, SDL_TRUE, colorkey);
	SDL_SetColorKey(muro2Exp, SDL_TRUE, colorkey);
	SDL_SetColorKey(muro3, SDL_TRUE, colorkey);
	SDL_SetColorKey(muro4, SDL_TRUE, colorkey);
	SDL_SetColorKey(muro5, SDL_TRUE, colorkey);
	SDL_SetColorKey(puerta, SDL_TRUE, colorkey);
	SDL_SetColorKey(puerta2, SDL_TRUE, colorkey);
	SDL_SetColorKey(bomba, SDL_TRUE, colorkey);
	SDL_SetColorKey(llamah1, SDL_TRUE, colorkey);
	SDL_SetColorKey(llamav1, SDL_TRUE, colorkey);
	SDL_SetColorKey(enemigo1, SDL_TRUE, colorkey);
	SDL_SetColorKey(enemigo2, SDL_TRUE, colorkey);
	SDL_SetColorKey(enemigo3, SDL_TRUE, colorkey);
	SDL_SetColorKey(enemigo4, SDL_TRUE, colorkey);
	SDL_SetColorKey(enemigo5, SDL_TRUE, colorkey);
	SDL_SetColorKey(enemigomuerto, SDL_TRUE, colorkey);
	
	return SUCCESS;
}
