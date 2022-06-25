#include "conv.h"

//----on-Chip buffers------//
static int ib[Tn][I_ROWS][I_COLS];
static int wb[Tn][Tm][KH][KW];
static int ob[Tm][O_ROWS][O_COLS];

void conv_top_hw(int i_fmap[Tn][I_ROWS][I_COLS],
          int wght[Tn][Tm][KH][KW],
          int o_fmap[Tm][O_ROWS][O_COLS])

{

//HLS pragmas for Buffer parition
#pragma HLS ARRAY_PARTITION variable = ib complete dim = 1
#pragma HLS ARRAY_PARTITION variable = wb complete dim = 1
#pragma HLS ARRAY_PARTITION variable = wb cyclic factor = 48 dim = 2
#pragma HLS ARRAY_PARTITION variable = ob complete dim = 1

//HLS pragma for sub-fucntion inline
#pragma HLS inline recursive

    //Convolution
    //Filter Loop
M_loop:
    for (int tm = 0; tm < M; tm += Tm)
    {
    //Channle Loop
    N_loop:
        for (int tn = 0; tn < N; tn += Tn)
        {
            //load feature maps from DDR to FPGA on-chip memory
            read_if_maps(i_fmap);
            //load weights from DDR to FPGA on-chip memory
            read_wghts(wght);
            //convolution hardware function
            convolution_hw(ib, wb, ob);
        }
        //read outputs from FPGA memory to DDR
        write_ofmaps(o_fmap);
        //printf("%d\n",ob[0][0][0]);
    }
}
//Load feature maps function
void read_if_maps(int i_fmap[Tn][I_ROWS][I_COLS])
{
#pragma HLS inline recursive
//copy input feature maps to input buffer
load_ifmap_0:
    for (int i = 0; i < Tn; i++)
    {
    load_ifmap_1:
        for (int j = 0; j < I_ROWS; j++)
        {
        load_ifmap_2:
            for (int k = 0; k < I_COLS; k++)
            {
#pragma HLS Pipeline II = 1
                ib[i][j][k] = i_fmap[i][j][k];
            }
        }
    }
}
//Load weights function
void read_wghts(int wght[Tn][Tm][KH][KW])
{
#pragma HLS inline recursive
//copy weights to weight buffer
load_wb_1:
    for (int l = 0; l < Tn; l++)
    {
    load_wb_2:
        for (int m = 0; m < Tm; m++)
        {
        load_wb_3:
            for (int n = 0; n < KH; n++)
            {

            load_wb_4:
                for (int o = 0; o < KW; o++)
                {
#pragma HLS Pipeline II = 1
                    wb[l][m][n][o] = wght[l][m][n][o];
                }
            }
        }
    }
}
//Store output feature maps
void write_ofmaps(int o_fmap[Tm][O_ROWS][O_COLS])

{
#pragma HLS inline recursive
store_ob_1:
    for (int i = 0; i < Tm; i++)
    {
    store_ob_2:
        for (int j = 0; j < O_ROWS; j++)
        {
        store_ob_3:
            for (int k = 0; k < O_COLS; k++)
            {
#pragma HLS Pipeline II = 1
                o_fmap[i][j][k] = ob[i][j][k]; 
                ob[i][j][k] = 0;
            }
        }
    }
}
