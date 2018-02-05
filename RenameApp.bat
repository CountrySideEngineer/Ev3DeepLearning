@echo off
@echo Rename a file "App" into "Ev3Car"

SET target_fil="ev3qlearn"
SET base_fil="app"

IF EXIST %target_fil% GOTO DEL_APP
IF NOT EXIST %target_fil% GOTO RENAME_APP

:DEL_APP
DEL %target_fil%

:RENAME_APP
REN %base_fil% %target_fil%
