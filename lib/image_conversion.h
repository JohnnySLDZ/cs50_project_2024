#ifndef IMAGE_CONVERSION
#define IMAGE_CONVERSION

unsigned char *load_image(const char *in, int *width, int *height, int *channels);
unsigned char *load_webp(const char *input_file, int *width, int *height);
void write_file(const char *ext, unsigned char *image, const char *out, int width, int height, int channels);
int to_jpg(unsigned char *in, const char *out, int width, int height, int channels);
int to_bmp(unsigned char *in, const char *out, int width, int height, int channels);
int to_webp(unsigned char * image_data, const char *output_file, int width, int height, int channels);

#endif
