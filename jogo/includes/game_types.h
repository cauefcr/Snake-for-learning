// #pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #include "win_stuff.h"
    #define clear system("cls")
#elif(defined __unix__ || defined __APPLE__ || \
      defined __linux__)  // posix-compliant-things
    #include <sys/time.h>
    #include <unistd.h>
    #include <termios.h>
    #include <sys/ioctl.h>
    #include "linux_stuff.h"
    #define clear system("clear")
/*#elif defined __APPLE__
  #include <sys/time.h>
  #include <unistd.h>
  #include <termios.h>
  #include "linux_stuff.h"
  #define clear system("clear")
*/
#endif

#define newline wprintf(L"\n")  // putchar('\n')
// #define m(i,j,max_i) [i*max_i+j]
#define cell(i, j) ->m[i][j]
//"sprites"

#ifdef _WIN32  // sorry windows, but you're bad at this
#define BG (L' ')
#define top_u (L"══")      //("==")
#define top_d (L"══")      //("==")
#define side_l (L"║ ")     //("| ")
#define side_r (L" ║")     //(" |")
#define corner_ur (L"═╗")  //("=\\")
#define corner_ul (L"╔═")  //("/=")
#define corner_dr (L"═╝")  //("=/")
#define corner_dl (L"╚═")  //("\\=")

#define snake_up (L"/\\")    //("/\\")
#define snake_down (L"/\\")  //("\\/")
#define snake_left (L">#")   //("<\%")
#define snake_right (L"#<")  //("\%>")
#define snake_body (L"##")   //("\%\%")
//#define food (L":B")         //(":B")
#else

#define BG (L' ')
#define top_u (L"══")      //("==")
#define top_d (L"══")      //("==")
#define side_l (L"║ ")     //("| ")
#define side_r (L" ║")     //(" |")
#define corner_ur (L"═╗")  //("=\\")
#define corner_ul (L"╔═")  //("/=")
#define corner_dr (L"═╝")  //("=/")
#define corner_dl (L"╚═")  //("\\=")

#define snake_up (L"ᒠᒍ")     //("/\\")
#define snake_down (L"ᒖᒉ")   //("\\/")
#define snake_left (L"ᑒ⠿")   //("<\%")
#define snake_right (L"⠿ᑣ")  //("\%>")
#define snake_body (L"⠿⠿")   //("\%\%")
// #define food     (L"හ୭")//(":B")
#endif
wchar_t food[][2] = {{L'A', L')'},//mais visivel maiuscula
                     {L'B', L')'},
                     {L'C', L')'},
                     {L'D', L')'},
                     {L'E', L')'}};


#define IS_BG 0  // for checking the .code field inside the blocks
#define IS_SNAKE 1
#define IS_FOOD 2
#define IS_TEXT 3

typedef struct block { //a célula
    unsigned short int i, j;//tem sua posição i,j no campo
    unsigned char code;//seu tipo
    wchar_t v[2];//sua representação visual
} block;

block empty_block = {//bloco vazio para facilitar a criação e update de novos blocos
    0,
    0,
    0,
    {BG, BG},
};

typedef struct field {//o campo
    block** m;  // contém a matriz de blocos
    int i, j; //e suas dimensões
} field;

block block_new(char, int, int, wchar_t, wchar_t);
field* field_new(int, int);
void field_print(field*);
void field_free(field*);
void field_clear(field*);

block block_new(char code, int i, int j, wchar_t v0, wchar_t v1) {
    block out;  // = (block *) malloc(sizeof(block));
    out.code = code; // faz e retorna um bloco
    out.v[0] = v0;
    out.v[1] = v1;
    out.i = i;
    out.j = j;
    return out;
}

field* field_new(int i, int j) { //aloca dinâmicamente um campo
    field* f = (field*)malloc(sizeof(field)); //primeiro a struct de fora
    f->m = (block**)malloc(sizeof(block*) * i); //depois as linhas
    for (int a = 0; a < i; a++) { //dai as colunas
        f->m[a] = (block*)malloc(sizeof(block) * j);
    }
    f->i = i;//marca o tamanho
    f->j = j;
    field_clear(f);//escreve as bordas e o background
    return f;
}

void field_clear(field* f) {
    for (int a = 0; a < f->i; a++) {
        for (int b = 0; b < f->j; b++) {
            f->m[a][b] = empty_block;//coloca um bloco vazio em tudo
        }
    }
    for (int a = 0; a < f->i; a++) {
        f cell(a, 0).v[0] = side_l[0];//coloca as bordas laterais
        f cell(a, 0).v[1] = side_l[1];
        f cell(a, f->j - 1).v[0] = side_r[0];
        f cell(a, f->j - 1).v[1] = side_r[1];
    }
    for (int b = 0; b < f->j; b++) {
        f cell(0, b).v[0] = top_u[0];//o topo
        f cell(0, b).v[1] = top_u[1];
        f cell(f->i - 1, b).v[0] = top_d[0];
        f cell(f->i - 1, b).v[1] = top_d[1];
    }
    f cell(0, 0).v[0] = corner_ul[0];//os cantos
    f cell(0, 0).v[1] = corner_ul[1];
    f cell(0, f->j - 1).v[0] = corner_ur[0];
    f cell(0, f->j - 1).v[1] = corner_ur[1];
    f cell(f->i - 1, 0).v[0] = corner_dl[0];
    f cell(f->i - 1, 0).v[1] = corner_dl[1];
    f cell(f->i - 1, f->j - 1).v[0] = corner_dr[0];
    f cell(f->i - 1, f->j - 1).v[1] = corner_dr[1];
}

void field_print(field* f) {//imprime o campo
    clear;//limpa a tela
    for (int i = 0; i < f->i; i++) {
        for (int j = 0; j < f->j; j++) {//imprime cada par de wchars
            wprintf(L"%lc%lc", f->m[i][j].v[0], f->m[i][j].v[1]);
        }
        newline;//pula a linha
    }
}

void field_free(field* f) {//limpa a memória do campo
    for (int i = 0; i < f->i; i++) {
        free(f->m[i]); //limpa as linhas
    }
    free(f->m);//as colunas
    free(f);//o campo em si
    return;
}

typedef struct question {//a questão
    wchar_t text[500];//contém a representação em string da questão
    char answer;//e qual a resposta correta (o indice do mesmo)
} question;

question read_question(const char* filename) {//le a pergunta que está no arquivo tal
    FILE* q_file = fopen(filename, "r");//abre o arquivo
    wchar_t tmp[500];//string temporária
    question q;//cria a pergunta
    fgetws(tmp, 500, q_file);//le a do arquivo na string temporária
    for (int i = 0, j = 0; i < 500 && tmp[i]; i++, j++) {//processa a string
        q.text[j] = tmp[i];
        if (tmp[i] == L'/' && tmp[i + 1] == L'n') {//se encontrar um \n
            i++;
            q.text[j] = L'\n';//transforma-o em \n mesmo
        }
        if (!(tmp[i + 1])) {//se o proximo indice for vazio
            q.text[j] = L'\0';//escreve o \0 para sinalizar o fim da string
        }
   }
   fgetws(tmp,500,q_file);//le novamente outra linha, dessa vez contendo a resposta
   q.answer = tmp[0]-L'0';//faz um watoi
   return q;
}
