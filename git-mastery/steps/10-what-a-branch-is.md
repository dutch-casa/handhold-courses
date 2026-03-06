---
title: What a Branch Is
---

# A Branch Is a Text File

You picture a branch as a diverging timeline. {{show: commit-chain slide 0.4s ease-out}} That mental model is useful. But it hides the truth about what a branch actually *is* in git's object model. {{annotate: main-ptr "main = one SHA"}} A branch is 41 bytes on disk. A filename and a SHA. That is the entire data structure.

Look at four commits in a chain. {{focus: tip}} The branch `main` is just a pointer to the last one. {{annotate: tip "tip commit"}} Nothing more. It's a file at `.git/refs/heads/main` whose only content is the SHA of that commit. Git reads that file, looks up the commit object, and hands you the tree.

Creating a branch creates a file. {{focus: none}} Deleting a branch deletes a file. {{annotate: base "ancestry unchanged"}} The commits themselves are untouched. The DAG doesn't change. Only the pointer changes. This matters because branch operations in git are O(1) — instant — regardless of how many commits are in the history.

```data:commit-chain type=graph layout=tree
(c1 "commit A") -> (c2 "commit B") -> (c3 "commit C") -> (c4 "commit D")
^main=c4
^HEAD=c4
---
tip: c4
base: c1
chain: c1, c2, c3, c4
```

{{clear: slide}}

Now look at what that pointer actually is on disk. {{show: branch-file typewriter 2s linear}} One line. One SHA. Forty hexadecimal characters plus a newline. {{annotate: sha-line "41 bytes total"}} This is the file git reads to know where `main` is. When you make a new commit, git writes the new commit's SHA into this file. That's the entire "advance a branch" operation.

```code:branch-file lang=bash
# The branch pointer lives here
$ cat .git/refs/heads/main
a3f2d9c1b7e4a88f0d5e3c2b9a6f4d1e8c7b3a5f

# That SHA is the commit object
$ git cat-file -p a3f2d9c1b7e4a88f0d5e3c2b9a6f4d1e8c7b3a5f
tree 9b8f4a2c1d0e7f3b6a5c8d2e1f0b9a4c7d6e3f2
parent 7e1a4c9b3f2d0e8a5c7b6d4f1e2a3c9b8d0f7e4
author Jane Smith <jane@example.com> 1709300000 +0000
committer Jane Smith <jane@example.com> 1709300000 +0000

Add login endpoint
---
sha-line: 2
```

{{clear: slide}}

# git branch Commands

You have a branch. {{show: branch-commands typewriter 2s linear}} Now you need to manage it. {{annotate: list-cmd "list with asterisk"}} `git branch` with no arguments lists all local branches, with an asterisk on the current one. Simple. Run it often.

Creating a new branch does not switch to it. {{focus: create-region}} `git branch <name>` creates the `.git/refs/heads/<name>` file pointing at HEAD's current commit. {{annotate: create-region "HEAD does not move"}} You are still on whatever branch you were on. This is often surprising. Use `git switch -c <name>` if you want create-and-switch in one step.

Deleting has two modes. {{focus: delete-region}} `git branch -d feature` is safe — it refuses to delete a branch with commits not reachable from any other branch. {{annotate: delete-region "-d is safe"}} `git branch -D feature` is the force version. It deletes unconditionally. Reach for `-D` rarely and only when you know the work is gone intentionally.

```code:branch-commands lang=bash
# List branches — asterisk marks current
$ git branch
* main
  feature/login
  fix/null-check

# Create a branch (stays on current branch)
$ git branch feature/dashboard

# Show last commit on each branch
$ git branch -v
* main           a3f2d9c Add login endpoint
  feature/login  7e1a4c9 WIP: session handling
  fix/null-check 9b8f4a2 Fix null in auth check

# Which branches are merged into current?
$ git branch --merged
  fix/null-check

# Which are NOT merged? (work you might lose if deleted)
$ git branch --no-merged
  feature/login

# Safe delete (fails if unmerged)
$ git branch -d fix/null-check

# Force delete
$ git branch -D feature/abandoned
---
list-cmd: 1-5
create-region: 7-8
delete-region: 22-25
```

{{clear: slide}}

`git branch -v` is underused. {{zoom: 1.2x}} It shows the last commit message on each branch. {{annotate: v-flag "last commit per branch"}} When you have a dozen branches and can't remember what each one was for, `-v` saves you from switching into each one to check. {{zoom: 1x}}

`--merged` and `--no-merged` are cleanup aids. {{focus: list-cmd}} Every branch in `--merged` is safe to delete — its commits are already in your current branch. {{annotate: list-cmd "safe to delete"}} Audit this list before big releases. {{focus: none}}

{{clear: slide}}

# git switch vs git checkout

`git checkout` does too many things. {{show: checkout-problems slide 0.4s ease-out}} It switches branches. It restores individual files. It checks out specific commits (detaching HEAD). It creates branches. One command, four completely different operations. {{annotate: checkout-node "too many jobs"}} Git added `git switch` in version 2.23 (August 2019) to split this into focused commands.

`git switch <branch>` moves to an existing branch. {{focus: switch-region}} `git switch -c <name>` creates a branch and switches to it. {{annotate: switch-region "single purpose"}} `git switch -` goes back to the previous branch — equivalent to `cd -` in bash. That last one is genuinely useful on a daily basis.

