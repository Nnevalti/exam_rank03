#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct		s_data
{
	int		w;
	int		h;
	char	c;
}					t_data;

typedef struct		s_shape
{
	char			c;
	float			x;
	float			y;
	float			r;
	char			col;
}					t_shape;

void	display_canvas(t_data data, char *canvas)
{
	int	x;
	int	y;

	y = 0;
	while (y < data.h)
	{
		x = 0;
		while (x < data.w)
		{
			write(1, &canvas[y * data.w + x], 1);
			x++;
		}
		write(1, "\n", 1);
		y++;
	}
}

int		in_border(t_shape shape, int x, int y)
{
	return (sqrtf(pow(shape.x - x, 2) + pow(shape.y - y, 2)) > shape.r - 1);
}

int		in_shape(t_shape shape, int x, int y)
{
	return (sqrtf(powf(shape.x - x, 2) + powf(shape.y - y, 2)) <= shape.r);
}

void	draw(t_data data, t_shape shape, char *canvas)
{
	int		x;
	int		y;

	y = 0;
	while (y < data.h)
	{
		x = 0;
		while (x < data.w)
		{
			if (shape.c == 'c' && in_shape(shape, x, y)
				&& in_border(shape, x, y))
				canvas[y * data.w + x] = shape.col;
			else if(shape.c == 'C' && in_shape(shape, x, y))
				canvas[y * data.w + x] = shape.col;
			x++;
		}
		y++;
	}
}

int			mini_paint(FILE *file)
{
	t_data	data;
	char	*canvas;
	t_shape	shape;

	if ((fscanf(file, "%d %d %c\n", &data.w, &data.h, &data.c) != 3))
		return (1);
	if (data.h <= 0 || data.h > 300 || data.w <= 0 || data.w > 300)
		return (1);
	if (!(canvas = malloc(sizeof(char) * data.h * data.w)))
		return (1);
	memset(canvas, data.c, data.h * data.w);
	while ((fscanf(file, "%c %f %f %f %c\n", &shape.c, &shape.x, &shape.y,
		&shape.r, &shape.col) == 5))
	{
		if (shape.r <= 0 || (shape.c != 'c' && shape.c != 'C'))
		{
			free(canvas);
			return (1);
		}
		draw(data, shape, canvas);
	}
	display_canvas(data, canvas);
	free(canvas);
	return (0);
}

int		main(int ac, char **av)
{
	FILE	*file;

	if (ac == 2)
	{
		if (!(file = fopen(av[1], "r")) || mini_paint(file))
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
