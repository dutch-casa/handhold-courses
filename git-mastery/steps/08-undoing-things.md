---
title: Undoing Things
---

# The Four Undo Tools

Four tools. Different situations. Wrong tool = broken history or lost work. {{show: undo-matrix slide 0.4s ease-out}} The 2x2 matrix: staged or unstaged changes, local or already-pushed commits. {{annotate: matrix-label "pick the right quadrant"}} Find your situation, use that tool. Don't guess.

Top-left: unstaged working directory change. `git restore`. {{focus: restore-quadrant}} Discards the working directory change, doesn't touch the index or history. {{annotate: restore-quadrant "restore = working dir"}} Safe, local, reversible only if you staged first.

Top-right: staged change you want to unstage. `git restore --staged`. {{focus: staged-quadrant}} Moves the change back to unstaged without touching the working directory. {{annotate: staged-quadrant "unstage safely"}} Your edits are still there — just not going into the next commit.

Bottom-left: local commit you want to undo. `git reset`. {{focus: reset-quadrant}} Moves the branch pointer back. Comes in three modes, which we'll cover. {{annotate: reset-quadrant "rewrite local history"}} Only safe for commits that haven't been pushed to shared remotes.

Bottom-right: pushed commit that needs undoing. `git revert`. {{focus: revert-quadrant}} Creates a new commit that inverts the target. {{annotate: revert-quadrant "safe for shared history"}} History is preserved, the bad change is cancelled, nobody's clone is broken.

```diagram:undo-matrix
restore-quadrant [git restore: unstaged changes]
staged-quadrant [git restore --staged: staged changes]
reset-quadrant [git reset: local commits]
revert-quadrant [git revert: pushed commits]
{UnstagedRow: restore-quadrant, staged-quadrant}
{CommitRow: reset-quadrant, revert-quadrant}
{LocalCol: restore-quadrant, reset-quadrant}
{SharedCol: staged-quadrant, revert-quadrant}
---
matrix-label: restore-quadrant
restore-quadrant: restore-quadrant
staged-quadrant: staged-quadrant
reset-quadrant: reset-quadrant
revert-quadrant: revert-quadrant
```

# git restore — Discard Working Directory Changes

{{clear: slide}}

`git restore` is the safe discard. {{show: restore-demo typewriter 1.5s linear}} It throws away uncommitted changes in your working directory, restoring the file to its state in the index. {{annotate: restore-command "working dir only"}} No history is modified. No commits are touched. It's the most local operation git has.

The caveat: this is the one git operation that can lose work permanently. {{focus: lost-work-note}} There's no undo. If you haven't committed or stashed the changes, `git restore` deletes them. {{annotate: lost-work-note "no recovery without stash"}} Git will warn you about this in some contexts but not all. Know what you're discarding before you run it.

To unstage a file without losing the working directory changes, use `--staged`. {{focus: staged-flag}} `git restore --staged src/auth.ts` moves the changes from staged back to unstaged. {{annotate: staged-flag "--staged = index only"}} The file in your working directory is unchanged. You've just removed it from the next commit.

```code:restore-demo lang=bash
# File has uncommitted changes
$ git status
On branch main
Changes not staged for commit:
        modified:   src/auth.ts

# Discard working directory changes
$ git restore src/auth.ts

# File is now clean
$ git status
On branch main
nothing to commit, working tree clean

# Unstage without losing changes
$ git restore --staged src/auth.ts
---
restore-command: 7-8
lost-work-note: 7-8
staged-flag: 15-16
```

# git reset — Three Modes

{{clear: slide}}

`git reset` moves the branch pointer. {{show: reset-modes slide 0.4s ease-out}} All three modes move HEAD and the current branch to the specified commit. The difference is what happens to the index and working directory after the move. {{annotate: head-move "HEAD moves in all three"}} Wrong mode = lost work.

`--soft` moves HEAD only. {{focus: soft-row}} The index and working directory are untouched. {{annotate: soft-row "--soft: HEAD only"}} Your changes are still staged, ready for a new commit. Use this to "uncommit" and recommit with a different message or different staging.

`--mixed` is the default. {{focus: mixed-row}} HEAD moves and the index is reset to match the new HEAD. {{annotate: mixed-row "--mixed: HEAD + index"}} Working directory is untouched. Your changes are back to unstaged. Run `git status` and you'll see modified files.

