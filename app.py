#!/usr/bin/env python3
"""Simple gym tracker prototype backed by SQLite.

Commands:
    python app.py init --seed          # create tables, optionally load sample data
    python app.py machines             # list machines with weight ranges
    python app.py sessions             # list logged sessions
    python app.py session <id>         # show sets for a session
"""

from __future__ import annotations

import argparse
import sqlite3
from datetime import datetime
from pathlib import Path
from typing import Dict, Iterable, List, Sequence


DB_DEFAULT = Path(__file__).with_name("gym.sqlite")

SAMPLE_MACHINES = [
    {
        "name": "Leg Press",
        "muscle_group": "B6",
        "weight_min": 0,
        "weight_max": 500,
        "note": "Plate-loaded leg press.",
    },
    {
        "name": "Horizontal Pull",
        "muscle_group": "B4",
        "weight_min": 0,
        "weight_max": 500,
        "note": "Cable or machine row.",
    },
    {
        "name": "Vertical Push (Not Chest Press)",
        "muscle_group": "B5",
        "weight_min": 0,
        "weight_max": 500,
        "note": "Overhead-style push machine.",
    },
    {
        "name": "Vertical Pull (Side)",
        "muscle_group": "B4",
        "weight_min": 0,
        "weight_max": 500,
        "note": "Lat pull-down variation.",
    },
]

SAMPLE_SESSION = {
    "started_at": datetime(2026, 1, 21, 18, 51).isoformat(timespec="minutes"),
    "note": "Imported from gym_21_1_2026.json sample log.",
    "exercises": [
        {
            "machine": "Leg Press",
            "sets": [
                {"reps": 10, "weight_lbs": 280},
                {"reps": 10, "weight_lbs": 280},
                {"reps": 10, "weight_lbs": 280},
                {"reps": 15, "weight_lbs": 280},
            ],
        },
        {
            "machine": "Horizontal Pull",
            "sets": [
                {"reps": 8, "weight_lbs": 95},
                {"reps": 8, "weight_lbs": 95},
                {"reps": 8, "weight_lbs": 95},
                {"reps": 12, "weight_lbs": 95},
            ],
        },
        {
            "machine": "Vertical Push (Not Chest Press)",
            "sets": [
                {"reps": 10, "weight_lbs": 55},
                {"reps": 10, "weight_lbs": 55},
                {"reps": 10, "weight_lbs": 55},
                {"reps": 12, "weight_lbs": 55},
            ],
        },
        {
            "machine": "Vertical Pull (Side)",
            "sets": [
                {"reps": 8, "weight_lbs": 45},
                {"reps": 8, "weight_lbs": 40},
                {"reps": 8, "weight_lbs": 40},
                {"reps": 8, "weight_lbs": 40},
            ],
        },
    ],
}


def connect(db_path: Path) -> sqlite3.Connection:
    conn = sqlite3.connect(db_path)
    conn.row_factory = sqlite3.Row
    conn.execute("PRAGMA foreign_keys = ON;")
    return conn


def create_schema(conn: sqlite3.Connection) -> None:
    conn.executescript(
        """
        CREATE TABLE IF NOT EXISTS machines (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            muscle_group TEXT,
            weight_min INTEGER NOT NULL DEFAULT 0,
            weight_max INTEGER NOT NULL DEFAULT 500,
            note TEXT,
            CHECK (weight_min <= weight_max)
        );

        CREATE TABLE IF NOT EXISTS sessions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            started_at TEXT NOT NULL DEFAULT (datetime('now')),
            note TEXT
        );

        CREATE TABLE IF NOT EXISTS exercises (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            session_id INTEGER NOT NULL,
            machine_id INTEGER NOT NULL,
            comment TEXT,
            FOREIGN KEY (session_id) REFERENCES sessions(id) ON DELETE CASCADE,
            FOREIGN KEY (machine_id) REFERENCES machines(id) ON DELETE CASCADE
        );

        CREATE TABLE IF NOT EXISTS exercise_sets (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            exercise_id INTEGER NOT NULL,
            set_number INTEGER NOT NULL,
            reps INTEGER NOT NULL,
            weight_lbs REAL,
            FOREIGN KEY (exercise_id) REFERENCES exercises(id) ON DELETE CASCADE,
            UNIQUE (exercise_id, set_number)
        );
        """
    )
    conn.commit()


