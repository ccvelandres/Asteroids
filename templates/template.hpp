#pragma once

/**
 * @file template.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 */

/**
 * @addtogroup OpenGL
 * @ingroup Renderer
 * @{
 */

/**
 * @brief Template class documentation
 *
 */
class Template
{
private:
protected:
public:
    Template();
    ~Template();
    Template(Template &o)             = default;
    Template &operator=(Template &o)  = default;
    Template(Template &&o)            = default;
    Template &operator=(Template &&o) = default;
    
};

/** @} endgroup ECS */