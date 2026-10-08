#ifndef INCLUDE_MODULES_ARGPAR_H_
#define INCLUDE_MODULES_ARGPAR_H_

#include "llpc/lib/types.h"

#include "llpc/lib/vector/vector.h"

#ifdef __cplusplus
#	define LLPC_SH_ARGPAR_OPEN			extern "C" {
#	define LLPC_SH_ARGPAR_CLOSE			}
#else
#	define LLPC_SH_ARGPAR_OPEN
#	define LLPC_SH_ARGPAR_CLOSE
#endif

LLPC_SH_ARGPAR_OPEN

typedef struct LLPC_GlobalArgPar
{
	const char *title;
	const char *about;

	char **argv;

	unsigned int argc;

	LLPC_Vector args; // LLPC_ArgPar

	llpc_bool __initialized;
} LLPC_GlobalArgPar;

typedef struct LLPC_ArgPar
{
	LLPC_Vector subparams; // LLPC_ArgPar

	char *id;
	char *param;
	char *sparam;
	char *help;

	llpc_bool required;
	llpc_bool __initialized;
} LLPC_ArgPar;

typedef enum LLPC_ArgParRegisterStat
{
	LLPC_ARGPARSTAT_OK,
	LLPC_ARGPARSTAT_NullFail,
	LLPC_ARGPARSTAT_RegisterVecFail,
} LLPC_ArgParRegisterStat;

LLPC_GlobalArgPar llpc_argpar_init(const char *title, const char *about);
void llpc_argpar_destroy(LLPC_GlobalArgPar *gap);
void llpc_argpar_destroySector(LLPC_ArgPar *gap);

llpc_bool llpc_argpar_get(LLPC_GlobalArgPar *gap, const char *fullID);

LLPC_ArgParRegisterStat llpc_argpar_register(LLPC_GlobalArgPar *gap, LLPC_ArgPar *ap);
LLPC_ArgPar llpc_argpar_sectorInit(LLPC_GlobalArgPar *gap, const char *id, const char *help,
		const llpc_bool shortCommands, const llpc_bool automatic);

llpc_bool llpc_argpar_getItem(LLPC_ArgPar *ap,
		unsigned int argc, char **argv, const char *id);

LLPC_ArgPar *llpc_argpar_find(
		LLPC_GlobalArgPar *gap,
		const char *fullID);
size_t llpc_argpar_helpWrite(char *buffer,
		size_t offset,
		const LLPC_ArgPar *ap,
		unsigned int depth);
size_t llpc_argpar_helpLength(const LLPC_ArgPar *ap);
char *llpc_argpar_help(LLPC_GlobalArgPar *gap, const char *fullID);

void llpc_argpar_sectorDestroy(LLPC_ArgPar *ap);

LLPC_SH_ARGPAR_CLOSE

#endif  // INCLUDE_MODULES_ARGPAR_H_

