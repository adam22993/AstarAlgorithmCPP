#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

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
    bool visited;                            // Has this node been visited before?
    bool obstacle;                           // Is this node an obstacle?
    bool start;                              // Is this node the start node?
    bool end;                                // Is this node the end node?
    bool path;                               // Is this node part of the final path?
    bool toggled;                            // Has this node been toggled?
    int gCost;                               // Distance from start node
    int hCost;                               // Distance from end node
    int fCost;                               // gCost + hCost
    int x;                                   // x coordinate
    int y;                                   // y coordinate
    std::vector<graphNode *> neighbors;       // List of neighboring nodes
    graphNode *parent;                       // Parent node
    sf::RectangleShape *shape;               // Shape to draw
};


graphNode *startNode;                            // Start node
graphNode *endNode;                              // End node
int mapWidth = 26;                               // Width of map 26
int mapHeight = 15;                              // Height of map 15
std::vector<graphNode *> nodes;                  // List of nodes
std::vector<graphNode *> openList;               // List of nodes to be evaluated


void calculateHCost(graphNode *node) {
    /*
     * Calculate the hCost of a node
     * hCost is the distance from the node to the end node
     */
    int x = abs(node->getX() - endNode->getX());
    int y = abs(node->getY() - endNode->getY());
    int calcPythag = pow(pow(x, 2) + pow(y, 2), 0.5);
    node->setHCost(calcPythag);
}

void calculateGCost(graphNode *node) {
    /*
     * Calculate the gCost of a node
     * gCost is the distance from the start node to the node
     */
    if (node->getParent() == nullptr) {
        node->setGCost(1);
    } else {
        node->setGCost(node->getParent()->getGCost() + 1);
    }
}

void calculateFCost(graphNode *node) {
    /*
     * Calculate the fCost of a node
     * fCost is the sum of the gCost and hCost
     */
    node->setFCost(node->getGCost() + node->getHCost());
}

void calculateCosts(graphNode *node) {
    /*
     * Calculate the gCost, hCost, and fCost of a node
     */
    calculateGCost(node);
    calculateHCost(node);
    calculateFCost(node);
}

void addNeighbors(graphNode *node) {
    /*
     * Add the neighbors of a node to the open list
     */
    int x = node->getX();
    int y = node->getY();
    if (x > 0) {
        graphNode *neighbor = nodes.at((x - 1) * mapHeight + y);
        if (!neighbor->getObstacle() && !neighbor->getVisited()) {
            neighbor->setParent(node);
            calculateCosts(neighbor);
            openList.push_back(neighbor);
        }
    }
    if (x < mapWidth - 1) {
        graphNode *neighbor = nodes.at((x + 1) * mapHeight + y);
        if (!neighbor->getObstacle() && !neighbor->getVisited()) {
            neighbor->setParent(node);
            calculateCosts(neighbor);
            openList.push_back(neighbor);
        }
    }
    if (y > 0) {
        graphNode *neighbor = nodes.at(x * mapHeight + y - 1);
        if (!neighbor->getObstacle() && !neighbor->getVisited()) {
            neighbor->setParent(node);
            calculateCosts(neighbor);
            openList.push_back(neighbor);
        }
    }
    if (y < mapHeight - 1) {
        graphNode *neighbor = nodes.at(x * mapHeight + y + 1);
        if (!neighbor->getObstacle() && !neighbor->getVisited()) {
            neighbor->setParent(node);
            calculateCosts(neighbor);
            openList.push_back(neighbor);
        }
    }
}

void findPath() {
    /*
     * Find the shortest path from the start node to the end node
     */
    graphNode *currentNode = endNode;
    if (currentNode->getParent() == nullptr) {
        std::cout << "No path found" << std::endl;
        return;
    }
    while (!currentNode->isStart()) {
        currentNode->setPath(true);
        currentNode = currentNode->getParent();
    }
}

