void field_write(field* f,
                 wchar_t* text,
                 unsigned short int i,
                 unsigned short int j) { //escreve um texto em um campo, na posição i,j
    for (int d = 0, k = 0, lines = j; text[d] && k + 1 < f->j; d += 2, k++) {
        if (k >= f->j - 2 || text[d] == L'\n') {
            d++;
            k = 0;
            lines++;
        }
        if (f cell(lines, k + i).code ==
            IS_BG) {  // only write on the background
            f cell(lines, k + i).v[0] = text[d];
            f cell(lines, k + i).code = IS_TEXT;
            f cell(lines, k + i).v[1] = BG;
        }
        if (text[d + 1] == L'\0') {
            break;
        }
        if (k >= f->j - 2 || text[d + 1] == L'\n') {
            // d--;       // don't write the \n
            // d++;
            k = 0;     // start writing from the start again
            lines++;   // jump a line
            continue;  // will only start text on the first block space
        }
        if (f cell(lines, k + i).code == IS_TEXT) {
            f cell(lines, k + i).v[1] = text[d + 1];
        }
    }
}

int dif_chooser(int sleep_time, int* board) {  // menu para escolher a dificuldade
    char c = 0, state = 1;
    field* f = field_new(board[0], board[1]); //cria um campo novo, do tamanho correto
    wchar_t* level; //cria um ponteiro pra string a ser impressa
    while (true) { //enquanto não for entrado um enter
        field_clear(f);//limpa o campo
        std_sleep(sleep_time);//espera o tempo dado
        c = input();//pega o input
        switch (state) {//escolhe a string
            case 0:
                level =
                    L"Escolha sua dificuldade:\n \n  ☒fácil \n  ◽médio \n  ◽difícil";
                break;
            case 1:
                level =
                    L"Escolha sua dificuldade:\n \n  ◽fácil \n  ☒médio \n  ◽difícil";
                break;
            case 2:
                level =
                    L"Escolha sua dificuldade:\n \n  ◽fácil \n  ◽médio \n  ☒difícil";
                break;
        }
        field_write(f, level, 1, 1);//escreve a string correta
        field_print(f);//escreve na tela
        switch (c) {//processa o input
            case '\n'://se for enter
                return state;//retorne o estado atual
            case 'w'://se apertar pra cima sobe
                if (state > 0)
                    state -= 1;
                break;
            case 's'://e pra baixo desce
                if (state < 2)
                    state += 1;
                break;
        }
    }
}

int game_over(int sleep_time, int* board, int food_n) {
    char c = 0, state = 1;
    field* f = field_new(board[0], board[1]);
    wchar_t level[100];
    while (true) {
        field_clear(f);
        std_sleep(sleep_time);
        c = input();
        switch (state) {
            case 0:
                swprintf(level, 100,
                         L"Você perdeu o jogo com %d quest%ls!\n Tentar novamente?\n ☒não "
                         L"\n ◽sim",
                         food_n,food_n==1?L"ão":L"ões");
                break;
            case 1:
                swprintf(level, 100,
                         L"Você perdeu o jogo com %d quest%ls!\n Tentar novamente?\n ◽não "
                         L"\n ☒sim",
                         food_n,food_n==1?L"ão":L"ões");
                break;
        }
        field_write(f, level, 1, 1);
        field_print(f);
        switch (c) {
            case '\n':
                return state;
            case 'w':
                if (state > 0)
                    state -= 1;
                break;
            case 's':
                if (state < 1)
                    state += 1;
                break;
        }
    }
}

void splash_screen(int sleep_time, int* board){
    wchar_t text[] = L"   ### #   #  ###  #  # ####  \n"
                     L"  #    ##  # #   # # #  #     \n"
                     L"   ##  # # # ##### ##   ###   \n"
                     L"     # #  ## #   # # #  #     \n"
                     L"  ###  #   # #   # #  # ####  \n"
                     L"         educativo            \n"
                     L"  \nmovimentação: \n"
                     L"w-cima\n"
                     L"a-esquerda\n"
                     L"s-baixo \n"
                     L"d-direita \n  \n"
                     L"Pressione <Enter> para continuar";
    char c = 0;
    field* f = field_new(board[0], board[1]);
    while(c == 0){
        field_clear(f);
        std_sleep(sleep_time);
        c = input();
        field_write(f, text, 1, 1);
        field_print(f);
        switch(c){
            case '\n':
            c = 1;
            break;
            default:
            c = 0;
            break;
        }
    }
}
