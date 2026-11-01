Draft toolchain descriptors (see doc/genmakefile-spec.md section 6 and
doc/genmakefile-toolchains.md for the sources of every line). They are data
for the descriptor interpreter that does not exist yet; compiler.c still
holds the same knowledge as code. Format: INI dialect of lib/ini; a key
repeated appends; "%" in names is the module/program name; $(VAR) is a
variable of the intermediate representation.

Lines marked "; ?" are not verified against a real build yet.
