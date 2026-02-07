#include "marching_cubes.h"

#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>

#include "generator.h"
#include "polygons.h"

MarchingCubes::MarchingCubes()
{

}

vector<float> MarchingCubes::GenerateVertices()
{
    // for (int x = 0; x < 25; x++) {
    //     for (int y = 0; y < 25; y++) {
    //         cout << "[";

    //         cout << "]" << endl;
    //     }
    //     cout << endl;
    // }
    for (int x = 0; x < 2500; x++) {
        for (int y = 0; y < 25; y++) {
            for (int z = 0; z < 25; z++) {
                float noise = layeredNoise3D(x / 10, y, z, 5, 0.07, 1.0f, 100);
                // Clamp just in case
                if (noise < 0.0f) noise = 0.0f;
                if (noise > 1.0f) noise = 1.0f;

                int brightness = static_cast<int>(noise * 255.0f);

                cout << "\033[48;2;"
                     << brightness << ";"
                     << brightness << ";"
                     << brightness << "m   ";
            }
            cout << "\033[0m\n"; // reset after each row
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        cout << endl << endl << endl;
    }
    return generateSphere(5, 100, 100);;
}