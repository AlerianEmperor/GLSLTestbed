#pragma once
#include PKCommon.glsl
#include Noise.glsl

#define VOLUME_DEPTH 128
#define VOLUME_WIDTH 160
#define VOLUME_HEIGHT 90
#define VOLUME_DEPTH_FLOAT 127.0f
#define VOLUME_SIZE_XY float2(159.0f, 89.0f)
#define VOLUME_SIZE_XYZ float3(159.0f, 89.0f, 127.0f)

PK_DECLARE_CBUFFER(pk_VolumeResources)
{
    float4 pk_Volume_WindDir;
    float pk_Volume_ConstantFog;
    float pk_Volume_HeightFogExponent;
    float pk_Volume_HeightFogOffset;
    float pk_Volume_HeightFogAmount;
    float pk_Volume_Density;
    float pk_Volume_Intensity;
    float pk_Volume_Anisotropy;
    float pk_Volume_NoiseFogAmount;
    float pk_Volume_NoiseFogScale;
    float pk_Volume_WindSpeed;
    sampler3D pk_Volume_ScatterRead;
};

layout(rgba16f) uniform image3D pk_Volume_Inject;
layout(rgba16f) uniform image3D pk_Volume_Scatter;

float GetVolumeCellDepth(float index)
{
    return pk_ProjectionParams.x * pow(pk_ExpProjectionParams.z, index / VOLUME_DEPTH);
}

float GetVolumeWCoord(float depth)
{
    return max(log2(depth) * pk_ExpProjectionParams.x + pk_ExpProjectionParams.y, 0.0);
}

float GetVolumeSliceWidth(int index)
{
    float near = GetVolumeCellDepth(index);
    float far = GetVolumeCellDepth(index + 1);
    return (far - near) / pk_ProjectionParams.z;
}


float3 GetVolumeCellNoise(uint3 id)
{
    return GlobalNoiseBlue(id.xy + id.z * int2(VOLUME_WIDTH, VOLUME_HEIGHT) + int(pk_Time.w * 1000).xx);
}