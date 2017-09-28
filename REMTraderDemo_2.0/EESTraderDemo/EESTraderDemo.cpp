#include "EESTraderDemo.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;


TraderDemo::TraderDemo(void)
{
	m_logonStatus = 0;
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
	DemoSleep(1000);
	NewOrder();

	DemoSleep(1000);
	CxlOrder();

	DemoSleep(1000);
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

	RESULT ret_err = m_tradeApi->ConnServer(m_tradeServerIp.c_str(), m_tradeServerPort, this, m_queryServerIp.c_str(), m_queryServerPort);
	if (ret_err != NO_ERROR)
	{
		printf("connect to REM server failed!\n");
		return false;
	}

	int waitTime = 0;//µÈ´ý³¬Ê±
	while (m_logonStatus != 2 && m_logonStatus != 3)
	{
		DemoSleep(100);
		waitTime++;
		if (waitTime >= 50)//5Ãë³¬Ê±
		{
			printf("wait for logon response timeout!\n");
			return false;
		}
	}
	
	return (2 == m_logonStatus);
}

void TraderDemo::Close()
{
	if (m_tradeApi)
	{
		m_tradeApi->DisConnServer();
	}

	UnloadEESTrader();
}

void TraderDemo::DemoSleep(int nMilliSeconds)
{
#ifdef WIN32

	Sleep(nMilliSeconds);
#else

	usleep(1000* nMilliSeconds);

#endif
	
}

void TraderDemo::Pause()
{
	string str_temp;

	printf("\npress any key to continue:\n");
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
		printf("load library(%s) failed.\n", EES_TRADER_DLL_NAME);
		return false;
	}

	funcCreateEESTraderApi createFun = (funcCreateEESTraderApi)GetProcAddress(m_handle, CREATE_EES_TRADER_API_NAME);
	if (!createFun)
	{
		printf("get function addresss(%s) failed!\n", CREATE_EES_TRADER_API_NAME);
		return false;
	}

	m_distoryFun = (funcDestroyEESTraderApi)GetProcAddress(m_handle, DESTROY_EES_TRADER_API_NAME);
	if (!createFun)
	{
		printf("get function addresss(%s) failed!\n", DESTROY_EES_TRADER_API_NAME);
		return false;
	}

	m_tradeApi = createFun();
	if (!m_tradeApi)
	{
		printf("create trade API object failed!\n");
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
		printf("load library(%s) failed.\n", EES_TRADER_DLL_NAME);
		return false;
	}

	funcCreateEESTraderApi createFun = (funcCreateEESTraderApi)dlsym(m_handle, CREATE_EES_TRADER_API_NAME);
	if (!createFun)
	{
		printf("get function addresss(%s) failed!\n", CREATE_EES_TRADER_API_NAME);
		return false;
	}

	m_distoryFun = (funcDestroyEESTraderApi)dlsym(m_handle, DESTROY_EES_TRADER_API_NAME);
	if (!createFun)
	{
		printf("get function addresss(%s) failed!\n", DESTROY_EES_TRADER_API_NAME);
		return false;
	}

	m_tradeApi = createFun();
	if (!m_tradeApi)
	{
		printf("create trade API object failed!\n");
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
	m_tradeServerIp = "127.0.0.1";
	m_tradeServerPort = 20000;
	m_queryServerIp = "127.0.0.1";
	m_queryServerPort = 20001;
	m_logonId = "00000001";
	m_logonPwd = "123456";
	m_account = "12345678";
	m_symbol = "IF1801";
	m_local_mac = "00-01-02-03-04-05";
	m_quantity = 2;

	string str_temp;
	string str_no = "n";

	
	cout << "trade server IP: "   << m_tradeServerIp << endl;
	cout << "trade server port: " << m_tradeServerPort << endl;
	cout << "query server IP: "   << m_queryServerIp << endl;
	cout << "query server port: " << m_queryServerPort << endl;
	cout << "logon id: " << m_logonId << endl;
	cout << "logon password: " << m_logonPwd << endl;
	cout << "account: " << m_account << endl;
	cout << "contract id: " << m_symbol << endl;
	cout << "order quantity: " << m_quantity << endl;
	cout << "local MAC: " << m_local_mac << endl;
	cout << "use these params? (y/n) ";
	cin >> str_temp;
	if (str_no != str_temp)
	{
		return;
	}

	
	cout << "input trade server IP: ";
	cin >> str_temp;
	m_tradeServerIp = str_temp;
	
	cout << "input trade server port: ";
	cin >> str_temp;
	m_tradeServerPort = atoi(str_temp.c_str());

	cout << "input query server IP: ";
	cin >> str_temp;
	m_queryServerIp = str_temp;

	cout << "input query server port: ";
	cin >> str_temp;
	m_queryServerPort = atoi(str_temp.c_str());

	cout << "input logon id: ";
	cin >> str_temp;
	m_logonId = str_temp;
	
	cout << "input password: ";
	cin >> str_temp;
	m_logonPwd = str_temp;
	
	cout << "input account: ";
	cin >> str_temp;
	m_account = str_temp;
	
	cout << "input contract id: ";
	cin >> str_temp;
	m_symbol = str_temp;
	
	cout << "input order qty: ";
	cin >> str_temp;
	m_quantity = atoi(str_temp.c_str());
	
	cout << "input new MAC, format like: 00-01-02-03-04-05";
	cin >> str_temp;	
	m_local_mac = str_temp;

}

