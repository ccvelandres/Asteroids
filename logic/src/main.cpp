#include <iostream>
#include <map>
#include <thread>
#include <typeindex>

#include <SDL2/SDL.h>

#include <core/game.hpp>
#include <core/event.hpp>
#include <core/ecs/entity.hpp>
#include <core/ecs/entityManager.hpp>
#include <core/ecs/components.hpp>
#include <core/graphics/renderer/renderer.hpp>
#include <core/utils/logging.hpp>
#include <core/assets/asset.hpp>
#include <core/assets/model.hpp>

#include <glm/gtx/string_cast.hpp>

#include <assimp/LogStream.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

constexpr int   windowWidth  = 640;
constexpr int   windowHeight = 480;
constexpr int   originX      = windowWidth / 2;
constexpr int   originY      = windowHeight / 2;
AudioComponent *audioComponent;

class Crate : public Entity
{
    TransformComponent *transform;
    MeshRenderer       *mesh;
public:
    Crate(const glm::vec3 &pos)
    {
        transform = &this->addComponent<TransformComponent>();
        transform->setPosition(pos);

        AssetID crateMesh    = Game::assetManager()->loadAsset(AssetType::Mesh, "crate.obj");
        AssetID crateTexture = Game::assetManager()->loadAsset(AssetType::Texture, "crate.png");
        mesh                 = &this->addComponent<MeshRenderer>(crateMesh, crateTexture);
    }
};

class CameraObject : public Entity
{
public:
    TransformComponent *transform;
    CameraComponent    *camera;
    InputComponent     *inputComponent;

    CameraObject()
    {
        transform = &this->addComponent<TransformComponent>();
        camera =
            &this->addComponent<CameraComponent>(glm::vec2(1280.0f, 720.0f), CameraComponent::Projection::Perspective);
        inputComponent = &this->addComponent<InputComponent>();
    }
};

struct mStream : public Assimp::LogStream
{
    void write(const char *message)
    {
        LOG_DEFINE_TAG("Assimp");
        L_INFO("{}", message);
    }
};

int main(int arc, char **argv)
{
    /** Set compact pattern for logging */
    core::utils::logging::setPattern("[%H:%M:%S.%e] [%t] %L : %v");
    core::utils::logging::setLevel(core::utils::logging::level::TRACE);

    Game *game = new Game("Asteroids", windowWidth, windowHeight);
    game->init();
    game->setTargetFPS(90);

    EntityManager *entityManager = Game::entityManager();

    {
        core::assets::Model model("assets/models/backpack/backpack.obj");
    }

    // Create crate entities
    std::array<Crate *, 8> crates = {&entityManager->addEntity<Crate>(glm::vec3(0.0f, 0.0f, 0.0f)),
                                     &entityManager->addEntity<Crate>(glm::vec3(2.5f, 0.0f, 0.0f)),
                                     &entityManager->addEntity<Crate>(glm::vec3(5.0f, 0.0f, 0.0f)),
                                     &entityManager->addEntity<Crate>(glm::vec3(0.0f, 0.0f, 2.5f)),
                                     &entityManager->addEntity<Crate>(glm::vec3(0.0f, 2.5f, 0.0f)),
                                     &entityManager->addEntity<Crate>(glm::vec3(0.0f, 5.0f, 0.0f)),
                                     &entityManager->addEntity<Crate>(glm::vec3(0.0f, 7.5f, 0.0f))};

    CameraObject  &camObject = entityManager->addEntity<CameraObject>();
    AudioListener &listener  = camObject.addComponent<AudioListener>();
    listener.listen();
    // reset position
    camObject.transform->setPosition(glm::vec3(0.0f, 0.0f, 0.0f))
        .setOrientation(TransformComponent::worldFront, TransformComponent::worldUp);
    camObject.camera->updateMatrix();

    // Set KeyDown input listener
    camObject.inputComponent->setListener(InputEventType::KeyDown,
                                          [&camObject](const InputEventType type, SDL_Event *ev) -> void {
                                              // L_TAG("main");
                                              // L_INFO_RATE(32, "cameraInput KeyDown Event: {}", ev->key.keysym.sym);
                                              CameraComponent    *cam = &camObject.getComponent<CameraComponent>();
                                              TransformComponent *transform =
                                                  &camObject.getComponent<TransformComponent>();
                                              switch (ev->key.keysym.sym)
                                              {
                                              case SDLK_w:
                                                  transform->translate(TransformComponent::worldFront, true);
                                                  break;
                                              case SDLK_s:
                                                  transform->translate(-TransformComponent::worldFront, true);
                                                  break;
                                              case SDLK_d:
                                                  transform->translate(TransformComponent::worldRight, true);
                                                  break;
                                              case SDLK_a:
                                                  transform->translate(-TransformComponent::worldRight, true);
                                                  break;
                                              case SDLK_LSHIFT:
                                                  transform->translate(TransformComponent::worldUp, true);
                                                  break;
                                              case SDLK_LCTRL:
                                                  transform->translate(-TransformComponent::worldUp, true);
                                                  break;
                                              case SDLK_KP_4:
                                                  transform->rotate(-5.0f, TransformComponent::worldUp);
                                                  break;
                                              case SDLK_KP_6:
                                                  transform->rotate(5.0f, TransformComponent::worldUp);
                                                  break;
                                              case SDLK_KP_2:
                                                  transform->rotate(-5.0f, TransformComponent::worldRight);
                                                  break;
                                              case SDLK_KP_8:
                                                  transform->rotate(5.0f, TransformComponent::worldRight);
                                                  break;
                                              }
                                              cam->updateMatrix();
                                              glm::quat q             = transform->getOrientation();
                                              float     rotationAngle = std::acos(q.w) * 2;
                                              float     td            = 1 / std::sin(rotationAngle / 2);
                                              glm::vec3 rotationAxis(q.x * td, q.y * td, q.z * td);

                                              // L_TRACE("position        {}", glm::to_string(transform->m_position));
                                              // L_TRACE("front:          {}", glm::to_string(transform->getFront()));
                                              // L_TRACE("right:          {}", glm::to_string(transform->getRight()));
                                              // L_TRACE("up:             {}", glm::to_string(transform->getUp()));
                                              // L_TRACE("rotationAxis:   {}", glm::to_string(rotationAxis));
                                              // L_TRACE("rotationAngle:  {}", rotationAngle);
                                              // L_TRACE("euler:          {}",
                                              // glm::to_string(glm::degrees(glm::eulerAngles(transform->getOrientation()))));
                                              // L_TRACE("orientation:    {}",
                                              // glm::to_string(transform->getOrientation())); L_TRACE("orientation:
                                              // {}", glm::to_string(glm::mat4_cast(transform->getOrientation())));
                                              // L_TRACE("viewMatrix:
                                              // {}", glm::to_string(cam->getViewMatrix())); L_TRACE("projection: {}",
                                              // glm::to_string(cam->getProjectionMatrix()));
                                          });

    audioComponent = &camObject.addComponent<AudioComponent>();

    core::audio::Audio &music0 = audioComponent->addAudioClip("ImperialMarch60.wav", false);
    music0.setLoop(true).setVolume(1.0f).play();
    game->startGameLoop();

    delete game;
    return 0;
}
