#pragma once

#include <renderer/common/vertex.hpp>
#include <renderer/common/mesh.hpp>
#include <time.hpp>

class Renderer {
private:
protected:
public:
    virtual ~Renderer() = default;
    virtual void init() = 0;
    virtual void update(const time_ms delta) = 0;
    virtual void clean() = 0;
    virtual void refresh() = 0;

    virtual bool renderBegin() = 0;
    virtual void render(const std::vector<Mesh> &meshes) = 0;
    virtual void renderEnd() = 0;
};