`--hard` is destructive. {{focus: hard-row}} HEAD moves, index resets, working directory resets. {{annotate: hard-row "--hard: everything"}} Changes are discarded. No stash, no undo. Use this when you genuinely want to erase work and return to a known-good state. Be certain.

```code:reset-demo lang=bash
# Soft reset — uncommit but keep staged
$ git reset --soft HEAD~1

# Mixed reset — uncommit and unstage (default)
$ git reset HEAD~1
$ git reset --mixed HEAD~1

# Hard reset — erase everything back to that commit
$ git reset --hard HEAD~1

# Reset to a specific commit
$ git reset --hard 7fd1a60b

# Show what changed after reset
$ git status
$ git log --oneline -5
---
soft-row: 1-2
mixed-row: 4-6
hard-row: 8-10
```

```diagram:reset-modes
soft-row [--soft: moves HEAD, keeps index + working dir]
mixed-row [--mixed: moves HEAD + index, keeps working dir]
hard-row [--hard: moves HEAD + index + working dir]
{ModeList: soft-row, mixed-row, hard-row}
---
soft-row: soft-row
mixed-row: mixed-row
hard-row: hard-row
head-move: soft-row
```

# git revert — The Safe Undo for Shared History

{{clear: slide}}

`git revert` doesn't erase. {{show: revert-graph slide 0.4s ease-out}} It adds. Given a commit to undo, revert computes the inverse of that commit's changes and creates a new commit applying the inverse. {{annotate: revert-commit "new commit at end"}} The bad commit stays in history. The new revert commit cancels its effects.

This is the only safe undo for shared history. {{focus: revert-commit}} Teammates who cloned before the revert just need to pull. {{clear: slide}} {{show: revert-demo slide 0.3s ease-out}} {{annotate: revert-commit "history intact"}} No divergence, no force-push, no conflict. The history tells the complete story: we shipped it, we broke it, we fixed it.

`git revert` with `--no-commit` stages the inverse without committing. {{focus: no-commit-line}} Useful when you want to review the inverted changes or combine multiple reverts into one commit. {{annotate: no-commit-line "--no-commit for review"}} After reviewing, `git commit` to complete the revert.

```data:revert-graph type=graph layout=tree
A -> B: parent
B -> C: parent
C -> bad: parent
bad -> revert-commit: parent
---
revert-commit: revert-commit
```

```code:revert-demo lang=bash
# Revert the last commit
$ git revert HEAD

# Revert a specific commit by hash
$ git revert 7fd1a60b

# Revert without auto-committing
$ git revert --no-commit 7fd1a60b
---
no-commit-line: 7-8
```

# When to Use Which

{{clear: slide}}

The decision is two questions. {{show: undo-decision slide 0.4s ease-out}} First: has the commit been pushed to a shared remote? {{annotate: pushed-check "first question"}} If yes, use revert. Full stop. Nothing else is safe.

If the commit is local only, second question: what state do you want the changes in? {{focus: local-branch}} Staged for recommitting: `--soft`. Back to unstaged, kept in working directory: `--mixed`. Gone entirely: `--hard`. {{annotate: local-branch "local = reset options"}} If you haven't committed yet: `git restore` for working directory, `git restore --staged` for the index.

The principle underneath all of this: never rewrite history that others have seen. {{focus: none}} Every rewrite that touches pushed commits forces teammates to reconcile diverged histories. {{annotate: revert-branch "safe for everyone"}} The minutes you spend avoiding a proper revert cost hours for everyone else.

```diagram:undo-decision
pushed-check [Is it pushed?]
revert-branch [git revert]
local-branch [Is it committed?]
soft-branch [git reset --soft: want changes staged]
mixed-branch [git reset --mixed: want changes unstaged]
hard-branch [git reset --hard: discard everything]
restore-branch [git restore: not committed]
pushed-check --> revert-branch: yes
pushed-check --> local-branch: no
local-branch --> soft-branch: yes, keep staged
local-branch --> mixed-branch: yes, keep unstaged
local-branch --> hard-branch: yes, discard
local-branch --> restore-branch: no
---
pushed-check: pushed-check
revert-branch: revert-branch
local-branch: local-branch
```
