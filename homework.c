#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int satir;

typedef struct s_node
{
	char			*name;
	char			*school_number;
	int				school_number_flag;
	int				birth_day;
	int				birth_day_flag;
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

t_node *lst_new(char *school_number, char *name, int birthday, int birth_day_flag, int school_number_flag)
{
    t_node *new = (t_node *)malloc(sizeof(t_node));
    
	if (!new)
        return NULL;

	new->name = name;
	new->school_number = school_number;
	new->birth_day = birthday;
	new->birth_day_flag = birth_day_flag;
	new->school_number_flag = school_number_flag;
    new->next = NULL;
    return new;
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
t_node	*ft_lstcpy(t_node *lst)
{
	t_node	*result;
	t_node	*result_current;

	result = lst_new(lst->school_number, lst->name, lst->birth_day, -1, -1);
	result_current = result;
	lst = lst->next;
	while (lst)
	{
		ft_lstadd_back(&result_current,lst_new(lst->school_number, lst->name, lst->birth_day, lst->birth_day_flag, lst->school_number_flag));
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

void	flag(t_node **lst, t_node **lst2)
{
	int		i;
	t_node	*temp;

	i = 1;
	temp = (*lst);
	while ((*lst2))
	{
		while ((*lst))
		{
			if ((*lst2)->birth_day == (*lst)->birth_day)
			{
				(*lst)->birth_day_flag = i;
				i++;
				break ;
			}
			(*lst) = (*lst)->next;
		}
		(*lst) = temp;
		free(*lst2);
		(*lst2) = (*lst2)->next;
	}
	(*lst) = temp;
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
            tmp = lst_new(strdup(school_number), strdup(name), birth_day, -1, -1);
            head = tmp;
        } 
		else
		{
            tmp->next = lst_new(strdup(school_number), strdup(name), birth_day, -1, -1);
            tmp = tmp->next;
        }

    }

    fclose(dosya);
    return head;
}
void	output(t_node *head)
{
	t_node *tmp = head;
	t_node *tmp2 = head->down;
	int count = 0;
	FILE *dosya = fopen("output.txt", "w");
	for (int i = 0; i < satir ; i++)
	{
		count++;
		fprintf(dosya, ft_itoa(tmp->value));
		tmp = tmp2;
		for (int j = 0; j < count; j++)
			tmp = tmp->next;
		if (tmp2->down)
			tmp2 = tmp2->down;
		if (i < satir - 1)
			fprintf(dosya, ",");
	}
	fprintf(dosya, "\n");
	
	tmp = head;
	tmp2 = head;
	count = 0;
	int	count_for_comma = 0;
	for (int i = 0; i < satir; i++)
	{
		tmp = head;
		for (int k = 0; k < satir; k++)
		{
			tmp2 = tmp;
			for (int k = 0; k < count; k++)
				tmp2 = tmp2->next;
			fprintf(dosya, ft_itoa(tmp2->value));
			tmp = tmp->down;
			count_for_comma++;
			if (count_for_comma < satir * satir)
				fprintf(dosya, ",");

		}
		count++;
	}
    fclose(dosya);
}

int main()
{
    t_node *head = connect();
	t_node *head_cpy = ft_lstcpy(head);

	t_node *sorted_lst = sort(head);

	// flag(&head_cpy, &sorted_lst);
    if (head != NULL)
		output(sorted_lst);
    return 0;
}
