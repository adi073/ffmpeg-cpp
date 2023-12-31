#include<iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

bool load_frame(const char* filename, int* width_out, int* height_out, unsigned char** data_out)
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
    
    // Setup a codec context for the decoder
    AVCodecContext *av_codec_ctx = avcodec_alloc_context3(av_codec);
    if (!av_codec_ctx){
        std::cout << "Couldn't allocate AVCodecContext" << std::endl;
        return false;
    }
    // The information that libav format got out of the file has to be placed into the codec context as its initial state
    if (av_codec_parameters_to_context(av_codec_ctx, av_codec_params) < 0){
        std::cout << "Couldn't initialize AVCodecContext\n" << std::endl;
        return false;
    }
    // Open the file and start reading from it
    if (avcodec_open2(av_codec_ctx, av_codec, NULL) < 0){
        std::cout << "Couldn't open codec" << std::endl;
        return false;
    }
    AVFrame *av_frame = av_frame_alloc();
    if (!av_frame){
        std::cout << "Couldn't allocate AVFrame" << std::endl;
        return false;
    }
    AVPacket *av_packet = av_packet_alloc();
    if (!av_packet){
        std::cout << "Couldn't allocate AVPacket" << std::endl;
        return false;
    }
    
    int response;
    while (av_read_frame(av_format_ctx, av_packet) >= 0){
        if (av_packet->stream_index != video_stream_idx){
            continue;
        }
        response = avcodec_send_packet(av_codec_ctx, av_packet);
        if (response < 0){
            std::cout << "Failed to decode packet: " << av_errstr(response) << std::endl;
            return false;
        }
        response = avcodec_receive_frame(av_codec_ctx, av_frame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF){
            continue;
        }else if(response < 0){
            std::cout << "Failed to decode packet" << av_err2str(reponse) << std::endl;
            return false;
        }

        av_packet_unref(av_packet);
        break;
    }

    //unsigned char *data = new unsigned char[av_frame->width * av_frame->height*3];
    //for (int x=0; x<av_frame->width; ++x){
    //    for (int y=0; y<av_frame->height; ++y){
    //        data[y * av_frame->width * 3 + x * 3] = av_frame->data[0][y * av_frame->linesize[0] + x];
    //        data[y * av_frame->width * 3 + x * 3] = av_frame->data[0][y * av_frame->linesize[0] + x];
    //        data[y * av_frame->width * 3 + x * 3] = av_frame->data[0][y * av_frame->linesize[0] + x];
    //    }
    //}
    //width_out = av_frame->width;
    //*height_out = av_frame->height;
    //*data_out = av_frame->data;

    unit8_t *data = new unit8_t [av_frame->width * av_frame->height *4];

    SwsContext *sws_scaler_ctx = sws_getContext(av_frame->width, av_frame->height, av_codec_ctx->pix_fmt,
                                                av_frame->widht, av_frame->height, AV_PIX_FMT_RGB0,
                                                SWS_BILINEAR, NULL, NULL, NULL);
    if (!sws_scaler_ctx){
        std::cout << "Couldn't initialize sw scaler" << std::endl;
        return false;
    }
    unit8_t *dest[4] = {data, NULL, NULL, NULL};
    int dest_linesize[4] = {av_frame->width * 4, 0, 0, 0};
    sws_scale(sws_scaler_ctx, av_frame->data, av_frame->linesize, 0, av_frame->height, dest, dest_linesize);
    sws_freeContext(sws_scaler_ctx);

    width_out = av_frame->width;
    *height_out = av_frame->height;
    *data_out = av_frame->data;
    

    avformat_close_input(&av_format_ctx);
    avformat_free_context(av_format_ctx);
    av_frame_free(&av_frame);
    av_packet_free(&av_packet);
    avcodec_free_context(&av_codec_ctx);
 
    return false;
}

































