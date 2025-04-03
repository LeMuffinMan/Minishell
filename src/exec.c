/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:49:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/03 17:57:58 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

int join_args(char *s1, char *s2, char *old_str)
{
  char *new_str;
	int		i;
	int		j;

	i = 0;
	new_str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (new_str == NULL)
		return (NULL);
	while (s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		new_str[i] = s2[j];
		i++;
		j++;
	}
	new_str[i] = '\0';
  free(old_str);
  return (new_str);
}

int	builtin_echo(t_tree **tree)
{
	// booleen ?
	int	option;
	int	i;
	char *s;

	option = 0;
	i = 1;
	// Si on a comme 1er param le -n : skip pour trouver des str a afficher
	if (!ft_strncmp(tree->arg[i], "-n", 2) && ft_strlen(tree->arg[i]) == 2)
	}
		option = 1;
	  i++;
  {
	while (tree->arg[i])
	{
		// si on a deja un -n, on skip tout ceux qui suivent
		if (option == 1)
		{
			while (tree->arg[i] && !ft_strncmp(tree->arg[i], "-n", 2)
				&& ft_strlen(tree->arg[1]) == 2)
				i++;
		}
		//on join tous les args avec un space entre chaque, sauf le dernier
    while (tree->arg[i] && tree->arg[i + 1])
	  {
	    s = join_args(tree->arg[i], " ")
      i++;
	  }
	  //si on n'avait pas de -n : on join le \n
	  if (option == 0)
	    s = join_args(tree->arg[i], "\n");
	  //si on dup2 au tout debut, ici 1 ecrira bien ou je vuex ?
		ft_putstr_fd(s, 1);
	}
	return (0);
}

  //des cas particuliers ?
int builtin_pwd(char **arg)
{
  if (arg[1])
  {
    ft_putstr_fd("Error : pwd does not support options\n", 1);
    //echo $? pour un pwd -e en bash renvoie exit code = 2
    return (2);
  }
  s = getcwd();
  ft_putstr_fd(s, 1);
  free(s);
  return (0);
}

int	exec(t_tree **tree)
{
	if (tree->arg[0] == "echo")
		builtin_echo();
	else if (tree->arg[0] == "cd")
		builtin_cd();
	else if (tree->arg[0] == "pwd")
		builtin_pwd();
	else if (tree->arg[0] == "export")
		builtin_export();
	else if (tree->arg[0] == "unset")
		builtin_unset();
	else if (tree->arg[0] == "env")
		builtin_env();
	else if (tree->arg[0] == "exit")
		builtin_exit();
	else
		printf("error : not a builtin\n");
}
