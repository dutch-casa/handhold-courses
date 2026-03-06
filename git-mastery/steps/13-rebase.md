---
title: Rebase
---

# The Problem Rebase Solves

Three days ago you branched `feature/login` from main. {{show: stale-branch slide 0.4s ease-out}} Since then, main moved forward by four commits. {{annotate: stale-base "feature base is now old"}} Your commits sit on top of old work. You want your feature to be based on the latest main before you open a pull request.

```data:stale-branch type=graph layout=tree
(c1 "init") -> (c2 "add auth") -> (c3 "add cache") -> (c4 "fix null") -> (c5 "update deps")
(c2 "add auth") -> (f1 "login form") -> (f2 "validation") -> (f3 "session")
^main=c5
^feature=f3
---
stale-base: c2
```

{{clear: slide}}

With merge, you'd get a merge commit and tangled history. {{show: merge-option slide 0.4s ease-out}} The graph would show two lines joining. {{annotate: messy-history "merge commit = history noise"}} For a long-running project with many features, this piles up fast. Reading `git log --graph` becomes archaeology.

With rebase, the commits are replanted. {{focus: messy-history}} Your three feature commits get new SHAs, new parents, and they land on top of the latest main. {{annotate: linear-result "linear history after rebase"}} The result looks as if you had branched from main today. The history is clean and readable. {{focus: none}}

```data:merge-option type=graph layout=tree
(c1 "init") -> (c2 "add auth") -> (c3 "add cache") -> (c4 "fix null") -> (c5 "update deps")
(c2 "add auth") -> (f1 "login form") -> (f2 "validation") -> (f3 "session")
(c5 "update deps") -> (mc "Merge feature into main")
(f3 "session") -> (mc "Merge feature into main")
^main=mc
^feature=f3
---
messy-history: mc
```

{{clear: slide}}

# What Rebase Actually Does

Rebase works in steps. {{show: rebase-step1 slide 0.4s ease-out}} Follow each one. Before: feature has commits F1, F2, F3 on top of C2 (the old base). Main is now at C5.

```data:rebase-step1 type=graph layout=tree
(c1 "init") -> (c2 "add auth") -> (c3 "add cache") -> (c4 "fix null") -> (c5 "update deps")
(c2 "add auth") -> (f1 "login form") -> (f2 "validation") -> (f3 "session")
^main=c5
^feature=f3
---
```

{{transform: rebase-step1->rebase-step2 0.4s ease-out}}

Step 1: Git saves the diffs for F1, F2, F3 as patches. It detaches the feature commits from C2. {{annotate: patch-saved "diff saved, not the commit"}} The commits become temporarily unreachable — they're still in git's object store but no branch pointer reaches them.

```data:rebase-step2 type=graph layout=tree
(c1 "init") -> (c2 "add auth") -> (c3 "add cache") -> (c4 "fix null") -> (c5 "update deps")
(f1 "login form [detached]") -> (f2 "validation [detached]") -> (f3 "session [detached]")
^main=c5
---
patch-saved: f1, f2, f3
```

{{transform: rebase-step2->rebase-step3 0.4s ease-out}}

Step 2: Git applies F1's patch on top of C5. The result is a new commit F1' — same diff, new parent (C5), new SHA. {{annotate: new-sha "new SHA because new parent"}} Then F2's patch on top of F1', producing F2'. Then F3's patch on top of F2', producing F3'.

```data:rebase-step3 type=graph layout=tree
(c1 "init") -> (c2 "add auth") -> (c3 "add cache") -> (c4 "fix null") -> (c5 "update deps")
(c5 "update deps") -> (f1p "login form'") -> (f2p "validation'") -> (f3p "session'")
^main=c5
^feature=f3p
---
new-sha: f1p, f2p, f3p
```

{{transform: rebase-step3->rebase-done 0.4s ease-out}}

Step 3: Feature branch pointer moves to F3'. The result is a perfectly linear history. {{annotate: linear-done "as if you branched today"}} The old F1, F2, F3 objects are now unreachable and will be garbage collected eventually. Main is still at C5. Now `git merge feature` into main will be a clean fast-forward.

```data:rebase-done type=graph layout=tree
(c1 "init") -> (c2 "add auth") -> (c3 "add cache") -> (c4 "fix null") -> (c5 "update deps") -> (f1p "login form'") -> (f2p "validation'") -> (f3p "session'")
^main=c5
^feature=f3p
---
linear-done: f1p, f2p, f3p
path: c1, c2, c3, c4, c5, f1p, f2p, f3p
```

{{clear: slide}}

# git rebase Commands

The basic command. {{show: rebase-commands typewriter 2s linear}} `git rebase main` — while on feature, replant feature commits onto main's tip. {{annotate: basic-rebase "rebase current branch"}} This is the 90% case.

```code:rebase-commands lang=bash
# Rebase current branch onto main
$ git switch feature/login
$ git rebase main

# Rebase feature onto main without switching first
$ git rebase main feature/login

# Surgical rebase: take commits from feature
# that are NOT on oldbase, and replay on newbase
$ git rebase --onto newbase oldbase feature

# Example: move commits between c3..feature onto main
$ git rebase --onto main c3 feature/login

# Conflict during rebase: resolve, stage, then
$ git add src/config.ts
$ git rebase --continue

# Or give up entirely
$ git rebase --abort
---
basic-rebase: 1-3
onto-region: 7-10
conflict-region: 12-15
```

