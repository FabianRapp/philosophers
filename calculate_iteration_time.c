/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_iteration_time.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 02:37:18 by frapp             #+#    #+#             */
/*   Updated: 2024/01/14 02:51:50 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// not performance critical
static double	determinant(double matrix[3][3])
{
	double	det;

	det = matrix[0][0] * (matrix[1][1] * matrix[2][2]
			- matrix[1][2] * matrix[2][1])
		- matrix[0][1] * (matrix[1][0] * matrix[2][2]
			- matrix[1][2] * matrix[2][0])
		+ matrix[0][2] * (matrix[1][0] * matrix[2][1]
			- matrix[1][1] * matrix[2][0]);
	return (det);
}

// not performance critical
// matrices[4][3][3] are 4 matrices; it has to be this way to to make it
// look somewhat reasonable and pass the norm
static void	cramers_rule(double points[3][2], double *a, double *b, double *c)
{
	double	matrices[4][3][3];
	double	det;
	int		i;

	i = 0;
	while (i < 3)
	{
		matrices[0][i][0] = points[i][0] * points[i][0];
		matrices[0][i][1] = points[i][0];
		matrices[0][i++][2] = 1;
	}
	ft_memcpy(matrices[1], matrices[0], sizeof(matrices[0]));
	ft_memcpy(matrices[2], matrices[0], sizeof(matrices[0]));
	ft_memcpy(matrices[3], matrices[0], sizeof(matrices[0]));
	i = -1;
	while (++i < 3)
	{
		matrices[1][i][0] = points[i][1];
		matrices[2][i][1] = points[i][1];
		matrices[3][i][2] = points[i][1];
	}
	det = determinant(matrices[0]);
	*a = determinant(matrices[1]) / det;
	*b = determinant(matrices[2]) / det;
	*c = determinant(matrices[3]) / det;
}

// // not performance critical
int64_t	calculate_iteration_time(int64_t x)
{
	double	a;
	double	b;
	double	c;
	double	points[3][2];

	points[0][0] = POINT1_X;
	points[0][1] = POINT1_Y;
	points[1][0] = POINT2_X;
	points[1][1] = POINT2_Y;
	points[2][0] = POINT3_X;
	points[2][1] = POINT3_Y;
	cramers_rule(points, &a, &b, &c);
	return ((int64_t)(a * x * x + b * x + c));
}
