/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:52:22 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/08 15:07:24 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

// singleton :
//
// Malloc custom : qui ajoute a la liste / la cree et la renvoie(singleton)
//
// delete node
//
// empty gc
//
// Donnes moi la cle : return un pointeur vers la liste : on le cree en static dans la fonction
//

// wrapper de malloc
//

// creer une static pour ne pas avoir a trimballer le gc partout !
t_gar	**get_garbage_collector(void)
{
	t_gar static *garbage_collector = NULL;
	//la norme me fera chier pour ca ?

	/* if (garbage_collector == NULL) */
	/* 	garbage_collector = NULL; */
	return (&garbage_collector);
}

 //tests pour get_garbage_collector
/* int	main(void) */
/* { */
/* 	t_gar	**gc; */
/**/
/*   //renvoie 0x1000 */
/* 	printf("%p\n", gc); */
/*   gc = NULL; */
/*   //renvoie (nil) */
/* 	printf("%p\n", gc); */
/* 	gc = get_garbage_collector(); */
/*   //renvoie l'adresse du gc  */
/* 	printf("%p\n", gc); */
/* 	gc = get_garbage_collector(); */
/*   //renvoie exactement la meme adresse (grace a la static)  */
/* 	printf("%p\n", gc); */
/*   return (0);	 */
/* } */

void free_array(void **array)
{
  int i;

  i = 0;
  while (array[i])
  {
    free(array[i]);
    i++;
  }
  free(array);
}

//parcours tout le gc pour chercher le pointeur qu'on veut free
//si il le trouve, il le free correctement (des char *s ou des char **s ou des listes)
  //return le nombre de nodes free
	// return 0 si rien n'a ete free
	// return -1 si le free a foire ?
//
//va au bout du gc, et ajoute le node avec la memoire allouee pointee par ptr
//renvoie 0 si le node a ete ajoute
//renvoie 1 si le node n'a pas ete ajoute (si il existe deja ?)
//renvoie < 0 si y'a eu une erreur : -1 pour malloc qui foire
int add_to_garbage_collector(void *ptr, void (*free_fct)(void*))
{
    t_gar **garbage_collector;
    t_gar *new_node;
    t_gar *tmp;
    garbage_collector = get_garbage_collector();
    tmp = *garbage_collector;
    while (tmp)
    {
        if (ptr == tmp->allocated_memory)
            return (1); // on a deja ajoute cette zone memoire au gc
        tmp = tmp->next;
    }
    new_node = malloc(sizeof(t_gar)); 
    if (!new_node)
        return (-1); // malloc failed
    new_node->next = NULL;
    new_node->allocated_memory = ptr;
    new_node->free_fct = free_fct;
    if (*garbage_collector == NULL)
        *garbage_collector = new_node;
    else 
    {
        tmp = *garbage_collector;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_node;
    }
    return (0);
}
//tests pour add_to_garbage_collector

int main (void)
{
	char *s;
	int i;
	t_gar	**gc;
	char *to_split = "hello world";
	char **split;
	t_env *env;

  gc = get_garbage_collector();
	s = malloc(sizeof(char) * 4);
	i = 0;
	while (i < 4)
	{
	  s[i] = i + 48;
	  i++;
  }
  printf("allocated string : %s | %p\n", s, s);
  //si il a pu ajouter le node sans erreur / si le node existait deja 
	if (add_to_garbage_collector(s, &free) == 0)
	  printf("the string inside gc : %s | %p\n", (char *)(*gc)->allocated_memory, (*gc)->allocated_memory);
  if (add_to_garbage_collector(s, &free) == 1) //si on tente d'ajouter un noeud qu'on a deja ajoute : 
    printf("%s is already inside gc !\n", s);

  split = ft_split(to_split, ' ');
  i = 0;
  while (split[i])
  {
    printf("allocated splited %d : %s\n", split[i]);
    i++;
  }
  if (add_to_garbage_collector(split, (void(*)(void*))&free_array) == 0)
    printf("the split array inside gc : %p\n", (*gc)->next->allocated_memory);
  if (add_to_garbage_collector(split, (void(*)(void*))&free_array) == 1)
    printf("the split array is already inside the gc\n");
  //meme test avec env
	return (0);
}

