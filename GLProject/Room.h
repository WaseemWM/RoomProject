
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Polygon.cpp"

class Room
{
private:
    Polygon floor;
    Polygon backWall;
    Polygon rightWall;
    Polygon leftWall;
    Polygon ceiling;
    Polygon frontWall;

public:
    Room();

    void draw(Shader &shader);
};

#include "Room.h"

Room::Room()
    : floor(
          std::vector<glm::vec3>{
              {-5.0f, -1.0f, -5.0f},
              {5.0f, -1.0f, -5.0f},
              {5.0f, -1.0f, 5.0f},
              {-5.0f, -1.0f, 5.0f}},
          glm::vec3(0.45f, 0.25f, 0.10f)),

      backWall(
          std::vector<glm::vec3>{
              {-5.0f, -1.0f, -5.0f},
              {5.0f, -1.0f, -5.0f},
              {5.0f, 4.0f, -5.0f},
              {-5.0f, 4.0f, -5.0f}},
          glm::vec3(0.8f, 0.8f, 0.8f)),

      rightWall(
          std::vector<glm::vec3>{
              {5.0f, -1.0f, -5.0f},
              {5.0f, -1.0f, 5.0f},
              {5.0f, 4.0f, 5.0f},
              {5.0f, 4.0f, -5.0f}},
          glm::vec3(0.8f, 0.8f, 0.8f)),

      leftWall(
          std::vector<glm::vec3>{
              {-5.0f, -1.0f, -5.0f},
              {-5.0f, -1.0f, 5.0f},
              {-5.0f, 4.0f, 5.0f},
              {-5.0f, 4.0f, -5.0f}},
          glm::vec3(0.8f, 0.8f, 0.8f)),

      ceiling(
          std::vector<glm::vec3>{
              {-5.0f, 4.0f, -5.0f},
              {5.0f, 4.0f, -5.0f},
              {5.0f, 4.0f, 5.0f},
              {-5.0f, 4.0f, 5.0f}},
          glm::vec3(0.7f, 0.7f, 0.7f)),

      frontWall(
          std::vector<glm::vec3>{
              {-5.0f, -1.0f, 5.0f},
              {5.0f, -1.0f, 5.0f},
              {5.0f, 4.0f, 5.0f},
              {-5.0f, 4.0f, 5.0f}},
          glm::vec3(0.8f, 0.8f, 0.8f))
{
}

void Room::draw(Shader &shader)
{
    glm::mat4 identity = glm::mat4(1.0f);

    floor.transformation(identity);
    backWall.transformation(identity);
    rightWall.transformation(identity);
    leftWall.transformation(identity);
    ceiling.transformation(identity);
    frontWall.transformation(identity);

    floor.draw(shader);
    backWall.draw(shader);
    rightWall.draw(shader);
    leftWall.draw(shader);
    ceiling.draw(shader);
    frontWall.draw(shader);
}
