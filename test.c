/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 14:59:12 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/16 17:28:59 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef enum e_seq_type
{
  O_AND,
  O_OR,
  SEQ
} t_seq_type;

typedef struct s_seq
{
  t_seq_type token;
  char *content;
  int sub_shell;
  struct s_seq *left;
  struct s_seq *right;
} t_seq;


//return 0 si on n'est pas sur un boolop valide : & a&& &&a etc ...
//return 1 si le boolop est valide
//return 2 pour un syntax error ?
int is_a_valid_boolop(char *str, int i)
{
  if ((str[i] == '&' && str[i + 1] && str[i] == str[i + 1])
  || (str[i] == '|' && str[i + 1] && str[i] == str[i + 1])) //verifier les char avant apres aussi !!
  {
    if (i == 0) //si le && est au tout debut de la string : syntax error 
      return (0);
    else if (i > 0)
    {
      if (str[i - 1] == ' ' || str[i - 1] == '(' || str[i - 1] == ')' || str[i - 1] == '\"' || str[i - 1] == '\'')
      {

        return (1);
      }
      else
      {
        return (2); // syntax erorr ?
      }
    }
  }
  return (0);
}

//i == 0 : on est sur un bool op ou la ligne est vide : un bool op en debut de ligne est forcement syntax error
//i == ft_strlen(i) : on est arrive a la fin de la ligne
//else
//on a trouve l'operateur a mettre en haut de l'arbre
//on doit lui mettre un end en parametre
int get_next_door(char *str, int i, int end) //ajouter un end ici !!!
{
  int parenthesis;
  int double_quoted;
  int single_quoted;

  single_quoted = 0;
  double_quoted = 0;
  parenthesis = 0;
  while (i < end) //i < end
  {
    if (str[i] == '\"')
    {
      if (double_quoted == 1)
        double_quoted = 0;
      else
        double_quoted = 1;
    }
    if (str[i] == '\'')
    {
      if (single_quoted == 0)
        single_quoted = 1;
      else
        single_quoted = 0;
    }
    printf("i = %d\n", i);
    //on a trouve le premier operateur en haut de l'arbre
    if (is_a_valid_boolop(str, i) && parenthesis == 0 && double_quoted == 0 && single_quoted == 0)
      return (i);
    i++;
  }
  return (i);
}

//add seq
      /* node->token = SEQ; */
      /* node->content = ft_strndup(str, i); */
      /**/
      /* node->token = SEQ; */
      /* node->content = ft_strndup(str + i + 2, stop); */
    //


//syntax error des quotes ici ?
//cherche le bool op a mettre tout en haut de l'arbre : le dernier qui n'est pas entre paremtheses 
//return (1) : on a trouve le main boolop
//return (2) : on a une syntax error
//retrun (0) : ce n'est pas encore le main boolop
int is_main_bool_op(char *str, int i, int end)
{
  int parenthesis;
  int double_quoted;
  int single_quoted;

  double_quoted = 0;
  single_quoted = 0;
  parenthesis = 0;
  if (is_a_valid_boolop(str, i))
  {
    i += 2;
    printf("i = %d | end = %d\n", i, end);
    while(i < end)
    {
      if (str[i] == '(')
        parenthesis++;
      if (str[i] == ')')
        parenthesis--;
      if (str[i] == '\"')
      {
        if (double_quoted == 0)
          double_quoted = 1;
        else
          double_quoted = 0;

      }
      if (str[i] == '\'')
      {
        if (single_quoted == 0)
          single_quoted = 1;
        else
          single_quoted = 0;
      }
      printf("str[%d] = %c | is_a_valid_boolop = %d | parenthesis = %d | single_quoted = %d | double_quoted = %d\n", i, str[i], is_a_valid_boolop(str, i), parenthesis, single_quoted, double_quoted);
      if ((str[i] == '&' || str[i] == '|') && is_a_valid_boolop(str, i) && parenthesis == 0 && single_quoted == 0 && double_quoted == 0)
      {
        i += 2; // a revoir l'incrementation de i ici
        printf("is_main_bool_op = %d\n", get_next_door(str, i, end));
        if (get_next_door(str, i, end) == end)
          return (1);
      }
      else
        i++;
    }
    if (i == end)
      return (2) ; //ici, soit on a aucun operateur hors parentheses : donc syntax error / soit on a ni boolop ni parnetheses : donc direct au parser normal
    if (get_next_door(str, i, end) == end) //si on a atteint le bout de line sans trouver d'operateur 
      return (1);
    else
      return (0);
  }
  return (0);
}


//return (0) : ce n'est pas un subshell
//return (1) : c'est un subshell
//return (2) : c'est une syntax error
int is_a_subshell_left(char *str, int end)
{
  int sub_parenthesis;

  sub_parenthesis = 0;
  while (end > 0)
  {
    if (str[end] == ')')
    {
      while (end > 0)
      {
        if (str[end] == ')')
          sub_parenthesis++;
        if (str[end] == '(')
          sub_parenthesis--;
        if (str[end] == '(' && sub_parenthesis == 0)
          return (1); // on a deux parentheses qui matchent : c'est un subshell
        end--;
      }
      return (2); //syntax error : parenthese non fernmee
    }
    end--;
  }
  return (0); // pas de parentheses trouvees : ce n'est pas un subshell
}

