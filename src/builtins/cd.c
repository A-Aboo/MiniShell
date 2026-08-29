/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static char	*cd_target(char **argv, char **env)
{
	char	*path;

	if (argv[1])
		return (argv[1]);
	path = get_env_value("HOME", env);
	if (!path)
	{
		print_builtin_error("cd", NULL, "HOME not set");
		return (NULL);
	}
	return (path);
}

static void	cd_error(char *path)
{
	char	*message;

	message = ft_strjoin("cd: ", path);
	if (!message)
	{
		print_perror(path);
		return ;
	}
	print_perror(message);
	free(message);
}

/* Both variables are kept in step with the shell's real directory so
   that $PWD and $OLDPWD stay usable after a cd. */

static void	update_pwd(char ***env, char *old_pwd, char *new_pwd)
{
	if (old_pwd)
		set_env_value(env, "OLDPWD", old_pwd);
	if (new_pwd)
		set_env_value(env, "PWD", new_pwd);
	free(old_pwd);
	free(new_pwd);
}

/* The logical path is preferred so that symlinks stay visible; if it
   cannot be entered, the target is used and PWD becomes the real one. */

static int	change_directory(char *path, char **logical)
{
	if (*logical && chdir(*logical) == 0)
		return (0);
	if (chdir(path) == -1)
		return (-1);
	free(*logical);
	*logical = getcwd(NULL, 0);
	return (0);
}

int	builtin_cd(char **argv, char ***env)
{
	char	*path;
	char	*logical;
	char	*old_pwd;

	if (argv[1] && argv[2])
		return (print_builtin_error("cd", NULL, "too many arguments"));
	path = cd_target(argv, *env);
	if (!path)
		return (1);
	if (!path[0])
		return (0);
	old_pwd = current_pwd(*env);
	logical = cd_logical_path(path, *env);
	if (change_directory(path, &logical) == -1)
	{
		free(old_pwd);
		free(logical);
		cd_error(path);
		return (1);
	}
	update_pwd(env, old_pwd, logical);
	return (0);
}
