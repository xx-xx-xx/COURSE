#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

//������������·��
#define  TXT_PATH_NAME    "1.txt"
void getTxtPartName(char *partName, string txtPathName);

int main()
{
	//ͳ��txt�е�������
	ifstream countSamplesNum(TXT_PATH_NAME);
	string samplesPathName;
	
	int samplesNum = 0;         //txt�е�������
	int splitNum = 0;           //�ƻ���ֵķ���
	int averageSamplesNum = 0;  //ƽ��ÿ�ݵ�����
	int remainder = 0;          //����

	//ѭ����ȡtxt�ĸ���
	while (1)
	{
		if (!getline(countSamplesNum, samplesPathName))
		{
			break;
		}
		if (samplesPathName.empty())
		{
			continue;
		}
		samplesNum++;
	}
	cout << "����txt����������" << samplesNum << endl;
	cout << "��������������";
	cin >> splitNum;
	
	averageSamplesNum = samplesNum / splitNum;
	remainder = samplesNum % splitNum;
	char  dispParams[1024];   //����ԭ�ļ�������
	char saveNameExt[1024];   //�������ļ������֣�Դ�ļ�������
	getTxtPartName(dispParams, TXT_PATH_NAME);   //������txt��·����ȡtxt�ļ���

	if (remainder) //��������
	{
		cout << endl << endl << "��ֽ�����£�" << endl << "ǰ" << (splitNum - 1) << "�ݵ�����Ϊ��" << averageSamplesNum
			<< ",   ���1�ݵ�����Ϊ��" << (samplesNum - averageSamplesNum * (splitNum - 1)) << endl << endl;
	}
	else   //��������
	{
		cout << endl << endl << "��ֽ�����£�" << endl << splitNum << "�ݵ�������Ϊ��" << averageSamplesNum << endl << endl;
	}

	//ʵ�ֲ�֣��ļ�����
	ifstream totalSamplesPath(TXT_PATH_NAME);
	string singleSamplesPath;

	for (int i = 0; i < splitNum - 1; i++)
	{
		sprintf(saveNameExt, "%s_%d.txt", dispParams, i);
		FILE *labelfilename = fopen(saveNameExt, "w+t");

		for (int j = 0; j < averageSamplesNum; j++)
		{
			getline(totalSamplesPath, singleSamplesPath);
			const char* ch = singleSamplesPath.c_str();
			
			if (j == 0) {
				fprintf(labelfilename, "%s", ch);
			}
			else {
				fprintf(labelfilename, "\n%s", ch);
			}
		}
		fclose(labelfilename);
	}

	//�������1��
	sprintf(saveNameExt, "%s_%d.txt", dispParams, (splitNum - 1));
	FILE *labelfilename = fopen(saveNameExt, "w+t");
	int flg = 0;

	while (getline(totalSamplesPath, singleSamplesPath))
	{
		const char* ch = singleSamplesPath.c_str();
		if (0 == flg)
		{
			fprintf(labelfilename, "%s", ch);
		}
		else
		{
			fprintf(labelfilename, "\n%s", ch);
		}
		flg++;
	}

	fclose(labelfilename);

	system("pause");
	return 0;
}

//��txtPathName��ȡ���һ����\\�����롰.��֮ǰ�Ĳ�������
void getTxtPartName(char *partName, string txtPathName)
{
	char tmpChar1[1024] = { '0', };
	char tmpChar2[1024] = { '0', };
	
	strcpy(tmpChar1, txtPathName.c_str());
	
	int pathNameLen = strlen(tmpChar1);
	int pos = pathNameLen;
	
	while (pos > 0)
	{
		pos--;
		if (tmpChar1[pos] == '\\')
		{
			pos++;
			break;
		}
	}

	memcpy(tmpChar2, tmpChar1 + pos, pathNameLen - pos);
	pos = strlen(tmpChar2);
	
	while (pos > 0)
	{
		if (tmpChar2[pos] == '.')
			break;
		pos--;
	}

	tmpChar2[pos] = 0;
	sprintf(partName, "%s", tmpChar2);

}
