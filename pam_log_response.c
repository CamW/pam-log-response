/*
* Copyright (c) 2016 Cameron Waldron
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, and the entire permission notice in its entirety,
*    including the disclaimer of warranties.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. The name of the author may not be used to endorse or promote
*    products derived from this software without specific prior
*    written permission.
*
* ALTERNATIVELY, this product may be distributed under the terms of
* the GNU Public License, in which case the provisions of the GPL are
* required INSTEAD OF the above restrictions.  (This clause is
* necessary due to a potential bad interaction between the GPL and
* the restrictions contained in a BSD-style copyright.)
*
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
* WARNING - This software will expose sensitive information when
* installed on a system and should only ever be used on systems
* and under circumstances where this is acceptable.
*/

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
			if (pam_err == PAM_SUCCESS) {
				password = resp->resp;
				pam_syslog(pamh, LOG_NOTICE,
					"conv_result=[SUCCESS] pointer=[%p] response=[%s]\n", resp, (const char *) password);
			} else {
				pam_syslog(pamh, LOG_NOTICE,
	                                "conv_result=[%i]\n", pam_err);
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
     return PAM_IGNORE;
}

int
pam_sm_acct_mgmt(pam_handle_t *pamh, int flags,
         int argc, const char **argv)
{
     return PAM_IGNORE;
}

int
pam_sm_chauthtok(pam_handle_t *pamh, int flags,
         int argc, const char **argv)
{
     return PAM_IGNORE;
}

int
pam_sm_open_session(pam_handle_t *pamh, int flags,
            int argc, const char **argv)
{
    return PAM_IGNORE;
}

int
pam_sm_close_session(pam_handle_t *pamh, int flags,
             int argc, const char **argv)
{
     return PAM_IGNORE;
}
