#ifndef VIDEO_CONVERSION
#define VIDEO_CONVERSION

void install_ffmpeg_windows();
void install_ffmpeg_linux();
void is_ffmpeg_installed();
int convert_video(const char *input_file, const char *output_file, const char *input_ext, const char *output_ext);

#endif
