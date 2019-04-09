#include "stdafx.h"
#include "ground.h"
#include "groundDlg.h"
#include "afxdialogex.h"
#include "MsHTML.h"

static unsigned int test_smd = 0;

extern double lon_global_rt;
extern double lat_global_rt;
extern mission_interface_def mission_interface[10];

void CgroundDlg::get_version(void)
{
	unsigned char buff[5];

	fm_link_send(233,buff,0);
}
/* get payload */
void CgroundDlg::get_payload(void)//228,229
{
	unsigned char buff[5];

	fm_link_send(228,buff,0);
}
void CgroundDlg::set_landing_area(void)
{
	if( lon_global_rt == 0 || lat_global_rt == 0 )
	{
		tip_one_line("�ɻ�δ��λ����ȴ�....����");
		return;		
	}
	tip_one_line("�������ý���㣬����Ϊ��ǰ�ɻ�λ��....����");
	/*---- set num 0 waypoint */
	mission_interface[0].mission[0].param1 = 1;
	mission_interface[0].mission[0].param2 = 0;
	mission_interface[0].mission[0].param3 = 0;
	mission_interface[0].mission[0].param4 = 13.5;
	mission_interface[0].mission[0].x = lat_global_rt;
	mission_interface[0].mission[0].y = lon_global_rt;
	mission_interface[0].mission[0].z = 105;
	mission_interface[0].mission[0].seq = 0;
	mission_interface[0].mission[0].command = 0x10;
	mission_interface[0].mission[0].target_system = 0x4E;
	mission_interface[0].mission[0].target_component = 0x7E;
	mission_interface[0].mission[0].frame = 0;
	mission_interface[0].mission[0].current = 0x1;
	mission_interface[0].mission[0].autocontinue = 1;
	mission_interface[0].mission[0].gm_teta = 0;
	/*-------------*/
	fm_link_send(39,(unsigned char *)&mission_interface[0].mission[0],sizeof(mission_interface[0].mission[0]));
}
/* parse the landing area */
void CgroundDlg::parse_landing_area(unsigned char * data,unsigned int len)
{
	if( len != 4 )
	{
		tip_one_line("����ACKЭ����󣡣�");
		return;;
	}
	/* get data */
	if( data[2] == 0 && data[3] == 0 )
	{
		char buffer[64];
		sprintf(buffer,"��������óɹ�������λ��Ϊ:%lf %lf",lat_global_rt,lon_global_rt);
		tip_one_line(buffer);
		/* switch */
		test_smd = 3;
	}
}
/*------------------------*/
void CgroundDlg::parse_payload(unsigned char * data,unsigned int len)
{
	if( 1 != len )
	{
		tip_one_line("�غ�Э����󣡣�");
		return;
	}
	/* get type */
	unsigned char type_l = data[0];

	char buffer[32];

	sprintf(buffer,"�غɴ��룺%d",type_l);

	tip_one_line(buffer);

	if( type_l == 0 )
	{
		tip_one_line("û���غɣ������޷���ɣ�������ɣ������޸Ĳ����ļ�");
	}
	else if( type_l == 100 )
	{
		tip_one_line("D200 A6000���������");
	}
	else if( type_l == 101 )
	{
		tip_one_line("D200 A6000���������");
	}
	else if( type_l == 102 )
	{
		tip_one_line("D200 LIDAR����");
	}
	else if( type_l == 103 )
	{
		tip_one_line("D200 �����+STIM300����");
	}
	else if( type_l == 104 )
	{
		tip_one_line("D200 A6000�����");
	}
	else if( type_l == 105 )
	{
		tip_one_line("D200 RIEGL MINI");
	}
	else if( type_l == 106 )
	{
		tip_one_line("D300 ������̨");
	}
	else if( type_l == 107 )
	{
		tip_one_line("D200 RX1 �����");
	}
	else if( type_l == 108 )
	{
		tip_one_line("D200 �Ⱥ������");
	}
	else if( type_l == 104 )
	{
		tip_one_line("D200 A6000�����");
	}
	else if( type_l == 200 )
	{
		tip_one_line("Vϵ�� A6000�����");
	}
	else if( type_l == 202 )
	{
		tip_one_line("Vϵ�� A6000�����");
	}
	else if( type_l == 206 )
	{
		tip_one_line("Vϵ�� ��Ƶ������̨");
	}
	else
	{
		tip_one_line("δ֪���غ�����");
		return;
	}
	/* zaihe matched ? */
	test_smd = 2;
}
/* get and decode the version */
void CgroundDlg::parse_version(unsigned char * data,unsigned int len)
{
	if( sizeof(feima_status_def) != len )
	{
		tip_one_line("�汾��Э����󣡣�");
		return;
	}
	/*-------*/
	feima_status_def feima_staus;
	/* copy data */
	memcpy(&feima_staus,data,len);
	/* */
	char buffer[200];
	/* show mileage and flightnum */
	sprintf(buffer,"������̣�%d���� ���мܴΣ�%d",feima_staus.mileage,feima_staus.flightNum);
	/* showe*/
	tip_one_line(buffer);
	/*--------------------------------------*/
	/* show radio */
	sprintf(buffer,"��̨ID��%s ��̨�汾��%s",feima_staus.radio_id,feima_staus.radio_fw);
	/* showe*/
	tip_one_line(buffer);
	/*--------------------------------------*/
	/* show fmID */
	sprintf(buffer,"���кţ�%s",feima_staus.fm_id);
	/* showe*/
	tip_one_line(buffer);
	/*--------------------------------------*/
	/* show fc ID */
	sprintf(buffer,"�ɿذ汾��%s",feima_staus.fc_fw);
	/* showe*/
	tip_one_line(buffer);
	/*--------------------------------------*/
	/* show other ids */
	sprintf(buffer,"�����汾��%d %d %d %d %d",feima_staus.gps_fw,feima_staus.battery_fw,feima_staus.mag1_fw,feima_staus.mag2_fw,feima_staus.gm_fw);
	/* showe*/
	tip_one_line(buffer);
	/*--------------------------------------*/
	/* show other ids */
	sprintf(buffer,"%d %d %d %d %d",feima_staus.esc1_fw,feima_staus.esc2_fw,feima_staus.esc3_fw,feima_staus.esc4_fw);
	/* showe*/
	tip_one_line(buffer);
	/*--------------------------------------*/
	/* show other ids */
	sprintf(buffer,"%d %d %d %d %d %d",feima_staus.sonar_fw,feima_staus.vision_fw,feima_staus.reserve1,feima_staus.reserve2,feima_staus.reserve3,feima_staus.reserve4);
	/* showe*/
	tip_one_line(buffer);
	/*--------------------------------------*/
	/* version has been ok . change */
	test_smd  = 1;
}
void CgroundDlg::fm_test_rev_thread(unsigned char ID,unsigned char * data,unsigned int len)
{
	switch(ID)
	{
	  case 232:
		  parse_version(data,len);
		  break;
	  case 229:
		  parse_payload(data,len);
		  break;
	  case 47:
		  if( test_smd == 2 )
		  {
			  parse_landing_area(data,len);
		  }
		  break;
	  default:
		  break;
	}
}
/* thread */
void CgroundDlg::test_thread_timer(void)
{
	switch(test_smd)
	{
	case 0:
		get_version();
		break;
	case 1:
		get_payload();
		break;
	case 2:
		set_landing_area();
		break;
	case 3:
		break;
	default:
		break;
	}
}