namespace rtspclientlibtestwinform
{
    partial class TestForm
    {
        /// <summary>
        /// Variabile di progettazione necessaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Liberare le risorse in uso.
        /// </summary>
        /// <param name="disposing">ha valore true se le risorse gestite devono essere eliminate, false in caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Codice generato da Progettazione Windows Form

        /// <summary>
        /// Metodo necessario per il supporto della finestra di progettazione. Non modificare
        /// il contenuto del metodo con l'editor di codice.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.TestPictureBox = new System.Windows.Forms.PictureBox();
            this.FramesPerSecondTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.ConnectButton = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.GrabTimer = new System.Windows.Forms.Timer(this.components);
            this.StateLabel = new System.Windows.Forms.Label();
            this.ControlsPanel = new System.Windows.Forms.Panel();
            this.SelectLogLevelComboBox = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.SelectClientComboBox = new System.Windows.Forms.ComboBox();
            ((System.ComponentModel.ISupportInitialize)(this.TestPictureBox)).BeginInit();
            this.ControlsPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // TestPictureBox
            // 
            this.TestPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.TestPictureBox.Location = new System.Drawing.Point(0, 0);
            this.TestPictureBox.Name = "TestPictureBox";
            this.TestPictureBox.Size = new System.Drawing.Size(640, 360);
            this.TestPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.TestPictureBox.TabIndex = 0;
            this.TestPictureBox.TabStop = false;
            // 
            // FramesPerSecondTextBox
            // 
            this.FramesPerSecondTextBox.Location = new System.Drawing.Point(491, 4);
            this.FramesPerSecondTextBox.Name = "FramesPerSecondTextBox";
            this.FramesPerSecondTextBox.Size = new System.Drawing.Size(30, 20);
            this.FramesPerSecondTextBox.TabIndex = 2;
            this.FramesPerSecondTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 7);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Address";
            // 
            // ConnectButton
            // 
            this.ConnectButton.Location = new System.Drawing.Point(228, 2);
            this.ConnectButton.Name = "ConnectButton";
            this.ConnectButton.Size = new System.Drawing.Size(75, 23);
            this.ConnectButton.TabIndex = 5;
            this.ConnectButton.Text = "Connect";
            this.ConnectButton.UseVisualStyleBackColor = true;
            this.ConnectButton.Click += new System.EventHandler(this.ConnectButton_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(459, 6);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(23, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "fr/s";
            // 
            // GrabTimer
            // 
            this.GrabTimer.Interval = 1000;
            this.GrabTimer.Tick += new System.EventHandler(this.GrabTimer_Tick);
            // 
            // StateLabel
            // 
            this.StateLabel.AutoSize = true;
            this.StateLabel.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.StateLabel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.StateLabel.Location = new System.Drawing.Point(535, 6);
            this.StateLabel.MinimumSize = new System.Drawing.Size(100, 2);
            this.StateLabel.Name = "StateLabel";
            this.StateLabel.Size = new System.Drawing.Size(100, 15);
            this.StateLabel.TabIndex = 7;
            this.StateLabel.Text = "-";
            this.StateLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ControlsPanel
            // 
            this.ControlsPanel.Controls.Add(this.SelectLogLevelComboBox);
            this.ControlsPanel.Controls.Add(this.label3);
            this.ControlsPanel.Controls.Add(this.SelectClientComboBox);
            this.ControlsPanel.Controls.Add(this.label1);
            this.ControlsPanel.Controls.Add(this.StateLabel);
            this.ControlsPanel.Controls.Add(this.FramesPerSecondTextBox);
            this.ControlsPanel.Controls.Add(this.label2);
            this.ControlsPanel.Controls.Add(this.ConnectButton);
            this.ControlsPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.ControlsPanel.Location = new System.Drawing.Point(0, 365);
            this.ControlsPanel.Name = "ControlsPanel";
            this.ControlsPanel.Size = new System.Drawing.Size(640, 28);
            this.ControlsPanel.TabIndex = 8;
            // 
            // SelectLogLevelComboBox
            // 
            this.SelectLogLevelComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.SelectLogLevelComboBox.FormattingEnabled = true;
            this.SelectLogLevelComboBox.Items.AddRange(new object[] {
            "TRACE",
            "DEBUG",
            "INFO",
            "ERROR"});
            this.SelectLogLevelComboBox.Location = new System.Drawing.Point(365, 3);
            this.SelectLogLevelComboBox.Name = "SelectLogLevelComboBox";
            this.SelectLogLevelComboBox.Size = new System.Drawing.Size(88, 21);
            this.SelectLogLevelComboBox.TabIndex = 10;
            this.SelectLogLevelComboBox.SelectionChangeCommitted += new System.EventHandler(this.SelectLogLevelComboBox_SelectionChangeCommitted);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(309, 8);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(50, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Log level";
            // 
            // SelectClientComboBox
            // 
            this.SelectClientComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.SelectClientComboBox.FormattingEnabled = true;
            this.SelectClientComboBox.Location = new System.Drawing.Point(58, 3);
            this.SelectClientComboBox.Name = "SelectClientComboBox";
            this.SelectClientComboBox.Size = new System.Drawing.Size(164, 21);
            this.SelectClientComboBox.TabIndex = 8;
            this.SelectClientComboBox.SelectionChangeCommitted += new System.EventHandler(this.SelectClientComboBox_SelectionChangeCommitted);
            // 
            // TestForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(640, 393);
            this.Controls.Add(this.ControlsPanel);
            this.Controls.Add(this.TestPictureBox);
            this.Name = "TestForm";
            this.Text = "RTSP Client Lib Test";
            ((System.ComponentModel.ISupportInitialize)(this.TestPictureBox)).EndInit();
            this.ControlsPanel.ResumeLayout(false);
            this.ControlsPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox TestPictureBox;
        private System.Windows.Forms.TextBox FramesPerSecondTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button ConnectButton;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Timer GrabTimer;
        private System.Windows.Forms.Label StateLabel;
        private System.Windows.Forms.Panel ControlsPanel;
        private System.Windows.Forms.ComboBox SelectClientComboBox;
        private System.Windows.Forms.ComboBox SelectLogLevelComboBox;
        private System.Windows.Forms.Label label3;
    }
}

