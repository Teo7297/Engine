#pragma once

#include "engine_includes.h"

static float angleBetweenVectors(const glm::vec2 vector1, const glm::vec2 vector2)
{
    // Calculate dot product
    float dotProduct = glm::dot(vector1, vector2);

    // Calculate magnitudes of the vectors
    float magnitude1 = glm::length(vector1);
    float magnitude2 = glm::length(vector2);

    // Calculate the cosine of the angle using dot product formula
    float cosAngle = dotProduct / (magnitude1 * magnitude2);

    // Convert cosine to angle in radians
    float angleRadians = std::acos(cosAngle);

    // Convert angle from radians to degrees
    float angleDegrees = glm::degrees(angleRadians);

    return angleDegrees;
}