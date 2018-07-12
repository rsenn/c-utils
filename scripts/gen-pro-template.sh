gen-pro-template () 
{ 

  SOURCE=${1%.*}.c

  SOURCES=$(scripts/get-sources.sh build/*/Debug/"${1%.*}")

  : ${SOURCES:="$SOURCE"}
  HEADERS=$(for x in $(sed -n '/include/ { s,.*"\([^"]*\)".*,\1,p }' $SOURCES); do
  find * -iname "$x"; done )

  PROJECT=${SOURCE%.*}.pro

    cat  >$PROJECT <<EOF 
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ${SOURCE%.*}

INCLUDEPATH += . \$\$PWD/lib

HEADERS = ${SOURCES}
SOURCES = ${HEADERS}

include(deployment.pri)
qtcAddDeployment()

EOF
}
