#pragma once
#include <cstddef>
#include <vector>
#include <iostream>

template <typename Ty, std::enable_if_t<std::is_arithmetic<Ty>::value, bool> = true>
class Vector2Base
{
public:
    Ty x, y;

    Vector2Base() {}
    Vector2Base(Ty x, Ty y) : x(x), y(y) {}
    Vector2Base(const Vector2Base &p1,
                const Vector2Base &p2) : x(p2.x - p1.x),
                                         y(p2.y - p1.y){};

    /**
     * @brief Returns the magnitude of the vector
     *
     * @return float magnitude of vector
     */
    float magnitude() const
    {
        return sqrtf((this->x * this->x) + (this->y * this->y));
    }

    /**
     * @brief Reflect a vector off a line defined by normal vector
     *
     * @param vect vector to reflect
     * @param normal normal vector
     * @return Vector2Base reflected vector
     */
    static Vector2Base reflect(const Vector2Base &vect, const Vector2Base &normal)
    {
        return vect - (project(vect, normal) * 2);
    }

    /**
     * @brief Returns a normalized vector (unit vector)
     *
     * @return Vector2Base normalized vector
     */
    Vector2Base normalize() const
    {
        float mag = this->magnitude();
        if (mag < 0.1f) return Vector2Base();
        return *this / mag;
    }

    /**
     * @brief Normalizes a vector
     *
     * @param vect vector to normalize
     */
    static void normalize(Vector2Base &vect)
    {
        float mag = vect.magnitude();
        if (mag < 0.1f) vect = Vector2Base(0);
        else vect /= mag;
    }

    /**
     * @brief Gets the projection vector of this vector onto @p rhs
     *
     * @param rhs vector to project this unto
     * @return Vector2Base projected vector
     */
    Vector2Base project(const Vector2Base &rhs) const
    {
        float num = this->dot(rhs);
        float den = rhs.dot(rhs);
        return rhs * (num / den);
    }

    /**
     * @brief Gets the projection vector of @p lhs onto @p rhs
     *
     * @param lhs vector to project
     * @param rhs vector to project unto
     * @return Vector2Base projected vector
     */
    static Vector2Base project(const Vector2Base &lhs, const Vector2Base &rhs)
    {
        return lhs.project(rhs);
    }

    /**
     * @brief Get the normal vector of this vector
     *
     * @return Vector3Base normal vector
     */
    template <typename T>
    Vector2Base<T> normal() const
    {
        return Vector2Base<T>(-this->y, this->x);
    }
    /**
     * @brief Get the normal vector from a line defined by two points
     *
     * @param p1 point 1
     * @param p2 point 2
     * @return Vector2Base
     */
    template <typename T>
    static Vector2Base<T> normal(const Vector2Base &p1, const Vector2Base &p2)
    {
        return Vector2Base<T>(p1.y - p2.y, p2.x - p1.x);
    }

    /**
     * @brief Get the cross product of 2D vectors (scalar)
     *
     * @param rhs vector to cross with this vector
     * @return float cross product of vector
     */
    float cross(const Vector2Base &rhs) const
    {
        return ((this->x * rhs.y) - (this->y * rhs.x));
    }

    /**
     * @brief Get the cross product of 2D vectors (scalar)
     *
     * @param lhs vector to cross
     * @param rhs vector to cross with
     * @return float cross product
     */
    static float cross(const Vector2Base &lhs, const Vector2Base &rhs)
    {
        return lhs.cross(rhs);
    }

    /**
     * @brief Get the dot/scalar product of 2D vectors
     *
     * @param rhs vector
     * @return float dot product
     */
    float dot(const Vector2Base &rhs) const
    {
        return ((this->x * rhs.x) + (this->y * rhs.y));
    }

    /**
     * @brief Get the dot/scalar product of 2D vectors
     *
     * @param lhs vector
     * @param rhs vector
     * @return float dot product
     */
    static float dot(const Vector2Base &lhs, const Vector2Base &rhs)
    {
        return lhs.dot(rhs);
    }

    // ************************************************
    // Vector2-Vector2 operators

    template <typename T>
    Vector2Base operator+(const Vector2Base<T> &vect) const
    {
        Vector2Base<T> c(*this);
        return c += vect;
    }

