# Gym Tracker App (Qt/QML + SQLite)

## Goal
- Build a gym-tracking app: machines (name, muscle_group, weight range), exercises/sets, and sessions with logged reps/weights.
- Keep frontend (QML) separate from backend (C++ core/data).
- Ready to swap SQLite for Postgres later, but default to local SQLite for development.

## Tech / Tools
- Qt 6 (Quick + QuickControls2) with CMake and `qt_add_executable`.
- QML for UI, C++ for backend/core, SQLite for storage; JSON in `data/` for seed/config data.
- Keep project buildable at all times (CMake must configure and compile).

## Repo layout
- `/app`      C++ entry point / application wiring
- `/core`     backend/domain code
- `/model`    Qt models/viewmodels exposed to QML
- `/data`     SQLite files, seed JSON, images, other resources
- `/sql`      database backend scripts/migrations
- `/qml`      frontend QML
- (root)      CMakeLists.txt, project docs (like this file)

## Working notes
- Follow the plants project CMake style (Qt targets via `qt_add_executable`, resources via `qt_add_resources` or similar).
- Keep DB access out of QML; expose models/services from C++.
