#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdio.h>
#include <webp/decode.h>
#include <webp/encode.h>
#include "image_conversion.h"

// Function to write the image file based on the desired output extension
void write_file(const char *ext, unsigned char *image, const char *out, int width, int height, int channels)
{
    if (strcmp(ext, ".bmp") == 0)
    {
        to_bmp(image, out, width, height, channels);
    }
    else if (strcmp(ext, ".jpg") == 0)
    {
        to_jpg(image, out, width, height, channels);
    }
    else if (strcmp(ext, ".webp") == 0)
    {
        to_webp(image, out, width, height, channels);
    }
    else
    {
        printf("Invalid extension for output file.\n");
    }
}

// Function to load an image from a file
unsigned char *load_image(const char *in, int *width, int *height, int *channels)
{
    // Load the image file using stb_image
    unsigned char *img = stbi_load(in, width, height, channels, 0);
    if (img == NULL) {
        printf("Error: Failed to load image file: %s\n", in);
        return NULL;
    }
    return img;
}

// Function to load a WebP image file
unsigned char *load_webp(const char *input_file, int *width, int *height)
{
    FILE *file = fopen(input_file, "rb");
    if (!file) {
        printf("Error: Failed to open WebP file: %s\n", input_file);
        return NULL;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for WebP data
    unsigned char *webp_data = (unsigned char *)malloc(file_size);
    if (!webp_data) {
        printf("Error: Failed to allocate memory for WebP data.\n");
        fclose(file);
        return NULL;
    }
    // Read the WebP file into memory
    fread(webp_data, file_size, 1, file);
    fclose(file);

    // Decode the WebP image into RGBA format
    unsigned char *decoded_data = WebPDecodeRGBA(webp_data, file_size, width, height);
    free(webp_data);
    if (!decoded_data) {
        printf("Error: Failed to decode WebP image.\n");
        return NULL;
    }
    return decoded_data;
}

// Function to save an image in JPG format
int to_jpg(unsigned char *in, const char *out, int width, int height, int channels)
{
    // Save as JPG
    if (!stbi_write_jpg(out, width, height, channels, in, 90)) {
        printf("Error: Failed to write JPG image: %s\n", out);
        stbi_image_free(in);
        return 2;
    }
    stbi_image_free(in);
    printf("Conversion to JPG complete: %s\n", out);
    return 0;
}

// Function to save an image in BMP format
int to_bmp(unsigned char *in, const char *out, int width, int height, int channels)
{
    // Save as BMP
    if (!stbi_write_bmp(out, width, height, channels, in)) {
        printf("Error: Failed to write BMP image: %s\n", out);
        stbi_image_free(in);
        return 2;
    }

    stbi_image_free(in);
    printf("Conversion to BMP complete: %s\n", out);
    return 0;
}

// Function to save an image in WebP format
int to_webp(unsigned char * image_data, const char *output_file, int width, int height, int channels)
{
    uint8_t *webp_data = NULL;
    size_t webp_size;

    // Encode the image into WebP format based on channel count
    if (channels == 3) {
        webp_size = WebPEncodeRGB(image_data, width, height, width * channels, 75, &webp_data);
    } else if (channels == 4) {
        webp_size = WebPEncodeRGBA(image_data, width, height, width * channels, 75, &webp_data);
    } else {
        printf("Error: Unsupported channel count for WebP encoding: %d\n", channels);
        stbi_image_free(image_data);
        return 1;
    }

    if (webp_size == 0) {
        printf("Error: Failed to encode WebP image.\n");
        stbi_image_free(image_data);
        return 1;
    }

    // Write the WebP data to an output file
    FILE *output = fopen(output_file, "wb");
    if (!output) {
        printf("Error: Failed to open output file for WebP: %s\n", output_file);
        WebPFree(webp_data);
        stbi_image_free(image_data);
        return 1;
    }
    fwrite(webp_data, webp_size, 1, output);
    fclose(output);

    printf("Successfully saved WebP image: %s\n", output_file);
    WebPFree(webp_data);
    stbi_image_free(image_data);

    return 0;
}
