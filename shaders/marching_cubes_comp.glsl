layout(local_size_x = 4, local_size_y = 4, local_size_z = 4) in;
struct Vertex {
    vec3 position;   // xyz
    vec3 normal;     // xyz
    vec3 color;      // rgb
};

layout(std430, binding = 0) buffer TriangleBuffer {
    uint floatCount;
    float vertices[];
};

vec3 colors[500]=vec3[](vec3(0.000,0.000,1.000),vec3(0.000,0.004,0.996),vec3(0.000,0.008,0.992),vec3(0.000,0.012,0.988),vec3(0.000,0.016,0.984),vec3(0.000,0.020,0.980),vec3(0.000,0.024,0.976),vec3(0.000,0.028,0.972),vec3(0.000,0.032,0.968),vec3(0.000,0.036,0.964),vec3(0.000,0.040,0.960),vec3(0.000,0.044,0.956),vec3(0.000,0.048,0.952),vec3(0.000,0.052,0.948),vec3(0.000,0.056,0.944),vec3(0.000,0.060,0.940),vec3(0.000,0.064,0.936),vec3(0.000,0.068,0.932),vec3(0.000,0.072,0.928),vec3(0.000,0.076,0.924),vec3(0.000,0.080,0.920),vec3(0.000,0.084,0.916),vec3(0.000,0.088,0.912),vec3(0.000,0.092,0.908),vec3(0.000,0.096,0.904),vec3(0.000,0.100,0.900),vec3(0.000,0.104,0.896),vec3(0.000,0.108,0.892),vec3(0.000,0.112,0.888),vec3(0.000,0.116,0.884),vec3(0.000,0.120,0.880),vec3(0.000,0.124,0.876),vec3(0.000,0.129,0.871),vec3(0.000,0.133,0.867),vec3(0.000,0.137,0.863),vec3(0.000,0.141,0.859),vec3(0.000,0.145,0.855),vec3(0.000,0.149,0.851),vec3(0.000,0.153,0.847),vec3(0.000,0.157,0.843),vec3(0.000,0.161,0.839),vec3(0.000,0.165,0.835),vec3(0.000,0.169,0.831),vec3(0.000,0.173,0.827),vec3(0.000,0.177,0.823),vec3(0.000,0.181,0.819),vec3(0.000,0.185,0.815),vec3(0.000,0.189,0.811),vec3(0.000,0.193,0.807),vec3(0.000,0.197,0.803),vec3(0.000,0.201,0.799),vec3(0.000,0.205,0.795),vec3(0.000,0.209,0.791),vec3(0.000,0.213,0.787),vec3(0.000,0.217,0.783),vec3(0.000,0.221,0.779),vec3(0.000,0.225,0.775),vec3(0.000,0.229,0.771),vec3(0.000,0.233,0.767),vec3(0.000,0.237,0.763),vec3(0.000,0.241,0.759),vec3(0.000,0.245,0.755),vec3(0.000,0.249,0.751),vec3(0.000,0.253,0.747),vec3(0.000,0.257,0.743),vec3(0.000,0.261,0.739),vec3(0.000,0.265,0.735),vec3(0.000,0.269,0.731),vec3(0.000,0.273,0.727),vec3(0.000,0.277,0.723),vec3(0.000,0.281,0.719),vec3(0.000,0.285,0.715),vec3(0.000,0.289,0.711),vec3(0.000,0.293,0.707),vec3(0.000,0.297,0.703),vec3(0.000,0.301,0.699),vec3(0.000,0.305,0.695),vec3(0.000,0.309,0.691),vec3(0.000,0.313,0.687),vec3(0.000,0.317,0.683),vec3(0.000,0.321,0.679),vec3(0.000,0.325,0.675),vec3(0.000,0.329,0.671),vec3(0.000,0.333,0.667),vec3(0.000,0.337,0.663),vec3(0.000,0.341,0.659),vec3(0.000,0.345,0.655),vec3(0.000,0.349,0.651),vec3(0.000,0.353,0.647),vec3(0.000,0.357,0.643),vec3(0.000,0.361,0.639),vec3(0.000,0.365,0.635),vec3(0.000,0.369,0.631),vec3(0.000,0.373,0.627),vec3(0.000,0.378,0.622),vec3(0.000,0.382,0.618),vec3(0.000,0.386,0.614),vec3(0.000,0.390,0.610),vec3(0.000,0.394,0.606),vec3(0.000,0.398,0.602),vec3(0.000,0.402,0.598),vec3(0.000,0.406,0.594),vec3(0.000,0.410,0.590),vec3(0.000,0.414,0.586),vec3(0.000,0.418,0.582),vec3(0.000,0.422,0.578),vec3(0.000,0.426,0.574),vec3(0.000,0.430,0.570),vec3(0.000,0.434,0.566),vec3(0.000,0.438,0.562),vec3(0.000,0.442,0.558),vec3(0.000,0.446,0.554),vec3(0.000,0.450,0.550),vec3(0.000,0.454,0.546),vec3(0.000,0.458,0.542),vec3(0.000,0.462,0.538),vec3(0.000,0.466,0.534),vec3(0.000,0.470,0.530),vec3(0.000,0.474,0.526),vec3(0.000,0.478,0.522),vec3(0.000,0.482,0.518),vec3(0.000,0.486,0.514),vec3(0.000,0.490,0.510),vec3(0.000,0.494,0.506),vec3(0.000,0.498,0.502),vec3(0.000,0.502,0.498),vec3(0.000,0.506,0.494),vec3(0.000,0.510,0.490),vec3(0.000,0.514,0.486),vec3(0.000,0.518,0.482),vec3(0.000,0.522,0.478),vec3(0.000,0.526,0.474),vec3(0.000,0.530,0.470),vec3(0.000,0.534,0.466),vec3(0.000,0.538,0.462),vec3(0.000,0.542,0.458),vec3(0.000,0.546,0.454),vec3(0.000,0.550,0.450),vec3(0.000,0.554,0.446),vec3(0.000,0.558,0.442),vec3(0.000,0.562,0.438),vec3(0.000,0.566,0.434),vec3(0.000,0.570,0.430),vec3(0.000,0.574,0.426),vec3(0.000,0.578,0.422),vec3(0.000,0.582,0.418),vec3(0.000,0.586,0.414),vec3(0.000,0.590,0.410),vec3(0.000,0.594,0.406),vec3(0.000,0.598,0.402),vec3(0.000,0.602,0.398),vec3(0.000,0.606,0.394),vec3(0.000,0.610,0.390),vec3(0.000,0.614,0.386),vec3(0.000,0.618,0.382),vec3(0.000,0.622,0.378),vec3(0.000,0.627,0.373),vec3(0.000,0.631,0.369),vec3(0.000,0.635,0.365),vec3(0.000,0.639,0.361),vec3(0.000,0.643,0.357),vec3(0.000,0.647,0.353),vec3(0.000,0.651,0.349),vec3(0.000,0.655,0.345),vec3(0.000,0.659,0.341),vec3(0.000,0.663,0.337),vec3(0.000,0.667,0.333),vec3(0.000,0.671,0.329),vec3(0.000,0.675,0.325),vec3(0.000,0.679,0.321),vec3(0.000,0.683,0.317),vec3(0.000,0.687,0.313),vec3(0.000,0.691,0.309),vec3(0.000,0.695,0.305),vec3(0.000,0.699,0.301),vec3(0.000,0.703,0.297),vec3(0.000,0.707,0.293),vec3(0.000,0.711,0.289),vec3(0.000,0.715,0.285),vec3(0.000,0.719,0.281),vec3(0.000,0.723,0.277),vec3(0.000,0.727,0.273),vec3(0.000,0.731,0.269),vec3(0.000,0.735,0.265),vec3(0.000,0.739,0.261),vec3(0.000,0.743,0.257),vec3(0.000,0.747,0.253),vec3(0.000,0.751,0.249),vec3(0.000,0.755,0.245),vec3(0.000,0.759,0.241),vec3(0.000,0.763,0.237),vec3(0.000,0.767,0.233),vec3(0.000,0.771,0.229),vec3(0.000,0.775,0.225),vec3(0.000,0.779,0.221),vec3(0.000,0.783,0.217),vec3(0.000,0.787,0.213),vec3(0.000,0.791,0.209),vec3(0.000,0.795,0.205),vec3(0.000,0.799,0.201),vec3(0.000,0.803,0.197),vec3(0.000,0.807,0.193),vec3(0.000,0.811,0.189),vec3(0.000,0.815,0.185),vec3(0.000,0.819,0.181),vec3(0.000,0.823,0.177),vec3(0.000,0.827,0.173),vec3(0.000,0.831,0.169),vec3(0.000,0.835,0.165),vec3(0.000,0.839,0.161),vec3(0.000,0.843,0.157),vec3(0.000,0.847,0.153),vec3(0.000,0.851,0.149),vec3(0.000,0.855,0.145),vec3(0.000,0.859,0.141),vec3(0.000,0.863,0.137),vec3(0.000,0.867,0.133),vec3(0.000,0.871,0.129),vec3(0.000,0.876,0.124),vec3(0.000,0.880,0.120),vec3(0.000,0.884,0.116),vec3(0.000,0.888,0.112),vec3(0.000,0.892,0.108),vec3(0.000,0.896,0.104),vec3(0.000,0.900,0.100),vec3(0.000,0.904,0.096),vec3(0.000,0.908,0.092),vec3(0.000,0.912,0.088),vec3(0.000,0.916,0.084),vec3(0.000,0.920,0.080),vec3(0.000,0.924,0.076),vec3(0.000,0.928,0.072),vec3(0.000,0.932,0.068),vec3(0.000,0.936,0.064),vec3(0.000,0.940,0.060),vec3(0.000,0.944,0.056),vec3(0.000,0.948,0.052),vec3(0.000,0.952,0.048),vec3(0.000,0.956,0.044),vec3(0.000,0.960,0.040),vec3(0.000,0.964,0.036),vec3(0.000,0.968,0.032),vec3(0.000,0.972,0.028),vec3(0.000,0.976,0.024),vec3(0.000,0.980,0.020),vec3(0.000,0.984,0.016),vec3(0.000,0.988,0.012),vec3(0.000,0.992,0.008),vec3(0.000,0.996,0.004),vec3(0.000,1.000,0.000),vec3(0.000,1.000,0.000),vec3(0.004,0.996,0.000),vec3(0.008,0.992,0.000),vec3(0.012,0.988,0.000),vec3(0.016,0.984,0.000),vec3(0.020,0.980,0.000),vec3(0.024,0.976,0.000),vec3(0.028,0.972,0.000),vec3(0.032,0.968,0.000),vec3(0.036,0.964,0.000),vec3(0.040,0.960,0.000),vec3(0.044,0.956,0.000),vec3(0.048,0.952,0.000),vec3(0.052,0.948,0.000),vec3(0.056,0.944,0.000),vec3(0.060,0.940,0.000),vec3(0.064,0.936,0.000),vec3(0.068,0.932,0.000),vec3(0.072,0.928,0.000),vec3(0.076,0.924,0.000),vec3(0.080,0.920,0.000),vec3(0.084,0.916,0.000),vec3(0.088,0.912,0.000),vec3(0.092,0.908,0.000),vec3(0.096,0.904,0.000),vec3(0.100,0.900,0.000),vec3(0.104,0.896,0.000),vec3(0.108,0.892,0.000),vec3(0.112,0.888,0.000),vec3(0.116,0.884,0.000),vec3(0.120,0.880,0.000),vec3(0.124,0.876,0.000),vec3(0.129,0.871,0.000),vec3(0.133,0.867,0.000),vec3(0.137,0.863,0.000),vec3(0.141,0.859,0.000),vec3(0.145,0.855,0.000),vec3(0.149,0.851,0.000),vec3(0.153,0.847,0.000),vec3(0.157,0.843,0.000),vec3(0.161,0.839,0.000),vec3(0.165,0.835,0.000),vec3(0.169,0.831,0.000),vec3(0.173,0.827,0.000),vec3(0.177,0.823,0.000),vec3(0.181,0.819,0.000),vec3(0.185,0.815,0.000),vec3(0.189,0.811,0.000),vec3(0.193,0.807,0.000),vec3(0.197,0.803,0.000),vec3(0.201,0.799,0.000),vec3(0.205,0.795,0.000),vec3(0.209,0.791,0.000),vec3(0.213,0.787,0.000),vec3(0.217,0.783,0.000),vec3(0.221,0.779,0.000),vec3(0.225,0.775,0.000),vec3(0.229,0.771,0.000),vec3(0.233,0.767,0.000),vec3(0.237,0.763,0.000),vec3(0.241,0.759,0.000),vec3(0.245,0.755,0.000),vec3(0.249,0.751,0.000),vec3(0.253,0.747,0.000),vec3(0.257,0.743,0.000),vec3(0.261,0.739,0.000),vec3(0.265,0.735,0.000),vec3(0.269,0.731,0.000),vec3(0.273,0.727,0.000),vec3(0.277,0.723,0.000),vec3(0.281,0.719,0.000),vec3(0.285,0.715,0.000),vec3(0.289,0.711,0.000),vec3(0.293,0.707,0.000),vec3(0.297,0.703,0.000),vec3(0.301,0.699,0.000),vec3(0.305,0.695,0.000),vec3(0.309,0.691,0.000),vec3(0.313,0.687,0.000),vec3(0.317,0.683,0.000),vec3(0.321,0.679,0.000),vec3(0.325,0.675,0.000),vec3(0.329,0.671,0.000),vec3(0.333,0.667,0.000),vec3(0.337,0.663,0.000),vec3(0.341,0.659,0.000),vec3(0.345,0.655,0.000),vec3(0.349,0.651,0.000),vec3(0.353,0.647,0.000),vec3(0.357,0.643,0.000),vec3(0.361,0.639,0.000),vec3(0.365,0.635,0.000),vec3(0.369,0.631,0.000),vec3(0.373,0.627,0.000),vec3(0.378,0.622,0.000),vec3(0.382,0.618,0.000),vec3(0.386,0.614,0.000),vec3(0.390,0.610,0.000),vec3(0.394,0.606,0.000),vec3(0.398,0.602,0.000),vec3(0.402,0.598,0.000),vec3(0.406,0.594,0.000),vec3(0.410,0.590,0.000),vec3(0.414,0.586,0.000),vec3(0.418,0.582,0.000),vec3(0.422,0.578,0.000),vec3(0.426,0.574,0.000),vec3(0.430,0.570,0.000),vec3(0.434,0.566,0.000),vec3(0.438,0.562,0.000),vec3(0.442,0.558,0.000),vec3(0.446,0.554,0.000),vec3(0.450,0.550,0.000),vec3(0.454,0.546,0.000),vec3(0.458,0.542,0.000),vec3(0.462,0.538,0.000),vec3(0.466,0.534,0.000),vec3(0.470,0.530,0.000),vec3(0.474,0.526,0.000),vec3(0.478,0.522,0.000),vec3(0.482,0.518,0.000),vec3(0.486,0.514,0.000),vec3(0.490,0.510,0.000),vec3(0.494,0.506,0.000),vec3(0.498,0.502,0.000),vec3(0.502,0.498,0.000),vec3(0.506,0.494,0.000),vec3(0.510,0.490,0.000),vec3(0.514,0.486,0.000),vec3(0.518,0.482,0.000),vec3(0.522,0.478,0.000),vec3(0.526,0.474,0.000),vec3(0.530,0.470,0.000),vec3(0.534,0.466,0.000),vec3(0.538,0.462,0.000),vec3(0.542,0.458,0.000),vec3(0.546,0.454,0.000),vec3(0.550,0.450,0.000),vec3(0.554,0.446,0.000),vec3(0.558,0.442,0.000),vec3(0.562,0.438,0.000),vec3(0.566,0.434,0.000),vec3(0.570,0.430,0.000),vec3(0.574,0.426,0.000),vec3(0.578,0.422,0.000),vec3(0.582,0.418,0.000),vec3(0.586,0.414,0.000),vec3(0.590,0.410,0.000),vec3(0.594,0.406,0.000),vec3(0.598,0.402,0.000),vec3(0.602,0.398,0.000),vec3(0.606,0.394,0.000),vec3(0.610,0.390,0.000),vec3(0.614,0.386,0.000),vec3(0.618,0.382,0.000),vec3(0.622,0.378,0.000),vec3(0.627,0.373,0.000),vec3(0.631,0.369,0.000),vec3(0.635,0.365,0.000),vec3(0.639,0.361,0.000),vec3(0.643,0.357,0.000),vec3(0.647,0.353,0.000),vec3(0.651,0.349,0.000),vec3(0.655,0.345,0.000),vec3(0.659,0.341,0.000),vec3(0.663,0.337,0.000),vec3(0.667,0.333,0.000),vec3(0.671,0.329,0.000),vec3(0.675,0.325,0.000),vec3(0.679,0.321,0.000),vec3(0.683,0.317,0.000),vec3(0.687,0.313,0.000),vec3(0.691,0.309,0.000),vec3(0.695,0.305,0.000),vec3(0.699,0.301,0.000),vec3(0.703,0.297,0.000),vec3(0.707,0.293,0.000),vec3(0.711,0.289,0.000),vec3(0.715,0.285,0.000),vec3(0.719,0.281,0.000),vec3(0.723,0.277,0.000),vec3(0.727,0.273,0.000),vec3(0.731,0.269,0.000),vec3(0.735,0.265,0.000),vec3(0.739,0.261,0.000),vec3(0.743,0.257,0.000),vec3(0.747,0.253,0.000),vec3(0.751,0.249,0.000),vec3(0.755,0.245,0.000),vec3(0.759,0.241,0.000),vec3(0.763,0.237,0.000),vec3(0.767,0.233,0.000),vec3(0.771,0.229,0.000),vec3(0.775,0.225,0.000),vec3(0.779,0.221,0.000),vec3(0.783,0.217,0.000),vec3(0.787,0.213,0.000),vec3(0.791,0.209,0.000),vec3(0.795,0.205,0.000),vec3(0.799,0.201,0.000),vec3(0.803,0.197,0.000),vec3(0.807,0.193,0.000),vec3(0.811,0.189,0.000),vec3(0.815,0.185,0.000),vec3(0.819,0.181,0.000),vec3(0.823,0.177,0.000),vec3(0.827,0.173,0.000),vec3(0.831,0.169,0.000),vec3(0.835,0.165,0.000),vec3(0.839,0.161,0.000),vec3(0.843,0.157,0.000),vec3(0.847,0.153,0.000),vec3(0.851,0.149,0.000),vec3(0.855,0.145,0.000),vec3(0.859,0.141,0.000),vec3(0.863,0.137,0.000),vec3(0.867,0.133,0.000),vec3(0.871,0.129,0.000),vec3(0.876,0.124,0.000),vec3(0.880,0.120,0.000),vec3(0.884,0.116,0.000),vec3(0.888,0.112,0.000),vec3(0.892,0.108,0.000),vec3(0.896,0.104,0.000),vec3(0.900,0.100,0.000),vec3(0.904,0.096,0.000),vec3(0.908,0.092,0.000),vec3(0.912,0.088,0.000),vec3(0.916,0.084,0.000),vec3(0.920,0.080,0.000),vec3(0.924,0.076,0.000),vec3(0.928,0.072,0.000),vec3(0.932,0.068,0.000),vec3(0.936,0.064,0.000),vec3(0.940,0.060,0.000),vec3(0.944,0.056,0.000),vec3(0.948,0.052,0.000),vec3(0.952,0.048,0.000),vec3(0.956,0.044,0.000),vec3(0.960,0.040,0.000),vec3(0.964,0.036,0.000),vec3(0.968,0.032,0.000),vec3(0.972,0.028,0.000),vec3(0.976,0.024,0.000),vec3(0.980,0.020,0.000),vec3(0.984,0.016,0.000),vec3(0.988,0.012,0.000),vec3(0.992,0.008,0.000),vec3(0.996,0.004,0.000),vec3(1.000,0.000,0.000));

