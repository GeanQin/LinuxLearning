#include <stdio.h>
#include <libavformat/avformat.h>
#include <libavutil/mathematics.h>

/**
 * ffmpeg make
 * ./configure --prefix=./install --target-os=linux --enable-small --disable-runtime-cpudetect 
 * --disable-all --enable-avformat --enable-avcodec --enable-avutil --disable-everything 
 * --enable-muxer=mp4 --enable-protocol=file --enable-encoder=mpeg4 --disable-doc --disable-debug 
 * --disable-iconv --disable-x86asm --enable-decoder=hevc --enable-demuxer=hevc --enable-decoder=aac 
 * --enable-demuxer=aac --enable-bsf=aac_adtstoasc --enable-parser=hevc
 */

#define GET_H265_FROM_FILE 1
#define GET_AAC_FROM_FILE 1
#define GET_G711A_FROM_FILE 0   // ffmpeg不支持

#define IN_V_FILE_PATH "/root/share_nfs/h265/test.h265"
#if GET_AAC_FROM_FILE
#define IN_A_FILE_PATH "/root/share_nfs/aac/test.aac"
#elif GET_G711A_FROM_FILE
#define IN_A_FILE_PATH "/root/share_nfs/g711a/test.pcm_alaw"
#endif
#define OUT_FILE_PATH "/root/share_nfs/mp4/test.mp4"

void print_ver()
{
    unsigned int ver = avformat_version();
    fprintf(stdout, "avformat ver=%d,%d,%d\n", AV_VERSION_MAJOR(ver), AV_VERSION_MINOR(ver), AV_VERSION_MICRO(ver));
}

