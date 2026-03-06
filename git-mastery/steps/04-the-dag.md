---
title: The DAG
---

# Commits are a chain

{{show: first-commit grow 0.5s spring}} Your first commit. A single node.

{{annotate: c1-node "SHA: a1b2c3d"}} It has a hash. That hash is its permanent identity. {{zoom: 1.3x}} {{focus: c1-node}} The hash is computed from the content of the commit — author, timestamp, message, and the tree it points to. Change any of those things, and you get a completely different hash.

{{zoom: 1x}} {{focus: none}}

```data:first-commit type=graph layout=tree
(c1 "a1b2c3d\nInitial commit")
---
c1-node: c1
```

# The second commit remembers the first

{{clear: slide}}

{{show: two-commits grow 0.5s spring}} A second commit appears. {{annotate: c2-node "SHA: e4f5a6b"}} It has its own hash.

{{focus: c2-node}} {{zoom: 1.3x}} But look at the edge. {{annotate: parent-edge "points to parent"}} Every commit — except the first — stores the hash of the commit that came before it. This is the parent pointer.

{{zoom: 1x}} {{focus: parent-edge}} The parent pointer is what makes history possible. {{annotate: parent-edge "immutable link"}} You can always walk backward from any commit to the very beginning of the repository.

{{focus: none}} Add more commits and the chain grows. {{clear: slide}} {{show: commit-chain slide 0.4s ease-out}} {{draw: edge1}} The first commit points back to nothing — it's the root. {{draw: edge2}} The second knows the first. {{draw: edge3}} Each new commit remembers exactly where it came from.

```data:two-commits type=graph layout=tree
(c1 "a1b2c3d") -> (c2 "e4f5a6b")
---
c2-node: c2
parent-edge: c1
```

```data:commit-chain type=graph layout=tree
(c1 "a1b2c3d") -> (c2 "e4f5a6b") -> (c3 "f7g8h9i") -> (c4 "j0k1l2m")
---
tip: c4
parent-edge: c3
edge1: c1, c2
edge2: c2, c3
edge3: c3, c4
```

# A branch is a label

{{clear: slide}}

{{show: branch-pointer grow 0.5s spring}} Four commits. Now add a branch called `main`.

{{zoom: 1.3x}} {{focus: main-label}} {{annotate: main-label "just a label"}} `main` is not a folder. Not a copy of the code. It's a pointer — a file in `.git/refs/heads/` containing 41 bytes: the hash of the current tip commit, plus a newline.

{{zoom: 1x}} {{clear: slide}}

{{show: branch-file typewriter 1s linear}} Prove it to yourself. {{focus: cat-command}} Run this in any git repo. {{annotate: cat-command "try this now"}} You'll see a SHA. That SHA IS the branch.

{{zoom: 1.2x}} {{focus: branch-content}} One line. One hash. That's all a branch is. {{annotate: branch-content "41 bytes total"}} Every branch operation — creating, switching, deleting, moving — is just reading or writing this file.

{{zoom: 1x}} {{focus: none}}

```data:branch-pointer type=graph layout=tree
(c1 "a1b2c3d") -> (c2 "e4f5a6b") -> (c3 "f7g8h9i") -> (c4 "j0k1l2m")
^main=c4
---
main-label: c4
```

```code:branch-file lang=bash
# A branch is literally a text file
cat .git/refs/heads/main
# j0k1l2m4n5o6p7q8r9s0t1u2v3w4x5y6z7a8b9c0d

# Git confirms it
git rev-parse main
# j0k1l2m4n5o6p7q8r9s0t1u2v3w4x5y6z7a8b9c0d
---
cat-command: 2
branch-content: 3
```

# HEAD — where you are

{{clear: slide}}

{{show: head-attached grow 0.5s spring}} There's one more pointer: `HEAD`. It tells git where YOU are right now.

{{focus: head-ptr}} {{zoom: 1.2x}} {{annotate: head-ptr "you are here"}} Usually, `HEAD` points at a branch. The branch points at a commit. This is the normal state — "attached HEAD."

{{zoom: 1x}} {{focus: none}} When you make a new commit, two things happen automatically. The new commit's parent is set to whatever HEAD currently points at. Then the branch moves forward to the new commit.

{{clear: slide}}

{{show: head-moves slide 0.5s ease-out}} Watch it happen. {{focus: old-state}} Before the commit: HEAD → main → c4.

{{flow: commit-flow}} {{focus: new-commit}} After: HEAD still points at main. {{annotate: new-commit "new commit"}} Main now points at c5. HEAD moved because main moved.

{{flow: none}} {{focus: none}}

```data:head-attached type=graph layout=tree
(c1 "c1") -> (c2 "c2") -> (c3 "c3") -> (c4 "c4")
^main=c4
^HEAD=c4
---
head-ptr: c4
main-label: c4
```

```data:head-moves type=graph layout=tree
(c1 "c1") -> (c2 "c2") -> (c3 "c3") -> (c4 "c4") -> (c5 "c5 ← new")
^main=c5
^HEAD=c5
---
old-state: c4
new-commit: c5
commit-flow: c4, c5
```

# Detached HEAD

{{clear: slide}}

