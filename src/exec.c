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

#include "libft.h"
#include "minishell.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int should_swap(t_env *tmp, t_env *next)
{
  int longest_len;

  if (ft_strlen(tmp->line) > ft_strlen(next->line))
    longest_len = ft_strlen(tmp->line);
  else
    longest_len = ft_strlen(next->line);

  if (ft_strncmp(tmp->line, next->line, longest_len) > 0)
    return (1);
  return (0);
}

void swap_lines(t_env *tmp, t_env *next)
{
  char *temp;

  temp = tmp->line;
  tmp->line = next->line;
  next->line = temp;
}

int sort_env(t_env **env)
{
  int swapped;
  t_env *tmp;

  if (!env || !(*env))
    return (1);

  swapped = 1;
  while (swapped)
  {
    swapped = 0;
    tmp = *env;
    while (tmp && tmp->next)
    {
      if (should_swap(tmp, tmp->next))
      {
        swap_lines(tmp, tmp->next);
        swapped = 1;
        tmp = *env;
      }
      tmp = tmp->next;
    }
  }
  return (0);
}

void join_strs(char *line, va_list strs)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	str = va_arg(strs, char *);
	while (str)
	{
		j = 0;
		while (str[j])
			line[i++] = str[j++];
		str = va_arg(strs, char *);
	}
	line[i] = '\0';
}


char *build_line(char *s, ...)
{
	va_list	strs;
	int		line_len;
	char	*line;
	char	*str;

	line_len = 0;
	va_start(strs, s);
	str = va_arg(strs, char *);
	while (str)
	{
		line_len += ft_strlen(str);
		str = va_arg(strs, char *);
	}
	va_end(strs);
	line = malloc(sizeof(char) * (line_len + 1));
	if (!line)
		return (NULL);
	va_start(strs, s);
	join_strs(line, strs);
	va_end(strs);

	return (line);
}

// tests :
// echo
// echo -n
// echo -n -n -n
// echo -n hello
// echo -n -n hello
// echo hello
int	builtin_echo(char **arg)
{
	int		option;
	int		i;
	char	*s;

	// booleen ?
	s = NULL;
	option = 0;
	if (!arg[1])
	{
		ft_putstr_fd("\n", 1);
		return (0);
	}
	i = 1;
	// Si on a comme 1er param le -n : skip pour trouver des str a afficher
	if (!ft_strncmp(arg[i], "-n", 2) && ft_strlen(arg[i]) == 2)
	{
		option = 1;
		i++;
	}
	// si on a deja un -n, on skip tout ceux qui suivent
	if (option == 1)
	{
		while (arg[i] && !ft_strncmp(arg[i], "-n", 2) && ft_strlen(arg[1]) == 2)
			i++;
	}
	// on join tous les args avec un space entre chaque, sauf le dernier
	while (arg[i])
	{
		s = build_line(s, arg[i], s);
		if (arg[i + 1])
			s = build_line(s, " ", s);
		i++;
	}
	// si on n'avait pas de -n : on join le \n
	if (option == 0)
		s = build_line(s, "\n", s);
	// si on dup2 au tout debut, ici 1 ecrira bien ou je vuex ?
	if (s)
		ft_putstr_fd(s, 1);
	free(s);
	return (0);
}

// tester en unsetant PWD ...
int	builtin_pwd(void)
{
	char	buf[PATH_MAX];
	char	*s;

	s = NULL;
	if (getcwd(buf, sizeof(buf)) != NULL)
	{
		s = build_line(buf, "\n", s);
		ft_putstr_fd(s, 1);
		free(s);
		return (0);
	}
	else
	{
		perror("pwd");
		//  1 ou errno ?
		return (1);
	}
	return (0);
}

int is_only_numeric_argument(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] > '9' || s[i] < '0')
			return (0);
		i++;
	}
	return (1);
}

