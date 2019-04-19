#!/bin/sh

IFS="
"
SOURCES=$(find "$@" -name "*.[ch]" )

find_source() {
  echo "$SOURCES" | grep -E "(/|^)$1\$"
}

grep -H -n '^\s*#\s*include' $SOURCES | 
  (IFS=":"; while read -r FILE LINE INCLUDE_CMD; do
  (IFS="
"; INCLUDE=${INCLUDE_CMD#*include}
   INCLUDE=${INCLUDE#[ \r\t]}
   INCLUDE=${INCLUDE%[ \r\t]}
   case "$INCLUDE" in
     *"<"*">"*) SYSINCLUDE=true ;;
     *) SYSINCLUDE=false ;;
   esac
   
   INCLUDE_FILE=${INCLUDE#*[\"<]}
   INCLUDE_FILE=${INCLUDE_FILE%[\">]*}
   INCLUDE_PATHS=$(find_source "$INCLUDE_FILE")

   for INCLUDE_PATH in $INCLUDE_PATHS; do 

     if [ ! -e "$INCLUDE_PATH" ]; then
       echo "File '$INCLUDE_PATH' not found!" 1>&2
       continue
     fi
     INCLUDE_REL=$(realpath --relative-to="$(dirname "$FILE")" "$INCLUDE_PATH")

     if [ "$SYSINCLUDE" = true ]; then
       INCLUDE_OUT="<$INCLUDE_REL>"
     else
       INCLUDE_OUT="\"$INCLUDE_REL\""
    fi

     if [ "$INCLUDE_REL" != "$INCLUDE_FILE" ]; then
       echo "sed -i '$LINE s|$INCLUDE|$INCLUDE_OUT|' $FILE" 
       echo "FILE=$FILE LINE=$LINE INCLUDE=$INCLUDE INCLUDE_REL=$INCLUDE_REL" 1>&2
     fi
   done)
  done)
