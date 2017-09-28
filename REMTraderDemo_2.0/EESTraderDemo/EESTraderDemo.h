/*!****************************************************************************
 @note   Copyright (coffee), 2005-2014, Shengli Tech. Co., Ltd.
 @file   EESTraderDemo.h
 @date   2014/4/27   12:46
 @author zhou.hu
 
 \brief  ������ʾEES����API��ʹ��ʾ��

 @note 
******************************************************************************/
#pragma once
#ifdef WIN32
/// add by zhou.hu review 2014/4/22 Windwosƽ̨���õ�ͷ�ļ�
#include <Windows.h>

typedef HMODULE		T_DLL_HANDLE;

#else
/// add by zhou.hu review 2014/4/22 linuxƽ̨���õ�ͷ�ļ�
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

typedef void*		T_DLL_HANDLE;

#endif

#include <string>
#include "EesTraderApi.h"

using std::string;

class TraderDemo : public EESTraderEvent
{
public:
	TraderDemo(void);
	virtual ~TraderDemo(void);

	/// \brief ������ں���
	void Run();

private:
	/// \brief ��ʼ��
	bool Init();
	/// \brief �ر�
	void Close();

	/// \brief ��ʱ����
	void DemoSleep(int nMilliSeconds);
	/// \brief ��ͣ
	void Pause();

private:
	/// \brief ����EES����API�Ķ�̬��
	bool LoadEESTrader();
	/// \brief �ͷ�EES����API�Ķ�̬��
	void UnloadEESTrader();

	/// \brief Windows����ؽ���API�Ķ�̬��
	bool Windows_LoadEESTrader();
	/// \brief Windows���ͷŽ���API�Ķ�̬��
	void Windows_UnloadEESTrader();

	/// \brief Linux�汾���ؽ���API�Ķ�̬��
	bool Linux_LoadEESTrader();
	/// \brief Linux�汾�ͷŽ���API�Ķ�̬��
	void Linux_UnloadEESTrader();

	/// \brief ��ʼ��������������
	void InputParam();

	/// \brief ��¼
	void Logon();
	/// \brief �µ�
	void NewOrder();
	/// \brief ����
	void CxlOrder();

private:
	/// \brief �����������¼�
	virtual void OnConnection(ERR_NO errNo, const char* pErrStr);
	/// \brief	�����������Ͽ������յ������Ϣ
	virtual void OnDisConnection(ERR_NO errNo, const char* pErrStr);
	/// \brief	��¼��Ϣ�Ļص�
	virtual void OnUserLogon(EES_LogonResponse* pLogon);
	/// \brief	�µ���REM���ܵ��¼�
	virtual void OnOrderAccept(EES_OrderAcceptField* pAccept);
	/// \brief	�µ����г����ܵ��¼�
	virtual void OnOrderMarketAccept(EES_OrderMarketAcceptField* pAccept);
	///	\brief	�µ���REM�ܾ����¼�
	virtual void OnOrderReject(EES_OrderRejectField* pReject);
	///	\brief	�µ����г��ܾ����¼�
	virtual void OnOrderMarketReject(EES_OrderMarketRejectField* pReject);
	///	\brief	�����ɽ�����Ϣ�¼�
	virtual void OnOrderExecution(EES_OrderExecutionField* pExec);
	///	\brief	�����ɹ������¼�
	virtual void OnOrderCxled(EES_OrderCxled* pCxled);
	///	\brief	�������ܾ�����Ϣ�¼�
	virtual void OnCxlOrderReject(EES_CxlOrderRej* pReject);

	
	

private:
	EESTraderApi*				m_tradeApi;				///< EES����API�ӿ�
	T_DLL_HANDLE				m_handle;				///< EES����API���
	funcDestroyEESTraderApi		m_distoryFun;			///< EES����API��̬�����ٺ���

	string						m_tradeServerIp;		///< ����ͨ��IP��ַ
	int							m_tradeServerPort;		///< ����ͨ���˿�
	string						m_queryServerIp;		///< ��ѯͨ��IP��ַ
	int							m_queryServerPort;		///< ��ѯͨ���˿�
	string						m_logonId;				///< ��¼���׷������
	string						m_logonPwd;				///< ��¼���׷�������
	string						m_account;				///< �����ʺ�
	string						m_symbol;				///< ���׺�Լ
	string						m_local_mac;			///< ����MAC��ַ
	int							m_quantity;				///< ��������
	EES_MarketToken				m_marketOrderID;		///< �г��������
	int							m_logonStatus;			///< 0-��δ��¼; 1-���������ڵ�¼; 2-��¼�ɹ�; 3-��¼ʧ�ܣ������Ѿ��Ͽ�����
};

