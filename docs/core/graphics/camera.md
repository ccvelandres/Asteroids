# Camera

# Table of Contents
- [Camera](#camera)
- [Table of Contents](#table-of-contents)
- [Controls](#controls)
  - [Translation](#translation)
    - [APIs](#apis)
    - [Sample Code](#sample-code)
  - [Rotation](#rotation)
    - [Euler Angles](#euler-angles)
      - [APIs](#apis-1)
      - [Sample code](#sample-code-1)
- [Projections](#projections)
  - [Perspective](#perspective)


# Controls 

## Translation

### APIs
- Translation 
  - `translate(const glm::vec3 &v)`
- Set position
  - `setPosition(const glm::vec3 &pos)`

### Sample Code

```c
Camera *cam;
// Translate camera 5 units along x-axis
cam->translate(glm::vec3(5.0f, 0.0f, 0.0f));
// Set camera position to origin
cam->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
```

## Rotation

### Euler Angles

#### APIs
- XYZ degrees (YAW-PITCH-ROLL)
  - `rotate(const glm::vec3 &e, bool relativeToSelf)`
- YXZ degrees (PITCH-YAW-ROLL)
  - `rotateXYZ(const glm::vec3 &e, bool relativeToSelf)`
- Set orientation to fixed euler angle
  - `setOrientationEuler(const glm::vec3 &v)`

Camera rotation APIs using Euler angles are by default XYZ (yaw-pitch-roll). For YXZ variant, see `rotateYXZ(...)`

#### Sample code

```c
Camera *cam;
// Pitch the camera up 45 degrees relative to itself
cam->rotate(glm::vec3(0.0f, 45.0f, 0.0f));
// Turn the camera left by 45 degrees relative to itself (yaw to left)
cam->rotate(glm::vec3(45.0f, 0.0f, 0.0f));
// Roll the camera by 45 degrees relative to itself
cam->rotate(glm::vec3(0.0f, 0.0f, 45.0f));
```

# Projections

## Perspective