@echo off
    echo NOTICE: Make sure to run batch file as administrator    
    set source= %~dp0.
    set /p target= "Enter the path to the Arduino libraries directory: "
    set exclude=

    forfiles /P "%source%" /C "cmd /c if @isdir==TRUE ( mklink /d \"%target%\@file\" @path )"