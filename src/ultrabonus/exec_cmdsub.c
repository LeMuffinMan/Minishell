
int find_cmd_sub(char *s)
{
	int i;

	i = 0;
	while (i < ft_strlen(s) - 2)
	{
		if (s[i] == '$' && s[i + 1] == '(')
		{
			if (find_last_parenthesis(s + i + 2))
				return(1);
			else 
				return(-1); //ou ouvrir un here doc avec une ) comme sep !
		}
		i++;
	}
	return (0);
}

int read_from_pipe(char *output)
{
  char *tmp;
  char *line;

  line = get_next_line(0);
  output = ft_strdup("");
  while (line)
  {
    tmp = output;
    output = ft_strjoin(output, line);
    free(tmp);
    free(line);
    line = NULL;
    line = get_next_line(0);
  }
  return(0);
}

int get_cmd_sub_output(char *cmd, char *output)
{
	pid_t pid_1;
	pid_t pid_2;
	int pipefd;
	char *output;

	if(pipe(pipefd) == -1)
	{
		//error
	}
	pid_1 = fork();
	if (pid_1 == -1)
	{
		//error
	}
	if (pid_1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit (new_exec(cmd));
	}
	else
		close(pipefd[1]);

	pid_2 = fork();
	if (pid_2 == -1)
	{
		//error
	}
	if (pid_2== 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exit(read_from_pipe(output));
		//faire un get_next_line sur la STDIN
	}
	else
	{
		close(pipefd[0]);
		return (wait_children());
	}
}

char *replace_cmd_sub(char *s, char *cmd_output)
{
	int start;
	char *begin;
	char *end;
	char *tmp;
	char *new_str;

	start = 0;
	while(s[start] != '$')
		start++;
	begin = malloc(sizeof(char) * start + 1);
	ft_strlcpy(s, begin, start);
	new_str = ft_strjoin(begin, cmd_output);
	free(begin);
	while(s[start] != ')')
		start++;
	end = s[start + 1];
	tmp = new_str;
	new_str = ft_strjoin(new_str, s[end]);
	free(tmp);
	return (new_str);
}

int expand_cmd_sub(char **content, t_var **env)
{
	int i;
	char *output;

	i = 0;
	while (content[i])
	{
		while (find_cmd_sub(content[i]))
		{
			get_cmd_sub_output(cmd, output);
			tmp = content[i];
			content[i] = replace_cmd_sub(content[i], output);
			free(tmp);
		}
		i++;
	}
}
