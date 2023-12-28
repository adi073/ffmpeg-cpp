#include<iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

bool load_frame(const char* filename, int* width, int* height, unsigned char** data)
{
    // Create av format context
    AVFormatContext* av_format_ctx = avformat_alloc_context();
    if (!av_format_ctx)
    {
        std::cout << "Couldn't create AVFormatContext\n" << std::endl;
        return false;
    }
    
    //open the input and put the information about the open file into the av format context.
    // It will return the integer/bool if it succeded or not.
    if (avformat_open_input(&av_format_ctx, filename, NULL, NULL) != 0)
    {
        std::cout << "Couldn't open video file\n" << std::endl;
        return false;
    }
    
    int video_stream_idx = -1;
    AVCodecParameters *av_codec_params;
    AVCodec *av_codec;
    for (int i=0; i<av_format_ctx->nb_streams; ++i){
        auto stream = av_format_ctx->nb_streams[i];
        av_codec_params = av_format_ctx->streams[i]->codecpar;
        av_codec = av_codec_find_decoder(av_codec_params->code_id);

        if (!av_codec){
            continue;
        }

        if (av_codec->code_type == AVMEDIA_TYPE_VIDEO){
            video_stream_idx = i;
            break;
        }

    }

    if (video_stream_idx == -1){
        std::cout << "Couldn't find valid video stream" << std::endl;
        return false;
    }


    return false;
}

