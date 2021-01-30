#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define M 23

typedef struct Queuenode_struct {
    int x;
    int y;
    struct Queuenode_struct* next;
} Queuenode;

typedef struct Queue_struct {
    Queuenode* head;
    Queuenode* tail;
} Queue;

void enqueue(Queue* queue, int x, int y);
Queuenode* dequeue(Queue* queue);
int isempty(Queue* queue);
void place(char plane[M][M+1], Queue* queue, int x, int y);

int main(int argc, char* argv[]) {
    //INITIALIZE
    int i, j;
    int x, y, h;
    int fps = 165;
    int argoff = 1;
    if (argc > 1 && !strcmp(argv[1], "--fps")) {
        fps = atoi(argv[2]);
        argoff = 3;
    }
    char plane[M][M+1];
    for (i = 0; i < M; i++) {
        for (j = 0; j < M; j++) {
            plane[j][i] = '0';
        }
    }
    for (i = 0; i < M; i++) {
        plane[i][M] = '\0';
    }
    for (i = argoff; i + 2 < argc; i += 3) {
        y = atoi(argv[i]);
        x = atoi(argv[i + 1]);
        h = atoi(argv[i + 2]);
        if (h < 0) 
            plane[y][x] = '#';
        else
            plane[y][x] = '0' + h;
    }
    if (plane[M / 2][M / 2] == '#') {
        printf("ERROR: CANNOT PUT A SINK WHERE THE SAND DROPS.");
        return 1;
    }
    Queue* queue = malloc(sizeof(Queue));
    Queuenode* c;

    //RUN
    while (1) {
        place(plane, queue, M / 2, M / 2);
        while (!isempty(queue)) {
            c = dequeue(queue);
            plane[c->y][c->x] -= 8;
            if (0 < c->x) {
                if (0 < c->y) {
                    place(plane, queue, c->x-1, c->y-1);
                }
                place(plane, queue, c->x-1, c->y);
                if (c->y < M - 1) {
                    place(plane, queue, c->x-1, c->y+1);
                }
            }
            if (0 < c->y) {
                place(plane, queue, c->x, c->y-1);
            }
            if (c->y < M - 1) {
                place(plane, queue, c->x, c->y+1);
            }
            if (c->x < M - 1) {
                if (0 < c->y) {
                    place(plane, queue, c->x+1, c->y-1);
                }
                place(plane, queue, c->x+1, c->y);
                if (c->y < M - 1) {
                    place(plane, queue, c->x+1, c->y+1);
                }
            }
            free(c);
        }
        for (i = 0; i < M; i++) {
            printf("%s\n", plane[i]);
        }
        printf("\n");
        usleep(1000000 / fps);
    }
    free(queue);
    return 0;
}

void enqueue(Queue* queue, int x, int y) {
    Queuenode* n = malloc(sizeof(Queuenode));
    n->x = x;
    n->y = y;
    if (isempty(queue)) {
        queue->head = n;
    }
    else {
        queue->tail->next = n;
    }
    queue->tail = n;
}

Queuenode* dequeue(Queue* queue) {
    Queuenode* n = queue->head;
    if (queue->head == queue->tail) {
        queue->head = NULL;
        queue->tail = NULL;
    }
    else {
        queue->head = n->next;
        n->next = NULL;
    }
    return n;
}

int isempty(Queue* queue) {
    return queue->head == NULL;
}

void place(char plane[M][M+1], Queue* queue, int x, int y) {
    if (plane[y][x] != '#') {
        if ((++plane[y][x] - '0') % 9 == 0) {
            enqueue(queue, x, y);
        }
    }
}