def seed_sample_data(conn: sqlite3.Connection) -> bool:
    cur = conn.execute("SELECT COUNT(*) AS count FROM sessions;")
    if cur.fetchone()["count"]:
        return False

    machine_ids: Dict[str, int] = {}
    for machine in SAMPLE_MACHINES:
        conn.execute(
            """
            INSERT INTO machines (name, muscle_group, weight_min, weight_max, note)
            VALUES (:name, :muscle_group, :weight_min, :weight_max, :note)
            ON CONFLICT(name) DO NOTHING;
            """,
            machine,
        )
        row = conn.execute(
            "SELECT id FROM machines WHERE name = ?;", (machine["name"],)
        ).fetchone()
        if row:
            machine_ids[machine["name"]] = int(row["id"])

    session_row = conn.execute(
        "INSERT INTO sessions (started_at, note) VALUES (?, ?);",
        (SAMPLE_SESSION["started_at"], SAMPLE_SESSION["note"]),
    )
    session_id = int(session_row.lastrowid)

    for exercise in SAMPLE_SESSION["exercises"]:
        machine_name = exercise["machine"]
        machine_id = machine_ids.get(machine_name)
        if machine_id is None:
            raise RuntimeError(f"Machine not found while seeding: {machine_name}")

        exercise_row = conn.execute(
            "INSERT INTO exercises (session_id, machine_id, comment) VALUES (?, ?, ?);",
            (session_id, machine_id, exercise.get("comment")),
        )
        exercise_id = int(exercise_row.lastrowid)

        for idx, set_entry in enumerate(exercise["sets"], start=1):
            conn.execute(
                """
                INSERT INTO exercise_sets (exercise_id, set_number, reps, weight_lbs)
                VALUES (?, ?, ?, ?);
                """,
                (exercise_id, idx, set_entry["reps"], set_entry["weight_lbs"]),
            )

    conn.commit()
    return True


def list_machines(conn: sqlite3.Connection) -> List[sqlite3.Row]:
    rows = conn.execute(
        """
        SELECT
            m.id,
            m.name,
            m.muscle_group,
            m.weight_min,
            m.weight_max,
            COUNT(es.id) AS set_count
        FROM machines m
        LEFT JOIN exercises e ON e.machine_id = m.id
        LEFT JOIN exercise_sets es ON es.exercise_id = e.id
        GROUP BY m.id, m.name, m.muscle_group, m.weight_min, m.weight_max
        ORDER BY m.name COLLATE NOCASE;
        """
    ).fetchall()
    return list(rows)


def list_sessions(conn: sqlite3.Connection) -> List[sqlite3.Row]:
    rows = conn.execute(
        """
        SELECT
            s.id,
            s.started_at,
            s.note,
            COUNT(DISTINCT e.id) AS exercise_count,
            COUNT(es.id) AS set_count,
            COALESCE(SUM(es.reps), 0) AS total_reps
        FROM sessions s
        LEFT JOIN exercises e ON e.session_id = s.id
        LEFT JOIN exercise_sets es ON es.exercise_id = e.id
        GROUP BY s.id
        ORDER BY s.started_at DESC;
        """
    ).fetchall()
    return list(rows)


def fetch_session_detail(conn: sqlite3.Connection, session_id: int) -> Dict[str, object]:
    meta = conn.execute(
        "SELECT id, started_at, note FROM sessions WHERE id = ?;", (session_id,)
    ).fetchone()
    if meta is None:
        raise ValueError(f"Session {session_id} not found.")

    sets = conn.execute(
        """
        SELECT
            m.name AS machine_name,
            m.muscle_group,
            es.set_number,
            es.reps,
            es.weight_lbs
        FROM exercise_sets es
        JOIN exercises ex ON es.exercise_id = ex.id
        JOIN machines m ON ex.machine_id = m.id
        WHERE ex.session_id = ?
        ORDER BY m.name COLLATE NOCASE, es.set_number;
        """,
        (session_id,),
    ).fetchall()

    return {"meta": meta, "sets": list(sets)}


