/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 13:27:06 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/01/31 15:41:18 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "ft/mem.h"
#include "ft/str.h"

static void	*ft_memmove(void *dst, const void *src, size_t len)
{
	const char	*c_src = src;
	char		*c_dst;

	c_dst = dst;
	if (dst > src)
		while (len--)
			c_dst[len] = c_src[len];
	else
		while (len--)
			*c_dst++ = *c_src++;
	return (dst);
}

char		*sh_getenv(struct s_shell *shell, char *name)
{
	const size_t	name_size = ft_strlen(name);
	size_t			size;
	size_t			len;
	char			*env;

	env = (char *)shell->buffer;
	size = shell->env_size;
	while (size)
	{
		len = ft_strlen(env);
		if (len >= name_size)
		{
			if (ft_memcmp(env, name, name_size) == 0
				&& env[name_size] == '=')
				return (env + name_size + 1);
		}
		env += len + 1;
		size -= len + 1;
	}
	return (NULL);
}

bool		sh_unsetenv(struct s_shell *shell, char *name)
{
	const size_t	name_size = ft_strlen(name);
	char			*env;
	size_t			len;

	if (name_size == 4 && ft_memcmp(name, "PATH", 4) == 0)
		shell->path = NULL;
	if ((env = sh_getenv(shell, name)))
	{
		len = ft_strlen(env) + name_size + 1;
		env -= name_size + 1;
		ft_memmove(env, env + len + 1,
			(char *)shell->buffer + shell->env_size - (env + len + 1));
		shell->env_size -= len + 1;
		shell->env_len -= 1;
		return (shell->dirty_env = true);
	}
	else
		return (false);
}

bool		sh_setenv(struct s_shell *shell, char *name, char *value)
{
	const size_t	name_size = ft_strlen(name);
	const size_t	value_size = ft_strlen(value);
	char			*env;

	sh_unsetenv(shell, name);
	if (shell->env_size + name_size + 1 + value_size + 1 > shell->buffer_size)
		return (false);
	env = (char *)shell->buffer + shell->env_size;
	if (name_size == 4 && ft_memcmp(name, "PATH", 4) == 0)
		shell->path = env;
	ft_memmove(env, name, name_size);
	ft_memmove(env + name_size + 1, value, value_size);
	env[name_size] = '=';
	env[name_size + 1 + value_size] = '\0';
	shell->env_size += name_size + 1 + value_size + 1;
	shell->env_len += 1;
	return (shell->dirty_env = true);
}
