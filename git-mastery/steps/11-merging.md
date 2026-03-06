---
title: Merging
---

# Why Merge

You branched off main three days ago. {{show: diverged-graph slide 0.4s ease-out}} You've made five commits on `feature/login`. Main has moved forward with two commits from other teammates. {{annotate: feature-tip "feature work done"}} Now the feature works. You need to bring it into main.

```data:diverged-graph type=graph layout=tree
(c1 "init") -> (c2 "setup") -> (c3 "base work")
(c3 "base work") -> (m1 "teammate fix") -> (m2 "teammate feat")
(c3 "base work") -> (f1 "login form") -> (f2 "validation") -> (f3 "session") -> (f4 "tests") -> (f5 "cleanup")
^main=m2
^feature=f5
^HEAD=m2
---
feature-tip: f5
```

{{clear: slide}}

The goal is simple: combine two histories into one. {{show: merge-goal slide 0.4s ease-out}} Every commit on `feature` should be reachable from `main` after the merge. {{annotate: diverge-point "common ancestor"}} The point where the branches split is called the common ancestor. Git uses this to compute what changed on each side.

Merging never destroys commits. {{focus: diverge-point}} It creates a new situation where both sets of commits are in the ancestry graph. {{annotate: merge-goal "history preserved"}} The commits themselves are untouched. Only pointers move and a new merge commit may be created. {{focus: none}}

# Fast-Forward Merge

The simplest case: no divergence. {{show: ff-before slide 0.4s ease-out}} You branched `feature` off `main`. Main has not moved since. Every commit on `feature` is a direct linear descendant of main's tip. {{annotate: ff-check "can fast-forward"}} There is no work on main that isn't already in feature's ancestry.

```data:ff-before type=graph layout=tree
(c1 "init") -> (c2 "setup") -> (c3 "base")
(c3 "base") -> (f1 "add form") -> (f2 "add tests")
^main=c3
^feature=f2
^HEAD=c3
---
ff-check: c3
```

{{transform: ff-before->ff-after 0.5s spring}}

Run `git merge feature`. Git detects no divergence. It moves the `main` pointer forward to `f2`. {{annotate: ff-result "no new commit needed"}} No merge commit is created. No new commit object at all. Git literally just writes a different SHA into `.git/refs/heads/main`. The history looks perfectly linear.

```data:ff-after type=graph layout=tree
(c1 "init") -> (c2 "setup") -> (c3 "base")
(c3 "base") -> (f1 "add form") -> (f2 "add tests")
^main=f2
^feature=f2
^HEAD=f2
---
ff-result: f2
```

{{clear: slide}}

`--no-ff` forces a merge commit even when a fast-forward is possible. {{show: no-ff-code typewriter 1.5s linear}} {{annotate: no-ff-flag "--no-ff = merge commit"}} This is the right choice in pull request workflows. It preserves the branch in the history graph — you can see that these commits came from a feature branch, not just a sequence of commits directly on main. {{focus: no-ff-flag}} GitHub's "Merge pull request" button does this by default.

```code:no-ff-code lang=bash
# Fast-forward (default when possible)
$ git merge feature/login
Updating c3..f2
Fast-forward
 src/login.ts | 42 +++++++++++++++++++++
 1 file changed, 42 insertions(+)

# Force a merge commit (better for PR workflows)
$ git merge --no-ff feature/login
Merge made by the 'ort' strategy.
 src/login.ts | 42 +++++++++++++++++++++
 1 file changed, 42 insertions(+)

# Resulting graph shows the branch grouping
$ git log --oneline --graph
*   a4b2c1d Merge branch 'feature/login'
|\
| * f2 add tests
| * f1 add form
|/
*   c3 base
---
no-ff-flag: 9-12
```

{{clear: slide}}

# Three-Way Merge

Real divergence. {{show: threeway-before slide 0.4s ease-out}} Both branches have commits the other doesn't. {{annotate: ancestor "common ancestor = c3"}} Git cannot simply move a pointer. It has to combine the changes.

```data:threeway-before type=graph layout=tree
(c1 "init") -> (c2 "setup") -> (c3 "base")
(c3 "base") -> (m1 "fix auth") -> (m2 "add cache")
(c3 "base") -> (f1 "add form") -> (f2 "add tests")
^main=m2
^feature=f2
^HEAD=m2
---
ancestor: c3
```

{{clear: slide}}

Git performs a three-way merge. {{show: threeway-merge-steps slide 0.4s ease-out}} It looks at three things: the common ancestor (C3), the tip of main (M2), and the tip of feature (F2). {{annotate: three-points "3 inputs, 1 output"}} For each changed file, git asks: what changed on main relative to the ancestor? What changed on feature? If the changes are to different lines, git combines them automatically.

The result is a merge commit with two parents. {{focus: three-points}} Normal commits have one parent. {{annotate: two-parents "merge commit = 2 parents"}} A merge commit has two. `git log --graph` shows this as two lines joining. This is the visible record that a merge happened.

