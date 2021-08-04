#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#endif

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

struct Hit
{
    int steps;
    int closest_dist;
    int total_length;
    Vec ray;
    Vec pos;
    std::shared_ptr<IObject> hit_obj = nullptr;
};

Hit RayMarch(const Vec& origin, const Vec& ray, const std::vector<std::shared_ptr<IObject>>& scene, double max_length);
void populateScene(std::vector<std::shared_ptr<IObject>>& scene);
void castRays(const Camera& cam, const std::vector<std::shared_ptr<IObject>>& scene, imBufDouble& imageBuffer);
Vec Shade(Hit hit, const std::vector<std::shared_ptr<IObject>>& scene);
#ifdef USE_OPENCV
void displayHistogram(cv::Mat src);
#endif
const unsigned int MAX_STEPS = 255;
const double MAX_LENGTH = 100;
const double EPSILON = 0.01;
const unsigned int THREADS_TO_USE = 24;

void populateScene(std::vector<std::shared_ptr<IObject>>& scene)
{
    std::shared_ptr<IObject> sphere_0 = std::make_shared<Sphere>(Vec(0,0,40), 15, Vec(200, 0, 30));
    std::shared_ptr<IObject> plane_0 = std::make_shared<Plane>(Vec(0,-1, 0), Vec(0, 1, 0), Vec(0, 100, 30));
    //scene.push_back(std::move(sphere_0));
    //scene.push_back(std::move(plane_0));

    std::shared_ptr<IObject> mandelBulb_0 = std::make_shared<MandelBulb>();
    scene.push_back(std::move(mandelBulb_0));
}

void castRays(const Camera& cam, const std::vector<std::shared_ptr<IObject>>& scene, imBufDouble& imageBuffer)
{
    unsigned int total_pixels = cam.X_RES*cam.Y_RES;
    volatile std::atomic<size_t> pixel_count(0); //To be used for parallelprocessing in the future.
    //std::valarray<double> imageBuffer( total_pixels*3);

    std::vector<std::future<void>> future_vector;

    for (int i = 0; i<THREADS_TO_USE; i++ )
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

                Hit hit = RayMarch(cam.position, ray_norm, scene, MAX_LENGTH);

                Vec color = Shade(hit, scene);
                int pixIdx = 3 * pix_copy;
                imageBuffer[pixIdx] = color.z;
                imageBuffer[pixIdx + 1] = color.y;
                imageBuffer[pixIdx + 2] = color.x;
            }
        }));
    }
}

double sceneSDF(const std::vector<std::shared_ptr<IObject>>& scene, Vec pos)
{
    double min_dist= 9e19;

    for(std::shared_ptr<IObject> obj: scene)
    {
        double dist = obj->SDF(pos);
        if (dist < min_dist) min_dist = dist;
    }

    return min_dist;
}
Hit RayMarch(const Vec& origin, const Vec& ray, const std::vector<std::shared_ptr<IObject>>& scene, double max_length)
{
    Vec pos = origin;
    double total_length=0;
    Hit hit;
    unsigned int i;

    for(i=0; i < MAX_STEPS; i++)
    {
        if (total_length > max_length)
        {
            total_length = max_length;
            break;
        }
        double min_dist = 9e16;
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
        pos = pos + ray*min_dist;
    }

    hit.ray = ray;
    hit.pos = pos;
    hit.total_length = total_length;
    hit.steps = i;
    return hit;
}

double clamp(double val, double min){
    return (val < min) ? min : val;
}

double softShadow(Vec lightRay, Vec pos, double minT, double maxT, double k, const std::vector<std::shared_ptr<IObject>>& scene)
{
    double res = 1;
    for(double t=minT; t<maxT;)
    {
        double dist = sceneSDF(scene, pos);
        if (dist< 0.001) return 0;
        res = std::min(res, k*dist/t);
        t+=dist;
    }
    return res;
}

