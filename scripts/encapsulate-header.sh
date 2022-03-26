#!/bin/sh
NL="
"
IFS="$NL"
str_toupper() {
	echo "$@" | tr "[[:lower:]]" "[[:upper:]]"
}

quote() {
	(
		OUT=
		for TOK; do
			case "$TOK" in
			*\ *) TOK="'$TOK'" ;;
			esac
			OUT="${OUT:+$OUT }$TOK"
		done
		echo "$OUT"
	)
}

exec_cmd() (
	[ "$DEBUG" = true ] && quote "$@" 1>&2

	command "$@"
)

check_remove() {
	 eval "exec_cmd $CHECK"
}

check_add() {
	! check_remove
}

add_expr() {
	EXPR="${EXPR:+$EXPR$NL}$*"
}

add_outer() {
	add_expr "1 i\\${NL}$PREPEND${NL}"
	add_expr "\$ a\\${NL}$APPEND${NL}"
}

remove_outer() {
	add_expr "$REMOVE"
}

encapsulate_header() {
	MODE=add
	while [ $# -gt 0 ]; do
		case "$1" in
		-x | --debug)
			DEBUG=true
			shift
 			;;
		-r | --remove)
			MODE=remove
			shift
 			;;
		-d | --defg*)
			ADD=group
			shift
 			;;
		-g | --guard*)
			ADD=guard
			shift
 			;;
		*) break ;;
		esac
	done

	for FILE; do
		(
			BASE=$(basename "$FILE" .h)
			MODULE=$(str_toupper "$BASE")

			case "$ADD" in
			group)
				PREPEND="/**\n * @defgroup   ${BASE}\n * @brief      ${MODULE} module.\n * @{\n */"
				APPEND="/** @} */"
				REMOVE="\\|/\*\*| {  :lp; \\|\*/|! { N; b lp }; /@[{}]/ d }"
				CHECK='grep -q @defgroup "$FILE"'
				;;
			guard)
				PREPEND="#ifndef ${MODULE}_H\n#define ${MODULE}_H"
				APPEND="#endif /* !defined(${MODULE}_H)"
				REMOVE="\\|#ifndef _*${MODULE}_H| { N; /#define/ {  d } }"
				CHECK='grep -q "#ifndef _*${MODULE}_H" "$FILE"'
				;;
			esac
			${MODE}_outer
			#echo "EXPR='$EXPR'" 1>&2
			if check_${MODE}; then
				exec_cmd sed -i -e "$EXPR" "$FILE"
			fi
		)
	done
}

encapsulate_header "$@"
