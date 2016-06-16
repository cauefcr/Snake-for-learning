// #include <termios.h>
// #include <sys/time.h>
// #include <unistd.h>
// void move_cursor(int x, int y){
//    printf("\033[%d;%dH",x,y);
// }

int kbhit() {//função para ver se alguma tecla foi apertada
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);  // STDIN_FILENO is 0
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

void nonblock(int state) {  // set terminal to and from non-blocking
    struct termios ttystate; // muda do modo normal, precisando dar enter pra pegar a tecla, pra não precisando

    // get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);

    if (state == 1) {
        // turn off canonical mode
        ttystate.c_lflag &= ~ICANON;
        // minimum of number input read.
        ttystate.c_cc[VMIN] = 1;
    } else if (state == 0) {
        // turn on canonical mode
        ttystate.c_lflag |= ICANON;
    }
    // set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

unsigned int input() {
    if (kbhit()) {//se alguma tecla for apertada
        return (unsigned int)fgetc(stdin); //retorne tal tecla
    } else {//caso contrario retorne vazio
        return 0;
    }
}

struct timespec wt[] = {{0, 0}}; //a struct global que vai ser utilizada para dar a pausa

void std_sleep(int waitTime) { //usleep via nanosleep
    wt[0].tv_nsec = waitTime * 1000;
    nanosleep(wt, NULL);
}

struct winsize w; // detalhes do tamanho do terminal

void start() { //roda funções especificas para se ver o tamanho do terminal e iniciar o modo async
    nonblock(1);  // STDOUT_FILENO is 0, usually
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return;
}

void end() { // volta o terminal ao modo anterior
    nonblock(0);
    return;
}

int width() { //retorna a largura do terminal
    return w.ws_col;
}

int height() { //retorna a altura do terminal
    // ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);
    return w.ws_row;
}
