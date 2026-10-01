#include "llpc/lib/process/process.h"
#include "llpc/lib/types.h"

LLPC_GlobalProcessInfo llpc_processData = { 0 };

LLPC_GlobalProcessInfo llpc_proc_init(void)
{
	LLPC_GlobalProcessInfo gpi = {
		.sectorList = llpc_vector_init(8, llpctrue)
	};

	gpi.lastError = LLPC_PROC_ERR_OK;
	gpi.__initialized = llpctrue;

	return gpi;
}

LLPC_ProcessError llpc_proc_destroy(LLPC_GlobalProcessInfo *gpi)
{
	if (!gpi || !gpi->__initialized)
		return LLPC_PROC_ERR_NULL;

	for (size_t i = 0 ; i < gpi->sectorList.size ; ++i)
	{
		const LLPC_ProcessSector *sector = gpi->sectorList.vec[i];

		const LLPC_ProcessError perr = llpc_proc_destroySector(gpi, sector->pid);

		if (perr != LLPC_PROC_ERR_OK)
			return perr;
	}

	return LLPC_PROC_ERR_OK;
}

LLPC_ProcessSector llpc_proc_initSector(LLPC_GlobalProcessInfo *gpi, void *data)
{
	LLPC_ProcessSector procSec = {0};

	if (llpc_vector_pushBack(&gpi->sectorList, data))
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

	llpc_bool found = llpcfalse;

	for (size_t i = 0 ; i < gpi->sectorList.size ; ++i)
	{
		LLPC_ProcessSector *sector = gpi->sectorList.vec[i];

		if (sector->pid == pid)
			sector->sig = LLPC_PSIG_KILL;
	}

	return found
		? LLPC_PROC_ERR_OK
		: LLPC_PROC_PIDNotFound;
}

