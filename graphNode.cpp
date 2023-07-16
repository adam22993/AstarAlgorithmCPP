//
// Created by Adam Simkin on 16/07/2023.
//
#pragma once
#include "Includes/graphNode.h"

class graphNode {
public:
    virtual void setVisited(bool visited) {
        this->visited = visited;
    }

    virtual void setObstacle(bool obstacle) {
        this->obstacle = obstacle;
    }

    virtual void setGCost(int gCost) {
        this->gCost = gCost;
    }

    virtual void setHCost(int hCost) {
        this->hCost = hCost;
    }

    virtual void setFCost(int fCost) {
        this->fCost = fCost;
    }

    virtual void setX(int x) {
        this->x = x;
    }

    virtual void setY(int y) {
        this->y = y;
    }

    virtual void setNeighbors(std::vector<graphNode *> neighbors) {
        this->neighbors = neighbors;
    }

    virtual void setParent(graphNode *parent) {
        this->parent = parent;
    }

    virtual void setShape(sf::RectangleShape *shape) {
        this->shape = shape;
    }

    virtual void setStart(bool start) {
        this->start = start;
    }

    virtual void setEnd(bool end) {
        this->end = end;
    }

    virtual void setPath(bool path) {
        this->path = path;
    }

    virtual bool getObstacle() {
        return obstacle;
    }

    virtual bool getVisited() {
        return visited;
    }

    virtual int getGCost() {
        return gCost;
    }

    virtual int getHCost() {
        return hCost;
    }

    virtual int getFCost() {
        return fCost;
    }

    virtual int getX() {
        return x;
    }

    virtual int getY() {
        return y;
    }

    virtual std::vector<graphNode *> getNeighbors() {
        return neighbors;
    }

    virtual graphNode *getParent() {
        return parent;
    }

    virtual sf::RectangleShape *getShape() {
        return shape;
    }

    virtual bool contains(sf::Vector2f point) {
        return shape->getGlobalBounds().contains(point);
    }

    virtual bool isStart() {
        return start;
    }

    virtual bool isEnd() {
        return end;
    }

    virtual bool isPath() {
        return path;
    }


    virtual void resetNodeStartEnd() {
        if (start) {
            start = false;
        } else if (end) {
            end = false;
        }
    }

    virtual void switchToggled() {
        this->toggled = !toggled;
    }

    virtual bool isToggled() {
        return toggled;
    }

    virtual void resetToggled() {
        this->toggled = false;
    }

    virtual std::string toString() {
        return "Node at (" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }


    virtual void draw() {
        /*
         * Draw the node
         * Colors:
         * - White: Unvisited
         * - Blue: Visited
         * - Green: Start
         * - Red: End
         * - Gray: Obstacle
         * - orange: Path
         */
        shape->setPosition(x * 50 + 4, y * 50 + 4);
        if (start) {
            shape->setFillColor(sf::Color::Green);
        } else if (end) {
            shape->setFillColor(sf::Color::Red);
        } else if (path) {
            shape->setFillColor(sf::Color(255, 165, 0)); // Orange
        } else if (obstacle) {
            shape->setFillColor(sf::Color(64, 64, 64)); // Gray
        } else if (visited) {
            shape->setFillColor(sf::Color::Blue);
        } else {
            shape->setFillColor(sf::Color::White);
        }

    }

private:
    bool visited{};                            // Has this node been visited before?
    bool obstacle{};                           // Is this node an obstacle?
    bool start{};                              // Is this node the start node?
    bool end{};                                // Is this node the end node?
    bool path{};                               // Is this node part of the final path?
    bool toggled{};                            // Has this node been toggled?
    int gCost{};                               // Distance from start node
    int hCost{};                               // Distance from end node
    int fCost{};                               // gCost + hCost
    int x{};                                   // x coordinate
    int y{};                                   // y coordinate
    std::vector<graphNode *> neighbors;       // List of neighboring nodes
    graphNode* parent{};                       // Parent node
    sf::RectangleShape* shape{};               // Shape to draw
};