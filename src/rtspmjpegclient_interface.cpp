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

#include <cstring>
#include "loghandler.hpp"
#include "rtspmjpegclient.hpp"
#include "rtspmjpegclient_interface.hpp"

using namespace loghandlerns;
using namespace rtspmjpegclientns;

QRTAPI void QRTCALL rtspmjpegclient_start_log(const char *outputLogFile, const char *errorLogFile)
{
    LogHandler::startLog(std::string(outputLogFile), std::string(errorLogFile));
}

QRTAPI void QRTCALL rtspmjpegclient_stop_log()
{
    LogHandler::stopLog();
}

QRTAPI int QRTCALL rtspmjpegclient_get_log_level()
{
    return LogHandler::getLogLevel();
}

QRTAPI void QRTCALL rtspmjpegclient_set_log_level(int logLevel)
{
    LogHandler::setLogLevel(logLevel);
}

QRTAPI void QRTCALL rtspmjpegclient_log_state(int clientId)
{
    RTSPMJPEGClient::getInstance()->logState(clientId);
}

bool check_client(int clientId)
{
    if (clientId < 0 || clientId >= RTSPMJPEGCLIENT_MAX_CLIENT_NUM)
    {
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << clientId << "] client id NOT valid!");
        return false;
    }

    return true;
}

void startup_thread_loop(RTSPMJPEGClientParameters *parameters)
{
    RTSPMJPEGClient *instance = RTSPMJPEGClient::getInstance();

    // wait initialization
    int wait = RTSPMJPEGCLIENT_TIMEOUT;
    int clientId = ((RTSPMJPEGClientParameters * ) parameters)->clientId;

    // start rtsp thread
    std::thread(RTSPMJPEGClient::threadLoop, parameters).detach();

    LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] thread started, waiting initialization...");

    while (wait-- > 0
           && (instance->clients[clientId] == NULL
           || (instance->clients[clientId]->state != RTSPMJPEGCLIENT_STATE_LOOPING
           && instance->clients[clientId]->state != RTSPMJPEGCLIENT_STATE_ERROR)))
    {
        if (instance->clients[clientId] != NULL)
            LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] STATE = " << state_to_string(instance->clients[clientId]->state));

        LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] wait = " << wait);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // if not initialized quit
    if (instance->clients[clientId] == NULL
        || instance->clients[clientId]->state != RTSPMJPEGCLIENT_STATE_LOOPING)
    {
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << clientId << "] rtsp thread not looping... something has gone wrong :(");
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] rtsp thread not looping... something has gone wrong :(");

        if (instance->clients[clientId] != NULL && instance->clients[clientId]->state > RTSPMJPEGCLIENT_STATE_CLEANED)
        {
            instance->abort(clientId);

            wait = RTSPMJPEGCLIENT_TIMEOUT;

            LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << clientId << "] aborting...");
            LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] aborting...");

            while (wait-- > 0 && instance->clients[clientId]->state == RTSPMJPEGCLIENT_STATE_ABORTING)
            {
                LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] STATE = " << state_to_string(instance->clients[clientId]->state));
                LOG4CPLUS_TRACE(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] wait = " << wait);

                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
    }
}

QRTAPI int QRTCALL rtspmjpegclient_start(int clientId, const char *address, int asyncCall)
{
    if (address == NULL)
    {
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << clientId << "] address is empty!");
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] address is empty!");

        return -1;
    }

    if (!check_client(clientId))
        return -1;

    // check that the client is not already started and is not doing anything
    if (RTSPMJPEGClient::getInstance()->clients[clientId] != NULL && RTSPMJPEGClient::getInstance()->clients[clientId]->state > RTSPMJPEGCLIENT_STATE_CLEANED)
    {
        LOG4CPLUS_WARN(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] client already started.");

        return 0;
    }

    RTSPMJPEGClientParameters *parameters = new RTSPMJPEGClientParameters;
    parameters->clientId = clientId;
    parameters->address = address;

    LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] starting " << (asyncCall == 1 ? "asynchronously" : "synchronously") << "...");

    if (asyncCall == 1)
    {
        // start a startup thread
        std::thread(startup_thread_loop, parameters).detach();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    else
        startup_thread_loop(parameters);

    return 0;
}

