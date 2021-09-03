#include "conv.h"
void convolution_hw(int i_fmap[Tn][I_ROWS][I_COLS],
                 int wght[Tn][Tm][KH][KW],
                 int o_fmap[Tm][O_ROWS][O_COLS])

{

//Convolution
KH_Loop:
  for (int i = 0; i < KH; i++)
  {
  KW_Loop:
    for (int j = 0; j < KW; j++)
    {

    R_Loop:
      for (int r = 0; r < O_ROWS; r++)
      {
      C_Loop:
        for (int c = 0; c < O_COLS; c++)

        {
#pragma HLS Pipeline II = 1
        M_Loop:
          for (int m = 0; m < Tm; m++)

          {
#pragma HLS Unroll
            int sum = 0;

          N_Loop:
            for (int n = 0; n < Tn; n++)
            {
#pragma HLS Unroll
              sum += i_fmap[n][i + r * Stride][j + c * Stride] * wght[n][m][i][j];
            }

            o_fmap[m][r][c] += sum;
          }
        }
      }
    }
  }
}
