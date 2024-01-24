#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define pi 3.1415926

void usage(FILE *fp){
    fprintf(fp, "encoder 0 input.bmp R.txt G.txt B.txt\n");
    return;
}

int demo0(char *fn_bmp, char *fn_R, char *fn_G, char *fn_B, char *fn_dim){
    return 1;
}

#pragma pack(1)
typedef struct {
    char signature[2];
    int fileSize;
    int reserved;
    int dataOffset;
    int headerSize;
    int width;
    int height;
    short planes;
    short bitsPerPixel;
    int compression;
    int imageSize;
    int xPixelsPerMeter;
    int yPixelsPerMeter;
    int colorsUsed;
    int colorsImportant;
} BMPHeader;

void readheader(FILE *fp, BMPHeader *header) {
    fread(&header->signature, sizeof(header->signature), 1, fp);
    fread(&header->fileSize, sizeof(header->fileSize), 1, fp);
    fread(&header->reserved, sizeof(header->reserved), 1, fp);
    fread(&header->dataOffset, sizeof(header->dataOffset), 1, fp);
    fread(&header->headerSize, sizeof(header->headerSize), 1, fp);
    fread(&header->width, sizeof(header->width), 1, fp);
    fread(&header->height, sizeof(header->height), 1, fp);
    fread(&header->planes, sizeof(header->planes), 1, fp);
    fread(&header->bitsPerPixel, sizeof(header->bitsPerPixel), 1, fp);
    fread(&header->compression, sizeof(header->compression), 1, fp);
    fread(&header->imageSize, sizeof(header->imageSize), 1, fp);
    fread(&header->xPixelsPerMeter, sizeof(header->xPixelsPerMeter), 1, fp);
    fread(&header->yPixelsPerMeter, sizeof(header->yPixelsPerMeter), 1, fp);
    fread(&header->colorsUsed, sizeof(header->colorsUsed), 1, fp);
    fread(&header->colorsImportant, sizeof(header->colorsImportant), 1, fp);
}

typedef struct RGB{
    unsigned char R,G,B ;
} RGB;
//紅、綠、藍

typedef struct YCrCb{
    double Y,Cr,Cb ;
} YCrCb;
//亮度、紅色色度、藍色色度

void DDCT(double *input, double *output){
    for (int i=0; i<8; i++) {
        float sum = 0;
        for (int j=0; j<8; j++) {
            sum+=input[j] * cos((2*j+1) * i * pi / 16);
        }
        output[i] = sum;
    }
}
//計算1DDTC

void DDDCT(double **input, double **output) {
    double temporary[8];

    for (int i=0; i<8; i++){
        DDCT(input[i], temporary);
        for (int j=0; j<8; j++){
            output[i][j] = temporary[j];
        }
    }

    for (int j=0; j<8; j++){
        for (int i=0; i<8; i++){
            temporary[i] = output[i][j];
        }
        DDCT(temporary, output[j]);
    }
}
//計算2DDTC，函數首先在每行上應用1DDCT，然後在每列上應用1DDCT

YCrCb RGBtoYCrCb(unsigned char R, unsigned char G, unsigned char B) {
    
    YCrCb YCrCb;

    YCrCb.Y  =  0.257*R + 0.564*G + 0.098*B + 16 ;
    YCrCb.Cr =  0.439*R - 0.368*G - 0.071*B + 128;
    YCrCb.Cb = -0.148*R - 0.291*G + 0.439*B + 128;

    return YCrCb;
}
// RGB轉換為YCrCb

void extractRGB(char *bmpFileName, char *rFileName, char *gFileName, char *bFileName, char *dimFileName) {
    FILE *bmpFile = fopen(bmpFileName, "rb");
    FILE *rFile = fopen(rFileName, "w");
    FILE *gFile = fopen(gFileName, "w");
    FILE *bFile = fopen(bFileName, "w");
    FILE *dimFile = fopen(dimFileName, "w");

    if (!bmpFile || !rFile || !gFile || !bFile || !dimFile) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    BMPHeader header;
    readheader(bmpFile,&header);

    int width = header.width;
    int height = header.height;

    unsigned char *data = (unsigned char *)malloc((size_t)width * height * 3);
    fread(&header, sizeof(BMPHeader), 1, bmpFile);
    fclose(bmpFile);
    
    fprintf(dimFile, "%d %d\n", width, height);

    fseek(bmpFile, header.dataOffset, SEEK_SET);
    /*使用 fseek 將檔案指標移動到BMP檔案中像素資料的開始位置header.dataOffset 
    儲存了像素資料相對於檔案開頭的偏移量，程式可以直接設定檔案指標到該位置以讀取像素資料*/

    int padding = (4 - (width * sizeof(unsigned char) * 3) % 4) % 4; 
    /*計算padding的變數，在BMP檔案中，每一行的像素資料都以4的倍數結尾，來確保資料在內存中會對齊，
    padding表示需要在每一行結尾填充的位元數組以滿足對齊條件。*/

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char pixel[3];
            fread(&pixel, sizeof(unsigned char), 3, bmpFile);

            unsigned char B = *(data + 3 * (width * y + x) + 0);
            unsigned char G = *(data + 3 * (width * y + x) + 1);
            unsigned char R = *(data + 3 * (width * y + x) + 2);

            fprintf(rFile, "%d ", pixel[2]);
            fprintf(gFile, "%d ", pixel[1]);
            fprintf(bFile, "%d ", pixel[0]);
        }

        fseek(bmpFile, padding, SEEK_CUR);
        fprintf(rFile, "\n");
        fprintf(gFile, "\n");
        fprintf(bFile, "\n");
    }

    fclose(bmpFile);
    fclose(rFile);
    fclose(gFile);
    fclose(bFile);
    fclose(dimFile);
}
/*讀取RGB值並將其寫入對應的輸出文件中，紅色寫入rFile 綠色寫入gFile 藍色寫入bFile，
RGB值寫入文件後，使用fseek將檔案指標移動到填充的位置。
關閉所有打開的檔案（）BMP檔案及輸出文件？*/


int main(int argc, char *argv[]) {

    int option = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <bmpFileName>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    option = atoi(argv[1]);

    switch(option){
        case 0:
            demo0(argv[2], argv[3], argv[4], argv[5], argv[6]);
            break;

        case 1:

            break;
    }

    

    char *bmpFileName = argv[1];
    char rFileName[20] = "R.txt";
    char gFileName[20] = "G.txt";
    char bFileName[20] = "B.txt";
    char dimFileName[20] = "dim.txt";

    extractRGB(bmpFileName, rFileName, gFileName, bFileName, dimFileName);

    printf("Extraction completed.\n");

    return 0;

    
}