#include <atlstr.h>
#include <stdio.h>
#include <vector>

struct displayMode
{
	int m_nWidth;
	int m_nHeight;

	displayMode(int nWidth_, int nHeight_) :
		m_nWidth(nWidth_), m_nHeight(nHeight_)
	{

	}
};

// �õ�������ʾ����ģʽ
void GetAllDisplayMode(std::vector<displayMode>& vecDisplayMode_)
{
	vecDisplayMode_.clear();

	BOOL		bRetVal;
	CString		sDevMode;

	int iMode = 0;

	DEVMODE 	 devmode;

	do
	{
		bRetVal = ::EnumDisplaySettings(NULL, iMode, &devmode);
		iMode++;
		if (bRetVal)
		{
			// ������ʾ���������Ϣ.. û���õ�.��ע�͵�
			//printf("%d x %d, %d bits %dhtz \n",
			//	devmode.dmPelsWidth, devmode.dmPelsHeight,
			//	devmode.dmBitsPerPel, devmode.dmDisplayFrequency);


			std::vector<displayMode>::iterator itBeg = vecDisplayMode_.begin();
			std::vector<displayMode>::iterator itEnd = vecDisplayMode_.end();
			for (NULL; itBeg != itEnd; ++itBeg)
			{
				if ((itBeg->m_nWidth == devmode.dmPelsWidth) && (itBeg->m_nHeight == devmode.dmPelsHeight))
				{
					break;
				}
			}

			if (itBeg == itEnd)
			{
				vecDisplayMode_.push_back(displayMode(devmode.dmPelsWidth, devmode.dmPelsHeight));
			}
			
		}
	} while (bRetVal);

}

// �õ���ǰ��ʾ����ģʽ
displayMode GetCurrentDisplayMode()
{
	BOOL		bRetVal;
	CString		sDevMode;

	int iMode = 0;

	DEVMODE 	 devmode;
	bRetVal = ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
	return displayMode(devmode.dmPelsWidth, devmode.dmPelsHeight);
}
int _tmainqqqqqqqqq(int argc, _TCHAR* argv[])
{
	std::vector<displayMode> vecDisplayMode_;

	GetAllDisplayMode(vecDisplayMode_);


	int nSize = vecDisplayMode_.size();
	for (int i = 0; i < nSize; ++i)
	{
		printf("%d x %d \n", vecDisplayMode_[i].m_nWidth, vecDisplayMode_[i].m_nHeight);
	}

	return 0;
}