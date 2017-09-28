#include "EESTraderDemo.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;


TraderDemo::TraderDemo(void)
{
}


TraderDemo::~TraderDemo(void)
{
}

void TraderDemo::Run()
{
	InputParam();

	bool ret = Init();
	if (!ret)
	{
		Pause();
		return;
	}
	DemoSleep();
	NewOrder();

	DemoSleep();
	CxlOrder();

	DemoSleep();
	Close();

	Pause();
	
}

bool TraderDemo::Init()
{

	bool ret = LoadEESTrader();
	if (!ret)
	{
		return false;
	}

	RESULT ret_err = m_tradeApi->ConnServer(m_traderServerIp.c_str(), m_traderServerPort, this);
	if (ret_err != NO_ERROR)
	{
		printf("����REM���׷�����ʧ��!\n");
		return false;
	}
	
	return true;
}

void TraderDemo::Close()
{
	if (m_tradeApi)
	{
		m_tradeApi->DisConnServer();
	}

	UnloadEESTrader();
}

void TraderDemo::DemoSleep()
{
#ifdef WIN32

	Sleep(1000);
#else

	usleep(1000000);

#endif
	
}

void TraderDemo::Pause()
{
	string str_temp;

	printf("\n�������ַ�������:\n");
	cin >> str_temp;
}

bool TraderDemo::LoadEESTrader()
{
#ifdef WIN32

	return Windows_LoadEESTrader();

#else

	return Linux_LoadEESTrader();

#endif
}

void TraderDemo::UnloadEESTrader()
{
#ifdef WIN32

	return Windows_UnloadEESTrader();

#else

	return Linux_UnloadEESTrader();

#endif
}

bool TraderDemo::Windows_LoadEESTrader()
{
#ifdef WIN32

	m_handle =  LoadLibrary(EES_TRADER_DLL_NAME);
	if (!m_handle)
	{
		printf("����REM���׶�̬��(%s)ʧ��\n", EES_TRADER_DLL_NAME);
		return false;
	}

	funcCreateEESTraderApi createFun = (funcCreateEESTraderApi)GetProcAddress(m_handle, CREATE_EES_TRADER_API_NAME);
	if (!createFun)
	{
		printf("��ȡREM���׶�̬�⺯����ַ(%s)ʧ��!\n", CREATE_EES_TRADER_API_NAME);
		return false;
	}

	m_distoryFun = (funcDestroyEESTraderApi)GetProcAddress(m_handle, DESTROY_EES_TRADER_API_NAME);
	if (!createFun)
	{
		printf("��ȡREM���׶�̬�⺯����ַ(%s)ʧ��!\n", DESTROY_EES_TRADER_API_NAME);
		return false;
	}

	m_tradeApi = createFun();
	if (!m_tradeApi)
	{
		printf("����REM���׶���ʧ��!\n");
		return false;
	}

#endif

	return true;
}

void TraderDemo::Windows_UnloadEESTrader()
{
#ifdef WIN32

	if (m_tradeApi)
	{
		m_distoryFun(m_tradeApi);
		m_tradeApi = NULL;
		m_distoryFun = NULL;
	}

	if (m_handle)
	{
		FreeLibrary(m_handle);
		m_handle = NULL;
	}
#endif
}

bool TraderDemo::Linux_LoadEESTrader()
{
#ifndef WIN32
	m_handle =  dlopen(EES_TRADER_DLL_NAME, RTLD_LAZY);
	if (!m_handle)
	{
		printf("����REM���׶�̬��(%s)ʧ��\n", EES_TRADER_DLL_NAME);
		return false;
	}

	funcCreateEESTraderApi createFun = (funcCreateEESTraderApi)dlsym(m_handle, CREATE_EES_TRADER_API_NAME);
	if (!createFun)
	{
		printf("��ȡREM���׶�̬�⺯����ַ(%s)ʧ��!\n", CREATE_EES_TRADER_API_NAME);
		return false;
	}

	m_distoryFun = (funcDestroyEESTraderApi)dlsym(m_handle, DESTROY_EES_TRADER_API_NAME);
	if (!createFun)
	{
		printf("��ȡREM���׶�̬�⺯����ַ(%s)ʧ��!\n", DESTROY_EES_TRADER_API_NAME);
		return false;
	}

	m_tradeApi = createFun();
	if (!m_tradeApi)
	{
		printf("����REM���׶���ʧ��!\n");
		return false;
	}
#endif

	return true;
}

void TraderDemo::Linux_UnloadEESTrader()
{
#ifndef WIN32
	if (m_tradeApi)
	{
		m_distoryFun(m_tradeApi);
		m_tradeApi = NULL;
		m_distoryFun = NULL;
	}

	if (m_handle)
	{
		dlclose(m_handle);
		m_handle = NULL;
	}
#endif
}

