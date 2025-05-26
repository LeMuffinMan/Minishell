
#include <errno.h>
#include "malloc_error_handlers.h"
#include "exec.h"
#include "libft.h"
#include "signals.h"
#include "utils.h"
#include <unistd.h>

#include <stdio.h>
int exec_boolop(t_tree **ast, t_lists *lists)
{
	int exit_code;

	exit_code = 0;
	/* if ((*ast)->left->token->token == GROUP_PARENTHESIS) */
	/* 	dprintf(2, "(*ast)->left token = GROUP_PARENTHESIS\n"); */
	/* if ((*ast)->left->token->token == GROUP_BOOLOP) */
	/* 	dprintf(2, "(*ast)->left token = GROUP_BOOLOP\n"); */
	/* if ((*ast)->right->token->token == GROUP_PARENTHESIS) */
	/* 	dprintf(2, "(*ast)->right token = GROUP_PARENTHESIS\n"); */
	/* if ((*ast)->right->token->token == GROUP_BOOLOP) */
	/* 	dprintf(2, "(*ast)->right token = GROUP_BOOLOP\n"); */
	if ((*ast)->left)
	{
		exit_code = exec_ast(&((*ast)->left), lists);
		/* dprintf(2, "exit_code left = %d\n", exit_code); */
		if (errno == ENOMEM)
			return (errno);
		if ((exit_code == 0 && (*ast)->token->token == O_AND) || 
			(exit_code != 0 && (*ast)->token->token == O_OR))
		{
			exit_code = exec_ast(&((*ast)->right), lists);
			/* dprintf(2, "exit_code right = %d\n", exit_code); */
			if (errno == ENOMEM)
				return (errno);
			return (exit_code);
		}
		else
			return (exit_code);
	}
	return (1); //cas ou left existe pas dans un node AND
}

int  exec_parenthesis_child(t_tree **ast, t_lists *lists)
{
	t_tree *sub_ast;
	int exit_code;

	exit_code = 0;
	if (close_origin_fds(lists->origin_fds) == -1)
		malloc_error_parenthesis_child(lists, NULL);
	setup_child_signals();
	sub_ast = parse((*ast)->token->group->content[0], *lists->env, lists);
	if (errno == ENOMEM)
		malloc_error_parenthesis_child(lists, NULL);
	exit_code = exec_ast(&sub_ast, lists);
	if (errno == ENOMEM)
		malloc_error_parenthesis_child(lists, &sub_ast);
	free_tree(&sub_ast);
	free_lists(lists);
	exit (exit_code);
}

int exec_parenthesis(t_tree **ast, t_lists *lists)
{
	pid_t pid;

	struct sigaction sa_ignore, sa_orig;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, &sa_orig);
	pid = fork();
	if (pid < 0)
		return (errno);
	if (pid == 0)
	{
		exec_parenthesis_child(ast, lists);
	}
	else
	{
		lists->exit_code = wait_children(pid, pid);
		if (errno == ENOMEM)
			return (errno);
		sigaction(SIGINT, &sa_orig, NULL);
		return (lists->exit_code);
	}
	return (1); // cas impossible ?
}


int exec_group_cmd(t_tree **ast, t_lists *lists)
{
	if ((*ast)->token->token == R_IN || (*ast)->token->token == APPEND
		|| (*ast)->token->token == TRUNC || (*ast)->token->token == HD)
		return (redirect_stdio(ast, lists));
	if ((*ast)->token->token == PIPE)
		return (exec_pipe(ast, lists));
	if ((*ast)->token->error != 126 && ((*ast)->token->token == BUILT_IN || (*ast)->token->token == CMD
		|| !ft_strncmp((*ast)->token->content[0], "source", 7))) // exclure les codes d'erreurs
	{
		lists->exit_code = exec_cmd(ast, lists);
		return (lists->exit_code);
	}
	return (-1); // impossible d'arriver ici : quelle code d'erreur ?
}

int exec_group_boolop(t_tree **ast, t_lists *lists)
{
	t_tree *sub_ast;

	sub_ast = NULL;
	sub_ast = parse((*ast)->token->content[0], *lists->env, lists);
	lists->exit_code = exec_ast(&sub_ast, lists);
	free_tree(&sub_ast);
	return (lists->exit_code);
}

