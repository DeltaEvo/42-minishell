/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 14:07:28 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/02/07 17:48:12 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "env.h"
#include "ft/str.h"
#include "ft/mem.h"
#include <pwd.h>

/*
** endpwent() must be called after this function
*/

static char	*get_user_home(struct s_shell *shell, char *name, size_t size)
{
	struct passwd	*pwd;
	char			*home;

	if (size == 0)
	{
		home = sh_getenv(shell, "HOME", 4);
		if (!home)
		{
			pwd = getpwuid(geteuid());
			return (pwd ? pwd->pw_dir : NULL);
		}
		return (home);
	}
	setpwent();
	while ((pwd = getpwent()))
		if (ft_strncmp(pwd->pw_name, name, size) == 0
				&& pwd->pw_name[size] == '\0')
			break ;
	return (pwd ? pwd->pw_dir : NULL);
}

static void	expand_home(struct s_shell *shell, char *arg, size_t *len,
		size_t *size)
{
	const size_t	i = arg - ((char *)shell->buffer + shell->env_size);
	char			*var;
	size_t			var_len;
	char			*end;

	if (*arg == '~')
	{
		if (!(end = ft_memchr(arg, '/', *len)))
			end = arg + *len;
		if (!(var = get_user_home(shell, arg + 1, end - arg - 1)))
			return ;
		var_len = ft_strlen(var);
		if (*size + var_len + shell->env_size - (end - arg)
				> shell->buffer_size)
			return ;
		ft_memmove(arg + var_len, end, *size - i - (end - arg));
		ft_memcpy(arg, var, var_len);
		*len = *len + var_len - (end - arg);
		*size = *size + var_len - (end - arg);
	}
}

static void	expand_dollar(struct s_shell *shell, char *arg, size_t *len,
		size_t *size)
{
	const size_t	i = arg - ((char *)shell->buffer + shell->env_size);
	char			*var;
	size_t			var_len;
	char			*dollar;
	char			*end;

	dollar = ft_memchr(arg, '$', *len);
	while (dollar)
	{
		if (arg + *len - (dollar + 1) == 0)
			break ;
		if (!(end = ft_memchr(dollar + 1, '$', arg + *len - (dollar + 1))))
			end = arg + *len;
		var = sh_getenv(shell, dollar + 1, end - dollar - 1);
		var_len = var ? ft_strlen(var) : 0;
		if (*size + var_len + shell->env_size - (end - dollar)
				> shell->buffer_size)
			return ;
		ft_memmove(dollar + var_len, end, *size - (end - arg) - i);
		ft_memcpy(dollar, var, var_len);
		*len = *len + var_len - (end - dollar);
		*size = *size + var_len - (end - dollar);
		dollar = ft_memchr(dollar + var_len, '$', *len - (dollar - arg));
	}
}

size_t		perform_expansion(struct s_shell *shell, size_t size)
{
	size_t	i;
	char	*arg;
	size_t	len;

	i = 0;
	while (i < size)
	{
		arg = (char *)shell->buffer + shell->env_size + i;
		len = ft_strlen(arg);
		expand_home(shell, arg, &len, &size);
		endpwent();
		expand_dollar(shell, arg, &len, &size);
		if (len == 0)
		{
			ft_memmove(arg, arg + 1, size - i - 1);
			size--;
		}
		else
			i += len + 1;
	}
	return (size);
}
