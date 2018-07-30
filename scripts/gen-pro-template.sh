gen-pro-template() { 
 (IFS="
 "
  SOURCE=${1##*/}.c

  SOURCES=$(echo $(scripts/get-sources.sh build/*/Debug/"${SOURCE%.*}" ) | sed 's| [^ ]\+\.h||g')

  : ${SOURCES:="$SOURCE"}
  HEADERS=$(for x in $(sed -n '/include/ { s,.*"\([^"]*\)".*,\1, ; s,lib/,, ; p }' $SOURCES | sort -u); do
  case "$x" in
    '#include'*|*[\<\>]*) continue ;;
  esac
  find . -iname "${x##*/}"; done )
  HEADERS=$(echo $HEADERS |sed 's,\s\+\./, ,g ; s|\sbuild/[^ ]*||g')

  PROJECT=${SOURCE%.*}.pro

    cat  >$PROJECT <<EOF 
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ${SOURCE%.*}

INCLUDEPATH += . \$\$PWD/lib

HEADERS = $(echo ${HEADERS})
SOURCES = $(echo ${SOURCES})

include(deployment.pri)
qtcAddDeployment()

EOF
)
}
