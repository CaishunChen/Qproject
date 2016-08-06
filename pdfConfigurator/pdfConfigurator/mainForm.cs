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
        /// <summary>
        /// 日期转换成unix时间戳
        /// </summary>
        /// <param name="dateTime"></param>
        /// <returns></returns>
         long DateTimeToUnixTimestamp(DateTime dateTime)
        {
            var start = new DateTime(1970, 1, 1, 0, 0, 0, dateTime.Kind);
            return Convert.ToInt64((dateTime - start).TotalSeconds);
        }

        ///// <summary>
        ///// unix时间戳转换成日期
        ///// </summary>
        ///// <param name="unixTimeStamp">时间戳（秒）</param>
        ///// <returns></returns>
        //DateTime UnixTimestampToDateTime(this DateTime target, long timestamp)
        //{
        //    var start = new DateTime(1970, 1, 1, 0, 0, 0, target.Kind);
        //    return start.AddSeconds(timestamp);
        //}
        string configFilePath="./Sys/Conf.cf";
        string timeFilePath = "./Sys/Time.tm";
        string RsmpFilePath = "./Sys/Rsmp.rm";
        string LanguageFilePath = "./Sys/langu.la";
        string pdfFilePath = "./dataLog.pdf";
        string SysLanguage = null;
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
            if (File.Exists(LanguageFilePath) == false)
            {
                File.WriteAllText(LanguageFilePath, "French");
            }

            SysLanguage = File.ReadAllText(LanguageFilePath);
            SurfaceUpdate(SysLanguage);
            languageCombox.Text = SysLanguage;


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
                    PdfConfig.AlarmDelay_s = (UInt16)(UInt16.Parse(AlarmDelayCombBox.Text.Remove(AlarmDelayCombBox.Text.Length - 7)) * 60);
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
                


                UInt32 timeStamp = (UInt32)DateTimeToUnixTimestamp(DateTime.Now);
                byte[] dataArray = BitConverter.GetBytes(timeStamp);
                File.WriteAllBytes(timeFilePath, dataArray);

                if (File.Exists(RsmpFilePath) == false)
                {
                    byte[] timeBytes = new byte[4];
                    timeBytes[0] = 0;
                    timeBytes[1] = 0;
                    timeBytes[2] = 0;
                    timeBytes[3] = 2;
                    File.WriteAllBytes(RsmpFilePath, timeBytes);

                }
                else
                {
                    byte[] timeBytes = File.ReadAllBytes(RsmpFilePath);
                    if (timeBytes.Length >= 4)
                    {
                        timeBytes[3] = 2;
                    }
                    else
                    {
                        timeBytes = new byte[4];
                        timeBytes[0] = 0;
                        timeBytes[1] = 0;
                        timeBytes[2] = 0;
                        timeBytes[3] = 2;

                    }
                    File.Delete(RsmpFilePath);
                    File.WriteAllBytes(RsmpFilePath, timeBytes);
                }
                string pdflang=pdfLanguageRead();
                if (pdflang != null) 
                {
                    if (pdflang != languageCombox.Text) 
                    {
                        pdfLanguageUpdate(pdflang, languageCombox.Text);
                    }
                }
                MessageBox.Show("Save Done");
            }

            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString());
            }
        }
        string pdfLanguageRead() 
        {
            string langu = null;
            StreamReader sr = new StreamReader(pdfFilePath);
            char[] strbuf=new char[3000];
            sr.Read(strbuf, 0, 3000);
            string pdfString = new string(strbuf);
            if(pdfString.Contains("Specification"))langu="English";
            else if (pdfString.Contains("Gerätespezifikation")) langu = "German";
            else if (pdfString.Contains("Spécifications de l'appareil")) langu = "French";
            else if (pdfString.Contains("Specifica apparecchio")) langu = "Italian";
            else if (pdfString.Contains("Especificaciones del aparato")) langu = "Spanish";
            return langu;
        }
        
        void pdfLanguageUpdate(string oldLang,string newLang) 
        {
            string allpdfStr = File.ReadAllText(pdfFilePath);
            string[] oldlangtemp=null,newlangtemp=null;
            switch (oldLang) 
            {
                case "English": oldlangtemp = pdfEnglishInfo; break;
                case "German": oldlangtemp = pdfGermanInfo; break;
                case "French": oldlangtemp = pdfFrenchInfo; break;
                case "Italian": oldlangtemp = pdfItalianInfo; break;
                case "Spanish": oldlangtemp = pdfSpanishInfo; break;
            }
            switch (newLang)
            {
                case "English": newlangtemp = pdfEnglishInfo; break;
                case "German": newlangtemp = pdfGermanInfo; break;
                case "French": newlangtemp = pdfFrenchInfo; break;
                case "Italian": newlangtemp = pdfItalianInfo; break;
                case "Spanish": newlangtemp = pdfSpanishInfo; break;
            }
            for (int i = 0; i < oldlangtemp.Length; i++) 
            {
                //if(allpdfStr.Contains(oldlangtemp[i]))
                //{
                    allpdfStr=allpdfStr.Replace(oldlangtemp[i], newlangtemp[i]);
                //}
                
            }
            File.WriteAllText(newLang + ".pdf", allpdfStr);
        }
        private void SysTimeCalButton_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK == MessageBox.Show("确认需要校准设备时间吗", "Confirm Message", MessageBoxButtons.OKCancel, MessageBoxIcon.Question)) 
            {
                try
                {
                    UInt32 timeStamp = (UInt32)DateTimeToUnixTimestamp(DateTime.Now);
                    byte[] dataArray = BitConverter.GetBytes(timeStamp);
                    File.WriteAllBytes(timeFilePath, dataArray);
                    
                    if(File.Exists(RsmpFilePath)==false)
                    {
                        byte[] timeBytes = new byte[4];
                        timeBytes[0] = 0;
                        timeBytes[1] = 0;
                        timeBytes[2] = 0;
                        timeBytes[3] = 2;
                        File.WriteAllBytes(RsmpFilePath, timeBytes);

                    }
                    else 
                    {
                        byte[] timeBytes=File.ReadAllBytes(RsmpFilePath);
                        if (timeBytes.Length >= 4)
                        {
                            timeBytes[3] = 2;
                        }
                        else 
                        {
                            timeBytes = new byte[4];
                            timeBytes[0] = 0;
                            timeBytes[1] = 0;
                            timeBytes[2] = 0;
                            timeBytes[3] = 2;

                        }
                        File.Delete(RsmpFilePath);
                        File.WriteAllBytes(RsmpFilePath, timeBytes);
                    }
                    MessageBox.Show("请拔出设备并重新插入设备使时间校准生效");
                }
                catch (Exception ex) 
                {
                    MessageBox.Show(ex.Message.ToString());
                }
    
            }

        }
        string[] EnglishSurface = 
        {
            "Language:",
            "Serial Number:",
            "Firmware Version:",
            "Original Time Zone:",
            "Sampling Rate:",
            "Start Delay:",
            "Alarm Delay:",
            "Alarm Type:",
            "High Alarm(℃):",
            "Low Alarm(℃):",
            "System Setup",
            "Temperature Measurement",
            "Save Parameter"
        };
        string[] GermanSurface = 
        {
            "Sprache:",
            "SN:",
            "Firmware Version:",
            "Original Zeitzone:",
            "Messinterval:",
            "Startverzögerung:",
            "Alarmverzögerung:",
            "Alarmtyp:",
            "Oberer Alarm(℃):",
            "Unterer Alarm(℃):",
            "System Setup",
            "Temperature Measurement",
            "Save Parameter"
        };
        string[] FrenchSurface = 
        {
            "Langue:",
            "SN:",
            "Version firmware:",
            "Fuseau hor. orig.:",
            "Intervalle mesure:",
            "Intervalle mesure:",
            "Temporis. Alarme:",
            "Type d'alarme:",
            "Alarme supérieure(℃):",
            "Alarme inférieure(℃):",
            "System Setup",
            "Temperature Measurement",
            "Save Parameter"
        };
        string[] ItalianSurface = 
        {
            "Lingua:",
            "SN:",
            "Versione firmware:",
            "Fuso orario orig.:",
            "Intervallo misura:",
            "Ritardo avvio:",
            "Ritardo allarme:",
            "Tipo allarme:",
            "Allarme superiore(℃):",
            "Allarme inferiore(℃):",
            "System Setup",
            "Temperature Measurement",
            "Save Parameter"
        };
        string[] SpanishSurface = 
        {
            "Ldioma:",
            "Nºserie\":",
            "Versión firmware:",
            "Zona horaria orig.:",
            "Intervalo medición:",
            "Temp. arranque:",
            "retardo alarma:",
            "Tipo de alarma:",
            "Alarma alta(℃):",
            "Alarma baja(℃):",
            "System Setup",
            "Temperature Measurement",
            "Save Parameter"
        };
        string[] SimplifiedChineseSurface = 
        {
            "语言:",
            "序列号:",
            "软件版本:",
            "时区:",
            "采样速率:",
            "开始延迟:",
            "报警延迟:",
            "报警类型:",
            "高报警值(℃):",
            "低报警值(℃):",
            "系统设置",
            "温度测量",
            "保存参数"
        };
        string[] TraditionalChineseSurface = 
        {
            "语言:",
            "序列号:",
            "软件版本:",
            "时区:",
            "采样速率:",
            "开始延迟:",
            "报警延迟:",
            "报警类型:",
            "高报警值(℃):",
            "低报警值(℃):",
            "系统设置",
            "温度测量",
            "保存参数"
        };
        void SurfaceUpdate(string langu) 
        {
            switch (langu)
            {
                case "English": 
                    {
                        surfaceLable0.Text = EnglishSurface[0];
                        surfaceLable1.Text = EnglishSurface[1];
                        surfaceLable2.Text = EnglishSurface[2];
                        surfaceLable3.Text = EnglishSurface[3];
                        surfaceLable4.Text = EnglishSurface[4];
                        surfaceLable5.Text = EnglishSurface[5];
                        surfaceLable6.Text = EnglishSurface[6];
                        surfaceLable7.Text = EnglishSurface[7];
                        surfaceLable8.Text = EnglishSurface[8];
                        surfaceLable9.Text = EnglishSurface[9];
                        groupBox1.Text = EnglishSurface[10];
                        groupBox2.Text = EnglishSurface[11];
                        SaveButton.Text = EnglishSurface[12];

                    } break;
                case "German":
                    {
                        surfaceLable0.Text = GermanSurface[0];
                        surfaceLable1.Text = GermanSurface[1];
                        surfaceLable2.Text = GermanSurface[2];
                        surfaceLable3.Text = GermanSurface[3];
                        surfaceLable4.Text = GermanSurface[4];
                        surfaceLable5.Text = GermanSurface[5];
                        surfaceLable6.Text = GermanSurface[6];
                        surfaceLable7.Text = GermanSurface[7];
                        surfaceLable8.Text = GermanSurface[8];
                        surfaceLable9.Text = GermanSurface[9];
                        groupBox1.Text = GermanSurface[10];
                        groupBox2.Text = GermanSurface[11];
                        SaveButton.Text = GermanSurface[12];

                    } break;
                case "French":
                    {
                        surfaceLable0.Text = FrenchSurface[0];
                        surfaceLable1.Text = FrenchSurface[1];
                        surfaceLable2.Text = FrenchSurface[2];
                        surfaceLable3.Text = FrenchSurface[3];
                        surfaceLable4.Text = FrenchSurface[4];
                        surfaceLable5.Text = FrenchSurface[5];
                        surfaceLable6.Text = FrenchSurface[6];
                        surfaceLable7.Text = FrenchSurface[7];
                        surfaceLable8.Text = FrenchSurface[8];
                        surfaceLable9.Text = FrenchSurface[9];
                        groupBox1.Text = FrenchSurface[10];
                        groupBox2.Text = FrenchSurface[11];
                        SaveButton.Text = FrenchSurface[12];

                    } break;
                case "Italian":
                    {
                        surfaceLable0.Text = ItalianSurface[0];
                        surfaceLable1.Text = ItalianSurface[1];
                        surfaceLable2.Text = ItalianSurface[2];
                        surfaceLable3.Text = ItalianSurface[3];
                        surfaceLable4.Text = ItalianSurface[4];
                        surfaceLable5.Text = ItalianSurface[5];
                        surfaceLable6.Text = ItalianSurface[6];
                        surfaceLable7.Text = ItalianSurface[7];
                        surfaceLable8.Text = ItalianSurface[8];
                        surfaceLable9.Text = ItalianSurface[9];
                        groupBox1.Text = ItalianSurface[10];
                        groupBox2.Text = ItalianSurface[11];
                        SaveButton.Text = ItalianSurface[12];

                    } break;
                case "Spanish":
                    {
                        surfaceLable0.Text = SpanishSurface[0];
                        surfaceLable1.Text = SpanishSurface[1];
                        surfaceLable2.Text = SpanishSurface[2];
                        surfaceLable3.Text = SpanishSurface[3];
                        surfaceLable4.Text = SpanishSurface[4];
                        surfaceLable5.Text = SpanishSurface[5];
                        surfaceLable6.Text = SpanishSurface[6];
                        surfaceLable7.Text = SpanishSurface[7];
                        surfaceLable8.Text = SpanishSurface[8];
                        surfaceLable9.Text = SpanishSurface[9];
                        groupBox1.Text = SpanishSurface[10];
                        groupBox2.Text = SpanishSurface[11];
                        SaveButton.Text = SpanishSurface[12];

                    } break;
            }

        }
        private void languageCombox_SelectedIndexChanged(object sender, EventArgs e)
        {
            SurfaceUpdate(languageCombox.Text);
            //File.WriteAllText(LanguageFilePath, languageCombox.Text);
        }

        private void StartDelayCombBox_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void AlarmDelayCombBox_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void AlarmTypeCombBox_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void SamplingRateCombBox_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void StartDelayCombBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {

        }
        string[] pdfEnglishInfo =
        {
            "SN      ",
            "      Device Specification      ",
			"Production date                 ",
			"Production lot                  ",
			"Firmware version                ",
			"Original time zone              ",
			"Start                           ",
			"Finish                          ",
			"Sampling Rate                   ",
			"Start Delay                     ",
			"Readings                        ",
			"Alarm Delay                     ",
			"Alarm Type                      ",
			"Statistics(excludes Start Delay)",
			"High Alarm                      ",
			"Low Alarm                       ",
			"Maximum                         ",
			"Average                         ",
			"Minimum                         ",
			"Std. Dev.                       ",
			"Total time within               ",
			"Total time above                ",
//			"Total time below                ",
			"        File Information        ",
			"File created                    ",
			"         Marked Events          ",
			"Date ",
			"Time  ",
			"Disable                         ",
			"Single event                    ",
			"Cumulative                      ",
        };
		string[] pdfGermanInfo =
        {
            "SN      ",
            "      Gerätespezifikation       ",
			"Herstellungs Datum              ",
			"Herstellungs Los                ",
			"Firmware Version                ",
			"Original Zeitzone               ",
			"Start                           ",
			"Ende                            ",
			"Messinterval                    ",
			"Startverzögerung                ",
			"Messungen                       ",
			"Alarmverzögerung                ",
			"Alarmtyp                        ",
			"Statistik(ohne Startverzögerung)",
			"Oberer Alarm                    ",
			"Unterer Alarm                   ",
			"Max Temp                        ",
			"Durchschnits Temp               ",
			"Min Temp                        ",
			"Std. Abweichung                 ",
			"Ges. Zeit innerh                ",
			"Ges. Zeit ausserh               ",
//			"Total time below                ",
			"        Datei-Information       ",
			"Datei erstellt                  ",
			"      Markierte Ereignisse      ",
			"Datum",
			"Zeit  ",
			"deaktivieren                    ",
			"Einzelereignis                  ",
			"Kumulativ                       ",
        };
		string[] pdfFrenchInfo =
        {
            "SN      ",
            "  Spécifications de l'appareil  ",
			"Date de fabricat.               ",
			"Lot de fabrication              ",
			"Version firmware                ",
			"Fuseau hor. orig.               ",
			"Début                           ",
			"Fin                             ",
			"Intervalle mesure               ",
			"Temporisation                   ",
			"Mesures                         ",
			"Temporis. Alarme                ",
			"Type d'alarme                   ",
			"Statistique (sans temporisation)",
			"Alarme supérieure               ",
			"Alarme inférieure               ",
			"Temp Max                        ",
			"Temp moyenne                    ",
			"Temp min                        ",
			"Déviation stand.                ",
			"Durée totale int.               ",
			"Durée totale ext.               ",
//			"Total time below                ",
			"     Information du fichier     ",
			"Fichier créé                    ",
			"        Événement marqué        ",
			"Date ",
			"Heure ",
			"Désactiver                      ",
			"Événement unique                ",
			"Cumulative                      ",
        };
        string[] pdfItalianInfo =
        {
            "SN      ",
            "      Specifica apparecchio     ",
			"Data produzione                 ",
			"Lotto produzione                ",
			"Versione firmware               ",
			"Fuso orario orig.               ",
			"Avvio                           ",
			"Fine                            ",
			"Intervallo misura               ",
			"Ritardo avvio                   ",
			"Letture                         ",
			"Ritardo allarme                 ",
			"Tipo allarme                    ",
			"Statistiche (escl.ritar. avvio) ",
			"Allarme superiore               ",
			"Allarme inferiore               ",
			"Temp. max.                      ",
			"Temp. media                     ",
			"Temp. min.                      ",
			"Deviaz. std.                    ",
			"Orario com. entro               ",
			"Orario com. oltre               ",
//			"Total time below                ",
			"        Informazione file       ",
			"File creato                     ",
			"     Eventi contrassegnati      ",
			"Data ",
			"Orario",
			"Disattivare                     ",
			"Evento singolo                  ",
			"Cumulativo                      ",
        };
        string[] pdfSpanishInfo =
        {
            "Nºserie\"",
            "  Especificaciones del aparato  ",
			"Fecha fabricación               ",
			"Lote fabricación                ",
			"Versión firmware                ",
			"Zona horaria orig.              ",
			"Arranqu                         ",
			"Fin                             ",
			"Intervalo medición              ",
			"Temp. arranque                  ",
			"Mediciones                      ",
			"Retardo alarma                  ",
			"Tipo de alarma                  ",
			"Estadística (sin ret. arranque) ",
			"Alarma alta                     ",
			"Alarma baja                     ",
			"Temperatura máx.                ",
			"Temperatura media               ",
			"Temperatura mín.                ",
			"Desviación estánd.              ",
			"Total horas int.                ",
			"Total horas ext.                ",
//			"Total time below                ",
			"     Información del fichero    ",
			"Fichero creado                  ",
			"       Eventos marcados         ",
			"Fecha",
			"Hora  ",
			"Desactivar                      ",
			"Evento indiv.                   ",
			"Cumulativo                      ",
        };


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
