#include "server.h"

/*
** Exit the server.
*/

void		exit_server(void)
{
	LOGI("Server instance (PID %ju) down.\n", (uintmax_t)getpid());
	exit(EXIT_FAILURE);
}

/*
** Stop the server when signal SIGINT is send.
*/

static void		sigstop(int sig)
{
	(void)sig;
	LOGD("%s: received %s signal. Stopping server...\n",
			__func__, strsignal(sig));
	exit_server();
}

/*
** Check for terminated processes and collect their exit status.
*/

static void		sigchld(int sig)
{
	int	status;
	pid_t   pid;

	(void)sig;
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
			LOGW("%s: abnormal termination of child PID %ju\n",
					__func__, (uintmax_t)pid);
}


			__attribute__((__noreturn__))
static void *		sighandler_thread(void* sig_set)
{
	LOGD("%s: spawned signal handling thread (tid=%p).\n",
			__func__, (void*)pthread_self());
	for(;;) {
		int sig;
		if (sigwait((sigset_t*)sig_set, &sig) == -1)
			PLOGF("%s: sigwait()", __func__);
		switch (sig) {
			case SIGINT:
			case SIGSTOP:
			case SIGTERM:
			case SIGQUIT:
				sigstop(sig);
				break;
			case SIGCHLD:
				sigchld(sig);
				break;
			default:
				LOGW("%s: PID %u: recieved unhandled signal %s.\n",
						__func__, getpid(), strsignal(sig));
				break;
		}
	}
}

/**
 *  called from main thread
 */
void sighandler_init() {
	sigset_t *sset = malloc(sizeof(sigset_t));
	pthread_t sigthrd;

	// Add desired signals
	sigemptyset(sset);
	sigaddset(sset, SIGCHLD);
	sigaddset(sset, SIGINT);
	sigaddset(sset, SIGTERM);
	sigaddset(sset, SIGSTOP);
	sigaddset(sset, SIGQUIT);

	// Block signals in main thread
	if (pthread_sigmask(SIG_BLOCK, sset, NULL) == -1)
		PLOGF("%s: sigprocmask", __func__);

	if (pthread_create(&sigthrd, NULL, sighandler_thread, sset) == -1)
		PLOGF("%s: pthread_create", __func__);
}

