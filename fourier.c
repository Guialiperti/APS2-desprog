#include <math.h>

#include "fourier.h"

void nft(double complex s[MAX_SIZE], double complex t[MAX_SIZE], int n, int sign) {
  for (int k = 0; k < n; k++) {
    t[k] = 0;

    for (int j = 0; j < n; j++) {
      t[k] += s[j] * cexp(sign * 2 * PI * k * j * I / n);
    }
  }
}

void nft_forward(double complex s[MAX_SIZE], double complex t[MAX_SIZE], int n) {
  nft(s, t, n, -1);
}

void nft_inverse(double complex t[MAX_SIZE], double complex s[MAX_SIZE], int n) {
  nft(t, s, n, 1);

  for (int k = 0; k < n; k++) {
    s[k] /= n;
  }
}

void fft(double complex s[MAX_SIZE], double complex t[MAX_SIZE], int n, int sign) {

  if (n == 1) { // base da recusão
    t[0] = s[0];
    return;
  }
  //Inicializa as arrays como 0
  int size = n / 2;
  double complex sp[size];
  double complex si[size];
  double complex tp[size];
  double complex ti[size];

  //divisão entre pares e impares
  int i_i = 0;
  int i_p = 0;
  for (int z = 0; z < n; z++) {
    if ((z) % 2) { //se for 0 False, !=0 True
      si[i_i] = s[z];
      i_i++;
    } else {
      sp[i_p] = s[z];
      i_p++;
    }
  }
  //Chamadas recursivas
  fft(sp, tp, size, sign);
  fft(si, ti, size, sign);

  for (int k = 0; k < size; k++) {
    t[k] = tp[k] + ti[k] * cexp(sign * 2 * PI * k * I / n);
    t[k + size] = tp[k] - ti[k] * cexp(sign * 2 * PI * k * I / n);
  }
}

void fft_forward(double complex s[MAX_SIZE], double complex t[MAX_SIZE], int n) {
  fft(s, t, n, -1);
}

void fft_inverse(double complex t[MAX_SIZE], double complex s[MAX_SIZE], int n) {
  fft(t, s, n, 1);

  for (int k = 0; k < n; k++) {
    s[k] /= n;
  }
}

void fft_forward_2d(double complex matrix[MAX_SIZE][MAX_SIZE], int width, int height) {
  //inicializando dois arrays para passar como parametros para a funcao fft_forward
  double complex row_s[width];
  double complex row_t[width];
  double complex column_s[height];
  double complex column_t[height];

  for (int j = 0; j < height; j++) {
    //preenchendo a lista com os valores da matriz
    for (int i = 0; i < width; i++) {
      row_s[i] = matrix[j][i];
    }
    //aplica a transformada nessa lista
    fft_forward(row_s, row_t, width);
    //subsititui a linha da matriz pela linha transformada
    for (int i = 0; i < width; i++) {

      matrix[j][i] = row_t[i];
    }
  }

  for (int i = 0; i < width; i++) {
    //preenche a lista com os valores da matriz
    for (int j = 0; j < height; j++) {
      column_s[j] = matrix[j][i];
    }
    //aplica a transformada na lista
    fft_forward(column_s, column_t, height);
    //substitui a coluna da matriz pela transformada
    for (int j = 0; j < height; j++) {

      matrix[j][i] = column_t[j];
    }
  }
  return;
}

void fft_inverse_2d(double complex matrix[MAX_SIZE][MAX_SIZE], int width, int height) {
  //inicializando dois arrays para passar como parametros para a funcao fft_inverse
  double complex row_s[width];
  double complex row_t[width];
  double complex column_s[height];
  double complex column_t[height];

  for (int j = 0; j < height; j++) {
    //preenchendo a lista com os valores da matriz
    for (int i = 0; i < width; i++) {
      row_s[i] = matrix[j][i];
    }
    //aplica a transformada na lista
    fft_inverse(row_s, row_t, width);
    //substitui a linha da matriz pela transformada
    for (int i = 0; i < width; i++) {

      matrix[j][i] = row_t[i];
    }
  }

  for (int i = 0; i < width; i++) {
    //preenchendo a lista com os valores da matriz
    for (int j = 0; j < height; j++) {
      column_s[j] = matrix[j][i];
    }
    //aplica a transformada na lista
    fft_inverse(column_s, column_t, height);
    //substitui a linha da matriz pela transformada
    for (int j = 0; j < height; j++) {
      matrix[j][i] = column_t[j];
    }
  }
  return;
}

void filter(double complex input[MAX_SIZE][MAX_SIZE], double complex output[MAX_SIZE][MAX_SIZE], int width, int height, int flip) {
  int center_x = width / 2;
  int center_y = height / 2;

  double variance = -2 * SIGMA * SIGMA;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int dx = center_x - (x + center_x) % width;
      int dy = center_y - (y + center_y) % height;

      double d = dx * dx + dy * dy;

      double g = exp(d / variance);

      if (flip) {
        g = 1 - g;
      }

      output[y][x] = g * input[y][x];
    }
  }
}

void filter_lp(double complex input[MAX_SIZE][MAX_SIZE], double complex output[MAX_SIZE][MAX_SIZE], int width, int height) {
  filter(input, output, width, height, 0);
}

void filter_hp(double complex input[MAX_SIZE][MAX_SIZE], double complex output[MAX_SIZE][MAX_SIZE], int width, int height) {
  filter(input, output, width, height, 1);
}
