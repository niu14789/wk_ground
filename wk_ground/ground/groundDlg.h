
// groundDlg.h : 头文件
//

#pragma once
#include "explorer1.h"
#include "afxwin.h"
#include "mscomm1.h"

typedef struct
{
	double lat;
	double lon;
	double height;
	double speed;
	int seq;
	int command;
	int autocontinue;
	int rev;
}waypoints_def;

typedef struct
{
	char mission_name[32];
	unsigned int points_num;
	waypoints_def waypoints[1000];
}mission_def;

typedef struct
{
	unsigned int start_flag;
	unsigned int current_index;
	unsigned int display_num;
	mission_def mission[100];
}mission_interface_def;


// CgroundDlg 对话框
class CgroundDlg : public CDialogEx
{
// 构造
public:
	CgroundDlg(CWnd* pParent = NULL);	// 标准构造函数
	void CgroundDlg::move_aircraft(float lon,float lat,int psi);
	void CgroundDlg::OnSize(UINT nType, int cx, int cy);
	void CgroundDlg::ChangeSize(CWnd *pWnd, int cx, int cy,unsigned int mode) ;
	void CgroundDlg::move_aircraft(float lon,float lat,int psi,int mode);
	void CgroundDlg::fucus_on_map(float lon,float lat);
	void CgroundDlg::gs_d_icr(unsigned char c);
	unsigned short CgroundDlg::holder_check_crc(unsigned short crc_origin_one,unsigned char *data,int len);
	unsigned short CgroundDlg::crc16_accumulate(unsigned char data,unsigned short crc_data);
	unsigned short CgroundDlg::crc16_accumulate_buffer(unsigned short buffer_length,unsigned char* data_buffer);
	void CgroundDlg::show_position(unsigned char * data,unsigned int len);
	int CgroundDlg::fmlink_package_create(unsigned char * buffer,unsigned int buffer_len,unsigned char ID,unsigned char * payload,unsigned int p_len);
    int CgroundDlg::send(unsigned char * data,unsigned int len);
	void CgroundDlg::fm_link_send(unsigned char ID , unsigned char *data,unsigned int len_p);
	void CgroundDlg::OnTimer(UINT_PTR nIDEvent) ;
	void CgroundDlg::heat_beats(void);
	void CgroundDlg::draw_lines_thread(void);
// 对话框数据
	enum { IDD = IDD_GROUND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_web;
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton8();
	CButton m_show_test;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	CMscomm1 m_mscomm;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	afx_msg void OnBnClickedButton13();
	CButton m_com_open_button;
	afx_msg void OnBnClickedButton834();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_combox_target;
};
