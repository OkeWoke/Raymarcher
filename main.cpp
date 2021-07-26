#include <opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>
#include <valarray>
#include <atomic>
#include <cmath>
#include <future>
#include <chrono>

#include "Vec.h"
#include "IObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "MandelBulb.h"

#include <climits>


typedef std::valarray<float> imBufDouble;
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

struct Hit
{
    int steps;
    int closest_dist;
    int total_length;
    Vec ray;
    Vec pos;
    std::shared_ptr<IObject> hit_obj = nullptr;
};

Hit RayMarch(const Vec& origin, const Vec& ray, const std::vector<std::shared_ptr<IObject>>& scene);
void populateScene(std::vector<std::shared_ptr<IObject>>& scene);
imBufDouble castRays(const Camera& cam, const std::vector<std::shared_ptr<IObject>>& scene);
Vec Shade(Hit hit, const std::vector<std::shared_ptr<IObject>>& scene);
void displayHistogram(cv::Mat src);

void populateScene(std::vector<std::shared_ptr<IObject>>& scene)
{
    std::shared_ptr<IObject> sphere_0 = std::make_shared<Sphere>(Vec(0,0,40), 15, Vec(200, 0, 30));
    std::shared_ptr<IObject> plane_0 = std::make_shared<Plane>(Vec(0,-1, 0), Vec(0, 1, 0), Vec(0, 100, 30));
    scene.push_back(std::move(sphere_0));
    scene.push_back(std::move(plane_0));

    std::shared_ptr<IObject> mandelBulb_0 = std::make_shared<MandelBulb>();
    //scene.push_back(std::move(mandelBulb_0));
}

imBufDouble castRays(const Camera& cam, const std::vector<std::shared_ptr<IObject>>& scene)
{
    unsigned int total_pixels = cam.X_RES*cam.Y_RES;
    volatile std::atomic<size_t> pixel_count(0); //To be used for parallelprocessing in the future.
    imBufDouble imageBuffer( total_pixels*3);

    std::vector<std::future<void>> future_vector;
    int cores_to_use = 8;
    for (int i = 0; i<cores_to_use; i++ )
    {
        future_vector.emplace_back(
        async(std::launch::async, [=, &cam, &scene, &imageBuffer, &pixel_count]()
        {
            while (true)
            {
                int pix_copy = pixel_count++;
                int x_index = pix_copy;
                if (x_index >= total_pixels) break;

                int y_index = x_index / cam.X_RES;
                x_index = x_index % cam.X_RES;

                Vec ray_dir =
                        -cam.FL * cam.n + cam.WIDTH * (((double) 2 * x_index / (cam.X_RES - 1)) - 1) * cam.u +
                        cam.HEIGHT * (((double) 2 * y_index / (cam.Y_RES - 1)) - 1) * cam.v;
                Vec ray_norm = normalise(ray_dir);

                Hit hit = RayMarch(cam.position, ray_norm, scene);

                Vec color = Shade(hit, scene);

                imageBuffer[3 * pix_copy] = color.z;
                imageBuffer[3 * pix_copy + 1] = color.y;
                imageBuffer[3 * pix_copy + 2] = color.x;
            }
        }));
    }
    return imageBuffer;
}

Hit RayMarch(const Vec& origin, const Vec& ray, const std::vector<std::shared_ptr<IObject>>& scene)
{
    unsigned int MAX_STEPS = 100;
    unsigned int MAX_LENGTH = 30;
    double EPSILON = 0.01;

    Vec pos = origin;
    double total_length=0;
    double min_dist = 9e16;
    Hit hit;
    unsigned int i;

    for(i=0; i < MAX_STEPS; i++)
    {
        for(std::shared_ptr<IObject> obj: scene)
        {
            double dist = obj->SDF(pos);

            if (dist < min_dist) min_dist =  dist;
            if (dist < EPSILON)
            {
                hit.hit_obj = obj;
                break;
            }
        }

        total_length+=min_dist;
        if (total_length > MAX_LENGTH)
        {
            total_length =MAX_LENGTH;// MAX_LENGTH; //for distance field images
            break;
        }
        pos = pos + ray*min_dist;
    }

    hit.ray = ray;
    hit.pos = pos;
    hit.total_length = total_length;
    hit.closest_dist = min_dist;
    hit.steps = i;
    return hit;
}

double clamp(double val, double min){
    return (val < min) ? min : val;
}

