using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace pdfConfigurator
{
    public partial class mainForm : Form
    {
        public mainForm()
        {
            InitializeComponent();
        }

        string configFilePath="./Sys/Conf.cf";
        pdfConfigParam PdfConfig; 
        private void mainForm_Load(object sender, EventArgs e)
        {
            PdfConfig = new pdfConfigParam(configFilePath);
            serialNumberTextBox.Text = PdfConfig.serialNumber;
            FirmwareVerTextBox.Text = PdfConfig.firmwareVersion;
            OriginalTimeZoneTextBox.Text = PdfConfig.originalTimeZone;
            //SamplingRateCombBox.Items.IsReadOnly = true;
            if (PdfConfig.SamplingRate_s <= 60)
            {
                SamplingRateCombBox.Text = PdfConfig.SamplingRate_s.ToString() + " Seconds";
            }
            else 
            {
                SamplingRateCombBox.Text = (PdfConfig.SamplingRate_s / 60).ToString() + " Minutes";
            }
            if (PdfConfig.StartDelay_s <= 60)
            {
                StartDelayCombBox.Text = PdfConfig.StartDelay_s.ToString() + " Seconds";
            }
            else
            {
                StartDelayCombBox.Text = (PdfConfig.StartDelay_s / 60).ToString() + " Minutes";
            }
            if (PdfConfig.AlarmDelay_s <= 60)
            {
                AlarmDelayCombBox.Text = PdfConfig.AlarmDelay_s.ToString() + " Seconds";
            }
            else
            {
                AlarmDelayCombBox.Text = (PdfConfig.AlarmDelay_s / 60).ToString() + " Minutes";
            }
            AlarmTypeCombBox.Text = PdfConfig.alarmType.ToString();
            HighAlarmATextBOX.Text = PdfConfig.ParamA_HighAlarm.ToString();
            LowAlarmATextBox.Text = PdfConfig.ParamA_LowAlarm.ToString();
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            try 
            {
                if (SamplingRateCombBox.Text.Contains("Seconds"))
                {
                    
                    PdfConfig.SamplingRate_s = UInt16.Parse(SamplingRateCombBox.Text.Remove(SamplingRateCombBox.Text.Length - 7));
                }
                else 
                {
                    PdfConfig.SamplingRate_s = (UInt16)(UInt16.Parse(SamplingRateCombBox.Text.Remove(SamplingRateCombBox.Text.Length - 7)) * 60);
                }
                if (StartDelayCombBox.Text.Contains("Seconds"))
                {
                    PdfConfig.StartDelay_s = UInt16.Parse(StartDelayCombBox.Text.Remove(StartDelayCombBox.Text.Length - 7));
                }
                else
                {
                    PdfConfig.StartDelay_s = (UInt16)(UInt16.Parse(StartDelayCombBox.Text.Remove(StartDelayCombBox.Text.Length - 7)) * 60);
                }
                if (AlarmDelayCombBox.Text.Contains("Seconds"))
                {
                    PdfConfig.AlarmDelay_s = UInt16.Parse(AlarmDelayCombBox.Text.Remove(AlarmDelayCombBox.Text.Length - 7));
                }
                else
                {
                    PdfConfig.AlarmDelay_s = (UInt16)(UInt16.Parse(AlarmDelayCombBox.Text.Remove(AlarmDelayCombBox.Text.Length - 7))*60);
                }
                switch (AlarmTypeCombBox.Text) 
                {
                    case "Disable": PdfConfig.alarmType = pdfConfigParam.ALARMTYPE.Disable; break;
                    case "Single": PdfConfig.alarmType = pdfConfigParam.ALARMTYPE.Single; break;
                    case "Cumulative": PdfConfig.alarmType = pdfConfigParam.ALARMTYPE.Cumulative; break;
                    default: break;
                }
                PdfConfig.ParamA_HighAlarm = float.Parse(HighAlarmATextBOX.Text);
                PdfConfig.ParamA_LowAlarm = float.Parse(LowAlarmATextBox.Text);
                PdfConfig.SaveToFile(configFilePath);
                MessageBox.Show("Save Done");

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString());
            }
        }
    }
    class pdfConfigParam 
    {
        public enum ALARMTYPE
        {
            Disable,
            Single,
            Cumulative
        }
        char[] CompanyName=new char[20];
        public string companyName;
	    char[] Serialnumber=new char[7];
        public string serialNumber;
	    char[] ProductionDate=new char[32];
        public string productionDate;
	    char[] ProductLot=new char[32];
        public string productLot;
	    char[] FirmwareVersion=new char[32];
        public string firmwareVersion;
	    char[] OriginalTimeZone=new char[32];
        public string originalTimeZone;
        public UInt16 SamplingRate_s;
        public UInt16 StartDelay_s;
        public UInt16 AlarmDelay_s;
	    char AlarmType;
        public ALARMTYPE alarmType;
        char ParameterCount;
	    char[] ParamA_Name=new char[32];
	    char[] ParamB_Name=new char[32];
	    char[] ParamC_Name=new char[32];
	    char[] ParamA_Unit=new char[4];
	    char[] ParamB_Unit=new char[4];
        char[] ParamC_Unit = new char[4];
	    public float ParamA_HighAlarm;
        public float ParamB_HighAlarm;
        public float ParamC_HighAlarm;
        public float ParamA_LowAlarm;
        public float ParamB_LowAlarm;
        public float ParamC_LowAlarm;

        byte[] paramBytes=new byte[1];
        public pdfConfigParam(string filePath) 
        {
            try
            {
                paramBytes =File.ReadAllBytes(filePath);
                int i = 0;
                Array.Copy(paramBytes, i, CompanyName, 0, 20); i += 20;
                Array.Copy(paramBytes, i, Serialnumber, 0, 7); i += 7;
                Array.Copy(paramBytes, i, ProductionDate, 0, 32); i += 32;
                Array.Copy(paramBytes, i, ProductLot, 0, 32); i += 32;
                Array.Copy(paramBytes, i, FirmwareVersion, 0, 32); i += 32;
                Array.Copy(paramBytes, i, OriginalTimeZone, 0, 32); i += 32;
                i++;

                UInt16 templow = (UInt16)paramBytes[i++];
                UInt16 tempHigh = (UInt16)paramBytes[i++];
                tempHigh <<= 8;
                SamplingRate_s = (UInt16)(tempHigh + templow);

                templow = (UInt16)paramBytes[i++];
                tempHigh = (UInt16)paramBytes[i++];
                tempHigh <<= 8;
                StartDelay_s = (UInt16)(tempHigh + templow);

                templow = (UInt16)paramBytes[i++];
                tempHigh = (UInt16)paramBytes[i++];
                tempHigh <<= 8;
                AlarmDelay_s = (UInt16)(tempHigh + templow);

                AlarmType = (char)paramBytes[i++];
                ParameterCount = (char)paramBytes[i++];
                Array.Copy(paramBytes, i, ParamA_Name, 0, 32); i += 32;
                Array.Copy(paramBytes, i, ParamB_Name, 0, 32); i += 32;
                Array.Copy(paramBytes, i, ParamC_Name, 0, 32); i += 32;
                Array.Copy(paramBytes, i, ParamA_Unit, 0, 4); i += 4;
                Array.Copy(paramBytes, i, ParamB_Unit, 0, 4); i += 4;
                Array.Copy(paramBytes, i, ParamC_Unit, 0, 4); i += 4;

                ParamA_HighAlarm=BitConverter.ToSingle(paramBytes, i); i += 4;
                ParamB_HighAlarm = BitConverter.ToSingle(paramBytes, i); i += 4;
                ParamC_HighAlarm = BitConverter.ToSingle(paramBytes, i); i += 4;
                ParamA_LowAlarm = BitConverter.ToSingle(paramBytes, i); i += 4;
                ParamB_LowAlarm = BitConverter.ToSingle(paramBytes, i); i += 4;
                ParamC_LowAlarm = BitConverter.ToSingle(paramBytes, i); i += 4;

                companyName = new string(CompanyName, 0, 20);
                serialNumber = new string(Serialnumber, 0, 7);
                productionDate = new string(ProductionDate, 0, 32);
                productLot = new string(ProductLot, 0, 32);
                firmwareVersion = new string(FirmwareVersion, 0, 32);
                originalTimeZone = new string(OriginalTimeZone, 0, 32);
                alarmType = (ALARMTYPE)AlarmType;
            }
            catch (Exception ex) 
            {
                MessageBox.Show(ex.Message.ToString());
            }
        }
        public void SaveToFile(string filePath)
        {
            try
            {
                byte[] temp=BitConverter.GetBytes(SamplingRate_s);
                Array.Copy(temp, 0, paramBytes, 156, 2);
                temp = BitConverter.GetBytes(StartDelay_s);
                Array.Copy(temp, 0, paramBytes, 158, 2);
                temp = BitConverter.GetBytes(AlarmDelay_s);
                Array.Copy(temp, 0, paramBytes, 160, 2);
                AlarmType = (char)alarmType;
                paramBytes[162] = (byte)AlarmType;
                temp = BitConverter.GetBytes(ParamA_HighAlarm);
                Array.Copy(temp, 0, paramBytes, 272, 4);
                temp = BitConverter.GetBytes(ParamA_LowAlarm);
                Array.Copy(temp, 0, paramBytes, 284, 4);
                File.WriteAllBytes(filePath, paramBytes);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString());
            }
        }

    }
}
