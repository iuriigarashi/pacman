#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <windows.h>
#include <time.h>

#define TAM 13
#define PAREDE 'X'
#define CHAO ' '
#define CIMA 'w'
#define BAIXO 's'
#define ESQUERDA 'a'
#define DIREITA 'd'
#define PARADO 'p'
#define MOVIMENTO 'm'

typedef struct map
{
    char dados[TAM][TAM];
} Mapa;

typedef struct pacman
{
    int i, j;
    char direction;
} Player;

typedef struct enemy
{
    int i, j;
    char direction;
} Enemy;

typedef struct coin
{
    int i, j;
} Coin;

typedef struct jogo
{
    Mapa mapa;
    Player pacman;
    unsigned int score;
    Coin coins[TAM*TAM];
    Enemy enemies[TAM*TAM];
    unsigned int total_coin_number;
    unsigned short int lifes;
    short int enemies_movement;
} Jogo;

void init_game(Jogo* g)
{
    srand(time(NULL));
    int total_enemies = 4;
    int e = 0;
    int x=0, y=0;
    char ch;
    //inicializa jogador
    g->pacman.i = TAM/2;
    g->pacman.j = TAM/2;
    g->pacman.direction = PARADO;
    //inicializa mapa
    /*for(int i=0; i< TAM; i++){
        for(int j=0; j < TAM; j++){
            if(i == 0 || j == 0 || i == TAM-1 || j == TAM-1){
                g->mapa.dados[i][j] = PAREDE;
            }else if(g->pacman.i != i || g->pacman.j != j){
                g->mapa.dados[i][j] = CHAO;
                g->coins[e].i = i;
                g->coins[e].j = j;
                e++;
            }else{
                g->mapa.dados[i][j] = CHAO;
            }
        }
    }*/
    //arquivo MAPA.TXT
    FILE *arquivo;
    arquivo = fopen("mapa.txt", "r");

    //Leitura do arquivo MAPA.TXT
    if(arquivo == NULL)
    {
        printf("ERRO: não foi possivel abrir mapa.txt\n");
        exit(1);
    }
    else
    {
        while( (ch = fgetc(arquivo))!= EOF )
        {
            if(ch != '\n')
            {
                g->mapa.dados[x][y] = ch;
                y++;
            }
            else
            {
                y=0;
                x++;
            }
        }
        fclose(arquivo);
    }

    int rdm, e2 = 0;
    for(int i=0; i< TAM; i++)
    {
        for(int j=0; j < TAM; j++)
        {
            if((g->pacman.i != i || g->pacman.j != j) && g->mapa.dados[i][j] != PAREDE)
            {
                g->mapa.dados[i][j] = CHAO;
                g->coins[e].i = i;
                g->coins[e].j = j;
                e++;
            }
            if((rdm = rand()%15) < 1 && ((g->pacman.i != i || g->pacman.j != j) && g->mapa.dados[i][j] != PAREDE) && e2 < total_enemies)
            {
                g->mapa.dados[i][j] = CHAO;
                g->enemies[e2].i = i;
                g->enemies[e2].j = j;
                e2++;
            }
        }
    }

    //inicializa entidades
    for(int i = e+1; i < TAM*TAM; i++)
    {
        g->coins[i].i = -1;
        g->coins[i].j = -1;
    }
    for(int i = e2+1; i < TAM*TAM; i++)
    {
        g->enemies[i].i = -1;
        g->enemies[i].j = -1;
        g->enemies[i].direction = PARADO;
    }
    //inicializa score
    g->score = 0;
    g->total_coin_number = e;
    g->lifes = 3;
    g->enemies_movement = PARADO;
}

int has_coin_in_pos(Jogo* g, int i, int j)
{
    for(int k=0; k < TAM*TAM; k++)
    {
        if(g->coins[k].i == i && g->coins[k].j == j)
            return k;
    }
    return -1;
}

int has_enemy_in_pos(Jogo* g, int i, int j)
{
    for(int k=0; k < TAM*TAM; k++)
    {
        if(g->enemies[k].i == i && g->enemies[k].j == j)
            return k;
    }
    return -1;
}

void init_enemies(Jogo* g)
{
    g->enemies_movement = MOVIMENTO;
}

void collision_coin_check(Jogo* g, int i, int j)
{
    int k;
    k = has_coin_in_pos(g, i, j);
    if(k >= 0)
    {
        g->coins[k].i = -1;
        g->coins[k].j = -1;
        g->score += 10;
        g->total_coin_number--;
    }
}

void collision_enemy_check(Jogo* g, int i, int j)
{
    int k;
    k = has_enemy_in_pos(g, i, j);
    if(k >= 0)
    {
        g->coins[k].i = -99;
        g->coins[k].j = -99;
        g->lifes--;
        g->pacman.i = TAM/2;
        g->pacman.j = TAM/2;
        g->pacman.direction = PARADO;
        g->enemies_movement = PARADO;
    }
}

