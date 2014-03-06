
#include "FW1FontWrapper/FW1FontWrapper.h"

class D3D11Font : GraphicsAsset
{
public:
    D3D11Font()
    {
        m_factory = NULL;
        m_fontWrapper = NULL;
    }
    ~D3D11Font()
    {
        SAFE_RELEASE(m_factory);
        SAFE_RELEASE(m_fontWrapper);
    }

    void init(GraphicsDevice &g, const std::string &fontName)
    {
        g.castD3D11().registerAsset(this);
        m_fontName = std::wstring(fontName.begin(), fontName.end());
        reset(g);
    }

    void drawString(GraphicsDevice &g, const std::string &text, const vec2i &pos, const float fontHeight, RGBColor color)
    {
        std::wstring wText(text.begin(), text.end());
        m_fontWrapper->DrawString(
            &g.castD3D11().context(),
            wText.c_str(),
            fontHeight,
            (FLOAT)pos.x,
            (FLOAT)pos.y,
            0xff0000ff,// Text color, 0xAaBbGgRr
            FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
            );
    }

    void release(GraphicsDevice &g)
    {
        SAFE_RELEASE(m_factory);
        SAFE_RELEASE(m_fontWrapper);
    }
    void reset(GraphicsDevice &g)
    {
        //Console::log() << "resetting fonts" << std::endl;
        release(g);
        D3D_VALIDATE(FW1CreateFactory(FW1_VERSION, &m_factory));
        D3D_VALIDATE(m_factory->CreateFontWrapper(&g.castD3D11().device(), m_fontName.c_str(), &m_fontWrapper));
    }

private:
    IFW1Factory *m_factory;
    IFW1FontWrapper *m_fontWrapper;
    std::wstring m_fontName;
};