/*!****************************************************************************
 @note   Copyright (coffee), 2005-2014, Shengli Tech. Co., Ltd.
 @file   guava_demo.h
 @date   2014/8/27   12:44
 @author zhou.hu
 
 @brief   本文件是GUAVA行情组播接口的示例程序

 @note 
******************************************************************************/
#pragma once
#include <vector>
#include "guava_quote.h"

using std::vector;


class guava_demo :public guava_quote_event
{
public:
	guava_demo(void);
	~guava_demo(void);

	/// \brief 示例函数入口函数
	void run();

private:
	virtual void on_receive_nomal(guava_udp_head* head, guava_udp_normal* data);
	virtual void on_receive_summary(guava_udp_head* head, guava_udp_summary* data);

	string to_string(guava_udp_head* ptr);
	string to_string(guava_udp_normal* ptr);
	string to_string(guava_udp_summary* ptr);

private:
	/// \brief 初始化参数调整方法
	void input_param();

	/// \brief 初始化
	bool init();

	/// \brief 关闭
	void close();

	/// \brief 暂停
	void pause();



private:
	multicast_info			m_cffex_info;		///< 中金接UDP信息
	guava_quote				m_guava;			///< 行情接收对象
	bool					m_quit_flag;		///< 退出标志
};

