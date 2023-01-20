/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: test <test@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 23:39:59 by bmoll-pe          #+#    #+#             */
/*   Updated: 2023/01/20 13:28:27 by test             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bmlib.h"
#include "structs.h"
#include "minishell.h"

static int	check_cmd_while(t_master *master, char **cmd);

char	*check_cmd(t_master *master, t_node *node)
{
	char	*cmd;
	char	*tmp;

	cmd = node->tokens[0];
	if (master->path)
	{
		tmp = ft_strjoin("/\0", node->tokens[0]);
		if (!tmp)
			error("ba.sh: memory alloc error\n", 1);
		if (check_cmd_while(master, &tmp))
			return (tmp);
		free(tmp);
	}
	if (access(cmd, F_OK) || !ft_strrchr(cmd, '/'))
		error("ba.sh: Command not found 1\n", 1);
		// exit (clean_exit(pip, error_msg(PPX, cmd, CNF, 127)));
	if (access(cmd, X_OK))
		error("ba.sh: permission deneied", 1);
		// exit (clean_exit(pip, error_msg(BSH, cmd, PMD, 126)));
	return (cmd);
}

static int	check_cmd_while(t_master *master, char **cmd)
{
	size_t	iter;
	char	*tmp;

	iter = 0;
	while (master->path[iter])
	{
		tmp = ft_strjoin(master->path[iter], *cmd);
		if (!tmp)
			error("ba.sh: memory alloc error\n", 1);
		if (!access(tmp, F_OK))
		{
			if (!access(tmp, X_OK))
				break ;
			free(tmp);
			error("ba.sh: permission denied\n", 1);
		}
		free(tmp);
		iter++;
	}
	if (!master->path[iter])
		return (0);
	free(*cmd);
	*cmd = tmp;
	return (1);
}