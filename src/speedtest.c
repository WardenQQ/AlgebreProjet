#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>

#include "speedtest.h"
#include "operation.h"
#include "limits.h"


static void speedtest_addition(int taille_min, int taille_max, int pas, int nb_sec);
static void speedtest_sub(int taille_min, int taille_max, int pas, int nb_sec);
static void speedtest_mult(int taille_min, int taille_max, int pas, int nb_sec);
static void speedtest_mult_scal(int taille_min, int taille_max, int pas, int nb_sec);
static void speedtest_expo(int taille_min, int taille_max, int pas, int nb_sec);
static void speedtest_transpose(int taille_min, int taille_max, int pas, int nb_sec);
static void speedtest_determinant(int taille_min, int taille_max, int pas, int nb_sec);
static void speedtest_invert(int taille_min, int taille_max, int pas, int nb_sec);
static void speedtest_solve(int taille_min, int taille_max, int pas, int nb_sec);
static void speedtest_rank(int taille_min, int taille_max, int pas, int nb_sec);

void speedtest(char *commande, int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./graph", "w");

  fprintf(file, "set xlabel \"Taille des matrices\"\n");
  fprintf(file, "set ylabel \"Temps en microsecondes\"\n");
  fprintf(file, "set terminal png size 640,480\n");
  fprintf(file, "set output \"graph.png\"\n");
  fprintf(file, "plot \"plot.dat\" with lines\n");

  if (strncmp(commande, "addition", STRING_MAX) == 0) {
    speedtest_addition(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "sub", STRING_MAX) == 0) {
    speedtest_sub(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "mult", STRING_MAX) == 0) {
    speedtest_mult(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "mult_scal", STRING_MAX) == 0) {
    speedtest_mult_scal(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "expo", STRING_MAX) == 0) {
    speedtest_expo(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "transpose", STRING_MAX) == 0) {
    speedtest_transpose(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "determinant", STRING_MAX) == 0) {
    speedtest_determinant(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "invert", STRING_MAX) == 0) {
    speedtest_invert(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "solve", STRING_MAX) == 0) {
    speedtest_solve(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "rank", STRING_MAX) == 0) { 
    speedtest_rank(taille_min, taille_max, pas, nb_sec);
  }

  switch (fork()) {
    case -1 :
      perror("fork");
      break;
    case 0 :
      execl("/bin/sh", "/bin/sh", "-c", "gnuplot ./graph", NULL);
      perror("exec");
      exit(1);
    default:
      break;
  }

  fclose(file);
}


static void speedtest_addition(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;
  
  unsigned long nb_usec = ~0uL;
  if (nb_sec > 0) {
    nb_usec = 1000000 * nb_sec;
  }
  unsigned long t = 0;

  for (int i = taille_min; i <= taille_max && t < nb_usec; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);
    Matrix B = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = addition(A, B);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %lu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(B);
    deleteMatrix(C);
  }

  fclose(file);
}


static void speedtest_sub(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long nb_usec = ~0uL;
  if (nb_sec > 0) {
    nb_usec = 1000000 * nb_sec;
  }
  unsigned long t = 0;

  for (int i = taille_min; i <= taille_max && t < nb_usec; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);
    Matrix B = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = sub(A, B);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %lu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(B);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_mult(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long nb_usec = ~0uL;
  if (nb_sec > 0) {
    nb_usec = 1000000 * nb_sec;
  }
  unsigned long t = 0;

  for (int i = taille_min; i <= taille_max && t < nb_usec; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);
    Matrix B = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = mult(A, B);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %lu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(B);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_mult_scal(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long nb_usec = ~0uL;
  if (nb_sec > 0) {
    nb_usec = 1000000 * nb_sec;
  }
  unsigned long t = 0;

  for (int i = taille_min; i <= taille_max && t < nb_usec; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = mult_scal(A,(E)i);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %lu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_expo(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long nb_usec = ~0uL;
  if (nb_sec > 0) {
    nb_usec = 1000000 * nb_sec;
  }
  unsigned long t = 0;

  for (int i = taille_min; i <= taille_max && t < nb_usec; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = expo(A,i);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %lu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_transpose(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long nb_usec = ~0uL;
  if (nb_sec > 0) {
    nb_usec = 1000000 * nb_sec;
  }
  unsigned long t = 0;

  for (int i = taille_min; i <= taille_max && t < nb_usec; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = transpose(A);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %lu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_determinant(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long nb_usec = ~0uL;
  if (nb_sec > 0) {
    nb_usec = 1000000 * nb_sec;
  }
  unsigned long t = 0;

  for (int i = taille_min; i <= taille_max && t < nb_usec; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    E det;
    determinant(A, &det);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %lu\n", i, t);

    deleteMatrix(A);
  }

  fclose(file);
}

static void speedtest_invert(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long nb_usec = ~0uL;
  if (nb_sec > 0) {
    nb_usec = 1000000 * nb_sec;
  }
  unsigned long t = 0;

  for (int i = taille_min; i <= taille_max && t < nb_usec; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = invert(A);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %lu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_solve(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long nb_usec = ~0uL;
  if (nb_sec > 0) {
    nb_usec = 1000000 * nb_sec;
  }
  unsigned long t = 0;

  for (int i = taille_min; i <= taille_max && t < nb_usec; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);
    Matrix B = aleatoire(i, 0, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = solve(A, B);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %lu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(B);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_rank(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long nb_usec = ~0uL;
  if (nb_sec > 0) {
    nb_usec = 1000000 * nb_sec;
  }
  unsigned long t = 0;

  for (int i = taille_min; i <= taille_max && t < nb_usec; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX); 

    gettimeofday(&start, NULL);
    rank(A);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %lu\n", i, t);

    deleteMatrix(A);
  }

  fclose(file);
}
