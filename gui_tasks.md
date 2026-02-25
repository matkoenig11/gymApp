# GUI Rework Task List (sequential, stop-go checkpoints)

We’ll execute tasks in order; after each task, we verify against the goal before moving on.

## Task 1 — Navigation simplification
Goal: Main/HeaderBar/SessionView use stacked navigation suited for mobile, with collapsible session list and FAB for “Start Session”.
- Convert SessionView to a single-column default layout; add collapse/expand toggle for session list.
- Add FAB for “Start New Session”.
- Ensure back navigation works via HeaderBar when pushing deeper views.

## Task 2 — Session editor accordions
Goal: SessionEditorPane broken into accordions (Meta, Exercises, Add Exercise); Exercise cards collapsed by default.
- Implement accordions and collapsed ExerciseCardEditor headers; move move/delete actions into overflow.
- Keep sets visible only when expanded.

## Task 3 — Machine list cleanup
Goal: Machine list minimal rows + modal sheet editor + filter chips.
- Collapsible MachineListItem rows (summary + chevron).
- Modal sheet (already exists) polished; add filter chips row (All/Upper/Lower/Pull/Push/Custom).
- Sticky “Add Machine” CTA.

## Task 4 — Database card declutter
Goal: DatabaseCard shows only essential info; advanced/seeding hidden behind expander.
- Add “Advanced” accordion with seed/import toggles; default collapsed.

## Task 5 — Polish pass
Goal: Consistent spacing, typography tokens, Material palette applied everywhere.
- Apply spacing tokens (8/12/16), font sizes from Main defaults.
- Ensure qtquickcontrols2.conf colors flow through controls; trim per-control inline colors.
