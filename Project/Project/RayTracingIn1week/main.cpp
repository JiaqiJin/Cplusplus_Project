#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

#include "vec3.h"
#include "ray.h"

/*(t^2)b·b + 2tb·(a - c) +  (a - c)· (a - c) - r^2 = 0 , t = unknown */
double hit_sphere(const vec3& center, float radius, const ray&r)
{
	vec3 A_C = r.origin() - center;
	vec3 B = r.direction();
	float a = dot(B, B);//b·b 
	float b = 2.0 * dot(A_C, B); //2tb·(a - c)
	float c = dot(A_C, A_C) - radius * radius; //a - c)· (a - c) - r^2
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
}

color ray_color(const ray& r)
{
	// 如果 ray r 击中球体的话，返回球体的颜色
	auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
	if (t > 0.0)
	{
		vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
		return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
	}
	// 否则，计算背景色并返回
	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
	// width, height, channels of image
	const auto aspect_ratio = 16.0 / 9.0;
	int nx = 1920; // width
	int ny = 1080; // height
	int channels = 3;
	// 存储图像数据
	unsigned char* data = new unsigned char[nx * ny * channels];

	// camera相关
	vec3 lower_left_corner(-2.0, -1.0, -1.0); // 左下角
	vec3 horizontal(4.0, 0.0, 0.0); // 横轴
	vec3 vertical(0.0, 2.0, 0.0); // 竖轴
	vec3 origin(0.0, 0.0, 0.0); // eye

	// 循环遍历图像nx*ny中的每个像素
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			// 确定 ray r
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);

			// 计算 ray r 返回的颜色
			vec3 color = ray_color(r);

			// 写入图像数据 data[y*width*channels + x*channels + index]
			data[(ny - j - 1) * nx * 3 + 3 * i + 0] = int(255.99 * color[0]);
			data[(ny - j - 1) * nx * 3 + 3 * i + 1] = int(255.99 * color[1]);
			data[(ny - j - 1) * nx * 3 + 3 * i + 2] = int(255.99 * color[2]);
		}
		// 计算渲染进度
		std::cout << (ny - j) / float(ny) * 100.0f << "%\n";
	}
	// 写出png图片
	stbi_write_png("ch00&01.png", nx, ny, channels, data, 0);

	std::cout << "Completed.\n";
	//system("PAUSE");
}
