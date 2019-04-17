#include "stdafx.h"
#include "ground.h"
#include "groundDlg.h"
#include "afxdialogex.h"
#include "MsHTML.h"

static unsigned int test_smd = 0;

extern double lon_global_rt;
extern double lat_global_rt;
extern mission_interface_def mission_interface[10];

static unsigned int current_point_index = 1;
static unsigned int current_read_index = 1;

extern char mission_buffer[40];

void CgroundDlg::get_version(void)
{
	/*-------------------------*/
	tip_one_line("正在获取飞控版本！！");
	/*-------------------------*/
	unsigned char buff[5];

	fm_link_send(233,buff,0);
}
/* get payload */
void CgroundDlg::get_payload(void)//228,229
{
	/*-------------------------*/
	tip_one_line("正在获取载荷类型！！");
	/*-------------------------*/
	unsigned char buff[5];

	fm_link_send(228,buff,0);
}
void CgroundDlg::set_landing_area(void)
{
	if( lon_global_rt == 0 || lat_global_rt == 0 )
	{
		tip_one_line("飞机未定位，请等待....！！");
		return;		
	}
	tip_one_line("正在设置降落点，设置为当前飞机位置....！！");
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
/* send waypoints num including the zero */
void CgroundDlg::send_wayponits_mount(void)
{
	tip_one_line("正在设置航点总数");
	unsigned short buffer[2];
	/* ------------- */
	buffer[0] = mission_interface[0].points_num + 1;
	buffer[1] = ( 0x4E << 8 ) | 0x7E;
	/* set to current */
	fm_link_send(44,(unsigned char *)buffer,sizeof(buffer));
}
/* camera take a pic */
void CgroundDlg::take_a_pic(void)
{
	static unsigned int pic_freq = 0;
	/*-------------------------*/
	if( (pic_freq++) % 5 )
	{
		return;
	}
	/*-------------------------*/
	tip_one_line("正在试拍！！");
	/*-------------------------*/
	static unsigned char pic_s = 1;
	/*take_pic_btn_event(1);*/
	digicam_control_def cfd;
	/*----------------------*/
	memset(&cfd,0,sizeof(cfd));
	/*----------------------*/
	cfd.target_system = 0x4E;
	cfd.target_component = 0x7E;
	cfd.session = 0xfe;
	cfd.command_id = pic_s++;
    /* send */
	fm_link_send( 155 , (unsigned char *)&cfd , 13 );
}
/* void waypoint upload */
void CgroundDlg::waypoint_upload(void)
{
	char buffer[64];
	sprintf(buffer,"正在上传航点：%d/%d",current_point_index,mission_interface[0].points_num);
	tip_one_line(buffer);

	fm_link_send(39,(unsigned char *)&mission_interface[0].mission[current_point_index],
		sizeof(mission_interface[0].mission[current_point_index]));
}
/* read the number of waypoints */
void CgroundDlg::read_number_of_waypoints(void)
{
	/*-------------------------*/
	tip_one_line("正在回读航点总数......");
	/*-------------------------*/
	unsigned char buff[5];

	fm_link_send(43,buff,0);
}
/* unclock */
void CgroundDlg::unlock_plane(void)
{
	/*-------------------------*/
	tip_one_line(" 飞机正在解锁......");
	/*-------------------------*/
    unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------*/
	*pd = 400;
	param[0] = 1;
	/*-------------------------*/
	fm_link_send(76,package,33);
	/*-------------------------*/
}
/*-------------------------*/
void CgroundDlg::take_off_plane(void)
{
	/*-------------------------*/
	tip_one_line(" 飞机正在起飞......");
	/*-------------------------*/
    unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------*/
	*pd = 23;
	param[0] = 1;
	/*-------------------------*/
	fm_link_send(76,package,33);
	/*-------------------------*/
}
/* set mission id */
void CgroundDlg::set_mission_id(void)
{
	/*-------------------------*/
	tip_one_line("正在设置mission ID......");
	/*-------------------------*/
	fm_link_send(225,(unsigned char *)mission_buffer,32);
}
/* void CgroundDlg */
void CgroundDlg::mission_id_parse(unsigned char * data,unsigned int len)
{
	if( len != 32 )
	{
		tip_one_line("missionID 协议错误！！");
		return;
	}
	/* judging */
	if( memcmp(mission_buffer,data,32) == 0 )
	{
		tip_one_line("missionID 设置成功");
		/* switch */
		test_smd = 3;
	}
}
/* parse */
void CgroundDlg::parse_unclock(unsigned char * data,unsigned int len)
{
	if( len != 3 )
	{
		tip_one_line("解锁协议错误！！");
		return;
	}	
	unsigned short * dt = (unsigned short *)data;
	/*------------------*/
	if( dt[0] == 400 )
	{
		if( data[2] == 0 )
		{
			tip_one_line("解锁成功！！");
			test_smd = 9;
		}
		else
		{
			tip_one_line("解锁失败，请检查......！！");
			test_smd =2000;//中断
		}
	}
}
/*--------------*/
void CgroundDlg::parse_unlock_plane(unsigned char * data,unsigned int len)
{
	if( len != 3 )
	{
		tip_one_line("起飞协议错误！！");
		return;
	}	
	unsigned short * dt = (unsigned short *)data;
	/*------------------*/
	if( dt[0] == 23 )
	{
		if( data[2] == 0 )
		{
			tip_one_line("起飞成功！！");
			test_smd = 10;
		}
		else
		{
			tip_one_line("解锁失败，请检查......！！");
			test_smd =2000;//中断
		}
	}
}
/* read and check */
void CgroundDlg::read_and_check(void)
{
	char buffer[64];
	sprintf(buffer,"正在回读并校验航点：%d/%d",current_read_index,mission_interface[0].points_num);
	tip_one_line(buffer);
	/*---------------*/
	unsigned short buf[2];

	buf[0] = current_read_index;
	buf[1] = 0x4E << 8 | 0x7E;
	/*------------------------*/
	fm_link_send(40,(unsigned char *)buf,sizeof(buf));
	/*------------------------*/
}
/* check */
void CgroundDlg::check_waypoints(unsigned char * data,unsigned int len)
{
	if( len != sizeof(mission_item_def) )
	{
		tip_one_line("航点协议错误！！");
		return;
	}
	/*-------------------------*/
	if( memcmp(data,&mission_interface[0].mission[current_read_index],len) == 0 )
	{
		char buffer[64];
		sprintf(buffer,"航点%d回读成功",current_read_index);
		tip_one_line(buffer);
		/*---------------*/
		current_read_index++;
		/*---------------*/
		if( current_read_index > mission_interface[0].points_num )
		{
			tip_one_line("航点回读校验完成");
			/* switch */
			test_smd = 8;
			/*--------*/
		}
	}
}
/* number of waypoints parse */
void CgroundDlg::parse_number_of_waypoints(unsigned char * data,unsigned int len)
{
	if( len != 4 )
	{
		tip_one_line("航点ACK1协议错误！！");
		return;;
	}
	/* pdf */
	unsigned short * pdf = ( unsigned short *)data;
	/*---------*/
	if( pdf[0] == ( mission_interface[0].points_num + 1 ) )
	{
		char buffer[64];
		sprintf(buffer,"航点总数回读成功 %d",pdf[0]);
		tip_one_line(buffer);
		/* swtich */
		test_smd = 7;
	}
}
/* void waypoint  */
void CgroundDlg::parse_waypoint_upload(unsigned char * data,unsigned int len)
{
	if( len != 4 )
	{
		tip_one_line("航点ACK2协议错误！！");
		return;;
	}
	/*-=-----------------------*/
	unsigned short * pdf = (unsigned short *)data;
	/*-------------------------*/
	if( current_point_index == pdf[1] )
	{
		char buffer[64];
		sprintf(buffer,"航点%d上传成功",current_point_index);
		tip_one_line(buffer);
		/* next point */
		current_point_index++;
		/* done */
		if( current_point_index > mission_interface[0].points_num )
		{
			tip_one_line("航点上传完成");
			test_smd = 6;
		}
	}
}
/* void parse take a pic */
void CgroundDlg::parse_take_apic(unsigned char * data,unsigned int len)
{
	if( len != sizeof(camera_feedback_def) )
	{
		tip_one_line("180协议错误");
		return;;
	}	
	/* ---------------- */
	camera_feedback_def cfd;
	/* jugding */
	memcpy(&cfd,data,len);
	/*------------------*/
	char buffer[32];
	CString d0;
	/* transform */
	USES_CONVERSION;
	/*--------------------------------*/
	sprintf(buffer,"%d",cfd.img_idx);
	d0 = A2T(buffer);
	m_pic_show.SetWindowTextW(d0);
	/*--------------------------------*/
	/* ok or not */
	if( cfd.flags == 2 )
	{
		char buffer[64];
		sprintf(buffer,"拍照成功 %d",cfd.img_idx);
		tip_one_line(buffer);
		/* switch */
		test_smd = 4;

	}
	else if( cfd.flags == 3 )
	{
		char buffer[64];
		sprintf(buffer,"拍照失败 %d",cfd.img_idx);
		tip_one_line(buffer);
	}
	else if( cfd.flags == 1 )
	{
		char buffer[64];
		sprintf(buffer,"收到拍照指令 %d",cfd.img_idx);
		tip_one_line(buffer);
	}
	else
	{
		char buffer[64];
		sprintf(buffer,"未知拍照指令 %d %d",cfd.img_idx,cfd.flags);
		tip_one_line(buffer);
	}
}
/* parse the landing msg */
void CgroundDlg::parse_waypoints_num(unsigned char * data,unsigned int len)
{
	if( len != 4 )
	{
		tip_one_line("航点ACK3协议错误！！");
		return;;
	}	
	/* get data */
	unsigned short * tu  = (unsigned short *)data;
	/*----------*/
	if( tu[1] == ( mission_interface[0].points_num + 1 ) )
	{
		char buffer[64];
		sprintf(buffer,"航点总数下发成功，数量为：%d",tu[1]);
		tip_one_line(buffer);
		/* switch */
		test_smd = 5;
	}
}
/* parse the landing area */
void CgroundDlg::parse_landing_area(unsigned char * data,unsigned int len)
{
	if( len != 4 )
	{
		tip_one_line("航点ACK4协议错误！！");
		return;;
	}
	/* get data */
	if( data[2] == 0 && data[3] == 0 )
	{
		char buffer[64];
		sprintf(buffer,"降落点设置成功，降落位置为:%lf %lf",lat_global_rt,lon_global_rt);
		tip_one_line(buffer);
		/* switch */
		test_smd = 2000;
	}
}
/*------------------------*/
void CgroundDlg::parse_payload(unsigned char * data,unsigned int len)
{
	if( 1 != len )
	{
		tip_one_line("载荷协议错误！！");
		return;
	}
	/* get type */
	unsigned char type_l = data[0];

	char buffer[32];

	sprintf(buffer,"载荷代码：%d",type_l);

	tip_one_line(buffer);

	if( type_l == 0 )
	{
		tip_one_line("没有载荷！！，无法起飞，若想起飞，可以修改测试文件");
	}
	else if( type_l == 100 )
	{
		tip_one_line("D200 A6000单相机！！");
	}
	else if( type_l == 101 )
	{
		tip_one_line("D200 五相机！！");
	}
	else if( type_l == 102 )
	{
		tip_one_line("D200 LIDAR！！");
	}
	else if( type_l == 103 )
	{
		tip_one_line("D200 单相机+STIM300！！");
	}
	else if( type_l == 104 )
	{
		tip_one_line("D200 A6000五相机");
	}
	else if( type_l == 105 )
	{
		tip_one_line("D200 RIEGL MINI");
	}
	else if( type_l == 106 )
	{
		tip_one_line("D300 三轴云台");
	}
	else if( type_l == 107 )
	{
		tip_one_line("D200 RX1 单相机");
	}
	else if( type_l == 108 )
	{
		tip_one_line("D200 热红外相机");
	}
	else if( type_l == 104 )
	{
		tip_one_line("D200 A6000五相机");
	}
	else if( type_l == 200 )
	{
		tip_one_line("V系列 A6000单相机");
	}
	else if( type_l == 202 )
	{
		tip_one_line("V系列 A6000四相机");
	}
	else if( type_l == 206 )
	{
		tip_one_line("V系列 视频三轴云台");
	}
	else
	{
		tip_one_line("未知的载荷类型");
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
		tip_one_line("版本号协议错误！！");
		return;
	}
	/*-------*/
	feima_status_def feima_staus;
	/* copy data */
	memcpy(&feima_staus,data,len);
	/* */
	char buffer[200];
	/* show mileage and flightnum */
	sprintf(buffer,"飞行里程：%d公里 飞行架次：%d",feima_staus.mileage,feima_staus.flightNum);
	/* showe*/
	tip_one_line(buffer);
	/*--------------------------------------*/
	/* show radio */
	sprintf(buffer,"电台ID：%s",feima_staus.radio_id);
	/* showe*/
	tip_one_line(buffer);
	/* show radio */
	sprintf(buffer,"电台版本：%s",feima_staus.radio_fw);
	/* showe*/
	tip_one_line(buffer);
	/*--------------------------------------*/
	/* show fmID */
	sprintf(buffer,"飞控序列号：%s",feima_staus.fm_id);
	/* showe*/
	tip_one_line(buffer);
	/*--------------------------------------*/
	/* show fc ID */
	sprintf(buffer,"飞控版本：%s",feima_staus.fc_fw);
	/* showe*/
	tip_one_line(buffer);
	/*--------------------------------------*/
	/* show other ids */
	sprintf(buffer,"其它版本：%d %d %d %d %d",feima_staus.gps_fw,feima_staus.battery_fw,feima_staus.mag1_fw,feima_staus.mag2_fw,feima_staus.gm_fw);
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
	  case 30:
		  att_show(data,len);
		  break;
	  case 42:
		  plane_status_show(data,len);
		  break;
	  case 47:
		  if( test_smd == 2 )
		  {
			  parse_landing_area(data,len);
		  }
		  else if( test_smd == 4 )
		  {
			  parse_waypoints_num(data,len);
		  }
		  else if( test_smd == 5 )
		  {
			  parse_waypoint_upload(data,len);
		  }
		  break;
	  case 180:
		  if( test_smd == 3 )
		  {
			  parse_take_apic(data,len);
		  }
		  else
		  {
			  pic_count(data,len);
		  }
		  break;
	  case 44:
		  parse_number_of_waypoints(data,len);
		  break;
	  case 39:
		  check_waypoints(data,len);
		  break;
	  case 77:
		  if( test_smd == 8 )
		  {
			  parse_unclock(data,len);
		  }
		  else if( test_smd == 9 )
		  {
			  parse_unlock_plane(data,len);
		  }
		  break;
	  case 225:
		  mission_id_parse(data,len);
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
		case 2000:
			set_mission_id();
			break;
		case 3:
			take_a_pic();
			break;
		case 4:
			send_wayponits_mount();
			break;
		case 5:
			waypoint_upload();
			break;
		case 6:
			read_number_of_waypoints();
			break;
		case 7:
			read_and_check();
			break;
		case 8:
			unlock_plane();
			break;
		case 9:
			take_off_plane();
			break;
		case 10:
			break;
		default:
			break;
	}
}
/*---------------------*/
void CgroundDlg::att_show(unsigned char * data,unsigned int len)
{
	if( len != sizeof(attitude_def) )
	{
		tip_one_line("姿态协议错误！！");
		return;
	}
	attitude_def att;
	/* set to  memory */
	memcpy(&att,data,len);
	/* show */
	char buffer[32];
	CString d0;
	/* transform */
	USES_CONVERSION;
	/* show */
	sprintf(buffer,"%3.2f",att.roll);
	d0 = A2T(buffer);
	m_att_roll.SetWindowTextW(d0);
	/*--------------------------------*/
	sprintf(buffer,"%3.2f",att.pitch);
	d0 = A2T(buffer);
	m_att_pit.SetWindowTextW(d0);
	/*--------------------------------*/
	sprintf(buffer,"%3.2f",att.yaw);
	d0 = A2T(buffer);
	m_att_yaw.SetWindowTextW(d0);
	/*--------------------------------*/
	sprintf(buffer,"%f s",(float)att.time_boot_ms / 1000);
	d0 = A2T(buffer);
	m_boot_time.SetWindowTextW(d0);
}
/* void parse take a pic */
void CgroundDlg::pic_count(unsigned char * data,unsigned int len)
{
	if( len != sizeof(camera_feedback_def) )
	{
		tip_one_line("180协议错误");
		return;
	}	
	/* ---------------- */
	camera_feedback_def cfd;
	/* jugding */
	memcpy(&cfd,data,len);
	/*------------------*/
	char buffer[32];
	CString d0;
	/* transform */
	USES_CONVERSION;
	/*--------------------------------*/
	sprintf(buffer,"%d",cfd.img_idx);
	d0 = A2T(buffer);
	m_pic_show.SetWindowTextW(d0);
	/*--------------------------------*/
}
/* show plane status */
void CgroundDlg::plane_status_show(unsigned char * data,unsigned int len)
{
	static unsigned short current_last = 0xffff;

	if( len != 2 )
	{
		tip_one_line("飞行状态协议错误");
		return;
	}
	/*------------------*/
	char buffer[32];
	CString d0;
	/* transform */
	USES_CONVERSION;
	unsigned short * current_seq = (unsigned short *)data;
	/*-----------------------*/
	if( *current_seq != current_last )
	{
		current_last = *current_seq;
		/* show */
		if( current_last >= 1000 )
		{
			switch(current_last)
			{
				case 1001:
					/*--------------------------------*/
					m_plane_status.SetWindowTextW(_T("飞机正在返航"));
					tip_one_line("飞机正在返航");
					/*--------------------------------*/
					break;
				case 1004:
					/*--------------------------------*/
					m_plane_status.SetWindowTextW(_T("飞机正在悬停"));
					tip_one_line("飞机正在悬停");
					/*--------*/
					break;
				case 1008:
					/*--------------------------------*/
					m_plane_status.SetWindowTextW(_T("飞机正在返航"));
					tip_one_line("飞机正在返航");
					/*--------*/
					break;
				default:
					sprintf(buffer,"飞机状态 %d ",current_last);
					tip_one_line(buffer);
					d0 = A2T(buffer);
					m_plane_status.SetWindowTextW(d0);
					break;
			}
		}
		else
		{
			sprintf(buffer,"正飞往第%d个航点",current_last);
			tip_one_line(buffer);
			d0 = A2T(buffer);
			m_plane_status.SetWindowTextW(d0);
		}
	}
}
/* set */