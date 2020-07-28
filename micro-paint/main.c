#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct	s_data
{
	int			width;
	int			height;
	char		c;
}				t_data;

typedef struct	s_shape
{
	char		c;
	float		x;
	float		y;
	float		w;
	float		h;
	char		col;
}				t_shape;

void			display_canvas(char *canvas, t_data data)
{
	int			i;
	int			j;
	i = 0;

	while (i < data.height)
	{
		j = 0;
		while (j < data.width)
		{
			write(1, &canvas[i * data.width + j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}

int				in_shape(t_shape shape, int i, int j)
{
	return (shape.x <= j && j <= shape.x + shape.w
		&& shape.y <= i && i <= shape.y + shape.h);
}

int				in_border(t_shape shape, int i, int j)
{
	return ((j < shape.x + 1 || j > shape.x + shape.w - 1
	|| i < shape.y + 1 || i > shape.y + shape.h - 1));
}
void			draw(t_data data, t_shape shape, char *canvas)
{
	int			i;
	int			j;
	i = 0;

	while (i < data.height)
	{
		j = 0;
		while (j < data.width)
		{
			// if (Xtl <= Xa <= Xbr and Ytl <= Ya <= Ybr)
			if (shape.c == 'R' && in_shape(shape, i, j))
				canvas[i * data.width + j] = shape.col;
			else if (shape.c == 'r' && in_shape(shape, i, j)
					&& in_border(shape, i, j))
				canvas[i * data.width + j] = shape.col;
			j++;
		}
		i++;
	}
}
int				micro_paint(FILE *file)
{
	t_data		data;
	t_shape		shape;
	char		*canvas;

	if ((fscanf(file, "%d %d %c\n", &data.width, &data.height, &data.c) != 3))
		return (1);
	if (data.width <= 0 || data.width > 300
		|| data.height <= 0 || data.height > 300)
		return (1);
	if (!(canvas = malloc(sizeof(char) * data.height * data.width)))
		return (1);
	memset(canvas, data.c, data.height * data.width);
	while ((fscanf(file, "%c %f %f %f %f %c\n", &shape.c, &shape.x, &shape.y,
			&shape.w, &shape.h, &shape.col) == 6))
	{
		if (shape.h <= 0 || shape.w <= 0 || (shape.c != 'r' && shape.c != 'R'))
		{
			free(canvas);
			return (1);
		}
		draw(data, shape, canvas);
	}
	display_canvas(canvas, data);
	free(canvas);
	return (0);

}

int				main (int ac, char **av)
{
	FILE 		*file;
	char 		*error;

	if (ac == 2)
	{
		if (!(file = fopen(av[1], "r")))
		{
			write(1, "Error: Operation file corrupted\n", 32);
			return (1);
		}
		if (micro_paint(file))
		{
			write(1, "Error: Operation file corrupted\n", 32);
			return (1);
		}
	}
	else
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	return (0);
}