```data:threeway-after type=graph layout=tree
(c1 "init") -> (c2 "setup") -> (c3 "base")
(c3 "base") -> (m1 "fix auth") -> (m2 "add cache")
(c3 "base") -> (f1 "add form") -> (f2 "add tests")
(m2 "add cache") -> (mc "Merge feature/login")
(f2 "add tests") -> (mc "Merge feature/login")
^main=mc
^feature=f2
^HEAD=mc
---
two-parents: mc
three-points: m2, f2, c3
```

{{clear: slide}}

```code:threeway-code lang=bash
# On main, with feature diverged
$ git merge feature/login
Merge made by the 'ort' strategy.
 src/login.ts  | 42 +++++++++++++++++++++
 src/session.ts| 18 +++++++++
 2 files changed, 60 insertions(+)

# View the merge commit's two parents
$ git cat-file -p HEAD
tree 9b8f4a2c1d0e7f3b6a5c8d2e1f0b9a4c7d6e3f2
parent a3f2d9c1b7e4a88f0d5e3c2b9a6f4d1e8c7b3a5f
parent 7e1a4c9b3f2d0e8a5c7b6d4f1e2a3c9b8d0f7e4
author Jane Smith <jane@example.com> 1709300000 +0000

Merge branch 'feature/login'
---
two-parents: 9-10
```

{{clear: slide}}

# git merge --squash

Sometimes the feature branch history is noise. {{show: squash-before slide 0.4s ease-out}} "WIP", "fix lint", "actually fix lint", "ok done" — eight commits that tell no story. {{annotate: noisy-commits "history noise"}} You want the feature in main as a single, clean, meaningful commit.

```data:squash-before type=graph layout=tree
(c1 "base") -> (m1 "main work")
(c1 "base") -> (f1 "WIP") -> (f2 "fix") -> (f3 "more fix") -> (f4 "lint") -> (f5 "ok done")
^main=m1
^feature=f5
---
noisy-commits: f1, f2, f3, f4, f5
```

{{clear: slide}}

`git merge --squash feature` takes all the changes from the feature branch and stages them. {{show: squash-after slide 0.4s ease-out}} It does not create a commit. It does not create a merge commit. {{annotate: squash-staged "staged, not committed"}} You write a single commit message that explains the whole feature. That one commit lands on main.

```data:squash-after type=graph layout=tree
(c1 "base") -> (m1 "main work") -> (sq "Add login feature")
^main=sq
---
squash-staged: sq
```

{{clear: slide}}

The feature branch commits are NOT in the merged history. {{show: squash-code typewriter 1.5s linear}} {{annotate: no-link "feature branch orphaned"}} The squash commit has one parent: the old main tip. There is no link to the feature branch. This means `git branch -d feature` will fail (unmerged) — use `-D`. {{focus: no-link}} The trade-off: you lose commit granularity for a cleaner main history.

```code:squash-code lang=bash
# Squash merge: all feature changes staged, no commit yet
$ git merge --squash feature/login
Squash commit -- not updating HEAD
Automatic merge went well; stopped before committing as requested

# You write the commit message
$ git commit -m "Add login feature with session handling and validation"

# The feature branch is NOT marked merged
$ git branch -d feature/login
error: The branch 'feature/login' is not fully merged.
# Use -D to force delete
$ git branch -D feature/login
---
squash-staged: 1-4
no-link: 11-14
```

{{clear: slide}}

# Reading the Merge Result

You merged. {{show: log-code typewriter 2s linear}} Now verify what happened. {{annotate: graph-flag "--graph shows branching"}} `git log --oneline --graph` is your first tool. It draws ASCII art of the commit graph. You can see merge commits, where branches joined, and the linear sequences on each path.

```code:log-code lang=bash
# Visualize the full graph
$ git log --oneline --graph --all
*   a4b2c1d (HEAD -> main) Merge branch 'feature/login'
|\
| * f2e1a3b (feature/login) Add tests for session
| * c9d8e7f Add login form and validation
|/
* 4b5c6d7 Add cache layer
* 3a2b1c0 Fix auth token expiry
* 1d2e3f4 Initial project setup

# See only merge commits
$ git log --merges --oneline
a4b2c1d Merge branch 'feature/login'

# Commits in main not in feature (what main added)
$ git log feature/login..main --oneline
4b5c6d7 Add cache layer
3a2b1c0 Fix auth token expiry

# Commits in feature not yet in main (what feature added)
$ git log main..feature/login --oneline
f2e1a3b Add tests for session
c9d8e7f Add login form and validation
---
graph-flag: 1-10
```

{{clear: slide}}

`git log A..B` is the commit range operator. {{zoom: 1.2x}} {{annotate: range-op "A..B = reachable from B, not A"}} It means: show commits reachable from B that are not reachable from A. {{zoom: 1x}} Use it constantly to understand what would be merged before you merge. {{focus: none}} Run it before every merge to know exactly what you're pulling in.
