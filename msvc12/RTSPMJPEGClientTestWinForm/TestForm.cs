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
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using RTSPMJPEGClientCs;
using log4net;

namespace rtspclientlibtestwinform
{
    /// <summary>
    /// TestForm
    /// </summary>
    public partial class TestForm : Form
    {
        #region ILog
        private ILog _logger { get { return LogManager.GetLogger(GetType()); } }
        #endregion

        #region Constants
        private const string PresetAddress = "rtsp://192.168.2.54/live/mjpeg";
        private const int PresetFramesPerSecond = 2;
        #endregion

        #region Private fields
        private readonly ObservableCollection<ClientDescriptor> _descriptors = new ObservableCollection<ClientDescriptor>();
        private ClientDescriptor _selectedClient = null;

        private int memoryUsageLoggingIntervalInMinutes = 1;
        #endregion

        #region Constructor
        /// <summary>
        /// Initializes a new instance of the <see cref="TestForm"/> class.
        /// </summary>
        public TestForm()
        {
            InitializeComponent();

            _logger.Info("init...");

            SelectLogLevelComboBox.SelectedIndex = 2;

            RTSPMJPEGClientWrapper.Instance.StartLog();
            RTSPMJPEGClientWrapper.Instance.SetLogLevel(Interoperability.LogLevelInfo);
            _logger.Debug("lib log started");

            var clients = ConfigurationManager.AppSettings.AllKeys.Where(c => c.StartsWith("Client")).ToList();

            if (clients.Count > 0)
            {
                foreach (var c in clients)
                {
                    int client;
                    if (!int.TryParse(c.Replace("Client", ""), out client))
                        continue;

                    SelectClientComboBox.Items.Add(new ClientDescriptor
                    {
                        ClientId = client,
                        Address = ConfigurationManager.AppSettings[c],
                        FramesPerSecond = PresetFramesPerSecond
                    });
                }
            }
            
            if (SelectClientComboBox.Items.Count == 0)
            {
                MessageBox.Show("No valid clients in App.config!");

                SelectClientComboBox.Items.Add(new ClientDescriptor
                {
                    ClientId = 0,
                    Address = PresetAddress,
                    FramesPerSecond = PresetFramesPerSecond
                });
            }

            SelectClientComboBox.SelectedIndex = 0;

            _selectedClient = (ClientDescriptor)SelectClientComboBox.SelectedItem;
            if (_selectedClient == null)
                return;

            FramesPerSecondTextBox.Text = _selectedClient.FramesPerSecond.ToString();

            _logger.Debug("selected client = [" + _selectedClient.ClientId + " - " + _selectedClient.Address + " - " + _selectedClient.FramesPerSecond + " fr/s]");
        }
        #endregion

        #region OnClosed
        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);

            _logger.Info("closing...");

            foreach (var d in SelectClientComboBox.Items)
                RTSPMJPEGClientWrapper.Instance.Stop(((ClientDescriptor)d).ClientId);

            foreach (var d in SelectClientComboBox.Items)
                RTSPMJPEGClientWrapper.Instance.Destroy(((ClientDescriptor)d).ClientId);

