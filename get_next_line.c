/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 17:21:51 by modat             #+#    #+#             */
/*   Updated: 2024/11/08 19:27:08 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>
#include <stdlib.h>

int	ft_find_newline(char *buffer)
{
	size_t	i;

	if (!buffer)
		return (-1);
	i = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char	*extract_line_and_leftover(char **leftover)
{
	char		*line;
	char		*temp;
	int			newline_index;

	newline_index = ft_find_newline(*leftover);
	line = ft_substr(*leftover, 0, newline_index + 1);
	temp = ft_substr(*leftover, newline_index + 1, -1);
	free(*leftover);
	*leftover = temp;
	return (line);
}

char	*read_linebyline(int fd, char *buffer, char **leftover)
{
	ssize_t	read_res;
	char	*temp;

	temp = NULL;
	read_res = 1;
	while (ft_strchr(*leftover, '\n') == NULL) // as long as *leftover does not contain a \n the loop continues
	{
		read_res = read(fd, buffer, BUFFER_SIZE);
		if (read_res == -1)
			return (NULL);
		if (read_res == 0) // read_res is 0, it means the end of the file has been reached
		{
			if (*leftover == NULL || **leftover == '\0') // Edge case: if there' no leftover data 
				return (NULL);
			temp = *leftover; // EOF has data assign it to temp 
			return ((*leftover = NULL), temp); // no data left in leftover all has been printed. Therefore, it assigned to NULL. Since there's no data to hold any more, then no need for it to store anything. 
// leftover is reset to NULL so that it's ready for the next read operation, if there is any further data to read. it returns NULL, meaning the file is empty or fully read. it makes sure that when the function is called again, it starts with an empty leftover state.
		}
		buffer[read_res] = '\0';
		*leftover = ft_strjoin(*leftover, buffer);
		if (!*leftover)
			return (NULL);
	}
	return (extract_line_and_leftover(leftover));
}
// if \n found and the function returned sth and not == NULL it means it has a \n 
char	*get_next_line(int fd)
{
	char		*buffer;
	static char	*leftover = NULL; // static variables are not automatically initialized, there fore it needs to initializing (NULL) to avoid initialize garbage data. initialized to NULL ensure it has a known clear state/ data. 
	char		*line;	

	if (leftover == NULL) // this is to check the hold data from previous calls and here checks the first value the leftover has "NULL". 
	{
		leftover = ft_strdup(""); // here to allocate a memory and prepare it to hold data for different calls, assign it to empty value.
		if (!leftover) // if the allocation didn't work then return null.
			return (NULL);
	}
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1)); 
	if (!buffer)
		return (free(leftover), leftover = NULL, NULL); // free leftover & assign it to NULL before return NULL when buffer fails to allocate a memory. Nonetheless, when it fails I must not leaving a memory not freed when finish the program. 
	if (BUFFER_SIZE < 1 || fd < 0) // Edge cases 
		return (free(buffer), NULL); // not freeing leftover here, because I still need it and use it. // leftover, being static, should not be freed in this case because it is intended to persist for future function calls and is not dynamically allocated in the same way as buffer.
	line = read_linebyline(fd, buffer, &leftover);
	if (!line)
	{
		free(leftover);
		leftover = NULL;
	}
	free(buffer);
	return (line);
}
/*
char	*extract_line_and_leftover(char **leftover, int newline_index)
{
	char		*line;
	char		*temp;

	line = ft_substr(*leftover, 0, newline_index + 1);
	temp = ft_substr(*leftover, newline_index + 1, -1);
	free(*leftover);
	*leftover = temp;
	return (line);
}

int	malloc_buffer(char **leftover, char **buffer)
{
	if (*leftover == NULL)
	{
		*leftover = ft_strdup("");
		if (!*leftover)
			return (1);
	}
	*buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!*buffer)
		return (free(*leftover), (*leftover = NULL), 1);
	return (0);
}

char	*get_next_line(int fd)
{
	static char	*leftover;
	char		*buffer;
	ssize_t		read_res;
	int			newline_index;
	char		*temp;

	if (malloc_buffer(&leftover, &buffer))
		return (NULL);
	while ((newline_index = ft_find_newline(leftover)) == -1)
	{
		read_res = read(fd, buffer, BUFFER_SIZE);
		if (read_res == -1)
			return (free(buffer), free(leftover), leftover = NULL, NULL);
		if (read_res == 0)
		{
			if (leftover && *leftover == '\0')
				return (free(buffer), free(leftover), (leftover = NULL), NULL);
			temp = leftover;
			return ((leftover = NULL), free(buffer), temp);
		}
		buffer[read_res] = '\0';
		leftover = ft_strjoin(leftover, buffer);
	}
	return (free(buffer), extract_line_and_leftover(&leftover, newline_index));
}
//
// char	*get_next_line(int	fd)
// {
// 	static char	*leftover = NULL;
// 	char		*buffer;
// 	ssize_t		read_res;
// 	int			newline_index;
// 	char		*line = NULL;
// 	 char		*temp;

// 	// Initialize leftover if NULL
// 	if (leftover == NULL)
// 		leftover = ft_strdup("");

// 	// Allocate and read into buffer
// 	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
// 	if (!buffer)
// 		return (NULL);
// 	while ((newline_index = ft_find_newline(leftover)) == -1)
// 	{
// 		read_res = read(fd, buffer, BUFFER_SIZE);
// 		if (read_res == -1) // Error occurred during read
// 		{
//     		free(buffer);
//         	free(leftover); // Free leftover if there's an error
//         	leftover = NULL;
//         	return (NULL);
// 		}
// 		if (read_res == 0)  // EOF or error
// 		{
// 			free(buffer);
// 			if (*leftover == '\0') // No data left
// 			{
// 				free(leftover);
// 				leftover = NULL;
// 				return (NULL);
// 			}
// 			line = leftover; // Last line without newline
// 			leftover = NULL;
// 			return (line);
// 		}
// 		buffer[read_res] = '\0';
// 		temp = leftover;
// 		leftover = ft_strjoin(leftover, buffer);
// 		free(temp);
// 	}
// 	free(buffer);

// 	// Extract the line up to the newline
// 	line = ft_substr(leftover, 0, newline_index + 1);

// 	// Update leftover to contain only the remaining part after the newline
// 	temp = ft_substr(leftover, newline_index + 1,
//	ft_strlen(leftover) - newline_index - 1);
// 	free(leftover);
// 	leftover = temp;
// 	return (line);
// }
*/
