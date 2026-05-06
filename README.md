# Digital_Habit_Tracker

<img width="1224" height="687" alt="image" src="https://github.com/user-attachments/assets/665b24d0-01c7-47b9-adfe-b1eb9f1f964c" />

A simple C++ console-based project to help users track daily and weekly habits, maintain streaks, earn points, and monitor progress.

## Features

- Add new habits
- View all habits in a colorful terminal dashboard
- Search habits by name
- Mark habits as completed
- Delete habits
- Track streaks and points
- Earn badges based on consistency
- Generate progress reports
- View leaderboard
- Save and load data using file handling

## C++ Concepts Used

- Classes and Objects
- Inheritance and Polymorphism
- Operator Overloading
- Vectors
- Functions
- File Handling
- Control Flow and Loops

## Project Structure

```text
Digital_Habit_Tracker/
├── Habit_tracker.cpp
├── habit_data.txt
├── .gitignore
├── readme.md
└── .vscode/
    ├── launch.json
    └── tasks.json
```

## How to Run

1. Open the project folder in VS Code or any C++ IDE.
2. Compile the program:

```bash
g++ Habit_tracker.cpp -o Habit_tracker
```

3. Run the program:

```bash
./Habit_tracker
```

On Windows Command Prompt:

```bash
Habit_tracker.exe
```

## How It Works

- Users can create daily or weekly habits.
- Completing a habit increases streak and points.
- Badges are awarded based on streak milestones.
- Data is stored in `habit_data.txt` so progress is preserved across runs.

## Future Improvements

- Add date-wise tracking
- Add password/login system
- Add monthly analytics
- Build a GUI version
