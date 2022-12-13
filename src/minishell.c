/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoll-pe <bmoll-pe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 13:06:45 by bruno             #+#    #+#             */
/*   Updated: 2022/12/13 23:40:21 by bmoll-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_master	master;
	t_bracket	*bracket;
	if (argc != 1)
		return (0);
	master.env = env;
	master.exit_code = 0;
	while (1)
	{
		if (parser(&master, &bracket, readline("ba.sh $ ")))
			return (1);
		// printf("->%s<-\n", input);
	}
	(void)argv;
	(void)env;
	exit (master.exit_code);
}