Vec Shade(Hit hit, const std::vector<std::shared_ptr<IObject>>& scene)
{
   // if (hit.hit_obj != nullptr) return Vec(1,1,1) * (pow(2,16)- 1)*0;
    //return Vec(1,1,1) * hit.steps;
    //if(hit.hit_obj == nullptr)
    {
        double t = 0.5*(hit.ray.y+1);
        return (1-t)*Vec(255, 255,255) + t*Vec(127, 127, 127);
    }

    Vec Light(10, 10, 0);
    Vec LightRay = (Light - hit.pos);
    double lightRayLength = LightRay.abs();
    LightRay = normalise(LightRay);
    Hit shadow_hit = RayMarch(hit.pos+0.001*LightRay, LightRay, scene);
    Vec normal = hit.hit_obj->getNormal(hit.pos);

    return 100*clamp(normal.dot(LightRay), 0)*hit.hit_obj->color/clamp(lightRayLength, 1);
}
void displayHistogram(imBufDouble& imageBuffer, unsigned int no_pixels)
{
//vovkos.github.io/doxyrest-showcase/opencv/sphinx_rtd_theme/page_tutorial_histogram_calculation.html
    double max_val = imageBuffer.max();
    double min_val = imageBuffer.min();
    double mean = imageBuffer.sum()/(3*no_pixels);
    imBufDouble imageBufferSTD = imageBuffer - mean; //copy
    imageBufferSTD*=imageBufferSTD; //square
    double std = sqrt(imageBufferSTD.sum()/(3*no_pixels));

    std::cout << "Mean Pixel Value: " << mean << std::endl;
    std::cout << "STD Pixel Value: " << std << std::endl;
    std::cout << "Max Pixel Value: " << max_val << std::endl;
    std::cout << "Min Pixel Value: " << min_val << std::endl;

    unsigned int no_bins = 800;
    std::valarray<unsigned int> hist_counts(no_bins);
    double bin_increment = (max_val-min_val)/no_bins;

    for (int k=0; k<no_pixels; k+=1)
    {
        int i = no_bins*((imageBuffer[k*3] - min_val)/(max_val-min_val));
        hist_counts[i]++;
    }

    unsigned int hist_height = 800;

    cv::Mat histImage( hist_height, no_bins, CV_8UC3, cv::Scalar( 0,0,0) );

    for( int i = 0; i < no_bins; i++ )
    {
        cv::line( histImage, cv::Point(i ,hist_height - hist_height*hist_counts[i]/(hist_counts.max())),
                  cv::Point(i ,hist_height),
              cv::Scalar( 255, 255, 255), 2, 8, 0  );
    }
    std::cout << "Histogram Computed" << std::endl;
    cv::imshow("Histogram", histImage );

    cv::waitKey(0);
}

int main() {
    if (sizeof(float) * CHAR_BIT != 32) std::cout << "Not 32-bit float "<< std::endl;
    std::cout << "Raymarcher!" << std::endl;
    Camera cam;
    cam.position = Vec(0,0.1,-3);
    cam.n = normalise(Vec(0,0,-1));
    cam.u = Vec(1, 0 ,0);
    cam.v = Vec(0, -1, 0);
    cam.X_RES = 800;
    cam.Y_RES = 800;
    cam.WIDTH = 5;
    cam.HEIGHT = 5;
    cam.FL = 10;

    std::vector<std::shared_ptr<IObject>> scene;
    populateScene(scene);

    auto cast_start = std::chrono::steady_clock::now();
    imBufDouble imageBuffer = castRays(cam, scene);
    auto cast_end = std::chrono::steady_clock::now();
    std::cout << "Render Time: " << (cast_end - cast_start)/std::chrono::milliseconds(1) << " (ms)" << std::endl;

    displayHistogram(imageBuffer, cam.X_RES*cam.Y_RES);
    double multiplier = 255/ (imageBuffer.max());
    imageBuffer = multiplier * (imageBuffer);

    imBufuInt cvBuffer(cam.Y_RES * cam.X_RES * 3);
    for (uint32_t i = 0; i < imageBuffer.size(); ++i)
    {
        cvBuffer[i] = static_cast<uint8_t>(imageBuffer[i]);
    }

    cv::Mat cv_img(cam.Y_RES, cam.X_RES, CV_8UC3, &(cvBuffer[0]), 3 * cam.X_RES);
    //displayHistogram(cv_img);
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
