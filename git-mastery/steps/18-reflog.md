---
title: The Reflog
---

# The Disaster Scenario

You ran `git reset --hard HEAD~3`. {{show: disaster-log typewriter 2s linear}} Three days of work. Gone from the log. {{annotate: missing-commits "3 commits missing"}} Your feature is gone. The commit graph shows nothing. The working directory matches what it looked like three days ago.

```code:disaster-log lang=bash
# git log after the bad reset
$ git log --oneline
4c3b2a1 (HEAD -> feature/login) Add base auth form
9b8f4a2 Fix session bug
1d2e3f4 Initial setup

# Where are the three commits you lost?
# Where is "Add validation", "Add tests", "Polish"?
# They're not in git log. They look gone.
---
missing-commits: 1-6
```

{{clear: slide}}

They are not gone. {{show: reflog-hope slide 0.4s ease-out}} Git never deletes commit objects when you reset. {{annotate: objects-safe "objects still exist"}} It just moves the branch pointer. The commit objects — the validation logic, the tests, the polish — are still in `.git/objects/`. Git keeps unreachable objects for at least 90 days before garbage collection.

The reflog is a journal of every HEAD movement. {{focus: objects-safe}} Every commit, reset, merge, rebase, cherry-pick, and switch is recorded with a timestamp. {{annotate: journal-note "every HEAD position recorded"}} The three commits you "lost" have SHA hashes in the reflog. You just need to find them. {{focus: none}}

# What the Reflog Is

`git reflog` shows HEAD's entire movement history. {{show: reflog-output typewriter 2s linear}} The format: SHA, then `HEAD@{N}`, then the action, then a description. {{annotate: head-index "HEAD@{N} = N moves ago"}} `HEAD@{0}` is where you are now. `HEAD@{1}` is where you were before the last action. `HEAD@{7}` is seven HEAD movements ago.

```code:reflog-output lang=bash
$ git reflog
4c3b2a1 HEAD@{0}: reset: moving to HEAD~3
b5c4d3e HEAD@{1}: commit: Polish and cleanup
a1b2c3d HEAD@{2}: commit: Add tests for validation
7e8f9a0 HEAD@{3}: commit: Add client-side validation
4c3b2a1 HEAD@{4}: commit: Add base auth form
9b8f4a2 HEAD@{5}: commit: Fix session bug
1d2e3f4 HEAD@{6}: commit: Initial setup
8d9e0f1 HEAD@{7}: checkout: moving from main to feature/login
---
head-index: 1-8
```

{{clear: slide}}

{{zoom: 1.2x}} {{focus: head-index}} The SHA on each line is the commit that HEAD pointed to AFTER that action. {{annotate: head-index "SHA = state after action"}} `HEAD@{0}` shows `4c3b2a1` — that's where you are now (after the bad reset). `HEAD@{1}` shows `b5c4d3e` — that's where you were before the reset. That's the commit you want. {{zoom: 1x}} {{focus: none}}

Every action type appears in the reflog. {{show: reflog-actions typewriter 1.5s linear}} Not just commits and resets. {{annotate: all-actions "all HEAD movements logged"}} Merges, rebases, cherry-picks, branch switches, amends — all recorded. The description field tells you exactly what happened.

```code:reflog-actions lang=bash
# Examples of different action types in the reflog
$ git reflog
a4b2c1d HEAD@{0}: merge feature/login: Merge made by 'ort'
b5c4d3e HEAD@{1}: checkout: moving from feature/login to main
c6d7e8f HEAD@{2}: rebase -i (finish): returning to feature/login
d9e0f1a HEAD@{3}: rebase -i (squash): Add login form
e2f3a4b HEAD@{4}: cherry-pick: Add emergency fix
f5a6b7c HEAD@{5}: commit (amend): Fix login: handle null session
a7b8c9d HEAD@{6}: commit: Fix login: handle null session
---
all-actions: 1-8
```

{{clear: slide}}

# Recovering with Reflog

Find the hash you want. {{show: recovery-steps typewriter 2s linear}} Look at the reflog output. Find the state BEFORE the bad action. {{annotate: find-hash "find the SHA before the bad action"}} In our disaster: the bad reset was at `HEAD@{0}`, so we want `HEAD@{1}` which is `b5c4d3e` — the commit we were at before the reset.

```code:recovery-steps lang=bash
# Find where you were before the disaster
$ git reflog
4c3b2a1 HEAD@{0}: reset: moving to HEAD~3
b5c4d3e HEAD@{1}: commit: Polish and cleanup   ← this is what we want

# OPTION A: move HEAD back there (destructive — moves current branch)
$ git reset --hard b5c4d3e

# OPTION B: create a recovery branch (safer — doesn't move current branch)
$ git branch recovery b5c4d3e

# Option B is safer: verify the recovery branch before moving HEAD
$ git log recovery --oneline -5
b5c4d3e Polish and cleanup
a1b2c3d Add tests for validation
7e8f9a0 Add client-side validation
4c3b2a1 Add base auth form
9b8f4a2 Fix session bug
---
find-hash: 1-3
```

{{clear: slide}}

Before: after the bad reset. {{show: recovery-before slide 0.4s ease-out}} HEAD is at C4. Commits C5, C6, C7 are unreachable from any branch. {{annotate: unreachable "unreachable, not deleted"}} They exist in the object store but no pointer reaches them.

```data:recovery-before type=graph layout=tree
(c1 "initial") -> (c2 "fix session") -> (c3 "add cache") -> (c4 "add base auth form")
(c4 "add base auth form") -> (c5 "add validation [unreachable]") -> (c6 "add tests [unreachable]") -> (c7 "polish [unreachable]")
^HEAD=c4
^feature=c4
---
unreachable: c5, c6, c7
find-hash: c7
```

