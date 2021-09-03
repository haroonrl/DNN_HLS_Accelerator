#ifndef _conv_
#define _conv_
//following parameters are for Alexnet layer #1
#define I_ROWS 227 //input feature map width
#define I_COLS 227 //input feature map height
#define O_ROWS 55  //output feature map height
#define O_COLS 55  //output feature map width
#define Stride 4   //Stride
#define Pad 0      //padding
#define Tm 48      //Filter Tile factor (output filter parallelsim factor)
#define Tn 1       //Channel Tile factor (input channel parallelsim factor)
#define N 3        //Num of channels
#define M 96       //Num of filter
#define KW 11      //Filter width
#define KH 11      //Filter height

//Top convoltion function
void conv_top_hw(int i_fmap[Tn][I_ROWS][I_COLS],
          int weight[Tn][Tm][KH][KW],
          int o_fmap[Tm][O_ROWS][O_COLS]);

//Compute convolution fucntion
void convolution_hw(int i_fmap[Tn][I_ROWS][I_COLS],
                 int wght[Tn][Tm][KH][KW],
                 int o_fmap[Tm][O_ROWS][O_COLS]);

//input feature maps load fucntion
void read_if_maps(int i_fmap[Tn][I_ROWS][I_COLS]);

//input weights load function
void read_wghts(int wght[Tn][Tm][KH][KW]);

//store ouput feature maps
void write_ofmaps(int o_fmap[Tm][O_ROWS][O_COLS]);

#endif
