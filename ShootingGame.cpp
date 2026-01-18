#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>
using namespace sf;

const int WIDTH = 1200;
const int HEIGHT = 800;

enum GameState { MENU, PLAYING, GAME_OVER };

struct Bullet {
    RectangleShape shape;
    bool active;
    bool fromEnemy;
};

struct Enemy {
    CircleShape shape;
    RectangleShape healthBar;
    float health;
    bool active;
};

// Game variables
int score = 0;
int lives = 3;
int level = 1;
int enemiesKilled = 0;
float shootTimer = 0;
float spawnTimer = 0;
float invincibleTimer = 0;
GameState state = MENU;

Vector2f playerPos;
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;

void createBullet(float x, float y, bool fromEnemy) {
    Bullet b;
    b.shape.setSize(Vector2f(5, 15));
    b.shape.setFillColor(fromEnemy ? Color::Red : Color::Yellow);
    b.shape.setPosition(x, y);
    b.active = true;
    b.fromEnemy = fromEnemy;
    bullets.push_back(b);
}

void createEnemy() {
    Enemy e;
    e.shape.setRadius(20);
    e.shape.setFillColor(Color(200, 0, 0));
    e.shape.setPosition(rand() % (WIDTH - 40), -40);
    e.healthBar.setSize(Vector2f(40, 4));
    e.healthBar.setFillColor(Color::Green);
    e.health = level;
    e.active = true;
    enemies.push_back(e);
}

