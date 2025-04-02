#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Function to install FFmpeg on Windows
void install_ffmpeg_windows()
{
    printf("Installing FFmpeg on Windows...\n");
    // Download FFmpeg using PowerShell
    int download_result = system("powershell -Command \"Invoke-WebRequest -Uri https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-essentials.zip -OutFile ffmpeg.zip\"");
    if (download_result != 0)
    {
        fprintf(stderr, "Error: Unable to download FFmpeg.\n");
        exit(1);
    }

    // Extract the ZIP file
    int extract_result = system("powershell -Command \"Expand-Archive -Path ffmpeg.zip -DestinationPath . -Force\"");
    if (extract_result != 0)
    {
        fprintf(stderr, "Error: Unable to extract FFmpeg.\n");
        exit(1);
    }

    // Add FFmpeg to PATH
    int move_result = system("setx PATH \"%cd%\\ffmpeg-release-essentials\\bin;%PATH%\"");
    if (move_result != 0)
    {
        fprintf(stderr, "Error: Unable to add FFmpeg to PATH.\n");
        exit(1);
    }
    printf("FFmpeg installed successfully. Please restart your terminal for PATH changes to take effect.\n");
}

// Function to install FFmpeg on Linux
void install_ffmpeg_linux()
{
    printf("Installing FFmpeg on Linux...\n");
    int install_result = system("sudo apt update && sudo apt install -y ffmpeg");
    if (install_result != 0)
    {
        fprintf(stderr, "Error: Unable to install FFmpeg on Linux.\n");
        exit(1);
    }
    printf("FFmpeg installed successfully on Linux.\n");
}

// Function to check if FFmpeg is installed
void is_ffmpeg_installed()
{
    // Check if "ffmpeg -version" executes successfully
#ifdef _WIN32
    int result = system("ffmpeg -version >nul 2>&1");
#else
    int result = system("ffmpeg -version >/dev/null 2>&1");
#endif
    if (result != 0)
    {
        char choice;
        printf("FFmpeg is not installed. Do you want to install it? (y/n): ");
        scanf(" %c", &choice);

        // Convert input to lowercase for uniformity
        choice = tolower(choice);
        if (choice == 'y')
        {
#ifdef _WIN32
            install_ffmpeg_windows();
#else
            install_ffmpeg_linux();
#endif
        }
        else
        {
            printf("FFmpeg is required to proceed. Exiting program.\n");
            exit(1);
        }
    }
}

// Function to convert videos using FFmpeg
int convert_video(const char *input_file, const char *output_file, const char *input_ext, const char *output_ext)
{
    // Construct the FFmpeg command
    char command[1024];

    // Determine the correct FFmpeg command based on extensions
    if (strcmp(input_ext, ".mp4") == 0 && strcmp(output_ext, ".avi") == 0) {
        snprintf(command, sizeof(command),
                 "ffmpeg -i \"%s\" -c:v libxvid -qscale:v 5 -c:a libmp3lame -b:a 192k \"%s\"",
                 input_file, output_file);
    } else if (strcmp(input_ext, ".mp4") == 0 && strcmp(output_ext, ".webm") == 0) {
        snprintf(command, sizeof(command),
                 "ffmpeg -i \"%s\" -c:v libvpx -b:v 1M -c:a libvorbis \"%s\"",
                 input_file, output_file);
    } else if (strcmp(input_ext, ".avi") == 0 && strcmp(output_ext, ".mp4") == 0) {
        snprintf(command, sizeof(command),
                 "ffmpeg -i \"%s\" -c:v libx264 -crf 23 -preset fast -c:a aac -b:a 128k \"%s\"",
                 input_file, output_file);
    } else if (strcmp(input_ext, ".avi") == 0 && strcmp(output_ext, ".webm") == 0) {
        snprintf(command, sizeof(command),
                 "ffmpeg -i \"%s\" -c:v libvpx -b:v 1M -c:a libvorbis \"%s\"",
                 input_file, output_file);
    } else if (strcmp(input_ext, ".webm") == 0 && strcmp(output_ext, ".mp4") == 0) {
        snprintf(command, sizeof(command),
                 "ffmpeg -i \"%s\" -c:v libx264 -crf 23 -preset fast -c:a aac -b:a 128k \"%s\"",
                 input_file, output_file);
    } else if (strcmp(input_ext, ".webm") == 0 && strcmp(output_ext, ".avi") == 0) {
        snprintf(command, sizeof(command),
                 "ffmpeg -i \"%s\" -c:v libxvid -qscale:v 5 -c:a libmp3lame -b:a 192k \"%s\"",
                 input_file, output_file);
    } else {
        fprintf(stderr, "Unsupported conversion: %s to %s\n", input_ext, output_ext);
        return 1;
    }

    // Execute the FFmpeg command
    printf("Executing command: %s\n", command);
    int result = system(command);

    // Check for errors
    if (result != 0) {
        fprintf(stderr, "Conversion failed. Please ensure FFmpeg is installed and try again.\n");
        return 1;
    }

    printf("Conversion completed successfully: %s -> %s\n", input_file, output_file);
    return 0;
}

