#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int satir;

typedef struct s_node
{
    int 			value;
    struct s_node	*down;
    struct s_node	*next;
} t_node;

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

t_node *lst_new(int content)
{
    t_node *new = malloc(sizeof(t_node));
    
	if (!new)
        return NULL;
    new->down = NULL;
    new->next = NULL;
    new->value = content;
    return new;
}

t_node *connect()
{
    FILE *dosya;
    int okunan;
    t_node *head = NULL;
    t_node *tmp = NULL;
    t_node *tmp2 = NULL;
	
    dosya = fopen("input.csv", "r");
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

    for (int i = 0; i < satir; i++)
	{
        for (int j = 0; j < satir; j++)
		{
            if (fscanf(dosya, "%d,", &okunan) == 1)
			{
                if (i == 0 && j == 0)
				{
                    head = lst_new(okunan);
                    tmp = head;
                    tmp2 = head;
                }
				else
				{
                    if (j == 0)
					{
                        tmp2->down = lst_new(okunan);
                        tmp2 = tmp2->down;
                        tmp = tmp2;
                    }
					else
					{
                        tmp->next = lst_new(okunan);
                        tmp = tmp->next;
                    }
                }
            }
			else
			{
                perror("the csv file is not square or file reading error.");
                return NULL;
            }
        }
    }
    fclose(dosya);
    return head;
}

void output_print(t_node *head)
{
    t_node *tmp = head;
	FILE *dosya = fopen("output_print.txt", "w");

    for (int i = 0; i < satir; i++)
	{
        for (int j = 0; j < satir; j++)
		{
			char *s = ft_itoa(tmp->value);
            fprintf(dosya,s);
			if (j < satir -1)
            	fprintf(dosya,",");
			tmp = tmp->next;
        }
		if (i < satir -1)
        	fprintf(dosya,"\n");
        head = head->down;
        tmp = head;
    }
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
    if (head != NULL)
	{
        output_print(head);
		output(head);
	}
    return 0;
}
