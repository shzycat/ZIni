# ZIni
A Real Crude Header-Only Cross-Platform Ini-File-Reader Written by C++

First step is to support the "utf-8 file" (or other encode compatible with ASCII)

NOTE :
It overload the [] , so you can get a std::string like this: oneZIniObject["x"]["y"]; but you should never change the value through this method. 
it's a UB here.