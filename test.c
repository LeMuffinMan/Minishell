
//donner un end start ?
int get_next_door(char *str, int i)
{
  int i;
  int parenthesis;

  parenthesis = 0;
  i = 0;
  while (str[i])
  {
    //on a trouve le premier operateur en haut de l'arbre
    if (is_a_valid_operator(str, i) && partenthesis == 0)
      return (i);
    //ignorer tout ce qui est entre quotes sur la meme methode que ( ? 
    i++;
  }
  return (i);
}
//i == 0 : on est sur un bool op ou la ligne est vide
//i == ft_strlen(i) : on est arrive a la fin de la ligne
//else
//on a trouve l'operateur a mettre en haut de l'arbre


//add seq
      /* node->token = SEQ; */
      /* node->content = ft_strndup(str, i); */
      /**/
      /* node->token = SEQ; */
      /* node->content = ft_strndup(str + i + 2, stop); */
    //

//remplacer strlen par end ?
int is_main_bool_op(char *str, int i, int end)
{
  if (is_a_valid_boolop(str, i))
  {
    i += 2;
    while(i < end && str[i] != '(' && str[i] != ')') //il faudrait avancer de deux si un validboolop
    {
      if (s[i] == '&' || s[i] == '|' && !is_a_valid_boolop(str, i))
        i += 2;
      else
        i++;
    }
    if (str[i] == '(')
    {
      while(i < end && str[i] != ')')
        i++;
      if (i == end)
        return (-1); //parenthese non fermee
      else if ((i + 1) == end)//si str[i] existe on a trouve la fermeture, si str[i + 1] existe pas : on a le bout de ligne
        return (1);
    }
    if (get_next_door(str, i) == end) //si on a atteint le bout de line sans trouver d'operateur 
      return (1);
    else
      return (0);
  }
  else
    return (0);
}

//passer en deux fonctions find_matching_open_bracket // close_bracket
int is_a_subshell(char *str, int end, int mode)
{
  int sub_parenthesis;

  sub_parenthesis = 0;
  if (mode == 0)
  {
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
            return (1);
          end--;
        }
        return (2); //syntax error : parenthese non fernmee
      }
      end--;
    }
    return (0); // pas de parentheses trouvees
  }
  if (mode == 1)
  {
    //pareil dans l'autre sens
  }
}

t_seq *get_sequences(char *str, int end)
{
	int		i;
	int last_door;
	char *seq;
	t_seq *node;

	i = start;
	/* if (!str) */
	/* 	return (1); */
	//trimer des espaces de debut et fin de ligne
  while (is_main_bool_op(str, i, end)) //si j'ai pas trouver l'op le + important : le dernier hors parentheses
  {
    last_door = i;
    i += get_next_door(str, i, end);
  }
  if (i != (ft_strlen(str)) && i > start)
  {
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
    if (is_a_subshell(str, i, 0)) // est-ce que left est un subshell ( )
      node->left->sub_shell = 1;
    else 
      node->left->sub_shell = 0;
    node->right = get_sequences((str + i + 2), (end - i - 2));
    if (!noright)
    {
      //differencer le malloc de la syntax error
      //si errno == MEMALOC : on se barre
      //sinon on syntax error
    }
    if (is_a_subshell(str + i + 2, end, 1))
      node->right->sub_shell = 1;
    else
      node->right->sub_shell = 0;
    node->content = NULL;
    return (node);
  }
  else if (i == end) //si on a pas trouve de main boolop : on a le bout de la branche
  {
    if (is_a_subshell(str, last_door, 1))
      return (get_sequences(str + 1, end - 1)); //marche pas pour les espaces ou sans espaces faut revoir + 1 - 1 !
    node->token = SEQ;
    node->content = ft_strndup(str, end);
    node->left = NULL;
    node->right = NULL;
    node->sub_shell = 0;
    return (node)
  }
  else
  {
    //la ligne se finit par un boolop : syntax error
    // la ligne commence par un boolop : syntax error
    // il manque une parenthese
    return (NULL);
  }
}
	  /* if (str[i] == '(') */
	  /* { */
	  /*   if (i += get_seq_line(str, i, seqs) == 1) */
   /*    { */
   /*      //syntax error : parenthese pas fermee ou rien */
   /*      // */
   /*    } */
   /*    //sinon un node a ete cree dans seqs */
   /*    //i est positionne a la fin de la parenthese creee */
	  /* } */
	  /* if (is_a_valid_operator(str, i)) */
	  /* { */
	  /*   //cree un node ET */
	  /*   //si un node existe, on le branche sur son left et on deplace la tete sur ce ET */
	  /*   get_door(str, i, seqs); */
   /*     */
	  /* } */
	}
}
