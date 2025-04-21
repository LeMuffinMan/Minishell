
char **build_new_array(char **array_to_update, char *alias_value, int alias_size, int line_size)
{
  char **new_array;
  char **alias_array;
  int i;

  new_array = malloc(sizeof(char *) * (alias_size + line_size);
  if (!new_array)
  {

  }
  new_array[0] = ft_strdup(array_to_update[0]);
  alias_array = ft_split(alias_value, ' ');
  i = 1;
  while (alias_array[i])
  {
    new_array[i] = alias_array[i];
    i++;
  }
  j = 1;
  while (array_to_update[j])
  {
    new_array[i] = array_to_update[j];
    i++;
    j++;
  }
  new_array[i] = NULL;
  return (new_array);
}

int count_words(char *s, char sep)
{
  int i;
  int words;
  int in_word;

  i = 0;
  words = 0;
  in_word = 0;
  while(s && s[i])
  {
    if (s[i] != sep)
      in_word = 1;
    if (in_word == 1 && s[i] == sep)
    {
      in_word = 0;
      words++;
    }
    i++;
  }
  return (words);
}

int array_size(char **array)
{
  int i;

  i = 0;
  while (array && array[i])
    i++;
  return (i);
}

char *get_alias_value(char *line, t_var **aliases)
{
  t_var *tmp;

  tmp = *aliases;
  while(tmp)
  {
    if (!ft_strncmp(line, tmp->value, ft_strlen(tmp->value) + 1))
      return (tmp->value);
    tmp = tmp->next;
  }
  return(NULL);
}


char **expand_alias(char **content, t_var **aliases)
{
  char **new_array;
  char *alias_value;
  int alias_size;
  int line_size;

  alias_size = 0;
  line_size = 0;
  new_array = NULL;
  if (*content && content[0])
  {
    if (is_known_alias(content[0]))
    {
      alias_value = get_alias_value(content[0], aliases);
      line_size = array_size(content);
      alias_size = count_words(alias_value, ' ');
      new_array = build_new_array(content, alias_value, alias_size, line_size)
    }
  }
  return (new_array);
}

int main(char **argv, char **env)
{
  char **new_array;
  new_array = expand_alias(argv, env); 

  return (0);
}
