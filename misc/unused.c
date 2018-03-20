/*
** print_status -- print the status line
**
** t_cursor*	cursor
*/

void		print_status(t_cursor *csr)
{
	const char		*status_text = "ft_select";
	char			*buff;
	char			*spaces;
	size_t			half_len;

	if (!tgetflag("hs") || !tgetflag("es"))
		return ;
	if (!(buff = (char*)malloc(sizeof(char) * (csr->ws.ws_col + 1))))
		return ;
	half_len = csr->ws.ws_col / 2 - ft_strlen(status_text) / 2 - 1;
	if (!(spaces = (char*)malloc(sizeof(char) * (half_len + 1))))
		return ;
	spaces[half_len] = '\0';
	outcap("ts");
	outcap("mr");
	ft_memset(spaces, ' ', half_len);
	ft_strcpy(buff, spaces);
	ft_strcat(buff, status_text);
	ft_strcat(buff, spaces);
	free(spaces);
	ft_putstr_fd(buff, FT_OUT_FD);
	outcap("me");
	ft_putstr_fd("\r\n", FT_OUT_FD);
	free(buff);
	outcap("fs");
}