
// groundDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ground.h"
#include "groundDlg.h"
#include "afxdialogex.h"
#include "MsHTML.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CRect m_rect,m_orignal;
static unsigned char show_and_hide = 0;
static unsigned char receive_buf[270];

double lon_global_rt = 0;
double lat_global_rt = 0;

static unsigned int focus_on_time = 0;

static unsigned int pos_switch = 0;

mission_interface_def mission_interface[10];//10 lines

/* ---------------*/
static unsigned char open_flag = 0 ;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CgroundDlg 对话框




CgroundDlg::CgroundDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CgroundDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgroundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_web);
	DDX_Control(pDX, IDC_BUTTON8, m_show_test);
	DDX_Control(pDX, IDC_MSCOMM1, m_mscomm);
	DDX_Control(pDX, IDC_BUTTON13, m_com_open_button);
	DDX_Control(pDX, IDC_COMBO1, m_combox_target);
	DDX_Control(pDX, IDC_LIST1, m_test_list);
	DDX_Control(pDX, IDC_LIST2, m_list_box);
	DDX_Control(pDX, IDC_STATIC200, m_pos_lat);
	DDX_Control(pDX, IDC_STATIC201, m_pos_lon);
	DDX_Control(pDX, IDC_STATIC202, m_pos_alt);
	DDX_Control(pDX, IDC_STATIC203, m_att_roll);
	DDX_Control(pDX, IDC_STATIC205, m_att_pit);
	DDX_Control(pDX, IDC_STATIC206, m_att_yaw);
	DDX_Control(pDX, IDC_STATIC433, m_pic_show);
	DDX_Control(pDX, IDC_STATIC675, m_plane_status);
	DDX_Control(pDX, IDC_STATIC4433, m_boot_time);
}

BEGIN_MESSAGE_MAP(CgroundDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CgroundDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CgroundDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CgroundDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CgroundDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CgroundDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CgroundDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON13, &CgroundDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON834, &CgroundDlg::OnBnClickedButton834)
	ON_BN_CLICKED(IDC_BUTTON10, &CgroundDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON9, &CgroundDlg::OnBnClickedButton9)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CgroundDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CgroundDlg 消息处理程序

static double line_def_def[51][3] = {
{22.544206,113.902321,1},
{22.541501,113.911051,1},
{22.542405,113.905796,1},
{22.542192,113.905753,1},
{22.541017,113.912580,1},
{22.540805,113.912538,1},
{22.541890,113.906228,1},
{22.541677,113.906185,1},
{22.540321,113.914067,1},
{22.540108,113.914024,1},
{22.541273,113.907255,1},
{22.541060,113.907213,1},
{22.539784,113.914629,1},
{22.539571,113.914586,1},
{22.540489,113.909248,1},
{22.540276,113.909206,1},
{22.539323,113.914749,1},
{22.539110,113.914706,1},
{22.539656,113.911529,1},
{22.539444,113.911487,1},
{22.538862,113.914869,1},
{22.538649,113.914826,1},
{22.539090,113.912262,1},
{22.538877,113.912220,1},
{22.538395,113.915021,1},
{22.538182,113.914978,1},
{22.538484,113.913226,1},
{22.538271,113.913183,1},
{22.537934,113.915139,1},
{22.537721,113.915097,1},
{22.538002,113.913463,1},
{22.537789,113.913421,1},
{22.537467,113.915295,1},
{22.537254,113.915253,1},
{22.537544,113.913565,1},
{22.537331,113.913522,1},
{22.536994,113.915485,1},
{22.536781,113.915442,1},
{22.537066,113.913784,1},
{22.536853,113.913742,1},
{22.536568,113.915400,1},
{22.536355,113.915357,1},
{22.536576,113.914072,1},
{22.536363,113.914030,1},
{22.536153,113.915248,1},
{22.535940,113.915205,1},
{22.536086,113.914360,1},
{22.535873,113.914318,1},
{22.535736,113.915114,1},
{22.535523,113.915071,1},
{22.535628,113.914462,-1},
};

