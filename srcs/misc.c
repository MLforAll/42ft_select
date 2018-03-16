/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:50:49 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/16 03:17:08 by kdumarai         ###   ########.fr       */
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

int		ft_strasciieq(char *s, char c)
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
** putcf
**
** int		char to print
*/

int		putcf(int c)
{
	write(FT_OUT_FD, &c, 1);
	return (1);
}

/*
** outcap_arg -- output a termcap with argument
**
** char*		name of termcap
** int			number of lines affected
*/

int		outcap_arg(char *name, int affcnt)
{
	char	*cap;

	cap = tgetstr(name, NULL);
	if (!cap)
		return (FALSE);
	tputs(cap, affcnt, &putcf);
	return (TRUE);
}

/*
** outcap -- output a termcap
**
** char*		name of termcap
*/

int		outcap(char *name)
{
	return (outcap_arg(name, 1));
}
