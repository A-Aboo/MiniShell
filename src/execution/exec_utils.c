/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	exec_error(char *cmd, int error)
{
	if (error == ENOEXEC)
	{
		print_builtin_error(NULL, cmd,
			"cannot execute binary file: Exec format error");
		return (126);
	}
	print_builtin_error(NULL, cmd, "Permission denied");
	return (126);
}

/* Reports why a command cannot be run and returns bash's exit code:
   127 when nothing was found, 126 when it exists but cannot run. */

int	command_error_code(char *cmd)
{
	struct stat	info;
	int			error;

	error = errno;
	if (!cmd || !cmd[0] || !ft_strchr(cmd, '/'))
	{
		print_builtin_error(NULL, cmd, "command not found");
		return (127);
	}
	if (stat(cmd, &info) == -1)
	{
		print_perror(cmd);
		return (127);
	}
	if (S_ISDIR(info.st_mode))
	{
		print_builtin_error(NULL, cmd, "Is a directory");
		return (126);
	}
	return (exec_error(cmd, error));
}

/* Bash calls a file binary when it starts with the ELF magic number,
   or when a NUL byte comes before its first newline. */

static int	is_binary_file(char *path)
{
	char	sample[80];
	int		size;
	int		fd;
	int		i;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (0);
	size = read(fd, sample, 80);
	close(fd);
	if (size < 0)
		return (0);
	if (size >= 4 && sample[0] == 0x7f && sample[1] == 'E'
		&& sample[2] == 'L' && sample[3] == 'F')
		return (1);
	i = 0;
	while (i < size && sample[i] != '\n')
	{
		if (sample[i] == '\0')
			return (1);
		i++;
	}
	return (0);
}

/* A text file that holds no executable format is handed to /bin/sh,
   which is what makes a script without a shebang line run. */

static void	exec_as_script(char *path, char **argv, char **env)
{
	char	**sh_argv;
	int		size;
	int		i;

	size = 0;
	while (argv[size])
		size++;
	sh_argv = malloc(sizeof(char *) * (size + 2));
	if (!sh_argv)
		return ;
	sh_argv[0] = "/bin/sh";
	sh_argv[1] = path;
	i = 1;
	while (argv[i])
	{
		sh_argv[i + 1] = argv[i];
		i++;
	}
	sh_argv[size + 1] = NULL;
	execve("/bin/sh", sh_argv, env);
	free(sh_argv);
}

/* Runs in the child: applies redirections then replaces the process. */

void	execute_external_child(t_command *cmd, char **env)
{
	char	*path;
	int		saved;
	int		code;

	setup_child_signals();
	if (apply_redirections(cmd->redirections) == -1)
		exit(1);
	if (!cmd->argv || !cmd->argv[0])
		exit(0);
	path = find_command_path(cmd->argv[0], env);
	if (!path)
		exit(command_error_code(cmd->argv[0]));
	execve(path, cmd->argv, env);
	saved = errno;
	if (saved == ENOEXEC && !is_binary_file(path))
		exec_as_script(path, cmd->argv, env);
	errno = saved;
	code = command_error_code(path);
	free(path);
	exit(code);
}
