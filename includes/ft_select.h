/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:28:15 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/14 18:18:19 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include "libft.h"
# include <termios.h>
# include <term.h>

typedef struct	s_tkeys
{
	char			*rightk;
	char			*leftk;
	char			*upk;
	char			*downk;
	char			*delk;
	char			bsk[2];
}				t_tkeys;

typedef struct	s_cursor
{
	unsigned long	pos;
	size_t			max;
	size_t			mlen;
	size_t			nlines;
	size_t			ncols;
}				t_cursor;

typedef struct	s_choice
{
	int				selected;
	char			*title;
	struct s_choice	*next;
	struct s_choice	*prev;
}				t_choice;

# define FT_OUT_FD	STDIN_FILENO
# define FT_PAD_NB	2

/*
** Set/Restore Terminal
*/

void			redraw_hdl(int sigc);

void			set_window_prop(t_cursor *dest);
void			restore_terminal(void);
int				init_terminal(void);

/*
** stuff
*/

void			return_res(t_choice *res);
void			clear_choices(t_cursor *csr);
void			print_with_csr(t_choice *choices, t_cursor *csr);
void			chk_keys(t_choice **choices, t_cursor *csr);

/*
** stuff
*/

t_choice		*ft_chnew(char *title, int selected);
void			ft_chpush(t_choice **headref, t_choice *new);
void			ft_chdelone(t_choice **headref, t_choice *ch);
void			ft_chdel(t_choice **headref);
void			switch_selected(t_choice *head, unsigned int idx);

/*
** Misc/Utilities
*/

int				putcf(int c);
int				outcap_arg(char *name, int affcnt);
int				outcap(char *name);
void			ft_lstdelf(void *dat, size_t datsize);

#endif
