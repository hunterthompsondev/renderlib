struct VertexInput
{
    float2 position : POSITION;
};

struct VertexOutput
{
    float4 position : SV_Position;
    float3 color    : COLOR0;
};

static const float3 colors[3] = float3[](
    float3(1.0, 0.0, 0.0),
    float3(0.0, 1.0, 0.0),
    float3(0.0, 0.0, 1.0)
);

[shader("vertex")]
VertexOutput vertexMain(VertexInput input, uint vertexIndex : SV_VertexID)
{

    VertexOutput output;
    output.position = float4(input.position, 0.0, 1.0);
    output.color = colors[vertexIndex % 3];
    return output;
}

[shader("fragment")]
float4 fragmentMain(VertexOutput input) : SV_Target
{
    return float4(input.color, 1.0);
}
