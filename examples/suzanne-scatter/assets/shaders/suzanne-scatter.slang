struct VertexInput
{
    [[vk::location(0)]] float3 position : POSITION;
    [[vk::location(1)]] float2 uv : TEXCOORD0;
    [[vk::location(2)]] float3 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

struct CameraUniforms
{
    float4x4 view;
    float4x4 projection;
    float4x4 viewProjection;
};

struct ObjectPushConstants
{
    uint32_t timeMs;
};

struct InstanceData
{
    float4x4 model;
};

[[vk::binding(0, 0)]] ConstantBuffer<CameraUniforms> camera;
[[vk::binding(1, 0)]] StructuredBuffer<InstanceData> instances;
[[vk::binding(2, 0)]] Sampler2D colorGrid;
[[vk::push_constant]] ObjectPushConstants object;

float hash(uint x)
{
    x ^= x >> 16;
    x *= 0x7feb352du;
    x ^= x >> 15;
    x *= 0x846ca68bu;
    x ^= x >> 16;

    return float(x) / 4294967295.0;
}

float3 randomAxis(uint seed)
{
    float x = hash(seed);
    float y = hash(seed + 1);
    float z = hash(seed + 2);

    return normalize(float3(
        x * 2.0 - 1.0,
        y * 2.0 - 1.0,
        z * 2.0 - 1.0
    ));
}

float4x4 axisAngleRotation(float3 axis, float angle)
{
    float s = sin(angle);
    float c = cos(angle);
    float t = 1.0 - c;

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return float4x4(
        t*x*x + c,     t*x*y - s*z,   t*x*z + s*y,   0.0,
        t*x*y + s*z,   t*y*y + c,     t*y*z - s*x,   0.0,
        t*x*z - s*y,   t*y*z + s*x,   t*z*z + c,     0.0,
        0.0,           0.0,           0.0,           1.0
    );
}


[[shader("vertex")]]
VertexOutput vertexMain(
    VertexInput input,
    uint instanceIndex : SV_InstanceID)
{
    VertexOutput output;

    float4x4 model = instances[instanceIndex].model;
    float timeSeconds = float(object.timeMs) * 0.001;
    float3 axis = randomAxis(instanceIndex);
    float speed = 0.5 + hash(instanceIndex + 3) * 1.5;
    float angle = timeSeconds * speed;
    float4x4 rotation = axisAngleRotation(axis, angle);

    float4x4 animatedModel = mul(model, rotation);
    float4 worldPosition = mul(animatedModel, float4(input.position, 1.0));

    output.position = mul(camera.viewProjection, worldPosition);
    output.uv = input.uv;
    output.normal = input.normal;

    return output;
}


[[shader("fragment")]]
float4 fragmentMain(VertexOutput input) : SV_Target
{
    float2 uv = float2(input.uv.x, 1.0 - input.uv.y);

    return colorGrid.Sample(uv);
}
