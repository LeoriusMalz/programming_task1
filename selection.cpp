#define _CRT_SECURE_NO_WARNINGS //prototypes.h begin (for me)
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "windows.h"

int size_counter(char *PATH_O);
void file_open(int *S, char *PATH_O, int Size);
void file_save(int *A, int Size);
void matrix_init(int *Matr, int Size);
void matrix_print(int *Matr, int Size);
void array_init(int *Arr, int Size);
void array_print(int *Arr, int Size);
void sheet_split(int *S, int *B, int *E, int Size);
void asp(int *A, int *B, int *E, int Size);
void bubbleSort(int *B, int *E, int Size);
void append_A(int *A, int a0, int a1, int Size); //prototypes.h end (for me)

#define ARROUT
#define INPUT

#define PATH "matrixes/app_"
#define EXT ".txt"
#define PATH_S "matrixes/max_app.txt"

int main(int argc, char **argv) {
    long int number = 0;
    long int format = 0;
    char *p;

#ifdef INPUT
    if (argc != 3) {
        printf("Error: %d argument received, 2 was expected.\n", argc-1);

        getchar();
        return 0;
    }

    number = strtol(argv[1], &p, 10);
    format = strtol(argv[2], &p, 10);
#endif

#ifndef INPUT
    scanf("%ld %ld", &number, &format);
#endif

    char path[30];

    snprintf(path, sizeof(path), "%s%ld%s", PATH, number, EXT);

    char *PATH_O = path;
    int sheet_size = size_counter(PATH_O);

    int *Beg = NULL;
    int *End = NULL;
    int *Sheet = NULL;
    int *Ans = NULL;

    Sheet = (int*)malloc(2 * sheet_size * sizeof(int));
    Ans = (int*)malloc(2 * sheet_size * sizeof(int));

    Beg = (int*)malloc(sheet_size * sizeof(int));
    End = (int*)malloc(sheet_size * sizeof(int));

    matrix_init(Sheet, sheet_size);
    matrix_init(Ans, sheet_size);

    file_open(Sheet, PATH_O, sheet_size);

    if ((format == 1) || (format == 3))
        matrix_print(Sheet, sheet_size);

    sheet_split(Sheet, Beg, End, sheet_size);

#ifndef ARROUT
    array_print(Beg, sheet_size);
    array_print(End, sheet_size);
#endif

    bubbleSort(Beg, End, sheet_size);

#ifndef ARROUT
    array_print(Beg, sheet_size);
    array_print(End, sheet_size);
#endif

    asp(Ans, Beg, End, sheet_size);

    free(Sheet);
    free(Beg);
    free(End);

    if ((format == 1) || (format == 3)) {
        matrix_print(Ans, sheet_size);

        getchar(); getchar();
    }

    if ((format == 2) || (format == 3)) {
        system("cls");
        file_save(Ans, sheet_size);
        printf("The list of applications was loaded from %s\nMaximum number of app. is preparing...\n", PATH_O);

        Sleep(1000);

        printf("File with the maximum number of applications was saved on path:\n%s\n", PATH_S);

        getchar();
    }

    free(Ans);

    return 0;
}

int size_counter(char *PATH_O) {
    int counter = 0;
    FILE *f;

    f = fopen(PATH_O, "r");

    fseek(f, 0, SEEK_SET);

    while (true) {
        int value = 0;

        if (fscanf(f, "%d", &value) == 1)
            counter++;

        if (feof(f))
            break;
    }

    fseek(f, 0, SEEK_SET);


    fclose(f);

    return ((int)counter/2);
}

void file_open(int *S, char *PATH_O, int Size) {
    FILE *f;

    f = fopen(PATH_O, "r");

    for (int i=0; i<Size; i++)
        for (int k=0; k<2; k++)
            fscanf(f, "%d", (S + i*Size + k));

    fclose(f);
}

void file_save(int *A, int Size) {
    FILE *f;
    int i = 0;

    f = fopen(PATH_S, "w");

    while (((*(A + i*Size + 0) != 0) || (*(A + i*Size + 1) != 0)) && (i<Size)) {
        fprintf(f, "%d %d\n", *(A + i*Size + 0), *(A + i*Size + 1));
        i++;
    }

    fclose(f);
}

void matrix_init(int *Matr, int Size) {
    for (int i=0; i<Size; i++)
        for (int k=0; k<2; k++)
            *(Matr + i*Size + k) = 0;
}

void matrix_print(int *Matr, int Size) {
    int i = 0;

    printf("\n");

    while (((*(Matr + i*Size + 0) != 0) || (*(Matr + i*Size + 1) != 0)) && (i<Size)) {
        printf("%d %d\n", *(Matr + i*Size + 0), *(Matr + i*Size + 1));
        i++;
    }

    printf("\n");
}

void array_print(int *Arr, int Size) {
    for (int i=0; i<Size; i++)
        printf("%d ", Arr[i]);

    printf("\n");
}

void sheet_split(int *S, int *B, int *E, int Size) {
    int mem0 = *(S + (Size-1)*Size + 0);
    int mem1 = *(S + (Size-1)*Size + 1);

    for (int i=0; i<Size; i++) {
        B[i] = *(S + i*Size + 0);
        E[i] = *(S + i*Size + 1);
    }

    B[Size-1] = mem0;
    E[Size-1] = mem1;
}

void asp(int *A, int *B, int *E, int Size) {
    int h = 1;
    int mem = 0;
    int p = 1;

    append_A(A, B[0], E[0], Size);

    while (h < Size) {
        if (B[h] >= E[mem]) {
            append_A(A, B[h], E[h], Size);
            mem = h;
            p++;
        }

        h++;
    }

    for (int k=p; k<Size; k++) {
        *(A + k*Size + 0) = 0;
        *(A + k*Size + 1) = 0;
    }
}

void bubbleSort(int *B, int *E, int Size) {
    for (int i=0; i<(Size-1); i++)
        for (int j=0; j<(Size-i-1); j++)
            if (E[j] > E[j+1]) {
                int varE = E[j];
                int varB = B[j];

                E[j] = E[j+1];
                E[j+1] = varE;

                B[j] = B[j+1];
                B[j+1] = varB;
            }
}

void append_A(int *A, int a0, int a1, int Size) {
    static int o = 0;

    *(A + o*Size + 0) = a0;
    *(A + o*Size + 1) = a1;

    o++;
}
