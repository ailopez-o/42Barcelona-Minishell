/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ailopez- <ailopez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 10:01:08 by test              #+#    #+#             */
/*   Updated: 2023/03/09 17:25:18 by ailopez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "utils.h"
#include "readline.h"
#include "signals.h"
#include <fcntl.h>
#include "redirects.h"
#include "executor.h"

//	---- local headers
static bool	prepare_fd(int *fd, char *data, int8_t type);
static bool	own_here_doc(int *fd_return, t_redirect *redi, t_env *env_list);
static bool	own_here_doc_while(int *fd, char *limitator, t_env *env_list, bool quoted_here);

//	---- public
bool	prepare_redirect(t_fdmanage **fdman, t_redirect *redi, t_env *env_list)
{
	int			tmp_fd;

	tmp_fd = 0;
	while (redi)
	{
		if (redi->type == RDOC)
		{
			own_here_doc(&tmp_fd, redi, env_list);
			// return (print_error(ft_strjoin("ba.sh: ", strerror(errno)), 1));// falta cerrar todo!@to_do
		}
		if (redi->type != RDOC)
		{
			prepare_fd(&tmp_fd, redi->data, redi->type);
		}
		if (tmp_fd > 0)
		{
			if (dup2(tmp_fd, redi->fd) < 0)
				return (print_error(ft_strjoin("ba.sh: ", strerror(errno)), 1));
			if ((close_fdman(fdman, redi->fd)) || (add_fdman(fdman, redi->fd, tmp_fd)))// no estoy seguro de si esto esta bien @to_do
				return (print_error(ft_strjoin("ba.sh: ", strerror(errno)), 1));
		}
		redi = redi->next;
	}
	return (0);
}

bool	add_fdman(t_fdmanage **fdman, int index, int fd)
{
	t_fdmanage 	*toadd;

	toadd = ft_calloc(sizeof(t_fdmanage), 1);
	if (!toadd)
		return (1);//ERROR GRAVE NOSE COMO ESCALAR @to_do
	toadd->index = index;
	toadd->fd = fd;
	toadd->next = *fdman;
	*fdman = toadd;
	return (0);
}

bool	close_fdman(t_fdmanage **fdman, int index)
{
	t_fdmanage 	*tmp;
	t_fdmanage 	*last;

	tmp = *fdman;
	last = NULL;
	while (tmp)
	{
		if (tmp->index == index)
		{
			if (tmp->fd > 1 && close(tmp->fd))
				return (1);//ERROR @to_do
			if (!last)
				{*fdman = tmp->next;tmp = tmp->next;}
			else
				{last->next = tmp->next;tmp = last->next;}
			free(tmp);
			return (0);
		}
		else
			{last = tmp;tmp = tmp->next;}
	}
	return (0);
}

//	---- private
static bool	prepare_fd(int *fd, char *data, int8_t type)
{
	if (type == RIN)
	{
		// if (access(data, F_OK))
		// 	return (print_error(ft_strjoin("ba.sh: ", ft_strjoin(data, ": No such a file or directory\n")), 1));
		// if (access(data, R_OK))
		// 	return (print_error(ft_strjoin("ba.sh: ", ft_strjoin(data, ": Permision denied\n")), 1));
		*fd = open(data, O_RDONLY);
	}
	else if (type == ROUT)
	{
		// if (!access(data, F_OK))
		// 	if (access(data, W_OK))
		// 		return (print_error(ft_strjoin("ba.sh: ", ft_strjoin(data, ": Permision denied\n")), 1));
		*fd = open(data, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	}
	else if (type == RADD)
	{
		// if (!access(data, F_OK))
		// 	if (access(data, W_OK))
		// 		return (print_error(ft_strjoin("ba.sh: ", ft_strjoin(data, ": Permision denied\n")), 1));
		*fd = open(data, O_CREAT | O_WRONLY | O_APPEND, 0666);
	}
	if (*fd < 0)
		printf("bash: %s: %s\n", data, strerror(errno));
	return (0);
}

static bool	own_here_doc(int *fd_return, t_redirect *redi, t_env *env_list)
{
	int		fd[2];
	int		status;
	pid_t	pid;

	if (pipe(fd) < 0)
		return (1);
	pid = fork();
	if (pid < 0 || (!pid && own_here_doc_while(fd, redi->data, env_list, redi->hdoc_is_quoted)))// ojo con esto nose si fucniona
		return (1);
	if (close(fd[1]) < 0)
		return (1);//ERROR!
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == 1)
	{
		close(fd[1]);
		return (1);//TENGO MIEDO YA VAN DEMASIADOS ERRORES
	}
	*fd_return = fd[0];
	return (0);
}

static bool	own_here_doc_while(int *fd, char *limitator, t_env *env_list, bool quoted_here)
{
	char	*line;

	if (close(fd[0]) < 0)
		return (1);//ERROR!
	while (42)
	{
		init_signals(HERE_DOC);
		line = readline("> ");
		init_signals(NO_INTERACTIVE);
		if (global.is_ctrlC || !line)
			exit (1);
		line = str_dollar_expander(line, quoted_here, env_list);
		if (!ft_strncmp(line, limitator, 0xffffffff))
			break ;
		if (write(fd[1], line, ft_strlen(line)) < 0 || write(fd[1], "\n", 1) < 0)
		{
			free(line);
			close(fd[1]);
			exit (1);//ERROR!
		}
		free(line);
	}
	if (line)
		free(line);
	if (close(fd[1]) < 0)
		exit (1);//ERROR!
	exit (0);
}