//return (0) : ce n'est pas un subshell
//return (1) : c'est un subshell
//return (2) : c'est une syntax error
int is_a_subshell_right(char *str, int end)
{
  int sub_parenthesis;
  int i;

  i = 0;
  sub_parenthesis = 0;
  while (end > 0)
  {
    if (str[i] == ')')
    {
      while (i > 0)
      {
        if (str[i] == ')')
          sub_parenthesis++;
        if (str[i] == '(')
          sub_parenthesis--;
        if (str[i] == '(' && sub_parenthesis == 0)
          return (1); // on a deux parentheses qui matchent : c'est un subshell
        i++;
      }
      return (2); //syntax error : parenthese non fernmee
    }
    i++;
  }
  return (0); // pas de parentheses trouvees : ce n'est pas un subshell
}

//Cherche le main boolop de la str donnee (le dernier boolop hors parentheses)
//Si on ne trouve pas d'operateur ou de parenthesis : on cree un node "SEQ" qui contient le char * du block a parser, et on le return
//si on trouveu un main boolop :
  //left = appel recursif sur la portion de str a gauche du boolop
  //right = appel recursif sur la portion de str a droite du boolop
  //left / right sont soit : 
    //un node boolop avec un booleen parenthesis set
    //un node SEQ pret a etre parser
//Return (NULL) : cas d'un syntax error OU un malloc qui foire (a differencier avec errno)
//return (node) : la tete ou un left / right a bracnher
t_seq *get_sequences(char *str, int end)
{
	int		i;
	int last_door;
	char *seq;
	t_seq *node;

	i = 0;
	//verifier si str existe et si str[i] exist !!
	/* if (!str) */
	/* 	return (1); */
	//trimer des espaces de debut et fin de ligne
	printf("end = %d\n", end);
	sleep(2);
  while (i < end && !is_main_bool_op(str, i, end)) //si j'ai pas trouver l'op le + important : le dernier hors parentheses
  {
    last_door = i;
    i += get_next_door(str, i, end);
    printf("next_door = %d\n", i);
    if (i == end) // gerer si 2 ou 0 ?
    {
      printf("found main bool op ! : str[%d] = %c%c\n", i, str[i], str[i + 1]);
      break ;
    }
  }
  if (i != (ft_strlen(str)) && i > 0)
  {
    node = malloc(sizeof(t_seq));
    if (!node)
    {
      //malloc error
    }
    node->sub_shell = 0;
    if (str[i] == '&')
      node->token = O_AND;
    else if (str[i] == '|')
      node->token = O_OR;
    node->left = get_sequences(str, i);
    if (!node->left)
    {
      //differencer le malloc de la syntax error
    }
    if (is_a_subshell_left(str, i)) // est-ce que left est un subshell ( )
      node->left->sub_shell = 1;
    else 
      node->left->sub_shell = 0;
    node->right = get_sequences((str + i + 2), (end - i - 2));
    if (!node->right)
    {
      //differencer le malloc de la syntax error
      //si errno == MEMALOC : on se barre
      //sinon on syntax error
    }
    if (is_a_subshell_right(str + i + 2, end))
      node->right->sub_shell = 1;
    else
      node->right->sub_shell = 0;
    node->content = NULL;
    return (node);
  }
  else if (i == end) //si on a pas trouve de main boolop : on a le bout de la branche
  {
    if (is_a_subshell_right(str, last_door))
      return (get_sequences(str + 1, end - 1)); //marche pas pour les espaces ou sans espaces faut revoir + 1 - 1 !
    node = malloc(sizeof(t_seq));
    if (!node)
    {
      //error malloc
    }
    node->token = SEQ;
    node->content = ft_strndup(str, end);
    node->left = NULL;
    node->right = NULL;
    node->sub_shell = 0;
    return (node);
  }
  else
  {
    //la ligne se finit par un boolop : syntax error
    // la ligne commence par un boolop : syntax error
    // il manque une parenthese
    return (NULL);
  }
}

int print_boolop(t_seq_type *type, int sub_shell)
{
  if (*type == O_AND)
    printf("AND");
  if (*type == O_OR)
    printf("OR");
  if (sub_shell == 1)
    printf(" in subshell\n");
  else
    printf("\n");
  return(0);
}

int print_sequence(t_seq *seq)
{
  t_seq *tmp;

  tmp = seq;
  if (!tmp)
    return (1);
  if (tmp->left)
    print_sequence(tmp->left);
  if (!(tmp->token == SEQ))
    print_boolop(&tmp->token, tmp->sub_shell);
  else
    printf("%s\n", tmp->content);
  if (tmp->right)
    print_sequence(tmp->right);
  return (0); 
}

int ft_int_strlen(char *s)
{
  int i;

  i = 0;
  if (!s || !*s)
    return (-1);
  while (s[i])
    i++;
  return (i);
}

int main (int ac, char **argv)
{
  t_seq *seq;

  int len = ft_int_strlen(argv[1]);
  printf("len = %d\n", len);
  seq = get_sequences(argv[1], len);
  //utiliser le principe du print de l'arbre ?
  print_sequence(seq);
  return (0);
}
