# Lab 4

| | |
|---|---|
| Repository | https://github.com/divyesh-dandwani/Snake-Game-CPP |
| Base tag | `lab4-base` at commit `205fd2a` |
| Pull request | https://github.com/divyesh-dandwani/Snake-Game-CPP/pull/1 |

---

## 1. Five rules — [5]

Written before opening the source. Behaviour, with an observable outcome.

| # | Rule |
|---|---|
| 1 | When the snake's head coordinates match any active obstacle block coordinate, the game immediately terminates (`gameOver = true`). |
| 2 | When the snake's head coordinates enter any coordinate occupied by its own body segments, the game immediately terminates (`gameOver = true`). |
| 3 | When the snake's head coordinates reach or cross the perimeter boundary walls, the game immediately terminates (`gameOver = true`). |
| 4 | When the snake eats food, the player score increments by 1 and the snake extends by retaining its tail segment instead of popping it. |
| 5 | Newly generated food never spawns on a coordinate currently occupied by any segment of the snake or by obstacle blocks. |

---

## 2. What you could test, and what stopped you — [10]

No source changes in this part. Every `file:line` below is a line in `lab4-base`.

| # | Rule | Test written? | Blocking dependency (`file:line` + what it is) |
|---|---|---|---|
| 1 | Obstacle collision detection | Yes | None (pure function in `snake_gamebox.cpp:187`) |
| 2 | Self-collision ends the game | No | `snake_gamebox.cpp:402` — `selectTheme()` calling `Terminal::getChar()` (`_getch()`) in an interactive loop inside `main()`, blocking automated execution |
| 3 | Wall collision ends the game | No | `snake_gamebox.cpp:402` — `selectTheme()` calling `Terminal::getChar()` in interactive menu loop inside `main()`, preventing automated headless execution |
| 4 | Eating food increases score and length | No | `snake_gamebox.cpp:496` — `Terminal::sleep_ms(1)` and `chrono::steady_clock` delay loop in `main()`, coupled with interactive `Terminal::getChar()` at line 431 |
| 5 | Food spawns only on unoccupied cells | No | `snake_gamebox.cpp:219` — `rand()` called directly inside `generateFood()`, non-deterministic coordinate generation preventing repeatable assertions |

> **Rules testable without modifying the source: 1 / 5**

"It needs user input" is not a blocking dependency. `main.cpp:214 — getch() called inside the game loop` is.

---

## 3. Coverage, and what it missed — [6]

| | |
|---|---|
| Line coverage | 14.2 % |
| Branch coverage | 11.8 % |
| Command used | `g++ --coverage -O0 -g -DUNIT_TEST snake_gamebox.cpp tests/test_snake.cpp -o tests/tests && ./tests/tests && gcov -b snake_gamebox.cpp` |

**One rule that is executed by the suite but not verified by it:**

| | |
|---|---|
| Rule | Food accessibility / free neighbor count requirement (candidate food must have >= 2 accessible adjacent cells) |
| Line that runs | `snake_gamebox.cpp:223` |
| The assertion that is missing | `assert(freeNeighborCount(food) >= 2)` to verify that the generated food coordinate is actually reachable |

---

## 4. The seam — [10]

| | |
|---|---|
| Rule made testable | Food never appears on a cell occupied by the snake or obstacle blocks (`generateFood` deterministic placement) |
| Commit 1 (seam) | `b40be21` |
| Commit 2 (test) | `HEAD` |
| Seam kind | object |
| Enabling point | `IRandom* rng = nullptr` parameter on `generateFood()` (falling back to `DefaultRandom` when null) |
| What production code gave up | Direct coupling to global C-runtime `rand()`. Introduced virtual method indirection (`IRandom::nextInt`), trading zero-cost static dispatch for runtime substitutability during tests. |

The last row is graded. If the honest answer is "nothing", write that and say why the seam cost nothing here.

---

## 5. The double — [4]

| | |
|---|---|
| What you passed through the seam | stub |
| The method under test | `generateFood` |

Two sentences: was the collaborator asked a question or told to do something, and why does that decide the answer above?

The collaborator (`IRandom`) was asked a question (`nextInt`) to provide candidate coordinate integers rather than told to perform an action or side-effect. Because the method under test inspects and uses the returned coordinate values to make placement decisions, the test double supplies canned answers and is therefore a stub rather than a mock or spy.

---

## 6. Two smells in your own tests — [5]

| | Smell | `file:line` | One-line fix |
|---|---|---|---|
| 1 | Assertion roulette | `tests/test_snake.cpp:78` | Add explicit failure messages to assertions: `assert((food.x > 0) && "food x must be within left wall");` |
| 2 | Eager test | `tests/test_snake.cpp:68` | Split into separate test functions: `test_food_inside_bounds()` and `test_food_not_on_obstacles()` |

Fixing them is optional. Finding them is not.
