@echo off

rem 一个简单的脚本，用于把windows文档换行符转换成linux文档换行符
rem 例如，把\r\n转换成\n
rem 原理是用type命令，把文件内容输出到屏幕，然后用find命令，把每行末尾的\r过滤掉，最后重定向到新文件中
rem 参考资料：[1] [2]

rem 定义一个函数，用于把windows文档换行符转换成linux文档换行符
:win2lin
set win_file=%1 rem 输入的windows文档文件名，例如test.txt
set lin_file=%2 rem 输出的linux文档文件名，例如test2.txt
rem 用type命令，把文件内容输出到屏幕，然后用find命令，把每行末尾的\r过滤掉，最后重定向到新文件中
type %win_file% | find /v "" > %lin_file%
goto :eof

rem 测试一下
set win_file=d:\winroute.txt
set lin_file=d:\route.txt
route print -4 | find "." | find /v ".." > d:\winroute.txt
call :win2lin %win_file% %lin_file%
echo 转换完成，结果保存在%lin_file%中
exit /b 0
