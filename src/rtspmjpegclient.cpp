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

#include <string>
#include "loghandler.hpp"
#include "rtspmjpegclient.hpp"

using namespace loghandlerns;

namespace rtspmjpegclientns {

const char *state_to_string(int state)
{
    switch (state)
    {
    case RTSPMJPEGCLIENT_STATE_ERROR:
        return "ERROR";

    case RTSPMJPEGCLIENT_STATE_EMPTY:
        return "EMPTY";

    case RTSPMJPEGCLIENT_STATE_CLEANED:
        return "CLEANED";

    case RTSPMJPEGCLIENT_STATE_INITIALIZING:
        return "INITIALIZING";

    case RTSPMJPEGCLIENT_STATE_LOOPING:
        return "LOOPING";

    case RTSPMJPEGCLIENT_STATE_STOPPING:
        return "STOPPING";

    case RTSPMJPEGCLIENT_STATE_ABORTING:
        return "ABORTING";

    default:
        return "UNKNOWN";
    }
}

RTSPMJPEGClient* RTSPMJPEGClient::_instance = NULL;

static int interrupt_cb(void *pClient)
{
    // if av_read_frame takes more than timeout then interrupt it
    if (((RTSPMJPEGClientStruct * ) pClient)->state == RTSPMJPEGCLIENT_STATE_LOOPING
        && (clock() - ((RTSPMJPEGClientStruct * ) pClient)->av_frame_read_ticks) / CLOCKS_PER_SEC > RTSPMJPEGCLIENT_AV_FRAME_READ_TIMEOUT)
        return 1;

    return ((RTSPMJPEGClientStruct * ) pClient)->state == RTSPMJPEGCLIENT_STATE_ABORTING
        || ((RTSPMJPEGClientStruct * ) pClient)->state == RTSPMJPEGCLIENT_STATE_STOPPING ? 1 : 0;
}

static const AVIOInterruptCB int_cb = { interrupt_cb, NULL };

RTSPMJPEGClient::RTSPMJPEGClient()
{
    av_register_all();
    avformat_network_init();

    for (int i = 0; i < RTSPMJPEGCLIENT_MAX_CLIENT_NUM; i++)
        clients[i] = NULL;
}

RTSPMJPEGClient* RTSPMJPEGClient::getInstance()
{
    if (!_instance)
        _instance = new RTSPMJPEGClient;

    return _instance;
}

void RTSPMJPEGClient::_logAVError(RTSPMJPEGClientStruct *pClient, int error)
{
    if (av_strerror(error, pClient->errbuf, RTSPMJPEGCLIENT_ERRBUF_SIZE) == 0)
    {
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << pClient->clientId << "] LibAV error: " << pClient->errbuf);
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] LibAV error: " << pClient->errbuf);
    }
    else
    {
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << pClient->clientId << "] LibAV error: Unknown error...");
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] LibAV error: Unknown error...");
    }
}

