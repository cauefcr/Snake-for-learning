#include "includes/game_types.h"
#include "includes/queue.h"
#include "includes/menus.h"

typedef struct snake {
    queue* q;
    char direction;
} snake;


block snake_move(field* f, snake* snake) {
    char dir = snake->direction;
    block next, out;
    block* tmp;
    tmp = queue_last(snake->q);
    bool grow = false;
    if (dir == 'w') {
        if (f cell(tmp->i - 1, tmp->j).code == IS_FOOD) {
            grow = true;
        }
        next =
            block_new(IS_SNAKE, tmp->i - 1, tmp->j, snake_up[0], snake_up[1]);
    } else if (dir == 's') {
        if (f cell(tmp->i + 1, tmp->j).code == IS_FOOD) {
            grow = true;
        }
        next = block_new(IS_SNAKE, tmp->i + 1, tmp->j, snake_down[0],
                         snake_down[1]);
    } else if (dir == 'a') {
        if (f cell(tmp->i, tmp->j - 1).code == IS_FOOD) {
            grow = true;
        }
        next = block_new(IS_SNAKE, tmp->i, tmp->j - 1, snake_left[0],
                         snake_left[1]);
    } else {  // if (dir == 'd') {
        if (f cell(tmp->i, tmp->j + 1).code == IS_FOOD) {
            grow = true;
        }
        next = block_new(IS_SNAKE, tmp->i, tmp->j + 1, snake_right[0],
                         snake_right[1]);
    }
    f cell(tmp->i, tmp->j).v[0] =
        snake_body[0];  // many segfaults were felt here
    f cell(tmp->i, tmp->j).v[1] = snake_body[1];
    tmp->v[0] = snake_body[0];
    tmp->v[1] = snake_body[1];
    queue_push(f, snake->q, next);
    out = f cell(next.i, next.j);  // what was the block before it became a
                                   // snake
    f cell(next.i, next.j) = next;
    if (!grow) {
        block tail = queue_pop(f, snake->q);
        f cell(tail.i, tail.j) = empty_block; // draws
    }
    return out;
}
bool correct_anwser(question* q, block* head){
    static int answer;
    answer = q->answer;
    if(head->v[0] == L'A'){
        if(answer == 1) return true;
    } else if(head->v[0] == L'B'){
        if(answer == 2) return true;
    } else if(head->v[0] == L'C'){
        if(answer == 3) return true;
    } else if(head->v[0] == L'D'){
        if(answer == 4) return true;
    } else if(head->v[0] == L'E'){
        if(answer == 5) return true;
    }
    return false;
}

bool snake_alive(field* f, snake* snake, block* head,question* q) {
    if (head->code == IS_SNAKE) {
        return true;
    }
    if(head->code == IS_FOOD){
        if(correct_anwser(q,head)){
            return false;//quit = false
        } else {
            return true;
        }
    }
    block* tmp = queue_last(snake->q);
    if (tmp->i == 0 || tmp->i == f->i - 1) {
        return true;
    } else if (tmp->j == 0 || tmp->j == f->j - 1) {
        return true;
    } else {
        return false;
    }
}

void new_fruit(field* f, int code) {
    int i = 1 + rand() % (f->i - 2);
    int j = 1 + rand() % (f->j - 2);
    if (f cell(i, j).code == IS_BG) {
        f cell(i, j) = block_new(2, i, j, food[code][0], food[code][1]);
    } else {
        new_fruit(f,code);
        return;
    }
}

void remove_block(field* f,int code){
    for(int i = f->i-1, j; i >= 0; i--){
        for(j = f->j-1; j >= 0; j--){
            if(f cell(i,j).code == code){
                f cell(i,j) = empty_block;
            }
        }
    }
}

// board size, width,height
int board[] = {23, 40};

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "");
    start();

    const char* filenames[] =  {"data/endgame",
                                "data/pergunta0",//d
                                "data/pergunta1",//b
                                "data/pergunta2",//c
                                "data/pergunta3",//b
                                "data/pergunta4",//a
                                "data/pergunta5",//e
                                "data/pergunta6",//d
                                "data/pergunta7",//b
                                "data/pergunta8",//b
                                "data/pergunta9",//e
                                "data/pergunta10",//d
                                "data/pergunta11",//a
                                "data/pergunta12",//a
                                "data/pergunta13",//d
                                "data/pergunta14",//d
                                "data/pergunta15",//e
                                "data/pergunta16",//a
                                "data/pergunta17",//d
                                "data/pergunta18",//a
                                "data/pergunta19"};//a
    int filenamesize = sizeof(filenames) / sizeof(filenames[0]);
    bool chosen[filenamesize];
    chosen[0] = true;
    for(int i = 1; i < filenamesize; i++){
        chosen[i] = false;
    }
    int sleep_time = 48000;
    field* f;
    board[1] = width()/2;//each block has 2 chars of width
    board[0] = height()-1;
    
    splash_screen(sleep_time,board);
    int difficulty = dif_chooser(sleep_time, board);

    switch (difficulty) {
        case 0:
            sleep_time += 16000;
            break;
        case 2:
            sleep_time -= 16000;
            break;
    }

    f = field_new(board[0], board[1]);
    snake* snake = malloc(sizeof(snake));
    snake->q = queue_new(4);
    snake->direction = 'w';
    block head;
    queue_push(f, snake->q, block_new(1, (f->i / 2) + 1, f->j / 2,
                                      snake_body[0], snake_body[1]));
    queue_push(f, snake->q,
               block_new(1, f->i / 2, f->j / 2, snake_up[0], snake_up[1]));
    question q;
    bool new_question = true,quit = false,new_answers = false;
    char dir;
    int in;
    while (!quit) {
        if(new_question){
            remove_block(f,IS_TEXT);
            int i;
            for(i = 0; i < filenamesize; i++){
                if(!chosen[i]) {
                    chosen[i] = true;
                    break;
                }
            }
            if(i == filenamesize){
                i = 0;//turn on congratulations
            }
            q = read_question(filenames[i]);
        }
        field_write(f,q.text,1,1);
        field_print(f);
        std_sleep(sleep_time);
        if(new_answers){
            std_sleep(sleep_time*10);
            new_answers = false;
        }
        in = input();
        dir = snake->direction;
        switch (in) {
            case 'w':
                if (dir != 's')
                    snake->direction = 'w';
                break;
            case 's':
                if (dir != 'w')
                    snake->direction = 's';
                break;
            case 'a':
                if (dir != 'd')
                    snake->direction = 'a';
                break;
            case 'd':
                if (dir != 'a')
                    snake->direction = 'd';
                break;
            default:
                break;
        }
        head = snake_move(f, snake);
        quit = snake_alive(f, snake, &head, &q);
        if(new_question){
            for(int i = 0; i < 5; i++){
                new_fruit(f,i);
            }
            new_question = false;
            new_answers = true;
        }
        if(!quit && head.code == IS_FOOD){
            new_question = true;
            remove_block(f,IS_FOOD);
        }
        // if (!(rand() % 60)) {
        //     new_fruit(f);
        // }
    }
    int snake_size = queue_len(snake->q) - 2;
    int retry = game_over(sleep_time, board, snake_size);

    field_free(f);
    queue_free(snake->q);
    free(snake);
    end();

    if (retry == 1)
        return main();
    clear;
    return 0;
}
