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
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.OriginalTimeZoneTextBox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.FirmwareVerTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.serialNumberTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.LowAlarmATextBox = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.HighAlarmATextBOX = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.SaveButton = new System.Windows.Forms.Button();
            this.SysTimeCalButton = new System.Windows.Forms.Button();
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
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.OriginalTimeZoneTextBox);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.FirmwareVerTextBox);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.serialNumberTextBox);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(6, 12);
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
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 190);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(71, 12);
            this.label7.TabIndex = 12;
            this.label7.Text = "Alarm Type:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 163);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(77, 12);
            this.label6.TabIndex = 10;
            this.label6.Text = "Alarm Delay:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 136);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(77, 12);
            this.label5.TabIndex = 8;
            this.label5.Text = "Start Delay:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 109);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(89, 12);
            this.label4.TabIndex = 6;
            this.label4.Text = "Sampling Rate:";
            // 
            // OriginalTimeZoneTextBox
            // 
            this.OriginalTimeZoneTextBox.Location = new System.Drawing.Point(147, 79);
            this.OriginalTimeZoneTextBox.Name = "OriginalTimeZoneTextBox";
            this.OriginalTimeZoneTextBox.ReadOnly = true;
            this.OriginalTimeZoneTextBox.Size = new System.Drawing.Size(150, 21);
            this.OriginalTimeZoneTextBox.TabIndex = 5;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 82);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(119, 12);
            this.label3.TabIndex = 4;
            this.label3.Text = "Original Time Zone:";
            // 
            // FirmwareVerTextBox
            // 
            this.FirmwareVerTextBox.Location = new System.Drawing.Point(147, 52);
            this.FirmwareVerTextBox.Name = "FirmwareVerTextBox";
            this.FirmwareVerTextBox.ReadOnly = true;
            this.FirmwareVerTextBox.Size = new System.Drawing.Size(150, 21);
            this.FirmwareVerTextBox.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 55);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(107, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "Firmware Version:";
            // 
            // serialNumberTextBox
            // 
            this.serialNumberTextBox.Location = new System.Drawing.Point(147, 25);
            this.serialNumberTextBox.Name = "serialNumberTextBox";
            this.serialNumberTextBox.ReadOnly = true;
            this.serialNumberTextBox.Size = new System.Drawing.Size(150, 21);
            this.serialNumberTextBox.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(89, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "Serial Number:";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.groupBox2.Controls.Add(this.LowAlarmATextBox);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.HighAlarmATextBOX);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Location = new System.Drawing.Point(6, 240);
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
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 52);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(95, 12);
            this.label8.TabIndex = 6;
            this.label8.Text = "LLow Alarm(℃):";
            // 
            // HighAlarmATextBOX
            // 
            this.HighAlarmATextBOX.Location = new System.Drawing.Point(147, 22);
            this.HighAlarmATextBOX.Name = "HighAlarmATextBOX";
            this.HighAlarmATextBOX.Size = new System.Drawing.Size(150, 21);
            this.HighAlarmATextBOX.TabIndex = 5;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(6, 25);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(95, 12);
            this.label9.TabIndex = 4;
            this.label9.Text = "High Alarm(℃):";
            // 
            // SaveButton
            // 
            this.SaveButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.SaveButton.Location = new System.Drawing.Point(101, 335);
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
            this.SysTimeCalButton.Location = new System.Drawing.Point(176, 335);
            this.SysTimeCalButton.Name = "SysTimeCalButton";
            this.SysTimeCalButton.Size = new System.Drawing.Size(127, 23);
            this.SysTimeCalButton.TabIndex = 4;
            this.SysTimeCalButton.Text = "System Time Cal";
            this.SysTimeCalButton.UseVisualStyleBackColor = true;
            this.SysTimeCalButton.Visible = false;
            this.SysTimeCalButton.Click += new System.EventHandler(this.SysTimeCalButton_Click);
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(329, 370);
            this.Controls.Add(this.SysTimeCalButton);
            this.Controls.Add(this.SaveButton);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
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

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox serialNumberTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox OriginalTimeZoneTextBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox FirmwareVerTextBox;
        private System.Windows.Forms.ComboBox AlarmTypeCombBox;
        private System.Windows.Forms.ComboBox AlarmDelayCombBox;
        private System.Windows.Forms.ComboBox StartDelayCombBox;
        private System.Windows.Forms.ComboBox SamplingRateCombBox;
        private System.Windows.Forms.TextBox LowAlarmATextBox;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox HighAlarmATextBOX;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button SaveButton;
        private System.Windows.Forms.Button SysTimeCalButton;

    }
}

