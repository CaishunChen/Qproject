using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace SimulationDataCreator
{
    class Program
    {
        
        static void Main(string[] args)
        {
            Console.WriteLine("*************************************");
            Console.WriteLine("*************************************");

            DateTime time = new DateTime();
            time = DateTime.Now;
            //time.AddSeconds()
            Random rd = new Random();
            UInt16 tempBaseV = 250, HrBaseV = 500, CO2Base = 500;
            int datalength = 8280;
            string[] csvData = new string[datalength];
            //string[] dataLineData = new string[datalength];
            UInt16[] tempData = new UInt16[datalength];
            UInt16[] HrData = new UInt16[datalength];
            UInt16[] Co2Data = new UInt16[datalength];

            FileStream fs1 = new FileStream("./1param/Dtli.dl", FileMode.Create, FileAccess.Write);//创建写入文件 
            StreamWriter sw = new StreamWriter(fs1);

            FileStream fs2 = new FileStream("./2param/Dtli.dl", FileMode.Create, FileAccess.Write);//创建写入文件 
            StreamWriter sw2 = new StreamWriter(fs2);

            FileStream fs3 = new FileStream("./3param/Dtli.dl", FileMode.Create, FileAccess.Write);//创建写入文件 
            StreamWriter sw3 = new StreamWriter(fs3);

            for (int i = 0; i < datalength; i++)
            {
                UInt16 temp = (UInt16)(tempBaseV + rd.Next(-250, 250));
                UInt16 hr = (UInt16)(HrBaseV + rd.Next(-500, 500));
                UInt16 CO2 = (UInt16)(CO2Base + rd.Next(-500, 500));

                string data1 = (i + 1).ToString("D4") + "    " + time.ToString("yy/MM/dd       HH:mm:ss") + "    " +
                    string.Format("{0:000.0}", temp / 10.0) + "  ";
                string data2 = (i + 1).ToString("D4") + " " + time.ToString("yy/MM/dd HH:mm:ss") + "  " + string.Format("{0:000.0}", temp / 10.0)+
                    "  " + string.Format("{0:000.0}", hr / 10.0)+"      ";
                string data3 = (i + 1).ToString("D4") + " " + time.ToString("yy/MM/dd HH:mm:ss") + "  " + string.Format("{0:000.0}", temp / 10.0) +
                    "  " + string.Format("{0:000.0}", hr / 10.0) + " " + string.Format("{0:000.0}", CO2 / 10.0);


                sw.Write(data1);
                sw2.Write(data2);
                sw3.Write(data3);

                time.AddSeconds(30);
                csvData[i] = (temp / 10.0).ToString("F1") + "," + (hr / 10.0).ToString("F1") + "," + (CO2 / 10.0).ToString("F1");

                tempData[i] = temp;
                HrData[i] = hr;
                Co2Data[i] = CO2;
            }
            sw.Close();
            fs1.Close();

            sw2.Close();
            fs2.Close();

            sw3.Close();
            fs3.Close();

            //File.WriteAllLines("Demo.txt", dataLineData);
            File.WriteAllLines("csvData.csv", csvData);


            byte[] byteData = new byte[datalength * 6];
            for (int i = 0, j = 0; i < datalength; i++) 
            {
                byteData[j]=(byte)(tempData[i]>>0);j++;
                byteData[j] = (byte)(tempData[i]>>8); j++;

                byteData[j] = 0; j++;
                byteData[j] = 0; j++;

                byteData[j] = 0; j++;
                byteData[j] = 0; j++;
            }
            File.WriteAllBytes("./1param/Dtbs.dt", byteData);

            for (int i = 0, j = 0; i < datalength; i++)
            {
                byteData[j] = (byte)(tempData[i] >> 0); j++;
                byteData[j] = (byte)(tempData[i] >> 8); j++;

                byteData[j] = (byte)(HrData[i] >> 0); j++;
                byteData[j] = (byte)(HrData[i] >> 8); j++;

                byteData[j] = 0; j++;
                byteData[j] = 0; j++;
            }
            File.WriteAllBytes("./2param/Dtbs.dt", byteData);

            for (int i = 0, j = 0; i < datalength; i++)
            {
                byteData[j] = (byte)(tempData[i] >> 0); j++;
                byteData[j] = (byte)(tempData[i] >> 8); j++;

                byteData[j] = (byte)(HrData[i] >> 0); j++;
                byteData[j] = (byte)(HrData[i] >> 8); j++;

                byteData[j] = (byte)(Co2Data[i] >> 0); j++;
                byteData[j] = (byte)(Co2Data[i] >> 8); j++;
            }
            File.WriteAllBytes("./3param/Dtbs.dt", byteData);
            Console.ReadLine();
        }
    }
}
