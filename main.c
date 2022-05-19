#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <math.h>

#define SIZE 200

typedef struct queue
{
    char url[200];
    float video_length;
    struct queue *next;
}Queue;

Queue *init_queue()
{
    return NULL;
}

FILE *open_file(char *input, char *mode)
{
    FILE *f = fopen(input, mode);
    if (f == NULL) exit(EXIT_FAILURE);
    return f;    
}

int is_list_empty(Queue *q)
{
    return q == NULL;
}

void format_url(char *string, char *url_, float *vid_l)
{
    int string_len = strlen(string);
    int space_i = 0;
    int exp = 0;
    int total = 0;

    for (int i = 0; i < string_len; i++)
    {
        if (string[i] == ' ')
            space_i = i;
    }

    for (int i = 0; i < space_i; i++)
        url_[i] = string[i];
    
    for (int i = string_len - 2; i > space_i; i--)
    {
        total += (string[i] - 48) * pow(10, exp);
        exp++;
    }

    *vid_l = (float)total;
}

Queue *insert_queue(Queue *q, char *url, float *vid_l, int *queue_size)
{
    if (is_list_empty(q))
    {
        q = (Queue*)malloc(sizeof(Queue));
        if (q == NULL) exit(1);

        strcpy(q->url, url);
        q->next = NULL;
        q->video_length = *vid_l;

        ++(*queue_size);

        return q;
    }

    Queue *new = (Queue*)malloc(sizeof(Queue));
    if (new == NULL) exit(1);

    strcpy(new->url, url);
    new->next = NULL;
    new->video_length = *vid_l;

    Queue *aux = q;

    while (aux->next != NULL) aux = aux->next;

    aux->next = new;

    ++(*queue_size);

    return q;
}

void print_queue(Queue *q)
{
    for (Queue *aux = q; aux->next != NULL; aux = aux->next)
        printf("%s %.2f\n", aux->url, aux->video_length);
}

void free_queue(Queue *q)
{
    for (Queue *aux = q; aux != NULL;)
    {
        Queue *f = aux->next;
        free(aux);
        aux = f;
    }
}

void run_queue(Queue *q, int queue_size, FILE *state)
{
    Queue *aux = q;
    for (int i = 0; i < queue_size; i++)
    {
        char message[] = "video playing...";
        fprintf(state, "%s", message);

        char command[SIZE];

        #ifdef _WIN32
        strcat(strcpy(command, "start "), aux->url);
        #elif __linux__
        strcat(strcpy(command, "xdg-open "), aux->url);
        #else
        strcpy(command, "Error, OS not recognised!");
        #endif

        system(command);

        sleep(5);

        SetCursorPos(1920/2, 1080/2);

        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		sleep(0.1);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

        printf("Video %d: %s - Video length: %.2f\n", i+1, aux->url, aux->video_length);

        sleep(aux->video_length * 60);
        aux = aux->next;
        system("taskkill /F /IM chrome.exe /T");
    }
}

int main(void)
{
    POINT mouse;

    Queue *q = init_queue();

    int queue_size = 0;

    char s[SIZE];
    char url[SIZE];
    float vid_l = 0;
    
    FILE *input = open_file("input.txt", "r");
    while(!feof(input))
    {
        fgets(s, SIZE, input);
        format_url(s, url, &vid_l);
        q = insert_queue(q, url, &vid_l, &queue_size);
    }
    fclose(input);

    FILE *state = open_file("state.txt", "w");
    print_queue(q);
    run_queue(q, queue_size, state);
    free_queue(q);
    fclose(state);
    return 0;
}