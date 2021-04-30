#pragma once
#include "Rendering/Objects/GraphicsObject.h"
#include <hlslmath.h>
#include <KTX/ktx.h>

namespace PK::Rendering::Objects
{
    using namespace PK::Math;

    struct TextureDescriptor
    {
        uint3 resolution = CG_UINT3_ZERO;
        GLenum colorFormat = GL_RGBA8;
        GLenum dimension = GL_TEXTURE_2D;
        GLenum wrapmodex = GL_REPEAT;
        GLenum wrapmodey = GL_REPEAT;
        GLenum wrapmodez = GL_REPEAT;
        GLenum filtermag = GL_LINEAR;
        GLenum filtermin = GL_LINEAR;
        uint miplevels = 0;
        uint msaaSamples = 0;
    };
    
    struct ImageBindDescriptor
    {
        GraphicsID graphicsId = 0;
        GLenum format = GL_RGBA32F;
        GLenum access = GL_READ_WRITE;
        int level = 0;
        int layer = 0;
        bool layered = false;
    };

    class Texture : public GraphicsObject
    {
        public:
            Texture(const TextureDescriptor& descriptor);
            virtual ~Texture() = default;
    
            inline TextureDescriptor GetDescriptor() const { return m_descriptor; }
            inline uint2 GetResolution2D() const { return m_descriptor.resolution.xy; }
            inline uint3 GetResolution3D() const { return m_descriptor.resolution; }
            inline uint GetWidth() const { return m_descriptor.resolution.x; }
            inline uint GetHeight() const { return m_descriptor.resolution.y; }
            inline uint GetDepth() const { return m_descriptor.resolution.z; }
            inline uint GetSize() const { return m_descriptor.resolution.x * m_descriptor.resolution.y * m_descriptor.resolution.z * GetTexelSize(m_descriptor.colorFormat); }
            inline uint8_t GetChannelCount() const { return GetChannelCount(m_channels); }
            inline GLenum GetDimension() const { return m_descriptor.dimension; }

            inline GLuint64 GetBindlessHandle() const { return glGetTextureHandleARB(m_graphicsId); }
            GLuint64 GetBindlessHandleResident() const 
            { 
                auto handle = glGetTextureHandleARB(m_graphicsId);

                if (!glIsTextureHandleResidentARB(handle))
                {
                    glMakeTextureHandleResidentARB(handle);
                }
                return handle; 
            }
            inline void MakeHandleResident() const { glMakeTextureHandleResidentARB(glGetTextureHandleARB(m_graphicsId)); }
            inline void MakeHandleNonResident() const { glMakeTextureHandleNonResidentARB(glGetTextureHandleARB(m_graphicsId)); }

            inline ImageBindDescriptor GetImageBindDescriptor(GLenum access, int level, int layer, bool layered) const { return { m_graphicsId, m_descriptor.colorFormat, access, level, layer, layered }; }
            inline GLuint64 GetImageHandle(GLenum format, int level, int layer, bool layered) const { return glGetImageHandleARB(m_graphicsId, level, layered, layer, format); }
            GLuint64 GetImageHandleResident(GLenum format, GLenum access, int level, int layer, bool layered) const 
            { 
                auto handle = glGetImageHandleARB(m_graphicsId, level, layered, layer, format);

                if (!glIsImageHandleResidentARB(handle))
                {
                    glMakeImageHandleResidentARB(handle, access);
                }

                return handle;
            }
            inline void MakeImageHandleResident(GLenum format, GLenum access, int level, int layer, bool layered) const { glMakeImageHandleResidentARB(glGetImageHandleARB(m_graphicsId, level, layered, layer, format), access); }
            inline void MakeImageHandleNonResident(GLenum format, int level, int layer, bool layered) const { glMakeImageHandleNonResidentARB(glGetImageHandleARB(m_graphicsId, level, layered, layer, format)); }

            void Clear(uint level, const void* clearValue) const;
            void SetWrapMode(GLenum x, GLenum y, GLenum z);
            void SetFilterMode(GLenum min, GLenum mag);
    
        protected:
            void SetDescriptor(const TextureDescriptor& descriptor);
    
            static void CreateTextureStorage(GraphicsID& graphicsId, const TextureDescriptor& descriptor);
            static GLenum GetFormatChannels(GLenum format);
            static GLenum GetFormatDataType(GLenum format);
            static GLenum GetWrapmodeFromString(const char* string);
            static uint8_t GetTexelSize(GLenum format);
            static uint8_t GetChannelCount(GLenum channels);
            static void GetDescirptorFromKTX(ktxTexture* tex, TextureDescriptor* desc, GLenum* channels);
    
            TextureDescriptor m_descriptor;
            GLenum m_channels;
    };
}