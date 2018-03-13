/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 19:20:41 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/13 21:16:34 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "ft_select.h"

static struct termios	saved_t;

void					restore_terminal(void)
{
	outcap("ve");
	outcap("ke");
	outcap("te");
	tcsetattr(FT_OUT_FD, TCSADRAIN, &saved_t);
}

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
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (tcsetattr(FT_OUT_FD, TCSADRAIN, &t) == -1)
		return (FALSE);
	if (!(success += outcap("ti") + outcap("ks") + outcap("vi")))
		return (FALSE);
	sigc = 0;
	while (sigc < 32)
	{
		signal(sigc, (sigc != SIGCONT) ? &signal_hdl : &redraw_hdl);
		sigc++;
	}
	signal(SIGWINCH, &redraw_hdl);
	return (TRUE);
}
