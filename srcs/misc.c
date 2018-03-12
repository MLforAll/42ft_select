/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:50:49 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/12 22:48:14 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

/*
** ft_lstdef -- del a chain with str as data
**
** void*		data of chain
** size_t		size of data of chain (ign)
*/

void	ft_lstdelf(void *dat, size_t datsize)
{
	(void)datsize;
	free(dat);
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
