# Gym Tracker Planning

## Terminology & Data Model
- Machine: equipment entry with `name`, `muscle_group`, `weight_min`, `weight_max`, `note`.
- Session: a workout event with `id`, `started_at`, optional `ended_at`, `note`, and ordered exercises.
- Exercise (within a session): references a machine (or name snapshot), optional comment, and ordered sets.
- Set (repetition block): fields `set_number`, `reps`, `weight_lbs`, optional `rpe`, `is_warmup`. One set = one performance at a weight for a count of reps.
- Example (from `data/gym_21_1_2026.json`):
  - Session started_at `2026-01-21T18:51:00`, note "Imported from manual log."
  - Exercise "Leg Press" (B6): sets (1:10x280, 2:10x280, 3:10x280, 4:15x280).
  - Exercise "Horizontal Pull" (B4): sets (1:8x95, 2:8x95, 3:8x95, 4:12x95).

## Components Needed (UI & Backend)
- Navigation shell: top bar/menu, page stack (Home, Machines, Session Log, History, Settings).
- Machines view: list/filter machines; detail/edit sheet; add machine flow.
- Session logger:
  - Start session button → current session state (date, note).
  - Exercise picker (machine list/search), ability to add ad-hoc machine name.
  - Set entry list per exercise (reps, weight, RPE, warmup flag), copy-last/add-next shortcuts, validation vs machine weight range.
  - Save/end session; mark ended_at.
- Session history:
  - List sessions (date, duration, total sets/tonnage).
  - Session detail: exercises and sets; edit/delete set; duplicate session to new.
  - Export/import hooks (JSON/CSV).
- Analytics (later): per-machine bests, weekly volume, PR badges.
- Settings/storage:
  - DB path selector (SQLite default, future Postgres).
  - Backup/restore; unit toggle (lbs/kg); feature flags.

## Backend Exposure (Qt 6.2+ pattern)
- Avoid QML context properties; expose C++ types via `qt_add_qml_module` + `QML_NAMED_ELEMENT`/`QML_SINGLETON` macros.
  - Backend-facing services stay in `/core`; QML-facing models/viewmodels live in `/model`.
  - Use QML imports (e.g., `import GymApp`) to access `MachineListModel`, `LoaderModel`, etc.
- Keep QML free of SQL; all DB work stays in C++ repositories/services.

## Done
- Machines list exposed as a QML singleton (`MachineList`) via `qt_add_qml_module`; QML imports the type without context properties.
- Models separated into `/model`, backend in `/core`, QML in `/qml`; build uses `qt_add_qml_module`.
- Seeded SQLite loaded directly; machines render in the UI. LoaderModel and Backend are QML-exposed; seed loader button added. Sessions repository/model and sessions tab are in place.

## Next Steps
- Expose DB switching in QML (Backend singleton) and surface current path/seed status in UI.
- Flesh out Session Log and History pages (per-exercise/sets view & edit) and add Settings page; route via StackView/TabBar.
- Implement session detail model (exercises/sets CRUD) and link to logger UI.
- Add import/export utilities (JSON/CSV) consistent with the example structure and wire to menu actions.
