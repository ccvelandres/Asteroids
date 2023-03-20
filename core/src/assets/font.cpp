#include <assets/font.hpp>
#include <assets/font_p.hpp>
#include <utils/logging.hpp>

#include <glm/gtx/string_cast.hpp>

namespace core::assets
{
    Font::Font(const AssetName &name) : m_internal(std::make_unique<Internal>())
    {
        L_TAG("Font::Font");
    }

    Font::Font() : m_internal(std::make_unique<Internal>())
    {
        L_TAG("Font::Font");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    Font::Font(std::unique_ptr<Internal> internal) : m_internal(std::move(internal))
    {
        L_TAG("Font::Font(Internal)");
        L_TRACE("Internal resources for ({}) moved from ({})",
                static_cast<void *>(this),
                static_cast<void *>(internal.get()));
    }
    
    Font::~Font()
    {
        L_TAG("Font::Font");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }

    Font::Font(Font &&o)            = default;
    Font &Font::operator=(Font &&o) = default;

    const std::string &Font::name() const noexcept { return this->m_internal->m_name; }
} // namespace core::assets