{{clear: slide}}

After recovery. {{show: recovery-after slide 0.4s ease-out}} `git branch recovery b5c4d3e` creates a branch pointer to C7. {{annotate: recovered "commits reachable again"}} Now all three "lost" commits are reachable. You can switch to `recovery`, verify the code, and reset your feature branch to point there.

```data:recovery-after type=graph layout=tree
(c1 "initial") -> (c2 "fix session") -> (c3 "add cache") -> (c4 "add base auth form")
(c4 "add base auth form") -> (c5 "add validation") -> (c6 "add tests") -> (c7 "polish")
^HEAD=c4
^feature=c4
^recovery=c7
---
recovered: c5, c6, c7, recovery
```

{{clear: slide}}

```code:full-recovery lang=bash
# Full recovery workflow
# Step 1: find the target SHA in reflog
$ git reflog
4c3b2a1 HEAD@{0}: reset: moving to HEAD~3
b5c4d3e HEAD@{1}: commit: Polish and cleanup

# Step 2: create recovery branch (non-destructive)
$ git branch recovery b5c4d3e

# Step 3: verify all your work is there
$ git log recovery --oneline
b5c4d3e Polish and cleanup
a1b2c3d Add tests for validation
7e8f9a0 Add client-side validation
4c3b2a1 Add base auth form

# Step 4: move your feature branch to the recovery point
$ git switch feature/login
$ git reset --hard b5c4d3e

# Step 5: cleanup the recovery branch
$ git branch -d recovery
---
find-hash: 1-3
recovered: 5-12
```

{{clear: slide}}

# Per-Branch Reflog

The default reflog is HEAD's movements. {{show: branch-reflog typewriter 1.5s linear}} You can also query a specific branch's reflog. {{annotate: branch-reflog-flag "reflog show <branch>"}} This is useful when you want to see where `main` was at a specific time, even if you've switched branches since.

```code:branch-reflog lang=bash
# Reflog for HEAD (all movements)
$ git reflog show HEAD

# Reflog for a specific branch
$ git reflog show main
a3f2d9c main@{0}: merge feature/login: Merge made by 'ort'
7e1a4c9 main@{1}: commit: Fix auth timeout
4c3b2a1 main@{2}: pull: Fast-forward

# Reflog for a remote-tracking branch
$ git reflog show origin/main

# Show timestamps instead of relative "N moves ago"
$ git reflog --date=iso
a3f2d9c HEAD@{2026-03-01 10:23:44 +0000}: merge feature/login: Merge made by 'ort'
7e1a4c9 HEAD@{2026-02-28 15:12:30 +0000}: commit: Fix auth timeout
---
branch-reflog-flag: 3-6
```

{{clear: slide}}

Reflog has a lifespan. {{show: reflog-expiry typewriter 1.5s linear}} It is not infinite storage. {{annotate: expiry-note "90 days default expiry"}} Git's gc runs periodically and prunes reflog entries older than 90 days (configurable). After 90 days, the unreachable commits can be garbage collected.

```code:reflog-expiry lang=bash
# Show how reflog expiry is configured
$ git config gc.reflogExpire
90.days

# Manually expire old reflog entries (use carefully)
$ git reflog expire --expire=90.days --all

# DANGEROUS: immediately prune unreachable objects
# Don't run this routinely — it destroys your safety net
$ git gc --prune=now

# Safe gc: only prunes objects older than 2 weeks
$ git gc
---
expiry-note: 1-3
```

{{clear: slide}}

# Reflog for Understanding What Happened

Reflog is not just for recovery. {{show: debug-reflog typewriter 1.5s linear}} It's a diagnostic tool. {{annotate: debug-use "reflog = git's flight recorder"}} After a complex interactive rebase with squash, fixup, and reorder, the reflog shows every intermediate state. You can understand exactly what happened in what order.

```code:debug-reflog lang=bash
# After a complex rebase: what happened?
$ git reflog
b5c4d3e HEAD@{0}: rebase -i (finish): returning to refs/heads/feature/login
a1b2c3d HEAD@{1}: rebase -i (fixup): Add tests for validation
7e8f9a0 HEAD@{2}: rebase -i (reword): Add client-side validation
4c3b2a1 HEAD@{3}: rebase -i (start): checkout HEAD~5

# View reflog in log format (more detail)
$ git log -g --oneline -10
b5c4d3e HEAD@{0}: rebase -i (finish)
a1b2c3d HEAD@{1}: rebase -i (fixup)
7e8f9a0 HEAD@{2}: rebase -i (reword)

# Show with ISO timestamps
$ git reflog --date=iso -10
b5c4d3e HEAD@{2026-03-01 11:45:22 +0000}: rebase -i (finish)
---
debug-use: 1-4
```

{{clear: slide}}

Before you panic about anything lost: check reflog first. {{show: reflog-mindset slide 0.4s ease-out}} This is the rule. {{annotate: panic-rule "reflog first, panic never"}} Bad reset? Reflog. Accidentally deleted branch? Reflog. Rebase went wrong? Reflog. Lost a commit after `git commit --amend`? Reflog. The original commit is still there under `HEAD@{1}`.

```code:reflog-mindset lang=bash
# The three-second check before any panic
$ git reflog | head -20

# Common recovery patterns:
# Lost after git reset --hard:
$ git reset --hard HEAD@{1}

# Lost after accidental branch delete:
$ git branch restored-feature HEAD@{3}

# Lost after bad rebase:
$ git reset --hard ORIG_HEAD   # git sets ORIG_HEAD before rebase/merge

# Verify ORIG_HEAD exists
$ cat .git/ORIG_HEAD
b5c4d3e1a2b3c4d5e6f7a8b9c0d1e2f3a4b5c6d7
---
panic-rule: 1-2
```
