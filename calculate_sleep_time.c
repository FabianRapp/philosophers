/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_sleep_time.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 15:45:20 by frapp             #+#    #+#             */
/*   Updated: 2024/01/10 22:33:38 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// not performance critical
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*restrict d;
	const char	*restrict s;

	if (!dst && !src && n)
		return (dst);
	d = (char *restrict)dst;
	s = (const char *restrict)src;
	while (n--)
		*d++ = *s++;
	return (dst);
}

// not performance critical
double	determinant(double matrix[3][3])
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
void	solve_quadratic(double points[3][2], double *a, double *b, double *c)
{
	double	matrix[3][3];
	double	matrix_a[3][3];
	double	matrix_b[3][3];
	double	matrix_c[3][3];
	double	det;
	int		i;
	// Populate the matrix with x^2, x, and 1
	i = 0;
	while (i < 3)
	{
		matrix[i][0] = points[i][0] * points[i][0];
		matrix[i][1] = points[i][0];
		matrix[i++][2] = 1;
	}
	ft_memcpy(matrix_a, matrix, sizeof(matrix));
	ft_memcpy(matrix_b, matrix, sizeof(matrix));
	ft_memcpy(matrix_c, matrix, sizeof(matrix));
	// Replace the appropriate columns with y values
	i = 0;
	while (i < 3)
	{
		matrix_a[i][0] = points[i][1];
		matrix_b[i][1] = points[i][1];
		matrix_c[i][2] = points[i][1];
		i++;
	}
	// Calculate determinants
	det = determinant(matrix);
	// Calculate coefficients
	*a = determinant(matrix_a) / det;
	*b = determinant(matrix_b) / det;
	*c = determinant(matrix_c) / det;
}

// // not performance critical
int64_t	quadratic_function(int64_t x)
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
	// Solve for coefficients
	solve_quadratic(points, &a, &b, &c);
	// Calculate y using the quadratic function y = ax^2 + bx + c
	return ((int64_t)(a * x * x + b * x + c));
}

// int64_t	quadratic_function(int64_t x)
// {
// 	double	a = 0.040218;
// 	double	b = 19.345708;
// 	double	c = 3922.133678;

// 	// Calculate y using the quadratic function y = ax^2 + bx + c
// 	return ((int64_t)(a * x * x + b * x + c));
// }
// a: 0.030218
// b: 19.345708
// c: 3922.133678