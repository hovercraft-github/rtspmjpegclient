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
using log4net;

namespace RTSPMJPEGClientCs
{
    /// <summary>
    /// RTSPMJPEGClientWrapper
    /// </summary>
    public class RTSPMJPEGClientWrapper
    {
        #region ILog
        private ILog Logger { get { return LogManager.GetLogger(GetType()); } }
        #endregion

        #region Constants
        /// <summary>
        /// The lo g_ leve l_ error
        /// </summary>
        public const int LogLevelError = 40000;

        /// <summary>
        /// The lo g_ leve l_ information
        /// </summary>
        public const int LogLevelInfo = 20000;

        /// <summary>
        /// The lo g_ leve l_ debug
        /// </summary>
        public const int LogLevelDebug = 10000;

        /// <summary>
        /// The lo g_ leve l_ trace
        /// </summary>
        public const int LogLevelTrace = 0;
        #endregion

        #region Private fields
        private static RTSPMJPEGClientWrapper _instance = null;
        private static object _lockObject = new object();
        private static int _runningClients = 0;
        #endregion

        #region Public properties
        /// <summary>
        /// Gets the running clients.
        /// </summary>
        /// <value>
        /// The running clients.
        /// </value>
        public int RunningClients
        {
            get { return _runningClients; }
        }
        #endregion

        #region Constructor
        private RTSPMJPEGClientWrapper() { }
        #endregion

        #region Instance
        /// <summary>
        /// Gets the instance.
        /// </summary>
        /// <value>
        /// The instance.
        /// </value>
        public static RTSPMJPEGClientWrapper Instance
        {
            get
            {
                lock (_lockObject)
                {
                    if (_instance == null)
                        _instance = new RTSPMJPEGClientWrapper();
                }

                return _instance;
            }
        }
        #endregion

        #region Public methods
        #region Clients
        /// <summary>
        /// Incs the running clients.
        /// </summary>
        /// <returns></returns>
        public int IncRunningClients()
        {
            lock (_lockObject)
            {
                _runningClients++;

                return RunningClients;
            }
        }

        /// <summary>
        /// Decimals the running clients.
        /// </summary>
        public void DecRunningClients()
        {
            lock (_lockObject)
            {
                _runningClients--;
            }
        }
        #endregion

        #region Log
        /// <summary>
        /// Starts the log.
        /// </summary>
        /// <param name="outFile">The out file.</param>
        /// <param name="errFile">The error file.</param>
        public void StartLog(string outFile = "", string errFile = "")
        {
            try
            {
                Interoperability.RTSPClientStartLog(outFile, errFile);
            }
            catch (Exception e)
            {
                Logger.Error("startLog: " + e.Message);
            }
        }

        /// <summary>
        /// Stops the log.
        /// </summary>
        /// <returns></returns>
        public void StopLog()
        {
            try
            {
                Interoperability.RTSPClientStopLog();
            }
            catch (Exception e)
            {
                Logger.Error(e.Message);
            }
        }

        /// <summary>
        /// Gets the log level.
        /// </summary>
        /// <returns></returns>
        public int GetLogLevel()
        {
            try
            {
                return Interoperability.RTSPClientGetLogLevel();
            }
            catch (Exception e)
            {
                Logger.Error(e.Message);
            }

            return -1;
        }

        /// <summary>
        /// Sets the log level.
        /// </summary>
        /// <param name="logLevel">The log level.</param>
        /// <returns></returns>
        public void SetLogLevel(int logLevel)
        {
            try
            {
                Interoperability.RTSPClientSetLogLevel(logLevel);
            }
            catch (Exception e)
            {
                Logger.Error(e.Message);
            }
        }

        /// <summary>
        /// Logs the state.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        public void LogState(int clientId)
        {
            try
            {
                Interoperability.RTSPClientLogState(clientId);
            }
            catch (Exception e)
            {
                Logger.Error(e.Message);
            }
        }
        #endregion

