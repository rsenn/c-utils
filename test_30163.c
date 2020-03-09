#include <sys/types.h>\n#include <sys/socket.h>\n#include <netinet/in.h>\n\nint main() {\n	struct sockaddr_in6 sa;\n	sa.sin6_family = PF_INET6;\n	sa.sin6_scope_id = 23;\n	(void)sa;\n	return 0;\n}\n
