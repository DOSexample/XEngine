#pragma once

class XTexture;
class XShader;
class XTransform;
class XRenderState {
public:
    enum class BlendMode {
        Opaque,         // No blending.
        AlphaBlend,     // Standard alpha blending.
        Additive,       // Additive blending.
        Multiply,       // Multiplicative blending.
        Screen,         // Screen blending.
        Subtractive,    // Subtractive blending.
        Replace,        // The "Replace" blend mode simply means that the source color replaces the destination color without any consideration for alpha values or other blending equations.
    };

    enum class DepthTest {    
        Never,          // Never passes the depth test.
        Less,           // Passes if the incoming depth value is less than the stored depth value.
        LessEqual,      // Passes if the incoming depth value is less than or equal to the stored depth value.
        Equal,          // Passes if the incoming depth value is equal to the stored depth value.
        Greater,        // Passes if the incoming depth value is greater than the stored depth value.
        GreaterEqual,   // Passes if the incoming depth value is greater than or equal to the stored depth value.
        NotEqual,       // Passes if the incoming depth value is not equal to the stored depth value.
        Always,         // Always passes the depth test, irrespective of depth value.
    };

    enum class CullMode {
        Never,          // Never passes the cull mode
        None,
        Front,
        Back,
    };

    enum class AlphaTestFunc {
        Never,          // Pixel is never drawn, regardless of alpha value.
        Less,           // Pixel is drawn if its alpha value is less than the reference alpha.
        LessEqual,      // Pixel is drawn if its alpha value is less than or equal to the reference alpha.
        Equal,          // Pixel is drawn if its alpha value is equal to the reference alpha.
        Greater,        // Pixel is drawn if its alpha value is greater than the reference alpha.
        NotEqual,       // Pixel is drawn if its alpha value is not equal to the reference alpha.
        GreaterEqual,   // Pixel is drawn if its alpha value is greater than or equal to the reference alpha.
        Always          // Pixel is always drawn, regardless of alpha value.
    };

private:
    bool blendEnabled;
    BlendMode blendMode;
    DepthTest depthTest;
    CullMode cullMode;
    bool depthWriteEnabled;

    bool alphaTestEnabled;
    AlphaTestFunc alphaTestFunc;
    int alphaReferenceValue;

public:
    XRenderState();


    // Setters
    void SetBlendMode(BlendMode mode) {
        blendMode = mode;
    }

    void SetDepthTest(DepthTest test) {
        depthTest = test;
    }

    void SetCullMode(CullMode mode) {
        cullMode = mode;
    }

    void SetDepthWrite(bool enabled) {
        depthWriteEnabled = enabled;
    }

    void EnableAlphaTest(bool enabled) {
        alphaTestEnabled = enabled;
    }

    void SetAlphaTestFunc(AlphaTestFunc func) {
        alphaTestFunc = func;
    }

    void SetAlphaReferenceValue(int value) {
        alphaReferenceValue = value;
    }

    void Apply(XTransform* transform = 0, XShader* xs = 0, XTexture* pDiffuseMap = 0, XTexture* pNormalMap = 0, XTexture* pSpecularMap = 0);

};