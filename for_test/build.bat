@echo OFF

echo 命令行编译批处理
echo by 晨旭
echo chenxublog.com
echo .
echo .
echo .
start jflashlite
:build
echo 按任意键再次编译
pause
cls
echo ↓↓↓↓↓↓↓↓↓↓↓↓编译信息↓↓↓↓↓↓↓↓↓↓↓↓
IarBuild.exe Prj\IAR\vcan_Kinetis.ewp -make FN15_Debug -log info
echo ↑↑↑↑↑↑↑↑↑↑↑↑编译结束↑↑↑↑↑↑↑↑↑↑↑↑
echo .
goto build
