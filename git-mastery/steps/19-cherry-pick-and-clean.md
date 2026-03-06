---
title: Cherry-Pick and Clean
---

# git cherry-pick — Surgical Commit Porting

The situation is specific. {{show: cherry-pick-scenario slide 0.4s ease-out}} A critical bug fix landed on `develop`. {{annotate: fix-on-develop "fix is on develop"}} Your `release/v1.2.x` branch needs that fix now. But `develop` also has half-finished features, breaking changes, and three commits that would destabilize the release. You cannot merge `develop`. You need exactly one commit.

```data:cherry-pick-scenario type=graph layout=tree
(r1 "v1.2.0") -> (r2 "patch prep") -> (r3 "docs update")
(d1 "v1.2.0") -> (d2 "new feature WIP") -> (d3 "CRITICAL FIX") -> (d4 "more WIP") -> (d5 "breaking change")
^release=r3
^develop=d5
^HEAD=r3
---
fix-on-develop: d3
```

{{clear: slide}}

Cherry-pick copies one commit. {{show: cherry-pick-after slide 0.4s ease-out}} It takes the diff of that commit and applies it to your current branch. {{annotate: new-sha-note "copy = new SHA, new parent"}} The original commit on `develop` is untouched. A new commit appears on `release` with the same changes but a different SHA and a different parent commit. Same diff. New identity.

```data:cherry-pick-after type=graph layout=tree
(r1 "v1.2.0") -> (r2 "patch prep") -> (r3 "docs update") -> (r4 "CRITICAL FIX [copy]")
(d1 "v1.2.0") -> (d2 "new feature WIP") -> (d3 "CRITICAL FIX") -> (d4 "more WIP") -> (d5 "breaking change")
^release=r4
^develop=d5
^HEAD=r4
---
new-sha-note: r4
fix-on-develop: d3
```

{{clear: slide}}

{{flow: cherry-path}} The path of the fix: from `develop` to `release` without pulling the surrounding noise. {{annotate: surgical "surgical — not merge"}} This is the core use case. Not `merge`. Not `rebase`. Cherry-pick. {{flow: none}}

# git cherry-pick Commands

One commit. One command. {{show: cherry-pick-commands typewriter 2s linear}} `git cherry-pick <hash>` applies the diff of that commit to your current branch and creates a new commit. {{annotate: basic-cp "basic form: one hash"}} Git uses the original commit's message by default. You can edit it with `--edit`.

```code:cherry-pick-commands lang=bash
# Apply a single commit to current branch
$ git cherry-pick 7e8f9a0

# Apply a range: A exclusive to B inclusive
$ git cherry-pick A..B
$ git cherry-pick a3f2d9c..b5c4d3e

# Apply a range: A inclusive to B inclusive
$ git cherry-pick A^..B
$ git cherry-pick a3f2d9c^..b5c4d3e

# Apply without committing (stage the changes, you commit)
$ git cherry-pick -n 7e8f9a0

# Apply and edit the commit message
$ git cherry-pick --edit 7e8f9a0

# Cherry-pick conflict: resolve, add, then continue
$ git add src/auth.ts
$ git cherry-pick --continue

# Give up on the cherry-pick
$ git cherry-pick --abort
---
basic-cp: 1-2
```

{{clear: slide}}

The `-n` flag is useful when you want to cherry-pick multiple commits but commit them as one. {{zoom: 1.2x}} {{annotate: no-commit-flag "-n = stage without committing"}} `git cherry-pick -n A..B` stages all the changes from those commits without creating any commits. Then you write one commit message covering all of them. {{zoom: 1x}} Same result as squash merge but from a different branch.

```code:cherry-pick-n-example lang=bash
# Stage several commits from develop without committing
$ git cherry-pick -n abc1234 def5678 ghi9012

# Review what was staged
$ git diff --cached

# Commit them together with one message
$ git commit -m "Port auth fixes from develop (3 commits)"
---
no-commit-flag: 1-2
```

{{clear: slide}}

# git clean — Removing Untracked Files

`git restore` and `git reset` don't touch untracked files. {{show: clean-explainer slide 0.4s ease-out}} This is intentional. {{annotate: untracked-blind "restore/reset skip untracked"}} An untracked file is one git has never been told about. It's outside git's model. Those commands operate on the index and tracked files only.

```code:clean-explainer lang=bash
# The problem: untracked files persist after reset
$ git status
On branch feature/login
Changes not staged for commit:
        modified:   src/auth.ts

Untracked files:
        build/
        src/temp-debug.ts
        src/test-script.js

$ git restore .       # restores auth.ts — but untracked files stay
$ git status
On branch feature/login
Untracked files:
        build/
        src/temp-debug.ts
        src/test-script.js
---
untracked-blind: 1-18
```

{{clear: slide}}

Always dry-run first. {{show: clean-dry-run typewriter 2s linear}} `git clean -n` shows you exactly what it would delete. {{annotate: dry-run-first "always -n before -f"}} Read the list. Verify nothing important is there. Then run the actual clean. Skipping the dry run is how you lose work that wasn't tracked.

```code:clean-dry-run lang=bash
# DRY RUN FIRST — see what would be deleted
$ git clean -n
Would remove build/output.js
Would remove src/temp-debug.ts
Would remove src/test-script.js

# Delete untracked files (not directories)
$ git clean -f

# Delete untracked files AND untracked directories
$ git clean -fd

# Delete ONLY gitignored files (clear build artifacts)
$ git clean -fX

# Delete everything: untracked + gitignored
$ git clean -fdx

# Confirm after:
$ git status
On branch feature/login
nothing to commit, working tree clean
---
dry-run-first: 1-4
```