int main(int argc, char *argv[])
{
    int ret, i = 0;
    AVFormatContext *ifmt_ctx_v = NULL, *ifmt_ctx_a = NULL, *ofmt_ctx = NULL;
    AVOutputFormat *ofmt = NULL;
    AVCodec *in_v_codec;
    AVCodec *in_a_codec;
    AVPacket pkt;
    int frame_index = 0;
    int videoindex_v = -1, videoindex_out = -1;
    int audioindex_a = -1, audioindex_out = -1;
    int64_t cur_pts_v = 0, cur_pts_a = 0;

    print_ver();

#if GET_H265_FROM_FILE
    if ((ret = avformat_open_input(&ifmt_ctx_v, IN_V_FILE_PATH, 0, 0)) < 0)
    {
        fprintf(stderr, "Could not open input file(%s).\n%s\n", IN_V_FILE_PATH, av_err2str(ret));
        return -1;
    }
    if ((ret = avformat_find_stream_info(ifmt_ctx_v, 0)) < 0)
    {
        fprintf(stderr, "Failed to retrieve input stream information\n");
        return -1;
    }
    av_dump_format(ifmt_ctx_v, 0, IN_V_FILE_PATH, 0);
#endif

#if GET_AAC_FROM_FILE
    if ((ret = avformat_open_input(&ifmt_ctx_a, IN_A_FILE_PATH, 0, 0)) < 0)
    {
        fprintf(stderr, "Could not open input file(%s).\n%s\n", IN_A_FILE_PATH, av_err2str(ret));
        return -1;
    }
    if ((ret = avformat_find_stream_info(ifmt_ctx_a, 0)) < 0)
    {
        fprintf(stderr, "Failed to retrieve input stream information\n");
        return -1;
    }
    av_dump_format(ifmt_ctx_a, 0, IN_A_FILE_PATH, 0);
#elif GET_G711A_FROM_FILE
    if ((ret = avformat_open_input(&ifmt_ctx_a, IN_A_FILE_PATH, 0, 0)) < 0)
    {
        fprintf(stderr, "Could not open input file(%s).\n%s\n", IN_A_FILE_PATH, av_err2str(ret));
        return -1;
    }
    if ((ret = avformat_find_stream_info(ifmt_ctx_a, 0)) < 0)
    {
        fprintf(stderr, "Failed to retrieve input stream information\n");
        return -1;
    }
    av_dump_format(ifmt_ctx_a, 0, IN_A_FILE_PATH, 0);
#endif

    // output to mp4
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, OUT_FILE_PATH);
    if (!ofmt_ctx)
    {
        fprintf(stderr, "Could not create output context\n");
        return -1;
    }
    ofmt = ofmt_ctx->oformat;

    for (i = 0; i < ifmt_ctx_v->nb_streams; i++)
    {
        if (!(in_v_codec = avcodec_find_decoder(ifmt_ctx_v->streams[i]->codecpar->codec_id)))
        {
            fprintf(stderr, "Could not find video encoder\n");
            return -1;
        }
        // Create output AVStream according to input AVStream
        if (ifmt_ctx_v->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            AVStream *in_stream = ifmt_ctx_v->streams[i];
            AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_v_codec);
            videoindex_v = i;
            if (!out_stream)
            {
                fprintf(stderr, "Failed allocating video output stream\n");
                return -1;
            }
            videoindex_out = out_stream->index;
            // Copy the settings of AVCodecContext
            AVCodecContext *codec_ctx = avcodec_alloc_context3(in_v_codec);
            ret = avcodec_parameters_to_context(codec_ctx, in_stream->codecpar);
            if (ret < 0)
            {
                fprintf(stderr, "Failed to copy in_stream codecpar to codec context\n");
                return -1;
            }

            codec_ctx->codec_tag = 0;
            if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
                codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

            ret = avcodec_parameters_from_context(out_stream->codecpar, codec_ctx);
            if (ret < 0)
            {
                fprintf(stderr, "Failed to copy codec context to out_stream codecpar context\n");
                return -1;
            }
            break;
        }
    }
    for (i = 0; i < ifmt_ctx_a->nb_streams; i++)
    {
        if (!(in_a_codec = avcodec_find_decoder(ifmt_ctx_a->streams[i]->codecpar->codec_id)))
        {
            fprintf(stderr, "Could not find audio encoder\n");
            return -1;
        }
        // Create output AVStream according to input AVStream
        if (ifmt_ctx_a->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            AVStream *in_stream = ifmt_ctx_a->streams[i];
            AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_a_codec);
            audioindex_a = i;
            if (!out_stream)
            {
                fprintf(stderr, "Failed allocating output audio stream\n");
                return -1;
            }
            audioindex_out = out_stream->index;
            // Copy the settings of AVCodecContext
            AVCodecContext *codec_ctx = avcodec_alloc_context3(in_a_codec);
            ret = avcodec_parameters_to_context(codec_ctx, in_stream->codecpar);
            if (ret < 0)
            {
                fprintf(stderr, "Failed to copy in_stream codecpar to codec context\n");
                return -1;
            }

            codec_ctx->codec_tag = 0;
            if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
                codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

            ret = avcodec_parameters_from_context(out_stream->codecpar, codec_ctx);
            if (ret < 0)
            {
                fprintf(stderr, "Failed to copy codec context to out_stream codecpar context\n");
                return -1;
            }
            break;
        }
    }
    av_dump_format(ofmt_ctx, 0, OUT_FILE_PATH, 1);
    // Open output file
    if (!(ofmt->flags & AVFMT_NOFILE))
    {
        if (avio_open(&ofmt_ctx->pb, OUT_FILE_PATH, AVIO_FLAG_WRITE) < 0)
        {
            fprintf(stderr, "Could not open output file '%s'\n", OUT_FILE_PATH);
            return -1;
        }
    }
    // Write file header
    if (avformat_write_header(ofmt_ctx, NULL) < 0)
    {
        fprintf(stderr, "Error occurred when opening output file\n");
        return -1;
    }

    // start write frame
    while (1)
    {
        AVFormatContext *ifmt_ctx;
        int stream_index = 0;
        AVStream *in_stream, *out_stream;

        // Get an AVPacket
        if (av_compare_ts(cur_pts_v, ifmt_ctx_v->streams[videoindex_v]->time_base, cur_pts_a, ifmt_ctx_a->streams[audioindex_a]->time_base) <= 0)
        {
            ifmt_ctx = ifmt_ctx_v;
            stream_index = videoindex_out;

            if (av_read_frame(ifmt_ctx, &pkt) >= 0)
            {
                do
                {
                    in_stream = ifmt_ctx->streams[pkt.stream_index];
                    out_stream = ofmt_ctx->streams[stream_index];

                    if (pkt.stream_index == videoindex_v)
                    {
                        // FIX：No PTS (Example: Raw H.264)
                        // Simple Write PTS
                        if (pkt.pts == AV_NOPTS_VALUE)
                        {
                            // Write PTS
                            AVRational time_base1 = in_stream->time_base;
                            // Duration between 2 frames (us)
                            int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(in_stream->r_frame_rate);
                            // Parameters
                            pkt.pts = (double)(frame_index * calc_duration) / (double)(av_q2d(time_base1) * AV_TIME_BASE);
                            pkt.dts = pkt.pts;
                            pkt.duration = (double)calc_duration / (double)(av_q2d(time_base1) * AV_TIME_BASE);
                            frame_index++;
                        }

                        cur_pts_v = pkt.pts;
                        break;
                    }
                } while (av_read_frame(ifmt_ctx, &pkt) >= 0);
            }
            else
            {
                break;
            }
        }
        else
        {
            ifmt_ctx = ifmt_ctx_a;
            stream_index = audioindex_out;
            
            if (av_read_frame(ifmt_ctx, &pkt) >= 0)
            {
                do
                {
                    in_stream = ifmt_ctx->streams[pkt.stream_index];
                    out_stream = ofmt_ctx->streams[stream_index];

                    if (pkt.stream_index == audioindex_a)
                    {

                        // FIX：No PTS
                        // Simple Write PTS
                        if (pkt.pts == AV_NOPTS_VALUE)
                        {
                            // Write PTS
                            AVRational time_base1 = in_stream->time_base;
                            // Duration between 2 frames (us)
                            int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(in_stream->r_frame_rate);
                            // Parameters
                            pkt.pts = (double)(frame_index * calc_duration) / (double)(av_q2d(time_base1) * AV_TIME_BASE);
                            pkt.dts = pkt.pts;
                            pkt.duration = (double)calc_duration / (double)(av_q2d(time_base1) * AV_TIME_BASE);
                            frame_index++;
                        }
                        cur_pts_a = pkt.pts;

                        break;
                    }
                } while (av_read_frame(ifmt_ctx, &pkt) >= 0);
            }
            else
            {
                break;
            }
        }

        // Convert PTS/DTS
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;
        pkt.stream_index = stream_index;

        fprintf(stdin, "Write 1 Packet. size:%5d\tpts:%ld\n", pkt.size, pkt.pts);
        // Write
        if (av_interleaved_write_frame(ofmt_ctx, &pkt) < 0)
        {
            printf("Error muxing packet\n");
            break;
        }
        av_packet_unref(&pkt);
    }
    av_write_trailer(ofmt_ctx);

    avformat_close_input(&ifmt_ctx_v);
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);

    return 0;
}