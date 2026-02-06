"""Seed the local SQLite database from bundled JSON fixtures.

Usage:
    python sql/seed_sqlite.py --db gym.sqlite --data-dir data --migrations-dir sql/migrations

The script:
  1) Applies all SQL migrations in order.
  2) Seeds machines from data/machines.json if the table is empty.
  3) Seeds one sample session from data/gym_*.json if the sessions table is empty.
"""

import argparse
import json
import sqlite3
from pathlib import Path
from typing import Iterable


def apply_migrations(conn: sqlite3.Connection, migrations_dir: Path) -> None:
    """Executes every *.sql migration file in order."""
    migrations: Iterable[Path] = sorted(migrations_dir.glob("*.sql"))
    if not migrations:
        print(f"[migrations] no migration files found in {migrations_dir}")
        return

    conn.execute("PRAGMA foreign_keys = ON;")
    for migration in migrations:
        sql = migration.read_text(encoding="utf-8")
        print(f"[migrations] applying {migration.name}")
        conn.executescript(sql)


def seed_machines(conn: sqlite3.Connection, machines_path: Path) -> None:
    if not machines_path.exists():
        print(f"[seed] machines file missing: {machines_path}")
        return

    existing = conn.execute("SELECT COUNT(*) FROM machines").fetchone()[0]
    if existing > 0:
        print(f"[seed] machines table already populated ({existing} rows); skipping")
        return

    machines = json.loads(machines_path.read_text(encoding="utf-8"))
    print(f"[seed] inserting {len(machines)} machines from {machines_path}")
    for machine in machines:
        conn.execute(
            """
            INSERT INTO machines (name, muscle_group, weight_min, weight_max, note)
            VALUES (?, ?, ?, ?, ?)
            """,
            (
                machine.get("name"),
                machine.get("muscle_group"),
                machine.get("weight_min", 0),
                machine.get("weight_max", 500),
                machine.get("note"),
            ),
        )


def seed_sample_session(conn: sqlite3.Connection, session_path: Path) -> None:
    if not session_path.exists():
        print(f"[seed] sample session file missing: {session_path}")
        return

    existing = conn.execute("SELECT COUNT(*) FROM sessions").fetchone()[0]
    if existing > 0:
        print(f"[seed] sessions table already populated ({existing} rows); skipping sample session")
        return

    session = json.loads(session_path.read_text(encoding="utf-8"))
    started_at = session.get("session_date") or session.get("date") or "1970-01-01T00:00:00"
    note = session.get("note")

    cur = conn.execute(
        "INSERT INTO sessions (started_at, note) VALUES (?, ?)",
        (started_at, note),
    )
    session_id = cur.lastrowid
    exercises = session.get("exercises", [])
    print(f"[seed] inserting session with {len(exercises)} exercises from {session_path}")

    for order_index, exercise in enumerate(exercises):
        machine_name = exercise.get("machine")
        machine_id = None
        if machine_name:
            row = conn.execute(
                "SELECT id FROM machines WHERE name = ?",
                (machine_name,),
            ).fetchone()
            machine_id = row[0] if row else None

        cur = conn.execute(
            """
            INSERT INTO exercises (session_id, machine_id, machine_name_snapshot, order_index, comment)
            VALUES (?, ?, ?, ?, ?)
            """,
            (session_id, machine_id, machine_name, order_index, exercise.get("comment")),
        )
        exercise_id = cur.lastrowid

        sets = exercise.get("sets", [])
        for set_index, set_row in enumerate(sets):
            set_number = set_row.get("set") or (set_index + 1)
            conn.execute(
                """
                INSERT INTO exercise_sets (exercise_id, set_number, reps, weight_lbs, is_warmup)
                VALUES (?, ?, ?, ?, ?)
                """,
                (
                    exercise_id,
                    set_number,
                    set_row.get("reps", 0),
                    set_row.get("weight_lbs"),
                    1 if set_row.get("is_warmup") else 0,
                ),
            )


def main() -> None:
    parser = argparse.ArgumentParser(description="Seed SQLite database for Gym Tracker.")
    parser.add_argument("--db", type=Path, default=Path("gym.sqlite"), help="Path to the SQLite database file.")
    parser.add_argument("--data-dir", type=Path, default=Path("data"), help="Directory containing seed JSON files.")
    parser.add_argument(
        "--migrations-dir",
        type=Path,
        default=Path("sql") / "migrations",
        help="Directory containing SQL migration files.",
    )
    parser.add_argument(
        "--sample-session-file",
        type=Path,
        default=None,
        help="Explicit sample session JSON file (defaults to the first gym_*.json in data-dir).",
    )

    args = parser.parse_args()
    session_file = args.sample_session_file
    if session_file is None:
        matches = sorted(args.data_dir.glob("gym_*.json"))
        session_file = matches[0] if matches else args.data_dir / "gym_21_1_2026.json"

    args.db.parent.mkdir(parents=True, exist_ok=True)
    conn = sqlite3.connect(args.db)

    try:
        apply_migrations(conn, args.migrations_dir)
        seed_machines(conn, args.data_dir / "machines.json")
        seed_sample_session(conn, session_file)
        conn.commit()
        print(f"[done] database ready at {args.db}")
    finally:
        conn.close()


if __name__ == "__main__":
    main()
