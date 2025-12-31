#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

vector<float> generateGrid(int width, int height, int scale, int numsPerVertex) {
    vector<float> vertices(width * height * numsPerVertex * 6);
    int i = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            vertices[i++] = x * scale;
            vertices[i++] = 0.0f;
            vertices[i++] = y * scale;
            // Fill in the blank spaces that aren't position.
            for (int num = 3; num < numsPerVertex; num++) {
                vertices[i++] = 0.0f;
            }

            vertices[i++] = x * scale + scale;
            vertices[i++] = 0.0f;
            vertices[i++] = y * scale;
            for (int num = 3; num < numsPerVertex; num++) {
                vertices[i++] = 0.0f;
            }

            vertices[i++] = x * scale;
            vertices[i++] = 0.0f;
            vertices[i++] = y * scale + scale;
            for (int num = 3; num < numsPerVertex; num++) {
                vertices[i++] = 0.0f;
            }

            vertices[i++] = x * scale + scale;
            vertices[i++] = 0.0f;
            vertices[i++] = y * scale;
            for (int num = 3; num < numsPerVertex; num++) {
                vertices[i++] = 0.0f;
            }

            vertices[i++] = x * scale + scale;
            vertices[i++] = 0.0f;
            vertices[i++] = y * scale + scale;
            for (int num = 3; num < numsPerVertex; num++) {
                vertices[i++] = 0.0f;
            }

            vertices[i++] = x * scale;
            vertices[i++] = 0.0f;
            vertices[i++] = y * scale + scale;
            for (int num = 3; num < numsPerVertex; num++) {
                vertices[i++] = 0.0f;
            }
        }
    }
    cout << vertices.size() << endl;
    return vertices;
}
