@echo on
@echo 复制windows交易API文件
copy ..\..\lib\debug\EESTraderApi.dll "trade\C++\win32_debug\" /Y
copy ..\..\lib\debug\EESTraderApi.lib "trade\C++\win32_debug\" /Y

copy ..\..\lib\release\EESTraderApi.dll "trade\C++\win32_release\" /Y
copy ..\..\lib\release\EESTraderApi.lib "trade\C++\win32_release\" /Y
@echo 复制windows行情API文件
copy ..\..\lib\debug\EESQuoteApi.dll "quote\C++\win32_debug\" /Y
copy ..\..\lib\debug\EESQuoteApi.lib "quote\C++\win32_debug\" /Y

copy ..\..\lib\release\EESQuoteApi.dll "quote\C++\win32_release\" /Y
copy ..\..\lib\release\EESQuoteApi.lib "quote\C++\win32_release\" /Y

@echo 复制windows CTP API文件

copy ..\..\lib\debug\thostmduserapi.dll CTP\win32_debug\ /Y
copy ..\..\lib\debug\thostmduserapi.lib CTP\win32_debug\ /Y
copy ..\..\lib\debug\thosttraderapi.dll CTP\win32_debug\ /Y
copy ..\..\lib\debug\thosttraderapi.lib CTP\win32_debug\ /Y
copy ..\..\lib\debug\EESQuoteApi.dll CTP\win32_debug\ /Y
copy ..\..\lib\debug\EESQuoteApi.lib CTP\win32_debug\ /Y

copy ..\..\lib\release\thostmduserapi.dll CTP\win32_release\ /Y
copy ..\..\lib\release\thostmduserapi.lib CTP\win32_release\ /Y
copy ..\..\lib\release\thosttraderapi.dll CTP\win32_release\ /Y
copy ..\..\lib\release\thosttraderapi.lib CTP\win32_release\ /Y
copy ..\..\lib\release\EESQuoteApi.dll CTP\win32_release\ /Y
copy ..\..\lib\release\EESQuoteApi.lib CTP\win32_release\ /Y

rem @echo 复制linux交易API文件
rem copy ..\..\lib\debug\libEESTraderApi.so "trade\C++\linux64_debug\" /Y
rem @echo 复制linux行情API文件
rem copy ..\..\lib\debug\libEESQuoteApi.so "quote\C++\linux64_debug\" /Y
rem @echo 复制linux CTP API文件
rem copy ..\..\lib\debug\libEESQuoteApi.so CTP\linux64_debug\ /Y
rem copy ..\..\lib\debug\ctplike\libthostmduserapi.so CTP\linux64_debug\ /Y
rem copy ..\..\lib\debug\ctplike\libthosttraderapi.so CTP\linux64_debug\ /Y
