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

using System;
using System.Runtime.InteropServices;

namespace RTSPMJPEGClientCs
{
    /// <summary>
    /// Interoperability
    /// </summary>
    public static class Interoperability
    {
        #region Constants
        /// <summary>
        /// The DLL path
        /// </summary>
        public const string DllPath = "RTSPMJPEGClient.dll";

        /// <summary>
        /// The RTSP client frame queue length
        /// </summary>
        public const int RTSPMJPEGClientFrameQueueLen = 30;

        /// <summary>
        /// The RTSP client frame buffer size
        /// </summary>
        public const int RTSPMJPEGClientFrameBufferSize = 262144;

        /// <summary>
        /// The rtspclient state ERROR
        /// </summary>
        public const int RTSPMJPEGCLIENT_STATE_ERROR = -1;

        /// <summary>
        /// The rtspclient state EMPTY
        /// </summary>
        public const int RTSPMJPEGCLIENT_STATE_EMPTY = 0;

        /// <summary>
        /// The rtspclient state CLEANED
        /// </summary>
        public const int RTSPMJPEGCLIENT_STATE_CLEANED = 1;

        /// <summary>
        /// The rtspclient state INITIALIZING
        /// </summary>
        public const int RTSPMJPEGCLIENT_STATE_INITIALIZING = 2;

        /// <summary>
        /// The rtspclient state ABORTING
        /// </summary>
        public const int RTSPMJPEGCLIENT_STATE_ABORTING = 3;

        /// <summary>
        /// The rtspclient state STOPPING
        /// </summary>
        public const int RTSPMJPEGCLIENT_STATE_STOPPING = 4;

        /// <summary>
        /// The rtspclient state LOOPING
        /// </summary>
        public const int RTSPMJPEGCLIENT_STATE_LOOPING = 5;

        /// <summary>
        /// The log level error
        /// </summary>
        public const int LogLevelError = 40000;

        /// <summary>
        /// The log level information
        /// </summary>
        public const int LogLevelInfo = 20000;

        /// <summary>
        /// The log level debug
        /// </summary>
        public const int LogLevelDebug = 10000;

        /// <summary>
        /// The log level trace
        /// </summary>
        public const int LogLevelTrace = 0;
        #endregion

        #region Public methods
        /// <summary>
        /// RTSP client start log.
        /// </summary>
        /// <param name="outputLogFile">The output log file.</param>
        /// <param name="errorLogFile">The error log file.</param>
        [DllImport(DllPath, EntryPoint = "rtspmjpegclient_start_log", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RTSPClientStartLog([MarshalAs(UnmanagedType.LPStr)]string outputLogFile, [MarshalAs(UnmanagedType.LPStr)]string errorLogFile);

        /// <summary>
        /// RTSP client stop log.
        /// </summary>
        [DllImport(DllPath, EntryPoint = "rtspmjpegclient_stop_log", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RTSPClientStopLog();

        /// <summary>
        /// RTSPs the client get log level.
        /// </summary>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "rtspmjpegclient_get_log_level", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RTSPClientGetLogLevel();

        /// <summary>
        /// RTSP client set log level.
        /// </summary>
        /// <param name="logLevel">The log level.</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "rtspmjpegclient_set_log_level", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RTSPClientSetLogLevel(int logLevel);

        /// <summary>
        /// RTSP client log state.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        [DllImport(DllPath, EntryPoint = "rtspmjpegclient_log_state", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RTSPClientLogState(int clientId);

        /// <summary>
        /// RTSP client start.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <param name="address">The address.</param>
        /// <param name="asyncCall">The asynchronous call.</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "rtspmjpegclient_start", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RTSPClientStart(int clientId, [MarshalAs(UnmanagedType.LPStr)]string address, int asyncCall);

        /// <summary>
        /// RTSP client wait.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "rtspmjpegclient_wait", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RTSPClientWait(int clientId);

        /// <summary>
        /// RTSP client resume.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "rtspmjpegclient_resume", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RTSPClientResume(int clientId);

        /// <summary>
        /// RTSP client stop.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "rtspmjpegclient_stop", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RTSPClientStop(int clientId);

        /// <summary>
        /// RTSP client destroy.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "rtspmjpegclient_destroy", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RTSPClientDestroy(int clientId);

        /// <summary>
        /// RTSP client get data.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <param name="frames">The frames.</param>
        /// <param name="fps">The FPS.</param>
        /// <param name="pData">The p data.</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "rtspmjpegclient_get_data", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RTSPClientGetData(int clientId, int frames, int fps, IntPtr pData);

        /// <summary>
        /// RTSP client get state.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "rtspmjpegclient_get_state", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RTSPClientGetState(int clientId);
        #endregion
    }

    #region RTSPMJPEGClientData
    /// <summary>
    /// RTSPMJPEGClientData
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RTSPMJPEGClientData
    {
        /// <summary>
        /// The client identifier
        /// </summary>
        public int clientId;

        /// <summary>
        /// The state
        /// </summary>
        public int state;

        /// <summary>
        /// The is waiting
        /// </summary>
        public int isWaiting;

        /// <summary>
        /// The frames read
        /// </summary>
        public int framesRead;

        /// <summary>
        /// The next
        /// </summary>
        public int next;

        /// <summary>
        /// The frame queue
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Interoperability.RTSPMJPEGClientFrameQueueLen * Interoperability.RTSPMJPEGClientFrameBufferSize, ArraySubType = UnmanagedType.I1)]
        public byte[] frameQueue;

        /// <summary>
        /// The frame sizes
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Interoperability.RTSPMJPEGClientFrameQueueLen, ArraySubType = UnmanagedType.I4)]
        public int[] frameSizes;

        /// <summary>
        /// Initializes a new instance of the <see cref="RTSPClientData"/> struct.
        /// </summary>
        /// <param name="init">if set to <c>true</c> [initialize].</param>
        public RTSPMJPEGClientData(bool init)
        {
            clientId = -1;
            state = 0;
            isWaiting = 0;
            framesRead = 0;
            next = 0;

            frameQueue = init ? new byte[Interoperability.RTSPMJPEGClientFrameQueueLen * Interoperability.RTSPMJPEGClientFrameBufferSize] : null;
            frameSizes = init ? new int[Interoperability.RTSPMJPEGClientFrameQueueLen] : null;
        }
    }
    #endregion
}