            RTSPMJPEGClientWrapper.Instance.StopLog();
            _logger.Info("closing... DONE");
        }
        #endregion

        #region Select log level
        private void SelectLogLevelComboBox_SelectionChangeCommitted(object sender, EventArgs e)
        {
            switch (SelectLogLevelComboBox.SelectedIndex)
            {
                case 0:
                    RTSPMJPEGClientWrapper.Instance.SetLogLevel(Interoperability.LogLevelTrace);
                    break;

                case 1:
                    RTSPMJPEGClientWrapper.Instance.SetLogLevel(Interoperability.LogLevelDebug);
                    break;

                case 3:
                    RTSPMJPEGClientWrapper.Instance.SetLogLevel(Interoperability.LogLevelError);
                    break;

                default:
                    RTSPMJPEGClientWrapper.Instance.SetLogLevel(Interoperability.LogLevelInfo);
                    break;
            }
        }
        #endregion

        #region Select client
        private void SelectClientComboBox_SelectionChangeCommitted(object sender, EventArgs e)
        {
            var selectedClient = (ClientDescriptor)SelectClientComboBox.SelectedItem;
            if (selectedClient == null)
                return;

            var state = RTSPMJPEGClientWrapper.Instance.GetState(selectedClient.ClientId);

            GrabTimer.Enabled = state == Interoperability.RTSPMJPEGCLIENT_STATE_LOOPING;
            selectedClient.Data.state = state;

            FramesPerSecondTextBox.Text = selectedClient.FramesPerSecond.ToString();
            StateLabel.Text = RTSPMJPEGClientWrapper.StateToString(state);
            ConnectButton.Text = selectedClient.Data.state == Interoperability.RTSPMJPEGCLIENT_STATE_LOOPING ? "Disconnect" : "Connect";

            _logger.Info("selected client = [" + selectedClient.ClientId + " - " + selectedClient.Address + " - " + selectedClient.FramesPerSecond + " fr/s - " + RTSPMJPEGClientWrapper.StateToString(state) + "]");

            _selectedClient = selectedClient;
        }
        #endregion

        #region Connect
        private void ConnectButton_Click(object sender, EventArgs e)
        {
            if (_selectedClient == null)
                return;

            try
            {
                if (ConnectButton.Text == "Connect")
                {
                    _logger.Info("connecting to " + _selectedClient.Address + "...");

                    if (string.IsNullOrEmpty(_selectedClient.Address))
                    {
                        MessageBox.Show("Insert the address first!");
                        _logger.Error("no address set");
                        return;
                    }

                    if (RTSPMJPEGClientWrapper.Instance.Start(_selectedClient.ClientId, _selectedClient.Address) < 0)
                    {
                        MessageBox.Show("Connect FAILED :( See lib log for further details.");
                        _logger.Error("connect FAILED!");

                        RTSPMJPEGClientWrapper.Instance.Stop(_selectedClient.ClientId);

                        return;
                    }

                    ConnectButton.Text = "Disconnect";

                    GrabTimer.Enabled = true;

                    _logger.Info("connecting to " + _selectedClient.Address + "... DONE");
                }
                else
                {
                    _logger.Info("disconnecting...");

                    GrabTimer.Enabled = false;

                    if (RTSPMJPEGClientWrapper.Instance.Stop(_selectedClient.ClientId) < 0)
                    {
                        MessageBox.Show("Disconnect FAILED :( See lib log for further details.");
                        _logger.Error("disconnecting... FAILED!");

                        return;
                    }

                    ConnectButton.Text = "Connect";

                    _logger.Info("disconnecting... DONE");
                }
            }
            catch (Exception ex)
            {
                _logger.Error("connect/disconnect = " + ex.Message);
            }

            var state = RTSPMJPEGClientWrapper.Instance.GetState(_selectedClient.ClientId);
            _selectedClient.Data.state = state;
            StateLabel.Text = RTSPMJPEGClientWrapper.StateToString(state);
        }
        #endregion

        #region Timer tick
        private void GrabTimer_Tick(object sender, EventArgs e)
        {
            if (_selectedClient == null)
                return;

            // check connection
            try
            {
                var state = RTSPMJPEGClientWrapper.Instance.GetState(_selectedClient.ClientId);
                _selectedClient.Data.state = state;
                StateLabel.Text = RTSPMJPEGClientWrapper.StateToString(state);

                _logger.Debug("state = " + StateLabel.Text);

                if (state >= Interoperability.RTSPMJPEGCLIENT_STATE_INITIALIZING
                    && state <= Interoperability.RTSPMJPEGCLIENT_STATE_STOPPING)
                    return;

                if (state < Interoperability.RTSPMJPEGCLIENT_STATE_INITIALIZING)
                {
                    GrabTimer.Enabled = false;

                    StateLabel.Text = "RECONNECTING";
                    _logger.Error("not LOOPING! Try to reconnect...");

                    if (RTSPMJPEGClientWrapper.Instance.Start(_selectedClient.ClientId, _selectedClient.Address) < 0)
                        _logger.Error("connect FAILED!");

                    GrabTimer.Interval = 1000;
                    GrabTimer.Enabled = true;

                    return;
                }
            }
            catch (Exception ex)
            {
                _logger.Error(ex.Message);
                return;
            }

            try
            {
                var dt = DateTime.UtcNow;

                // log private memory usage
                if (memoryUsageLoggingIntervalInMinutes > 0 && dt.Minute % memoryUsageLoggingIntervalInMinutes == 0 && dt.Second == 0)
                {
                    try
                    {
                        _logger.Info("Private memory used = " + (Process.GetCurrentProcess().PrivateMemorySize64 / 1048576) + "MB");
                    }
                    catch (Exception ex)
                    {
                        _logger.Warn("trying to get memory usage: " + ex.Message);
                    }
                }

                if (_selectedClient.FrameToShow >= _selectedClient.Data.framesRead)
                {
                    int fps;
                    if (!int.TryParse(FramesPerSecondTextBox.Text, out fps))
                        fps = 1;

                    GrabTimer.Interval = 1000 / fps;

                    _logger.Debug("requesting " + fps + " frames...");

                    if (RTSPMJPEGClientWrapper.Instance.Wait(_selectedClient.ClientId) < 0)
                        throw new Exception("error on Wait request");

                    // frames == -1 ===> all new frames
                    var ret = RTSPMJPEGClientWrapper.Instance.GetData(_selectedClient.ClientId, -1, fps, ref _selectedClient.Data);

                    if (RTSPMJPEGClientWrapper.Instance.Resume(_selectedClient.ClientId) < 0)
                        throw new Exception("error on Resume request");

                    if (ret != 0)
                    {
                        StateLabel.Text = "GRAB FAILED";
                        throw new Exception("requesting " + fps + " frames... FAILED!");
                    }

                    _logger.Debug("requesting " + fps + " frames... DONE");

                    _selectedClient.FrameToShow = 0;
                    _selectedClient.Data.framesRead = Math.Min(_selectedClient.Data.framesRead, fps);
                }

                if (_selectedClient.Data.state != Interoperability.RTSPMJPEGCLIENT_STATE_LOOPING || _selectedClient.Data.framesRead == 0)
                    return;

                try
                {
                    using (var mStream = new MemoryStream())
                    {
                        mStream.Write(_selectedClient.Data.frameQueue, _selectedClient.FrameToShow * Interoperability.RTSPMJPEGClientFrameBufferSize, _selectedClient.Data.frameSizes[_selectedClient.FrameToShow]);
                        TestPictureBox.Image = new Bitmap(mStream, false);
                    }
                }
                catch (Exception ex)
                {
                    _logger.Error("frame " + _selectedClient.FrameToShow + " load FAILED! => " + ex.Message);
                }

                _selectedClient.FrameToShow++;
            }
            catch (Exception ex)
            {
                _logger.Error(ex.Message);
            }
        }
        #endregion
    }

    #region ClientDescriptor
    /// <summary>
    /// ClientDescriptor
    /// </summary>
    public class ClientDescriptor
    {
        /// <summary>
        /// The client identifier
        /// </summary>
        public int ClientId;

        /// <summary>
        /// The address
        /// </summary>
        public string Address = null;

        /// <summary>
        /// The data
        /// </summary>
        public RTSPMJPEGClientData Data = new RTSPMJPEGClientData(true);

        /// <summary>
        /// The frame to show
        /// </summary>
        public int FrameToShow = 0;

        /// <summary>
        /// The frames per second
        /// </summary>
        public int FramesPerSecond = 1;

        /// <summary>
        /// Returns a <see cref="System.String" /> that represents this instance.
        /// </summary>
        /// <returns>
        /// A <see cref="System.String" /> that represents this instance.
        /// </returns>
        public override string ToString()
        {
            return ClientId + " - " + Address;
        }
    }
    #endregion
}
