/*
 * Copyright 2016 Qualibit S.r.l.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http ://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RTSPMJPEGCLIENT_HPP
#define RTSPMJPEGCLIENT_HPP

#include <string>
#include "rtspmjpegclient_common.hpp"

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#ifdef __cplusplus
} // __cplusplus defined.
#endif

namespace rtspmjpegclientns {

    const char *state_to_string(int state);

    // input
    struct _RTSPMJPEGClientParameters {
        const char *address;
        int clientId;
    };

    typedef struct _RTSPMJPEGClientParameters RTSPMJPEGClientParameters;

    // struct
    struct _RTSPMJPEGClientStruct {
        // private
        AVFormatContext *_pFormatCtx;
        AVCodecContext *_pCodecCtxOrig;
        AVCodecContext *_pCodecCtx;
        AVCodec *_pCodec;
        AVPacket _packet;

        char errbuf[RTSPMJPEGCLIENT_ERRBUF_SIZE];

        int _videoStream;

        // public
        int clientId;
        std::string address;
        int state;
        bool isWaiting;
        int newFramesNum;
        int next;
        uint8_t *frameQueue[RTSPMJPEGCLIENT_FRAME_QUEUE_LEN];
        int frameSizes[RTSPMJPEGCLIENT_FRAME_QUEUE_LEN];
    };

    typedef struct _RTSPMJPEGClientStruct RTSPMJPEGClientStruct;

    class RTSPMJPEGClient {
    private:
        static RTSPMJPEGClient *_instance;

        RTSPMJPEGClient();

        void _logAVError(RTSPMJPEGClientStruct *pClient, int error);
        void _clean(RTSPMJPEGClientStruct *pClient, int stateToSet = RTSPMJPEGCLIENT_STATE_CLEANED);
        int _init(RTSPMJPEGClientStruct *pClient);
        int _readFrame(RTSPMJPEGClientStruct *pClient);

    public:
        RTSPMJPEGClientStruct *clients[RTSPMJPEGCLIENT_MAX_CLIENT_NUM];

        static RTSPMJPEGClient *getInstance();

        static void threadLoop(RTSPMJPEGClientParameters *parameters);

        int wait(int clientId);
        int resume(int clientId);
        void stop(int clientId);
        void abort(int clientId);
        int destroy(int clientId);

        void logState(int clientId);
    };

}
#endif
