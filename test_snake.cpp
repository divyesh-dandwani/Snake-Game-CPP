#define UNIT_TEST
#include "../snake_gamebox.cpp"
#include <cassert>
#include <iostream>
#include <vector>

// ==========================================
// Test Double: Stub for IRandom Seam (Part E)
// ==========================================
// StubRandom provides canned answers to nextInt() calls to force
// deterministic behavior in generateFood().
class StubRandom : public IRandom {
private:
    std::vector<int> values;
    size_t index;

public:
    StubRandom(const std::vector<int>& cannedValues)
        : values(cannedValues), index(0) {}

    int nextInt(int max) override {
        if (values.empty()) return 0;
        int val = values[index % values.size()];
        index++;
        return val % max;
    }
};

// ==========================================
// Unit Test 1: Obstacle Collision (Rule 1)
// ==========================================
void test_hit_block_collision() {
    std::vector<Point> blocks = {{5, 5}, {6, 5}, {7, 5}};
    Point headHitting{5, 5};
    Point headFree{10, 10};

    // Tests pure logic of hitBlock without I/O
    assert(hitBlock(blocks, headHitting) == true);
    assert(hitBlock(blocks, headFree) == false);
    std::cout << "[PASS] test_hit_block_collision\n";
}

// ==========================================
// Unit Test 2: Seam Test - Food Placement (Rule 5)
// ==========================================
void test_generate_food_deterministic_placement() {
    int W = 78, H = 25;
    std::vector<Point> snake = {{15, 10}, {14, 10}, {13, 10}};
    std::vector<Point> blocks = {{20, 10}, {21, 10}};

    // Canned sequence: first candidate coordinates (13, 8) in nextInt coordinate space
    // maps to f.x = 13 + 2 = 15, f.y = 8 + 2 = 10 (collides with snake head at 15,10).
    // Second candidate (28, 13) maps to f.x = 30, f.y = 15 (free cell).
    StubRandom stubRng({13, 8, 28, 13});

    Point food = generateFood(W, H, snake, blocks, &stubRng);

    // Verify candidate on snake was rejected and free position was chosen
    assert(food.x == 30);
    assert(food.y == 15);
    assert(!hitBlock(blocks, food));
    std::cout << "[PASS] test_generate_food_deterministic_placement\n";
}

// ==========================================
// Unit Test 3: Multi-Assertion Test (Demonstrating Test Smells for Part F)
// ==========================================
void test_eager_food_and_obstacle_checks() {
    int W = 78, H = 25;
    std::vector<Point> snake = {{10, 10}, {9, 10}};
    std::vector<Point> blocks = {{12, 12}};

    StubRandom stubRng({18, 8});
    Point food = generateFood(W, H, snake, blocks, &stubRng);

    // Test Smell 1: Assertion Roulette (lines 78-82)
    // Run of bare asserts with no explanatory failure message
    assert(food.x > 0);
    assert(food.x < W - 1);
    assert(food.y > 0);
    assert(food.y < H - 1);
    assert(!(food.x == 10 && food.y == 10));

    // Test Smell 2: Eager Test (line 68: test_eager_food_and_obstacle_checks)
    // One test function verifying boundary rules, snake non-overlap, and obstacle collisions
    assert(hitBlock(blocks, {12, 12}) == true);
    assert(hitBlock(blocks, {10, 10}) == false);
    std::cout << "[PASS] test_eager_food_and_obstacle_checks\n";
}

int main() {
    std::cout << "Running Snake Game Test Suite...\n";
    test_hit_block_collision();
    test_generate_food_deterministic_placement();
    test_eager_food_and_obstacle_checks();
    std::cout << "All unit tests executed successfully.\n";
    return 0;
}
