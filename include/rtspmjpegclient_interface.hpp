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

#ifndef RTSPMJPEGCLIENT_INTERFACE_HPP
#define RTSPMJPEGCLIENT_INTERFACE_HPP

#include <chrono>
#include <thread>
#include "rtspmjpegclient_common.hpp"

#ifdef _MSC_VER
#define QRT_EXPORTS

#ifdef QRT_EXPORTS
#define QRTAPI __declspec(dllexport)
#else
#define QRTAPI __declspec(dllimport)
#endif

#define QRTCALL __cdecl
#else
#define QRTAPI
#define QRTCALL
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    struct _RTSPMJPEGClientData {
        int clientId;
        int state;
        int isWaiting;
        int framesRead;
        int next;
        unsigned char frameQueue[RTSPMJPEGCLIENT_FRAME_QUEUE_LEN * RTSPMJPEGCLIENT_FRAME_BUFFER_SIZE];
        int frameSizes[RTSPMJPEGCLIENT_FRAME_QUEUE_LEN];
    };

    typedef struct _RTSPMJPEGClientData RTSPMJPEGClientData;

    QRTAPI void QRTCALL rtspmjpegclient_start_log(const char *outputLogFile, const char *errorLogFile);
    QRTAPI void QRTCALL rtspmjpegclient_stop_log();
    QRTAPI int QRTCALL rtspmjpegclient_get_log_level();
    QRTAPI void QRTCALL rtspmjpegclient_set_log_level(int logLevel);

    QRTAPI void QRTCALL rtspmjpegclient_log_state(int client);

    QRTAPI int QRTCALL rtspmjpegclient_start(int clientId, const char *address, int asyncCall);
    QRTAPI int QRTCALL rtspmjpegclient_wait(int clientId);
    QRTAPI int QRTCALL rtspmjpegclient_resume(int clientId);
    QRTAPI int QRTCALL rtspmjpegclient_stop(int clientId);
    QRTAPI int QRTCALL rtspmjpegclient_destroy(int clientId);

    QRTAPI int QRTCALL rtspmjpegclient_get_data(int clientId, int frames, int fps, RTSPMJPEGClientData *data);
    QRTAPI int QRTCALL rtspmjpegclient_get_state(int clientId);

#ifdef __cplusplus
} // __cplusplus defined.
#endif

#endif
