/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directories.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoll-pe <bmoll-pe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 19:52:00 by bmoll-pe          #+#    #+#             */
/*   Updated: 2023/03/31 14:27:05 by bmoll-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "utils.h"
#include "env.h"
#include "builtin_utils.h"

int	exec_pwd(t_node *node)
{
	char	*buff;

	if (node->tokens[1])
		return (print_error(NULL, 0, 1));
	buff = ft_calloc(PATH_MAX + 1, 1);
	if (!getcwd(buff, PATH_MAX))
		return (print_error(node->tokens[1], 0, 1));
	if (ft_printf("%s\n", buff) == -1)
	{
		free(buff);
		buff = NULL;
		return (print_error(NULL, 0, 1));
	}
	free(buff);
	buff = NULL;
	return (0);
}

int	exec_cd(t_master *master, t_node *node)
{
	bool	err;
	char	*old_pwd;
	char	*pwd;

	err = false;
	global.num_return_error = 1;
	if (env_search(master->env_list, "PWD"))
		old_pwd = env_get_value(master->env_list, "PWD");
	else
		old_pwd = get_current_pwd();
	if (!old_pwd)
		return (print_error(NULL, 0, 1));
	if (node->tokens[0] && !node->tokens[1] && !env_search(master->env_list, "HOME"))
		return (print_error(ft_strdup("cd: HOME not set"), 1, 1));
	if (node->tokens[0] && !node->tokens[1])
		pwd = env_get_value(master->env_list, "HOME");
	else
		pwd = node->tokens[1];
	if (pwd && *pwd && chdir(pwd) == -1)
		err = true;
	if (err)
		return (print_error(ft_strjoin("cd: ",pwd), 0, 1));
	else
	{
		if (env_change_value(master->env_list, "OLDPWD", old_pwd))
			return (print_error(NULL, 0, 1));
		if (env_change_value(master->env_list, "PWD", get_current_pwd()))
			return (print_error(NULL, 0, 1));
	}
	global.num_return_error = 0;
	return (0);
}
