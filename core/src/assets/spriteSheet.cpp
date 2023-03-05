#include <core/assets/spriteSheet.hpp>
#include <utils/logging.hpp>

#include <core/utils/json_impl.hpp>

#include <filesystem>

namespace core::assets
{
    static void loadAsepriteArraySheet(SpriteSheet::SpriteData &spriteData) { L_TAG("loadAsepriteArraySheet"); }

    static SpriteSheet::SpriteData loadSpriteSheet(const AssetName &name)
    {
        L_TAG("SpriteSheet(assetName)");
        SpriteSheet::SpriteData spriteData;

        return spriteData;
    }

    SpriteSheet::SpriteSheet()  = default;
    SpriteSheet::~SpriteSheet() = default;

    SpriteSheet::SpriteSheet(const AssetName &name)
    {
        L_TAG("SpriteSheet(assetName)");
        using json = core::utils::json;

        auto resolveMetafileFunc = [](const AssetName &name) {
            // resolve paths for sprite sheet
            const auto &assetPaths = AssetInventory::getInstance().lookupAssets(AssetType::SpriteSheet, name);

            // look for json file in assetPaths
            auto spriteMeta = std::find_if(assetPaths.begin(), assetPaths.end(), [](const auto &path) {
                constexpr const char jsonExt[] = ".json";
                return std::strncmp(path.c_str() + path.find_last_of("."), jsonExt, std::strlen(jsonExt)) == 0 ? true
                                                                                                               : false;
            });

            if (spriteMeta == assetPaths.end())
            {
                L_THROW_RUNTIME("Could not find sprite sheet metadata: {}", name);
            }

            return *spriteMeta;
        };

        std::string metaFilepath = resolveMetafileFunc(name);

        // verify that the file exists
        if (!std::filesystem::exists(metaFilepath))
        {
            L_THROW_RUNTIME("Could not find sprite sheet json: {}", metaFilepath);
        }

        // parse json file
        auto fileParserFunc = [](const AssetName &name) {
            std::ifstream f(name);
            return json::parse(f);
        };
        // load and parse metafile
        json jsonData = fileParserFunc(metaFilepath);

        // Parse metadata
        L_TRACE("Parsing spritesheet for {}", metaFilepath);
        try
        {
            json metadata = jsonData["meta"];

            m_sheetData.sheetName     = name;
            m_sheetData.texture       = metadata.at("image");
            m_sheetData.textureFormat = metadata.at("format");

            L_TRACE("Image: {}", m_sheetData.texture);
            L_TRACE("ImageFormat: {}", m_sheetData.textureFormat);

            // parse frames tags
            {
                json frameTags = metadata.at("frameTags");
                m_sheetData.tags.reserve(frameTags.size());
                L_TRACE("Tags: {}", frameTags.size());

                for (auto &frameTag : frameTags)
                {
                    std::pair<std::string, Tag> node;

                    frameTag.at("name").get_to(node.first);
                    frameTag.at("from").get_to(node.second.from);
                    frameTag.at("to").get_to(node.second.to);
                    node.second.direction = frameTag.at("direction") == "forward" ? true : false;

                    L_TRACE("\tName: {}, from: {}, to: {}, forward: {}",
                            name,
                            node.second.from,
                            node.second.to,
                            node.second.direction);
                    m_sheetData.tags.insert(std::move(node));
                }
            }
            // parse layers
            {
                json layers = metadata.at("layers");
                m_sheetData.layers.reserve(layers.size());
                L_TRACE("Layers: {}", layers.size());

                for (auto &layer : layers)
                {
                    std::pair<std::string, Layer> node;

                    layer.at("name").get_to(node.first);
                    layer.at("opacity").get_to(node.second.opacity);
                    layer.at("blendMode").get_to(node.second.blendMode);

                    L_TRACE("\tName: {}, opacity: {}, blendMode: {}", name, node.second.opacity, node.second.blendMode);
                    m_sheetData.layers.insert(std::move(node));
                }
            }
            // parse frames
            {
                json frames = jsonData["frames"];
                m_sheetData.frames.resize(frames.size());
                L_TRACE("Frames: {}", frames.size());

                for (auto &frame : frames)
                {
                    Frame fr;

                    frame.at("filename").get_to(fr.name);
                    frame.at("rotated").get_to(fr.rotated);
                    frame.at("trimmed").get_to(fr.trimmed);
                    frame.at("frame").at("x").get_to(fr.frame.x);
                    frame.at("frame").at("y").get_to(fr.frame.y);
                    frame.at("frame").at("w").get_to(fr.frame.w);
                    frame.at("frame").at("h").get_to(fr.frame.h);
                    frame.at("spriteSourceSize").at("x").get_to(fr.spriteSourceSize.x);
                    frame.at("spriteSourceSize").at("y").get_to(fr.spriteSourceSize.y);
                    frame.at("spriteSourceSize").at("w").get_to(fr.spriteSourceSize.w);
                    frame.at("spriteSourceSize").at("h").get_to(fr.spriteSourceSize.h);
                    frame.at("sourceSize").at("w").get_to(fr.sourceSize.w);
                    frame.at("sourceSize").at("h").get_to(fr.sourceSize.h);
                    frame.at("duration").get_to(fr.duration);

                    // Extract layer name and tag name from filename
                    std::string layerName, tagName;
                    {
                        int         i = -1, s = -1;
                        const char *c = fr.name.c_str();
                        for (int pos = 0; pos < name.length(); pos++)
                        {
                            char x = c[pos];
                            if (s < 0 && c[pos] == '(')
                                s = pos + 1;
                            else if (s > 0 && c[pos] == ')')
                            {
                                if (i < 0)
                                    layerName = fr.name.substr(s, i = pos - s);
                                else
                                {
                                    tagName = fr.name.substr(s, pos - s);
                                    break;
                                }
                                s = -1;
                            }
                        }
                    }

                    L_TRACE("\tName: {}", fr.name);
                    L_TRACE("\t  Rotated: {}", fr.rotated ? "true" : "false");
                    L_TRACE("\t  Trimmed: {}", fr.trimmed ? "true" : "false");
                    L_TRACE("\t  Frame:            {{ X: {:4}, Y: {:4}, W: {:4}, H: {:4} }}",
                            fr.frame.x,
                            fr.frame.y,
                            fr.frame.w,
                            fr.frame.h);
                    L_TRACE("\t  SpriteSourceSize: {{ X: {:4}, Y: {:4}, W: {:4}, H: {:4} }}",
                            fr.spriteSourceSize.x,
                            fr.spriteSourceSize.y,
                            fr.spriteSourceSize.w,
                            fr.spriteSourceSize.h);
                    L_TRACE("\t  SourceSize:       {{ W: {:4}, H: {:4} }}", fr.sourceSize.w, fr.sourceSize.h);
                    L_TRACE("\t  Duration: {:4} ms", fr.duration);
                }
            }
        }
        catch (std::exception &ex)
        {
            L_THROW_RUNTIME("Exception during spritesheet parsing: {}", ex.what());
        }
        // m_sheetData.texturePath = json["meta"]

        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    const std::string &SpriteSheet::getTextureName() const { return m_sheetData.texture; }
    const SpriteSheet::SpriteData &SpriteSheet::getSpriteData() const { return m_sheetData; }
} // namespace core::assets