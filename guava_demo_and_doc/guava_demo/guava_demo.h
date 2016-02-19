/*!****************************************************************************
 @note   Copyright (coffee), 2005-2014, Shengli Tech. Co., Ltd.
 @file   guava_demo.h
 @date   2014/8/27   12:44
 @author zhou.hu
 
 @brief   ���ļ���GUAVA�����鲥�ӿڵ�ʾ������

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

	/// \brief ʾ��������ں���
	void run();

private:
	virtual void on_receive_nomal(guava_udp_head* head, guava_udp_normal* data);
	virtual void on_receive_summary(guava_udp_head* head, guava_udp_summary* data);

	string to_string(guava_udp_head* ptr);
	string to_string(guava_udp_normal* ptr);
	string to_string(guava_udp_summary* ptr);

private:
	/// \brief ��ʼ��������������
	void input_param();

	/// \brief ��ʼ��
	bool init();

	/// \brief �ر�
	void close();

	/// \brief ��ͣ
	void pause();



private:
	multicast_info			m_cffex_info;		///< �н��UDP��Ϣ
	guava_quote				m_guava;			///< ������ն���
	bool					m_quit_flag;		///< �˳���־
};