void draw_scene(Jogo* g)
{
    int k, l;
    for(int i=0; i< TAM; i++)
    {
        for(int j=0; j < TAM; j++)
        {
            k = has_coin_in_pos(g, i, j);
            l = has_enemy_in_pos(g, i, j);
            if(g->mapa.dados[i][j] == PAREDE)
            {
                textbackground(RED);
                printf("  ");
                textbackground(BLACK);
            }
            else if(l >= 0)
            {
                textbackground(BLACK);
                textcolor(LIGHTBLUE);
                printf("o ");
                textcolor(WHITE);
                textbackground(BLACK);
            }
            else if(k >= 0)
            {
                textbackground(BLACK);
                textcolor(YELLOW);
                printf(". ");
                textcolor(WHITE);
                textbackground(BLACK);
            }
            else if(g->pacman.i == i && g->pacman.j == j)
            {
                textbackground(BLACK);
                textcolor(WHITE);
                printf("o ");
                textcolor(WHITE);
                textbackground(BLACK);
            }
            else
            {
                textbackground(BLACK);
                textcolor(WHITE);
                printf("  ");
                textcolor(WHITE);
                textbackground(BLACK);
            }
        }
        printf("\n");
    }
}

int collision_wall_check(Jogo* g, int i, int j)
{
    if(i < 0 || i > TAM-1 || j < 0 || j > TAM-1)
        return 0;
    if(g->mapa.dados[i][j] == PAREDE)
        return 1;
    return 0;
}

void move_enemies(Jogo* g)
{

    int rdm;
    if(g->enemies_movement != PARADO)
    {
        for(int k=0; k < TAM*TAM; k++)
        {
            rdm = (rand()%3)-1;
            if(g->enemies[k].i >= 0)
            {
                if(!collision_wall_check(g, g->enemies[k].i + rdm, g->enemies[k].j)){
                    g->enemies[k].i += rdm;
                    if(g->enemies[k].i < 0)
                        g->enemies[k].i = TAM-1;
                    if(g->enemies[k].i > TAM-1)
                        g->enemies[k].i = 0;
                }
            }

            if(rdm == 0)
            {
                rdm = (rand()%3)-1;
                if(g->enemies[k].j >= 0)
                {
                    if(!collision_wall_check(g, g->enemies[k].i, g->enemies[k].j + rdm)){
                        g->enemies[k].j += rdm;
                        if(g->enemies[k].j < 0)
                            g->enemies[k].j = TAM-1;
                        if(g->enemies[k].j > TAM-1)
                            g->enemies[k].j = 0;
                    }
                }
            }
        }
    }
}

void move_pacman(Jogo* g)
{
    if(g->pacman.direction == CIMA && !collision_wall_check(g, g->pacman.i-1, g->pacman.j))
    {
        g->pacman.i--;
        if(g->pacman.i < 0)
            g->pacman.i = TAM-1;
    }
    else if(g->pacman.direction == BAIXO && !collision_wall_check(g, g->pacman.i+1, g->pacman.j))
    {
        g->pacman.i++;
        if(g->pacman.i > TAM-1)
            g->pacman.i = 0;
    }
    else if(g->pacman.direction == DIREITA && !collision_wall_check(g, g->pacman.i, g->pacman.j+1))
    {
        g->pacman.j++;
        if(g->pacman.j > TAM-1)
            g->pacman.j = 0;
    }
    else if(g->pacman.direction == ESQUERDA && !collision_wall_check(g, g->pacman.i, g->pacman.j-1))
    {
        g->pacman.j--;
        if(g->pacman.j < 0)
            g->pacman.j = TAM-1;
    }
}

void change_direction(Jogo* g, char c)
{
    if(c == CIMA)
        g->pacman.direction = CIMA;
    else if(c == BAIXO)
        g->pacman.direction = BAIXO;
    else if(c == DIREITA)
        g->pacman.direction = DIREITA;
    else if(c == ESQUERDA)
        g->pacman.direction = ESQUERDA;
}

int main()
{
    Jogo j;
    init_game(&j);
    int jogando = 1;
    char c = 'v';

    while(jogando)
    {
        delay(150);
        clrscr();
        if(kbhit())
        {
            c = getch();
            change_direction(&j, c);
            init_enemies(&j);
        }
        move_pacman(&j);
        collision_coin_check(&j, j.pacman.i, j.pacman.j);
        collision_enemy_check(&j, j.pacman.i, j.pacman.j);
        draw_scene(&j);
        move_enemies(&j);
        printf("Score: %d\nLifes: %d\n", j.score, j.lifes);
        if(j.total_coin_number == 0 || j.lifes == 0) jogando = 0;

    }

    printf("Game over.");

    return 0;
}
