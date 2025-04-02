#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/image_conversion.h"
#include "lib/video_conversion.h"

int main(int argc, char *argv[])
{
    int option;
    char *options = "iv";
    // Define long options for command-line arguments
    struct option long_options[] = {
        {"image", no_argument, NULL, 'i'}, // Image conversion option
        {"video", no_argument, NULL, 'v'}, // Video conversion option
        {NULL, 0, NULL, 0}
    };

    // Parse the first option using getopt_long
    option = getopt_long(argc, argv, options, long_options, NULL);
    if (option == '?')
    {
        printf("Invalid option. Use --image or --video.\n");
        return 1;
    }

    // Check if additional options were provided after the first one
    if (getopt_long(argc, argv, options, long_options, NULL) != -1) {
        printf("Only one option allowed.\n");
        return 2;
    }

    // Ensure there are exactly two additional arguments for input and output files
    if (argc != optind + 2) {
        printf("Usage: ./project [--image | --video] infile outfile\n");
        return 3;
    }

    const char *infile = argv[optind]; // Input file
    const char *outfile = argv[optind + 1]; // Output file
    const char *in_ext = strrchr(infile, '.'); // Extract extension of input file
    const char *out_ext = strrchr(outfile, '.'); // Extract extension of output file

    // Check if input or output file has no extension
    if (in_ext == NULL || out_ext == NULL) {
        printf("Error: Invalid file extensions\n");
        return 3;
    }

    switch (option) {
        case 'i':
            // Validate input file extensions for images
            if (strcmp(in_ext, ".bmp") == 0 || strcmp(in_ext, ".jpg") == 0) {
                int width, height, channels;
                unsigned char *image = load_image(infile, &width, &height, &channels);
                if (!image) {
                    printf("Error: Failed to load image\n");
                    return 4;
                }
                write_file(out_ext, image, outfile, width, height, channels);
            } else if (strcmp(in_ext, ".webp") == 0) {
                int width, height;
                unsigned char *image = load_webp(infile, &width, &height);
                if (!image) {
                    printf("Error: Failed to load WebP image\n");
                    return 4;
                }
                write_file(out_ext, image, outfile, width, height, 4);
            } else {
                printf("Error: Invalid extensions for image option\n");
                return 3;
            }
            break;

        case 'v':
            is_ffmpeg_installed(); // Check if FFmpeg is installed
            convert_video(infile, outfile, in_ext, out_ext); // Perform video conversion
            break;

        default:
            printf("Error: Invalid option selected\n");
            return 1;
    }

    return 0;
}
