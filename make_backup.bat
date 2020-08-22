@echo off
set "backupfolder=..\.____Backups\vVRC7_backups\"
echo build>__exclusions.txt
echo __exclusions.txt>>__exclusions.txt

for /f %%I in ('wmic os get localdatetime ^|find "20"') do set dd=%%I
echo %dd%
set dd=%dd:~6,2%-%dd:~4,2%-%dd:~0,4%
set dt=%time:~0,-6%
set dt=%dt::=.%

echo Target = %backupfolder%vVRC7 %dd% %dt%\
xcopy /E /Q /I /EXCLUDE:__exclusions.txt "*.*" "%backupfolder%vVRC7 %dd% %dt%\"
del /F /Q __exclusions.txt
