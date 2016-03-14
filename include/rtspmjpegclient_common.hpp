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

#ifndef RTSPMJPEGCLIENT_COMMON_H
#define RTSPMJPEGCLIENT_COMMON_H

#define RTSPMJPEGCLIENT_FRAME_BUFFER_SIZE 262144 // 2^8
#define RTSPMJPEGCLIENT_FRAME_QUEUE_LEN 30
#define RTSPMJPEGCLIENT_TIMEOUT 20
#define RTSPMJPEGCLIENT_MAX_CLIENT_NUM 20

// STATES
#define RTSPMJPEGCLIENT_STATE_ERROR -1
#define RTSPMJPEGCLIENT_STATE_EMPTY 0
#define RTSPMJPEGCLIENT_STATE_CLEANED 1
#define RTSPMJPEGCLIENT_STATE_INITIALIZING 2
#define RTSPMJPEGCLIENT_STATE_ABORTING 3
#define RTSPMJPEGCLIENT_STATE_STOPPING 4
#define RTSPMJPEGCLIENT_STATE_LOOPING 5


#define RTSPMJPEGCLIENT_ERRBUF_SIZE 512

#endif
