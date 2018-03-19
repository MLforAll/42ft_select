/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:24:55 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/19 03:27:49 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

/*
** get_choices
**
** char**		arguments of prgm aka av + 1
*/

static size_t	get_choices(t_choice **dest, t_cursor *csr, char **args)
{
	size_t		ret;
	t_choice	*new;

	if (!dest)
		return (0);
	ret = 0;
	*dest = NULL;
	while (*args)
	{
		if (!(new = ft_chnew(*args, NO)))
			return (0);
		if (new->titlelen > csr->mlen)
			csr->mlen = new->titlelen;
		ft_chpush(dest, new);
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
** fatal
**
** const char*	app's name
** const char*	error message
*/

static void		fatal(const char *app, const char *err)
{
	ft_putstr_fd(app, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(err, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

/*
** main
**
** int			number of args from shell
** char**		arguments from shell
*/

int				main(int ac, char **av)
{
	const char	*app_name = av[0];
	t_choice	*choices;
	t_cursor	csr;
	t_tkeys		kcmps;
	char		*termtype;
	int			show_res;

	if (!(termtype = getenv("TERM")))
		fatal(app_name, "TERM env var missing");
	if (tgetent(NULL, termtype) <= 0)
		fatal(app_name, "Invalid terminal");
	if (!ft_isatty(FT_OUT_FD))
		fatal(app_name, "Invalid output fd");
	ft_bzero(&csr, sizeof(t_cursor));
	if (ac == 1 || !(csr.max = get_choices(&choices, &csr, av + 1)))
		return (EXIT_FAILURE);
	set_window_prop(&csr);
	if (!fill_kcmps(&kcmps) || !init_terminal())
		fatal(app_name, "Error setting the terminal");
	show_res = chk_keys(&choices, &csr, &kcmps);
	restore_terminal();
	if (show_res && !ft_isatty(STDOUT_FILENO))
		return_res(choices);
	ft_chdel(&choices);
	return (EXIT_SUCCESS);
}
