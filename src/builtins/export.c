

#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

int is_duplicated_var(char *var, t_env **env)
{
	t_env *tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(var, tmp->line, ft_strlen(tmp->line)) 
			&& ft_strlen(var) == ft_strlen(tmp->line))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int equal_checker(char *s)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (s && s[i])
	{
		if (s[i] == '=')
			count++;
		i++;
	}
	if (count == 1)
		return (0);
	else
		return (1);
}

//ne pas afficher TOUTES les variables
//afficher _= si on fait un bash -c export | grep ?
//certaines variables sont marquees comme exportees ?
//export affiche toutes les variables exportees
//_ est pas une variable mais listee comme un charactere special 
//la commande set renvoie : _=env
//Faire un booleen dans env pour marquer les variables exportees et les autres ? 
//differencier env grep _= / echo $_
//
//Doc bash : 
// At shell startup set to the pathname used to invoke the shell or shell script being executed as passed in the environment or argument list
// + Expands to the last argument to the previous simple command executed in the foreground after expansion.
// Also set to the full pathname used to invoke each command executed and placd in the environment exported to that command
// When checking mailm this parameter holds the name of the mail file
// Si je fais VAR=var puis export : elle n'apparait pas MAIS echo $VAR fonctionne !
// Si je fais ENSUITE export VAR : elle apparait dans l'export 
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
			//declare -x toujours ?
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
			//a tester le equal_checker 
			if (!is_duplicated_var(arg[i], env) && !equal_checker(arg[i]))
				add_back(env, arg[i]);
			i++;
		}
	}
	return (0);
}