int RTSPMJPEGClient::_init(RTSPMJPEGClientStruct *pClient)
{
    // state is set to ERROR by the caller
    if (pClient->address.empty())
        return -1;

    pClient->state = RTSPMJPEGCLIENT_STATE_INITIALIZING;
    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] STATE = " << state_to_string(pClient->state));

    pClient->_pCodecCtxOrig = NULL;
    pClient->_pCodecCtx = NULL;
    pClient->_pCodec = NULL;

    LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] Trying to open address " << pClient->address);

    // open rtsp
    pClient->_pFormatCtx = avformat_alloc_context();
    pClient->_pFormatCtx->interrupt_callback = int_cb;
    pClient->_pFormatCtx->interrupt_callback.opaque = pClient;

    int ret = 0;

    // open input
    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] avformat_open_input() [TCP]... ");

    // try with TCP
    AVDictionary *options = NULL;
    av_dict_set(&options, "rtsp_transport", "tcp", 0);
    ret = avformat_open_input(&pClient->_pFormatCtx, pClient->address.c_str(), NULL, &options);
    av_dict_free(&options);

    if (ret < 0)
    {
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << pClient->clientId << "] Could not open address [TCP] " << pClient->address << "!");
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] Could not open address [TCP] " << pClient->address << "!");
        _logAVError(pClient, ret);

        // this to avoid to try UDP while stopping or aborting
        if (pClient->state != RTSPMJPEGCLIENT_STATE_INITIALIZING)
            return -1;

        // try with UDP
        LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] avformat_open_input() [UDP]... ");
        ret = avformat_open_input(&pClient->_pFormatCtx, pClient->address.c_str(), NULL, NULL);

        if (ret < 0)
        {
            LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << pClient->clientId << "] Could not open address [UDP] " << pClient->address << "!");
            LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] Could not open address [UDP] " << pClient->address << "!");
        _logAVError(pClient, ret);

        return -1;
    }
    }

    // find stream info
    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] avformat_find_stream_info()... ");

    // see https://ffmpeg.zeranoe.com/forum/viewtopic.php?t=1211
    int64_t max_analyze_duration = pClient->_pFormatCtx->max_analyze_duration;

    pClient->_pFormatCtx->max_analyze_duration = 0;
    ret = avformat_find_stream_info(pClient->_pFormatCtx, NULL);
    pClient->_pFormatCtx->max_analyze_duration = max_analyze_duration;

    if (ret < 0)
    {
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << pClient->clientId << "] Could not get stream info! ");
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] Could not get stream info! ");
        _logAVError(pClient, ret);

        return - 1;
    }

    // search video stream
    pClient->_videoStream = -1;

    for (int i = 0; i < (int ) pClient->_pFormatCtx->nb_streams; i++)
    {
        if (pClient->_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO
            && pClient->_pFormatCtx->streams[i]->codec->codec_id == AV_CODEC_ID_MJPEG)
        {
            pClient->_videoStream = i;
            break;
        }
    }

    if (pClient->_videoStream == -1)
    {
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << pClient->clientId << "] Could not find any MJPEG stream!");
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] Could not find any MJPEG stream!");

        return - 1;
    }

    pClient->_pCodecCtxOrig = pClient->_pFormatCtx->streams[pClient->_videoStream]->codec;

    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] avcodec_alloc_context3()... ");
    pClient->_pCodecCtx = avcodec_alloc_context3(NULL);

    // find the decoder for the video stream
    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] avcodec_find_decoder()... ");
    pClient->_pCodec = avcodec_find_decoder(pClient->_pCodecCtxOrig->codec_id);

    if (pClient->_pCodec == NULL)
    {
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << pClient->clientId << "] Unsupported codec!");
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] Unsupported codec!");

        return -1; // codec not found
    }

    // copy context
    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] avcodec_copy_context()... ");
    ret = avcodec_copy_context(pClient->_pCodecCtx, pClient->_pCodecCtxOrig);

    if (ret != 0)
    {
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << pClient->clientId << "] Could not copy codec context!");
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] Could not copy codec context!");
        _logAVError(pClient, ret);

        return - 1; // error copying codec context
    }

    // open codec
    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] avcodec_open2()... ");
    ret = avcodec_open2(pClient->_pCodecCtx, pClient->_pCodec, NULL);

    if (ret < 0)
    {
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << pClient->clientId << "] Could not open codec!");
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] Could not open codec!");
        _logAVError(pClient, ret);

        return - 1;
    }

    return 0;
}

void RTSPMJPEGClient::_clean(RTSPMJPEGClientStruct *pClient, int stateToSet)
{
    // close the codecs
    if (pClient->_pCodecCtx != NULL)
    {
        avcodec_close(pClient->_pCodecCtx);
        pClient->_pCodecCtx = NULL;
    }

    if (pClient->_pCodecCtxOrig != NULL)
    {
        avcodec_close(pClient->_pCodecCtxOrig);
        pClient->_pCodecCtxOrig = NULL;
    }

    // close the video input
    if (pClient->_pFormatCtx != NULL)
    {
        avformat_close_input(&pClient->_pFormatCtx);
        pClient->_pFormatCtx = NULL;
    }

    pClient->state = stateToSet;

    pClient->isWaiting = false;
    pClient->newFramesNum = 0;

    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] STATE = " << state_to_string(pClient->state));
}

int RTSPMJPEGClient::_readFrame(RTSPMJPEGClientStruct *pClient)
{
    int ret = 0;

    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] av_read_frame()...");

    pClient->av_frame_read_ticks = clock();

    // is this a packet from the video stream?
    if (av_read_frame(pClient->_pFormatCtx, &pClient->_packet) < 0 || pClient->_packet.stream_index != pClient->_videoStream)
    {
        if (pClient->state == RTSPMJPEGCLIENT_STATE_ABORTING || pClient->state == RTSPMJPEGCLIENT_STATE_STOPPING)
            LOG4CPLUS_WARN(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] frame not read because STOPPING or ABORTING");
        else
        {
            LOG4CPLUS_WARN(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] frame read failed or stream not valid!");
            _logAVError(pClient, ret);

            pClient->state = RTSPMJPEGCLIENT_STATE_ABORTING;
        }

        ret = -1;
    } else if (!pClient->isWaiting && pClient->_packet.size <= RTSPMJPEGCLIENT_FRAME_BUFFER_SIZE) {

        // copy the packet (jpg) in the next frame buffer position
        memcpy(pClient->frameQueue[pClient->next], pClient->_packet.data, pClient->_packet.size);
        pClient->frameSizes[pClient->next] = pClient->_packet.size;

        pClient->next = (pClient->next + 1) % RTSPMJPEGCLIENT_FRAME_QUEUE_LEN;

        pClient->newFramesNum = std::min(pClient->newFramesNum + 1, RTSPMJPEGCLIENT_FRAME_QUEUE_LEN);
    }

    LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] new frames num = " << pClient->newFramesNum << "\tnext = " << pClient->next);

    // free the packet that was allocated by av_read_frame
    av_free_packet(&pClient->_packet);

    return ret;
}

