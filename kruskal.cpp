#define _CRT_SECURE_NO_WARNINGS //prototypes.h begin (for me)
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "windows.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int size_counter(char *PATH_O);
void file_open(int *A, char *PATH_O, int Size);
void file_save(int *B, int Size);
void clr_init(int *Clr, int Size);
void matrix_print(int *Matr, int Size);
void array_print(int *Arr, int Size);
void weight_matrix(int *Matr, int *W, int *V, int *H, int Size);
void spanning_tree(int *Matr, int *W, int *V, int *H, int *Clr, int n, int Size);
void bubbleSort(int *W, int *V, int *H, int Size);
void append(int *W, int *V, int *H, int ch1, int ch2, int ch3); //prototypes.h end (for me)

#define ARROUT
#define INPUT

#define PATH "matrixes/graph_"
#define EXT ".txt"
#define PATH_S "matrixes/spanning_tree.txt"

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
    scanf("%d %d", &number, &format);
#endif

    char path[30];

    snprintf(path, sizeof(path), "%s%ld%s", PATH, number, EXT);

    char *PATH_O = path;
    int tree_size = size_counter(PATH_O);

    int *A = NULL;
    int *B = NULL;
    int *Weight = NULL;
    int *Vert = NULL;
    int *Horz = NULL;
    int *Color = NULL;

    A = (int*)malloc(tree_size * tree_size * sizeof(int));
    B = (int*)malloc(tree_size * tree_size * sizeof(int));

    Weight = (int*)malloc((tree_size * tree_size - tree_size)/2 * sizeof(int));
    Vert = (int*)malloc((tree_size * tree_size - tree_size)/2 * sizeof(int));
    Horz = (int*)malloc((tree_size * tree_size - tree_size)/2 * sizeof(int));
    Color = (int*)malloc(tree_size * sizeof(int));

    file_open(A, PATH_O, tree_size);
    clr_init(Color, tree_size);
    weight_matrix(A, Weight, Vert, Horz, tree_size);

    if ((format == 1) || (format == 3))
        matrix_print(A, tree_size);

#ifndef ARROUT
    array_print(Weight, (tree_size * tree_size - tree_size)/2);
    array_print(Vert, (tree_size * tree_size - tree_size)/2);
    array_print(Horz, (tree_size * tree_size - tree_size)/2);
#endif

    bubbleSort(Weight, Vert, Horz, (tree_size * tree_size - tree_size)/2);

#ifndef ARROUT
    array_print(Weight, (tree_size * tree_size - tree_size)/2);
    array_print(Vert, (tree_size * tree_size - tree_size)/2);
    array_print(Horz, (tree_size * tree_size - tree_size)/2);
#endif

    spanning_tree(B, Weight, Vert, Horz, Color, (tree_size * tree_size - tree_size)/2, tree_size);

    free(A);
    free(Weight);
    free(Vert);
    free(Horz);
    free(Color);

    if ((format == 1) || (format == 3)) {
        matrix_print(B, tree_size);

        getchar(); getchar();
    }

    if ((format == 2) || (format == 3)) {
        system("cls");
        file_save(B, tree_size);
        printf("The adjacency matrix of the input tree was loaded from %s\nSpanning tree is preparing...\n", PATH_O);

        Sleep(1000);

        printf("The file with the adjacency matrix of the spanning tree was saved on path:\n%s\n", PATH_S);

        getchar();
    }

    free(B);

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

    return ((int)sqrt(counter));
}

void file_open(int *A, char *PATH_O, int Size) {
    FILE *f;

    f = fopen(PATH_O, "r");

    for (int i=0; i<Size; i++)
        for (int k=0; k<Size; k++)
            fscanf(f, "%d", (A + i*Size + k));

    fclose(f);
}

void file_save(int *B, int Size) {
    FILE *f;

    f = fopen(PATH_S, "w");

    for (int i=0; i<Size; i++) {
        for (int k=0; k<Size; k++)
            fprintf(f, "%d ", *(B + i*Size + k));

        fprintf(f, "\n");
    }

    fclose(f);
}

void matrix_print(int *Matr, int Size) {
    for (int i=0; i<Size; i++) {
        for (int k=0; k<Size; k++)
            printf("%d ", *(Matr + i*Size + k));

        printf("\n");
    }

    printf("\n");
}

void array_print(int *Arr, int Size) {
    for (int i=0; i<Size; i++)
        printf("%d ", Arr[i]);

    printf("\n");
}

void clr_init(int *Clr, int Size) {
    for (int i=0; i<Size; i++)
        Clr[i] = i;
}

void weight_matrix(int *Matr, int *W, int *V, int *H, int Size) {
    for (int i=0; i<Size; i++)
        for (int k=0; k<Size; k++)
            if ((i < k) && ((Matr + i*Size + k) != 0))
                append(W, V, H, *(Matr + i*Size + k), i, k);
}

void spanning_tree(int *Matr, int *W, int *V, int *H, int *Clr, int n, int Size) {
    int k = 0;

    for (int i=0; i<Size; i++)
        for (int h=0; h<Size; h++)
            *(Matr + i*Size + h) = 0;

    for (int i=0; i<n; i++) {
        if ((W[i] != 0) && (k < 1)) {
            Clr[V[i]] = Clr[H[i]] = -Size;

            *(Matr + V[i]*Size + H[i]) = W[i];
            *(Matr + H[i]*Size + V[i]) = W[i];

            k++;
        }

        else if ((W[i] != 0) && (k >= 1))
            if (Clr[V[i]] != Clr[H[i]]) {
                if (Clr[V[i]] * Clr[H[i]] == 0)
                    Clr[V[i]] = Clr[H[i]] = -MAX(abs(Clr[V[i]]),abs(Clr[H[i]]));

                else if ((Clr[V[i]] > 0) && (Clr[H[i]] > 0))
                    Clr[V[i]] = Clr[H[i]] = -MIN(V[i],H[i]);

                else if (Clr[V[i]] * Clr[H[i]] < 0)
                    Clr[V[i]] = Clr[H[i]] = MIN(Clr[V[i]],Clr[H[i]]);

                else if ((Clr[V[i]] < 0) && (Clr[H[i]] < 0)) {
                    int minhash = MIN(Clr[V[i]],Clr[H[i]]);
                    int maxhash = MAX(Clr[V[i]],Clr[H[i]]);

                    for (int j=0; j<n; j++)
                        if (Clr[j] == maxhash)
                            Clr[j] = minhash;
                }

                *(Matr + V[i]*Size + H[i]) = W[i];
                *(Matr + H[i]*Size + V[i]) = W[i];
            }
    }
}

void bubbleSort(int *W, int *V, int *H, int Size) {
    for (int i=0; i<(Size-1); i++)
        for (int j=0; j<(Size-i-1); j++)
            if (W[j] > W[j+1]) {
                int varW = W[j];
                int varV = V[j];
                int varH = H[j];

                W[j] = W[j+1];
                W[j+1] = varW;

                V[j] = V[j+1];
                V[j+1] = varV;

                H[j] = H[j+1];
                H[j+1] = varH;
            }
}

void append(int *W, int *V, int *H, int ch1, int ch2, int ch3) {
    static int j = 0;

    W[j] = ch1;
    V[j] = ch2;
    H[j] = ch3;
    j++;
}
