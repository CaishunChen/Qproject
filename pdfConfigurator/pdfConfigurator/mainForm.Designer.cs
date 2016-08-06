namespace pdfConfigurator
{
    partial class mainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(mainForm));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.AlarmTypeCombBox = new System.Windows.Forms.ComboBox();
            this.AlarmDelayCombBox = new System.Windows.Forms.ComboBox();
            this.StartDelayCombBox = new System.Windows.Forms.ComboBox();
            this.SamplingRateCombBox = new System.Windows.Forms.ComboBox();
            this.surfaceLable7 = new System.Windows.Forms.Label();
            this.surfaceLable6 = new System.Windows.Forms.Label();
            this.surfaceLable5 = new System.Windows.Forms.Label();
            this.surfaceLable4 = new System.Windows.Forms.Label();
            this.OriginalTimeZoneTextBox = new System.Windows.Forms.TextBox();
            this.surfaceLable3 = new System.Windows.Forms.Label();
            this.FirmwareVerTextBox = new System.Windows.Forms.TextBox();
            this.surfaceLable2 = new System.Windows.Forms.Label();
            this.serialNumberTextBox = new System.Windows.Forms.TextBox();
            this.surfaceLable1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.LowAlarmATextBox = new System.Windows.Forms.TextBox();
            this.surfaceLable9 = new System.Windows.Forms.Label();
            this.HighAlarmATextBOX = new System.Windows.Forms.TextBox();
            this.surfaceLable8 = new System.Windows.Forms.Label();
            this.SaveButton = new System.Windows.Forms.Button();
            this.SysTimeCalButton = new System.Windows.Forms.Button();
            this.languageCombox = new System.Windows.Forms.ComboBox();
            this.surfaceLable0 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.AlarmTypeCombBox);
            this.groupBox1.Controls.Add(this.AlarmDelayCombBox);
            this.groupBox1.Controls.Add(this.StartDelayCombBox);
            this.groupBox1.Controls.Add(this.SamplingRateCombBox);
            this.groupBox1.Controls.Add(this.surfaceLable7);
            this.groupBox1.Controls.Add(this.surfaceLable6);
            this.groupBox1.Controls.Add(this.surfaceLable5);
            this.groupBox1.Controls.Add(this.surfaceLable4);
            this.groupBox1.Controls.Add(this.OriginalTimeZoneTextBox);
            this.groupBox1.Controls.Add(this.surfaceLable3);
            this.groupBox1.Controls.Add(this.FirmwareVerTextBox);
            this.groupBox1.Controls.Add(this.surfaceLable2);
            this.groupBox1.Controls.Add(this.serialNumberTextBox);
            this.groupBox1.Controls.Add(this.surfaceLable1);
            this.groupBox1.Location = new System.Drawing.Point(6, 38);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(309, 222);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "System Setup";
            // 
            // AlarmTypeCombBox
            // 
            this.AlarmTypeCombBox.FormattingEnabled = true;
            this.AlarmTypeCombBox.Items.AddRange(new object[] {
            "Disable",
            "Single",
            "Cumulative"});
            this.AlarmTypeCombBox.Location = new System.Drawing.Point(147, 187);
            this.AlarmTypeCombBox.Name = "AlarmTypeCombBox";
            this.AlarmTypeCombBox.Size = new System.Drawing.Size(150, 20);
            this.AlarmTypeCombBox.TabIndex = 17;
            this.AlarmTypeCombBox.SelectedIndexChanged += new System.EventHandler(this.AlarmTypeCombBox_SelectedIndexChanged);
            // 
            // AlarmDelayCombBox
            // 
            this.AlarmDelayCombBox.FormattingEnabled = true;
            this.AlarmDelayCombBox.Items.AddRange(new object[] {
            "0 Minutes",
            "5 Minutes",
            "30 Minutes",
            "45 Minutes",
            "60 Minutes",
            "90 Minutes",
            "120 Minutes"});
            this.AlarmDelayCombBox.Location = new System.Drawing.Point(147, 159);
            this.AlarmDelayCombBox.Name = "AlarmDelayCombBox";
            this.AlarmDelayCombBox.Size = new System.Drawing.Size(150, 20);
            this.AlarmDelayCombBox.TabIndex = 16;
            this.AlarmDelayCombBox.SelectedIndexChanged += new System.EventHandler(this.AlarmDelayCombBox_SelectedIndexChanged);
            // 
            // StartDelayCombBox
            // 
            this.StartDelayCombBox.FormattingEnabled = true;
            this.StartDelayCombBox.Items.AddRange(new object[] {
            "0 Minutes",
            "5 Minutes",
            "30 Minutes",
            "45 Minutes",
            "60 Minutes",
            "90 Minutes",
            "120 Minutes"});
            this.StartDelayCombBox.Location = new System.Drawing.Point(147, 133);
            this.StartDelayCombBox.Name = "StartDelayCombBox";
            this.StartDelayCombBox.Size = new System.Drawing.Size(150, 20);
            this.StartDelayCombBox.TabIndex = 15;
            this.StartDelayCombBox.SelectedIndexChanged += new System.EventHandler(this.StartDelayCombBox_SelectedIndexChanged_1);
            // 
            // SamplingRateCombBox
            // 
            this.SamplingRateCombBox.FormattingEnabled = true;
            this.SamplingRateCombBox.Items.AddRange(new object[] {
            "30 Seconds",
            "5 Minutes",
            "10 Minutes",
            "30 Minutes",
            "60 Minutes",
            "90 Minutes",
            "120 Minutes"});
            this.SamplingRateCombBox.Location = new System.Drawing.Point(147, 106);
            this.SamplingRateCombBox.Name = "SamplingRateCombBox";
            this.SamplingRateCombBox.Size = new System.Drawing.Size(150, 20);
            this.SamplingRateCombBox.TabIndex = 14;
            this.SamplingRateCombBox.SelectedIndexChanged += new System.EventHandler(this.SamplingRateCombBox_SelectedIndexChanged);
            // 
            // surfaceLable7
            // 
            this.surfaceLable7.AutoSize = true;
            this.surfaceLable7.Location = new System.Drawing.Point(6, 190);
            this.surfaceLable7.Name = "surfaceLable7";
            this.surfaceLable7.Size = new System.Drawing.Size(71, 12);
            this.surfaceLable7.TabIndex = 12;
            this.surfaceLable7.Text = "Alarm Type:";
            // 
            // surfaceLable6
            // 
            this.surfaceLable6.AutoSize = true;
            this.surfaceLable6.Location = new System.Drawing.Point(6, 163);
            this.surfaceLable6.Name = "surfaceLable6";
            this.surfaceLable6.Size = new System.Drawing.Size(77, 12);
            this.surfaceLable6.TabIndex = 10;
            this.surfaceLable6.Text = "Alarm Delay:";
            // 
            // surfaceLable5
            // 
            this.surfaceLable5.AutoSize = true;
            this.surfaceLable5.Location = new System.Drawing.Point(6, 136);
            this.surfaceLable5.Name = "surfaceLable5";
            this.surfaceLable5.Size = new System.Drawing.Size(77, 12);
            this.surfaceLable5.TabIndex = 8;
            this.surfaceLable5.Text = "Start Delay:";
            // 
            // surfaceLable4
            // 
            this.surfaceLable4.AutoSize = true;
            this.surfaceLable4.Location = new System.Drawing.Point(6, 109);
            this.surfaceLable4.Name = "surfaceLable4";
            this.surfaceLable4.Size = new System.Drawing.Size(89, 12);
            this.surfaceLable4.TabIndex = 6;
            this.surfaceLable4.Text = "Sampling Rate:";
            // 
            // OriginalTimeZoneTextBox
            // 
            this.OriginalTimeZoneTextBox.Location = new System.Drawing.Point(147, 79);
            this.OriginalTimeZoneTextBox.Name = "OriginalTimeZoneTextBox";
            this.OriginalTimeZoneTextBox.ReadOnly = true;
            this.OriginalTimeZoneTextBox.Size = new System.Drawing.Size(150, 21);
            this.OriginalTimeZoneTextBox.TabIndex = 5;
            // 
            // surfaceLable3
            // 
            this.surfaceLable3.AutoSize = true;
            this.surfaceLable3.Location = new System.Drawing.Point(6, 82);
            this.surfaceLable3.Name = "surfaceLable3";
            this.surfaceLable3.Size = new System.Drawing.Size(119, 12);
            this.surfaceLable3.TabIndex = 4;
            this.surfaceLable3.Text = "Original Time Zone:";
            // 
            // FirmwareVerTextBox
            // 
            this.FirmwareVerTextBox.Location = new System.Drawing.Point(147, 52);
            this.FirmwareVerTextBox.Name = "FirmwareVerTextBox";
            this.FirmwareVerTextBox.ReadOnly = true;
            this.FirmwareVerTextBox.Size = new System.Drawing.Size(150, 21);
            this.FirmwareVerTextBox.TabIndex = 3;
            // 
            // surfaceLable2
            // 
            this.surfaceLable2.AutoSize = true;
            this.surfaceLable2.Location = new System.Drawing.Point(6, 55);
            this.surfaceLable2.Name = "surfaceLable2";
            this.surfaceLable2.Size = new System.Drawing.Size(107, 12);
            this.surfaceLable2.TabIndex = 2;
            this.surfaceLable2.Text = "Firmware Version:";
            // 
            // serialNumberTextBox
            // 
            this.serialNumberTextBox.Location = new System.Drawing.Point(147, 25);
            this.serialNumberTextBox.Name = "serialNumberTextBox";
            this.serialNumberTextBox.ReadOnly = true;
            this.serialNumberTextBox.Size = new System.Drawing.Size(150, 21);
            this.serialNumberTextBox.TabIndex = 1;
            // 
            // surfaceLable1
            // 
            this.surfaceLable1.AutoSize = true;
            this.surfaceLable1.Location = new System.Drawing.Point(6, 28);
            this.surfaceLable1.Name = "surfaceLable1";
            this.surfaceLable1.Size = new System.Drawing.Size(89, 12);
            this.surfaceLable1.TabIndex = 0;
            this.surfaceLable1.Text = "Serial Number:";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.groupBox2.Controls.Add(this.LowAlarmATextBox);
            this.groupBox2.Controls.Add(this.surfaceLable9);
            this.groupBox2.Controls.Add(this.HighAlarmATextBOX);
            this.groupBox2.Controls.Add(this.surfaceLable8);
            this.groupBox2.Location = new System.Drawing.Point(6, 264);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(309, 85);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Temperature Measurement";
            // 
            // LowAlarmATextBox
            // 
            this.LowAlarmATextBox.Location = new System.Drawing.Point(147, 49);
            this.LowAlarmATextBox.Name = "LowAlarmATextBox";
            this.LowAlarmATextBox.Size = new System.Drawing.Size(150, 21);
            this.LowAlarmATextBox.TabIndex = 7;
            // 
            // surfaceLable9
            // 
            this.surfaceLable9.AutoSize = true;
            this.surfaceLable9.Location = new System.Drawing.Point(6, 52);
            this.surfaceLable9.Name = "surfaceLable9";
            this.surfaceLable9.Size = new System.Drawing.Size(89, 12);
            this.surfaceLable9.TabIndex = 6;
            this.surfaceLable9.Text = "Low Alarm(℃):";
            // 
            // HighAlarmATextBOX
            // 
            this.HighAlarmATextBOX.Location = new System.Drawing.Point(147, 22);
            this.HighAlarmATextBOX.Name = "HighAlarmATextBOX";
            this.HighAlarmATextBOX.Size = new System.Drawing.Size(150, 21);
            this.HighAlarmATextBOX.TabIndex = 5;
            // 
            // surfaceLable8
            // 
            this.surfaceLable8.AutoSize = true;
            this.surfaceLable8.Location = new System.Drawing.Point(6, 25);
            this.surfaceLable8.Name = "surfaceLable8";
            this.surfaceLable8.Size = new System.Drawing.Size(95, 12);
            this.surfaceLable8.TabIndex = 4;
            this.surfaceLable8.Text = "High Alarm(℃):";
            // 
            // SaveButton
            // 
            this.SaveButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.SaveButton.Location = new System.Drawing.Point(101, 376);
            this.SaveButton.Name = "SaveButton";
            this.SaveButton.Size = new System.Drawing.Size(127, 23);
            this.SaveButton.TabIndex = 3;
            this.SaveButton.Text = "Save Parameter";
            this.SaveButton.UseVisualStyleBackColor = true;
            this.SaveButton.Click += new System.EventHandler(this.SaveButton_Click);
            // 
            // SysTimeCalButton
            // 
            this.SysTimeCalButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.SysTimeCalButton.Location = new System.Drawing.Point(176, 376);
            this.SysTimeCalButton.Name = "SysTimeCalButton";
            this.SysTimeCalButton.Size = new System.Drawing.Size(127, 23);
            this.SysTimeCalButton.TabIndex = 4;
            this.SysTimeCalButton.Text = "System Time Cal";
            this.SysTimeCalButton.UseVisualStyleBackColor = true;
            this.SysTimeCalButton.Visible = false;
            this.SysTimeCalButton.Click += new System.EventHandler(this.SysTimeCalButton_Click);
            // 
            // languageCombox
            // 
            this.languageCombox.FormattingEnabled = true;
            this.languageCombox.Items.AddRange(new object[] {
            "English",
            "German",
            "French",
            "Italian",
            "Spanish"});
            this.languageCombox.Location = new System.Drawing.Point(153, 12);
            this.languageCombox.Name = "languageCombox";
            this.languageCombox.Size = new System.Drawing.Size(150, 20);
            this.languageCombox.TabIndex = 5;
            this.languageCombox.SelectedIndexChanged += new System.EventHandler(this.languageCombox_SelectedIndexChanged);
            // 
            // surfaceLable0
            // 
            this.surfaceLable0.AutoSize = true;
            this.surfaceLable0.Location = new System.Drawing.Point(12, 15);
            this.surfaceLable0.Name = "surfaceLable0";
            this.surfaceLable0.Size = new System.Drawing.Size(59, 12);
            this.surfaceLable0.TabIndex = 6;
            this.surfaceLable0.Text = "Language:";
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(329, 411);
            this.Controls.Add(this.surfaceLable0);
            this.Controls.Add(this.languageCombox);
            this.Controls.Add(this.SaveButton);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.SysTimeCalButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "mainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "DataLogger Configurator";
            this.Load += new System.EventHandler(this.mainForm_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label surfaceLable2;
        private System.Windows.Forms.TextBox serialNumberTextBox;
        private System.Windows.Forms.Label surfaceLable1;
        private System.Windows.Forms.Label surfaceLable7;
        private System.Windows.Forms.Label surfaceLable6;
        private System.Windows.Forms.Label surfaceLable5;
        private System.Windows.Forms.Label surfaceLable4;
        private System.Windows.Forms.TextBox OriginalTimeZoneTextBox;
        private System.Windows.Forms.Label surfaceLable3;
        private System.Windows.Forms.TextBox FirmwareVerTextBox;
        private System.Windows.Forms.ComboBox AlarmTypeCombBox;
        private System.Windows.Forms.ComboBox AlarmDelayCombBox;
        private System.Windows.Forms.ComboBox StartDelayCombBox;
        private System.Windows.Forms.ComboBox SamplingRateCombBox;
        private System.Windows.Forms.TextBox LowAlarmATextBox;
        private System.Windows.Forms.Label surfaceLable9;
        private System.Windows.Forms.TextBox HighAlarmATextBOX;
        private System.Windows.Forms.Label surfaceLable8;
        private System.Windows.Forms.Button SaveButton;
        private System.Windows.Forms.Button SysTimeCalButton;
        private System.Windows.Forms.ComboBox languageCombox;
        private System.Windows.Forms.Label surfaceLable0;

    }
}

