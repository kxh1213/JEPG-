#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

unsigned char header[];

void createBMP(char *rFileName, char *gFileName, char *bFileName, char *dimFileName, char *bmpFileName) {
    FILE *rFile = fopen(rFileName, "r");
    FILE *gFile = fopen(gFileName, "r");
    FILE *bFile = fopen(bFileName, "r");
    FILE *dimFile = fopen(dimFileName, "r");
    FILE *bmpFile = fopen(bmpFileName, "wb");
}

int main(int argc, char *argv[]){
    int R,G,B;
    int width;
    int height;


    if(argc<4){
        fprintf(stderr, "%s", argv[0]);
        return 1;
    }

    FILE *bmpFile ;
    FILE *rFile;
    FILE *gFile;
    FILE *bFile;
    FILE *dimFile;

    return 0;
}