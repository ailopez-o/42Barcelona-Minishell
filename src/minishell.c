/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aitoraudicana <aitoraudicana@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/01/29 15:21:24 by aitoraudica      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "structs.h"
#include "minishell.h"
#include "bmlib.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

static void	init_master(t_master *master, char **env);
char	*expander(char *data, t_master *master);



int	main(int argc, char **argv, char **env)
{
	t_master	master;
	char		*line;
	//char		*expanded;
	char		**tokens;
	// int			fd;
	// char		*gnl;

	// (void) expanded;
	// fd = open("cmd.txt", O_RDONLY);
	// gnl = get_next_line(fd);
	// close(fd);
	// expanded = expand_data(gnl);

	
	(void) tokens;
	(void)argv;
	ft_bzero(&master, sizeof(t_master));
	if (argc != 1)
		return (0);
	init_master(&master, env);
	while (1)
	{
		line = readline("\033[38;5;143mba.sh $ \033[0;39m");
		if (!line)
		{
			system("leaks minishell");
			exit(1);
		}
		if (line [0])
		{
			add_history(line);
			if (!syntax_check(line))
			{
				if (parser(&master.node, line, 1))
					error("ba.sh: error parsing input\n", 1);
				execute_command(&master, master.node);
				//develop(&master.node);
				executor(&master, master.node);
				master.node = free_tree(master.node);
			}
			else
			{
				free(line);
				write(2, "ba.sh: syntax error near unexpected token\n", 42);
				// falta que se quede en la variable exit code el numero 258
			}
		}
	}
	env_free_list(master.env_list);
	return (0);
}

static void	init_master(t_master *master, char **env)
{
	master->path = NULL;
	if (*env)
	{
		master->env_list = env_parser(env);
		master->tild_value = env_get_value(master->env_list, "HOME");
		add_bash_lvl(master, env_search(master->env_list, "SHLVL"));
		if (!master->tild_value)
			master->tild_value = ft_substr("/Users/UserID", 0, 14);// en este caso y...
		if (!master->tild_value)
			exit (1);// error de memoria exit el que sea
	}
	else
	{
		ft_printf("no hay env!\n");
		default_env(master);
		// master->env_list = NULL;
		master->tild_value = ft_substr("/Users/UserID", 0, 14);// en este, hay que hacer una funcion para calcular el valor
	}
  //ft_printf("tilde value: ->%s<-\n", master->tild_value);
}

void	develop(t_node **node)// no entiendo esta funcion
{
	print_parse_tree(*node);
}

t_node	*free_tree(t_node *node)
{
	t_node	*temp;

	while (node)
	{
		if (node->child)
			free_tree(node->child);
		temp = node->next;
		free (node->data);
		if (node->tokens)
			free_split(node->tokens);
		free (node);
		node = temp;
	}
	return (NULL);
}

void	error(char *error, int num_error)
{
	// perror(error);
	write(2, error, ft_strlen(error));
	//ft_putstr_fd(2, error);
	exit(num_error);
}
