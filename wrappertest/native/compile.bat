@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
cl test.cpp /I%DART_SDK%\include /LDd /MDd %DART_SDK%\bin\dart.lib