{{clear: slide}}

`-fX` is the most useful form for build artifact cleanup. {{zoom: 1.2x}} {{annotate: x-flag "-fX = gitignored files only"}} It deletes files that match `.gitignore` patterns: build outputs, `node_modules`, compiled artifacts, cache directories. It leaves your actual untracked source files alone. {{zoom: 1x}} Run it before benchmarks or CI-style local builds to get a clean slate.

# .gitignore

`.gitignore` tells git which files to pretend don't exist. {{show: gitignore-diagram slide 0.4s ease-out}} `git status` won't show them. `git add .` won't add them. `git clean -f` won't touch them (unless you use `-x`). {{annotate: ignore-invisible "ignored = invisible to git"}} They're in your working directory but outside git's model.

```code:gitignore-file lang=bash
# .gitignore — annotated real example

# Build output directories
dist/
build/
out/

# Dependencies (massive, reproducible)
node_modules/
.pnp
.pnp.js

# Environment files (secrets — NEVER commit)
.env
.env.local
.env.*.local

# System files
.DS_Store
Thumbs.db

# Editor directories
.vscode/
.idea/
*.swp

# Log files
*.log
npm-debug.log*

# Coverage reports
coverage/
.nyc_output/
---
ignore-invisible: 1-25
```

{{clear: slide}}

{{zoom: 1.2x}} {{focus: ignore-invisible}} Pattern syntax: a trailing `/` matches only directories. A leading `!` negates a pattern (include what would otherwise be ignored). `*` matches anything in a single path component. `**` matches across path separators. {{annotate: pattern-syntax "glob patterns"}} {{zoom: 1x}} {{focus: none}}

Debug ignored files with `git check-ignore`. {{show: check-ignore typewriter 1.5s linear}} When you add a file and git ignores it and you don't know which rule is doing it, this command tells you. {{annotate: check-ignore-cmd "check-ignore -v = which rule"}}

```code:check-ignore lang=bash
# Which .gitignore rule is ignoring this file?
$ git check-ignore -v src/dist/main.js
.gitignore:4:dist/  src/dist/main.js

# Check if a specific file would be ignored
$ git check-ignore -v .env.development
.gitignore:11:.env.*  .env.development

# Check multiple files
$ git check-ignore -v build/ .DS_Store node_modules/
.gitignore:2:build/       build/
.gitignore:14:.DS_Store   .DS_Store
.gitignore:7:node_modules/ node_modules/
---
check-ignore-cmd: 1-3
pattern-syntax: 1-9
```

{{clear: slide}}

`.gitignore` applies recursively. {{show: gitignore-scope typewriter 1.5s linear}} A root `.gitignore` applies to the whole repo. {{annotate: recursive-ignore "applies from placement down"}} You can also place `.gitignore` files in subdirectories — they apply to that directory and its descendants only. Files already tracked are NOT retroactively ignored. Adding a tracked file to `.gitignore` does nothing until you stop tracking it.

```code:gitignore-scope lang=bash
# Root .gitignore: applies to whole repo
/repo/.gitignore

# Subdirectory .gitignore: applies to that dir only
/repo/src/.gitignore

# Already-tracked file: .gitignore has NO EFFECT
$ git status
        modified: src/secrets.json   ← already tracked

# Adding it to .gitignore after the fact:
$ echo "src/secrets.json" >> .gitignore
$ git status
        modified: src/secrets.json   ← STILL shows! .gitignore doesn't help
---
recursive-ignore: 1-4
```

{{clear: slide}}

# Stopping Tracking a File That's Already Tracked

You committed `.env`. {{show: stop-tracking slide 0.4s ease-out}} It's in git's history. Adding it to `.gitignore` does not help — git already knows about it. {{annotate: cached-remove "git rm --cached = stop tracking"}} You need two steps: stop tracking it, then ignore it.

```code:stop-tracking lang=bash
# WRONG: just adding to .gitignore does nothing for tracked files
$ echo ".env" >> .gitignore
$ git status
        modified: .env   ← still tracked

# CORRECT: stop tracking without deleting the local file
$ git rm --cached .env
rm '.env'

# The file is still on disk
$ ls -la .env
-rw-r--r--  .env

# Now add to .gitignore
$ echo ".env" >> .gitignore

# Commit both changes
$ git add .gitignore
$ git commit -m "Stop tracking .env and ignore it"

# From now on: .env is not tracked, not staged, not shown
$ git status
On branch main
nothing to commit, working tree clean
---
cached-remove: 8-9
```

{{clear: slide}}

The file is still in the old commits. {{show: history-warning slide 0.4s ease-out}} `git rm --cached` stops tracking going forward. {{annotate: old-commits-have-it "old history still has it"}} Anyone who clones and checks out an old commit will see the file. If it contained secrets, rotate them. A file in git history is never truly private unless you rewrite the entire history with `git filter-repo` (a separate tool). The moment you pushed it, assume it's compromised.

```code:history-warning lang=bash
# The .env is still accessible in old commits
$ git show HEAD~1:.env
DATABASE_URL=postgres://user:password@host/db
SECRET_KEY=super-secret-value-exposed

# If secrets were exposed: rotate them immediately
# Do not rely on git history rewriting to secure credentials

# If you must rewrite history (no push yet):
# git filter-repo --path .env --invert-paths
# (requires installing git-filter-repo separately)

# If already pushed: assume compromised, rotate all secrets
---
old-commits-have-it: 1-3
```
