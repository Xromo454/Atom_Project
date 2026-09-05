@echo off
set "PATH=%~dp0runtime;C:\msys64\ucrt64\bin;%PATH%"
"%~dp0app.exe"
pause