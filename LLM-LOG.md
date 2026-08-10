# LLM Usage Log (LLM-LOG.md)

## 1. Setup
- **Model(s)**: Gemini 3.6 Flash
- **Tool / harness**: Antigravity IDE (Agentic CLI / IDE)
- **IDE / editor**: VS Code / Antigravity
- **Did you paste this assignment document into it?**: Yes (Uploaded PDF images and OCR text of Lab Assignment 3 into context)

## 2. Session Log

### Prompt 1
> "This is the assignment I've to accomplish, so help me deal with it and guide me step by step if I've to do anything from my side manually. One thing I've done is I've forked the https://github.com/divyesh-dandwani/Snake-Game-CPP.git repo into mine: https://github.com/LoveShah21/Snake-Game-CPP.git. Guide me further" [Attached Assignment PDF Document]

### Relevant Reply Output (Code Produced)
The assistant analyzed the repository structure (`snake_gamebox.cpp`), ensured line endings were normalized (`core.autocrlf false`), and implemented multiplayer support by modifying:
1. `generateFood()` — Signature updated to accept `snake1` and `snake2` vectors to ensure generated food does not spawn inside either snake body.
2. `showScoreboard()` — Updated parameters to take `score1` and `score2` and displayed both player scores (`P1:` and `P2:`) along with controls (`P1:Arrows │ P2:WASD`).
3. `main()` — Initialized two snake vectors (`snake1` and `snake2`), assigned Arrow keys to Player 1 and WASD to Player 2, implemented collision checks for wall/self/other-snake with announcements for which player lost, handled shared fruit growth, and rendered both snakes.

### Prompt 2
> PR Review Feedback:
> 1) Collision resolution misses the case where both players move their heads into the same cell on the same tick (head1 == head2). Also body-collision loops include tail segment which produces false-positive collisions when snake moves without eating food.
> 2) len1 and len2 are unused variables.

### Relevant Reply Output (Code Produced)
1. Updated collision logic in `main()` to explicitly handle head-to-head collisions (`head1.x == head2.x && head1.y == head2.y`), causing both players to lose.
2. Evaluated food eating (`ateFood1`, `ateFood2`) prior to checking body collisions, dynamically calculating body check lengths (`checkLen = ateFood ? snake.size() : snake.size() - 1`) to prevent false-positive collisions on moving tail segments.
3. Removed unused `len1` and `len2` variable declarations and increments.

### Attempt Outcome
- **Working Attempt**: Attempt 2 resolved all edge-case review findings.
- **Errors/Failures in earlier attempts**: Initial version missed head-to-head edge case and included tail segment in non-growing body collision checks.
- **Total Prompts to Working Code**: 2 prompts.
- **Code Provided to LLM**: Whole repository / `snake_gamebox.cpp`.
