/*
    This .hlsl file is for sample test.
*/

cbuffer CameraBuffer : register(b0)
{
    matrix gView;
    matrix gProjection;
}

cbuffer Instance : register(b1)
{
    matrix gWorld;
}

struct VertexInput
{
    float4 mPosition : POSITION0;
    float3 mNormal : NORMAL0;
    float2 mTexcoord : TEXCOORD0;
};

struct PixelInput
{
    float4 mPosition : SV_Position;
    float4 mNormal : TEXCOORD0;
    float2 mTexcoord : TEXCOORD1;
};

PixelInput SampleVS(VertexInput input)
{
    PixelInput output = (PixelInput) 0;
    
    input.mPosition.w = 1.0f;
    
    float4 world = mul(input.mPosition, gWorld);
    float4 view = mul(world, gView);
    float4 projection = mul(view, gProjection);
    
    
    float4 normal = float4(mul(input.mNormal, (float3x3) gWorld), 1.0f);
    
    float2 uv = input.mTexcoord;
    
    output.mPosition = projection;
    output.mNormal = normal;
    output.mTexcoord = uv;
    
    return output;
}

float4 SamplePS(PixelInput input) : SV_Target0
{
    float4 position = input.mPosition;
    float4 normal = input.mNormal;
    float2 uv = input.mTexcoord;
    
    return position;
}