:build
IarBuild.exe Prj\IAR\vcan_Kinetis.ewp -make FN15_Debug -log info
echo 按任意键再次编译
pause
goto build
