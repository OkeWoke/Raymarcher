#include <iostream>
#include <opencv2/opencv.hpp>
#include "Vec.h"
#include "IObject.h"
#include "Sphere.h"
#include "Plane.h"
#include <vector>
#include <valarray>
#include <atomic>
#include <cmath>

typedef std::valarray<double> imBufDouble;
typedef std::valarray<uint8_t> imBufuInt;

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

double RayMarch(const Vec& origin, const Vec& ray, const std::vector<std::shared_ptr<IObject>>& scene);
void populateScene(std::vector<std::shared_ptr<IObject>>& scene);
imBufDouble castRays(const Camera& cam, const std::vector<std::shared_ptr<IObject>>& scene);

void populateScene(std::vector<std::shared_ptr<IObject>>& scene)
{
    std::shared_ptr<IObject> sphere_0 = std::make_shared<Sphere>(Vec(0,0,40), 15, Vec(0, 255, 0));
    std::shared_ptr<IObject> plane_0 = std::make_shared<Plane>(Vec(0,-6, 0), Vec(0, 1, 0), Vec(0, 255, 0));
    scene.push_back(std::move(sphere_0));
    scene.push_back(std::move(plane_0));
}

imBufDouble castRays(const Camera& cam, const std::vector<std::shared_ptr<IObject>>& scene)
{
    unsigned int total_pixels = cam.X_RES*cam.Y_RES;
    volatile std::atomic<size_t> pixel_count(0); //To be used for parallelprocessing in the future.
    imBufDouble imageBuffer( total_pixels*3);
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

        double dist = RayMarch(cam.position, ray_norm, scene);

        imageBuffer[bufferByteCount] = dist;
        imageBuffer[bufferByteCount + 1] = dist;
        imageBuffer[bufferByteCount + 2] = dist;
        bufferByteCount += 3;
    }

    return imageBuffer;
}

double RayMarch(const Vec& origin, const Vec& ray, const std::vector<std::shared_ptr<IObject>>& scene)
{
    unsigned int MAX_STEPS = 40;
    unsigned int MAX_LENGTH = 50;

    double epsilon = 0.0001;
    Vec pos = origin;
    double total_length=0;
    std::shared_ptr<IObject> hit_obj = nullptr;
    std::vector<double> distances;

    Sphere sphere_0(Vec(1,1,2), 0.4, Vec(0, 255, 0));
    Sphere sphere_1(Vec(0,5,25), 15, Vec(0, 255, 0));
    Sphere sphere_2(Vec(0,8,20), 15, Vec(0, 255, 0));
    Sphere sphere_3(Vec(0,20,25), 8, Vec(0, 255, 0));
    Sphere sphere_4(Vec(0,0,5), 10, Vec(0, 255, 0));
    Plane plane_0(Vec(0,-6, 0), Vec(0, 1, 0), Vec(0, 255, 0));

    for(unsigned int i=0; i < MAX_STEPS; i++)
    {
        double min_dist = 9e16;
        double rep = 2;
        Vec modulo_pos(fmod(pos.x, rep), fmod(pos.y, rep), fmod(pos.z, rep));
        double sphere_0_dist = sphere_0.SDF(modulo_pos);
        double plane_0_dist = plane_0.SDF(pos);
        double sphere_1_dist = sphere_1.SDF(pos);
        double sphere_2_dist = sphere_2.SDF(pos);
        double sphere_3_dist = sphere_3.SDF(pos);
        double sphere_4_dist = sphere_4.SDF(pos);

        min_dist = sphere_0_dist;
        /**min_dist = std::max(plane_0_dist, -1*sphere_0_dist);
        min_dist = std::max(min_dist, -1*sphere_4_dist);
        double other_min = std::max(sphere_1_dist, -1*sphere_2_dist);
        min_dist = std::min(min_dist, other_min);
        min_dist = std::min(sphere_3_dist, min_dist);**/


        if (min_dist < epsilon)
        {
            break;
        }

        total_length+=min_dist;
        if (total_length > MAX_LENGTH)
        {
            //total_length =MAX_LENGTH;// MAX_LENGTH; //for distance field images
            //break;
        }
        if (hit_obj != nullptr) break;
        pos = pos + ray*min_dist;
    }
    return 255-total_length;
}

int main() {
    std::cout << "Raymarcher!" << std::endl;
    Camera cam;
    cam.position = Vec(0,0,0);
    cam.n = normalise(Vec(-0.5,-0.5,-1));
    cam.u = Vec(1, 0 ,0);
    cam.v = Vec(0, -1, 0);
    cam.X_RES = 800;
    cam.Y_RES = 800;
    cam.WIDTH = 5;
    cam.HEIGHT = 5;
    cam.FL = 6;

    std::vector<std::shared_ptr<IObject>> scene;
    populateScene(scene);

    imBufDouble imageBuffer = castRays(cam, scene);
    double multiplier = 255.0 / (imageBuffer.max());
    imageBuffer = multiplier * (imageBuffer);

    imBufuInt cvBuffer(cam.Y_RES * cam.X_RES * 3);
    for (uint32_t i = 0; i < imageBuffer.size(); ++i) {
        cvBuffer[i] = static_cast<uint8_t>(imageBuffer[i]);
    }

    cv::Mat cv_img(cam.Y_RES, cam.X_RES, CV_8UC3, &(cvBuffer[0]), 3 * cam.X_RES);

    cv::imshow("Raymarcher!111!!11!", cv_img);
    cv::waitKey(0);
    cv::destroyAllWindows();

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream filename;
    filename << "/Users/liammurphy/Documents/Raymarcher/renders/render_"  << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << ".png";

    cv::imwrite(filename.str(), cv_img);
    std::cout <<"Raymarcher Finished" << std::endl;
    return 0;
}

/** Original Scene File method... not useful for experimenting with CSG.
        for(std::shared_ptr<IObject> obj: scene)
        {
            distances.push_back( obj->SDF(pos));
        }
        for(double dist: distances)
        {
            if (dist < min_dist) min_dist =  dist;

            if (dist < epsilon)
            {
                break;
            }
        }**/