void AstarAlgorithm() {
    /*
     * A* Pathfinding Algorithm
     */
    if (startNode == nullptr || endNode == nullptr) {
        std::cout << "Start or end node not set" << std::endl;
        return;
    }
    openList.push_back(startNode);
    while (!openList.empty()) {
        graphNode *currentNode = openList.at(0);
        for (graphNode *node : openList) {
            if (node->getFCost() < currentNode->getFCost()) {
                currentNode = node;
            }
        }
        if (currentNode->isEnd()) {
            findPath();
            return;
        }
        currentNode->setVisited(true);
        currentNode->setPath(false);
        openList.erase(std::remove(openList.begin(), openList.end(), currentNode), openList.end());
        addNeighbors(currentNode);
    }
    std::cout << "No path found" << std::endl;
}

void resetNodes() {
    /*
     * Reset the nodes to their default values
     */
    nodes.clear();
    for (int i = 0; i < mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {
            auto *node = new graphNode();
            node->setX(i);
            node->setY(j);
            node->setVisited(false);
            node->setObstacle(false);
            node->setGCost(1);
            node->setHCost(1);
            node->setFCost(1);
            node->setPath(false);
            node->setStart(false);
            node->setEnd(false);
            node->setParent(nullptr);
            node->resetToggled();
            node->setNeighbors(std::vector<graphNode *>());
            node->setShape(new sf::RectangleShape(sf::Vector2f(45, 45)));
            nodes.push_back(node);
        }
    }
    startNode = nullptr;
    endNode = nullptr;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1303, 760), "A* Pathfinding");

    for (int i = 0; i < mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {
            graphNode *node = new graphNode();
            node->setX(i);
            node->setY(j);
            node->setVisited(false);
            node->setObstacle(false);
            node->setGCost(0);
            node->setHCost(0);
            node->setFCost(0);
            node->setStart(false);
            node->setEnd(false);
            node->setParent(nullptr);
            node->resetToggled();
            node->setNeighbors(std::vector<graphNode *>());
            node->setShape(new sf::RectangleShape(sf::Vector2f(45, 45)));
            nodes.push_back(node);
        }
    }
    int counter = 0;

    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::Space) {
                    if (counter % 2 == 0){
                        AstarAlgorithm();
                        // draw path
                        for (auto &node: nodes) {
                            if (node->isPath()) {
                                node->getShape()->setFillColor(sf::Color::Blue);
                            }
                        }
                        counter++;
                    } else {
                        resetNodes();
                        counter++;
                    }

                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {

                // Check for left mouse button pressed event
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Get the mouse position at the start of dragging
                    sf::Vector2f initialMousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                    // Check if any node was clicked and start dragging if it was clicked
                    for (auto &node : nodes) {
                        if (node->contains(initialMousePosition)) {
                            if (!node->isStart() && !node->isEnd()) {
                                node->setObstacle(!node->getObstacle());
                                break;
                            }
                        }
                    }
                }



                if (event.mouseButton.button == sf::Mouse::Right) {
                    // Get the mouse position
                    sf::Vector2f mousePosition = window.mapPixelToCoords(
                            sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                    // Check if any node was clicked and perform an action (e.g., change color)
                    for (auto &node: nodes) {
                        if (node->contains(mousePosition)) {
                            if (node->getObstacle())
                                break;
                            if (!node->isStart() && !node->isEnd() && startNode == nullptr) {
                                startNode = node;
                                startNode->setStart(true);
                                break;
                            } else if (!node->isEnd() && !node->isStart() && endNode == nullptr) {
                                endNode = node;
                                endNode->setEnd(true);
                                break;
                            } else {
                                if (node->isStart()) {
                                    node->resetNodeStartEnd();
                                    startNode = nullptr;
                                    break;
                                } else if (node->isEnd()) {
                                    node->resetNodeStartEnd();
                                    endNode = nullptr;
                                    break;
                                }
                            }
                        }
                    }
                }

            }
        }
        window.clear();
        for (graphNode *node: nodes) {
            node->draw();
            window.draw(*node->getShape());
        }
        window.display();
    }
}