{{clear: slide}}

{{zoom: 1.2x}} The `--onto` form is surgical. {{annotate: onto-region "--onto surgically replants"}} `git rebase --onto newbase oldbase branch` means: take the commits on branch that are NOT on oldbase, and replay them onto newbase. {{zoom: 1x}} Use it when a branch was accidentally based on the wrong branch.

Rebase conflicts work like merge conflicts. {{focus: conflict-region}} You see the same `<<<<<<<` markers. {{annotate: conflict-region "resolve then --continue"}} You resolve them the same way. But instead of `git commit` to finish, you run `git rebase --continue` to move on to the next commit in the sequence. Each commit in the rebase is replayed one at a time. A 5-commit rebase with 2 conflicting commits means you resolve conflicts twice. {{focus: none}}

{{clear: slide}}

# The Golden Rule

NEVER rebase commits that have been pushed and other people are using. {{show: golden-rule-diagram slide 0.4s ease-out}} This is not a preference. It is a hard rule. {{annotate: rewrite-warning "rebase rewrites SHAs"}} Rebase changes commit SHAs. If a teammate has your old SHA in their local history, and you push a rebased version with a new SHA, their git and yours have diverged. Their subsequent pushes will try to re-add commits you've rewritten. It's a mess that requires force-coordination to clean up.

```diagram:golden-rule-diagram
your-old [your old SHA: abc1234]
your-new [your new SHA: xyz9876]
teammate [teammate's repo has abc1234]
remote [remote: abc1234]
conflict-node [history conflict]
your-old --> your-new: you rebase
your-old --> remote: you already pushed
remote --> teammate: teammate fetched
your-new --> remote: you force push
teammate --> conflict-node: teammate's history broken
{Safe: your-old}
{Danger: your-new, conflict-node}
---
rewrite-warning: your-new
```

{{clear: slide}}

Safe zones for rebase. {{show: safe-zones typewriter 1.5s linear}} {{annotate: local-safe "local-only = safe"}} Local branches that have never been pushed: always safe to rebase. Your own fork before opening a PR: safe. After a PR is open and others have checked it out: dangerous.

The common workflow: rebase before you push. {{focus: local-safe}} `git rebase -i origin/main` while on your feature branch, before you run `git push`. {{annotate: pre-push-rebase "rebase before first push"}} Clean up the history, squash noise commits, reword messages. Then push once. The remote never sees the messy intermediate state. {{focus: none}}

```code:safe-zones lang=bash
# SAFE: local feature branch, not yet pushed
$ git rebase -i origin/main   # clean up before pushing
$ git push -u origin feature/login

# SAFE: your own fork, before PR is opened
$ git rebase upstream/main
$ git push --force-with-lease origin feature/login

# DANGEROUS: PR is open, teammates have pulled your branch
# Don't do this without coordinating with your team
$ git rebase main
$ git push --force-with-lease   # they'll have to re-pull

# NEVER DO THIS on shared branches:
$ git push --force origin main
---
local-safe: 1-3
pre-push-rebase: 1-3
```

{{clear: slide}}

# Rebase vs Merge — When to Use Which

This is not a religious debate. {{show: decision-matrix slide 0.4s ease-out}} Each tool is right in different situations. {{annotate: context-matters "context decides"}} The question is: what history do you want to see?

```diagram:decision-matrix
pr-merge [merge for PRs]
local-update [rebase to update local]
main-target [merging INTO main]
feature-update [keeping feature current]
linear-history [clean linear history]
branch-preserved [branch visible in graph]
pr-merge --> branch-preserved: no-ff merge commit
pr-merge --> main-target
local-update --> linear-history: rebase first
local-update --> feature-update
{Use-Merge: pr-merge, main-target, branch-preserved}
{Use-Rebase: local-update, feature-update, linear-history}
---
context-matters: pr-merge, local-update
```

{{clear: slide}}

Use merge for pull requests. {{show: when-to-use typewriter 1.5s linear}} The merge commit records when a feature branch landed in main. {{annotate: use-merge "PRs → merge --no-ff"}} It's a useful historical record. GitHub, GitLab, and Bitbucket all create a merge commit on PR close — that's the right behavior.

Use rebase to stay current with main during development. {{focus: use-merge}} `git pull --rebase` keeps your local branch linear. {{annotate: use-rebase "daily updates → rebase"}} Set it globally and forget it: `git config --global pull.rebase true`. Your local history will be clean, and when you eventually merge the PR, the merge commit is meaningful rather than being noise on top of noise. {{focus: none}}

```code:when-to-use lang=bash
# Set pull.rebase globally — do this once
$ git config --global pull.rebase true

# Now git pull does fetch + rebase (not fetch + merge)
$ git pull   # equivalent to: git fetch && git rebase origin/main

# PR merge: use --no-ff to preserve the branch context
$ git switch main
$ git merge --no-ff feature/login
$ git push

# The opinionated setup for daily work:
# - pull.rebase = true  (linear local history)
# - merge --no-ff for PRs  (visible branch history)
---
use-rebase: 1-4
use-merge: 7-9
```