Teach yourself `switch`. {{focus: none}} Use `checkout` only when reading old tutorials or working with a git version before 2.23. {{annotate: modern-cmd "2.23+ standard"}} The command surface is smaller and the intent is unambiguous. `switch` means you're moving branches. Nothing else.

```diagram:checkout-problems
switch-node [git switch]
checkout-node [git checkout]
switch-branch [switch branch]
restore-file [restore file]
detach-head [detach HEAD]
create-branch [create branch]
switch-node --> switch-branch: git switch
switch-node --> create-branch: git switch -c
checkout-node --> switch-branch: git checkout branch
checkout-node --> restore-file: git checkout -- file
checkout-node --> detach-head: git checkout <sha>
checkout-node --> create-branch: git checkout -b branch
{Modern: switch-node, switch-branch, create-branch}
{Legacy: checkout-node}
---
switch-region: switch-node, switch-branch, create-branch
modern-cmd: switch-node
```

{{clear: slide}}

```code:switch-commands lang=bash
# Move to an existing branch
$ git switch feature/login

# Create and switch in one step (preferred over checkout -b)
$ git switch -c feature/dashboard

# Go back to the previous branch (very useful)
$ git switch -

# For historical context: what checkout was doing
$ git checkout feature/login    # ← equivalent to switch
$ git checkout -b feature/new   # ← equivalent to switch -c
$ git checkout -- src/auth.ts   # ← this is git restore now
---
switch-region: 1-10
modern-cmd: 1-6
```

{{clear: slide}}

# How HEAD Moves When You Switch

HEAD before the switch. {{show: head-before slide 0.4s ease-out}} You're on `main`. HEAD points to main. Main points to commit C4. {{annotate: head-ptr "HEAD → main → C4"}} Everything is a pointer chain. Three hops: HEAD → main → commit.

```data:head-before type=graph layout=tree
(c1 "commit A") -> (c2 "commit B") -> (c3 "commit C") -> (c4 "commit D")
(c3 "commit C") -> (f1 "feat: add X") -> (f2 "feat: add Y")
^main=c4
^HEAD=c4
^feature=f2
---
head-ptr: c4
```

{{clear: slide}}

Now run `git switch feature`. {{show: head-after slide 0.4s ease-out}} HEAD now points to `feature`. Feature points to F2. {{annotate: head-ptr-after "HEAD → feature → F2"}} The working directory has been updated to match the tree at F2. Files that exist in F2 but not in C4 appear. Files that exist in C4 but not in F2 disappear.

```data:head-after type=graph layout=tree
(c1 "commit A") -> (c2 "commit B") -> (c3 "commit C") -> (c4 "commit D")
(c3 "commit C") -> (f1 "feat: add X") -> (f2 "feat: add Y")
^main=c4
^feature=f2
^HEAD=f2
---
head-ptr-after: f2
```

{{clear: slide}}

Git manages the transition. {{show: switch-detail typewriter 1.5s linear}} It computes the diff between the two trees and applies the changes to your working directory and index. {{annotate: working-dir "working dir updated"}} If you have uncommitted modifications to files that differ between branches, git refuses the switch and tells you to commit or stash first. It will not silently overwrite your work.

```code:switch-detail lang=bash
# HEAD before switch
$ cat .git/HEAD
ref: refs/heads/main

# Switch branch
$ git switch feature/login

# HEAD after switch — one line changed
$ cat .git/HEAD
ref: refs/heads/feature/login

# Git blocked the switch (uncommitted conflict)
$ git switch feature/login
error: Your local changes to the following files would be overwritten:
        src/auth.ts
Please commit your changes or stash them before you switch branches.
---
working-dir: 4-5
```

{{clear: slide}}

# What git switch Actually Changes

Three things change when you switch. {{show: three-changes slide 0.4s ease-out}} Not one. Not two. Three. {{annotate: head-change "1: HEAD pointer"}} First: the `.git/HEAD` file changes to point to the new branch. Second: the index is updated to reflect the new branch's tree. Third: the working directory is updated to match the index.

```diagram:three-changes
head-file [.git/HEAD]
index [index / staging area]
working-dir [working directory]
commit-tree [commit tree]
head-file --> index: 2. index updated
index --> working-dir: 3. files updated
commit-tree --> head-file: 1. HEAD updated
{Changed: head-file, index, working-dir}
---
head-change: head-file
```

{{clear: slide}}

Branch naming conventions exist for a reason. {{show: naming-conventions typewriter 1.5s linear}} `feature/`, `fix/`, `chore/`, `release/` prefixes give you grouping in `git branch` output and in CI systems. {{annotate: prefix-group "prefix = namespace"}} They're not enforced by git but they're enforced by sanity. A list of twenty branches named `johns-work`, `fix`, `test2`, `new-thing` is a maintenance nightmare.

```code:naming-conventions lang=bash
# Common convention prefixes
feature/user-auth          # new functionality
feature/dashboard-v2

fix/null-check-in-session  # bug fixes
fix/missing-env-validation

chore/update-deps          # maintenance, no user-facing change
chore/remove-dead-code

release/v1.2.0             # release prep branches
hotfix/v1.1.1              # emergency production fix

# Check HEAD file vs branch ref
$ cat .git/HEAD
ref: refs/heads/feature/user-auth

$ cat .git/refs/heads/feature/user-auth
a3f2d9c1b7e4a88f0d5e3c2b9a6f4d1e8c7b3a5f
---
prefix-group: 1-11
```
