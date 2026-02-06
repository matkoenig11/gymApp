SQL assets live here. Layout:

- `migrations/` — ordered migration files (SQLite-first; keep statements portable for Postgres).
- `seed_sqlite.py` — applies migrations, seeds machines from `data/machines.json`, and drops in one sample session from `data/gym_*.json` when tables are empty.

Quick start (SQLite):

```
python sql/seed_sqlite.py --db gym.sqlite --data-dir data --migrations-dir sql/migrations
```

Notes:
- Re-running is safe: migrations are idempotent and seeding skips if tables already contain rows.
- For Postgres later, mirror the migration files with dialect tweaks (identity columns, timestamptz) and swap the connection in the backend.