void TraderDemo::Logon()
{
	if (!m_tradeApi)
	{
		printf("INVALID api object\n");
		return;
	}

	m_tradeApi->UserLogon(m_logonId.c_str(), m_logonPwd.c_str(), "TradeDemo", m_local_mac.c_str());
}

void TraderDemo::NewOrder()
{
	string str_temp;
	string str_no = "n";
	cout << "send order? (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
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
		printf("send order failed(%d)\n", ret);
		return;
	}

	printf("send order successfully\n");
}

void TraderDemo::CxlOrder()
{
	string str_temp;
	string str_no = "n";
	cout << "send cancel? (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
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
		printf("send cancel failed(%d)\n", ret);
		return;
	}

	printf("send cancel successfully\n");
}

void TraderDemo::OnConnection(ERR_NO errNo, const char* pErrStr)
{
	if (errNo != NO_ERROR)
	{
		printf("connect to rem server failed(%d), %s!\n", errNo, pErrStr);
		return;
	}
	m_logonStatus = 1;
	Logon();
}


void TraderDemo::OnDisConnection(ERR_NO errNo, const char* pErrStr)
{
	printf("disconnect from rem server(%d), %s!\n", errNo,pErrStr);
	m_logonStatus = 3;
}


void TraderDemo::OnUserLogon(EES_LogonResponse* pLogon)
{	

	if (pLogon->m_Result != NO_ERROR)
	{
		m_logonStatus = 3;
		printf("logon failed, result=%d\n", pLogon->m_Result);
		return;
	}
	m_logonStatus = 2;
	printf("logon successfully, trading date(%u), max token(%d)\n", pLogon->m_TradingDate, pLogon->m_MaxToken);
}

void TraderDemo::OnOrderAccept(EES_OrderAcceptField* pAccept)
{

	printf("---------------------------------------------------------\n");
	printf("OnOrderAccept\n");
	printf("---------------------------------------------------------\n");
	printf("ClientOrderToken:     %d\n", pAccept->m_ClientOrderToken);
	printf("REM Order ID    :     %lld\n", pAccept->m_MarketOrderToken);
	printf("Contract ID     :     %s\n", pAccept->m_Symbol);
	printf("price           :     %.4f\n", pAccept->m_Price);
	printf("quantity        :     %d\n", pAccept->m_Qty);
	printf("\n");

	m_marketOrderID = pAccept->m_MarketOrderToken;
}

void TraderDemo::OnOrderMarketAccept(EES_OrderMarketAcceptField* pAccept)
{
	
	printf("---------------------------------------------------------\n");
	printf("OnOrderMarketAccept\n");
	printf("---------------------------------------------------------\n");
	printf("REM Order ID         :     %lld\n", pAccept->m_MarketOrderToken);
	printf("Exchange OrderSysID  :     %s\n", pAccept->m_MarketOrderId);
	printf("\n");

	m_marketOrderID = pAccept->m_MarketOrderToken;
}

void TraderDemo::OnOrderReject(EES_OrderRejectField* pReject)
{
	printf("---------------------------------------------------------\n");
	printf("OnOrderReject\n");
	printf("---------------------------------------------------------\n");
	printf("ClientOrderToken:     %d\n", pReject->m_ClientOrderToken);	
	printf("ReasonCode      :     %d\n", int(pReject->m_ReasonCode));
	printf("GrammerText     :     %s\n", pReject->m_GrammerText);
	printf("RiskText        :     %s\n", pReject->m_RiskText);
	printf("\n");
}

void TraderDemo::OnOrderMarketReject(EES_OrderMarketRejectField* pReject)
{
	printf("---------------------------------------------------------\n");
	printf("OnOrderMarketReject\n");
	printf("---------------------------------------------------------\n");
	printf("Exchange OrderSysID  :     %lld\n", pReject->m_MarketOrderToken);
	printf("Reason Text          :     %.100s\n", pReject->m_ReasonText);
	printf("\n");
}

void TraderDemo::OnOrderExecution(EES_OrderExecutionField* pExec)
{
	
	printf("---------------------------------------------------------\n");
	printf("OnOrderExecution\n");
	printf("---------------------------------------------------------\n");
	printf("ClientOrderToken:     %d\n",   pExec->m_ClientOrderToken);
	printf("MarketOrderToken:     %lld\n", pExec->m_MarketOrderToken);
	printf("Quantity        :     %d\n",   pExec->m_Quantity);
	printf("Price           :     %.4f\n", pExec->m_Price);
	printf("ExecutionID     :     %lld\n", pExec->m_ExecutionID);
	printf("MarketExecID    :     %.30s\n", pExec->m_MarketExecID);
	printf("\n");
}

void TraderDemo::OnOrderCxled(EES_OrderCxled* pCxled)
{

	printf("---------------------------------------------------------\n");
	printf("OnOrderCxled\n");
	printf("---------------------------------------------------------\n");
	printf("ClientOrderToken:     %d\n",   pCxled->m_ClientOrderToken);
	printf("MarketOrderToken:     %lld\n", pCxled->m_MarketOrderToken);
	printf("\n");
}

void TraderDemo::OnCxlOrderReject(EES_CxlOrderRej* pReject)
{	

	printf("---------------------------------------------------------\n");
	printf("OnCxlOrderReject\n");
	printf("---------------------------------------------------------\n");
	printf("m_MarketOrderToken:     %lld\n", pReject->m_MarketOrderToken);
	printf("m_ReasonCode      :     %u\n", pReject->m_ReasonCode);
	printf("m_ReasonText    :     %s\n", pReject->m_ReasonText);
	printf("\n");
}
