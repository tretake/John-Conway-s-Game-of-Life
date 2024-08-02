#include "SDL.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

int largura_tela;
int altura_tela;
bool rodando = true;

SDL_Window* janela = NULL;
SDL_Surface* superficie = NULL;
SDL_Renderer* render = NULL;

SDL_Event e;
bool quit = false;

int iniciar_SDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("falhou ao iniciar SDL, erro : %s \n", SDL_GetError());
		return EXIT_FAILURE;
	}
	else
	{
		janela = SDL_CreateWindow("ola mundo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, largura_tela, altura_tela, SDL_WINDOW_SHOWN);
		if (janela == NULL)
		{
			printf("falhou ao criar a janela, erro : %s \n", SDL_GetError());
			return EXIT_FAILURE;
		}
		else
		{
			/*
			psuperficie = SDL_GetWindowSurface(pjanela);
			SDL_FillRect(psuperficie, NULL, 0x000000);
			SDL_UpdateWindowSurface(pjanela);
			*/
			render = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);

			if (render == NULL)
			{
				printf("falhou ao criar o render, erro %s", SDL_GetError());
				return EXIT_FAILURE;
			}
		}
	}
}

void imput()
{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
				rodando = false;
			}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_r)
				{
					rodando = false;
				}
			}

		}
}

void fechar_SDL(SDL_Window* pjanela)
{
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(janela);
	SDL_Quit();

}


//limitador de frames
Uint32 tick_intervalo = 100; //8 -> 125fps
Uint32 proximo_tick;
float frames = 0;
int segundo = 0;
void  limit_frames()
{
	Uint32 agora;
	frames++;

	agora = SDL_GetTicks();

	/*if (agora > segundo + 1000)
	{
		std::cout << frames << "\n\n";

		segundo = agora;

		frames = 0;
	}*/


	if (proximo_tick > agora)
	{
		SDL_Delay(proximo_tick - agora);
	}
	else if (proximo_tick < agora)
		proximo_tick = agora;


	proximo_tick += tick_intervalo;
}


struct tile
{
	SDL_FRect area;
	bool estado = 1;
	bool estado_futuro = 1;
};
std::vector<tile> pac;

int quantidade_x;
int quantidade_y;


bool get_estado(int x, int y)
{
	if (x < 0)
		x = quantidade_x -1;
	if (y < 0)
		y = quantidade_y-1;

	if (x == quantidade_x)
		x = 0;
	if (y == quantidade_y)
		y = 0;


	return pac[x + y * quantidade_x].estado;
}

int main(int argc, char *argv[])
{
	quit = false;
	while (quit == false) {
		std::cout << "digite o comprimento da tela : ";
		std::cin >> largura_tela;
		std::cout << "digite a altura da tela : ";
		std::cin >> altura_tela;

		system("cls");

		std::cout << "aperte 'r' na janela da simulação para redefinir o tamanho da tela e os cubos\n";

		iniciar_SDL();

		srand((unsigned)time(NULL));

		int aresta = 10;

		quantidade_x = largura_tela / aresta;
		quantidade_y = altura_tela / aresta;
		for (int j = 0 ; j < quantidade_y ; j++) 
		{
			for (int i = 0; i < quantidade_x; i++)
			{
				bool estado = rand() % 2;
				tile a = { i * aresta + 1 , j* aresta +1 , 10 -2 , 10- 2, estado,estado};
				pac.push_back(a);
			}
		}

		//GAME

		proximo_tick = SDL_GetTicks() + tick_intervalo;
		rodando = true;
		while (rodando)
		{
			SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(render);




			SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF);



		
			for (int j = 0; j < quantidade_y; j++)
			{
				for (int i = 0; i < quantidade_x; i++)
				{
					int count = 0;
					if (pac[i + j * quantidade_x].estado) {
						SDL_RenderFillRectF(render, &pac[i + j * quantidade_x].area);
					}

					for (int y = j-1; y <= j+1; y++)
					{
						for (int x = i-1; x <= i+1; x++)
						{
							if (x == i && y == j)
								continue;

							if (get_estado(x, y))
								count++;
						}
					}

					if (pac[i + j * quantidade_x].estado == 1)
					{
						if (count < 2)
							pac[i + j * quantidade_x].estado_futuro = 0;
						else if(count == 2 || count == 3)
							pac[i + j * quantidade_x].estado_futuro = 1;
						else if(count > 3)
							pac[i + j * quantidade_x].estado_futuro = 0;
					}
					else if(count == 3)
					{
						pac[i + j * quantidade_x].estado_futuro = 1;
					}
				}
			}

			for (int i = 0; i < pac.size(); i++)
			{
				pac[i].estado = pac[i].estado_futuro;
			}
			






			limit_frames();
			SDL_RenderPresent(render);
			imput();
		}

		pac.clear();
		fechar_SDL(janela);
	}

		return EXIT_SUCCESS;
	
}