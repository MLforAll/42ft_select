/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 19:20:41 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/19 04:02:22 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "ft_select.h"

static struct termios	saved_t;

/*
** fill_kcmps
**
** t_tkeys*		struct containing term movkeys caps
*/

int						fill_kcmps(t_tkeys *dest)
{
	dest->rightk = tgetstr("kr", NULL);
	dest->upk = tgetstr("ku", NULL);
	dest->downk = tgetstr("kd", NULL);
	dest->leftk = tgetstr("kl", NULL);
	dest->delk = tgetstr("kD", NULL);
	(dest->bsk)[0] = 127;
	(dest->bsk)[1] = '\0';
	return (TRUE);
}

/*
** restore_terminal
*/

void					restore_terminal(void)
{
	outcap("ve");
	outcap("ke");
	outcap("te");
	tcsetattr(FT_OUT_FD, TCSADRAIN, &saved_t);
}

/*
** signal_hdl -- handle signals (except SIGCONT and SIGWINCH)
**
** int			signal code
*/

static void				signal_hdl(int sigc)
{
	const char	sim_sigtstp[2] = {saved_t.c_cc[VSUSP], '\0'};

	restore_terminal();
	if (sigc == SIGTSTP)
	{
		signal(sigc, SIG_DFL);
		ioctl(STDIN_FILENO, TIOCSTI, sim_sigtstp);
		return ;
	}
	exit(EXIT_SUCCESS);
}

/*
** set_read_timeout
**
** cc_t				timeout
** struct termios*	existing dump termios struct
**						(will NOT apply changes if provided)
*/

int						set_read_timeout(cc_t timeout, struct termios *tptr)
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
** init_terminal
*/

int						init_terminal(void)
{
	struct termios	t;
	int				success;
	int				sigc;

	success = 0;
	if (tcgetattr(FT_OUT_FD, &saved_t) == -1)
		return (FALSE);
	t = saved_t;
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_oflag &= ~OPOST;
	set_read_timeout(0, &t);
	if (tcsetattr(FT_OUT_FD, TCSADRAIN, &t) == -1)
		return (FALSE);
	if (!(success += outcap("ti") + outcap("ks") + outcap("vi")))
		return (FALSE);
	sigc = 0;
	while (sigc < 32)
	{
		if (sigc != 11)
			signal(sigc, (sigc != SIGCONT) ? &signal_hdl : &redraw_hdl);
		sigc++;
	}
	signal(SIGWINCH, &redraw_hdl);
	return (TRUE);
}