void TraderDemo::InputParam()
{
	m_traderServerIp = "10.1.52.212";
	m_traderServerPort = 20000;
	m_logonId = "00000001";
	m_logonPwd = "123456";
	m_account = "00612955";
	m_symbol = "IF1406";
	m_local_mac = "00-01-02-03-04-05";
	m_quantity = 2;

	string str_temp;
	string str_no = "n";

	
	cout << "���׷�����IP��ַ: " << m_traderServerIp << endl;
	cout << "���׷������˿�  : " << m_traderServerPort << endl;
	cout << "��¼���        : " << m_logonId << endl;
	cout << "��¼����        : " << m_logonPwd << endl;
	cout << "�����ʺ�        : " << m_account << endl;
	cout << "��Լ            : " << m_symbol << endl;
	cout << "����            : " << m_quantity << endl;
	cout << "����MAC��ַ		 : " << m_local_mac << endl;
	cout << "�Ƿ�ʹ��ȱʡ���ò��� (y/n) ";
	cin >> str_temp;
	if (str_no != str_temp)
	{
		return;
	}

	cout << "�Ƿ�ʹ��ȱʡ���׷�����IP��ַ " << m_traderServerIp << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "�������µĽ��׷�����IP��ַ: ";
		cin >> str_temp;
		m_traderServerIp = str_temp;
	}

	cout << "�Ƿ�ʹ��ȱʡ���׷������˿� " << m_traderServerPort << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "�������µĽ��׷������˿�: ";
		cin >> str_temp;
		m_traderServerPort = atoi(str_temp.c_str());
	}

	cout << "�Ƿ�ʹ��ȱʡ��¼��� " << m_logonId << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "�������µĵ�¼���: ";
		cin >> str_temp;
		m_logonId = str_temp;
	}

	cout << "�Ƿ�ʹ��ȱʡ��¼���� " << m_logonPwd << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "�������µĵ�¼����: ";
		cin >> str_temp;
		m_logonPwd = str_temp;
	}

	cout << "�Ƿ�ʹ��ȱʡ�����ʺ� " << m_account << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "�������µĽ����ʺ�: ";
		cin >> str_temp;
		m_account = str_temp;
	}

	cout << "�Ƿ�ʹ��ȱʡ���׺�Լ " << m_symbol << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "�������µĽ��׺�Լ: ";
		cin >> str_temp;
		m_symbol = str_temp;
	}

	cout << "�Ƿ�ʹ��ȱʡ�������� " << m_quantity << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "�������µĽ�������: ";
		cin >> str_temp;
		m_quantity = atoi(str_temp.c_str());
	}

	str_temp = "";
	while (str_temp.length() != 17)
	{
		cout << "�������µı���MAC��ַ����ʽ����: 00-01-02-03-04-05";
		cin >> str_temp;
	}
	
	m_local_mac = str_temp;

}

void TraderDemo::Logon()
{
	if (!m_tradeApi)
	{
		printf("��Ч��REM����\n");
		return;
	}

	m_tradeApi->UserLogon(m_logonId.c_str(), m_logonPwd.c_str(), "TradeDemo", m_local_mac.c_str());
}

void TraderDemo::NewOrder()
{
	string str_temp;
	string str_no = "n";
	cout << "�Ƿ��Ͷ��� (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		return;
	}

	if (!m_tradeApi)
	{
		printf("��Ч��REM����\n");
		return;
	}


	EES_ClientToken order_token = 0;
	m_tradeApi->GetMaxToken(&order_token);

	EES_EnterOrderField temp;
	memset(&temp, 0, sizeof(EES_EnterOrderField));
	temp.m_Tif = EES_OrderTif_Day;
	temp.m_HedgeFlag = EES_HedgeFlag_Speculation;

	strcpy(temp.m_Account, m_account.c_str());
	strcpy(temp.m_Symbol, m_symbol.c_str());
	temp.m_Side = EES_SideType_open_long;
	//temp.m_Side = EES_SideType_open_short;
	temp.m_Exchange = EES_ExchangeID_cffex;
	temp.m_SecType = EES_SecType_fut;
	temp.m_Price = 2100.0;
	temp.m_Qty = m_quantity;
	temp.m_ClientOrderToken = order_token + 1;

	RESULT ret = m_tradeApi->EnterOrder(&temp);
	if (ret != NO_ERROR)
	{
		printf("���Ͷ���ʧ��(%d)\n", ret);
		return;
	}

	printf("���Ͷ����ɹ�\n");
}

void TraderDemo::CxlOrder()
{
	string str_temp;
	string str_no = "n";
	cout << "�Ƿ��ͳ������� (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		return;
	}


	if (!m_tradeApi)
	{
		printf("��Ч��REM����\n");
		return;
	}

	EES_CancelOrder  temp;
	memset(&temp, 0, sizeof(EES_CancelOrder));

	strcpy(temp.m_Account, m_account.c_str());
	temp.m_Quantity = m_quantity;
	temp.m_MarketOrderToken = m_marketOrderID;

	RESULT ret = m_tradeApi->CancelOrder(&temp);
	if (ret != NO_ERROR)
	{
		printf("���ͳ�������ʧ��(%d)\n", ret);
		return;
	}

	printf("���ͳ�������ɹ�\n");
}

void TraderDemo::OnConnection(ERR_NO errNo, const char* pErrStr)
{
	if (errNo != NO_ERROR)
	{
		printf("����REM���׷�����ʧ��(%d), %s!\n", errNo,pErrStr);
		return;
	}

	Logon();
}


