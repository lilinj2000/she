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
		printf("连接REM交易服务器失败!\n");
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

	printf("\n按任意字符键继续:\n");
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
		printf("加载REM交易动态库(%s)失败\n", EES_TRADER_DLL_NAME);
		return false;
	}

	funcCreateEESTraderApi createFun = (funcCreateEESTraderApi)GetProcAddress(m_handle, CREATE_EES_TRADER_API_NAME);
	if (!createFun)
	{
		printf("获取REM交易动态库函数地址(%s)失败!\n", CREATE_EES_TRADER_API_NAME);
		return false;
	}

	m_distoryFun = (funcDestroyEESTraderApi)GetProcAddress(m_handle, DESTROY_EES_TRADER_API_NAME);
	if (!createFun)
	{
		printf("获取REM交易动态库函数地址(%s)失败!\n", DESTROY_EES_TRADER_API_NAME);
		return false;
	}

	m_tradeApi = createFun();
	if (!m_tradeApi)
	{
		printf("创建REM交易对象失败!\n");
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
		printf("加载REM交易动态库(%s)失败\n", EES_TRADER_DLL_NAME);
		return false;
	}

	funcCreateEESTraderApi createFun = (funcCreateEESTraderApi)dlsym(m_handle, CREATE_EES_TRADER_API_NAME);
	if (!createFun)
	{
		printf("获取REM交易动态库函数地址(%s)失败!\n", CREATE_EES_TRADER_API_NAME);
		return false;
	}

	m_distoryFun = (funcDestroyEESTraderApi)dlsym(m_handle, DESTROY_EES_TRADER_API_NAME);
	if (!createFun)
	{
		printf("获取REM交易动态库函数地址(%s)失败!\n", DESTROY_EES_TRADER_API_NAME);
		return false;
	}

	m_tradeApi = createFun();
	if (!m_tradeApi)
	{
		printf("创建REM交易对象失败!\n");
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

	
	cout << "交易服务器IP地址: " << m_traderServerIp << endl;
	cout << "交易服务器端口  : " << m_traderServerPort << endl;
	cout << "登录编号        : " << m_logonId << endl;
	cout << "登录密码        : " << m_logonPwd << endl;
	cout << "交易帐号        : " << m_account << endl;
	cout << "合约            : " << m_symbol << endl;
	cout << "数量            : " << m_quantity << endl;
	cout << "本机MAC地址		 : " << m_local_mac << endl;
	cout << "是否使用缺省配置参数 (y/n) ";
	cin >> str_temp;
	if (str_no != str_temp)
	{
		return;
	}

	cout << "是否使用缺省交易服务器IP地址 " << m_traderServerIp << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "请输入新的交易服务器IP地址: ";
		cin >> str_temp;
		m_traderServerIp = str_temp;
	}

	cout << "是否使用缺省交易服务器端口 " << m_traderServerPort << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "请输入新的交易服务器端口: ";
		cin >> str_temp;
		m_traderServerPort = atoi(str_temp.c_str());
	}

	cout << "是否使用缺省登录编号 " << m_logonId << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "请输入新的登录编号: ";
		cin >> str_temp;
		m_logonId = str_temp;
	}

	cout << "是否使用缺省登录密码 " << m_logonPwd << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "请输入新的登录密码: ";
		cin >> str_temp;
		m_logonPwd = str_temp;
	}

	cout << "是否使用缺省交易帐号 " << m_account << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "请输入新的交易帐号: ";
		cin >> str_temp;
		m_account = str_temp;
	}

	cout << "是否使用缺省交易合约 " << m_symbol << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "请输入新的交易合约: ";
		cin >> str_temp;
		m_symbol = str_temp;
	}

	cout << "是否使用缺省交易数量 " << m_quantity << " (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		cout << "请输入新的交易数量: ";
		cin >> str_temp;
		m_quantity = atoi(str_temp.c_str());
	}

	str_temp = "";
	while (str_temp.length() != 17)
	{
		cout << "请输入新的本机MAC地址，格式类似: 00-01-02-03-04-05";
		cin >> str_temp;
	}
	
	m_local_mac = str_temp;

}

void TraderDemo::Logon()
{
	if (!m_tradeApi)
	{
		printf("无效的REM对象\n");
		return;
	}

	m_tradeApi->UserLogon(m_logonId.c_str(), m_logonPwd.c_str(), "TradeDemo", m_local_mac.c_str());
}

void TraderDemo::NewOrder()
{
	string str_temp;
	string str_no = "n";
	cout << "是否发送订单 (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		return;
	}

	if (!m_tradeApi)
	{
		printf("无效的REM对象\n");
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
		printf("发送订单失败(%d)\n", ret);
		return;
	}

	printf("发送订单成功\n");
}

void TraderDemo::CxlOrder()
{
	string str_temp;
	string str_no = "n";
	cout << "是否发送撤单请求 (y/n) ";
	cin >> str_temp;
	if (str_no == str_temp)
	{
		return;
	}


	if (!m_tradeApi)
	{
		printf("无效的REM对象\n");
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
		printf("发送撤单请求失败(%d)\n", ret);
		return;
	}

	printf("发送撤单请求成功\n");
}

