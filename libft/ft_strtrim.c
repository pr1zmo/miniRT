/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 20:23:41 by mel-bouh          #+#    #+#             */
/*   Updated: 2023/11/06 22:40:29 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check(char const *set, char s1)
{
	if (!set)
		return (0);
	while (*set)
	{
		if (*set == s1)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	size_t	size;
	char	*trim;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	size = ft_strlen(s1);
	while (check(set, *(s1 + i)) && *(s1 + i))
		i++;
	while (check(set, *(s1 + (size - 1))) && size > i)
		size--;
	trim = (char *)malloc(size - i + 1);
	if (trim == NULL)
		return (NULL);
	j = 0;
	while (i < size)
	{
		*(trim + j) = *(s1 + i);
		i++;
		j++;
	}
	*(trim + j) = '\0';
	return (trim);
}

/*#include <stdio.h>
#include <stdlib.h>

int main() {
    // Test 1: Basic test case
    const char *s1 = "   xxx   xxx";
    const char *set = " x";
    char *result1 = ft_strtrim(s1, set);
    printf("Test 1: Trimmed string: \"%s\"\n", result1);
    free(result1); // Free the allocated memory

    // Test 2: Edge case with empty string
    const char *s2 = "";
    const char *set2 = " ";
    char *result2 = ft_strtrim(s2, set2);
    printf("Test 2: Trimmed string: \"%s\"\n", result2);
    free(result2); // Free the allocated memory

    // Test 3: Edge case with all characters in 'set'
    const char *s3 = "abc";
    const char *set3 = "abc";
    char *result3 = ft_strtrim(s3, set3);
    printf("Test 3: Trimmed string: \"%s\"\n", result3);
    free(result3); // Free the allocated memory

    // Test 4: Edge case with 'set' being empty
    const char *s4 = "Hello, World!";
    const char *set4 = "";
    char *result4 = ft_strtrim(s4, set4);
    printf("Test 4: Trimmed string: \"%s\"\n", result4);
    free(result4); // Free the allocated memory

    // Test 5: Edge case with 'set' containing control characters
    const char *s5 = "\t\r\nHello, World!\t\r\n";
    const char *set5 = "\t\r\n";
    char *result5 = ft_strtrim(s5, set5);
    printf("Test 5: Trimmed string: \"%s\"\n", result5);
    free(result5); // Free the allocated memory

    return 0;
}
*/
