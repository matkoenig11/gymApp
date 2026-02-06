# Session -> Exercises -> Sets

This app tracks a workout as a hierarchy: one **session** contains ordered **exercises**, and each exercise owns ordered **exercise_sets**.

## Entities
- **Session**: one workout; has start time (auto "now"), optional end time, and a note.
- **Exercise**: a movement performed inside a session; links to exactly one session, can reference a machine (or store the machine name snapshot), carries an order index and optional comment.
- **Exercise Set**: a single set inside an exercise; numbered per exercise (1, 2, 3...), stores reps, weight in lbs, optional RPE, and warm-up flag.

## Relationships & Rules
- Session -> Exercises: one-to-many; deleting a session deletes its exercises (and their sets).
- Exercise -> Exercise Sets: one-to-many; deleting an exercise deletes its sets.
- Exercise -> Machine (optional): deleting a machine just nulls the link; history stays because the name snapshot is kept.
- Constraints: set_number starts at 1 and is unique within an exercise; reps >= 0; machine names are unique; weight_min <= weight_max on machines.

## UI Walkthrough (current QML views)
1) **Open editor**
   - In `SessionView.qml`, press `Start New Session` to create a fresh session (SessionEditor loads with a blank session), or select a row in the left list and press `Edit Selected` to load that session into the editor tab.
2) **Session header actions** (`SessionEditorPane.qml`)
   - `End Session`: sets `ended_at` to now for the loaded session.
   - `Delete`: removes the session and cascades deletion to its exercises and sets.
3) **Session note**
   - Edit the note text area and press `Save Note` to persist it.
4) **Add exercise to this session**
   - In the "Add Exercise" box: pick a machine from the combo box (or leave it and type a custom name), optionally enter a comment, then press `Add`. The exercise is appended with the next `order_index`.
5) **Manage exercises** (per card in `ExerciseCardEditor.qml`)
   - Reorder with the `Move Up` / `Move Down` arrow buttons.
   - `Delete` removes this exercise (its sets are also removed).
   - Update comment in the text area and press `Save Notes`.
6) **Add sets inside an exercise**
   - On the card's "Add set" row: set `reps`, `weight`, `RPE`, and `Warmup`, then press `Add`. The new set gets `set_number = last + 1`.
7) **Edit or remove a set** (`SetRowEditor.qml`)
   - Each set row shows its number and fields. Change values and press `Update` to save; press `Delete` to remove that set.
8) **View vs edit**
   - Use the right-side tabs in `SessionView.qml`: `Details` to view the session (read-only list from `SessionDetail`), `Edit` to modify it (`SessionEditorPane`).

## UX Acceptance Notes
- User always sees exercises filtered to the open session; sets are shown inline per exercise card.
- Reordering exercises only affects the current session.
- Removing a session removes its exercises/sets; removing an exercise removes its sets only.
- Machine delete does not break history because exercises keep the machine name snapshot and null the machine link.
