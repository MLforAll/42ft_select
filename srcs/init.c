/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 19:20:41 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/15 23:15:28 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "ft_select.h"

static struct termios	saved_t;

/*
** set_window_prop
**
** t_cursor*	destination struct
*/

void					set_window_prop(t_cursor *dest)
{
	ft_bzero(&dest->ws, sizeof(struct winsize));
	ioctl(FT_OUT_FD, TIOCGWINSZ, &dest->ws);
	if (dest->max < dest->ws.ws_row || dest->mlen > dest->ws.ws_col
		|| dest->mlen <= 0)
		dest->ncols = 1;
	else
		dest->ncols = dest->ws.ws_col / (dest->mlen + FT_PAD_NB);
	if (dest->ncols <= 0)
		dest->ncols = 1;
	dest->nlines = dest->max / dest->ncols + (dest->max % dest->ncols);
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
	t.c_cc[VMIN] = 0;
	t.c_cc[VTIME] = 2;
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
