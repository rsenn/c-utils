const Grammars = require("ebnf").Grammars;
const fs = require("fs");

const ansiC = fs.readFileSync("../ansi-c.ebnf").toString();
const cSource = fs.readFileSync("./cparser.e").toString();

//console.log("ANSI C Grammar: ", ansiC);
//console.log("C Source: ", cSource);
console.log("Grammars: ", Object.keys(Grammars));
console.log("getRules: ", Grammars.W3C.getRules);
console.log("RULES: ", Grammars.W3C.RULES);

String.prototype.substr = function(start, length) {
  return this.substring(start, start + length);
};

let json_grammar = `/* https://www.ietf.org/rfc/rfc4627.txt */
value                ::= false | null | true | object | array | number | string
BEGIN_ARRAY          ::= WS* #x5B WS*  /* [ left square bracket */
BEGIN_OBJECT         ::= WS* #x7B WS*  /* { left curly bracket */
END_ARRAY            ::= WS* #x5D WS*  /* ] right square bracket */
END_OBJECT           ::= WS* #x7D WS*  /* } right curly bracket */
NAME_SEPARATOR       ::= WS* #x3A WS*  /* : colon */
VALUE_SEPARATOR      ::= WS* #x2C WS*  /* , comma */
WS                   ::= [#x20#x09#x0A#x0D]+   /* Space | Tab | \n | \r */
false                ::= "false"
null                 ::= "null"
true                 ::= "true"
object               ::= BEGIN_OBJECT (member (VALUE_SEPARATOR member)*)? END_OBJECT
member               ::= string NAME_SEPARATOR value
array                ::= BEGIN_ARRAY (value (VALUE_SEPARATOR value)*)? END_ARRAY
 
number                ::= "-"? ("0" | [1-9] [0-9]*) ("." [0-9]+)? (("e" | "E") ( "-" | "+" )? ("0" | [1-9] [0-9]*))?
 
/* STRINGS */
 
string                ::= '"' (([#x20-#x21] | [#x23-#x5B] | [#x5D-#xFFFF]) | #x5C (#x22 | #x5C | #x2F | #x62 | #x66 | #x6E | #x72 | #x74 | #x75 HEXDIG HEXDIG HEXDIG HEXDIG))* '"'
HEXDIG                ::= [a-fA-F0-9]`;

