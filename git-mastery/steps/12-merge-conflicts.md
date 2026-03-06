---
title: Merge Conflicts
---

# How Conflicts Happen

Two developers. One file. {{show: conflict-origin slide 0.4s ease-out}} Dev A is on `feature/auth`. Dev B is on `feature/dashboard`. Both branch from the same commit. Both open `src/config.ts`. Both change line 12. {{annotate: same-line "same line, different edits"}} Neither knows about the other's change.

```diagram:conflict-origin
base-commit [base commit]
dev-a [dev A: feature/auth]
dev-b [dev B: feature/dashboard]
file-a [config.ts line 12: authTimeout = 30]
file-b [config.ts line 12: authTimeout = 60]
base-commit --> dev-a: branch
base-commit --> dev-b: branch
dev-a --> file-a: edits line 12
dev-b --> file-b: edits line 12
{Conflict: file-a, file-b}
---
same-line: file-a, file-b
```

{{clear: slide}}

Git cannot decide which value is correct. {{show: conflict-diagram slide 0.4s ease-out}} 30 seconds? 60 seconds? That's a product decision, not a text-diff problem. {{annotate: no-autosolve "git won't guess"}} Git is a data structure tool. It faithfully records that both changed the same line. The resolution is yours.

You try to merge. {{focus: no-autosolve}} Git applies all the changes it can automatically. {{annotate: blocked-state "merge paused"}} When it hits a line where both sides changed the same content differently, it stops. It writes conflict markers into the file and tells you to resolve it before continuing. The merge is in-progress and blocked until you act.

# Reading the Conflict Markers

Git rewrites the conflicted file. {{show: conflict-file typewriter 2s linear}} It inserts three markers that you must understand cold. {{annotate: head-marker "your current branch"}} The file is no longer syntactically valid code — you must edit it before anything compiles.

```code:conflict-file lang=bash
# src/config.ts — after a conflict
export const config = {
  appName: "Handhold",
  version: "1.2.0",
<<<<<<< HEAD
  authTimeout: 30,
  sessionStore: "redis",
=======
  authTimeout: 60,
  sessionStore: "memory",
>>>>>>> feature/dashboard
  logLevel: "warn",
};
---
head-marker: 5-7
divider-region: 8
incoming-region: 9-11
```

{{clear: slide}}

The `<<<<<<< HEAD` line opens the conflict block. {{show: marker-detail slide 0.4s ease-out}} {{annotate: head-marker "your side starts here"}} Everything between this marker and `=======` is what YOUR current branch (HEAD) has. This is your work.

The `=======` line is the divider. {{focus: divider-region}} Everything between `=======` and `>>>>>>>>` is the incoming change. {{annotate: divider-region "divides the two sides"}} The text after `>>>>>>>` is the branch name (or commit SHA) that you were merging in. This is the incoming work.

The markers are not part of the code. {{focus: incoming-region}} You delete all three of them. {{annotate: incoming-region "incoming branch side"}} You keep the version you want — yours, theirs, or a blend. Then save the file. The conflict markers must be completely gone before you commit. {{focus: none}}

```code:marker-detail lang=bash
# Full anatomy of a conflict block
<<<<<<< HEAD                          # YOUR side begins
  authTimeout: 30,                    # your value
  sessionStore: "redis",              # your value
=======                               # DIVIDER — do not keep this
  authTimeout: 60,                    # incoming value
  sessionStore: "memory",             # incoming value
>>>>>>> feature/dashboard             # INCOMING side ends

# After resolution (you chose a blend):
  authTimeout: 60,                    # took their timeout
  sessionStore: "redis",              # kept your store
---
head-marker: 1-4
divider-region: 5
incoming-region: 6-8
```

{{clear: slide}}

# Resolving by Hand

Open the file. {{show: resolve-steps typewriter 1.5s linear}} Delete everything git added: the `<<<<<<<`, `=======`, and `>>>>>>>` lines. {{annotate: delete-markers "remove all three markers"}} Keep the code you want. It might be your version, their version, or a combination where you keep pieces of both.

```code:resolve-steps lang=bash
# BEFORE: conflicted file (git status shows "both modified")
$ git status
On branch main
You have unmerged paths.
  (fix conflicts and run "git commit")

Unmerged paths:
  (use "git add <file>..." to mark resolution)
        both modified:   src/config.ts

# You resolve: edit src/config.ts, delete markers, keep right code

# AFTER: mark it resolved
$ git add src/config.ts

# Check — now "modified" not "both modified"
$ git status
On branch main
All conflicts fixed but you are still merging.
  (use "git commit" to conclude merge)

Changes to be committed:
        modified:   src/config.ts

# Finish the merge
$ git commit
---
delete-markers: 1-10
```

{{clear: slide}}

