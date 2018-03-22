/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:28:15 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/22 16:46:25 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include "libft.h"
# include <termios.h>
# include <term.h>

/*
** Data Structures
*/

typedef struct	s_tkeys
{
	char			*rightk;
	char			*leftk;
	char			*upk;
	char			*downk;
	char			*delk;
	char			bsk[2];
}				t_tkeys;

typedef struct	s_choice
{
	int				selected;
	char			*title;
	size_t			titlelen;
	struct s_choice	*next;
	struct s_choice	*prev;
}				t_choice;

typedef struct	s_env
{
	t_choice		*choices;
	unsigned long	pos;
	size_t			max;
	size_t			mlen;
	struct winsize	ws;
	size_t			nlines;
	size_t			ncols;
	unsigned int	vscroll;
	unsigned int	scroll_off;
}				t_env;

/*
** Macros
*/

# define FT_OUT_FD	STDIN_FILENO
# define FT_PAD_NB	2

/*
** Set/Restore Terminal
*/

int				set_read_timeout(cc_t timeout, struct termios *tptr);
int				init_restore_terminal(int init, char *vsusp_ptr);

/*
** Display
*/

void			set_window_prop(t_env *dest);
int				fill_kcmps(t_tkeys *dest);
int				init_restore_display(t_env *env, int init);

/*
** Printing
*/

void			clear_choices(t_env *env);
void			print_with_env(t_env *env);

/*
** Interaction
*/

void			mov_page(t_env *env);
void			mov_up(t_env *env);
void			mov_down(t_env *env);
void			mov_left(t_env *env);
void			mov_right(t_env *env);
int				chk_keys(t_env *env, t_tkeys *kcmps);

/*
** Signals
*/

void			signal_hdl(int sigc);
void			redraw_hdl(unsigned long long sigc);
int				set_signals(void);

/*
** List Utilities
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
int				outcap(char *name);
int				movcap(unsigned int x, unsigned int y);

#endif
