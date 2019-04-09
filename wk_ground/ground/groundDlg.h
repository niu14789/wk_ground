
// groundDlg.h : 头文件
//

#pragma once
#include "explorer1.h"
#include "afxwin.h"
#include "mscomm1.h"

#pragma pack(1) //设置单字节对齐
typedef struct 
{
	float param1;
	float param2;
	float param3;
	float param4;
	float x;
	float y;
	float z;
	unsigned short seq;
	unsigned short command;
	unsigned char target_system;
	unsigned char target_component;
	unsigned char frame;
	unsigned char current;
	unsigned char autocontinue;	
	float gm_teta;
}mission_item_def;

#pragma pack()

typedef struct
{
	char mission_name[32];
	unsigned int start_flag;
	unsigned int display_num;
	unsigned int points_num;
	mission_item_def mission[1000];
}mission_interface_def;

#pragma pack(1) //设置单字节对齐
typedef struct
{
	unsigned short mileage;
	int flightNum;
	char radio_id[16];
	char radio_fw[16];
	char fm_id[17];
	char fc_fw[22];//niu 20171206
	unsigned short gps_fw;
	unsigned short battery_fw;
	unsigned short mag1_fw;
	unsigned short mag2_fw;
	unsigned short gm_fw;
	unsigned short esc1_fw;
	unsigned short esc2_fw;
	unsigned short esc3_fw;
	unsigned short esc4_fw;
	unsigned short sonar_fw;
	unsigned short vision_fw;
	unsigned short reserve1;
	unsigned short reserve2;
	unsigned short reserve3;
	unsigned short reserve4;	
}feima_status_def;
#pragma pack()

// CgroundDlg 对话框
class CgroundDlg : public CDialogEx
{
// 构造
public:
	void CgroundDlg::set_landing_area(void);
	void CgroundDlg::parse_landing_area(unsigned char * data,unsigned int len) ;
	void CgroundDlg::parse_payload(unsigned char * data,unsigned int len);
	void CgroundDlg::get_payload(void);
	void CgroundDlg::fm_test_rev_thread(unsigned char ID,unsigned char * data,unsigned int len);
	void CgroundDlg::test_thread_timer(void);
	void CgroundDlg::parse_version(unsigned char * data,unsigned int len);
	void CgroundDlg::get_version(void);
	void CgroundDlg::tip_one_line(const char * format);
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
	CListCtrl m_test_list;
	CListBox m_list_box;
};
