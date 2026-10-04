#ifndef INCLUDE_STRING_PARSER_H_
#define INCLUDE_STRING_PARSER_H_

#ifdef __cplusplus
#	define LLPC_STRING_PARSER_OPEN		extern "C" {
#	define LLPC_STRING_PARSER_CLOSE		}
#else
#	define LLPC_STRING_PARSER_OPEN
#	define LLPC_STRING_PARSER_CLOSE
#endif

LLPC_STRING_PARSER_OPEN

char *llpc_strpar_argv2str(unsigned int start, unsigned int argc, char **argv);
char *llpc_strpar_getString(const char *full);

LLPC_STRING_PARSER_CLOSE

#endif  // INCLUDE_STRING_PARSER_H_