//le wrapper renvoie le retour de malloc (un pointeur)
//juste avant, il tente d'ajouter ce pointeur au gc si il n'y est pas deja 
void	*garbage_collector_wrapper(void *ptr,  void (*free_fct)(void*))
{
	add_to_garbage_collector(ptr, &free);
	return (ptr);
}

//tests wrapper
/* int main (void) */
/* { */
/* 	char *s; */
/* 	int i; */
/* 	t_gar	**gc; */
/**/
/*   gc = get_garbage_collector(); */
/* 	s = garbage_collector_wrapper(malloc(sizeof(char) * 4)); */
/* 	i = 0; */
/* 	while (i < 4) */
/* 	{ */
/* 	  s[i] = i + 48; */
/* 	  i++; */
/*   } */
/*   printf("allocated string : %s at adress %p\n", s, s); */
/*   printf("string found in gc : %s at adress %p\n", (char *)(*gc)->allocated_memory, (char *)(*gc)->allocated_memory); */
/* 	return (0); */
/* } */

// delete one node
/* int	free_from_garbage_collector(void *ptr) */
/* { */
/* 	t_gar	*tmp; */
/* 	int		removed; */
/**/
/* 	removed = 0; */
/* 	tmp = get_garbage_collector(); */
/* 	while (tmp) */
/* 	{ */
/* 		if (ptr == garbage_collector->allocated_memory) */
/* 		{ */
/* 			// si c'est une string simple */
/* 			// si c'est un array */
/* 			// si c'est une liste */
/* 			// on free correctement */
/* 			removed++; */
/* 		} */
/* 		tmp = tmp->next; */
/* 	} */
/* 	// renvoie le nombre de nodes free */
/* 	return (removed); */
/* } */

/* int main (void) */
/* { */
/* 	char *s; */
/* 	char *s1; */
/* 	int i; */
/* 	t_gar	**gc; */
/**/
/*   gc = get_garbage_collector(); */
/* 	s = garbage_collector_wrapper(malloc(sizeof(char) * 5)); */
/* 	i = 0; */
/* 	while (i < 4) */
/* 	{ */
/* 	  s[i] = i + 48; */
/* 	  i++; */
/*   } */
/*   s[i] = '\0'; */
/*   printf("string 1 local : %s | %p\n", s, s); */
/*   printf("string 1 in gc : %s | %p\n", (char *)(*gc)->allocated_memory, (*gc)->allocated_memory); */
/*   s1 = garbage_collector_wrapper(malloc(sizeof(char) * 5)); */
/* 	i = 0; */
/* 	while (i < 4) */
/* 	{ */
/* 	  s1[i] = i + 6 + '0'; */
/* 	  i++; */
/*   } */
/*   s1[i] = '\0'; */
/*   printf("string 2 local : %s | %p\n", s1, s1); */
/*   printf("string 2 in gc : %s | %p\n", (char *)(*gc)->next->allocated_memory, (*gc)->next->allocated_memory); */
/*   return (0); */
/* } */
/**/

/* int	clear_garbage_collector(t_gar **garbage_collector) */
/* { */
/* 	while (garbage_collector) */
/* 	{ */
/* 		if (garbage_collector->allocated_memory) */
/* 		{ */
/* 			// si c'est une string simple */
/* 			// si c'est un array */
/* 			// si c'est une liste */
/* 			// on free correctement */
/* 		} */
/* 		garbage_collector = garbage_collector->next; */
/* 	} */
/* 	garbage_collector = NULL; */
/* 	return (0); */
/* } */

