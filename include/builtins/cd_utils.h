
#ifndef CD_UTILS_H
# define CD_UTILS_H

int	change_directory(char *path);
int	builtin_cd_without_arg(t_var **env);
int find_last_slash(char *buf);
int trim_pwd(char **s);

#endif