    template <typename T>
    Vector2Base operator-(const Vector2Base<T> &vect) const
    {
        Vector2Base<T> c(*this);
        return c -= vect;
    }

    template <typename T>
    Vector2Base operator*(const Vector2Base<T> &vect) const
    {
        Vector2Base<T> c(*this);
        return c *= vect;
    }

    template <typename T>
    Vector2Base operator/(const Vector2Base<T> &vect) const
    {
        Vector2Base<T> c(*this);
        return c /= vect;
    }

    template <typename T>
    Vector2Base &operator+=(const Vector2Base<T> &vect)
    {
        this->x += vect.x;
        this->y += vect.y;
        return *this;
    }

    template <typename T>
    Vector2Base &operator-=(const Vector2Base<T> &vect)
    {
        this->x -= vect.x;
        this->y -= vect.y;
        return *this;
    }

    template <typename T>
    Vector2Base &operator*=(const Vector2Base<T> &vect)
    {
        this->x *= vect.x;
        this->y *= vect.y;
        return *this;
    }

    template <typename T>
    Vector2Base &operator/=(const Vector2Base<T> &vect)
    {
        this->x /= vect.x;
        this->y /= vect.y;
        return *this;
    }

    // ************************************************
    // Vector2-IntegerType operators
    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector2Base operator+(const Vector2Base &vect, const T &f)
    {
        Vector2Base c(vect);
        return c += f;
    }
    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector2Base operator-(const Vector2Base &vect, const T &f)
    {
        Vector2Base c(vect);
        return c -= f;
    }
    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector2Base operator*(const Vector2Base &vect, const T &f)
    {
        Vector2Base c(vect);
        return c *= f;
    }
    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector2Base operator/(const Vector2Base &vect, const T &f)
    {
        Vector2Base c(vect);
        return c /= f;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector2Base &operator+=(Vector2Base &vect, const T &f)
    {
        vect.x += f;
        vect.y += f;
        return vect;
    }
    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector2Base &operator-=(Vector2Base &vect, const T &f)
    {
        vect.x -= f;
        vect.y -= f;
        return vect;
    }
    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector2Base &operator*=(Vector2Base &vect, const T &f)
    {
        vect.x *= f;
        vect.y *= f;
        return vect;
    }
    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector2Base &operator/=(Vector2Base &vect, const T &f)
    {
        vect.x /= f;
        vect.y /= f;
        return vect;
    }

    // ************************************************
    // Vector2-Vector2<T> assignment operators

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    inline Vector2Base &operator=(const Vector2Base<T> &vect)
    {
        this->x = static_cast<Ty>(vect.x);
        this->y = static_cast<Ty>(vect.y);
        return *this;
    }

    // ************************************************
    // Vector2-Vector2<T> conversion operators

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    operator Vector2Base<T>()
    {
        return Vector2Base<T>(static_cast<T>(this->x), static_cast<T>(this->y));
    }

    operator bool() {
        return abs(x) + abs(y);
    }

    // ************************************************
    // Vector2 friend functions

    template <typename T1, typename T2>
    friend bool operator==(const Vector2Base<T1> &lhs, const Vector2Base<T2> &rhs);

    template <typename T>
    friend std::ostream &operator<<(std::ostream &os, const Vector2Base<T> &vect);
};

using Vector2F = Vector2Base<float>;
using Vector2 = Vector2Base<int>;

// ************************************************
// Vector2 friend function definitions

template <typename T1, typename T2, std::enable_if_t<std::is_same<T1, T2>::value, bool> = true>
bool operator==(const Vector2Base<T1> &lhs, const Vector2Base<T2> &rhs)
{
    return (lhs.x == rhs.x && lhs.y == rhs.y ? true : false);
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Vector2Base<T> &vect)
{
    return os << "(" << vect.x << "," << vect.y << ")";
}

template <typename Ty, std::enable_if_t<std::is_arithmetic<Ty>::value, bool> = true>
class Vector3Base
{
public:
    Ty x, y, z;