uniform int gridSizeX;
uniform int gridSizeY;
uniform int gridSizeZ;
uniform int localSize;
uniform int iterationCount;
uniform float surfaceLevel;

uint index3D(uint x, uint y, uint z)
{
    return x + y * gridSizeX + z * gridSizeX * gridSizeY;
}

// Compute Zn^2 + C.
vec2 computeNext(vec2 current, vec2 constant) {
    // Zn^2
    const float zr = current.x * current.x - current.y * current.y;
    const float zi = 2.0 * current.x * current.y;

    return vec2(zr, zi) + constant;
}

float mod2(vec2 z) {
    return dot(z, z);  // x*x + y*y
}

vec3 mandelbulbNext(vec3 z, vec3 c, float power)
{
    float r = length(z);

    float theta = acos(z.z / r);
    float phi = atan(z.y, z.x);

    float zr = pow(r, power);

    theta *= power;
    phi *= power;

    return zr * vec3(
    sin(theta) * cos(phi),
    sin(theta) * sin(phi),
    cos(theta)
    ) + c;
}

float computeIterationsSmooth3D(vec3 z0, vec3 c, int maxIterations)
{
    vec3 z = z0;
    int iteration = 0;

    while(length(z) < 4.0 && iteration < maxIterations)
    {
        z = mandelbulbNext(z, c, 8.0);
        iteration++;
    }

    float r = length(z);
    float smoothV = float(iteration) - log2(log2(r));

    return smoothV;
}

