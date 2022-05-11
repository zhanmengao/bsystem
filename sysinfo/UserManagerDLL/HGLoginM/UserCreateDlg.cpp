// UserCreateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HGLoginM.h"
#include "UserCreateDlg.h"
#include "afxdialogex.h"
//#include "WinUser.h"
#include "HGLoginMView.h"
#include "MainFrm.h"

#include "MD5Checksum.h"



// CUserCreateDlg �Ի���

IMPLEMENT_DYNAMIC(CUserCreateDlg, CDialogEx)

CUserCreateDlg::CUserCreateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserCreateDlg::IDD, pParent)
{

}

CUserCreateDlg::~CUserCreateDlg()
{
}

void CUserCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT6, SLEdit);
	DDX_Control(pDX, IDC_EDIT2, PawStrEdit);
	DDX_Control(pDX, IDC_EDIT1, UNzEdit);
	DDX_Control(pDX, IDC_EDIT5, b0Edit);
	DDX_Control(pDX, IDC_EDIT3, SZsEdit);
	DDX_Control(pDX, IDC_EDIT4, SZeEdit);
	DDX_Control(pDX, IDC_CHECK1, PawTUserN);
	DDX_Control(pDX, IDC_CHECK2, PawTIPs);
	DDX_Control(pDX, IDC_CHECK3, PawTUAM);
}


BEGIN_MESSAGE_MAP(CUserCreateDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUserCreateDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &CUserCreateDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT5, &CUserCreateDlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT3, &CUserCreateDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CUserCreateDlg::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_CHECK1, &CUserCreateDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CUserCreateDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CUserCreateDlg::OnBnClickedCheck3)
END_MESSAGE_MAP()


// CUserCreateDlg ��Ϣ�������


CString CUserCreateDlg::SCnewUserName( int zz_i,CString& bwStr )
{
	CString nameStr;
	UNzEdit.GetWindowText( nameStr);

	CString uStr;
	b0Edit.GetWindowText( uStr );
	int  b0_i = _ttoi(  uStr );

	CString zz_iStr;
	zz_iStr.Format(_T("%d"),zz_i);
	
	int slen = zz_iStr.GetLength();
	
	CString RnameStr;
	CString lin ;
	
	for(  int l = 0 ; l< b0_i - slen ;l++  )
	{
		lin += _T("0");
	}

	bwStr = lin + zz_iStr;

	RnameStr = nameStr + bwStr;

	return RnameStr;
}

void CUserCreateDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	if (  ! TestUserName() )
	{
		MessageBox( _T("���ò�������ȷ"),_T("��ʾ"),MB_OK );
		return;
	}

	//CWinUser CreateNewUser;

	CString sNum,eNum;

	SZsEdit.GetWindowText( sNum );
	SZeEdit.GetWindowText( eNum );

	int s =  _ttoi( sNum );
	int e =  _ttoi( eNum );

	CString newUserName,UserPawStr;

	CString outTextStr,tmpStr;

	CString IPbwStr;
	CRITICAL_SECTION CriticalSection;
//	InitializeCriticalSection(&CriticalSection);

	if (!PawTUserN.GetCheck() && !PawTIPs.GetCheck())
	{
		PawStrEdit.GetWindowText(UserPawStr);
	}

	//AfxMessageBox(UserPawStr);

    for( int i = s ; i <= e ; i++ )
	{
	//	EnterCriticalSection(&CriticalSection);
		newUserName = SCnewUserName( i,IPbwStr );



		if ( PawTUserN.GetCheck() )
		{
			UserPawStr = newUserName;
		}

        if ( PawTIPs.GetCheck() )
		{
            UserPawStr = IPbwStr;
		}
		
		if ( PawTUAM.GetCheck() )
		{

			MD5 md5; 	
			std::string tmpstr = CStringA( newUserName );			
			md5.update( tmpstr );
			newUserName.ReleaseBuffer();
						
			CString newPassword(md5.toString().c_str());
			UserPawStr = L"Ve:" + newPassword.Left(8).MakeUpper();
			//AfxMessageBox( UserPawStr + L" " + newUserName );			
			//UserPawStr = L"Ve:" + newPassword;
		}


	    //CreateNewUser.AddSysUser( newUserName, UserPawStr  );
		
		tmpStr = GetMListView->UserCon.ReturnErrInfo(
		                      GetMListView->UserCon.AddSysUser( newUserName, UserPawStr ));

		outTextStr.Format( _T("����û���%s ��ʼ���룺%s  ״̬��%s"), newUserName,UserPawStr ,tmpStr ) ;
		GetMainFrame->m_wndOutput.FillBuildWindow( outTextStr );

		tmpStr = GetMListView->UserCon.ReturnErrInfo(
			GetMListView->UserCon.AddUserToGroup( _T("users"), newUserName ));

		outTextStr.Format( _T("����û���%s ��users�� %s"), newUserName, tmpStr ) ;
		GetMainFrame->m_wndOutput.FillBuildWindow( outTextStr );
//		LeaveCriticalSection(&CriticalSection);
		
	}
