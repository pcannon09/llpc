#ifndef INCLUDE_LIB_ASCIIBANNERS_H_
#define INCLUDE_LIB_ASCIIBANNERS_H_

#include <efi/efi.h>

#ifdef __cplusplus
# 	define LLPC_CPP_ASCII_BANNERS_OPEN 		extern "C" {
# 	define LLPC_CPP_ASCII_BANNERS_CLOSE 		}
#else
# 	define LLPC_CPP_ASCII_BANNERS_OPEN
# 	define LLPC_CPP_ASCII_BANNERS_CLOSE
#endif

LLPC_CPP_ASCII_BANNERS_OPEN

static inline const CHAR16 *llpc_ascii_fatalBanner(void)
{
	const CHAR16 *fatalBanner =
		L"                                                      \n"
		L" ███████╗ █████╗ ████████╗ █████╗ ██╗             ██╗ \n"
		L" ██╔════╝██╔══██╗╚══██╔══╝██╔══██╗██║         ██╗██╔╝ \n"
		L" █████╗  ███████║   ██║   ███████║██║         ╚═╝██║  \n"
		L" ██╔══╝  ██╔══██║   ██║   ██╔══██║██║         ██╗██║  \n"
		L" ██║     ██║  ██║   ██║   ██║  ██║███████╗    ╚═╝╚██╗ \n"
		L" ╚═╝     ╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚══════╝        ╚═╝ \n";

	return fatalBanner;
}

LLPC_CPP_ASCII_BANNERS_CLOSE

#endif  // INCLUDE_LIB_ASCIIBANNERS_H_

