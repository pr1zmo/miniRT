#include "../minirt.h"

t_vector compute_bump_normal(t_vector point, t_vector normal, t_texture bump_map)
{
	// Step 1: Compute UV coordinates
	float u = 0.5 + atan2(normal.z, normal.x) / (2 * M_PI);
	float v = 0.5 - asin(normal.y) / M_PI;

	// Step 2: Sample bump map and calculate derivatives
	float delta = 0.001;
	float center = sample_texture(bump_map, u, v);
	float du = (sample_texture(bump_map, u + delta, v) - center) / delta;
	float dv = (sample_texture(bump_map, u, v + delta) - center) / delta;

	// Step 3: Compute tangent and bitangent
	t_vector tangent = vec_norm((t_vector){-normal.z, 0, normal.x});
	t_vector bitangent = vec_cro(normal, tangent);

	// Step 4: Perturb normal in tangent space
	t_vector perturbed_tangent = {-du, -dv, 1.0};
	perturbed_tangent = vec_norm(perturbed_tangent);

	// Step 5: Transform to world space
	t_matrix3x3 tbn = {tangent, bitangent, normal};
	t_vector perturbed_world = mat3x3_mult_vec(tbn, perturbed_tangent);
	return vec_norm(perturbed_world);
}

// Usage in shading
int compute_lighting(t_rt *rt, t_vector point, t_vector normal, ...)
{
	t_vector bumped_normal = compute_bump_normal(point, normal, rt->bump_map);
	// Use bumped_normal for lighting calculations
	...
}