        #region Client
        /// <summary>
        /// States to string.
        /// </summary>
        /// <param name="state">The state.</param>
        /// <returns></returns>
        public static string StateToString(int state)
        {
            switch (state)
            {
                case Interoperability.RTSPMJPEGCLIENT_STATE_ERROR:
                    return "ERROR";

                case Interoperability.RTSPMJPEGCLIENT_STATE_EMPTY:
                    return "EMPTY";

                case Interoperability.RTSPMJPEGCLIENT_STATE_CLEANED:
                    return "CLEANED";

                case Interoperability.RTSPMJPEGCLIENT_STATE_INITIALIZING:
                    return "INITIALIZING";

                case Interoperability.RTSPMJPEGCLIENT_STATE_LOOPING:
                    return "LOOPING";

                case Interoperability.RTSPMJPEGCLIENT_STATE_STOPPING:
                    return "STOPPING";

                case Interoperability.RTSPMJPEGCLIENT_STATE_ABORTING:
                    return "ABORTING";

                default:
                    return "UNKNOWN";
            }
        }

        /// <summary>
        /// Starts the specified address.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <param name="address">The address.</param>
        /// <param name="isAsyncCall">if set to <c>true</c> [is asynchronous call].</param>
        /// <returns></returns>
        public int Start(int clientId, string address, bool isAsyncCall = true)
        {
            try
            {
                return Interoperability.RTSPClientStart(clientId, address, isAsyncCall ? 1 : 0);
            }
            catch (Exception e)
            {
                Logger.Error(e.Message);
            }

            return -1;
        }

        /// <summary>
        /// Waits the specified client identifier.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <returns></returns>
        public int Wait(int clientId)
        {
            try
            {
                return Interoperability.RTSPClientWait(clientId);
            }
            catch (Exception e)
            {
                Logger.Error(e.Message);
            }

            return -1;
        }

        /// <summary>
        /// Resumes the specified client identifier.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <returns></returns>
        public int Resume(int clientId)
        {
            try
            {
                return Interoperability.RTSPClientResume(clientId);
            }
            catch (Exception e)
            {
                Logger.Error(e.Message);
            }

            return -1;
        }

        /// <summary>
        /// Stops the specified client identifier.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <returns></returns>
        public int Stop(int clientId)
        {
            try
            {
                return Interoperability.RTSPClientStop(clientId);
            }
            catch (Exception e)
            {
                Logger.Error(e.Message);
            }

            return -1;
        }

        /// <summary>
        /// Destroys the specified client identifier.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <returns></returns>
        public int Destroy(int clientId)
        {
            try
            {
                return Interoperability.RTSPClientDestroy(clientId);
            }
            catch (Exception e)
            {
                Logger.Error(e.Message);
            }

            return -1;
        }

        /// <summary>
        /// Gets the data.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <param name="frames">The frames.</param>
        /// <param name="fps">The FPS.</param>
        /// <param name="data">The data.</param>
        /// <returns></returns>
        public int GetData(int clientId, int frames, int fps, ref RTSPMJPEGClientData data)
        {
            int ret = -1;

            var pData = new IntPtr();

            try
            {
                Logger.Debug("allocing space...");
                pData = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(RTSPMJPEGClientData)));
                Marshal.StructureToPtr(data, pData, false);
                Logger.Debug("allocing space... DONE!");

                Logger.Debug("getting data...");
                ret = Interoperability.RTSPClientGetData(clientId, frames, fps, pData);
                

                Logger.Debug("getting result back...");
                data = (RTSPMJPEGClientData)Marshal.PtrToStructure(pData, typeof(RTSPMJPEGClientData));
                Logger.Debug("getting result back... DONE!");
            }
            catch (Exception e)
            {
                    Logger.Error(e.Message);
                    data = new RTSPMJPEGClientData();
            }
            finally
            {
                Logger.Debug("freeing memory...");
                Marshal.FreeHGlobal(pData);
                Logger.Debug("freeing memory... DONE!");
            }

            return ret;
        }

        /// <summary>
        /// Gets the state.
        /// </summary>
        /// <param name="clientId">The client identifier.</param>
        /// <returns></returns>
        public int GetState(int clientId)
        {
            try
            {
                return Interoperability.RTSPClientGetState(clientId);
            }
            catch (Exception e)
            {
                Logger.Error(e.Message);
            }

            return -1;
        }
        #endregion
        #endregion
    }
}
