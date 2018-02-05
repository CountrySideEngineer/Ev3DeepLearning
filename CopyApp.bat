@echo off
@echo Copy App File To Brick Ev3 as Binary.
@echo Open command prompt and change the working folder to current.

SET SRC_BIN="ev3qlearn"
SET DST_PATH="H:\ev3rt\apps"

COPY /Y /B %SRC_BIN% %DST_PATH%
START "" %DST_PATH%
