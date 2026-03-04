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
- Treat QML warnings as errors: new work must run cleanly without runtime QML warnings.
- When a task is completed, **always compile and run** before calling it done; if you can’t, say so explicitly.
  - Typical build command (Debug): `cmake --build build --config Debug`
  - Typical sanity run (desktop): `build/Debug/gymapp_bin.exe`
  - Targeted tests we rely on: `ctest -C Debug -R "Session(DetailModelTest|EditorModelInvokeTest)"` (extend as needed)
- Preserve UI state where possible; avoid regressions like scroll jumps or auto-expanding cards unless explicitly requested.
- **CRITICAL**: Never overwrite or revert the latest user-edited lines/files. If there’s any doubt, ask before touching them. Always prefer minimal diffs that respect the user’s recent changes.