void TraderDemo::OnConnection(ERR_NO errNo, const char* pErrStr)
{
	if (errNo != NO_ERROR)
	{
		printf("连接REM交易服务器失败(%d), %s!\n", errNo,pErrStr);
		return;
	}

	Logon();
}


void TraderDemo::OnDisConnection(ERR_NO errNo, const char* pErrStr)
{
	printf("与REM交易服务器连接断开(%d), %s!\n", errNo,pErrStr);
}


void TraderDemo::OnUserLogon(EES_LogonResponse* pLogon)
{
	if (!pLogon)
	{
		printf("接收到REM交易服务器登录响应,但结果信息为空!\n");
		return;
	}

	if (pLogon->m_Result != NO_ERROR)
	{
		printf("登录REM交易服务器失败, %d\n", pLogon->m_Result);
		return;
	}

	printf("登录REM交易服务器成功, 交易日(%u), 最大的订单编号(%d)\n", pLogon->m_TradingDate, pLogon->m_MaxToken);
}

void TraderDemo::OnOrderAccept(EES_OrderAcceptField* pAccept)
{
	if (!pAccept)
	{
		printf("接收到REM交易服务器返回的订单被接受消息,但结果信息为空!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("订单被REM接受\n");
	printf("---------------------------------------------------------\n");
	printf("客户订单编号:     %d\n", pAccept->m_ClientOrderToken);
	printf("REM发订单号 :     %lld\n", pAccept->m_MarketOrderToken);
	printf("合约        :     %s\n", pAccept->m_Symbol);
	printf("价格        :     %.4f\n", pAccept->m_Price);
	printf("数量        :     %d\n", pAccept->m_Qty);
	printf("\n");

	m_marketOrderID = pAccept->m_MarketOrderToken;
}

void TraderDemo::OnOrderMarketAccept(EES_OrderMarketAcceptField* pAccept)
{
	if (!pAccept)
	{
		printf("接收到REM交易服务器返回的订单被市场接受消息,但结果信息为空!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("订单被市场接受\n");
	printf("---------------------------------------------------------\n");
	printf("REM发订单号 :     %lld\n", pAccept->m_MarketOrderToken);
	printf("市场订单号  :     %s\n", pAccept->m_MarketOrderId);
	printf("\n");

	m_marketOrderID = pAccept->m_MarketOrderToken;
}

void TraderDemo::OnOrderReject(EES_OrderRejectField* pReject)
{
	if (!pReject)
	{
		printf("接收到REM交易服务器返回的订单被市场拒绝消息,但结果信息为空!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("订单被市场拒绝\n");
	printf("---------------------------------------------------------\n");
	printf("客户订单编号:     %d\n", pReject->m_ClientOrderToken);
	printf("拒绝者      :     %d\n", int(pReject->m_RejectedMan));
	printf("拒绝原因    :     %d\n", int(pReject->m_ReasonCode));
	printf("合法性检查  :     %s\n", pReject->m_GrammerText);
	printf("风控检查    :     %s\n", pReject->m_RiskText);
	printf("\n");
}

void TraderDemo::OnOrderMarketReject(EES_OrderMarketRejectField* pReject)
{
	if (!pReject)
	{
		printf("接收到REM交易服务器返回的订单被拒绝消息,但结果信息为空!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("订单REM被拒绝\n");
	printf("---------------------------------------------------------\n");
	printf("市场订单号  :     %d\n", pReject->m_MarketOrderToken);
	printf("\n");
}

void TraderDemo::OnOrderExecution(EES_OrderExecutionField* pExec)
{
	if (!pExec)
	{
		printf("接收到REM交易服务器返回的订单成交消息,但结果信息为空!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("订单成交\n");
	printf("---------------------------------------------------------\n");
	printf("客户订单编号:     %d\n", pExec->m_ClientOrderToken);
	printf("市场订单编号:     %lld\n", pExec->m_MarketOrderToken);
	printf("成交数量    :     %d\n", pExec->m_Quantity);
	printf("成交价格    :     %.4f\n", pExec->m_Price);
	printf("成交编号    :     %lld\n", pExec->m_ExecutionID);
	printf("\n");
}

void TraderDemo::OnOrderCxled(EES_OrderCxled* pCxled)
{
	if (!pCxled)
	{
		printf("接收到REM交易服务器返回的订单被撤消息,但结果信息为空!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("订单被撤消\n");
	printf("---------------------------------------------------------\n");
	printf("客户订单编号:     %d\n", pCxled->m_ClientOrderToken);
	printf("市场订单编号:     %lld\n", pCxled->m_MarketOrderToken);
	printf("\n");
}

void TraderDemo::OnCxlOrderReject(EES_CxlOrderRej* pReject)
{
	if (!pReject)
	{
		printf("接收到REM交易服务器返回的撤单被拒绝消息,但结果信息为空!\n");
		return;
	}

	printf("---------------------------------------------------------\n");
	printf("撤单被拒绝\n");
	printf("---------------------------------------------------------\n");
	printf("市场订单编号:     %lld\n", pReject->m_MarketOrderToken);
	printf("错误码      :     %u\n", pReject->m_ReasonCode);
	printf("错误原因    :     %s\n", pReject->m_ReasonText);
	printf("\n");
}