void RTSPMJPEGClient::threadLoop(RTSPMJPEGClientParameters *parameters)
{
    int clientId = parameters->clientId;

    RTSPMJPEGClient *instance = getInstance();
    RTSPMJPEGClientStruct *pClient = instance->clients[clientId];

    if (pClient == NULL)
    {
        pClient = new RTSPMJPEGClientStruct;

        pClient->clientId = clientId;
        pClient->state = RTSPMJPEGCLIENT_STATE_CLEANED;
        pClient->isWaiting = false;
        pClient->newFramesNum = 0;
        pClient->next = 0;

        for (int i = 0; i < RTSPMJPEGCLIENT_FRAME_QUEUE_LEN; i++)
        {
            pClient->frameQueue[i] = (uint8_t * ) av_malloc(RTSPMJPEGCLIENT_FRAME_BUFFER_SIZE);
            pClient->frameSizes[i] = 0;
        }

        instance->clients[clientId] = pClient;

        LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] allocated space for a new client...");
    }

    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] threadLoop()...");

    pClient->address = ((RTSPMJPEGClientParameters * ) parameters)->address;

    delete parameters;

    if (instance->_init(pClient) < 0)
    {
        instance->_clean(pClient, RTSPMJPEGCLIENT_STATE_ERROR);
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << pClient->clientId << "] init failed!");
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] init failed!");

        return;
    }

    pClient->state = RTSPMJPEGCLIENT_STATE_LOOPING;

    while (pClient->state == RTSPMJPEGCLIENT_STATE_LOOPING && instance->_readFrame(pClient) == 0)
        LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << pClient->clientId << "] while()...");

    // set CLEANED only if it was correctly stopped. ERROR otherwise. Don't set
    // any state greater then CLEANED because they're all states indicating an
    // action being taken
    instance->_clean(pClient, pClient->state == RTSPMJPEGCLIENT_STATE_STOPPING ? RTSPMJPEGCLIENT_STATE_CLEANED : RTSPMJPEGCLIENT_STATE_ERROR);
}

int RTSPMJPEGClient::wait(int clientId)
{
    if (clients[clientId]->state != RTSPMJPEGCLIENT_STATE_LOOPING)
        return -1;

    clients[clientId]->isWaiting = true;

    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] STATE = " << state_to_string(clients[clientId]->state));

    return 0;
}

int RTSPMJPEGClient::resume(int clientId)
{
    if (clients[clientId]->state != RTSPMJPEGCLIENT_STATE_LOOPING)
        return -1;

    clients[clientId]->isWaiting = false;

    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] STATE = " << state_to_string(clients[clientId]->state));

    return 0;
}

void RTSPMJPEGClient::stop(int clientId)
{
    if (clients[clientId]->state <= RTSPMJPEGCLIENT_STATE_CLEANED)
        return;

    clients[clientId]->state = RTSPMJPEGCLIENT_STATE_STOPPING;

    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] STATE = " << state_to_string(clients[clientId]->state));
}

void RTSPMJPEGClient::abort(int clientId)
{
    if (clients[clientId]->state <= RTSPMJPEGCLIENT_STATE_CLEANED)
        return;

    clients[clientId]->state = RTSPMJPEGCLIENT_STATE_ABORTING;

    LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] STATE = " << state_to_string(clients[clientId]->state));
}

int RTSPMJPEGClient::destroy(int clientId)
{
    RTSPMJPEGClientStruct *pClient = clients[clientId];

    // WARNING! Not thread safe!
    if (pClient->state > RTSPMJPEGCLIENT_STATE_CLEANED)
        return -1;

    clients[clientId] = NULL;

    for (int i = 0; i < RTSPMJPEGCLIENT_FRAME_QUEUE_LEN; i++)
    {
        av_free(pClient->frameQueue[i]);
        pClient->frameQueue[i] = NULL;
    }

    delete pClient;

    return 0;
}

void RTSPMJPEGClient::logState(int clientId)
{
    if (clientId > 0)
    {
        LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] STATE = " << state_to_string(clients[clientId]->state));

        return;
    }

    // print all instantiated client's state
    for (int i = 0; i < RTSPMJPEGCLIENT_MAX_CLIENT_NUM; i++)
    {
        if (clients[i] != NULL)
            LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << i << "] STATE = " << state_to_string(clients[i]->state));
    }

    // dump information about file onto standard error
    //av_dump_format(_pFormatCtx, 0, address.c_str(), 0);
}

}
