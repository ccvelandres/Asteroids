#include <iostream>
#include <map>
#include <thread>
#include <typeindex>

#include <core/game.hpp>
#include <core/event.hpp>
#include <core/ecs/entity.hpp>
#include <core/ecs/entityManager.hpp>
#include <core/ecs/components.hpp>
#include <core/graphics/renderer.hpp>
#include <core/ui/text/fontLoader.hpp>
#include <core/utils/logging.hpp>
#include <core/assets/model.hpp>

#include <glm/gtx/string_cast.hpp>

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
        core::assets::Model model("assets/models/crate.obj");
        
        AssetID crateMesh    = Game::assetManager()->loadMesh(model.getMeshes()[0]); // Game::assetManager()->loadAsset(AssetType::Mesh, "crate.obj");
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
            &this->addComponent<CameraComponent>(CameraComponent::Projection::Perspective);
        inputComponent = &this->addComponent<InputComponent>();
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
    L_TAG("main");

    {
        // core::assets::Model model("assets/models/crate.obj");
        // core::assets::Model model("assets/models/backpack/backpack.obj");
    }

    // Create crate entities
    std::array<Crate *, 8> crates = {&entityManager->addEntity<Crate>(glm::vec3(0.0f, 0.0f, 0.0f)),
                                     &entityManager->addEntity<Crate>(glm::vec3(2.5f) * TransformComponent::worldFront),
                                     &entityManager->addEntity<Crate>(glm::vec3(2.5f) * TransformComponent::worldUp),
                                     &entityManager->addEntity<Crate>(glm::vec3(5.0f) * TransformComponent::worldUp),
                                     &entityManager->addEntity<Crate>(glm::vec3(2.5f) * TransformComponent::worldRight),
                                     &entityManager->addEntity<Crate>(glm::vec3(5.0f) * TransformComponent::worldRight),
                                     &entityManager->addEntity<Crate>(glm::vec3(7.5f) * TransformComponent::worldRight)};

    CameraObject  &camObject = entityManager->addEntity<CameraObject>();
    AudioListener &listener  = camObject.addComponent<AudioListener>();
    listener.listen();
    // reset position
    camObject.transform->setPosition(glm::vec3(0.0f, 0.0f, -10.0f))
        .setOrientation(TransformComponent::worldFront, TransformComponent::worldUp);
    // camObject.camera->setProjection(CameraComponent::Projection::Orthographic);
    camObject.camera->updateMatrix();

    {
        core::assets::Font atlas = core::ui::FontLoader().openFont("assets/fonts/arial.ttf").generateFont();
        
        // core::assets::Texture texture = core::assets::Texture(surface);
        // core::assets::Mesh mesh = core::assets::Mesh(vector);

        // AssetManager *assetManager = Game::assetManager();

        // AssetID quadId = assetManager->loadAsset(AssetType::Mesh, "quad.obj");
        // AssetID textureId = assetManager->loadTexture(texture);

        // Entity &font = entityManager->addEntity<Entity>();
        // TransformComponent &transform = font.addComponent<TransformComponent>();
        // MeshRenderer &mesh = font.addComponent<MeshRenderer>(quadId, textureId);
    }

    // Set KeyDown input listener
    camObject.inputComponent->setListener(
        InputEventType::KeyDown,
        [&camObject](const InputEventType type, SDL_Event *ev) -> void {
            static bool relative = true;
            L_TAG("inputListener");
            // L_INFO_RATE(32, "cameraInput KeyDown Event: {}", ev->key.keysym.sym);
            CameraComponent    *cam       = &camObject.getComponent<CameraComponent>();
            TransformComponent *transform = &camObject.getComponent<TransformComponent>();
            switch (ev->key.keysym.sym)
            {
            case SDLK_w:
                transform->translate(-TransformComponent::worldFront, relative);
                break;
            case SDLK_s:
                transform->translate(TransformComponent::worldFront, relative);
                break;
            case SDLK_d:
                transform->translate(TransformComponent::worldRight, relative);
                break;
            case SDLK_a:
                transform->translate(-TransformComponent::worldRight, relative);
                break;
            case SDLK_r:
                L_TRACE("pre ori:     {}", glm::to_string(transform->getOrientation()));
                transform->setOrientation(glm::conjugate(transform->getOrientation()));
                L_TRACE("pos ori:     {}", glm::to_string(transform->getOrientation()));
                break;
            case SDLK_LSHIFT:
                transform->translate(TransformComponent::worldUp, relative);
                break;
            case SDLK_LCTRL:
                transform->translate(-TransformComponent::worldUp, relative);
                break;
            case SDLK_KP_4:
                transform->rotate(-5.0f, TransformComponent::worldUp, relative);
                break;
            case SDLK_KP_6:
                transform->rotate(5.0f, TransformComponent::worldUp, relative);
                break;
            case SDLK_KP_2:
                transform->rotate(-5.0f, TransformComponent::worldRight, relative);
                break;
            case SDLK_KP_8:
                transform->rotate(5.0f, TransformComponent::worldRight, relative);
                break;
            case SDLK_KP_7:
                transform->rotate(-5.0f, TransformComponent::worldFront, relative);
                break;
            case SDLK_KP_9:
                transform->rotate(5.0f, TransformComponent::worldFront, relative);
                break;
            case SDLK_KP_0:
                relative = !relative;
                L_TRACE("Transform relative to {}", relative ? "self" : "world");
                break;
            case SDLK_KP_ENTER:
                glm::vec3 forward = TransformComponent::worldFront * (transform->getOrientation());
                glm::vec3 right = TransformComponent::worldRight * (transform->getOrientation());
                glm::vec3 up = TransformComponent::worldUp * (transform->getOrientation());
                
                L_TRACE("forward:     {}", glm::to_string(forward));
                L_TRACE("right:       {}", glm::to_string(right));
                L_TRACE("up:          {}", glm::to_string(up));;
                break;
            }
            cam->updateMatrix();
        });

    audioComponent = &camObject.addComponent<AudioComponent>();

    core::audio::Audio &music0 = audioComponent->addAudioClip("ImperialMarch60.wav", false);
    music0.setLoop(true).setVolume(1.0f).play();
    game->startGameLoop();

    delete game;
    return 0;
}