vec3 interpolateVerts(vec3 p1, vec3 p2, float valp1, float valp2)
{
    float t = (surfaceLevel - valp1) / (valp2 - valp1);
    return p1 + t * (p2 - p1);
}
        
bool isGround(float value) {
    return value < surfaceLevel;
}

float GetValue(float x, float y, float z) {
    vec3 coord = (vec3(x, y, z) / float(max(gridSizeX, max(gridSizeY, gridSizeZ))) * 2.0 - 1.0) * 1.3;
    float iterations = computeIterationsSmooth3D(coord, coord, iterationCount);
    return iterations / iterationCount;
}

vec3 gradientColor(float a, float b, float c)
{
    // Normalize density around the surface level
    float average = (a + b + c) * 0.333;
    float t = clamp((average - surfaceLevel) * 100.0, 0.0, 1.0);
    int idx = int(clamp(t * 499.0, 0.0, 499.0));
    vec3 color = colors[idx];
    return color;
}

// Adds the vertex to the list. This does not do the atomic addition.
void addVertex(Vertex vertex, uint index) {
    vertices[index] = vertex.position.x;
    vertices[index + 1] = vertex.position.y;
    vertices[index + 2] = vertex.position.z;
    vertices[index + 3] = vertex.normal.x;
    vertices[index + 4] = vertex.normal.y;
    vertices[index + 5] = vertex.normal.z;
    vertices[index + 6] = vertex.color.x;
    vertices[index + 7] = vertex.color.y;
    vertices[index + 8] = vertex.color.z;
}

