#include "llpc/shell/modules/argpar.h"
#include "llpc/lib/alloc/calloc.h"
#include "llpc/lib/alloc/free.h"
#include "llpc/lib/string/string.h"
#include "llpc/lib/types.h"

#define LLPC_ARGPAR_DEFAULT_ALLOC 		4

LLPC_GlobalArgPar llpc_argpar_init(const char *title, const char *about)
{
	LLPC_GlobalArgPar gap = {0};

	gap.title = title;
	gap.about = about;
	gap.args = llpc_vector_init(LLPC_ARGPAR_DEFAULT_ALLOC, llpctrue);

	if (!gap.args.__initialized)
		return gap;

	gap.__initialized = llpctrue;

	return gap;
}

void llpc_argpar_destroySector(LLPC_ArgPar *ap)
{
	// Delete all its vector children
	for (size_t i = 0 ; i < ap->subparams.size ; ++i)
	{
		LLPC_ArgPar *delAP = ap->subparams.vec[i];

		if (delAP->subparams.size > 0)
			llpc_argpar_destroySector(delAP);

		else llpc_vector_destroy(&ap->subparams);
	}
}

void llpc_argpar_destroy(LLPC_GlobalArgPar *gap)
{
	if (!gap->__initialized)
		return;

	// Go one by one to delete the top-level of the vector
	for (size_t i = 0 ; i < gap->args.size ; ++i)
	{
		LLPC_ArgPar *delAP = gap->args.vec[i];

		llpc_argpar_destroySector(delAP);
	}

	llpc_vector_destroy(&gap->args);
}

LLPC_ArgParRegisterStat llpc_argpar_register(LLPC_GlobalArgPar *gap, LLPC_ArgPar *ap)
{
	if (!gap || !gap->__initialized || !ap)
		return LLPC_ARGPARSTAT_NullFail;

	// Register
	if (llpc_vector_pushBack(&gap->args, ap) != LLPC_VEC_OK)
		return LLPC_ARGPARSTAT_RegisterVecFail;

	return LLPC_ARGPARSTAT_OK;
}

LLPC_ArgPar llpc_argpar_sectorInit(LLPC_GlobalArgPar *gap, const char *id, const char *help,
		const llpc_bool shortCommands, const llpc_bool automatic)
{
	LLPC_ArgPar argpar = {0};

	if (!gap->__initialized)
		return argpar;

	argpar.id = llpc_strdup(id);
	argpar.subparams = llpc_vector_init(LLPC_ARGPAR_DEFAULT_ALLOC, llpctrue);

	if (automatic)
	{
		argpar.param = "--";
		llpc_strAddIdx(&argpar.param, id, llpc_strlen(argpar.param));

		if (shortCommands)
		{
			argpar.sparam = "-";
			const char idCh[2] = { id[0], '\0' };
			llpc_strAddIdx(&argpar.sparam, idCh, llpc_strlen(argpar.sparam));
		}

		argpar.required = llpcfalse;
		argpar.help = llpc_strdup(help);
	}

	argpar.__initialized = llpctrue;

	return argpar;
}

llpc_bool llpc_argpar_getItem(LLPC_ArgPar *ap,
		unsigned int argc, char **argv, const char *id)
{
	if (!ap || !ap->__initialized || !argv || !id || argc == 0)
		return llpcfalse;

	for (unsigned int i = 1 ; i < argc ; ++i)
	{
		if (llpc_strcmp(argv[i], id))
			return llpctrue;
	}

	return llpcfalse;
}

llpc_bool llpc_argpar_get(LLPC_GlobalArgPar *gap, const char *fullID)
{
	if (!gap || !gap->__initialized || !fullID)
		return llpcfalse;

	char **ids = NULL;
	const unsigned int idCount = llpc_split(fullID, '.', &ids);

	if (!ids || idCount == 0)
		return llpcfalse;

	LLPC_Vector *params = &gap->args;
	LLPC_ArgPar *current = NULL;
	llpc_bool found = llpcfalse;

	for (unsigned int i = 0; i < idCount; ++i)
	{
		current = NULL;

		for (size_t j = 0; j < params->size; ++j)
		{
			LLPC_ArgPar *item = params->vec[j];

			if (!item || !item->__initialized || !item->id)
				continue;

			if (llpc_strcmp(item->id, ids[i]))
			{
				current = item;
				break;
			}
		}

		if (!current)
			goto cleanup;  // Execution exits here.

		params = &current->subparams;
	}

	if (current)
	{
		for (unsigned int i = 0 ; i < gap->argc ; ++i)
		{
			if ((current->param &&
						llpc_strcmp(gap->argv[i], current->param)) ||
					(current->sparam &&
					 llpc_strcmp(gap->argv[i], current->sparam)))
			{
				found = llpctrue;
				break;
			}
		}
	}

cleanup:
	for (unsigned int i = 0 ; i < idCount ; ++i)
		LLPC_FREE(ids[i]);

	LLPC_FREE(ids);

	return found;
}

void llpc_argpar_sectorDestroy(LLPC_ArgPar *ap)
{
	ap->__initialized = llpcfalse;
}

