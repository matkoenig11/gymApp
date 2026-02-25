# GUI Rework Plan (mobile-first cleanup)

Goal: reduce on-screen clutter, keep touch-friendly interactions, and stage changes per QML file so we can implement incrementally.

## Main.qml
- Replace heavy scroll-within-stack with a lighter landing layout: hero card + two big buttons (“Sessions”, “Machines”) that navigate (push) to dedicated pages.
- Add a simple bottom nav (2 tabs) for quick switching; remove nested TabBar/SwipeView duplication.
- Keep margins/padding tuned for mobile; enforce single-column flow.

## HeaderBar.qml
- Keep back/menu, but auto-hide menu on narrow screens; move import/export into a slide-over menu/drawer to declutter the top bar.
- Increase title size slightly; add optional subtitle for current page (e.g., “Machines” / “Sessions”).

## DatabaseCard.qml
- Collapse advanced DB info behind an “Advanced…” expander; default view shows only current DB name/path and a “Change DB” button.
- Move seed/sample buttons into a secondary “Developer tools” section hidden by default.

## MachineListCard.qml
- Make list full-width and taller; move add button to a sticky footer bar (“+ Add machine”).
- Row tap opens editor in a modal sheet; keep per-row info minimal (name + muscle group + weight range); hide notes until expanded or in editor.
- Add top filter chip row: “All / Upper / Lower / Pull / Push / Custom” to reduce list size on mobile.

## MachineListItem.qml
- Default collapsed row: name + muscle group + weight range, chevron indicator.
- Expand on tap to reveal note and edit/delete actions; avoid separate edit buttons in header.

## SessionView.qml
- Replace side-by-side list/detail with a stacked flow: top session list (compact cards) and a toggle to open detail/editor views in a new page/stack push. On wide screens, allow two-column via `Pane` + `Visible` bindings.
- Add a “collapse session list” button; when collapsed, detail/editor fills the viewport.
- Provide floating “Start Session” button (FAB) for quick access.

## SessionListPage.qml (if used)
- Align styling with new session cards; keep it single-column with pull-to-refresh affordance (swipe or refresh button at top).

## SessionEditorPane.qml
- Break into accordions: Session meta (date, note), Exercises list, Add exercise.
- Keep exercises in a tall ListView; each ExerciseCardEditor starts collapsed (summary only); expand to edit sets/notes.
- Add bottom padding and a floating “Add Exercise” button that scrolls to the add section.

## ExerciseCardEditor.qml
- Default collapsed header: machine/custom name, muscle group, set count; chevron to expand.
- Expanded view: notes + sets + add-set row; move move-up/down/delete into an overflow menu to reduce button noise.

## SetRowEditor.qml
- Compact row layout; hide less-used fields behind an “Advanced” toggle (e.g., RPE, warmup) for mobile; keep reps/weight visible.

## SessionListPage.qml / SetRowEditor.qml (general)
- Ensure consistent 8/12/16 px spacing tokens; apply `font.pixelSize` aligned with Main defaults.

Implementation order (suggested sprints):
1) Navigation simplification: Main + HeaderBar + SessionView layout swap (stacked, collapsible list).
2) Exercise/session editor accordions + collapsed cards.
3) Machine flows: list item collapse + modal sheet editor + filters.
4) Polish pass: spacing tokens, typography, FAB, advanced collapsibles, drawer for imports.