void TraderDemo::OnDisConnection(ERR_NO errNo, const char* pErrStr)
{
	printf("��REM���׷��������ӶϿ�(%d), %s!\n", errNo,pErrStr);
}


void TraderDemo::OnUserLogon(EES_LogonResponse* pLogon)
{
	if (!pLogon)
	{
		printf("���յ�REM���׷�������¼��Ӧ,�������ϢΪ��!\n");
		return;
	}

	if (pLogon->m_Result != NO_ERROR)
	{
		printf("��¼REM���׷�����ʧ��, %d\n", pLogon->m_Result);
		return;
	}

	printf("��¼REM���׷������ɹ�, ������(%u), ���Ķ������(%d)\n", pLogon->m_TradingDate, pLogon->m_MaxToken);
}

void TraderDemo::OnOrderAccept(EES_OrderAcceptField* pAccept)
{
	if (!pAccept)
	{
		printf("���յ�REM���׷��������صĶ�����������Ϣ,�������ϢΪ��!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("������REM����\n");
	printf("---------------------------------------------------------\n");
	printf("�ͻ��������:     %d\n", pAccept->m_ClientOrderToken);
	printf("REM�������� :     %lld\n", pAccept->m_MarketOrderToken);
	printf("��Լ        :     %s\n", pAccept->m_Symbol);
	printf("�۸�        :     %.4f\n", pAccept->m_Price);
	printf("����        :     %d\n", pAccept->m_Qty);
	printf("\n");

	m_marketOrderID = pAccept->m_MarketOrderToken;
}

void TraderDemo::OnOrderMarketAccept(EES_OrderMarketAcceptField* pAccept)
{
	if (!pAccept)
	{
		printf("���յ�REM���׷��������صĶ������г�������Ϣ,�������ϢΪ��!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("�������г�����\n");
	printf("---------------------------------------------------------\n");
	printf("REM�������� :     %lld\n", pAccept->m_MarketOrderToken);
	printf("�г�������  :     %s\n", pAccept->m_MarketOrderId);
	printf("\n");

	m_marketOrderID = pAccept->m_MarketOrderToken;
}

void TraderDemo::OnOrderReject(EES_OrderRejectField* pReject)
{
	if (!pReject)
	{
		printf("���յ�REM���׷��������صĶ������г��ܾ���Ϣ,�������ϢΪ��!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("�������г��ܾ�\n");
	printf("---------------------------------------------------------\n");
	printf("�ͻ��������:     %d\n", pReject->m_ClientOrderToken);
	printf("�ܾ���      :     %d\n", int(pReject->m_RejectedMan));
	printf("�ܾ�ԭ��    :     %d\n", int(pReject->m_ReasonCode));
	printf("�Ϸ��Լ��  :     %s\n", pReject->m_GrammerText);
	printf("��ؼ��    :     %s\n", pReject->m_RiskText);
	printf("\n");
}

void TraderDemo::OnOrderMarketReject(EES_OrderMarketRejectField* pReject)
{
	if (!pReject)
	{
		printf("���յ�REM���׷��������صĶ������ܾ���Ϣ,�������ϢΪ��!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("����REM���ܾ�\n");
	printf("---------------------------------------------------------\n");
	printf("�г�������  :     %d\n", pReject->m_MarketOrderToken);
	printf("\n");
}

void TraderDemo::OnOrderExecution(EES_OrderExecutionField* pExec)
{
	if (!pExec)
	{
		printf("���յ�REM���׷��������صĶ����ɽ���Ϣ,�������ϢΪ��!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("�����ɽ�\n");
	printf("---------------------------------------------------------\n");
	printf("�ͻ��������:     %d\n", pExec->m_ClientOrderToken);
	printf("�г��������:     %lld\n", pExec->m_MarketOrderToken);
	printf("�ɽ�����    :     %d\n", pExec->m_Quantity);
	printf("�ɽ��۸�    :     %.4f\n", pExec->m_Price);
	printf("�ɽ����    :     %lld\n", pExec->m_ExecutionID);
	printf("\n");
}

void TraderDemo::OnOrderCxled(EES_OrderCxled* pCxled)
{
	if (!pCxled)
	{
		printf("���յ�REM���׷��������صĶ���������Ϣ,�������ϢΪ��!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("����������\n");
	printf("---------------------------------------------------------\n");
	printf("�ͻ��������:     %d\n", pCxled->m_ClientOrderToken);
	printf("�г��������:     %lld\n", pCxled->m_MarketOrderToken);
	printf("\n");
}

void TraderDemo::OnCxlOrderReject(EES_CxlOrderRej* pReject)
{
	if (!pReject)
	{
		printf("���յ�REM���׷��������صĳ������ܾ���Ϣ,�������ϢΪ��!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("�������ܾ�\n");
	printf("---------------------------------------------------------\n");
	printf("�г��������:     %lld\n", pReject->m_MarketOrderToken);
	printf("������      :     %u\n", pReject->m_ReasonCode);
	printf("����ԭ��    :     %s\n", pReject->m_ReasonText);
	printf("\n");
}
