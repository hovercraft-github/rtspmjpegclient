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

#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include "rtspmjpegclient_interface.hpp"

#define TEST_ADDRESS "rtsp://192.168.1.54/live/mjpeg"
#define TEST_FRAME_NUM 10
#define TEST_FRAME_PER_LOOP 2

int main()
{
    rtspmjpegclient_start_log("", "");

    rtspmjpegclient_start(0, TEST_ADDRESS, 1);

    int nframe = 0;
    RTSPMJPEGClientData *data = new RTSPMJPEGClientData;

    while (nframe < TEST_FRAME_NUM)
    {
        rtspmjpegclient_log_state(0);

        data->state = rtspmjpegclient_get_state(0);

        if (data->state != RTSPMJPEGCLIENT_STATE_LOOPING)
        {
            if (data->state < RTSPMJPEGCLIENT_STATE_INITIALIZING)
                rtspmjpegclient_start(0, TEST_ADDRESS, 1);

            std::this_thread::sleep_for(std::chrono::seconds(1));

            continue;
        }

        if (rtspmjpegclient_wait(0) != 0)
            continue;

        rtspmjpegclient_get_data(0, TEST_FRAME_PER_LOOP, 0, data);

        rtspmjpegclient_resume(0);

        for (int i = 0; i < data->framesRead; i++)
        {
            std::string name = "test";
            name += std::to_string(static_cast<long long>(nframe));
            name += ".jpg";
            std::ofstream(name, std::ios::binary).write((char * ) &data->frameQueue[i * RTSPMJPEGCLIENT_FRAME_BUFFER_SIZE], data->frameSizes[i]);

            nframe++;
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    rtspmjpegclient_stop(0);

    delete data;

    rtspmjpegclient_stop_log();

    return 0;
}
