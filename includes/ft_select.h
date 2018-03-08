/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:28:15 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/08 22:16:47 by kdumarai         ###   ########.fr       */
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

typedef struct	s_choice
{
	int				selected;
	char			*title;
	struct s_choice	*next;
}				t_choice;

/*
**
*/

void		return_res(t_list *res);
void		clear_choices(size_t len);
void		print_with_csr(t_choice *choices, t_cursor *csr);
t_list		*chk_keys(int fd, t_choice *choices, t_cursor *csr);

/*
**
*/

t_choice	*ft_chnew(char *title, int selected);
void		ft_chpush(t_choice **headref, t_choice *new);
void		ft_chdelone(t_choice **headref, t_choice *ch);
void		ft_chdel(t_choice **headref);

/*
** Misc/Utilities
*/

int			putcf(int c);
int			outcap_arg(char *name, int affcnt);
int			outcap(char *name);
void		ft_lstdelf(void *dat, size_t datsize);

#endif