BOOL CgroundDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetClientRect(&m_rect);
	GetClientRect(&m_orignal);
	m_web.Navigate(_T("E:\\wk_ground\\wk_ground\\ground\\map_huba.html"),NULL,NULL,NULL,NULL);
	/*------------------------------*/
	focus_on_time = 0;
	/*---- read from files ----*/
	mission_interface[0].start_flag = 0xffff;//not display
    mission_interface[0].display_num = 0;
	mission_interface[0].points_num = 0;
	/*----------------------------------*/
	for( unsigned int i = 1 ; i < 51 ; i ++ )
	{
		mission_interface[0].mission[i].x = line_def_def[i][0];
		mission_interface[0].mission[i].y = line_def_def[i][1];
		mission_interface[0].mission[i].z = 105;
		mission_interface[0].mission[i].param4 = 13.5;
		mission_interface[0].mission[i].seq = i;
		mission_interface[0].mission[i].command = 0x01;
		mission_interface[0].mission[i].autocontinue = 1;
		mission_interface[0].points_num++;
		/* take pictures or not */
		mission_interface[0].mission[i].param1 = (i%2)?0:1;
		mission_interface[0].mission[i].target_system = 0x4E;
		mission_interface[0].mission[i].target_component = 0x7E;
		mission_interface[0].mission[i].frame = 0;
		mission_interface[0].mission[i].current = 0;
	}
	/*------------------------------*/
	int tmp = mission_interface[0].points_num;
	/* set autocontinue to 0 */
	mission_interface[0].mission[1].autocontinue = 0;
	mission_interface[0].mission[tmp].autocontinue = 0;
	/*------------------------------*/

	m_list_box.ResetContent();
	m_list_box.AddString(_T("先打开测试文件，再打开始测试"));

	m_pos_lat.SetWindowTextW(_T(""));
	m_pos_lon.SetWindowTextW(_T(""));
	m_pos_alt.SetWindowTextW(_T(""));
	m_att_roll.SetWindowTextW(_T(""));
	m_att_pit.SetWindowTextW(_T(""));
	m_att_yaw.SetWindowTextW(_T(""));
	m_pic_show.SetWindowTextW(_T(""));
	m_plane_status.SetWindowTextW(_T(""));
	m_boot_time.SetWindowTextW(_T(""));
	/*-------------------------------*/
	/* list contrl */
	DWORD dwStyle = m_test_list.GetExtendedStyle();     
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	/*--------*/
	m_test_list.SetExtendedStyle(dwStyle);
	/* capute */
	m_test_list.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_test_list.InsertColumn(1, _T("测试项描述"), LVCFMT_LEFT, 80);
	m_test_list.InsertColumn(2, _T("通过条件"), LVCFMT_LEFT, 80);
	m_test_list.InsertColumn(3, _T("接口函数"), LVCFMT_LEFT, 80);
	m_test_list.InsertColumn(4, _T("测试状态"), LVCFMT_LEFT, 80);
	m_test_list.InsertColumn(5, _T("参数"), LVCFMT_LEFT, 80);
	/*------------------------------*/
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CgroundDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CgroundDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CgroundDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*              system command                */


void CgroundDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
    if(nType==1) 
	{
		return;//最小化则什么都不做  
	}
    // TODO: Add your message handler code here  
    CWnd *pWnd;   

   pWnd = GetDlgItem(IDC_EXPLORER1);  
    ChangeSize(pWnd, cx, cy,0);  

    pWnd = GetDlgItem(IDC_LIST1);  
    ChangeSize(pWnd, cx, cy,0); 
	
	
	//pWnd = GetDlgItem(IDC_BUTTON10);  
	//ChangeSize(pWnd, cx, cy,1);


	//pWnd = GetDlgItem(IDC_BUTTON13);  
	//ChangeSize(pWnd, cx, cy,1);
	
	//pWnd = GetDlgItem(IDC_BUTTON14);  
	//ChangeSize(pWnd, cx, cy,1);

	//pWnd = GetDlgItem(IDC_BUTTON15);  
	//ChangeSize(pWnd, cx, cy,1);
	
	//pWnd = GetDlgItem(IDC_BUTTON16);  
	//ChangeSize(pWnd, cx, cy,1);

	//pWnd = GetDlgItem(IDC_BUTTON17);  
	//ChangeSize(pWnd, cx, cy,1);

    GetClientRect(&m_rect);// 将变化后的对话框大小设为旧大小   
}
void CgroundDlg::ChangeSize(CWnd *pWnd, int cx, int cy,unsigned int mode)  
{  
    if(pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建   
    {  
        CRect rect;   //获取控件变化前的大小    
        pWnd->GetWindowRect(&rect);  
        ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标  

		if( mode != 2 )
		{
           rect.left=rect.left*cx/m_rect.Width();//调整控件大小  
		}

        rect.right=rect.right*cx/m_rect.Width();  

		if( mode == 1 )
		{
			rect.top=rect.top*cy/m_rect.Height();  
		}else
		{
			rect.top=rect.top;//*cy/m_rect.Height();  
		}
        
		if( mode != 2 )
		{
          rect.bottom=rect.bottom*cy/m_rect.Height(); 
		}
        pWnd->MoveWindow(rect);//设置控件大小  
    }  
}  
/*---------------------------------------------------------------------------------------------------------------------------*/
void CgroundDlg::move_aircraft(float lon,float lat,int psi)
{
		// TODO: 在此添加控件通知处理程序代码
   	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	CComDispatchDriver spScript;
	spDoc->get_Script(&spScript);
//113.913834,22.55618
	CComVariant var[3] = {lon,lat,psi},varRet;
	spScript.Invoke2(L"show_aircraft_position",&var[0],&var[1],&varRet);
    spScript.Invoke1(L"show_aircraft_psi",&var[2],&varRet);
}

void CgroundDlg::OnBnClickedButton17()
{
	// TODO: 在此添加控件通知处理程序代码
   	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	CComDispatchDriver spScript;
	spDoc->get_Script(&spScript);
//113.913834,22.55618
	CComVariant var[3],varRet;
	spScript.Invoke0(L"start_draw",&varRet);	// TODO: 在此添加控件通知处理程序代码
}

void CgroundDlg::OnBnClickedButton2()
{
// TODO: 在此添加控件通知处理程序代码
   	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	CComDispatchDriver spScript;
	spDoc->get_Script(&spScript);
//113.913834,22.55618
	CComVariant var[3],varRet;
	spScript.Invoke0(L"start_draw",&varRet);	// TODO: 在此添加控件通知处理程序代码
}

char mission_buffer[40];

void CgroundDlg::OnBnClickedButton3()
{
//	// TODO: 在此添加控件通知处理程序代码
//// TODO: 在此添加控件通知处理程序代码
//   	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
//	CComDispatchDriver spScript;
//	spDoc->get_Script(&spScript);
//   //113.913834,22.55618
//	CComVariant var[3],varRet;
//	spScript.Invoke0(L"set_point_start",&varRet);	// TODO: 在此添加控件通知处理程序代码、、
	if( !open_flag)
	{
		AfxMessageBox(_T("先打开串口，并连接电台"));
		return;
	}
	tip_one_line("测试开始,启动测试线程");
  tip_one_line("正在获取版本号");
  /* set the time to focus */
  focus_on_time = 0;
  /* start the test thread */
  SetTimer(3,1000,NULL);
  /* create mission id buffer */
  SYSTEMTIME st;
  GetLocalTime(&st);
  sprintf_s(mission_buffer,sizeof(mission_buffer),"mission--%04d-%02d-%02d-%02d-%02d-%02d-%03d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
}

void CgroundDlg::tip_one_line(const char * format)
{
	char time_buffer[200];
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(time_buffer,sizeof(time_buffer),"[%02d:%02d:%02d_%03d]：%s",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds,format);
	/* set to m_list box */
	CString d0;
	/* transform */
	USES_CONVERSION;

	d0 = A2T(time_buffer);

	m_list_box.AddString(d0);

	int count = 0;
	count = m_list_box.GetCount ();

	m_list_box.SetCurSel (count - 1);
}
double pos[100][2];

static unsigned int index = 0;
static unsigned int switch_one = 0;
static unsigned int freq = 0;

void CgroundDlg::OnBnClickedButton4()
{
	//// TODO: 在此添加控件通知处理程序代码,
 //  	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	//CComDispatchDriver spScript;
	//spDoc->get_Script(&spScript);
 //  //113.913834,22.55618
	//CComVariant var[3],varRet;

	//while(1)
	//{

	//	var[0] = index;
	//	var[1] = switch_one;
	//	spScript.Invoke2(L"get_info",&var[0],&var[1],&varRet);	// TODO: 在此添加控件通知处理程序代码

	//	pos[index][switch_one] = varRet.dblVal;

	//	if( pos[index][switch_one] == 0 )
	//	{
	//		break;
	//	}

	//	switch_one ^= 1;

	//	freq ++;

	//	if( freq >=2 )
	//	{
	//		freq = 0;
	//		index ++;
	//	}
	//	
	//}
	//
	if( lon_global_rt != 0 && lat_global_rt != 0 )
	{
	   fucus_on_map((float)lon_global_rt,(float)lat_global_rt);
	}
	/*----------*/
}


void CgroundDlg::OnBnClickedButton5()
{
   	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	CComDispatchDriver spScript;
	spDoc->get_Script(&spScript);
   //113.913834,22.55618
	CComVariant var[3],varRet;
	spScript.Invoke0(L"show_home",&varRet);	// TODO: 在此添加控件通知处理程序代码
}
void CgroundDlg::move_aircraft(float lon,float lat,int psi,int mode)
{
		// TODO: 在此添加控件通知处理程序代码
   	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	CComDispatchDriver spScript;
	spDoc->get_Script(&spScript);
//113.913834,22.55618
	CComVariant var[4] = {lon,lat,psi},varRet;
	spScript.Invoke2(L"show_aircraft_position",&var[0],&var[1],&varRet);
	var[3] = mode;
    spScript.Invoke2(L"show_aircraft_psi",&var[2],&var[3],&varRet);
}
void CgroundDlg::fucus_on_map(float lon,float lat)
{
	// TODO: 在此添加控件通知处理程序代码

	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	CComDispatchDriver spScript;
	spDoc->get_Script(&spScript);
//113.913834,22.55618
	CComVariant var1 = lon,var2 = lat,varRet;
	spScript.Invoke2(L"show_aircraft",&var1,&var2,&varRet);
}

void CgroundDlg::OnBnClickedButton6()
{
//	static unsigned int cnt = 0;
//
//	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
//	CComDispatchDriver spScript;
//	spDoc->get_Script(&spScript);
////113.913834,22.55618
//	CComVariant var[3],varRet;
//	/*----------------------*/
//	//spScript.Invoke0(L"draw_start",&varRet);//
//	//spScript.Invoke0(L"draw_pgon_start",&varRet);
//	/*----------------------*/
//	if( mission_interface.start_flag != 1 )
//	{
//		OnBnClickedButton10();
//
//		mission_interface.current_index = 0;
//		mission_interface.start_flag = 1;//start display
//		mission_interface.display_num = 0;
//		/*--------------------------------*/
//		double lon_tmp_f = mission_interface.mission[mission_interface.current_index].waypoints[1].lon;
//		double lat_tmp_f = mission_interface.mission[mission_interface.current_index].waypoints[1].lat;
//		/*---------------------------------*/
//		fucus_on_map((float)lon_tmp_f,(float)lat_tmp_f);
//		/*---------------------------------*/
//		SetTimer(2,100,NULL);
//		/*--------------------------------*/
//	}
	// TODO: 在此添加控件通知处理程序代码
}
/* draw lines task */
void CgroundDlg::draw_lines_thread(void)
{
 //  	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	//CComDispatchDriver spScript;
	//spDoc->get_Script(&spScript);
	//CComVariant var[3],varRet;
	/*------------------*/
	if( mission_interface[0].start_flag == 1 )
	{
		/*---------------------------------------*/
		if( mission_interface[0].display_num == mission_interface[0].points_num )
		{
			double lon_tmp_f = mission_interface[0].mission[1].y;
		    double lat_tmp_f = mission_interface[0].mission[1].x;
			///*---------------------------------*/
			//fucus_on_map((float)lon_tmp_f,(float)lat_tmp_f);
		    //var[0] = 1;
		    //spScript.Invoke1(L"draw_lines",&var[0],&varRet);//
			//spScript.Invoke1(L"draw_plogon",&var[0],&varRet);

			//fucus_on_map((float)lon_tmp_f,(float)lat_tmp_f);
			if( lon_global_rt > 10 )
			{
				move_aircraft((float)lon_global_rt,(float)lat_global_rt,0,3);
				move_aircraft((float)lon_tmp_f,(float)lat_tmp_f,0,3);
				move_aircraft((float)lon_global_rt,(float)lat_global_rt,0,3);
			}
			/* Kill */
			mission_interface[0].start_flag = 0xffff;
			mission_interface[0].display_num = 0;
			KillTimer(2);
			/*-------------------*/
			pos_switch = 0;
			/*-------------------*/
			return;
		}
		/*---------------------------------------*/
		int num = mission_interface[0].display_num + 1;
		double lon_tmp = mission_interface[0].mission[num].y;
		double lat_tmp = mission_interface[0].mission[num].x;
		/* draw lines */
		move_aircraft((float)lon_tmp,(float)lat_tmp,0,3);
		//var[0] = lon_tmp;
		//var[1] = lat_tmp;
		/*-------------------------------------------------*/
		//spScript.Invoke2(L"set_polyline_data",&var[0],&var[1],&varRet);//set_polygon_data
		//spScript.Invoke2(L"set_polygon_data",&var[0],&var[1],&varRet);
		/*---------------------------*/
		mission_interface[0].display_num++;
		/*---------------------------*/
	}
}

void CgroundDlg::OnBnClickedButton7()
{
	double lon_tmp_f = mission_interface[0].mission[1].y;
	double lat_tmp_f = mission_interface[0].mission[1].x;
	/*---------------------------------*/
	fucus_on_map((float)lon_tmp_f,(float)lat_tmp_f);
}
/*-------fmlink--------*/
const unsigned char CRC16_TABLE[256] = {
	50, 124, 137, 0, 237, 217, 104, 119, 0, 0, 0,    //×¢Òâ´ËÐÐ11¸ö
	89,  0,  0,  0,  0,  0,  0,  0,  0, 214,    
	159, 220, 168, 24, 23, 170, 144, 67, 115, 39,   
	246, 185, 104, 237, 244, 222, 212, 9, 254, 230, 
	28, 28, 132, 221, 232, 11, 153, 41, 39, 0,    
	0, 0, 0, 15, 3, 0, 230, 0, 0, 0,   
	153, 183, 51, 82, 118, 148, 21, 0, 243, 124,
	0, 0, 38, 20, 158, 152, 143, 0, 0, 0,
    106, 49, 22, 143, 140, 5, 150, 0, 231, 183,
	63, 54, 0, 0, 0, 0, 0, 0, 0, 175,
	102, 158, 208, 56, 93, 138, 108, 32, 185, 84,
	34, 0, 124, 237, 4, 76, 128, 56, 116, 134,
	237, 203, 250, 87, 203, 220, 25, 226, 46, 29,
	223, 85, 6, 229, 203, 1,  0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 154, 49, 0, 134,
	219, 208, 188, 84, 22, 19, 21, 134, 0, 78,
	68, 189, 127, 154, 21, 21, 144, 1, 234, 73,
	181, 22, 83, 167, 138, 234, 240, 47, 189, 52,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,  0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 204, 49, 170, 44, 83, 46,  0};

//***********************************************************************************

/*
 *  if the crc data will be  same as the data calc,then return 0;
 *
 *  else return fail none-true
 *
 */
unsigned short CgroundDlg::holder_check_crc(unsigned short crc_origin_one,unsigned char *data,int len)
{
  /* resave it now */
	unsigned short crc;

    crc = crc16_accumulate_buffer(len,data);

	if(crc_origin_one!=0)
	{
		if(crc == crc_origin_one)
			return 0;/* matchs ok */
	}

	return crc;
}

//************************************************************************************
unsigned short CgroundDlg::crc16_accumulate(unsigned char data,unsigned short crc_data)
{
	unsigned char ch;
		ch = (unsigned char)(data ^ (unsigned char)(crc_data & 0x00ff));
		ch = (unsigned char)(ch ^ (ch << 4));
		crc_data=((crc_data >> 8) ^ (ch << 8) ^ (ch << 3) ^ (ch >> 4));
		return crc_data;
}
//************************************************************************************

//************************************************************************************
unsigned short CgroundDlg::crc16_accumulate_buffer(unsigned short buffer_length,unsigned char* data_buffer)
{
	unsigned short i;
	unsigned short crc=0xf00f;
	for( i=1;i<buffer_length;i++)
	{
	crc=crc16_accumulate(data_buffer[i],crc );

	}
	crc=crc16_accumulate( CRC16_TABLE[data_buffer[5]],crc);
	return crc;
}
void CgroundDlg::gs_d_icr(unsigned char c)
{
	static unsigned short cnt2 = 0,crc = 0,cnt = 0;
	static unsigned char step_head = 0;
	static unsigned char step_head2 = 0;

	switch(step_head)
	{
	 case 0:
		 if(c == 0x46)
		 {
			 receive_buf[0] = c;
			 step_head = 1;
		 }
		 else{
			 step_head = 0;
		 }
		 break;
	 case 1:
		 if(c == 0x4d||c == 0x4e)
		 {
			 receive_buf[1] = c;
			 step_head = 2;
		 }
	     else
		 {
			step_head = 0;
		 }
		 break;
	 case 2:
		 if( c == 0x7e )
		 {
		     receive_buf[2] = c;
		     step_head = 3;
		 }else
		 {
			 step_head = 0;
		 }
		 break;
	 case 3:
		 receive_buf[3]=c;
	     step_head = 4;
		 break;
	 case 4:
         receive_buf[4]  = c;
		 step_head = 5;
		 break;
	 case 5:
		 receive_buf[5] = c;
		 step_head = 6;
		 cnt = 0;
		 break;

	 case 6:
		 if((receive_buf[3]+2) > cnt )
		 {
			 receive_buf[6+cnt] = c;
			 cnt++;
		 }
		 if((receive_buf[3]+2) == cnt)
		 {
			 memcpy(&crc,receive_buf+6+(receive_buf[3]),2);
			 cnt = 0;
			 if(crc == holder_check_crc(0,receive_buf,(receive_buf[3]+6)))
			 {		
				fm_test_rev_thread(receive_buf[5],receive_buf+6,receive_buf[3]);

			    if( receive_buf[5] == 33 || receive_buf[5] == 30 )
				{
					show_position(&receive_buf[6],receive_buf[3]);
				}
			 }
					 
			 step_head = 0;
		 }
		 break;
	 default :
		 step_head = 0;
		 break;
	}

}

BEGIN_EVENTSINK_MAP(CgroundDlg, CDialogEx)
	ON_EVENT(CgroundDlg, IDC_MSCOMM1, 1, CgroundDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


void CgroundDlg::OnCommMscomm1()
{
	VARIANT InputData;   
	int leng;
	static char cData[1024];   
	long k;

    COleSafeArray fs;

	if (m_mscomm.get_CommEvent() == 2)   
	{
		leng = m_mscomm.get_InBufferCount(); 
		
		InputData = m_mscomm.get_Input();

		fs = InputData; //V

        for( k = 0 ; k < (long)fs.GetOneDimSize() ; k++ ) 
		{
			fs.GetElement(&k, cData + k); 
		}
		/* fmlink */
		for(int i=0;i<leng;i++)
		{
		   gs_d_icr(cData[i]);
		}	
	}
}


void CgroundDlg::OnBnClickedButton13()
{
    unsigned char com_num;	
	CString dis;

    if(open_flag == 0)//has not opened
	{
		com_num = 7;

		if (m_mscomm.get_PortOpen())
        {
            m_mscomm.put_PortOpen(FALSE);
        }
        m_mscomm.put_CommPort(com_num); //
        m_mscomm.put_InBufferSize(1024); //
        m_mscomm.put_OutBufferSize(1024);//
        m_mscomm.put_InputLen(0);//
        m_mscomm.put_InputMode(1);//
        m_mscomm.put_RThreshold(1);//
		
		m_mscomm.put_Settings(_T("57600,n,8,1"));
	
        if (!m_mscomm.get_PortOpen())//
        {
            m_mscomm.put_PortOpen(TRUE);//
        }
        else
        {
            m_mscomm.put_OutBufferCount(0);
        }
		/* change the caption */
		open_flag = 1;
		m_com_open_button.SetWindowTextW(_T("close"));
	}else
	{
	    m_mscomm.put_PortOpen(FALSE);
        open_flag = 0;
		m_com_open_button.SetWindowTextW(_T("open"));
		KillTimer(1);
	}
}
/* fm link create buffer */
int CgroundDlg::fmlink_package_create(unsigned char * buffer,unsigned int buffer_len,unsigned char ID,unsigned char * payload,unsigned int p_len)
{
	unsigned char cache[256+8];
	static unsigned int seq = 0;
	unsigned short crc;
	cache[0] = 0x46;
	cache[1] = 0x4e;
	cache[2] = 0xff;
	cache[3] = p_len;
	cache[4] = 0xb4;
	cache[5] = ID;
	memcpy(&cache[6],payload,p_len);
	
	crc = holder_check_crc(0,cache,p_len+6);

	cache[6+p_len] = crc & 0xff;

	cache[6+p_len+1] = crc >> 8;

	memcpy(buffer,cache,8+p_len);

	return 8+p_len;
}
int CgroundDlg::send(unsigned char * data,unsigned int len)
{
   CByteArray hexdata; 

   for( unsigned int i = 0 ; i < len ; i++ )
   {
     hexdata.Add(data[i]);
   }
   /*----------------------*/
   m_mscomm.put_Output(COleVariant(hexdata));
   /*---------*/
   return len;
}
void CgroundDlg::fm_link_send(unsigned char ID , unsigned char *data,unsigned int len_p)
{
	/*-------------------------*/
	unsigned char buffer[300];
	int len;

	len = fmlink_package_create(buffer,sizeof(buffer),ID,data,len_p);

	send(buffer,len);
}

void CgroundDlg::OnBnClickedButton834()
{

   unsigned char buffer[8] = { 0x46 ,0x4D ,0x55 ,0x00 ,0x00 ,0x11 ,0xAA ,0x55 };

   if( !open_flag )
   {
     AfxMessageBox(_T("请先打开串口"));
   }

   CByteArray hexdata; 

   for( int i = 0 ; i < 8 ; i++ )
   {
     hexdata.Add(buffer[i]);
   }

   m_mscomm.put_Output(COleVariant(hexdata)); //
   /*  --------------------- */
   SetTimer(1,1000,NULL);
   /*------------------------*/
}
/* show rt postion */
void CgroundDlg::show_position(unsigned char * data,unsigned int len)
{
	static float psi = 0;

	if( pos_switch == 1 )
	{
		return;
	}

	if( len == 26 )
	{
		/*-------------------------*/
		global_position_int_def pos;
		/*=========================*/
		memcpy(&pos,data,len);
		/*-------------------------*/
		double lat = (double)pos.lat / 10000000.0;
		double lon = (double)pos.lon / 10000000.0;
		double alt = (double)pos.alt / 1000.0;
		/*-------------------------*/
		lon_global_rt = lon;
		lat_global_rt = lat;
		/* focus to aircraft */
		if( lon_global_rt != 0 && lat_global_rt != 0 )
		{
			if( focus_on_time++ < 100 )
			{
			  fucus_on_map((float)lon_global_rt,(float)lat_global_rt);
			}
			/*-------------------------*/
			move_aircraft((float)lon,(float)lat,(int)psi,2);
		}
		/* show off */
		char buffer[32];
		CString d0;
		/* transform */
		USES_CONVERSION;
		/*----------------------------------*/
		sprintf(buffer,"%lf",lat);
		d0 = A2T(buffer);
		m_pos_lat.SetWindowTextW(d0);
		/*----------------------------------*/
		sprintf(buffer,"%lf",lon);
		d0 = A2T(buffer);
		m_pos_lon.SetWindowTextW(d0);
		/*----------------------------------*/
		sprintf(buffer,"%lf",alt);
		d0 = A2T(buffer);
		m_pos_alt.SetWindowTextW(d0);
		/*----------------------------------*/
	}
	else if( len == 16 )
	{
		float * tmp_f = (float *)data;

		psi = tmp_f[3] * 57.3f * (-1.0f);
	}
}
/* time tick */
void CgroundDlg::OnTimer(UINT_PTR nIDEvent) 
{
  switch(nIDEvent)
  {
    case 1://radio manager
	  heat_beats();
	case 2:
		draw_lines_thread();
	break;
	case 3:
		test_thread_timer();
	break;
    default:break;
  }
  /* overrade */
  CDialog::OnTimer(nIDEvent);
}
/* heatbeats */
void CgroundDlg::heat_beats(void)
{
	unsigned char buff[5];

	fm_link_send(0,buff,5);
}

void CgroundDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码 clearAll
	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	CComDispatchDriver spScript;
	spDoc->get_Script(&spScript);
//113.913834,22.55618
	CComVariant  varRet;
	spScript.Invoke0(L"clearAll",&varRet);
}


void CgroundDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	draw_lines_thread();
}


void CgroundDlg::OnCbnSelchangeCombo1()
{
	int index = m_combox_target.GetCurSel();
	// TODO: 在此添加控件通知处理程序代码
	static unsigned int cnt = 0;

	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	CComDispatchDriver spScript;
	spDoc->get_Script(&spScript);
//113.913834,22.55618
	CComVariant var[3],varRet;
	/*----------------------*/
	//spScript.Invoke0(L"draw_start",&varRet);//
	//spScript.Invoke0(L"draw_pgon_start",&varRet);
	/*----------------------*/
	if( mission_interface[0].start_flag != 1 )
	{
		OnBnClickedButton10();

		mission_interface[0].start_flag = 1;//start display
		mission_interface[0].display_num = 0;
		/*--------------------------------*/
		double lon_tmp_f = mission_interface[0].mission[1].y;
		double lat_tmp_f = mission_interface[0].mission[1].x;
		/*---------------------------------*/
		fucus_on_map((float)lon_tmp_f,(float)lat_tmp_f);
		/*---------------------------------*/
		pos_switch = 1;
		/*---------------------------------*/
		SetTimer(2,10,NULL);
		/*--------------------------------*/
	}
}
