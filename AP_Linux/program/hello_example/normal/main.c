#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tee_client_api.h>
#include "../include/common.h"

int main()
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session session;
	TEEC_Operation op = {0, };
	TEEC_UUID uuid = TA_EXAMPLE_UUID;
	uint32_t errOrigin;

	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
	{
		printf("ERROR: result %x\n", res);
		return 0;
	}

	res = TEEC_OpenSession(&ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &errOrigin);
	if (res != TEEC_SUCCESS) 
	{
		printf("ERROR: result %x\n", res);
		return 0;
	}

	res = TEEC_InvokeCommand(&session, TA_CMD_PRINTHELLO, &op, &errOrigin);
	if (res != TEEC_SUCCESS)
	{
		printf("ERROR: result %x\n", res);
		return 0;
	}	

	TEEC_CloseSession(&session);
	TEEC_FinalizeContext(&ctx);

	return 0;
}