let grammar = `translation_unit ::= (external_declaration)*
external_declaration ::= function_definition | declaration
function_definition ::= (declaration_specifier)* declarator (declaration)* compound_statement
declaration_specifier ::= storage_class_specifier | type_specifier | type_qualifier
storage_class_specifier ::= auto | register | static | extern | typedef
type_specifier ::= void | char | short | int | long | float | double | signed | unsigned | struct_or_union_specifier | enum_specifier | typedef_name
struct_or_union_specifier ::= struct_or_union identifier '{' (struct_declaration)+ '}' | struct_or_union '{' (struct_declaration)+ '}' | struct_or_union identifier
struct_or_union ::= struct | union
struct_declaration ::= (specifier_qualifier)* struct_declarator_list
specifier_qualifier ::= type_specifier | type_qualifier
struct_declarator_list ::= struct_declarator | struct_declarator_list ',' struct_declarator
struct_declarator ::= declarator | declarator ':' constant_expression | ':' constant_expression
declarator ::= (pointer)? direct_declarator
pointer ::= '*' (type_qualifier)* (pointer)?
type_qualifier ::= const | volatile
direct_declarator ::= identifier | '(' declarator ')' | direct_declarator '[' (constant_expression)? ']' | direct_declarator '(' parameter_type_list ')' | direct_declarator '(' (identifier)* ')'
constant_expression ::= conditional_expression
conditional_expression ::= logical_or_expression | logical_or_expression '?' expression ':' conditional_expression
logical_or_expression ::= logical_and_expression | logical_or_expression '||' logical_and_expression
logical_and_expression ::= inclusive_or_expression | logical_and_expression '&&' inclusive_or_expression
inclusive_or_expression ::= exclusive_or_expression | inclusive_or_expression '|' exclusive_or_expression
exclusive_or_expression ::= and_expression | exclusive_or_expression '^' and_expression
and_expression ::= equality_expression | and_expression '&' equality_expression
equality_expression ::= relational_expression | equality_expression '==' relational_expression | equality_expression '!=' relational_expression
relational_expression ::= shift_expression | relational_expression '<' shift_expression | relational_expression '>' shift_expression | relational_expression '<=' shift_expression | relational_expression '>=' shift_expression
shift_expression ::= additive_expression | shift_expression '<<' additive_expression | shift_expression '>>' additive_expression
additive_expression ::= multiplicative_expression | additive_expression '+' multiplicative_expression | additive_expression '-' multiplicative_expression
multiplicative_expression ::= cast_expression | multiplicative_expression '*' cast_expression | multiplicative_expression '/' cast_expression | multiplicative_expression '%' cast_expression
cast_expression ::= unary_expression | '(' type_name ')' cast_expression
unary_expression ::= postfix_expression | '++' unary_expression | '--' unary_expression | unary_operator cast_expression | sizeof unary_expression | sizeof type_name
postfix_expression ::= primary_expression | postfix_expression '[' expression ']' | postfix_expression '(' (assignment_expression)* ')' | postfix_expression '.' identifier | postfix_expression '->' identifier | postfix_expression '++' | postfix_expression '--'
primary_expression ::= identifier | constant | string | '(' expression ')'
constant ::= integer-constant | character-constant | floating-constant | enumeration-constant
expression ::= assignment_expression | expression ',' assignment_expression
assignment_expression ::= conditional_expression | unary_expression assignment_operator assignment_expression
assignment_operator ::= '=' | '*=' | '/=' | '%=' | '+=' | '-=' | '<<=' | '>>=' | '&=' | '^=' | '|='
unary_operator ::= '&' | '*' | '+' | '-' | '~' | '!'
type_name ::= (specifier_qualifier)+ (abstract_declarator)?
parameter_type_list ::= parameter_list | parameter_list ',' '...'
parameter_list ::= parameter_declaration | parameter_list ',' parameter_declaration
parameter_declaration ::= (declaration_specifier)+ declarator | (declaration_specifier)+ abstract_declarator | (declaration_specifier)+
abstract_declarator ::= pointer | pointer direct_abstract_declarator | direct_abstract_declarator
direct_abstract_declarator ::=  '(' abstract_declarator ')' | (direct_abstract_declarator)? '[' (constant_expression)? ']' | (direct_abstract_declarator)? '(' (parameter_type_list)? ')'
enum_specifier ::= enum identifier '{' enumerator_list '}' | enum '{' enumerator_list '}' | enum identifier
enumerator_list ::= enumerator | enumerator_list ',' enumerator
enumerator ::= identifier | identifier '=' constant_expression
typedef_name ::= identifier
declaration ::=  (declaration_specifier)+ (init_declarator)*
init_declarator ::= declarator | declarator '=' initializer
initializer ::= assignment_expression | '{' initializer_list '}' | '{' initializer_list ',' '}'
initializer_list ::= initializer | initializer_list ',' initializer
compound_statement ::= '{' (declaration)* (statement)* '}'
statement ::= labeled_statement | expression_statement | compound_statement | selection_statement | iteration_statement | jump_statement
labeled_statement ::= identifier ':' statement | case constant_expression ':' statement | default ':' statement
expression_statement ::= (expression)? ';'
selection_statement ::= if '(' expression ')' statement | if '(' expression ')' statement else statement | switch '(' expression ')' statement
iteration_statement ::= while '(' expression ')' statement | do statement while '(' expression ')' ';' | for '(' (expression)? ';' (expression)? ';' (expression)? ')' statement
jump_statement ::= goto identifier ';' | continue ';' | break ';' | return (expression)? ';'`;

//grammar = grammar.replace(/'/g, '"');
grammar = json_grammar.split(/\n/g).join("\n");

try {
  console.log("Grammar: ", grammar);
  let rules = Grammars.W3C.getRules(grammar);
  console.log("rules: ", rules);

  let parser = new Grammars.W3C.Parser(grammar);
  console.log("Parser: ", parser); /*let ast = parser.getAST(grammar);

  //let ast = parser.getAST( '{"a":false,"b":"asd\\n      asd ","list":[1,2,3,true]}');

  /*

console.log("parser: ", parser);

*/
  console.log("ast: ", ast);
} catch(error) {
  console.error("ERROR: ", error);
}
