/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:24:55 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/21 02:26:07 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

/*
** get_choices
**
** t_env*		environement
** char**		arguments of prgm aka av + 1
*/

static size_t	get_choices(t_env *env, char **args)
{
	size_t		ret;
	t_choice	*new;

	if (!env)
		return (0);
	ret = 0;
	env->choices = NULL;
	while (*args)
	{
		if (!(new = ft_chnew(*args, NO)))
			return (0);
		if (new->titlelen > env->mlen)
			env->mlen = new->titlelen;
		ft_chpush(&env->choices, new);
		args++;
		ret++;
	}
	return (ret);
}

/*
** return_res
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
** fill_tcdb -- fill the termcap database
**
** const char*	name of app (av[0])
*/

static void		fill_tcdb(const char *app_name)
{
	char		*termtype;
	char		ans;

	ans = '\0';
	if (!(termtype = getenv("TERM")))
	{
		ft_putstr_fd("TERM env var not found.\r\n", FT_OUT_FD);
		while (ans != 'y')
		{
			ft_putstr_fd("Use default value ? (y/N) ", FT_OUT_FD);
			if (read(FT_OUT_FD, &ans, 1) < 1)
				fatal(app_name, "\nwrite error");
			ft_putstr_fd("\r\n", FT_OUT_FD);
			ans = ft_tolower(ans);
			if (ans == 'y')
				termtype = "xterm";
			else if (ans == 'n')
				fatal(app_name, "User canceled the operation");
		}
	}
	if (tgetent(NULL, termtype) <= 0)
		fatal(app_name, "Invalid terminal");
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
	t_tkeys		kcmps;
	int			show_res;
	char		vsusp_char;

	vsusp_char = 0;
	ft_bzero(&env, sizeof(t_env));
	if (!isatty(FT_OUT_FD))
		fatal(av[0], "Invalid output fd");
	if (!set_signals() || !init_restore_terminal(YES, &vsusp_char))
		fatal(av[0], "Init error");
	signal_hdl(vsusp_char);
	redraw_hdl((unsigned long long)&env);
	fill_tcdb(av[0]);
	if (ac == 1 || !(env.max = get_choices(&env, av + 1)))
		fatal(av[0], "Missing arguments or malloc error");
	if (!init_display(&env) || !fill_kcmps(&kcmps))
		fatal(av[0], "Error setting display");
	show_res = chk_keys(&env, &kcmps);
	init_restore_terminal(NO, NULL);
	(show_res && !isatty(STDOUT_FILENO)) ? return_res(env.choices) : 0;
	ft_chdel(&env.choices);
	return (EXIT_SUCCESS);
}
