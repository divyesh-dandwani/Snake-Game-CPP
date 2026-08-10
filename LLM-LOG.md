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

### Attempt Outcome
- **Working Attempt**: Attempt 1 worked on the first try.
- **Errors/Failures in earlier attempts**: None. Compiled cleanly on first compilation attempt with `g++ -std=c++11 snake_gamebox.cpp -o snake.exe`.
- **Total Prompts to Working Code**: 1 prompt.
- **Code Provided to LLM**: Whole repository / `snake_gamebox.cpp` (read directly by the agent from workspace).
