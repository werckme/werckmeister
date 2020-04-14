@ECHO OFF
call "%VS140COMNTOOLS%\vsvars32.bat"

SET PWD="%cd%"
for %%i in ("%~dp0.") do SET "CWD=%%~fi"

cd %CWD%
cmake .. -G "Visual Studio 14 2015"
msbuild werckmeister.sln /p:Configuration=Release && cpack -G WIX
