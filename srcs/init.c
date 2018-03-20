/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 19:20:41 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/20 00:57:04 by kdumarai         ###   ########.fr       */
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
	static char	vsusp_char = 0;
	const char	sim_sigtstp[2] = {vsusp_char, '\0'};

	if (vsusp_char == 0)
	{
		vsusp_char = sigc;
		return ;
	}
	init_restore_terminal(NO);
	if (sigc == SIGTSTP)
	{
		signal(sigc, SIG_DFL);
		ioctl(STDIN_FILENO, TIOCSTI, sim_sigtstp);
		return ;
	}
	exit(EXIT_SUCCESS);
}

/*
** set_signals
*/

int			set_signals(void)
{
	const int		sigs[] = {SIGHUP, SIGINT, SIGQUIT, SIGTERM,
							SIGTSTP, SIGCONT};
	static void		(*hdls[])(int) = {&signal_hdl, &signal_hdl, &signal_hdl,
								&signal_hdl, SIG_IGN, SIG_IGN, NULL};
	unsigned int	idx;

	idx = 0;
	while (hdls[idx])
	{
		if (signal(sigs[idx], hdls[idx]) == SIG_ERR)
			return (FALSE);
		idx++;
	}
	return (TRUE);
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
	struct termios	t;
	struct termios	*dest;

	if (!tptr)
	{
		if (tcgetattr(FT_OUT_FD, &t) == -1)
			return (FALSE);
		dest = &t;
	}
	else
		dest = tptr;
	dest->c_cc[VMIN] = (timeout == 0) ? 1 : 0;
	dest->c_cc[VTIME] = timeout;
	if (!tptr && tcsetattr(FT_OUT_FD, TCSADRAIN, dest) == -1)
		return (FALSE);
	return (TRUE);
}

/*
** init_restore_terminal
**
** int				init or restore terminal
*/

int			init_restore_terminal(int init)
{
	static struct termios	saved_t;
	struct termios			t;

	if (!init)
	{
		outcap("ve");
		outcap("ke");
		outcap("te");
		tcsetattr(FT_OUT_FD, TCSADRAIN, &saved_t);
		return (TRUE);
	}
	if (tcgetattr(FT_OUT_FD, &saved_t) == -1)
		return (FALSE);
	t = saved_t;
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_oflag &= ~OPOST;
	set_read_timeout(0, &t);
	if (tcsetattr(FT_OUT_FD, TCSADRAIN, &t) == -1)
		return (FALSE);
	return (saved_t.c_cc[VSUSP]);
}
