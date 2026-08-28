#include "header.h"


int	g_signal_status = 0;


static void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		g_signal_status = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}


void	setup_signals(void)
{
	g_signal_status = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}


void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}