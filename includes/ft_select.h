/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:28:15 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/07 21:57:15 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT
# define FT_SELECT

# include "libft.h"
# include <termios.h>
# include <term.h>

typedef struct	s_tkeys
{
	char			*rightk;
	char			*leftk;
	char			*upk;
	char			*downk;
}				t_tkeys;

typedef struct	s_cursor
{
	unsigned long	pos;
	size_t			max;
}				t_cursor;

/*
**
*/

void	print_with_csr(t_list *choices, t_cursor *csr);
t_list	*chk_keys(int fd, t_list *choices);

/*
** Misc/Utilities
*/

void	ft_lstdelf(void *dat, size_t datsize);
int		putcf(int c);
int		outcap(char *name);

#endif
