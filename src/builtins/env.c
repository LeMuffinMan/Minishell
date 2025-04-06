
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

/* static int should_swap(t_env *tmp, t_env *next) */
/* { */
/*   int longest_len; */
/**/
/*   if (ft_strlen(tmp->line) > ft_strlen(next->line)) */
/*     longest_len = ft_strlen(tmp->line); */
/*   else */
/*     longest_len = ft_strlen(next->line); */
/**/
/*   if (ft_strncmp(tmp->line, next->line, longest_len) > 0) */
/*     return (1); */
/*   return (0); */
/* } */
/**/
/* static void swap_lines(t_env *tmp, t_env *next) */
/* { */
/*   char *temp; */
/**/
/*   temp = tmp->line; */
/*   tmp->line = next->line; */
/*   next->line = temp; */
/* } */

/* int sort_env(t_env **env) */
/* { */
/*   int swapped; */
/*   t_env *tmp; */
/**/
/*   if (!env || !(*env)) */
/*     return (1); */
/**/
/*   swapped = 1; */
/*   while (swapped) */
/*   { */
/*     swapped = 0; */
/*     tmp = *env; */
/*     while (tmp && tmp->next) */
/*     { */
/*       if (should_swap(tmp, tmp->next)) */
/*       { */
/*         swap_lines(tmp, tmp->next); */
/*         swapped = 1; */
/*         tmp = *env; */
/*       } */
/*       tmp = tmp->next; */
/*     } */
/*   } */
/*   return (0); */
/* } */

//ne pas afficher les variables non exportees ? ou justement on affiche tout ?
//NE PAS AFFICHER LES VARIABLES SANS VALEUR !
int builtin_env(t_env **env)
{
	t_env *tmp;

	if (!*env)
	  return (0); // 0 ?
	tmp = *env;
	while (tmp)
	{
	  #include <stdio.h>
	  /* printf("%s %s\n", tmp->value, tmp->key); */
	  if (tmp->value && tmp->env == 1)
	  {
			ft_putstr_fd(tmp->key, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(tmp->value, 1);
			ft_putstr_fd("\n", 1);
		}
	    /* s = build_line(s, tmp->key, "=", tmp->value, "\n"); */
		tmp = tmp->next;
	}
	return (0);
}