void resetGame() {
    playerPos = Vector2f(WIDTH / 2, HEIGHT - 80);
    bullets.clear();
    enemies.clear();
    score = 0;
    lives = 3;
    level = 1;
    enemiesKilled = 0;
    shootTimer = 0;
    spawnTimer = 0;
    invincibleTimer = 0;
}

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Space Combat");
    window.setFramerateLimit(60);

    // Player
    RectangleShape player(Vector2f(40, 40));
    player.setFillColor(Color::Green);
    player.setOrigin(20, 20);
    resetGame();

    // Font
    Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    if (!font.getInfo().family.empty()) {}
    else font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

    Text scoreText, livesText, levelText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setPosition(10, 10);
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setPosition(10, 40);
    levelText.setFont(font);
    levelText.setCharacterSize(24);
    levelText.setPosition(10, 70);

    Text titleText;
    titleText.setFont(font);
    titleText.setCharacterSize(72);
    titleText.setFillColor(Color::Cyan);
    titleText.setString("SPACE COMBAT");
    titleText.setPosition(WIDTH / 2 - 300, 200);

    Text menuText;
    menuText.setFont(font);
    menuText.setCharacterSize(32);
    menuText.setString("Press SPACE to Play\nPress ESC to Exit");
    menuText.setPosition(WIDTH / 2 - 180, 400);

    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        // ===== MENU =====
        if (state == MENU) {
            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                state = PLAYING;
                resetGame();
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();
        }

        // ===== PLAYING =====
        else if (state == PLAYING) {
            shootTimer += dt;
            spawnTimer += dt;
            invincibleTimer -= dt;

            // Player movement
            float speed = 300 * dt;
            if (Keyboard::isKeyPressed(Keyboard::Left) && playerPos.x > 20)
                playerPos.x -= speed;
            if (Keyboard::isKeyPressed(Keyboard::Right) && playerPos.x < WIDTH - 20)
                playerPos.x += speed;
            if (Keyboard::isKeyPressed(Keyboard::Up) && playerPos.y > 20)
                playerPos.y -= speed;
            if (Keyboard::isKeyPressed(Keyboard::Down) && playerPos.y < HEIGHT - 20)
                playerPos.y += speed;

            // Player shooting
            if (Keyboard::isKeyPressed(Keyboard::Space) && shootTimer > 0.2f) {
                createBullet(playerPos.x, playerPos.y - 30, false);
                shootTimer = 0;
            }

            // Spawn enemies
            float spawnRate = 1.5f - level * 0.1f;
            if (spawnRate < 0.5f) spawnRate = 0.5f;
            if (spawnTimer > spawnRate) {
                createEnemy();
                spawnTimer = 0;
            }

            // Update bullets
            for (auto &b : bullets) {
                if (b.active) {
                    float speed = b.fromEnemy ? 200 : -400;
                    b.shape.move(0, speed * dt);
                    if (b.shape.getPosition().y < -20 || b.shape.getPosition().y > HEIGHT + 20)
                        b.active = false;
                }
            }

            // Update enemies
            for (auto &e : enemies) {
                if (e.active) {
                    e.shape.move(0, (100 + level * 10) * dt);
                    
                    // Enemy shooting
                    if (rand() % 100 < 1)
                        createBullet(e.shape.getPosition().x + 20, e.shape.getPosition().y + 20, true);
                    
                    if (e.shape.getPosition().y > HEIGHT + 50)
                        e.active = false;
                }
            }

            // Collision: Player bullets vs Enemies
            for (auto &b : bullets) {
                if (!b.active || b.fromEnemy) continue;
                for (auto &e : enemies) {
                    if (!e.active) continue;
                    if (b.shape.getGlobalBounds().intersects(e.shape.getGlobalBounds())) {
                        b.active = false;
                        e.health--;
                        if (e.health <= 0) {
                            e.active = false;
                            score += 10 * level;
                            enemiesKilled++;
                            if (enemiesKilled >= 10) {
                                level++;
                                enemiesKilled = 0;
                            }
                        } else {
                            e.healthBar.setSize(Vector2f(40 * e.health / level, 4));
                        }
                        break;
                    }
                }
            }

            // Collision: Enemy bullets vs Player
            if (invincibleTimer <= 0) {
                for (auto &b : bullets) {
                    if (b.active && b.fromEnemy) {
                        float dx = playerPos.x - b.shape.getPosition().x;
                        float dy = playerPos.y - b.shape.getPosition().y;
                        if (sqrt(dx*dx + dy*dy) < 30) {
                            b.active = false;
                            lives--;
                            invincibleTimer = 2.0f;
                            if (lives <= 0)
                                state = GAME_OVER;
                        }
                    }
                }

                // Player vs Enemies
                for (auto &e : enemies) {
                    if (e.active) {
                        float dx = playerPos.x - (e.shape.getPosition().x + 20);
                        float dy = playerPos.y - (e.shape.getPosition().y + 20);
                        if (sqrt(dx*dx + dy*dy) < 40) {
                            e.active = false;
                            lives--;
                            invincibleTimer = 2.0f;
                            if (lives <= 0)
                                state = GAME_OVER;
                        }
                    }
                }
            }

            // Update HUD
            std::ostringstream ss;
            ss << "Score: " << score;
            scoreText.setString(ss.str());
            ss.str("");
            ss << "Lives: " << lives;
            livesText.setString(ss.str());
            ss.str("");
            ss << "Level: " << level;
            levelText.setString(ss.str());
        }

        // ===== GAME OVER =====
        else if (state == GAME_OVER) {
            if (Keyboard::isKeyPressed(Keyboard::R))
                state = MENU;
        }

        // ===== RENDERING =====
        window.clear(Color::Black);

        if (state == PLAYING) {
            // Draw bullets
            for (auto &b : bullets)
                if (b.active)
                    window.draw(b.shape);

            // Draw enemies
            for (auto &e : enemies) {
                if (e.active) {
                    window.draw(e.shape);
                    e.healthBar.setPosition(e.shape.getPosition().x, e.shape.getPosition().y - 10);
                    window.draw(e.healthBar);
                }
            }

            // Draw player (flash when invincible)
            if (invincibleTimer <= 0 || (int)(invincibleTimer * 10) % 2 == 0) {
                player.setPosition(playerPos);
                window.draw(player);
            }

            // Draw HUD
            window.draw(scoreText);
            window.draw(livesText);
            window.draw(levelText);
        }
        else if (state == MENU) {
            window.draw(titleText);
            window.draw(menuText);
        }
        else if (state == GAME_OVER) {
            Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setCharacterSize(64);
            gameOverText.setFillColor(Color::Red);
            gameOverText.setString("GAME OVER");
            gameOverText.setPosition(WIDTH / 2 - 200, HEIGHT / 2 - 100);
            window.draw(gameOverText);

            Text scoreDisplay;
            scoreDisplay.setFont(font);
            scoreDisplay.setCharacterSize(32);
            std::ostringstream ss;
            ss << "Final Score: " << score << "\nPress R to Menu";
            scoreDisplay.setString(ss.str());
            scoreDisplay.setPosition(WIDTH / 2 - 150, HEIGHT / 2);
            window.draw(scoreDisplay);
        }

        window.display();
    }

    return 0;
}
