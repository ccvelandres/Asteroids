

#include <iostream>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

void log(glm::mat4 &mat)
{
    spdlog::info("");
    // mat.
}

int main(int argc, const char *argv[])
{
    glm::mat4 identity(1.0f);
    {
        spdlog::info("");
        spdlog::info("---------------------------------------------");
        spdlog::info("translation");
        glm::vec3 position(0.0f);
        glm::vec3 direction(10.0f, 10.0f, 5.0f);
        spdlog::info("position:                 {}", glm::to_string(position));
        spdlog::info("direction:                {}", glm::to_string(direction));

        glm::mat4 translationMatrix = glm::translate(identity, direction);
        spdlog::info("translationMatrix:        {}", glm::to_string(translationMatrix));
    }
    {
        spdlog::info("");
        spdlog::info("---------------------------------------------");
        spdlog::info("rotation");
        glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);
        float     rotationAngle = glm::radians(90.0f);
        spdlog::info("rotationAxis:             {}", glm::to_string(rotationAxis));
        spdlog::info("rotationAngle:            {}", rotationAngle);

        glm::mat4 rotationMatrix = glm::rotate(identity, rotationAngle, rotationAxis);
        spdlog::info("rotationMatrix:           {}", glm::to_string(rotationMatrix));

        glm::vec4 position(5.0f, 0.0f, 0.0f, 1.0f);

        spdlog::info("position:                 {}", glm::to_string(position));
        spdlog::info("rotatedPosition:          {}", glm::to_string(rotationMatrix * position));
    }
    {
        spdlog::info("");
        spdlog::info("---------------------------------------------");
        spdlog::info("scale");
        glm::vec3 scaleFactor(0.5f, 1.0f, 2.0f);
        spdlog::info("scaleFactor:              {}", glm::to_string(scaleFactor));

        glm::mat4 scalingMatrix = glm::scale(identity, scaleFactor);
        spdlog::info("scalingMatrix:            {}", glm::to_string(scalingMatrix));
    }

    {
        spdlog::info("");
        spdlog::info("---------------------------------------------");
        spdlog::info("viewMatrix");
        glm::vec3 position(-1.0f, 0.0f, -1.0f), target(1.0f, 0.0f, 1.0f), right(-1.0f, 0.0f, 0.0f);
        spdlog::info("identity:                 {}", glm::to_string(identity));
        spdlog::info("position:                 {}", glm::to_string(position));
        spdlog::info("target:                   {}", glm::to_string(target));
        spdlog::info("right:                    {}", glm::to_string(right));

        glm::vec3 up = glm::cross(right, target);
        spdlog::info("up:                       {}", glm::to_string(up));

        glm::mat4 viewMatrix = glm::lookAt(position, target + position, up);
        spdlog::info("viewMatrix:               {}", glm::to_string(viewMatrix));
    }
    {
        spdlog::info("");
        spdlog::info("---------------------------------------------");
        spdlog::info("projectionMatrix");
        {
            spdlog::info("perspective");
            float aspectRatio       = 4.0f / 3.0f;
            float fieldOfView       = 90.0f;
            float nearClippingPlane = 0.1;
            float farClippingPlane  = 100.0f;

            glm::mat4 perspectiveView =
                glm::perspective(glm::radians(fieldOfView), aspectRatio, nearClippingPlane, farClippingPlane);
            spdlog::info("perspectiveView:          {}", glm::to_string(perspectiveView));
        }
        {
            spdlog::info("orthogonal");
            glm::vec2 screenSize(800.0f, 600.0f);
            float     nearClip         = -2.0f;
            float     farClip          = 2.0f;
            glm::mat4 orthographicView = glm::ortho(0.0f, screenSize.x, 0.0f, screenSize.y, nearClip, farClip);
            spdlog::info("orthographicView:         {}", glm::to_string(orthographicView));
        }
    }

    {
        spdlog::info("");
        spdlog::info("---------------------------------------------");
        spdlog::info("quaternion components");
        glm::vec3 euler       = glm::vec3(glm::radians(90.0f), glm::radians(0.0f), glm::radians(0.0f));
        glm::quat orientation = glm::quat(euler);
        spdlog::info("euler:                    {}", glm::to_string(euler));
        spdlog::info("quat:                     {}", glm::to_string(orientation));

        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f) * (orientation);
        glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f) * (orientation);
        glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f) * (orientation);
        spdlog::info("front:                    {}", glm::to_string(front));
        spdlog::info("right:                    {}", glm::to_string(right));
        spdlog::info("up:                       {}", glm::to_string(up));
    }
}