# pam-log-response
Diagnostic PAM module which logs responses to the PAM conversation messages.

##WARNING
This module is intended for debugging and should never be used on a system which you care about. It logs PAM conversation responses (passwords) amoung other things to the system log files.

##Compatibility
This module was built on and has only ever been used on Ubuntu(16.04) but could easily be modified to run on other unix based systems which support PAM. All instructions below assume Ubuntu 16.04.

##Usage
To build and install the module, run the following:
```
make
sudo make install
```
To uninstall the module:
```
sudo make uninstall
```

Once installed, install pamtester `sudo apt-get install pamtester` and the run `pamtester newsvc <yourUsername> authenticate`.

Finally, check the logfile at `/var/log/auth.log` and notice that a line as below was written to the log.
```
Aug 25 14:31:16 <MACHINE_NAME> pamtester: pam_log_response(newsvc:auth): conv_result=[SUCCESS] pointer=[0xcc42b0] response=[<PASSWORD>]
```

