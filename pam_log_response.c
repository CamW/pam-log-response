/*-
 * This module is intended to be used for PAM debugging
 * purposes only and should not be used on a production machine.
 */

/*#include "config.h"*/
#define DEFAULT_USER "nobody"
#include <stdio.h>
#include <syslog.h>
#define PAM_SM_AUTH
#define PAM_SM_ACCOUNT
#define PAM_SM_SESSION
#define PAM_SM_PASSWORD

#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <security/_pam_macros.h>

static char password_prompt[] = "Password:";

int
pam_sm_authenticate(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	pam_syslog(pamh, LOG_NOTICE,
		"in pam_sm_authenticate.");

	struct pam_conv *conv;
	struct pam_message msg;
	const struct pam_message *msgp;
	struct pam_response *resp;

	struct passwd *pwd;
	char *crypt_password, *password;
	int pam_err, retry;

	pam_err = pam_get_item(pamh, PAM_CONV, (const void **)&conv);
	if (pam_err != PAM_SUCCESS)
		return (PAM_SYSTEM_ERR);
	msg.msg_style = PAM_PROMPT_ECHO_OFF;
	msg.msg = password_prompt;
	msgp = &msg;

	for (retry = 0; retry < 3; ++retry) {

		resp = NULL;
		pam_err = (*conv->conv)(1, &msgp, &resp, conv->appdata_ptr);
		if (resp != NULL) {
			pam_syslog(pamh, LOG_NOTICE,
				"conv result is [%i]", pam_err);
			if (pam_err == PAM_SUCCESS) {
				password = resp->resp;
				pam_syslog(pamh, LOG_NOTICE,
					"pointer=[%p] response=[%s]\n", resp, (const char *) password);
			} else {
				free(resp->resp);
			}
			free(resp);

		}

		if (pam_err == PAM_SUCCESS)
			break;
	}
	if (pam_err == PAM_CONV_ERR)
		return (pam_err);
	if (pam_err != PAM_SUCCESS)
		return (PAM_AUTH_ERR);

	free(password);

	return (PAM_SUCCESS);
}


int
pam_sm_setcred(pam_handle_t *pamh, int flags,
           int argc, const char **argv)
{
     return PAM_SUCCESS;
}

int
pam_sm_acct_mgmt(pam_handle_t *pamh, int flags,
         int argc, const char **argv)
{
     return PAM_SUCCESS;
}

int
pam_sm_chauthtok(pam_handle_t *pamh, int flags,
         int argc, const char **argv)
{
     return PAM_SUCCESS;
}

int
pam_sm_open_session(pam_handle_t *pamh, int flags,
            int argc, const char **argv)
{
    return PAM_SUCCESS;
}

int
pam_sm_close_session(pam_handle_t *pamh, int flags,
             int argc, const char **argv)
{
     return PAM_SUCCESS;
}