def print_machines(rows: Sequence[sqlite3.Row]) -> None:
    if not rows:
        print("No machines found. Run `python app.py init --seed` to load sample data.")
        return

    print("Machines")
    print("--------")
    for row in rows:
        muscle = row["muscle_group"] or "unspecified"
        print(
            f"[{row['id']}] {row['name']} | muscle {muscle} | "
            f"range {row['weight_min']}..{row['weight_max']} lb | "
            f"sets logged: {row['set_count']}"
        )


def print_sessions(rows: Sequence[sqlite3.Row]) -> None:
    if not rows:
        print("No sessions found.")
        return

    print("Sessions")
    print("--------")
    for row in rows:
        print(
            f"[{row['id']}] {row['started_at']} | "
            f"exercises: {row['exercise_count']} | "
            f"sets: {row['set_count']} | total reps: {row['total_reps']}"
        )
        if row["note"]:
            print(f"    note: {row['note']}")


def print_session_detail(detail: Dict[str, object]) -> None:
    meta = detail["meta"]
    rows: Iterable[sqlite3.Row] = detail["sets"]

    print(f"Session {meta['id']} @ {meta['started_at']}")
    if meta["note"]:
        print(f"Note: {meta['note']}")
    print("Machine / sets")
    print("--------------")

    current_machine = None
    for row in rows:
        machine_name = row["machine_name"]
        if machine_name != current_machine:
            current_machine = machine_name
            muscle = row["muscle_group"] or "unspecified"
            print(f"{machine_name} ({muscle})")
        print(f"  set {row['set_number']}: {row['reps']} reps @ {row['weight_lbs']} lb")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Gym tracker CLI (SQLite).")
    parser.add_argument(
        "--db",
        type=Path,
        default=DB_DEFAULT,
        help=f"Path to the SQLite DB file (default: {DB_DEFAULT})",
    )

    sub = parser.add_subparsers(dest="command", required=True)

    init_cmd = sub.add_parser("init", help="Create tables (and optionally seed data).")
    init_cmd.add_argument(
        "--seed",
        action="store_true",
        help="Insert a sample session and machines when no sessions exist.",
    )
    init_cmd.set_defaults(func=handle_init)

    machines_cmd = sub.add_parser("machines", help="List machines and weight ranges.")
    machines_cmd.set_defaults(func=handle_machines)

    sessions_cmd = sub.add_parser("sessions", help="List sessions.")
    sessions_cmd.set_defaults(func=handle_sessions)

    session_cmd = sub.add_parser(
        "session", help="Show details (sets) for a given session."
    )
    session_cmd.add_argument("session_id", type=int, help="Session ID to show.")
    session_cmd.set_defaults(func=handle_session_detail)

    return parser


def handle_init(args: argparse.Namespace) -> None:
    with connect(args.db) as conn:
        create_schema(conn)
        seeded = False
        if args.seed:
            seeded = seed_sample_data(conn)
    if args.seed:
        msg = "Sample data loaded." if seeded else "Sample data skipped (sessions exist)."
        print(f"Schema created/verified. {msg}")
    else:
        print("Schema created/verified.")


def handle_machines(args: argparse.Namespace) -> None:
    with connect(args.db) as conn:
        rows = list_machines(conn)
    print_machines(rows)


def handle_sessions(args: argparse.Namespace) -> None:
    with connect(args.db) as conn:
        rows = list_sessions(conn)
    print_sessions(rows)


def handle_session_detail(args: argparse.Namespace) -> None:
    with connect(args.db) as conn:
        detail = fetch_session_detail(conn, args.session_id)
    print_session_detail(detail)


def main(argv: Sequence[str] | None = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    args.func(args)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
