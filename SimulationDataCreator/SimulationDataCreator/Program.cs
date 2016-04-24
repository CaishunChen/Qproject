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
            Random rd = new Random();
            UInt16 tempBaseV = 250, HrBaseV = 500;
            int datalength = 8280;
            string[] csvData = new string[datalength];
            UInt16[] tempData = new UInt16[datalength];
            UInt16[] HrData = new UInt16[datalength];
            for (int i = 0; i < datalength; i++)
            {
                UInt16 temp = (UInt16)(tempBaseV + rd.Next(-250, 250));
                UInt16 hr = (UInt16)(HrBaseV + rd.Next(-500, 500));

                csvData[i] = (temp / 10.0).ToString("F1") + "," + (hr / 10.0).ToString("F1");
                tempData[i] = temp;
                HrData[i] = hr;
            }
            File.WriteAllLines("csvData.csv", csvData);
            byte[] byteData = new byte[datalength * 4];
            for (int i = 0, j = 0; i < datalength; i++) 
            {
                byteData[j]=(byte)(tempData[i]>>0);j++;
                byteData[j] = (byte)(tempData[i]>>8); j++;
                byteData[j] = (byte)(HrData[i] >> 0); j++;
                byteData[j] = (byte)(HrData[i]>>8); j++;
            }
            File.WriteAllBytes("byteData.bin", byteData);
            Console.ReadLine();
        }
    }
}
