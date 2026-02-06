# Session CRUD Plan

## Current Snapshot
- Schema already supports `sessions`, ordered `exercises` (`order_index`), and ordered `exercise_sets` (`set_number` with UNIQUE per exercise).
- Backend: `SessionRepository` only lists sessions; `MachineRepository` is read-only. No create/update/delete paths for sessions, exercises, or sets.
- Models: `SessionListModel` and `SessionDetailModel` are read-only; QML (`SessionView.qml`) only shows lists and details, no editing UI.

## Backend Work (repositories/services)
- **SessionRepository additions**
  - `int createSession(const QString &startedAt, const QString &note);`
  - `bool updateSessionMeta(int id, const QString &startedAt, const QString &note);`
  - `bool endSession(int id, const QString &endedAt);`
  - `bool deleteSession(int id);`
  - `Session fetchOne(int id);` (for editor preload).
- **Exercise operations** (same class or new `ExerciseRepository` to avoid bloat)
  - `int addExercise(int sessionId, std::optional<int> machineId, const QString &machineNameSnapshot, const QString &comment, int orderIndex = -1);`
  - `bool updateExercise(int exerciseId, std::optional<int> machineId, const QString &machineNameSnapshot, const QString &comment);`
  - `bool removeExercise(int exerciseId);` (auto-shift remaining order indices).
  - `bool reorderExercises(int sessionId, const QList<int> &exerciseIdsInOrder);`
  - `QVector<ExerciseRow> fetchExercises(int sessionId);`
- **Set operations** (could live with exercises repo or a small `SetRepository`)
  - `int addSet(int exerciseId, int setNumber, int reps, double weightLbs, double rpe, bool isWarmup);`
  - `bool updateSet(int setId, int setNumber, int reps, double weightLbs, double rpe, bool isWarmup);`
  - `bool removeSet(int setId);`
  - `bool resequenceSets(int exerciseId);` (keep UNIQUE `(exercise_id, set_number)` intact after inserts/deletes).
- **Cross-cutting**
  - Wrap multi-step edits in transactions.
  - Keep `PRAGMA foreign_keys=ON` and rely on `ON DELETE CASCADE` for exercises/sets.
  - Return IDs on create so models can update immediately.

## Model Layer (QML-facing)
- **SessionDetailModel**: keep as read-only viewer; refactor to reuse new fetch helpers for consistency.
- **New SessionEditorModel (singleton)**
  - Holds editable state for one session (exercises + sets).
  - API:
    - `Q_INVOKABLE bool loadSession(int sessionId);` (new or existing; creates in DB if `sessionId < 0`).
    - `Q_INVOKABLE int addExercise(int machineId, const QString &customName, const QString &comment);`
    - `Q_INVOKABLE bool updateExercise(int exerciseId, const QString &comment, int machineId, const QString &customName);`
    - `Q_INVOKABLE bool moveExercise(int exerciseId, int toIndex);`
    - `Q_INVOKABLE bool removeExercise(int exerciseId);`
    - `Q_INVOKABLE int addSet(int exerciseId, int reps, double weight, double rpe, bool warmup);`
    - `Q_INVOKABLE bool updateSet(int setId, int reps, double weight, double rpe, bool warmup);`
    - `Q_INVOKABLE bool removeSet(int setId);`
    - `Q_INVOKABLE bool updateSessionNote(const QString &note);`
    - `Q_INVOKABLE bool endSessionNow();` (sets `ended_at` to current time).
  - Emits `sessionChanged`, `exercisesChanged`, `setsChanged`, and hooks to refresh `SessionList`/`SessionDetail` after commits.
  - Expose roles for nested views: machine name, comment, ordered sets, etc.
- **Optional helper models**: lightweight `ExerciseListModel` and `SetListModel` to simplify nested QML bindings; can be internal to `SessionEditorModel`.

## QML Components to Add (one by one)
- `SessionEditorPage` (replaces/augments the right pane in `SessionView.qml`):
  - Shows session meta (date, note, end-session button) and list of exercises with inline set editors.
- `SessionHeaderForm`:
  - Inputs for start time (read-only for now), note text area, `End Session`/`Delete Session` buttons.
- `ExercisePickerDialog`:
  - Lists machines from `MachineList`; allows free-text custom machine name; returns selection to editor.
- `ExerciseCardEditor`:
  - Displays machine name, note field, reorder drag handle, `Add Set`, `Edit`, `Delete` actions.
- `SetRowEditor`:
  - Inline row with `set #`, reps, weight, RPE, warmup toggle, `Save`/`Delete`.
- `AddSetRow`:
  - Compact inputs to quickly append a new set (pre-fills next set_number).
- `ConfirmationDialog`:
  - Reusable modal for destructive actions (remove exercise / remove set / delete session).
- `FloatingActionButton` or primary button:
  - `Start New Session` -> calls `SessionEditorModel.loadSession(-1)` and switches to editor view.

## Step-by-Step Implementation Order
1. Backend CRUD: extend repositories (or add Exercise/Set repos) with create/update/delete + ordering/resquence, wrapped in transactions.
2. Wire repository instances into models (setDatabase in `app/main.cpp` wiring) similar to existing pattern.
3. Build `SessionEditorModel` with in-memory representation mirroring DB rows; keep read-only `SessionDetailModel` intact.
4. Update `SessionView.qml` to add an editor pane:
   - Add "Start Session" button.
   - Swap detail view to editor when a session is loaded/created.
   - Use new components for exercises and sets.
5. Add dialogs/forms: picker, confirmation, inline set editor; ensure all actions call `SessionEditorModel` methods.
6. Refresh flows: after any commit, reload `SessionList` and (if open) `SessionDetail` to keep read-only views in sync.
7. Testing: add lightweight C++ tests (in-memory SQLite) for repository CRUD and ordering; manual QML smoke test for add/edit/remove paths.
