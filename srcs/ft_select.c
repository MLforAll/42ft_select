/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:24:55 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/23 21:00:53 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

/*
** get_choices (private)
**
** t_env*		environement
** char**		arguments of prgm aka av + 1
*/

static int		get_choices(t_env *env, char **args)
{
	t_choice	*new;

	if (!env)
		return (FALSE);
	env->max = 0;
	env->choices = NULL;
	while (*args)
	{
		if (**args)
		{
			if (!(new = ft_chnew(*args, NO)))
				return (FALSE);
			if (new->titlelen > env->mlen)
				env->mlen = new->titlelen;
			ft_chpush(&env->choices, new);
			env->max++;
		}
		args++;
	}
	return ((env->max > 0));
}

/*
** return_res (private)
**
** t_choice*	all choices (arguments)
*/

static void		return_res(t_choice *choices)
{
	int				first;

	first = YES;
	while (choices)
	{
		if (choices->selected)
		{
			if (!first)
				ft_putchar(' ');
			else
				first = NO;
			ft_putstr(choices->title);
		}
		choices = choices->next;
	}
	if (!first)
		ft_putchar('\n');
}

/*
** fill_tcdb -- fill the termcap database (private)
*/

static void		fill_tcdb(void)
{
	char		*termtype;
	char		ans;
	char		*tmp;

	ans = 0;
	if (!(termtype = getenv("TERM")))
	{
		ft_putstr_fd("TERM env var not found.\r\n", FT_OUT_FD);
		while (ans != 'y')
		{
			ft_putstr_fd("Use default value ? (y/N) ", FT_OUT_FD);
			if (read(FT_OUT_FD, &ans, 1) < 1)
				fatal(FT_APP_NAME, "\nwrite error");
			ft_putstr_fd("\r\n", FT_OUT_FD);
			ans = ft_tolower(ans);
			if (ans == 'y')
				termtype = "xterm";
			else if (ans == 'n')
				fatal(FT_APP_NAME, "User canceled the operation");
		}
	}
	if (tgetent(NULL, termtype) <= 0)
		fatal(FT_APP_NAME, "Invalid terminal");
	tmp = tgetstr("pc", NULL);
	PC = (tmp) ? *tmp : 0;
}

/*
** main
**
** int			number of args from shell
** char**		arguments from shell
*/

int				main(int ac, char **av)
{
	t_env		env;
	int			show_res;
	char		vsusp_char;

	vsusp_char = 0;
	ft_bzero(&env, sizeof(t_env));
	if (!isatty(FT_OUT_FD))
		fatal(FT_APP_NAME, "Invalid output fd");
	if (ac == 1 || !get_choices(&env, av + 1))
		fatal(FT_APP_NAME, "Arguments error");
	if (!init_restore_terminal(YES, &vsusp_char))
		fatal(FT_APP_NAME, "Init error");
	signal_hdl(vsusp_char);
	redraw_hdl((unsigned long long)&env);
	fill_tcdb();
	if (!init_restore_display(&env, YES))
		fatal(FT_APP_NAME, "Error setting display");
	show_res = chk_keys(&env);
	init_restore_display(NULL, NO);
	init_restore_terminal(NO, NULL);
	(show_res) ? return_res(env.choices) : 0;
	ft_chdel(&env.choices);
	return (EXIT_SUCCESS);
}