QRTAPI int QRTCALL rtspmjpegclient_wait(int clientId)
{
    if (!check_client(clientId))
        return -1;

    return RTSPMJPEGClient::getInstance()->wait(clientId);
}

QRTAPI int QRTCALL rtspmjpegclient_resume(int clientId)
{
    if (!check_client(clientId))
        return -1;

    return RTSPMJPEGClient::getInstance()->resume(clientId);
}

QRTAPI int QRTCALL rtspmjpegclient_stop(int clientId)
{
    if (!check_client(clientId))
        return -1;

    RTSPMJPEGClient *instance = RTSPMJPEGClient::getInstance();
    RTSPMJPEGClientStruct *pClient = instance->clients[clientId];

    if (pClient == NULL || pClient->state <= RTSPMJPEGCLIENT_STATE_CLEANED)
        return 0;

    pClient->state = RTSPMJPEGCLIENT_STATE_STOPPING;

    LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] waiting rtsp thread terminating...");

    int wait = RTSPMJPEGCLIENT_TIMEOUT;

    while (wait-- > 0 && (pClient->state != RTSPMJPEGCLIENT_STATE_CLEANED && pClient->state != RTSPMJPEGCLIENT_STATE_ERROR))
        std::this_thread::sleep_for(std::chrono::seconds(1));

    if (pClient->state == RTSPMJPEGCLIENT_STATE_CLEANED || pClient->state == RTSPMJPEGCLIENT_STATE_ERROR)
        LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] thread stopped");
    else
        LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER)), "[CLIENT " << clientId << "] stop FAILED!");

    LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)), "[CLIENT " << clientId << "] STATE = " << state_to_string(pClient->state));

    return pClient->state > RTSPMJPEGCLIENT_STATE_CLEANED ? -1 : 0;
}

QRTAPI int QRTCALL rtspmjpegclient_destroy(int clientId)
{
    if (!check_client(clientId))
        return -1;

    RTSPMJPEGClient *instance = RTSPMJPEGClient::getInstance();
    RTSPMJPEGClientStruct *pClient = instance->clients[clientId];

    if (pClient == NULL)
        return 0;

    return instance->destroy(clientId);
}

QRTAPI int QRTCALL rtspmjpegclient_get_data(int clientId, int frames, int fps, RTSPMJPEGClientData *data)
{
    if (!check_client(clientId))
        return -1;

    RTSPMJPEGClient *instance = RTSPMJPEGClient::getInstance();
    RTSPMJPEGClientStruct *pClient = instance->clients[clientId];

    if (pClient == NULL)
        return 0;

    frames = frames <= 0 ? pClient->newFramesNum : std::min(frames, RTSPMJPEGCLIENT_FRAME_QUEUE_LEN);

    data->clientId = pClient->clientId;
    data->state = pClient->state;
    data->isWaiting = pClient->isWaiting == true ? 1 : 0;
    data->next = pClient->next;

    int cur = pClient->next - frames;
    if (cur < 0)
        cur += RTSPMJPEGCLIENT_FRAME_QUEUE_LEN;

    int step = 1;

    if (fps > 0)
        step = (int ) std::max(ceil((double ) frames / (double ) fps), 1.0);

    data->framesRead = 0;

    for (int i = 0; i * step < frames; i++)
    {
        std::memcpy((char * ) &data->frameQueue[i * RTSPMJPEGCLIENT_FRAME_BUFFER_SIZE], (char * ) pClient->frameQueue[(cur + i * step) % RTSPMJPEGCLIENT_FRAME_QUEUE_LEN], pClient->frameSizes[(cur + i * step) % RTSPMJPEGCLIENT_FRAME_QUEUE_LEN]);
        data->frameSizes[i] = pClient->frameSizes[(cur + i * step) % RTSPMJPEGCLIENT_FRAME_QUEUE_LEN];
        data->framesRead++;
    }

    pClient->newFramesNum = 0;

    return 0;
}

QRTAPI int QRTCALL rtspmjpegclient_get_state(int clientId)
{
    if (!check_client(clientId))
        return -1;

    RTSPMJPEGClientStruct *pClient = RTSPMJPEGClient::getInstance()->clients[clientId];

    if (pClient == NULL)
        return RTSPMJPEGCLIENT_STATE_EMPTY;

    return pClient->state;
}
