start jflashlite
:build
echo 按任意键再次编译
pause
IarBuild.exe Prj\IAR\vcan_Kinetis.ewp -make FN15_Debug -log info
goto build
