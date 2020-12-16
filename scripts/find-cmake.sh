#!/bin/sh

exec find ${@:-*} "(" -iname CMakeLists.txt -or -iname "*.cmake" -or -iwholename "*/cmake/*" ")" -and -not "(" -wholename "*build/*" -and -not -wholename "*build/cmake/*" ")" -and -not -wholename "*3rdparty*"