//	DeleteCriticalSection(&CriticalSection);

	CDialogEx::OnOK();
}


void CUserCreateDlg::SZTChong()
{

	UNzEdit.SetWindowText( _T("USER") );
	b0Edit.SetWindowText( _T("3") );
	SZsEdit.SetWindowText( _T("1") );
	SZeEdit.SetWindowText( _T("100") );

	PawTUAM.SetCheck( false );

	SLEdit.SetWindowText( _T("USER001 -- USER100") );

}

BOOL CUserCreateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SZTChong();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CUserCreateDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TestUserName();
}


void CUserCreateDlg::OnEnChangeEdit5()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	

	TestUserName();

}


BOOL CUserCreateDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CUserCreateDlg::TestUserName()
{


	CString nStr;
	UNzEdit.GetWindowText( nStr);

	nStr = nStr.Trim();
	if( nStr.GetLength() == 0  )
	{
		SLEdit.SetWindowText(_T("�û�������Ϊ��"));
		return FALSE;
	}

	CString uStr;
	b0Edit.GetWindowText( uStr );

	int  i = _ttoi(  uStr );	

	if ( i<1 || i > 5 )
	{
		SLEdit.SetWindowText(_T("��1 - 5����������"));
		return FALSE;
	}

	CString zsStr ,zeStr;
	SZsEdit.GetWindowText( zsStr );
	SZeEdit.GetWindowText( zeStr );

	int zs = _ttoi( zsStr );
	int ze = _ttoi( zeStr );

	if (  ( zs< 1 || ze < 1 ) ||
		ze < zs ||
		ze > 10000  )
	{
		SLEdit.SetWindowText(_T("������Χ��������"));
		return FALSE;
	}

	int slen = zsStr.GetLength();
	int elen = zeStr.GetLength();

    CString SnameStr ,EnameStr;
	
	CString lin ;
	for(  int l=0 ; l< i - slen ;l++  )
	{
		lin += _T("0");
	}
	
	SnameStr = nStr + lin + zsStr;

	lin = _T("");	
	for(  int l=0 ; l< i - elen ;l++  )
	{
		lin += _T("0");
	}	
	EnameStr = nStr + lin + zeStr;

	SLEdit.SetWindowText( SnameStr + _T(" -- ") + EnameStr );

	if( PawTUserN.GetCheck() )
	{
	  PawStrEdit.SetWindowText( SnameStr + _T("(ʾ��)") );
	}

	if ( PawTUAM.GetCheck()  )
	{

 		//CString md5UserName = CMD5Checksum::GetMD5( SnameStr + L"001" );
		//PawStrEdit.SetWindowText( L"Ve:" + md5UserName.Left(8) ); 

		MD5 md5;
		std::string tmpstr = CStringA( SnameStr + L"001" );
		md5.update(tmpstr);
		//newUserName.ReleaseBuffer();

		CString newPassword(md5.toString().c_str());
		CString UserPawStr = L"Ve:" + newPassword.Left(8).MakeUpper();

		PawStrEdit.SetWindowText( UserPawStr  );

	}
	
	return TRUE;
}

void CUserCreateDlg::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TestUserName();
}


void CUserCreateDlg::OnEnChangeEdit4()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TestUserName();
}


void CUserCreateDlg::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if( PawTIPs.GetCheck() )
	{
		PawTIPs.SetCheck(FALSE );
	}

	if (PawTUAM.GetCheck())
	{
		PawTUAM.SetCheck(FALSE);
	}

	if( PawTUserN.GetCheck() )
	{
		PawStrEdit.EnableWindow( FALSE );
		TestUserName();
	}
	else
	{
        PawStrEdit.EnableWindow( TRUE );
		PawStrEdit.SetWindowText( _T(""));
	}


}


void CUserCreateDlg::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( PawTUserN.GetCheck() )
	{
		PawTUserN.SetCheck( FALSE );

	}

	if ( PawTUAM.GetCheck())
	{
		PawTUAM.SetCheck(FALSE);
	}

	if ( PawTIPs.GetCheck()  )
	{
		PawStrEdit.EnableWindow( FALSE );
		TestUserName();
	}
	else
	{
		PawStrEdit.EnableWindow( TRUE );
		PawStrEdit.SetWindowText( _T(""));
	}

}


void CUserCreateDlg::OnBnClickedCheck3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (PawTUserN.GetCheck())
	{
		PawTUserN.SetCheck(FALSE);

	}

	if (PawTIPs.GetCheck())
	{
		PawTIPs.SetCheck(FALSE);
	}
	
	if (  PawTUAM.GetCheck()  ) 
	{
	
		PawStrEdit.EnableWindow(FALSE);
		TestUserName();
	}
	else 
	{
		PawStrEdit.EnableWindow(TRUE);
		PawStrEdit.SetWindowText(_T(""));
	
	}

	
	

}
