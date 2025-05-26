
#include "env_utils.h"
#include "libft.h"
#include <errno.h>
#include <limits.h>
#include <unistd.h>

static int	handle_oldpwd(t_var **env, t_var **old_pwd)
{
	*old_pwd = is_known_key(env, "OLDPWD");
	if (!*old_pwd)
	{
		add_back_var(env, "OLDPWD=", 3);
		if (errno == ENOMEM)
			return (ENOMEM);
		*old_pwd = is_known_key(env, "OLDPWD");
	}
	return (0);
}

static int	handle_pwd(t_var **env, t_var **pwd)
{
	*pwd = is_known_key(env, "PWD");
	if (!*pwd)
	{
		add_back_var(env, "PWD=", 3);
		if (errno == ENOMEM)
			return (ENOMEM);
		*pwd = is_known_key(env, "PWD");
	}
	return (0);
}

static int	update_oldpwd(t_var *old_pwd, t_var *pwd)
{
	if (old_pwd->value)
		free(old_pwd->value);
	if (pwd->value)
		old_pwd->value = ft_strdup(pwd->value);
	else
		old_pwd->value = ft_strdup("");
	if (!old_pwd->value)
		return (ENOMEM);
	return (0);
}

static int	update_pwd(t_var *pwd)
{
	char	buf[PATH_MAX];

	if (getcwd(buf, sizeof(buf)) != NULL)
	{
		if (pwd->value)
			free(pwd->value);
		pwd->value = ft_strdup(buf);
		if (!pwd->value)
			return (ENOMEM);
	}
	else if (errno == ENOENT)
	{
		ft_putstr_fd("cd: error retrieving current directory: " \
			"getcwd: cannot access parent directories: " \
			"No such file or directory\n", 2);
		pwd->value = ft_strjoin(pwd->value, "/..");
		if (!pwd->value)
			return (ENOMEM);
		return (1);
	}
	return (0);
}

int	update_env(t_var **env)
{
	t_var	*old_pwd;
	t_var	*pwd;
	int		exit_code;

	exit_code = handle_oldpwd(env, &old_pwd);
	if (exit_code != 0)
		return (exit_code);
	exit_code = handle_pwd(env, &pwd);
	if (exit_code != 0)
		return (exit_code);
	exit_code = update_oldpwd(old_pwd, pwd);
	if (exit_code != 0)
		return (exit_code);
	return (update_pwd(pwd));
}