size_t llpc_argpar_helpLength(const LLPC_ArgPar *ap)
{
	if (!ap || !ap->__initialized)
		return 0;

	size_t length = 0;

	if (ap->param)
		length += llpc_strlen(ap->param);

	if (ap->sparam)
		length += llpc_strlen(ap->sparam) + 3;

	if (ap->help)
		length += llpc_strlen(ap->help) + 3;

	length += 1;

	for (size_t i = 0 ; i < ap->subparams.size ; ++i)
	{
		LLPC_ArgPar *child = ap->subparams.vec[i];

		if (!child)
			continue;

		length += llpc_argpar_helpLength(child);
	}

	return length;
}

size_t llpc_argpar_helpWrite(char *buffer,
		size_t offset,
		const LLPC_ArgPar *ap,
		unsigned int depth)
{
	if (!ap || !ap->__initialized)
		return offset;

	for (unsigned int i = 0 ; i < depth ; ++i)
	{
		buffer[offset++] = ' ';
		buffer[offset++] = ' ';
	}

	if (ap->param)
	{
		size_t length = llpc_strlen(ap->param);

		for (size_t i = 0 ; i < length ; ++i)
			buffer[offset++] = ap->param[i];
	}

	if (ap->sparam)
	{
		buffer[offset++] = ' ';
		buffer[offset++] = '(';

		size_t length = llpc_strlen(ap->sparam);

		for (size_t i = 0 ; i < length ; ++i)
			buffer[offset++] = ap->sparam[i];

		buffer[offset++] = ')';
	}

	if (ap->help)
	{
		buffer[offset++] = ' ';
		buffer[offset++] = '-';
		buffer[offset++] = ' ';

		size_t length = llpc_strlen(ap->help);

		for (size_t i = 0 ; i < length ; ++i)
			buffer[offset++] = ap->help[i];
	}

	buffer[offset++] = '\n';

	for (size_t i = 0 ; i < ap->subparams.size ; ++i)
	{
		LLPC_ArgPar *child = ap->subparams.vec[i];

		if (!child)
			continue;

		offset = llpc_argpar_helpWrite(
			buffer,
			offset,
			child,
			depth + 1);
	}

	return offset;
}

LLPC_ArgPar *llpc_argpar_find(
		LLPC_GlobalArgPar *gap,
		const char *fullID)
{
	if (!gap || !gap->__initialized || !fullID)
		return NULL;

	char **ids = NULL;
	const unsigned int idCount =
		llpc_split(fullID, '.', &ids);

	if (!ids || idCount == 0)
		return NULL;

	LLPC_Vector *params = &gap->args;
	LLPC_ArgPar *current = NULL;

	for (unsigned int i = 0 ; i < idCount ; ++i)
	{
		current = NULL;

		for (size_t j = 0 ; j < params->size ; ++j)
		{
			LLPC_ArgPar *item = params->vec[j];

			if (!item || !item->__initialized || !item->id)
				continue;

			if (llpc_strcmp(item->id, ids[i]))
			{
				current = item;
				break;
			}
		}

		if (!current)
			break;

		params = &current->subparams;
	}

	for (unsigned int i = 0 ; i < idCount ; ++i)
		LLPC_FREE(ids[i]);

	LLPC_FREE(ids);

	return current;
}

char *llpc_argpar_help(
		LLPC_GlobalArgPar *gap,
		const char *fullID)
{
	if (!gap || !gap->__initialized)
		return NULL;

	LLPC_ArgPar *target = NULL;

	if (fullID)
	{
		target = llpc_argpar_find(gap, fullID);

		if (!target)
			return NULL;
	}

	size_t length = 0;

	if (!target)
	{
		if (gap->title)
			length += llpc_strlen(gap->title) + 1;

		if (gap->about)
			length += llpc_strlen(gap->about) + 1;

		for (size_t i = 0; i < gap->args.size; ++i)
		{
			LLPC_ArgPar *ap = gap->args.vec[i];

			if (!ap)
				continue;

			length += llpc_argpar_helpLength(ap);
		}
	}
	else
	{
		length = llpc_argpar_helpLength(target);
	}

	char *help = llpc_calloc(length + 1, sizeof(*help));

	if (!help)
		return NULL;

	size_t offset = 0;

	if (!target)
	{
		if (gap->title)
		{
			size_t len = llpc_strlen(gap->title);

			for (size_t i = 0; i < len; ++i)
				help[offset++] = gap->title[i];

			help[offset++] = '\n';
		}

		if (gap->about)
		{
			size_t len = llpc_strlen(gap->about);

			for (size_t i = 0; i < len; ++i)
				help[offset++] = gap->about[i];

			help[offset++] = '\n';
		}

		for (size_t i = 0; i < gap->args.size; ++i)
		{
			LLPC_ArgPar *ap = gap->args.vec[i];

			if (!ap)
				continue;

			offset = llpc_argpar_helpWrite(
				help,
				offset,
				ap,
				0);
		}
	}
	else
	{
		offset = llpc_argpar_helpWrite(
			help,
			0,
			target,
			0);
	}

	help[offset] = '\0';

	return help;
}

