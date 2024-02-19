@echo off

:: Move to correct working directory
:: https://stackoverflow.com/questions/672693/windows-batch-file-starting-directory-when-run-as-admin
@setlocal enableextensions
@cd /d "%~dp0"

start premake5 --file=project.lua vs2022

echo Successfully set up project.

pause