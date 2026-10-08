#include "llpc/lib/globals.h"

#include "llpc/lib/process/process.h"
#include "llpc/lib/alloc/calloc.h"
#include "llpc/lib/string/string.h"

LLPC_GlobalProcessInfo llpc_processData = { 0 }; // Also as `GPI` / `Global Process Info`

#define LLPC_PROC_DEFAULT_LOGLEVEL 		LLPC_LL_Extra

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

		if (llpc_strcmp(sector->id, name))
			return *sector;
	}

	// Create empty values if failed
	LLPC_ProcessSector procSec = {0};

	return procSec;
}

LLPC_ProcessSector llpc_proc_initSector(LLPC_GlobalProcessInfo *gpi, const char *name,
		const LLPC_ProcessSector *sector)
{
	LLPC_ProcessSector *procSec = llpc_calloc(1, sizeof(*procSec));

	if (!procSec)
		return (LLPC_ProcessSector){ .initError = LLPC_PROC_ERR_NULL };

	if (!gpi || !gpi->__initialized)
	{
		procSec->initError = LLPC_PROC_ERR_NULL;
		return *procSec;
	}

	procSec->id = llpc_strdup(name);
	procSec->pid = gpi->lastPID++;
	sector = procSec;

	if (!sector)
	{
		procSec->initError = LLPC_PROC_ERR_NULL;
		return *procSec;
	}

	llpc_extlog(LLPC_PROC_DEFAULT_LOGLEVEL, "Create PID: ", llpcfalse);
	if (LLPC_LOG_LVLCHECK(LLPC_PROC_DEFAULT_LOGLEVEL))
		Print(L"%u\r\n", procSec->pid);

	if (llpc_vector_pushBack(&gpi->sectorList, procSec) != LLPC_VEC_OK)
	{
		procSec->initError = LLPC_PROC_ERR_VectorAction;
		return *procSec;
	}

	procSec->initError = LLPC_PROC_ERR_OK;

	return *procSec;
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

		// If found, do the actions...
		if (sector->pid == pid)
		{
			llpc_extlog(LLPC_PROC_DEFAULT_LOGLEVEL, "Destroyed PID: ", llpcfalse);
			if (LLPC_LOG_LVLCHECK(LLPC_PROC_DEFAULT_LOGLEVEL))
				Print(L"%u\r\n", sector->pid);

			sector->sig = LLPC_PSIG_CLEAN;

			// Find index of the PID to delete
			for (size_t findPID = 0 ; findPID < gpi->sectorList.size ; ++findPID)
			{
				const LLPC_ProcessSector *procsecIt = gpi->sectorList.vec[findPID];

				if (!procsecIt)
					continue;

				if (procsecIt->pid == pid)
				{
					const LLPC_VectorError code =
						llpc_vector_deleteIndex(&gpi->sectorList, findPID - 1, llpctrue);

					if (code != LLPC_VEC_OK)
					{
						llpc_extlog(LLPC_LL_Error, "Could not delete index ", llpcfalse);

						if (LLPC_LOG_LVLCHECK(LLPC_LL_Error))
							Print(L"`%u`; Staged for deletion at program ending\r\n", pid);

						continue;
					}
				}
			}

			found = llpctrue;

			return status;
		}
	}

	if (!found)
		status = LLPC_PROC_PIDNotFound;

	return status;
}

