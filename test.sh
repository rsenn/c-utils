sed "s,\r,," ../pictest/build/mplab/pictest_xc8.mcp |(IFS="="; 
I=0
nl="
"
varname="section"
while read -r NAME VALUE; do 

case "$NAME" in
  \[*\])
     NAME=${NAME#'['}
     NAME=${NAME%']'}
     [ $I -eq 0 ] && {
echo -n "$nl  "
   } || {
     echo -n "$nl  ${varname} = "
 }
     echo "ini_new(&${varname}->next, \"$NAME\");"
     ;;

     *)
       VALUE=${VALUE//"\\"/"\\\\"}
echo "  ini_set(${varname}, \"$NAME\", \"$VALUE\");";
;;
esac
: $((I++))
 done) 