{{show: detached-head grow 0.4s ease-out}} Here's something that trips everyone up the first time: detached HEAD.

{{focus: head-direct}} {{zoom: 1.3x}} {{annotate: head-direct "danger zone"}} Normally HEAD → branch → commit. In detached HEAD state, HEAD → commit directly. No branch in between.

{{zoom: 1x}} {{focus: warning-zone}} This happens when you `git checkout <commit-hash>` or `git checkout <tag>`. {{annotate: warning-zone "floating in history"}} You're exploring old history. Perfectly safe to look around.

{{focus: none}} {{clear: slide}}

{{show: detached-problem slide 0.4s ease-out}} The danger: if you make commits in this state and then switch branches, those commits become unreachable. {{focus: orphan-commits}} {{annotate: orphan-commits "nothing points here"}} Git won't delete them immediately — they sit in the object store until garbage collection runs. But they're lost from your navigation perspective.

{{zoom: 1.2x}} {{focus: fix-it}} The fix is simple. If you want to keep commits made in detached HEAD: `git switch -c new-branch-name`. {{annotate: fix-it "save your work"}} That creates a branch pointing at your commits, reattaching HEAD.

{{zoom: 1x}} {{focus: none}}

```data:detached-head type=graph layout=tree
(c1 "c1") -> (c2 "c2") -> (c3 "c3") -> (c4 "c4")
^main=c4
^HEAD=c2
---
head-direct: c2
warning-zone: c2
main-label: c4
```

```data:detached-problem type=graph layout=tree
(c1 "c1") -> (c2 "c2") -> (c3 "c3") -> (c4 "c4")
(c2 "c2") -> (d1 "d1 orphan") -> (d2 "d2 orphan")
^main=c4
---
orphan-commits: d1, d2
fix-it: d1
```

# A merge has two parents

{{clear: slide}}

{{show: before-merge grow 0.5s spring}} Two developers, two branches, both building features. {{focus: main-tip}} {{annotate: main-tip "main branch"}} The `main` branch has been moving forward.

{{focus: feature-tip}} {{annotate: feature-tip "feature branch"}} A `feature` branch split off from an earlier commit and has its own new commits.

{{flow: both-lines}} They diverged. Both have changes the other doesn't. {{flow: none}} {{focus: none}}

{{clear: slide}}

{{show: after-merge slide 0.5s ease-out}} The merge commit. {{focus: merge-node}} {{zoom: 1.3x}} {{annotate: merge-node "two parents"}} This commit is special. It has two parents — one from each branch. Both lines of history are preserved, fused into a single point.

{{zoom: 1x}} {{focus: none}} The graph is no longer a straight line. It's a directed acyclic graph — a DAG. Commits flow in one direction (parent ← child), and there are no cycles. The history is a graph, not a list.

```data:before-merge type=graph layout=tree
(c1 "c1") -> (c2 "c2") -> (c3 "c3") -> (c4 "c4")
(c2 "c2") -> (f1 "f1") -> (f2 "f2")
^main=c4
^feature=f2
---
main-tip: c4
feature-tip: f2
both-lines: c2, c3, c4, f1, f2
```

```data:after-merge type=graph layout=tree
(c1 "c1") -> (c2 "c2") -> (c3 "c3") -> (c4 "c4") -> (m1 "merge")
(c2 "c2") -> (f1 "f1") -> (f2 "f2") -> (m1 "merge")
^main=m1
---
merge-node: m1
```

# The DAG is immutable

{{clear: slide}}

{{show: immutable-chain grow 0.5s spring}} Here's the most important thing to internalize: {{zoom: 1.2x}} {{focus: c3-node}} you cannot change a commit. {{annotate: c3-node "permanent"}} Every bit of a commit is baked into its SHA hash. The content, the author, the timestamp, the parent. Change any of it, you get a new object with a different hash.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

{{show: rebase-demo slide 0.5s ease-out}} When you run `git rebase`, it looks like history changed. {{focus: old-commits}} {{annotate: old-commits "original commits"}} But the original commits are still there in the object store.

{{flow: replay-flow}} {{focus: new-commits}} {{annotate: new-commits "new SHAs"}} Rebase created NEW commits with the same content but different parents — and therefore different hashes. The old commits still exist until garbage collection removes them.

{{flow: none}} {{zoom: 1.2x}} {{focus: new-tip}} The branch pointer moved to the new commits. {{annotate: new-tip "branch moved"}} That's all that changed from git's perspective.

{{zoom: 1x}} {{focus: none}} This is why rebasing pushed commits causes problems. Other people have your old commit hashes. You've replaced them with new ones they don't know about. History diverges.

```data:immutable-chain type=graph layout=tree
(c1 "c1") -> (c2 "c2") -> (c3 "c3 ← try to change this") -> (c4 "c4")
^main=c4
---
c3-node: c3
```

```data:rebase-demo type=graph layout=tree
(base "base") -> (a "A") -> (b "B")
(base "base") -> (a-prime "A'") -> (b-prime "B'")
^old-branch=b
^new-branch=b-prime
---
old-commits: a, b
new-commits: a-prime, b-prime
replay-flow: base, a-prime, b-prime
new-tip: b-prime
```
