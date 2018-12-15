#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/reboot.h>
#include <signal.h>

enum	halt_t
{
	//	busybox specials
	WARM_HALT,
	WARM_POWEROFF,
	WARM_REBOOT,
	//	system interfaces
	COOL_HALT,
	COOL_POWEROFF,
	COOL_REBOOT,
};


int	Sys_shutdown(enum halt_t what)
{
	int	rc;
	switch(what)
	{
	case WARM_HALT:
		rc = kill(1, SIGUSR1);
		break;
	case WARM_POWEROFF:
		rc = kill(1, SIGUSR2);
		break;
	case WARM_REBOOT:
		rc = kill(1, SIGTERM);			
		break;

	case COOL_HALT:
		rc = reboot(RB_HALT_SYSTEM);
		break;
	case COOL_POWEROFF:
		rc = reboot(RB_POWER_OFF);
		break;
	case COOL_REBOOT:
		rc = reboot(RB_AUTOBOOT);
		break;
	default:
		return EINVAL;
	}
	return (rc == -1) ? errno : 0;	
}



int main()
{
    Sys_shutdown(COOL_REBOOT);
    return 0;
}



