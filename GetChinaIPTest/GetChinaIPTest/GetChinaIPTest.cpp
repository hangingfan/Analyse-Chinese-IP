// GetChinaIPTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <map>
#include <string>

#include<iostream>
#include<fstream>

using namespace std;

map<int, map<int,vector<int>>> m_IpMap;

void GetIntFromIP(string FirstIp, string LastIP)
{
	int FirstIP1 = 0;  //与LastIP1相同
	int FirstIP2 = 0;
	int FirstIP3 = 0;

	int LastIP1 = 0;
	int LastIP2 = 0;
	int LastIP3 = 0;

	int iIndex = FirstIp.find('.');
	FirstIP1 = stoi(FirstIp.substr(0, iIndex));
	FirstIp = FirstIp.substr(iIndex + 1, FirstIp.length() - iIndex - 1);

	iIndex = FirstIp.find('.');
	FirstIP2 = stoi(FirstIp.substr(0, iIndex));
	FirstIp = FirstIp.substr(iIndex + 1, FirstIp.length() - iIndex - 1);

	iIndex = FirstIp.find('.');
	FirstIP3 = stoi(FirstIp.substr(0, iIndex));
	FirstIp = FirstIp.substr(iIndex + 1, FirstIp.length() - iIndex - 1);



	iIndex = LastIP.find('.');
	LastIP1 = stoi(LastIP.substr(0, iIndex));
	LastIP = LastIP.substr(iIndex + 1, LastIP.length() - iIndex - 1);

	iIndex = LastIP.find('.');
	LastIP2 = stoi(LastIP.substr(0, iIndex));
	LastIP = LastIP.substr(iIndex + 1, LastIP.length() - iIndex - 1);

	iIndex = LastIP.find('.');
	LastIP3 = stoi(LastIP.substr(0, iIndex));


	if (FirstIP2 != LastIP2)  //不同时， LastIP3都是255
	{
		map<int, vector<int>> IP2Temp;
		if (m_IpMap.find(FirstIP1) != m_IpMap.end())  //已经存在,先获取之前的数据
		{
			IP2Temp = m_IpMap[FirstIP1];
		}
		

		for (int i = FirstIP2; i <= LastIP2; ++i)
		{
			vector<int> IP3Vec;
			for (int i = 0; i < 256; ++i)
			{
				IP3Vec.push_back(i);
			}

			IP2Temp[i] = IP3Vec;
		}
		m_IpMap[FirstIP1] = IP2Temp;
	}
	else    //相同时，IP3需要每个都记录下来
	{
		map<int, vector<int>> IP2Temp;
		if (m_IpMap.find(FirstIP1) != m_IpMap.end())  //已经存在,先获取之前的数据
		{
			IP2Temp = m_IpMap[FirstIP1];
		}

		vector<int> IP3Vec;
		for (int i = FirstIP3; i <= LastIP3; ++i)
		{
			IP3Vec.push_back(i);
		}
		IP2Temp[FirstIP2] = IP3Vec;

		m_IpMap[FirstIP1] = IP2Temp;
	}
}

bool bRet = false;
map<int, map<int, vector<int>>>::iterator IP1iter;
map<int, vector<int>>::iterator IP2iter;
vector<int>::iterator IP3iter;


bool IsChinaIP(int IP1, int IP2, int IP3)
{
	bRet = false;
	do 
	{
		IP1iter = m_IpMap.find(IP1);
		
		if (IP1iter == m_IpMap.end())	//第一层就不符合了
		{
			break;
		}


		IP2iter = IP1iter->second.find(IP2);

		if (IP2iter == IP1iter->second.end())	//第二层不符合
		{
			break;
		}

		IP3iter = find(IP2iter->second.begin(), IP2iter->second.end(), IP3);

		if (IP3iter == IP2iter->second.end())	//第三层不符合
		{
			break;
		}

		bRet = true;

	} while (false);

	return bRet;
}

int main()
{
	m_IpMap.clear();
	char AllData[300000];

	ifstream myReadFile;
	myReadFile.open("AllChinaIp.txt");

	while (!myReadFile.eof()) {
		myReadFile >> AllData;
	}

	string strData(AllData);
	strData = strData.substr(3, strData.length() - 3);

	myReadFile.close();

	int iIndex = 0;   //分行符
	int iIpIndex = 0;	//  IP之间的分行符
	int iSerial = 0;
	while ((iIndex = strData.find('&')) != -1)
	{
		if((iIpIndex = strData.find('-')) == -1)
		{
			++iSerial;
			continue;
		}

		GetIntFromIP(strData.substr(0, iIpIndex), strData.substr(iIpIndex + 1, iIndex - iIpIndex - 2));

		strData = strData.substr(iIndex + 1, strData.length() - 1);

		++iSerial;
	}

	bool bRet = IsChinaIP(125, 119, 84);

    return 0;
}

