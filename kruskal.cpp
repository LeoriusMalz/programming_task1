#define _CRT_SECURE_NO_WARNINGS //prototypes.h begin (for me)
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "windows.h"

int size_counter(char *PATH_O);
void file_open(int *A, char *PATH_O, int SIZE);
void file_save(int *B, int SIZE);
void clr_init(int *Clr, int SIZE);
void matrix_print(int *A, int SIZE);
void array_print(int *A, int SIZE);
void weight_matrix(int *A, int *W, int *V, int *H, int SIZE);
void spanning_tree(int *B, int *W, int *V, int *H, int *Clr, int n, int SIZE);
void bubbleSort(int *W, int *V, int *H, int SIZE);
void appendW(int *W, int ch);
void appendV(int *V, int ch);
void appendH(int *H, int ch);

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b)) //prototypes.h end (for me)

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

    snprintf(path, sizeof(path), "%s%d%s", PATH, number, EXT);

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

void file_open(int *A, char *PATH_O, int tree_size) {
    FILE *f;

    f = fopen(PATH_O, "r");

    for (int i=0; i<tree_size; i++)
        for (int k=0; k<tree_size; k++)
            fscanf(f, "%d", (A + i*tree_size + k));

    fclose(f);
}

void file_save(int *B, int tree_size) {
    FILE *f;

    f = fopen(PATH_S, "w");

    for (int i=0; i<tree_size; i++) {
        for (int k=0; k<tree_size; k++)
            fprintf(f, "%d ", *(B + i*tree_size + k));

        fprintf(f, "\n");
    }

    fclose(f);
}

void matrix_print(int *A, int tree_size) {
    for (int i=0; i<tree_size; i++) {
        for (int k=0; k<tree_size; k++)
            printf("%d ", *(A + i*tree_size + k));

        printf("\n");
    }

    printf("\n");
}

void array_print(int *A, int tree_size) {
    for (int i=0; i<tree_size; i++)
        printf("%d ", A[i]);

    printf("\n");
}

void clr_init(int *Clr, int tree_size) {
    for (int i=0; i<tree_size; i++)
        Clr[i] = i;
}

void weight_matrix(int *A, int *W, int *V, int *H, int tree_size) {
    for (int i=0; i<tree_size; i++)
        for (int k=0; k<tree_size; k++)
            if ((i < k) && ((A + i*tree_size + k) != 0)) {
                appendW(W, *(A + i*tree_size + k));
                appendV(V, i);
                appendH(H, k);
            }
}

void spanning_tree(int *B, int *W, int *V, int *H, int *Clr, int n, int tree_size) {
    int k = 0;

    for (int i=0; i<tree_size; i++)
        for (int k=0; k<tree_size; k++)
            *(B + i*tree_size + k) = 0;

    for (int i=0; i<n; i++) {
        if ((W[i] != 0) && (k < 1)) {
            Clr[V[i]] = Clr[H[i]] = -tree_size;

            *(B + V[i]*tree_size + H[i]) = W[i];
            *(B + H[i]*tree_size + V[i]) = W[i];

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

                *(B + V[i]*tree_size + H[i]) = W[i];
                *(B + H[i]*tree_size + V[i]) = W[i];
            }
    }
}

void bubbleSort(int *W, int *V, int *H, int tree_size) {
    for (int i=0; i<(tree_size-1); i++)
        for (int j=0; j<(tree_size-i-1); j++)
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

void appendW(int *W, int ch) {
    static int j = 0;

    W[j] = ch;
    j++;
}

void appendV(int *V, int ch) {
    static int j = 0;

    V[j] = ch;
    j++;
}

void appendH(int *H, int ch) {
    static int j = 0;

    H[j] = ch;
    j++;
}
