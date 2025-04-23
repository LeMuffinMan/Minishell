
void	delete_loaded_variables(t_var **env, void (*del)(void *))
{
  t_var *tmp;

  tmp = *env;
  *env = (*env)->next;
  tmp->next = tmp->next->next;
	(*del)((*env)->key);
	(*del)((*env)->value);
	free(*env);
	return (0);
}

int builtin_source(t_var **env)
{
  t_var *tmp;

  tmp = *env;
  while (tmp)
  {
    if (tmp->next->loaded == 1)
      delete_loaded_variables(tmp, &free)
    tmp = tmp->next;
  }
  load_minishell_rc(env);
  return (0);
}
