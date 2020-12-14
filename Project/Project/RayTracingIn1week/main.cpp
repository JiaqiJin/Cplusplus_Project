﻿#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

#include "helper.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

color ray_color(const ray& r, const hittable& world)
{
	hit_record rec;
	if (world.hit(r, 0.001, infinity, rec)) {
		// 若 ray 交到场景中 diffuse 物体，trace 新的 ray
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		// pick random point S inside this unit radius and send a ray from the hit point P to random point S
		return 0.5 * ray_color(ray(rec.p, target - rec.p), world);
	}
	else {
		// 否则，计算背景色并返回
		vec3 unit_direction = unit_vector(r.direction());
		float t = (0.5 * unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main() {
	// width, height, channels of image
	const auto aspect_ratio = 16.0 / 9.0;
	int nx = 400; // width
	int ny = static_cast<int>(nx / aspect_ratio);
	int channels = 3;
	int ns = 100; // sample count

	// 存储图像数据
	unsigned char* data = new unsigned char[nx * ny * channels];


	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	camera cam;

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
				col += ray_color(r, world);
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
