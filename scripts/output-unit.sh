output-unit () 
{ 
    while [ $# -gt 0 ]; do
        echo "                <Unit filename=\"$1\"><Option compilerVar=\"CC\" /></Unit>";
        shift;
    done
}
