#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include "../include/common.h"

TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t __maybe_unused paramTypes, TEE_Param __maybe_unused parameters[4], void __maybe_unused **session)
{
	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void __maybe_unused *session)
{
}

static TEE_Result printHello(void)
{	
	IMSG("Hello\n");
	return TEE_SUCCESS;
}

TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sessionId, uint32_t commandId, uint32_t __maybe_unused parametersType, TEE_Param __maybe_unused parameters[4])
{
	switch (commandId)
	{
	case TA_CMD_PRINTHELLO:
		return printHello();
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}