#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

using namespace std;
using namespace sf;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 400;
const int Q_TABLE_SIZE = 100;  // States (distance buckets)
const float LEARNING_RATE = 0.1f;
const float DISCOUNT_FACTOR = 0.9f;

// Q-table: States (distance) x Actions (0 = no jump, 1 = jump)
float q_table[Q_TABLE_SIZE][2] = { 0 };

// Function to select an action using epsilon-greedy policy
int chooseAction(int state, float epsilon) {
    if ((rand() % 100) < (epsilon * 100)) {
        return rand() % 2;  // Explore: Random action
    }
    // Exploit: Choose action with highest Q-value
    return (q_table[state][1] > q_table[state][0]) ? 1 : 0;
}

// Update Q-table
void updateQTable(int state, int action, int reward, int nextState) {
    float maxNextQ = max(q_table[nextState][0], q_table[nextState][1]);
    q_table[state][action] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * maxNextQ - q_table[state][action]);
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Initialize SFML window
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dino RL Bot with Images");
    window.setFramerateLimit(60);

    // Load textures
    Texture dinoTexture, cactusTexture;
    if (!dinoTexture.loadFromFile("C:\\Users\\user\\Downloads\\Dino.png") ||
        !cactusTexture.loadFromFile("C:\\Users\\user\\Downloads\\cactus.png")) {
        cerr << "Error loading images!" << endl;
        return -1;
    }


    // Load font
    Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf")) {
        cerr << "Error loading font!" << endl;
        return -1;
    }

    // Create sprites
    Sprite dino(dinoTexture);
    Sprite cactus(cactusTexture);

    // Scale images to fit the game
    dino.setScale(1.0f, 1.0f);  // Adjust scale
    cactus.setScale(1.0f, 1.0f);

    // Set initial positions
    dino.setPosition(100, 300);
    cactus.setPosition(800, 300);

    // Text objects for score and crashes
    Text scoreText, crashText, actionText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(10, 10);

    crashText.setFont(font);
    crashText.setCharacterSize(20);
    crashText.setFillColor(Color::Black);
    crashText.setPosition(10, 40);

    actionText.setFont(font);
    actionText.setCharacterSize(20);
    actionText.setFillColor(Color::Black);
    actionText.setPosition(10, 70);

    int dinoYVelocity = 0;
    bool isJumping = false;

    float cactusSpeed = 5.0f;
    int score = 0;
    int crashes = 0;
    int state = 0;  // Current state (distance bucket)
    int action = 0; // Current action (0 or 1)

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // Calculate current state (distance bucket)
        int distance = cactus.getPosition().x - dino.getPosition().x;
        state = max(0, min(Q_TABLE_SIZE - 1, distance / 10));  // Bucket the distance

        // Choose an action (epsilon-greedy)
        action = chooseAction(state, 0.1f);  // Epsilon = 0.1 (10% exploration)

        // Apply action
        if (action == 1 && !isJumping) {  // Jump
            isJumping = true;
            dinoYVelocity = -15;
        }

        // Update Dino's position
        if (isJumping) {
            dino.move(0, dinoYVelocity);
            dinoYVelocity += 1;  // Simulate gravity
            if (dino.getPosition().y >= 300) {  // Back to ground
                dino.setPosition(100, 300);
                isJumping = false;
            }
        }

        // Move cactus
        cactus.move(-cactusSpeed, 0);
        if (cactus.getPosition().x < -20) {
            cactus.setPosition(800, 300);  // Reset cactus
            score++;
        }

        // Check collision
        int reward = 1;  // Default reward for survival
        if (cactus.getGlobalBounds().intersects(dino.getGlobalBounds())) {
            reward = -100;  // Negative reward for collision
            crashes++;
            cactus.setPosition(800, 300);  // Reset cactus
        }

        // Update Q-table
        int nextDistance = cactus.getPosition().x - dino.getPosition().x;
        int nextState = max(0, min(Q_TABLE_SIZE - 1, nextDistance / 10));
        updateQTable(state, action, reward, nextState);

        // Update score and crashes
        ostringstream oss;
        oss << "Score: " << score;
        scoreText.setString(oss.str());

        oss.str("");
        oss << "Crashes: " << crashes;
        crashText.setString(oss.str());

        // Display action taken
        actionText.setString("Action: " + string(action == 1 ? "Jump" : "Run"));

        // Clear screen
        window.clear(Color::White);

        // Draw objects
        window.draw(dino);
        window.draw(cactus);
        window.draw(scoreText);
        window.draw(crashText);
        window.draw(actionText);

        window.display();
    }

    return 0;
}