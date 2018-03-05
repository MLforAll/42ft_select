/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:50:49 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/05 22:02:44 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

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

int		putcf(int c)
{
	write(1, &c, 1);
	return (1);
}
