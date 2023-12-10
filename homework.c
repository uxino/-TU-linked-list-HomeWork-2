#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int satir;

typedef struct s_node
{
	char			*name;
	char			*school_number;
	int				birth_day;
    struct s_node	*next;
}	t_node;

int	ft_nbrlen(int n)
{
	int	i;

	i = 0;
	if (n <= 0)
		i = 1;
	while (n)
	{
		n /= 10;
		++i;
	}
	return (i);
}

void	ft_lstadd_back(t_node **lst, t_node *new)
{
	t_node	*tmp;

	tmp = *lst;
	if (!tmp)
	{
		*lst = new;
		return ;
	}
	while (tmp -> next)
		tmp = tmp -> next;
	tmp ->next = new;
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		len2;

	len = ft_nbrlen(n);
	len2 = ft_nbrlen(n);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	if (n == 0)
		str[0] = '0';
	if (n < 0)
	{
		str[0] = '-';
		if (n == -2147483648)
		{
			str[--len] = '8';
			n /= 10;
		}
		n = -n;
	}
	while (len-- && n != 0)
	{
		str[len] = (n % 10) + '0';
		n /= 10;
	}
	str[len2] = '\0';
	return (str);
}

t_node *lst_new(char *school_number, char *name, int birthday)
{
    t_node *new = (t_node *)malloc(sizeof(t_node));
    
	if (!new)
        return NULL;

	new->name = name;
	new->school_number = school_number;
	new->birth_day = birthday;
    new->next = NULL;
    return new;
}

int ft_dif(const char *s, const char *p)
{
    int i = 0;
    while (s[i] && p[i] && s[i] == p[i])
        i++;
    return s[i] - p[i];
}

t_node	*find_and_extract_largest(t_node **lst)
{
	t_node	*current;
	t_node	*prev;
	t_node	*largest;
	t_node	*prev_largest;

	current = *lst;
	prev = NULL;
	largest = current;
	prev_largest = NULL;
	while (current != NULL)
	{
		if (current->birth_day > largest->birth_day)
		{
			largest = current;
			prev_largest = prev;
		}
		prev = current;
		current = current->next;
	}
	if (prev_largest != NULL)
		prev_largest->next = largest->next;
	else
		*lst = largest->next;
	return (largest);
}

t_node	*find_and_extract_largest2(t_node **lst)
{
	t_node	*current;
	t_node	*prev;
	t_node	*largest;
	t_node	*prev_largest;

	current = *lst;
	prev = NULL;
	largest = current;
	prev_largest = NULL;
	while (current != NULL)
	{
		if (ft_dif(current->school_number, largest->school_number) > 0)
		{
			largest = current;
			prev_largest = prev;
		}
		prev = current;
		current = current->next;
	}
	if (prev_largest != NULL)
		prev_largest->next = largest->next;
	else
		*lst = largest->next;
	return (largest);
}


t_node	*ft_lstcpy(t_node *lst)
{
	t_node	*result;
	t_node	*result_current;

	result = lst_new(lst->school_number, lst->name, lst->birth_day);
	result_current = result;
	lst = lst->next;
	while (lst)
	{
		ft_lstadd_back(&result_current,lst_new(lst->school_number, lst->name, lst->birth_day));
		lst = lst->next;
	}
	return (result);
}

t_node	*sort(t_node *lst)
{
	t_node	*sorted;
	t_node	*largest;

	sorted = NULL;
	while (lst)
	{
		largest = find_and_extract_largest(&lst);
		largest->next = sorted;
		sorted = largest;
	}
	return (sorted);
}

t_node	*sort2(t_node *lst)
{
	t_node	*sorted;
	t_node	*largest;

	sorted = NULL;
	while (lst)
	{
		largest = find_and_extract_largest2(&lst);
		largest->next = sorted;
		sorted = largest;
	}
	return (sorted);
}

t_node *connect()
{
    FILE *dosya;
    t_node *head = NULL;
    t_node *tmp = NULL;
	char	name[50];
	char	school_number[10];
	int		birth_day;

    dosya = fopen("input.txt", "r");
    if (dosya == NULL)
	{
        perror("File cannot open!");
        return NULL;
    }
    satir = 0;

    // Dosyadaki satır sayısını bulma
    while (fscanf(dosya, "%*[^\n]\n") != EOF)
        satir++;
    rewind(dosya);

	while (fscanf(dosya, "%19[^;];%49[^;];%d\n", school_number, name, &birth_day) == 3)
	{
        if (head == NULL)
		{
            tmp = lst_new(strdup(school_number), strdup(name), birth_day);
            head = tmp;
        } 
		else
		{
            tmp->next = lst_new(strdup(school_number), strdup(name), birth_day);
            tmp = tmp->next;
        }
    }
    fclose(dosya);
    return head;
}
void	output(t_node *head, t_node *head2)
{
	t_node *tmp;
	t_node *tmp2;

	tmp = head;
	tmp2 = head2;
	FILE *dosya = fopen("output.txt", "w");
	fprintf(dosya, "Student names in ascending order by birthday:\n");
	while (tmp)
	{
		fprintf(dosya, "%s;%d\n", tmp->name, tmp->birth_day);
		tmp = tmp->next;
	}
	fprintf(dosya, "School numbers by the faculty codes in ascending order:\n");
	while (tmp2)
	{
		fprintf(dosya, "%s\n", tmp2->school_number);
		tmp2 = tmp2->next;
	}
	fclose(dosya);
}

int main()
{
    t_node *head = connect();
	t_node *head_cpy = ft_lstcpy(head);
	t_node *head_cpy2 = ft_lstcpy(head);

	t_node *sorted_lst = sort(head_cpy); // finish
	t_node *sorted_lst2 = sort2(head_cpy2);

    if (head != NULL)
		output(sorted_lst, sorted_lst2);
    return 0;
}
