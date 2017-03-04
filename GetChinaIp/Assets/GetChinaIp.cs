using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using UnityEngine;

public class GetChinaIp : MonoBehaviour {
    string strData = "";
    int iNum = 0;
    public void AnalyseIP()
    {
        strData = "";
        iNum = 0;

        string[] lineArray;

        TextAsset binAsset = Resources.Load("All", typeof(TextAsset)) as TextAsset;
        lineArray = binAsset.text.Split("\n"[0]);

#if UNITY_EDITOR
        for (int i = 0; i < lineArray.Length - 1; ++i)
        {
            lineArray[i] = lineArray[i].Substring(0, lineArray[i].Length - 1);
        }

        //最后一行必须这样写，不然最后一个字会被抹去
        lineArray[lineArray.Length - 1] = lineArray[lineArray.Length - 1].Substring(0, lineArray[lineArray.Length - 1].Length);
#endif

        for (int i = 0; i < lineArray.Length; i++)
        {
            if(lineArray[i].Contains("CN") == false)
            {
                continue;
            }
            else
            {
                if (lineArray[i].Contains("ipv4") == false)
                {
                    continue;
                }
                else
                {
                    ++iNum;

                    string strTemp = lineArray[i];
                    //apnic|CN|ipv4|103.80.184.0|1024|20161202|allocated
                    int iIndex1  = strTemp.IndexOf("ipv4|", 0);        //第三个|
                    iIndex1 += 5;
                    strTemp = strTemp.Substring(iIndex1, strTemp.Length - iIndex1 - 1);
                    int iIndex2 = strTemp.IndexOf("|");
                    string strFirstIP = strTemp.Substring(0, iIndex2);

                    strTemp = strTemp.Substring(iIndex2 + 1, strTemp.Length - iIndex2 - 1);
                    int iIndex3 = strTemp.IndexOf("|");

                    
                    string strNum = strTemp.Substring(0, iIndex3);

                    WriteNewContent(strFirstIP, int.Parse(strNum));
                }
            }
        }

        Debug.Log("Num" + iNum);
        File.WriteAllText(Application.dataPath + "/Resources/AllChinaIp.txt", strData, Encoding.UTF8);
    }


    /// ***********************************************************************
    /// author   : lta
    /// Created  : 03-03-2017
    /// purpose  : 将IP和NUM分析， 得到最大的IP字段
    /// ***********************************************************************
    private void WriteNewContent(string strTempIp, int iNum)
    {
        int iFirstIP1 = 0;  //开始IP段的第一个IP， 112.11.22.33
        int iFirstIP2 = 0;
        int iFirstIP3 = 0;
        int iFirstIP4 = 0;

        int iLastIP1 = 0;  //结束IP段的最后一个IP， 112.12.23.66
        int iLastIP2 = 0;
        int iLastIP3 = 0;
        int iLastIP4 = 0;

        string strIp = strTempIp;

        int iIndex = strIp.IndexOf('.');
        iLastIP1 = iFirstIP1 = int.Parse(strIp.Substring(0, iIndex));
        strIp = strIp.Substring(iIndex + 1, strIp.Length - iIndex - 1);


        iIndex = strIp.IndexOf('.');
        iLastIP2 = iFirstIP2 = int.Parse(strIp.Substring(0, iIndex));
        strIp = strIp.Substring(iIndex + 1, strIp.Length - iIndex - 1);

        iIndex = strIp.IndexOf('.');
        iLastIP3 =  iFirstIP3 = int.Parse(strIp.Substring(0, iIndex));
        strIp = strIp.Substring(iIndex + 1, strIp.Length - iIndex - 1);

        iLastIP4 = iFirstIP4 = int.Parse(strIp);

        if(iNum < 257)
        {
            iLastIP4 = 255;
        }
        else if(iNum < 65537)
        {
            iLastIP4 = 255;
            iLastIP3 = Mathf.Min(255, iFirstIP3 + iNum / 256);
        }
        else if(iNum < 16777217)
        {
            iLastIP4 = 255;
            iLastIP3 = 255;
            iLastIP2 = Mathf.Min(255, iFirstIP2 + iNum / 65536);
        }

        string strLastIp = iLastIP1 + "." + iLastIP2 + "." + iLastIP3 + "." + iLastIP4;

        strData += strTempIp + "-" + strLastIp + "&";
    }
}
