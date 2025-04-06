
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

char *get_home_path(t_env **env)
{
	t_env *tmp;
	char *path;

	tmp = *env;
	path = NULL;
	while (tmp)
	{
		if (!ft_strncmp("HOME=", tmp->line, 5) && ft_strlen(tmp->line) > 5)
		{
			path = tmp->line + 5;
			break ;
		}
		tmp = tmp->next;
	}
	return (path);
}

char *get_pwd_from_env(t_env **env)
{
	t_env *tmp;

	while (tmp)
	{
		if (!ft_strncmp("PWD=", tmp->line, ft_strlen(tmp->line)))
			break;
		tmp = tmp->next;
	}
	//si on a trouve PWD dans l'env
	if (tmp)
		return (tmp->line);
	else //pas sur du tout de ce qui suit 
		return (NULL); //Si null c'est qu'on n'a pas trouve
}

int update_old_pwd(char *s)
{
	//Si PWD n'existe pas 
	if (!updated_var)
	{

	}
}

//env -i bash
//
//env 
//cd ..
//env
//==> OLDPWD existe et est a jour
//env
//unset PWD
//env
//==>Ne doit afficher ni PWD ni OLD_PWD
//env
//unset PWD
//export PWD=lol
//pwd
//env
//==>ne change PAS la variable PWD 
//cd ..
//env 
//==>PWD est a jour et OLD_PWD = lo,
//
//
//Quand on fait cd :
//Si la variable n'existe pas : on annule la mise a jour
//si elle existe, on la met a jour, et on update / cree OLD_PWD
//Si PWD est declaree en locale mais pas exportee : on cree OLDPWD a partir de la varialbe localem mais on ne l'exporte pas PWD
//Dans tous les cas, un echo $PWD doit afficher le path actuel meme si pas exportee
int change_directory_and_update_env(t_env **env, char *path)
{
	char	buf[PATH_MAX];
	char *old_pwd;

	old_pwd = get_pwd_from_env(env);
	if (chdir(s) == 0)
	{
		update_old_pwd(old_pwd);
		getcwd(buf, sizeof(buf));
		//securiser !!
		update_env_var("PWD=", buf);
	}
	else
	{
		//dossier introuvable / pas de permissions
	}
}
//ajouter l'acces a des dossiers non permis ?
//cd dans un fichier ?
//mkdir -p a/b/c && cd $_
int	builtin_cd(char **arg, t_env **env)
{
	char	*s;

	/* char *new_path; */
	/* char	buf[PATH_MAX]; */
	/* new_path = NULL; */
	if (!arg[1])
	{
		s = get_home_path(env); // revoir ca
		if (!s)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 1);
			return (1);
		}
		else
			chdir(s);
		return (0);
	}
	s = ft_strdup("");
	if (arg[1][0] && arg[1][0] == '/') // Si on chere cd ~ on doit adapter ca
	{
		if (access(arg[1], F_OK) == 0)
			chdir(arg[1]); // a proteger !
		else
		{
			s = ft_strjoin(s, "minishell: cd: ", s);
			s = ft_strjoin(s, arg[1], s);
			s = ft_strjoin(s, ": No such file or directory\n", s);
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
	free(s);
	return (0);
}