    Vector3Base() : x(0), y(0), z(0) {}
    template <typename T, std::enable_if_t<std::is_arithmetic<Ty>::value, bool> = true>
    Vector3Base(const T& f) : x(f), y(f), z(f) {}
    Vector3Base(Ty x, Ty y, Ty z) : x(x), y(y), z(z) {}
    template <typename T>
    Vector3Base(const Vector2Base<T> &vect) : x(static_cast<Ty>(vect.x)),
                                              y(static_cast<Ty>(vect.y)),
                                              z(0) {}
    Vector3Base(const Vector3Base &p1,
                const Vector3Base &p2) : x(p2.x - p1.x),
                                         y(p2.y - p1.y),
                                         z(p2.z - p1.z) {}

    /**
     * @brief Returns the magnitude of the vector
     *
     * @return float magnitude of vector
     */
    float magnitude() const
    {
        return sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
    }

    /**
     * @brief Reflects a vector off a plane defined by a normal vector
     *
     * @param vect vector to reflect
     * @param normal normal vector of a plane
     * @return Vector3Base reflected vector
     */
    static Vector3Base reflect(const Vector3Base &vect, const Vector3Base &normal)
    {
        return vect - (project(vect, normal) * 2);
    }

    /**
     * @brief Returns a normalized vector (unit vector)
     * @note does not work properly on int vectors
     *
     * @return Vector3Base normalized vector
     */
    Vector3Base normalize() const
    {
        float mag = this->magnitude();
        if (mag < 0.1f) return Vector3Base();
        return *this / mag;
    }

    /**
     * @brief Returns a normalized vector (unit vector)
     *
     * @param vect vector to normalize
     */
    static void normalize(Vector3Base &vect)
    {
        float mag = vect.magnitude();
        if (mag < 0.1f) vect = Vector3Base(0);
        else vect /= mag;
    }

    /**
     * @brief Get the projection vector of this vector unto @p rhs
     *
     * @param rhs vector to project unto
     * @return Vector3Base projected vector
     */
    Vector3Base project(const Vector3Base &rhs) const
    {
        float num = this->dot(rhs);
        float den = rhs.dot(rhs);
        return rhs * (num / den);
    }

    /**
     * @brief Get the projection vector of @p lhs unto @p rhs
     *
     * @param lhs vector to project
     * @param rhs vector to project unto
     * @return Vector3Base projected vector
     */
    static Vector3Base project(const Vector3Base &lhs, const Vector3Base &rhs)
    {
        return lhs.project(rhs);
    }

    /**
     * @brief Get the normal vector from 3 points
     * @note points should no be collinear
     * @param p1 point 1
     * @param p2 point 2
     * @param p3 point 3
     * @return Vector3Base normal vector
     */
    static Vector3Base normal(const Vector3Base &p1, const Vector3Base &p2, const Vector3Base &p3)
    {
        return (p2 - p1).cross(p3 - p1);
    }

    /**
     * @brief Get the cross product of 2 vectors
     *
     * @param rhs vector to cross to
     * @return Vector3Base cross product
     */
    Vector3Base cross(const Vector3Base &rhs) const
    {
        Vector3Base<Ty> c((this->y * rhs.z) - (this->z * rhs.y),
                          (this->z * rhs.x) - (this->x * rhs.z),
                          (this->x * rhs.y) - (this->y * rhs.x));
        return c;
    }

    /**
     * @brief Get the cross product of 2 vectors
     *
     * @param lhs vector
     * @param rhs vector
     * @return Vector3Base cross product
     */
    static Vector3Base cross(const Vector3Base &lhs, const Vector3Base &rhs)
    {
        return lhs.cross(rhs);
    }

    /**
     * @brief Get the scalar product of 2 vectors
     *
     * @param rhs vector
     * @return float scalar product
     */
    float dot(const Vector3Base &rhs) const
    {
        return (this->x * rhs.x) + (this->y * rhs.y) + (this->z * rhs.z);
    }

    /**
     * @brief Get the scalar product of 2 vectors
     *
     * @param lhs vector
     * @param rhs vector
     * @return float scalar product
     */
    static float dot(const Vector3Base &lhs, const Vector3Base &rhs)
    {
        return lhs.dot(rhs);
    }

    // ************************************************
    // Vector3-Vector3 operators

