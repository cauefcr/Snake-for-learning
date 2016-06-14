#include "game_types.h"
#include "queue.h"

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

bool snake_alive(field* f, snake* snake, block* head) {
    if (head->code == IS_SNAKE) {
        return true;
    }
    if(head->code == IS_FOOD){
        return false;
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

void new_fruit(field* f) {
    int i = 1 + rand() % (f->i - 2);
    int j = 1 + rand() % (f->j - 2);
    if (f cell(i, j).code == IS_BG) {
        f cell(i, j) = block_new(IS_FOOD, i, j, food[0], food[1]);
    } else {
        new_fruit(f);
        return;
    }
    return;
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

    int sleep_time = 48000;
    field* f;
    board[1] = width()/2;//each block has 2 chars of width
    board[0] = height()-1;

    f = field_new(board[0], board[1]);
    snake* snake = malloc(sizeof(snake));
    snake->q = queue_new(4);
    snake->direction = 'w';
    block head;
    queue_push(f, snake->q, block_new(1, (f->i / 2) + 1, f->j / 2,
                                      snake_body[0], snake_body[1]));
    queue_push(f, snake->q,
               block_new(1, f->i / 2, f->j / 2, snake_up[0], snake_up[1]));
    bool quit = false; //new_question = true,
    char dir;
    int in;
    while (!quit) {
        field_print(f);
        std_sleep(sleep_time);
        in = input();
        dir = snake->direction;
        switch (in) {
            case 'w':
                if (snake->direction != 's')
                    snake->direction = 'w';
                break;
            case 's':
                if (snake->direction != 'w')
                    snake->direction = 's';
                break;
            case 'a':
                if (snake->direction != 'd')
                    snake->direction = 'a';
                break;
            case 'd':
                if (snake->direction != 'a')
                    snake->direction = 'd';
                break;
            default:
                break;
        }
        head = snake_move(f, snake);
        quit = snake_alive(f, snake, &head);
        if(rand()%60 == 0){
            new_fruit(f);
        }
    }
    int snake_size = queue_len(snake->q) - 2;
    wprintf(L"you lost the game with %d rats!\n", snake_size);

    field_free(f);
    queue_free(snake->q);
    free(snake);
    end();
    return 0;
}
