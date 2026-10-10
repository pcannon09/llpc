#ifndef INCLUDE_STRING_PARSER_H_
#define INCLUDE_STRING_PARSER_H_

#include <efi/x86_64/efibind.h>

#ifdef __cplusplus
#	define LLPC_STRING_PARSER_OPEN		extern "C" {
#	define LLPC_STRING_PARSER_CLOSE		}
#else
#	define LLPC_STRING_PARSER_OPEN
#	define LLPC_STRING_PARSER_CLOSE
#endif

LLPC_STRING_PARSER_OPEN

#define LLPC_STR_ISWHITESPACE(_ch) \
	((_ch) == '\0' || (_ch) == ' ' || (_ch) == '\n' || (_ch) == '\t')

void llpc_strpar_skipWhitespaceC16(CHAR16 **ch16);
void llpc_strpar_skipWhitespace(char **ch);

char *llpc_strpar_argv2str(unsigned int start, unsigned int argc, char **argv);
char *llpc_strpar_getString(const char *full);

LLPC_STRING_PARSER_CLOSE

#endif  // INCLUDE_STRING_PARSER_H_

