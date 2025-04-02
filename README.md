# Image and Video Converter
#### Video Demo:  <https://youtu.be/cuMBjPN6XMk>
### Used Libraries:
- [Getopt](https://www.gnu.org/software/libc/manual/html_node/Getopt.html)
- [STB](https://github.com/nothings/stb/tree/master)
- [Webp Codec](https://github.com/webmproject/libwebp/tree/main)
- [FFmeg](https://www.ffmpeg.org/)
#### Description:

The project is a converter for images and videos, designed to convert files from one format to another within their respective categories. The motivation for this project arises from my university experience, where I have frequently needed to prepare slides and videos for various academic projects. Often, when sourcing images from the internet or extracting videos using screen recording or editing applications, I encounter file formats that I am not accustomed to working with or that are incompatible with other tools I use. This challenge inspired the creation of a versatile file conversion program.

The project is a command-line-based program written in C. To use the program, the user executes it with the command `./project`, followed by the option `--image` to indicate an image conversion or `--video` for video conversion. Next, the input and output file names, separated by a space, must be provided. Each file name must include its extension, and if the file is located in a different folder from the project, the file path must be specified, such as `images/input.bmp`. The program verifies the validity of the file extensions based on the selected option and ensures compatibility with the supported formats. Currently, the program supports six of the most common file extensions for these types of media: **JPG**, **BMP**, and **WEBP** for images, and **MP4**, **AVI**, and **WEBM** for videos. For example, the commands `./project --image images/input.jpg output.bmp` or `./project --video videos/input.webm output.mp4` demonstrate typical usage of the program.

The project is organized into several components: the main file project3.c, a Makefile to automate code compilation, a lib folder for header files and complementary .c files, an images folder containing sample images for testing, and a videos folder for video testing purposes.

Within the lib folder, the following files are included:

- `stb_image.h` and `stb_image_write.h`: These are header files from the STB library, used for image handling.
- `image_conversion.h`: This file contains the function prototypes for image conversion.
- `image_conversion.c`: This file defines the actual functions used for image conversion.
- `video_conversion.h`: This file contains the function prototypes for video conversion.
- `video_conversion.c`: This file defines the actual functions used for video conversion.

The logic behind each file, the libraries used, and the design decisions will now be explained in detail.

### Main File: project3.c

The main file uses the command-line processing library getopt to handle the options provided as command-line arguments. Specifically, the program uses the `getopt_long()` function, which allows for more readable multi-character options by prefixing them with a double dash `--`. After processing the options, the file names provided in the **argv** array are stored in variables, and their extensions are extracted using the `strrchr()` function, which is part of the standard C library string.h. The program then verifies the presence of extensions for both files and uses a conditional switch structure to validate the user-provided option. Depending on the extensions of the input and output files, the necessary functions are invoked to handle the conversion.

### Image Conversion

For image conversion, the initial idea was to use a similar approach to the one used in the problema set 4 example, **Filter** (where we manipulated raw image data by extracting bits of real information, stripping metadata), and then rearranging the data in a new format. This approach was relatively straightforward for BMP uncompressed images. However, when dealing with other image formats, the data is often compressed or encoded, making it more practical to use third-party libraries for encoding and decoding.

After researching suitable libraries, I stumbled upon **STB**, single-file public domain (or MIT-licensed) libraries for C/C++. These libraries include `stb_image.h` for loading images and `stb_image_write.h` for writing images. While these libraries support JPG and BMP formats, they do not handle the WEBP format. Therefore, the **WebP codec** library, a library specifically designed for encoding and decoding WEBP images, was also incorporated into the project.

#### image_conversion.c

This file defines the following functions:

- `load_image()`: Uses the `stbi_load()` function from the STB library `stb_image.h` to load images in JPG or BMP formats.
- `load_webp()`: Uses the `WebPDecodeRGBA()` function from `webp/decode.h` to decode and load images in WEBP format.
- `to_bmp()` and `to_jpg()`: These functions use `stbi_write_bmp()` and `stbi_write_jpg()` from the STB library `stb_image_write.h` to write image data into their respective formats with the already loaded information.
- `to_webp()`: Utilizes `WebPEncodeRGBA()` and `WebPEncodeRGB()` from the WebP codec library `webp/encode.h` to encode images into WEBP format.
- `write_file()`: This function checks the output file’s extension and invokes the appropriate function to perform the required conversion.

### Video Conversion

For video conversion, the project employs **FFmpeg**, a comprehensive multimedia framework capable of decoding, encoding, transcoding, muxing, demuxing, streaming, filtering, and playing various media files, including videos. FFmpeg enables format conversion via command-line commands, provided the framework is installed on the system.

#### video_conversion.c

This file includes the following functions:

- `is_ffmpeg_installed()`: Checks if FFmpeg is installed on the system. If not, it prompts the user to install it. If the user declines, the program terminates; otherwise, it invokes one of the following functions, depending on the operating system of the device:
    - `install_ffmpeg_windows()`: Installs FFmpeg on Windows using terminal commands via the `system()` function.
    - `install_ffmpeg_linux()`: Installs FFmpeg on Linux in a similar manner.
- `convert_video()`: Verifies the extensions of the input and output files and executes an FFmpeg command to perform the desired conversion.

