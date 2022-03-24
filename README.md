# ZIni
A Simple Header-Only Cross-Platform Ini-File-Reader Written by C++

First step is to support the "utf-8 file" (or other encode compatible with ASCII)

# NOTE :
1) It overload the [ ] , so you can get a std::string like this: oneZIniObject["x"]["y"]; but you should never change the value through this method. it's a UB here.

2) You can enable the fastMode by calling the member function(enableFastMode()), but as you enable this mode, ZINI will updata the ini file only in it's destructor. This will not affect you to get the correct and lastest value in that ini file(it means you can still get the right value you just set).
Enable this mode can strongly decrease the time-cost of writing file when you have to write a large number of key-value in that ini-file.
In this mode, if you want to immediately update the content in that file, call update(); if you don't call, updating file will be done in it's destructor. And ZIni will not do unnecessary updating.
