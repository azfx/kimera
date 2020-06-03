#ifndef DECODER_CODEC_H
#define DECODER_CODEC_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#include "kimera/state.h"

typedef struct {
    AVCodecContext *codec_ctx;
    AVCodecParserContext *parser_ctx;
    bool has_parser;
    AVPacket* retard;
    AVFrame* frame;
} DecoderState;

bool start_decoder(DecoderState*, State*);
void close_decoder(DecoderState*);
bool decoder_push(DecoderState*, char*, uint32_t, uint64_t);
bool needs_parser(enum AVCodecID);

#endif