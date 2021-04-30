#pragma once
#include "Utilities/Ref.h"
#include "Core/NoCopy.h"
#include "ECS/EntityDatabase.h"
#include "ECS/Contextual/EntityViews/EntityViews.h"
#include "Rendering/Batching.h"
#include "Rendering/Culling.h"
#include "Rendering/Objects/Buffer.h"
#include "Rendering/Objects/RenderTexture.h"
#include "Rendering/Objects/TextureXD.h"
#include <hlslmath.h>

namespace PK::Rendering
{
    using namespace PK::Math;
    using namespace PK::Rendering::Objects;

    struct ShadowmapLightTypeData
    {
        Utilities::Ref<RenderTexture> SceneRenderTarget;
        Shader* ShaderRenderShadows = nullptr;
        Shader* ShaderBlur = nullptr;
        uint viewFirst = 0;
        uint viewCount = 0;
        uint atlasBaseIndex = 0;
        uint maxBatchSize = 0;
    };
    
    struct ShadowmapData
    {
        ShadowmapLightTypeData LightIndices[(int)LightType::TypeCount];
        Batching::IndexedMeshBatchCollection Batches;
        Utilities::Ref<RenderTexture> MapIntermediate;
        Utilities::Ref<RenderTexture> ShadowmapAtlas;

        static constexpr uint BatchSize = 4;
        static constexpr uint TileSize = 512;
        static constexpr uint TileCountPerAxis = 8; 
        static constexpr uint TotalTileCount = 8 * 8; 
        static constexpr float CascadeLinearity = 0.5f;
    };

    class LightsManager : public PK::Core::NoCopy
    {
        public:
            LightsManager(AssetDatabase* assetDatabase);

            void Preprocess(PK::ECS::EntityDatabase* entityDb, Core::BufferView<uint> visibleLights, const uint2& resolution, const float4x4& inverseViewProjection, float zNear, float zFar);

            void UpdateLightTiles(const uint2& resolution);

            void DrawDebug();

            const Ref<RenderTexture>& GetShadowmapAtlas() const { return m_shadowmapData.ShadowmapAtlas; }

        private:
            void UpdateShadowmaps(PK::ECS::EntityDatabase* entityDb, const float4x4& inverseViewProjection, float znear, float zfar);
            void UpdateLightBuffers(PK::ECS::EntityDatabase* entityDb, Core::BufferView<uint> visibleLights, const float4x4& inverseViewProjection, float znear, float zfar);

            const uint MaxLightsPerTile = 64;
            const uint GridSizeX = 16;
            const uint GridSizeY = 9;
            const uint GridSizeZ = 24;
            const uint ClusterCount = GridSizeX * GridSizeY * GridSizeZ;

            std::vector<PK::ECS::EntityViews::LightRenderable*> m_visibleLights;
            uint m_visibleLightCount;

            ShaderPropertyBlock m_properties;
            ShadowmapData m_shadowmapData;

            Shader* m_computeLightAssignment;
            Shader* m_computeDepthTiles;
            Shader* m_debugVisualize;

            Utilities::Ref<ComputeBuffer> m_lightsBuffer;
            Utilities::Ref<ComputeBuffer> m_lightMatricesBuffer;
            Utilities::Ref<ComputeBuffer> m_lightDirectionsBuffer;
            Utilities::Ref<ComputeBuffer> m_globalLightsList;
            Utilities::Ref<ComputeBuffer> m_globalLightIndex;
            Utilities::Ref<RenderBuffer> m_lightTiles;
            Utilities::Ref<ComputeBuffer> m_depthTiles;
    };
}