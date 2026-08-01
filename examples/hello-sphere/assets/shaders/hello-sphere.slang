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
    float4x4 model;
};

[[vk::binding(0, 0)]] ConstantBuffer<CameraUniforms> camera;
[[vk::binding(1, 0)]] Sampler2D colorGrid;
[[vk::push_constant]] ObjectPushConstants object;

[[shader("vertex")]]
VertexOutput vertexMain(VertexInput input)
{
    VertexOutput output;
    output.position = mul(camera.viewProjection, mul(object.model, float4(input.position, 1.0)));
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
