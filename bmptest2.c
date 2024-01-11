#include <stdio.h>
#include <stdlib.h>

#define IMAGE_WIDTH 1280  // 720p resolution
#define IMAGE_HEIGHT 720

#pragma pack(push, 1)
typedef struct {
    unsigned char signature[2];
    unsigned int fileSize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int dataOffset;
    unsigned int headerSize;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    int xPixelsPerMeter;
    int yPixelsPerMeter;
    unsigned int totalColors;
    unsigned int importantColors;
} BMPHeader;
#pragma pack(pop)

void stitchImages(const char* file1, const char* file2, const char* file3, const char* file4) {
    FILE *image1, *image2, *image3, *image4, *finalImage;
    unsigned char *buffer1, *buffer2, *buffer3, *buffer4;
    
    // Open the input image files
    image1 = fopen(file1, "rb");
    image2 = fopen(file2, "rb");
    image3 = fopen(file3, "rb");
    image4 = fopen(file4, "rb");
    
    if (!image1 || !image2 || !image3 || !image4) {
        printf("Error opening input image files.\n");
        return;
    }
    
    // Calculate the dimensions of the final image
    int finalWidth = 2 * IMAGE_WIDTH;
    int finalHeight = 2 * IMAGE_HEIGHT;
    
    // Allocate memory for pixel buffers
    buffer1 = (unsigned char*)malloc(3 * IMAGE_WIDTH * IMAGE_HEIGHT);
    buffer2 = (unsigned char*)malloc(3 * IMAGE_WIDTH * IMAGE_HEIGHT);
    buffer3 = (unsigned char*)malloc(3 * IMAGE_WIDTH * IMAGE_HEIGHT);
    buffer4 = (unsigned char*)malloc(3 * IMAGE_WIDTH * IMAGE_HEIGHT);
    
    // Read pixel data from input images
    BMPHeader bmp1,bmp2,bmp3,bmp4;
    fread(&bmp1, sizeof(BMPHeader), 1, image1);
    fseek(image1, bmp1.dataOffset, SEEK_SET);
    fread(&bmp2, sizeof(BMPHeader), 1, image2);
    fseek(image2, bmp2.dataOffset, SEEK_SET);
    fread(&bmp3, sizeof(BMPHeader), 1, image3);
    fseek(image3, bmp3.dataOffset, SEEK_SET);
    fread(&bmp4, sizeof(BMPHeader), 1, image4);
    fseek(image4, bmp4.dataOffset, SEEK_SET);
    fread(buffer1, sizeof(unsigned char), 3 * IMAGE_WIDTH * IMAGE_HEIGHT, image1);
    fread(buffer2, sizeof(unsigned char), 3 * IMAGE_WIDTH * IMAGE_HEIGHT, image2);
    fread(buffer3, sizeof(unsigned char), 3 * IMAGE_WIDTH * IMAGE_HEIGHT, image3);
    fread(buffer4, sizeof(unsigned char), 3 * IMAGE_WIDTH * IMAGE_HEIGHT, image4);
    
    // Close input image files
    fclose(image1);
    fclose(image2);
    fclose(image3);
    fclose(image4);
    
    // Create the final image file
    finalImage = fopen("final_image.bmp", "wb");
    
    // BMP header information
    BMPHeader header;
    header.signature[0] = 'B';
    header.signature[1] = 'M';
    header.fileSize = 54 + (3 * finalWidth * finalHeight);
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.dataOffset = 54;
    header.headerSize = 40;
    header.width = finalWidth;
    header.height = finalHeight;
    header.planes = 1;
    header.bitsPerPixel = 24;
    header.compression = 0;
    header.imageSize = 3 * finalWidth * finalHeight;
    header.xPixelsPerMeter = 0;
    header.yPixelsPerMeter = 0;
    header.totalColors = 0;
    header.importantColors = 0;
    
    // Write BMP header to the final image file
    fwrite(&header, sizeof(BMPHeader), 1, finalImage);
    
    // Write pixel data to the final image file
    int row, col;
    for (row = 0; row < IMAGE_HEIGHT; row++) {
        for (col = 0; col < IMAGE_WIDTH; col++) {
            fwrite(buffer1 + row * 3 * IMAGE_WIDTH + col * 3, sizeof(unsigned char), 3, finalImage);
        }
        for (col = 0; col < IMAGE_WIDTH; col++) {
            fwrite(buffer2 + row * 3 * IMAGE_WIDTH + col * 3, sizeof(unsigned char), 3, finalImage);
        }
    }
    for (row = IMAGE_HEIGHT; row < finalHeight; row++) {
        for (col = 0; col < IMAGE_WIDTH; col++) {
            fwrite(buffer3 + (row - IMAGE_HEIGHT) * 3 * IMAGE_WIDTH + col * 3, sizeof(unsigned char), 3, finalImage);
        }
        for (col = 0; col < IMAGE_WIDTH; col++) {
            fwrite(buffer4 + (row - IMAGE_HEIGHT) * 3 * IMAGE_WIDTH + col * 3, sizeof(unsigned char), 3, finalImage);
        }
    }
    
    // Close the final image file
    fclose(finalImage);
    
    // Free memory
    free(buffer1);
    free(buffer2);
    free(buffer3);
    free(buffer4);
    
    printf("Image stitching complete. Final image saved as 'final_image.bmp'.\n");
}

int main() {
    char file1[100], file2[100], file3[100], file4[100];
    
    printf("Enter the file name for the first image: ");
    scanf("%s", file1);
    printf("Enter the file name for the second image: ");
    scanf("%s", file2);
    printf("Enter the file name for the third image: ");
    scanf("%s", file3);
    printf("Enter the file name for the fourth image: ");
    scanf("%s", file4);
    
    stitchImages(file1, file2, file3, file4);
    
    return 0;
}
