#include <iostream>
#include <opencv2/opencv.hpp>
#include "Vec.h"
#include "IObject.h"
#include "Sphere.h"
#include "Plane.h"
#include <vector>
#include <valarray>
#include <atomic>

typedef std::valarray<uint8_t> imBufType;

struct Camera
{
    Vec position;
    Vec u, v, n;
    unsigned int X_RES;
    unsigned int Y_RES;
    double WIDTH;
    double HEIGHT;
    double FL;
};

double RayMarch(Vec origin, Vec ray, std::vector<IObject*>& scene);
void populateScene(std::vector<IObject*>& scene);
imBufType castRays(Camera& cam, std::vector<IObject*>& scene);

void populateScene(std::vector<IObject*>& scene)
{
    Sphere sphere_0(Vec(0,1,5), 5, Vec(0, 255, 0));
    Plane plane_0(Vec(0,-1, 0), Vec(0, 1, 0), Vec(0, 255, 0));
    scene.push_back(&sphere_0);
    scene.push_back(&plane_0);
}

imBufType castRays(Camera& cam, std::vector<IObject*>& scene)
{
    unsigned int total_pixels = cam.X_RES*cam.Y_RES;
    volatile std::atomic<size_t> pixel_count(0); //To be used for parallelprocessing in the future.
    imBufType imageBuffer( total_pixels*3);
    uint64_t bufferByteCount = 0;
    while(true) {

        int x_index = pixel_count++;

        if (x_index >= total_pixels) {
            break;
        }
        int y_index = x_index / cam.X_RES;
        x_index = x_index % cam.X_RES;

        Vec ray_dir =
                -cam.FL * cam.n + cam.WIDTH * (((double) 2 * x_index / (cam.X_RES - 1)) - 1) * cam.u +
                cam.HEIGHT * (((double) 2 * y_index / (cam.Y_RES - 1)) - 1) * cam.v;
        Vec ray_norm = normalise(ray_dir);

        uint8_t dist = static_cast<uint8_t>(255*float(y_index)/cam.Y_RES);//RayMarch(cam.position, ray_norm, scene);

        imageBuffer[bufferByteCount] = dist;
        imageBuffer[bufferByteCount + 1] = dist;
        imageBuffer[bufferByteCount + 2] = dist;
        bufferByteCount += 3;
    }

    return imageBuffer;
}

double RayMarch(Vec origin, Vec ray, std::vector<IObject*>& scene)
{
    return 255;
}

int main() {
    std::cout << "Raymarcher!" << std::endl;
    Camera cam;
    cam.position = Vec();
    cam.n = Vec(0,0,1);
    cam.X_RES = 800;
    cam.Y_RES = 800;
    cam.WIDTH = 5;
    cam.HEIGHT = 5;

    std::vector<IObject*> scene;
    populateScene(scene);

    imBufType imageBuffer = castRays(cam, scene);
    cv::Mat cv_img(cam.Y_RES, cam.X_RES, CV_8UC3, &(imageBuffer[0]), 3*cam.X_RES);

    cv::imshow("Raymarcher!111!!11!", cv_img);
    cv::waitKey(0);
    std::cout <<"Raymarcher Finished" << std::endl;
    return 0;
}
