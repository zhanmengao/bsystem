#include "stdafx.h"
#include "BmpSave_CImage.h"


BOOL SaveBmp(HBITMAP hBmp)
{
	CImage image;

	// ����λͼ���
	image.Attach(hBmp);

	// �����jpg��ʽͼƬ
	image.Save("mybmp1.jpg");

	return TRUE;
}


BOOL ConverPicture()
{
	CImage image;

	// ����ͼƬ
	image.Load("1.jpg");

	// ����ͼƬ
	// bmp��ʽ
	image.Save("ConverPicture.bmp");
	// png��ʽ
	image.Save("ConverPicture.png");
	// jpg��ʽ
	image.Save("ConverPicture.jpg");
	// gif��ʽ
	image.Save("ConverPicture.gif");


	return TRUE;
}