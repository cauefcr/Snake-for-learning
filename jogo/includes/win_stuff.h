//#include <windows.h>

CONSOLE_SCREEN_BUFFER_INFO csbi;
void start() {//pega o tamanho da tela
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return;
}

void end() {//só existe para não dar problema de função não encontrada
    return;
}
int width() {//retorna a largura da tela
    return csbi.srWindow.Right - csbi.srWindow.Left;// + 1;
}

int height() {//retorna a altura da tela
    return csbi.srWindow.Bottom - csbi.srWindow.Top;// + 1;
}

unsigned int input() {
    static unsigned int in = 0;
    if (_kbhit()) {//se uma tecla foi apertada
        in = _getch(); //retorne um caracter
        if(in == 13) in = '\n';
        return in;
    } else {//caso contrario retorne nada
        return 0;
    }
}

void std_sleep(int waitTime) {//implementação de usleep para o windows
    __int64 time1 = 0, time2 = 0, freq = 0;
    // é ruim, mas é o melhor da pra fazer
    QueryPerformanceCounter((LARGE_INTEGER*)&time1);
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

    do {
        QueryPerformanceCounter((LARGE_INTEGER*)&time2);//ela basicamente espera passar tanto tempo de cpu para sair
    } while ((time2 - time1) < waitTime);//bloqueando os processos enquanto isso
}