Vec Shade(Hit hit, const std::vector<std::shared_ptr<IObject>>& scene)
{
    return Vec(1,1,1)*hit.total_length;
    if(hit.total_length == MAX_LENGTH || hit.hit_obj == nullptr)
    {
        double t = 0.5*(hit.ray.y+1);
        return (1-t)*Vec(180, 180,180) + t*Vec(60, 60, 150);
    }
  

    Vec Light(3, 3, -8);
    Vec LightRay = (Light - hit.pos);
    double lightRayLength = LightRay.abs();
    LightRay = normalise(LightRay);
    Hit shadow_hit = RayMarch(hit.pos+0.001*LightRay, LightRay, scene, lightRayLength);
    //double shadow = softShadow(LightRay, hit.pos+EPSILON*LightRay, 0.01, lightRayLength, 2, scene);
    if (shadow_hit.hit_obj == nullptr ) return Vec(0,0,0);
    Vec normal = hit.hit_obj->getNormal(hit.pos-normalise(hit.ray)*2*EPSILON);

    return 3*clamp(normal.dot(LightRay), 0)*hit.hit_obj->color;//clamp(lightRayLength, 1);
}
#ifdef USE_OPENCV
void displayHistogram(imBufDouble& imageBuffer, unsigned int no_pixels)
{
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

    for (int k=0; k<no_pixels; k+=1)
    {
        int i = no_bins*((imageBuffer[k*3] - min_val)/(max_val-min_val));
        hist_counts[i]++;
    }

    unsigned int hist_height = 800;

    cv::Mat histImage( hist_height, no_bins+2, CV_8UC3, cv::Scalar( 0,0,0) );

    std::valarray<unsigned int> hist_counts_sorted = hist_counts;
    std::sort(std::begin(hist_counts_sorted), std::end(hist_counts_sorted));
    unsigned int render_offset = 0;
    unsigned int max_val_to_render = hist_counts_sorted[no_bins-render_offset-1];

    for( int i = 0; i < no_bins-render_offset; i++ )
    {
        cv::line( histImage, cv::Point(i+1 ,hist_height - hist_height*hist_counts[i]/max_val_to_render),
                  cv::Point(i+1 ,hist_height),
              cv::Scalar( 255, 255, 255), 2, 8, 0  );
    }

    std::cout << "Histogram Range Max: " << max_val_to_render << ", Min: " << hist_counts_sorted[0] << std::endl;
    std::cout << "Histogram Computed" << std::endl;
    cv::imshow("Histogram", histImage );

    cv::waitKey(0);
    cv::destroyAllWindows();
}
#endif
int main() {
    std::cout << "Raymarcher!" << std::endl;

    Camera cam;
    cam.position = Vec(0,0,-2.8);
    cam.n = normalise(Vec(0,0,-1));
    cam.u = Vec(1, 0 ,0);
    cam.v = Vec(0, -1, 0);
    cam.X_RES = 800;
    cam.Y_RES = 800;
    cam.WIDTH = 5;
    cam.HEIGHT = 5;
    cam.FL = 10;

    int total_px = cam.Y_RES * cam.X_RES;
    std::vector<std::shared_ptr<IObject>> scene;
    populateScene(scene);

    imBufDouble imageBuffer(total_px * 3);

    auto cast_start = std::chrono::steady_clock::now();
    castRays(cam, scene, imageBuffer);
    auto cast_end = std::chrono::steady_clock::now();
    std::cout << "Render Time: " << (cast_end - cast_start)/std::chrono::milliseconds(1) << " (ms)" << std::endl;

    //displayHistogram(imageBuffer, cam.X_RES*cam.Y_RES);

    double multiplier = 1/ (imageBuffer.max());
    imageBuffer = 255* pow(imageBuffer*multiplier, 0.454);

    imBufuInt cvBuffer(cam.Y_RES * cam.X_RES * 3);
    for (uint32_t i = 0; i < imageBuffer.size(); ++i)
    {
        cvBuffer[i] = static_cast<uint8_t>(imageBuffer[i]);
    }

#ifdef USE_OPENCV
    cv::Mat cv_img(cam.Y_RES, cam.X_RES, CV_8UC3, &(cvBuffer[0]), 3 * cam.X_RES);
    cv::imshow("Raymarcher!111!!11!", cv_img);
    cv::waitKey(0);
    cv::destroyAllWindows();

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream filename;
    // mac "/Users/liammurphy/Documents/Raymarcher/renders/render_"
    // win "C:\\Users\\OkeWoke\\Documents\\Raymarcher\\renders\\render_"
    filename <<  "C:\\Users\\OkeWoke\\Documents\\Raymarcher\\renders\\render_" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << ".png";
    cv::imwrite(filename.str(), cv_img);
#endif
    std::cout <<"Raymarcher Finished" << std::endl;

    return 0;
}