    template <typename T>
    Vector3Base operator+(const Vector3Base<T> &vect) const
    {
        Vector3Base<T> c(*this);
        return c += vect;
    }

    template <typename T>
    Vector3Base operator-(const Vector3Base<T> &vect) const
    {
        Vector3Base<T> c(*this);
        return c -= vect;
    }

    template <typename T>
    Vector3Base operator*(const Vector3Base<T> &vect) const
    {
        Vector3Base<T> c(*this);
        return c *= vect;
    }

    template <typename T>
    Vector3Base operator/(const Vector3Base<T> &vect) const
    {
        Vector3Base<T> c(*this);
        return c /= vect;
    }

    template <typename T>
    Vector3Base &operator+=(const Vector3Base<T> &vect)
    {
        this->x += vect.x;
        this->y += vect.y;
        this->z += vect.z;
        return *this;
    }

    template <typename T>
    Vector3Base &operator-=(const Vector3Base<T> &vect)
    {
        this->x -= vect.x;
        this->y -= vect.y;
        this->z -= vect.z;
        return *this;
    }

    template <typename T>
    Vector3Base &operator*=(const Vector3Base<T> &vect)
    {
        this->x *= vect.x;
        this->y *= vect.y;
        this->z *= vect.z;
        return *this;
    }

    template <typename T>
    Vector3Base &operator/=(const Vector3Base<T> &vect)
    {
        this->x /= vect.x;
        this->y /= vect.y;
        this->z /= vect.z;
        return *this;
    }

    // ************************************************
    // Vector3-IntegerType operators

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector3Base operator+(const Vector3Base &vect, const T &f)
    {
        Vector3Base c(vect);
        return c += f;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector3Base operator-(const Vector3Base &vect, const T &f)
    {
        Vector3Base c(vect);
        return c -= f;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector3Base operator*(const Vector3Base &vect, const T &f)
    {
        Vector3Base c(vect);
        return c *= f;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector3Base operator/(const Vector3Base &vect, const T &f)
    {
        Vector3Base c(vect);
        return c /= f;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector3Base &operator+=(Vector3Base &vect, const T &f)
    {
        vect.x += f;
        vect.y += f;
        vect.z += f;
        return vect;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector3Base &operator-=(Vector3Base &vect, const T &f)
    {
        vect.x -= f;
        vect.y -= f;
        vect.z -= f;
        return vect;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector3Base &operator*=(Vector3Base &vect, const T &f)
    {
        vect.x *= f;
        vect.y *= f;
        vect.z *= f;
        return vect;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    friend inline Vector3Base &operator/=(Vector3Base &vect, const T &f)
    {
        vect.x /= f;
        vect.y /= f;
        vect.z /= f;
        return vect;
    }

    // ************************************************
    // Vector3-Vector3<T> assignment operators

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    inline Vector3Base &operator=(const Vector3Base<T> &vect)
    {
        this->x = static_cast<Ty>(vect.x);
        this->y = static_cast<Ty>(vect.y);
        return *this;
    }

    // ************************************************
    // Vector3-Vector3<T> conversion operators

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    operator Vector3Base<T>()
    {
        return Vector3Base<T>(static_cast<T>(this->x), static_cast<T>(this->y), static_cast<T>(this->z));
    }

    operator bool() {
        return abs(x) + abs(y) + abs(z);
    }

    // ************************************************
    // Vector3 friend functions
    /** @todo: add support for <format> */
    
    template <typename T1, typename T2>
    friend bool operator==(const Vector3Base<T1> &lhs, const Vector3Base<T2> &rhs);

    template <typename T>
    friend std::ostream &operator<<(std::ostream &os, const Vector3Base<T> &vect);
};

using Vector3F = Vector3Base<float>;
using Vector3 = Vector3Base<int>;

// ************************************************
// Vector3 friend function definitions

template <typename T1, typename T2, std::enable_if_t<std::is_same<T1, T2>::value, bool> = true>
bool operator==(const Vector3Base<T1> &lhs, const Vector3Base<T2> &rhs)
{
    return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z ? true : false);
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Vector3Base<T> &vect)
{
    return os << "(" << vect.x << "," << vect.y << "," << vect.z << ")";
}