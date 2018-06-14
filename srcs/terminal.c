/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 19:20:41 by kdumarai          #+#    #+#             */
/*   Updated: 2018/06/14 05:37:49 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "ft_select.h"

/*
** signal_hdl -- handle signals (except SIGCONT and SIGWINCH)
**
** int			signal code
*/

void		signal_hdl(int sigc)
{
	static char		vsusp_char = 0;
	const char		sim_sigtstp[2] = {vsusp_char, '\0'};

	if (vsusp_char == 0)
	{
		vsusp_char = (char)sigc;
		return ;
	}
	init_restore_display(NULL, NO);
	init_restore_terminal(NO, NULL);
	if (sigc == SIGTSTP)
	{
		signal(sigc, SIG_DFL);
		ioctl(STDIN_FILENO, TIOCSTI, sim_sigtstp);
		return ;
	}
	exit(EXIT_SUCCESS);
}

/*
** set_signals (private)
*/

static int	set_signals(void)
{
	const int		killers[13] = {SIGHUP, SIGINT, SIGQUIT, SIGPIPE, SIGALRM,
								SIGTERM, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF,
								SIGUSR1, SIGUSR2, 0};
	const int		toblock[5] = {SIGTSTP, SIGCONT, SIGTTIN, SIGTTOU, 0};
	unsigned int	idx;
	int				success;

	idx = 0;
	success = 1;
	while (killers[idx])
	{
		if (signal(killers[idx], &signal_hdl) == SIG_ERR)
			success = 0;
		idx++;
	}
	idx = 0;
	while (toblock[idx])
	{
		if (signal(toblock[idx], SIG_IGN) == SIG_ERR)
			success = 0;
		idx++;
	}
	return (success);
}

/*
** set_read_timeout
**
** cc_t				timeout
** struct termios*	existing dump of termios struct
**						(will NOT apply changes if provided)
*/

int			set_read_timeout(cc_t timeout, struct termios *tptr)
{
	static int				noget = FALSE;
	static struct termios	t;
	struct termios			*dest;
	cc_t					new_vmin;

	if (!tptr)
	{
		if (!noget && tcgetattr(FT_OUT_FD, &t) == -1)
			return (FALSE);
		noget = TRUE;
		dest = &t;
	}
	else
		dest = tptr;
	new_vmin = (timeout == 0) ? 1 : 0;
	if (dest->c_cc[VTIME] == timeout && dest->c_cc[VMIN] == new_vmin)
		return (TRUE);
	dest->c_cc[VMIN] = new_vmin;
	dest->c_cc[VTIME] = timeout;
	if (!tptr && tcsetattr(FT_OUT_FD, TCSADRAIN, dest) == -1)
		return (FALSE);
	return (TRUE);
}

/*
** init_restore_terminal
**
** int				init or restore terminal
** char*			ptr to store vsusp char
*/

int			init_restore_terminal(int init, char *vsusp_ptr)
{
	static int				is_init = FALSE;
	static struct termios	saved_t;
	struct termios			t;
	int						sigret;

	if (!init)
	{
		if (!is_init)
			return (FALSE);
		is_init = FALSE;
		return (tcsetattr(FT_OUT_FD, TCSADRAIN, &saved_t) == -1 ? FALSE : TRUE);
	}
	if (is_init || tcgetattr(FT_OUT_FD, &saved_t) == -1)
		return (FALSE);
	if (!(sigret = set_signals()))
		ft_putendl_fd("Some signal couldn't be caught.", STDERR_FILENO);
	t = saved_t;
	t.c_lflag &= ~(ICANON | ECHO | ((!sigret || !isatty(1)) ? ISIG : 0));
	t.c_oflag &= (unsigned long)~OPOST;
	set_read_timeout(0, &t);
	if (tcsetattr(FT_OUT_FD, TCSADRAIN, &t) == -1)
		return (FALSE);
	(vsusp_ptr) ? *vsusp_ptr = (char)saved_t.c_cc[VSUSP] : 0;
	ospeed = (short)saved_t.c_ospeed;
	return ((is_init = TRUE));
}