// a voir
int	builtin_exit(char **arg, t_env **env)
{
	int	n;
	char *s;

	s = NULL;
	if (!arg[1])
	{
		free_list(env); //double free ?
		exit(0);
	}
	if (arg[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	//a tester
	if (!is_only_numeric_argument(arg[1]))
	{
		s = build_line(NULL, "minishell: exit: ", arg[1], ": numeric argument required\n", NULL);
		ft_putstr_fd(s, 1);
		free(s);
		/* free_list(env); */ // segfault ?
		exit (2);
	}
	//Si on a autre chose que des digits : 
	//message d'erreur + retour d'erreur 2
	n = ft_atoi(arg[1]);
	if (n > 255)
		n = n % 256;
	exit(n);
	return (0);
}

//ajouter l'acces a des dossiers non permis ?
//cd dans un fichier ?
int	builtin_cd(char **arg)
{
	char	*s;

	/* char *new_path; */
	/* char	buf[PATH_MAX]; */
	/* new_path = NULL; */
	s = NULL;
	if (!arg[1])
	{
		// si HOME existe : on chdir(HOME)
		// si non :
		ft_putstr_fd("minishell: cd: HOME not set", 1);
		return (1);
	}
	if (arg[1][0] && arg[1][0] == '/') // Si on chere cd ~ on doit adapter ca
	{
		if (access(arg[1], F_OK) == 0)
			chdir(arg[1]); // a proteger !
		else
		{
			s = build_line("minishell: cd: ", arg[1], s);
			s = build_line(s, ": No such file or directory\n", s);
			ft_putstr_fd(s, 2);
			free(s);
			return (1);
		}
	}
	else if (arg[1][0]) // on est sur un chemin relatif 
	{
		if (access(arg[1], F_OK) == 0)
			chdir(arg[1]); // a proteger !
	}
	/* free(new_path); */
	return (0);
}

int builtin_env(t_env **env)
{
	t_env *tmp;

	tmp = *env;
	/* if (!env) */
	/* PWD=/home/muffin */
	/* SHLVL=1 */
	/* _=/usr/bin/env */
	while (tmp)
	{
		printf("%s\n", tmp->line);
		tmp = tmp->next;
	}
	return (0);
}

int builtin_unset(t_env **env, char **arg)
{
	t_env *tmp;
	int i;

	i = 1;
	while (arg[i])
	{
		tmp = *env;
		while (tmp)
		{
			if (ft_strncmp(arg[i], tmp->line, ft_strlen(arg[i])) == 0 &&
    		tmp->line[ft_strlen(arg[i])] == '=')
				{
					if (tmp->prev)
						tmp->prev->next = tmp->next;
					if (tmp->next)
						tmp->next->prev = tmp->prev;
					free(tmp->line);
					free(tmp);
					break;
				}
				else
					tmp = tmp->next;
		}
		i++;
	}
	return (0);
}


int builtin_export(t_env **env, char **arg)
{
	t_env *tmp;
	char *s;
	int i;
	char **line;
	
	s = NULL;
	line = NULL;
	tmp = *env;
	//LS_COLOR et la ligne longue disparait ??
	sort_env(&tmp);
	if (!arg[1]) //export sans arguments affiche l'env avec declare + x au debut de chaque lignes 
	{
		while (tmp)
		{
			line = ft_split(tmp->line, '=');
			/* if (line[1]) comment ca se pass si PWD= ? */
			s = build_line(NULL, "declare -x ", line[0], "=\"", line[1], "\"\n", NULL);
			ft_putstr_fd(s, 1);
			tmp = tmp->next;
			free(s);
			s = NULL;
			ft_free(line);
			line = NULL;
		}
		if (s)
			free(s);
		if (line)
			ft_free(line);
		return (0);
	}
	else 
	{
		i = 1;
		while (arg[i])
		{
			add_back(env, arg[i]);
			i++;
		}
	}
	while(tmp->next)
		tmp = tmp->next;
	//ajouter un maillon a env, avec la string var
	return (0);
}

int	exec(char **arg, t_env **env)
{
	/* faire un switch case et y mettre les returns */
	if (!*arg)
		return (0);
	else if (!ft_strncmp(arg[0], "echo", 4))
		return(builtin_echo(arg));
	else if (!ft_strncmp(arg[0], "cd", 2))
		return(builtin_cd(arg));
	else if (!ft_strncmp(arg[0], "pwd", 3))
		return(builtin_pwd());
	else if (!ft_strncmp(arg[0], "export", 6))
		return (builtin_export(env, arg));
	else if (!ft_strncmp(arg[0], "unset", 5))
		return(builtin_unset(env, arg));
	else if (!ft_strncmp(arg[0], "env", 3))
		return (builtin_env(env));
	else if (!ft_strncmp(arg[0], "exit", 4))
		return(builtin_exit(arg, env));
	else // pas printf ! utiliser fd_putstr_fd
		printf("error : not a builtin\n");
	return (0);
}
