#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

#include "helper.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

hitable* random_scene()
{
	int n = 500;
	hitable** list = new hitable * [n + 1];
	list[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			auto choose_mat = random_double();
			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - point3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8f)
				{
					vec3 albedo = vec3::random() * vec3::random();
					list[i++] = new sphere(center, 0.2f, new lambertian(albedo));
				}
				else if (choose_mat < 0.95f)
				{
					auto albedo = vec3::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					list[i++] = new sphere(center, 0.2f, new metal(albedo, fuzz));
				}
				else
				{
					list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.5f));
	list[i++] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(vec3(0.4f, 0.2f, 0.1f)));
	list[i++] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));

	return new hitable_list(list, i);
}

int main() {
	// width, height, channels of image
	const auto aspect_ratio = 3.0 / 2.0;
	int nx = 1200; // width
	int ny = static_cast<int>(nx / aspect_ratio);
	int channels = 3;
	int ns = 100; // sample count
	const int max_depth = 50;
	auto aperture = 0.1;
	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	auto dist_to_focus = 10.0;

	// 存储图像数据
	unsigned char* data = new unsigned char[nx * ny * channels];


	// 场景相关
	hitable* wordl = random_scene();
	//world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	//world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);

	// 循环遍历图像nx*ny中的每个像素
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			// 采样 ns 次
			for (int k = 0; k < ns; k++) {
				float u = (i + random_double()) / float(nx);
				float v = (j + random_double()) / float(ny);
				// 确定 ray r
				ray r = cam.get_ray(u, v);
				// 累加 ray r 射入场景 world 后，返回的颜色
				col += color_ray(r, wordl, 0);
			}
			col /= float(ns);
			// gammar 矫正
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			// 写入图像数据 data[y*width*channels + x*channels + index]
			data[(ny - j - 1) * nx * 3 + 3 * i + 0] = int(255.99 * col[0]);
			data[(ny - j - 1) * nx * 3 + 3 * i + 1] = int(255.99 * col[1]);
			data[(ny - j - 1) * nx * 3 + 3 * i + 2] = int(255.99 * col[2]);
		}
		// print渲染进度
		std::cout << (ny - j) / float(ny) * 100.0f << "%\n";
	}
	// 写出png图片
	stbi_write_png("ch07.png", nx, ny, channels, data, 0);

	std::cout << "Completed.\n";
	//system("PAUSE");
	return 0;
}
