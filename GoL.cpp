#include <SFML/Graphics.hpp>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 600;
const int CELL_SIZE = 10;
const int GRID_WIDTH = WIDTH / CELL_SIZE;
const int GRID_HEIGHT = HEIGHT / CELL_SIZE;

sf::Vector2i getMousePositionInGrid(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return sf::Vector2i(mousePos.x / CELL_SIZE, mousePos.y / CELL_SIZE);
}

int wrapCoordinate(int coordinate, int max) {
    if (coordinate < 0) return max - 1;
    if (coordinate >= max) return 0;
    return coordinate;
}

int countNeighbours(const std::vector<std::vector<bool>>& grid, int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nx = wrapCoordinate(x + i, GRID_WIDTH);
            int ny = wrapCoordinate(y + j, GRID_HEIGHT);
            if (grid[nx][ny]) count++;
        }
    }
    return count;
}

void updateGrid(std::vector<std::vector<bool>>& grid) {
    std::vector<std::vector<bool>> newGrid = grid;
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            int neighbours = countNeighbours(grid, x, y);
            if (grid[x][y]) {
                if (neighbours < 2 || neighbours > 3) newGrid[x][y] = false;
            } else {
                if (neighbours == 3) newGrid[x][y] = true;
            }
        }
    }
    grid = newGrid;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WIDTH, HEIGHT)), "Conway's Game of Life");
    window.setFramerateLimit(60);

    std::vector<std::vector<bool>> grid(GRID_WIDTH, std::vector<bool>(GRID_HEIGHT, false));
    bool simulationRunning = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                simulationRunning = !simulationRunning;

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i pos = getMousePositionInGrid(window);
                pos.x = wrapCoordinate(pos.x, GRID_WIDTH);
                pos.y = wrapCoordinate(pos.y, GRID_HEIGHT);
                grid[pos.x][pos.y] = !grid[pos.x][pos.y];
            }
        }

        window.clear(sf::Color::Black);
        for (int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE));
                cell.setFillColor(grid[x][y] ? sf::Color::White : sf::Color::Black);
                window.draw(cell);
            }
        }
        window.display();

        if (simulationRunning) {
            updateGrid(grid);
        }
    }

    return 0;
}
