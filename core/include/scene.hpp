#pragma once

#include <bitset>

class SceneScript {
private:
protected:
public:
};

enum class SceneEvent {
    INIT,

};

class Scene {
private:
protected:
public:
    Scene() {}

    /** Called during when the scene is switched in */
    virtual void init() = 0;
    /** Called every start of frame */
    virtual void preUpdate() = 0;
    /** Called on every frame based on unscaled time */
    virtual void fixedUpdate() = 0;
    /** Called every frame based on scaled time */
    virtual void update() = 0;
    /** Called every frame after all update is finished */
    virtual void postUpdate() = 0;
    /** Called when the scene is switched out */
    virtual void clean() = 0;

    /** Attach a script to the scene */
    // void a

};