`git add` on a resolved file is how you signal resolution. {{focus: delete-markers}} Git tracks which files are still conflicted by watching what's in the index. {{annotate: delete-markers "git add = resolved"}} When you `git add src/config.ts`, git removes the conflict flag for that file. Once all conflicted files are resolved and staged, you run `git commit` to complete the merge. Git pre-populates the commit message with merge info. {{focus: none}}

Multiple conflicts in multiple files work the same way. {{show: multi-conflict typewriter 1.5s linear}} Resolve each file individually. `git add` each one. {{annotate: status-check "check status often"}} When `git status` shows no more "both modified" entries, run `git commit`.

```code:multi-conflict lang=bash
# Multiple files in conflict
$ git status
Unmerged paths:
        both modified:   src/config.ts
        both modified:   src/auth.ts
        both modified:   src/session.ts

# Resolve one by one, then add each
$ git add src/config.ts
$ git add src/auth.ts
$ git add src/session.ts

# All resolved — finish the merge
$ git commit
---
status-check: 2-6
```

{{clear: slide}}

# Helpful Tools

You don't always have to manually delete markers. {{show: conflict-tools typewriter 1.5s linear}} Git provides tools for common cases. {{annotate: ours-flag "--ours takes your entire version"}} `git checkout --ours <file>` replaces the file entirely with your version — removes the conflict markers and uses your side. Useful when you're certain your changes win.

```code:conflict-tools lang=bash
# Take your version entirely (discard incoming)
$ git checkout --ours src/config.ts
$ git add src/config.ts

# Take incoming version entirely (discard yours)
$ git checkout --theirs src/config.ts
$ git add src/config.ts

# Show combined diff before resolution
$ git diff --cc src/config.ts

# Give up — reset to state before the merge started
$ git merge --abort

# Open a visual diff tool (uses whatever you've configured)
$ git mergetool src/config.ts
---
ours-flag: 1-3
theirs-flag: 5-7
```

{{clear: slide}}

`git diff --cc` shows the combined diff. {{zoom: 1.2x}} {{annotate: cc-flag "--cc = combined diff"}} The `--cc` flag shows you both sides of the conflict at once, with `+` marking what you'd keep from each side, before you've resolved anything. {{zoom: 1x}} It's a fast read of what actually differs.

`git merge --abort` is the escape hatch. {{focus: cc-flag}} If you started the merge, looked at the conflicts, and decided this is the wrong moment — abort. {{annotate: abort-flag "abandon, go back"}} It returns your working directory and index to exactly the state before the merge began. The feature branch is untouched. Nothing is lost. {{focus: none}}

`git mergetool` opens a three-pane visual diff if you have one configured (`vimdiff`, `meld`, `kdiff3`, VS Code). {{show: mergetool-config typewriter 1.5s linear}} {{annotate: tool-config "configure once, reuse"}} Configure it once and it's always available.

```code:mergetool-config lang=bash
# Configure VS Code as your merge tool
$ git config --global merge.tool vscode
$ git config --global mergetool.vscode.cmd \
    'code --wait $MERGED'

# Then just run:
$ git mergetool
---
tool-config: 1-4
```

{{clear: slide}}

# Preventing Conflicts

Conflicts are not bad luck. {{show: prevention-diagram slide 0.4s ease-out}} They're a signal: two people touched the same file over too long a time. {{annotate: short-lived "short branches = fewer conflicts"}} The fix is structural.

```diagram:prevention-diagram
long-branch [long-lived branch]
short-branch [short-lived branch]
main-node [main]
conflict-node [conflict zone]
clean-merge [clean merge]
long-branch --> conflict-node: days of divergence
short-branch --> clean-merge: hours of divergence
main-node --> long-branch
main-node --> short-branch
{Problem: long-branch, conflict-node}
{Better: short-branch, clean-merge}
---
short-lived: short-branch, clean-merge
```

{{clear: slide}}

Short branches beat long ones. {{show: prevention-rules typewriter 1.5s linear}} A branch that lives less than a day rarely conflicts. {{annotate: fetch-often "fetch = stay informed"}} A branch that lives two weeks is conflict debt accumulating daily.

Fetch regularly. {{focus: fetch-often}} `git fetch` costs nothing and shows you what's changing on main. {{annotate: communicate "talk to teammates"}} When you see a teammate touching the same files you're about to touch, coordinate before you write code, not after you've both written it.

```code:prevention-rules lang=bash
# Stay current — fetch before you start a new session
$ git fetch origin

# See what changed on main since you branched
$ git log HEAD..origin/main --oneline --name-only

# See if your files conflict with recent main changes
$ git diff HEAD...origin/main -- src/config.ts

# Merge main into your branch often (don't wait)
$ git merge origin/main

# The ideal: short-lived branches, merged daily
# feature/ branches that live > 3 days are a smell
---
fetch-often: 1-3
communicate: 7-9
```
