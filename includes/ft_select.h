/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:28:15 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/19 19:57:44 by kdumarai         ###   ########.fr       */
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
	struct winsize	ws;
	size_t			nlines;
	size_t			ncols;
	unsigned int	vscroll;
	unsigned int	scroll_off;
}				t_cursor;

typedef struct	s_choice
{
	int				selected;
	char			*title;
	size_t			titlelen;
	struct s_choice	*next;
	struct s_choice	*prev;
}				t_choice;

# define FT_OUT_FD	STDIN_FILENO
# define FT_PAD_NB	2

/*
** Set/Restore Terminal
*/

int				fill_kcmps(t_tkeys *dest);
void			restore_terminal(void);
int				set_read_timeout(cc_t timeout, struct termios *tptr);
int				init_terminal(void);

/*
** printing
*/

void			clear_choices(t_cursor *csr);
void			erase_curr_line(t_cursor *csr);
void			print_with_csr(t_choice *choices, t_cursor *csr);
void			set_window_prop(t_cursor *dest);

/*
** interaction
*/

void			mov_up(t_choice **ch, t_cursor *csr);
void			mov_down(t_choice **ch, t_cursor *csr);
void			mov_left(t_choice **ch, t_cursor *csr);
void			mov_right(t_choice **ch, t_cursor *csr);
void			redraw_hdl(int sigc);
int				chk_keys(t_choice **choices, t_cursor *csr, t_tkeys *kcmps);

/*
** stuff
*/

t_choice		*ft_chnew(char *title, int selected);
void			ft_chpush(t_choice **headref, t_choice *new);
void			ft_chdelone(t_choice **headref, t_choice *ch);
void			ft_chdel(t_choice **headref);
t_choice		*ft_chgetidx(t_choice *head, unsigned int idx);

/*
** Misc/Utilities
*/

int				ft_strasciieq(char *s, char c);
int				putcf(int c);
int				outcap_arg(char *name, int affcnt);
int				outcap(char *name);
int				movcap(unsigned int x, unsigned int y);

#endif
