/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:50:49 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/24 13:39:22 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

/*
** ft_strasciieq
**
** char*	string
** int		char
*/

int			ft_strasciieq(char *s, char c)
{
	if (!s || *s != c)
		return (FALSE);
	while (*(++s))
	{
		if (*s != 0)
			return (FALSE);
	}
	return (TRUE);
}

/*
** putcf (private)
**
** int		char to print
*/

static int	putcf(int c)
{
	write(FT_OUT_FD, &c, 1);
	return (1);
}

/*
** outcap -- output a termcap
**
** char*		name of termcap
*/

int			outcap(char *name)
{
	char	*cap;

	cap = tgetstr(name, NULL);
	if (!cap)
		return (FALSE);
	tputs(cap, 1, &putcf);
	return (TRUE);
}

/*
** movcap -- move cursor using termcap
**
** unsigned int		x position
** unsigned int		y position
*/

int			movcap(unsigned int x, unsigned int y)
{
	const char		*cmstr = tgetstr("cm", NULL);
	char			*gotostr;

	if (!cmstr || !(gotostr = tgoto(cmstr, x, y)))
		return (FALSE);
	tputs(gotostr, 1, &putcf);
	return (TRUE);
}

/*
** fatal
**
** const char*	app's name
** const char*	error message
*/

void		fatal(const char *app, const char *err)
{
	init_restore_display(NULL, NO);
	init_restore_terminal(NO, NULL);
	if (app)
	{
		ft_putstr_fd(app, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(err, STDERR_FILENO);
	exit(EXIT_FAILURE);
}
