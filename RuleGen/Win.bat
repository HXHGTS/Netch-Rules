@echo off

route print -4 | find "." | find /v ".." > d:\route.txt

exit /b 0