void main() {
    uvec3 id = gl_GlobalInvocationID;
    if (id.x >= gridSizeX - 1 ||
        id.y >= gridSizeY - 1 ||
        id.z >= gridSizeZ - 1)
        return;

    float West = id.x; float East = id.x + 1.0;
    float Bottom = id.y; float Top = id.y + 1.0;
    float South = id.z; float North = id.z + 1.0;
    float WBSLevel = GetValue(West, Bottom, South); bool WBSGround = isGround(WBSLevel);
    float WBNLevel = GetValue(West, Bottom, North); bool WBNGround = isGround(WBNLevel);
    float EBSLevel = GetValue(East, Bottom, South); bool EBSGround = isGround(EBSLevel);
    float EBNLevel = GetValue(East, Bottom, North); bool EBNGround = isGround(EBNLevel);
    float WTSLevel = GetValue(West, Top, South); bool WTSGround = isGround(WTSLevel);
    float WTNLevel = GetValue(West, Top, North); bool WTNGround = isGround(WTNLevel);
    float ETSLevel = GetValue(East, Top, South); bool ETSGround = isGround(ETSLevel);
    float ETNLevel = GetValue(East, Top, North); bool ETNGround = isGround(ETNLevel);

    int cubeIndex = 0;
    bool values[8] = {WBSGround, EBSGround, EBNGround, WBNGround, WTSGround, ETSGround, ETNGround, WTNGround};
    for (int i = 0; i < 8; i++) {
        if (values[i]) {
            cubeIndex |= 1 << i;
        }
    }

    vec3[8] corners = vec3[8](
        vec3(West, Bottom, South),  // 0
        vec3(East, Bottom, South),  // 1
        vec3(East, Bottom, North),  // 2
        vec3(West, Bottom, North),  // 3
        vec3(West, Top, South),     // 4
        vec3(East, Top, South),     // 5
        vec3(East, Top, North),     // 6
        vec3(West, Top, North)      // 7
    );

    float[8] levels = {WBSLevel, EBSLevel, EBNLevel, WBNLevel, WTSLevel, ETSLevel, ETNLevel, WTNLevel};
    const int[16] triangulation = triTable[cubeIndex];

    for (int i = 0; triangulation[i] != -1; i += 3) {
        int edgeIndex = triangulation[i];
        if (edgeIndex == -1) {
            continue;
        }

        // Get indices of corner points A and B for each of the three edges
        // of the cube that need to be joined to form the triangle.
        int a0 = cornerIndexAFromEdge[triangulation[i]];
        int b0 = cornerIndexBFromEdge[triangulation[i]];

        int a1 = cornerIndexAFromEdge[triangulation[i+1]];
        int b1 = cornerIndexBFromEdge[triangulation[i+1]];

        int a2 = cornerIndexAFromEdge[triangulation[i+2]];
        int b2 = cornerIndexBFromEdge[triangulation[i+2]];

        vec3 posA = interpolateVerts(corners[a0], corners[b0], levels[a0], levels[b0]) / 32.0;
        vec3 posB = interpolateVerts(corners[a1], corners[b1], levels[a1], levels[b1]) / 32.0;
        vec3 posC = interpolateVerts(corners[a2], corners[b2], levels[a2], levels[b2]) / 32.0;

        // Gradient-based colors
        float valA = (levels[a0] + levels[b0]) * 0.5;
        float valB = (levels[a1] + levels[b1]) * 0.5;
        float valC = (levels[a2] + levels[b2]) * 0.5;

        vec3 color = gradientColor(valA, valB, valC);
        Vertex vertexA = Vertex(posA, vec3(1.0), color);
        Vertex vertexB = Vertex(posB, vec3(1.0), color);
        Vertex vertexC = Vertex(posC, vec3(1.0), color);

        vec3 normal = computeNormal(
            vertexC.position.x, vertexC.position.y, vertexC.position.z,
            vertexB.position.x, vertexB.position.y, vertexB.position.z,
            vertexA.position.x, vertexA.position.y, vertexA.position.z
        );
        vertexA.normal = vec3(normal);
        vertexB.normal = vec3(normal);
        vertexC.normal = vec3(normal);

        uint baseIndex = atomicAdd(floatCount, 3 * 9);
        addVertex(vertexC, baseIndex);
        addVertex(vertexB, baseIndex + 9);
        addVertex(vertexA, baseIndex + 18);
    }
}
