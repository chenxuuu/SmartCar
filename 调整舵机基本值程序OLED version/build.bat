@echo OFF

echo �����б���������
echo by ����
echo chenxublog.com
echo .
echo .
echo .
start jflashlite
:build
echo ��������ٴα���
pause
cls
echo ������������������������������Ϣ������������������������
IarBuild.exe Prj\IAR\vcan_Kinetis.ewp -make FN15_Debug -log info
echo �������������������������������������������������������
set d=%date:~0,10%
set t=%time:~0,8%
echo ����ִ��ʱ�䣺 %d% %t%
echo .
goto build
