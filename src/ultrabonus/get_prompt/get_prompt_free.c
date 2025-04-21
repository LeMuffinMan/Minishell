/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:44:39 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 16:12:28 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include <stdlib.h>

int free_prompt(t_prompt *prompt)
{
  if (!prompt)
    return (1);
  if (prompt->ps1)
    free(prompt->ps1);  
  if (prompt->user)
    free(prompt->user);
  if (prompt->uid)
    free(prompt->uid);
  if (prompt->hostname)
    free(prompt->hostname);
  if (prompt->hostname_long)
    free(prompt->hostname_long);
  if (prompt->pwd)
    free(prompt->pwd);
  if (prompt->git_branch)
    free(prompt->git_branch);
  if (prompt->user_type)
    free(prompt->user_type);
  if (prompt->prompt)
    free(prompt->prompt);
  free(prompt);
  return (0);
}

