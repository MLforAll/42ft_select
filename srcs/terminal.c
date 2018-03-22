/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 19:20:41 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/22 17:41:43 by kdumarai         ###   ########.fr       */
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
		vsusp_char = sigc;
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
** set_signals
*/

int			set_signals(void)
{
	const int		killers[] = {SIGHUP, SIGINT, SIGQUIT, SIGPIPE, SIGALRM,
								SIGTERM, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF,
								SIGUSR1, SIGUSR2, 0};
	const int		toblock[] = {SIGTSTP, SIGCONT, SIGTTIN, SIGTTOU, 0};
	const int		*sigs[] = {killers, toblock, NULL};
	static void		(*hdls[])(int) = {&signal_hdl, SIG_IGN, NULL};
	unsigned int	idx[2];

	idx[0] = 0;
	while (hdls[idx[0]] && sigs[idx[0]])
	{
		idx[1] = 0;
		while (sigs[idx[0]][idx[1]])
		{
			if (signal(sigs[idx[0]][idx[1]], hdls[idx[0]]) == SIG_ERR)
				return (FALSE);
			idx[1]++;
		}
		idx[0]++;
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
	static int				noget = FALSE;
	static struct termios	t;
	struct termios			*dest;

	if (!tptr)
	{
		if (!noget && tcgetattr(FT_OUT_FD, &t) == -1)
			return (FALSE);
		noget = TRUE;
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
** char*			ptr to store vsusp char
*/

int			init_restore_terminal(int init, char *vsusp_ptr)
{
	static int				is_init = FALSE;
	static struct termios	saved_t;
	struct termios			t;

	if (!init)
	{
		if (!is_init)
			return (FALSE);
		is_init = FALSE;
		return (tcsetattr(FT_OUT_FD, TCSADRAIN, &saved_t) == -1 ? FALSE : TRUE);
	}
	if (is_init || tcgetattr(FT_OUT_FD, &saved_t) == -1)
		return (FALSE);
	t = saved_t;
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_oflag &= ~OPOST;
	set_read_timeout(0, &t);
	if (tcsetattr(FT_OUT_FD, TCSADRAIN, &t) == -1)
		return (FALSE);
	if (vsusp_ptr)
		*vsusp_ptr = saved_t.c_cc[VSUSP];
	return ((is_init = TRUE));
}
