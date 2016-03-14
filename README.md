RTSP (MJPEG) Client library 
===========================
Tiny (simple and raw, multi stream) client library (written in C++11) to read a
RTP MJPEG streams. It uses ffmpeg library to setup a RTSP connection.

RTSPMJPEGClientCs is a C# wrapper.
RTSPMJPEGClientTestWinForm is a WinForm small client (VS2013).
Qt directory contains Qt projects to build the lib and test programs (tested
with Ubuntu 14.04).

Requirements
------------
To compile the following libraries are needed:

 * [log4cplus](https://github.com/log4cplus/log4cplus)
 * [ffmpeg](https://www.ffmpeg.org/)
 * [log4net](https://logging.apache.org/log4net/) for C# stuff (NuGet)

Getting started
---------------
See tests code to get a first view.

Each client (stream) is managed by a thread. You need to start a client (the
max number of client is hardcoded) by calling the interface method

> rtspmjpegclient_start(int clientId, const char *address, int asyncCall)

passing the id (stream number) and the RTSP address. Pass asyncCall = 1 to
to avoid to wait initialization.
Once initialized, a client keeps reading frames in a circular buffer (max
frame number hardcoded). Each time you want to grab frames call

> rtspclient_wait(int clientId)
> rtspclient_get_data(int clientId, int frames, int fps, RTSPClientData *data)
> rtspclient_resume(int clientId)

wait and resume are not mandatory but avoid client thread to overwrite current
frames. get_data fills a RTSPMJPEGClientData struct with the frames read. Each
time get_data is called, the new frames counter is zeroed.

> rtspmjpegclient_get_state(int clientId)

to get the client/stream state.

> rtspmjpegclient_destroy(int clientId)

to close a stream (thread ends but client object is not deleted).

> rtspmjpegclient_destroy(int clientId)

to completely delete a client object and free memory.

States
------
From rtspmjpegclient_common.h:

 * RTSPMJPEGCLIENT_STATE_ERROR -1
 * RTSPMJPEGCLIENT_STATE_EMPTY 0
 * RTSPMJPEGCLIENT_STATE_CLEANED 1
 * RTSPMJPEGCLIENT_STATE_INITIALIZING 2
 * RTSPMJPEGCLIENT_STATE_ABORTING 3
 * RTSPMJPEGCLIENT_STATE_STOPPING 4
 * RTSPMJPEGCLIENT_STATE_LOOPING 5

License
-------
Copyright 2016 Qualibit S.r.l.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
