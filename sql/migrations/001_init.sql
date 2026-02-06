-- Migration: 001_init.sql
-- Creates the baseline schema for the Gym Tracker app (SQLite-first, Postgres-ready layout).

PRAGMA foreign_keys = ON;

BEGIN TRANSACTION;

CREATE TABLE IF NOT EXISTS schema_migrations (
    id INTEGER PRIMARY KEY,
    version TEXT NOT NULL UNIQUE,
    applied_at TEXT NOT NULL DEFAULT (datetime('now'))
);

CREATE TABLE IF NOT EXISTS machines (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE,
    muscle_group TEXT,
    weight_min INTEGER NOT NULL DEFAULT 0,
    weight_max INTEGER NOT NULL DEFAULT 500,
    note TEXT,
    is_active INTEGER NOT NULL DEFAULT 1,
    created_at TEXT NOT NULL DEFAULT (datetime('now')),
    updated_at TEXT,
    CHECK (weight_min <= weight_max)
);

CREATE TABLE IF NOT EXISTS sessions (
    id INTEGER PRIMARY KEY,
    started_at TEXT NOT NULL DEFAULT (datetime('now')),
    ended_at TEXT,
    note TEXT
);

CREATE TABLE IF NOT EXISTS exercises (
    id INTEGER PRIMARY KEY,
    session_id INTEGER NOT NULL,
    machine_id INTEGER, 
    machine_name_snapshot TEXT,
    order_index INTEGER NOT NULL DEFAULT 0,
    comment TEXT,
    FOREIGN KEY (session_id) REFERENCES sessions(id) ON DELETE CASCADE,
    FOREIGN KEY (machine_id) REFERENCES machines(id) ON DELETE SET NULL
);
CREATE INDEX IF NOT EXISTS idx_exercises_session ON exercises(session_id);

CREATE TABLE IF NOT EXISTS exercise_sets (
    id INTEGER PRIMARY KEY,
    exercise_id INTEGER NOT NULL,
    set_number INTEGER NOT NULL,
    reps INTEGER NOT NULL,
    weight_lbs REAL,
    rpe REAL,
    is_warmup INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY (exercise_id) REFERENCES exercises(id) ON DELETE CASCADE,
    CHECK (set_number > 0),
    CHECK (reps >= 0),
    UNIQUE (exercise_id, set_number)
);
CREATE INDEX IF NOT EXISTS idx_sets_exercise ON exercise_sets(exercise_id);

INSERT OR IGNORE INTO schema_migrations (version) VALUES ('001_init');

COMMIT;
