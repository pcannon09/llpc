#include "llpc/lib/process/process.h"
#include "llpc/lib/string/string.h"
#include "llpc/lib/types.h"
#include "llpc/lib/logging/logger.h"
#include "llpc/predefines.h"

LLPC_GlobalProcessInfo llpc_processData = { 0 }; // Also as `GPI` / `Global Process Info`

LLPC_GlobalProcessInfo llpc_proc_init(void)
{
	LLPC_GlobalProcessInfo gpi = {
		.sectorList = llpc_vector_init(8, llpctrue)
	};

	gpi.startPID = 2;
	gpi.lastPID = gpi.startPID;
	gpi.lastError = LLPC_PROC_ERR_OK;
	gpi.__initialized = llpctrue;

	return gpi;
}

LLPC_ProcessError llpc_proc_destroy(LLPC_GlobalProcessInfo *gpi)
{
	if (!gpi || !gpi->__initialized)
		return LLPC_PROC_ERR_NULL;

	LLPC_ProcessError perr = LLPC_PROC_ERR_OK;

	for (size_t i = 0 ; i < gpi->sectorList.size ; ++i)
	{
		const LLPC_ProcessSector *sector = gpi->sectorList.vec[i];
		const LLPC_ProcessError tmpPerr = llpc_proc_destroySector(gpi, sector->pid);

		llpc_extlog(LLPC_LL_Debug, "Destroyed PID: ", llpcfalse);

		if (LLPC_LOG_LVLCHECK(LLPC_LL_Debug))
			Print(L"%u\r\n", sector->pid);

		if (tmpPerr != LLPC_PROC_ERR_OK)
			perr = tmpPerr;
	}

	return perr;
}

LLPC_ProcessSector llpc_proc_getSectorByPID(LLPC_GlobalProcessInfo *gpi, const LLPC_PID pid)
{
	for (size_t i = 0 ; i < gpi->sectorList.size ; ++i)
	{
		const LLPC_ProcessSector *sector = gpi->sectorList.vec[i];

		if (sector->pid == pid)
			return *sector;
	}

	// Create empty values if failed
	LLPC_ProcessSector procSec = {0};

	return procSec;
}

LLPC_ProcessSector llpc_proc_getSectorByName(LLPC_GlobalProcessInfo *gpi, const char *name)
{
	for (size_t i = 0 ; i < gpi->sectorList.size ; ++i)
	{
		const LLPC_ProcessSector *sector = gpi->sectorList.vec[i];

		if (llpc_strcmp(sector->name, name))
			return *sector;
	}

	// Create empty values if failed
	LLPC_ProcessSector procSec = {0};

	return procSec;
}

LLPC_ProcessSector llpc_proc_initSector(LLPC_GlobalProcessInfo *gpi, const char *name, void *data)
{
	LLPC_ProcessSector procSec = {0};

	if (!gpi || !gpi->__initialized)
	{
		procSec.initError = LLPC_PROC_ERR_NULL;
		return procSec;
	}

	procSec.data = data;
	procSec.name = llpc_strdup(name);
	procSec.pid = gpi->lastPID++;

	llpc_extlog(LLPC_LL_Debug, "PID: ", llpcfalse);
	if (LLPC_LOG_LVLCHECK(LLPC_LL_Debug))
		Print(L"%u\r\n", procSec.pid);

	if (llpc_vector_pushBack(&gpi->sectorList, &procSec))
	{
		procSec.initError = LLPC_PROC_ERR_VectorAction;
		return procSec;
	}

	procSec.initError = LLPC_PROC_ERR_OK;

	return procSec;
}

LLPC_ProcessError llpc_proc_destroySector(LLPC_GlobalProcessInfo *gpi, const LLPC_PID pid)
{
	if (!gpi || !gpi->__initialized)
		return LLPC_PROC_ERR_NULL;

	LLPC_ProcessError status = LLPC_PROC_ERR_OK;
	llpc_bool found = llpcfalse;

	for (size_t i = 0 ; i < gpi->sectorList.size ; ++i)
	{
		LLPC_ProcessSector *sector = gpi->sectorList.vec[i];

		if (!sector)
			return LLPC_PROC_InvalidData;

		if (sector->pid == pid)
		{
			sector->sig = LLPC_PSIG_KILL;
			found = llpctrue;

			return status;
		}
	}

	if (!found)
		status = LLPC_PROC_PIDNotFound;

	return status;
}

