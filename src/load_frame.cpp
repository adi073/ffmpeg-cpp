#include<iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

bool load_frame(const char* filename, int* width, int* height, unsigned char** data)
{
    AVFormatContext* av_format_ctx = avformat_alloc_context();
    if (!av_format_ctx)
    {
        std::cout << "Couldn't create AVFormatContext\n" << std::endl;
        return false;
    }

    if (avformat_open_input(&av_format_ctx, filename, NULL, NULL) != 0)
    {
        std::cout << "Couldn't open video file\n" << std::endl;
        return false;
    }
    return false;
}

1:46:00
