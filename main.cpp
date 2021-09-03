#include <stdio.h>
#include "conv.h"
#define DEBUG
int main()
{

    //Hardware on-chip buffer emulation
    static int i_fmap_hw[Tn][I_ROWS][I_COLS];
    static int weight_hw[Tn][Tm][KH][KW];
    static int o_fmap_hw[Tm][O_ROWS][O_COLS];
    //Buffers on DDR
    static int i_fmap_sw[N][I_ROWS][I_COLS];
    static int weight_sw[N][M][KH][KW];
    static int o_fmap_sw[M][O_ROWS][O_COLS];
    //Output Buffer read ouput from FPGA memory
    static int o_fmap_hw_out[M][O_ROWS][O_COLS];

    //Loop and tile variables
    int m, n, r, c, i, j, x, y, z, tm, tn;

//Display i_fmap_hw buffer
#ifdef DEBUG
    for (c = 0; c < Tn; c++)
    {
        for (i = 0; i < I_ROWS; i++)
        {
            for (j = 0; j < I_COLS; j++)
            {
                i_fmap_hw[c][i][j] = 1;

                
            }
        }
    }
#endif

#ifdef DEBUG
    for (c = 0; c < N; c++)
    {
        for (i = 0; i < I_ROWS; i++)
        {
            for (j = 0; j < I_COLS; j++)
            {

                i_fmap_sw[c][i][j] = 1;
                
            }
        }
    }
#endif

//Display weight_hw buffer
#ifdef DEBUG

    for (c = 0; c < Tn; c++)
    {
        for (m = 0; m < M; m++)
        {
            for (i = 0; i < KH; i++)
            {
                for (j = 0; j < KW; j++)
                {
                    weight_hw[c][m][i][j] = 1;

                    
                }
            }
        }
    }
#endif
//Display weight_hw buffer
#ifdef DEBUG

    for (c = 0; c < N; c++)
    {
        for (m = 0; m < M; m++)
        {
            for (i = 0; i < KH; i++)
            {
                for (j = 0; j < KW; j++)
                {

                    weight_sw[c][m][i][j] = 1;
                    
                }
            }
        }
    }
#endif

    //Convolution hardware fucntion
    printf("off-loading to CNN hardware accelerator......\n");
    for (tm = 0; tm < M; tm += Tm)
    {
        for (tn = 0; tn < N; tn += Tn)
        {

            conv_top_hw(i_fmap_hw, weight_hw, o_fmap_hw);
        }

        //store
        for (x = 0; x < Tm; x++)
        {
            for (y = 0; y < O_ROWS; y++)
            {
                for (z = 0; z < O_COLS; z++)
                {
                    o_fmap_hw_out[x + tm][y][z] = o_fmap_hw[x][y][z];
                   
                }
            }
        }
    }

    //convolutoin sw fucntion
    //computing on CPU
    for (m = 0; m < M; m++)
    {
        for (n = 0; n < N; n++)
        {
            for (r = 0; r < O_ROWS; r++)
            {
                for (c = 0; c < O_COLS; c++)
                {
                    for (i = 0; i < KH; i++)
                    {
                        for (j = 0; j < KW; j++)
                        {
                            o_fmap_sw[m][r][c] += i_fmap_sw[n][i + r * Stride][j + c * Stride] * weight_sw[n][m][i][j];
                        }
                    }
                }
            }
        }
    }

    printf("Now Comparing hardware and software Results.....\n");
    //comparer hw and sw results
    for (m = 0; m < M; m++)
    {
        for (r = 0; r < O_ROWS; r++)
        {
            for (c = 0; c < O_COLS; c++)
            {

                if (o_fmap_sw[m][r][c] != o_fmap_hw_out[m][r][c])
                {

                    printf("Simulation Failed at  o_fmap_hw [%d][%d][%d] != o_fmap_sw [%d][%d][%d] %d  and %d \n ", m, r, c, m, r, c, o_fmap_sw[m][r][c], o_fmap_hw_out[m][r][c]);

                    goto end;
                }
            }
        }
    }

    printf("Simulation Passed Successfully!!!\n");
end:

    return 0;
}
