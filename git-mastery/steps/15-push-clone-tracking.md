---
title: Push, Clone, and Tracking
---

# git clone

Clone is not download. {{show: clone-steps slide 0.4s ease-out}} People treat it as "get the files". It's more structured than that. {{annotate: six-steps "6 distinct operations"}} Git clone is six operations in sequence. Understanding each one explains behavior that otherwise looks like magic.

```diagram:clone-steps
step-1 [1. mkdir the directory]
step-2 [2. git init]
step-3 [3. add origin remote]
step-4 [4. fetch all objects]
step-5 [5. create remote-tracking branches]
step-6 [6. checkout default branch]
step-1 --> step-2
step-2 --> step-3
step-3 --> step-4
step-4 --> step-5
step-5 --> step-6
{Setup: step-1, step-2, step-3}
{Data: step-4, step-5}
{Final: step-6}
---
six-steps: step-1, step-2, step-3, step-4, step-5, step-6
```

{{clear: slide}}

After clone, you have a fully connected local repository. {{show: clone-result slide 0.4s ease-out}} Every commit, every branch, every tag — local. {{annotate: full-history "full history locally"}} Remote-tracking branches exist for every remote branch. The default branch (usually `main`) is checked out. `origin` is configured pointing at the source URL.

```code:clone-result lang=bash
# Clone a repo
$ git clone https://github.com/you/yourrepo.git
Cloning into 'yourrepo'...
remote: Enumerating objects: 247, done.
remote: Counting objects: 100% (247/247), done.
Receiving objects: 100% (247/247), 1.24 MiB | 2.8 MiB/s, done.

# What clone created:
$ cd yourrepo

# 1. Initialized git repo
$ ls -la .git/
HEAD  config  description  hooks/  info/  objects/  refs/

# 2. Origin remote is configured
$ git remote -v
origin  https://github.com/you/yourrepo.git (fetch)
origin  https://github.com/you/yourrepo.git (push)

# 3. All remote branches are tracked
$ git branch -a
* main
  remotes/origin/HEAD -> origin/main
  remotes/origin/main
  remotes/origin/feature/login
  remotes/origin/release/v1.2.0

# 4. You're on the default branch
$ git log --oneline -3
a3f2d9c (HEAD -> main, origin/main) Add login endpoint
7e1a4c9 Fix null check
9b8f4a2 Initial setup
---
full-history: 9-25
six-steps: 1-6
```

{{clear: slide}}

Clone into a specific directory. {{show: clone-opts typewriter 1.5s linear}} `git clone <url> <dir>` names the target directory. {{annotate: dir-arg "specify target directory"}} `--depth 1` for a shallow clone — only the latest commit, no history. Useful for CI pipelines where you don't need git history. `--branch <name>` checks out a specific branch instead of the default.

```code:clone-opts lang=bash
# Clone into a named directory
$ git clone https://github.com/you/repo.git my-project

# Shallow clone: latest commit only (no full history)
$ git clone --depth 1 https://github.com/you/repo.git

# Clone a specific branch
$ git clone --branch feature/login https://github.com/you/repo.git

# Clone with SSH (preferred for push access)
$ git clone git@github.com:you/repo.git
---
dir-arg: 1-2
```

{{clear: slide}}

# git push

Push sends object data. {{show: push-explainer slide 0.4s ease-out}} It is not file upload. {{annotate: object-transfer "pushing git objects, not files"}} Git computes which commit objects the remote doesn't have, packs them, and sends the pack. The remote unpacks the objects and updates its branch reference. Your files are reconstructed from objects on both sides.

```diagram:push-explainer
local-commits [local commits: A, B, C]
remote-old [remote: only has A]
pack-transfer [pack transfer: B, C]
remote-new [remote: now has A, B, C]
local-commits --> pack-transfer: git computes diff
pack-transfer --> remote-new: objects sent
remote-old --> remote-new: B, C added
{Source: local-commits}
{Transit: pack-transfer}
{Dest: remote-old, remote-new}
---
object-transfer: pack-transfer
```

{{clear: slide}}

Push requires the remote to be fast-forwardable. {{show: push-commands typewriter 2s linear}} If someone else pushed to the remote branch since your last fetch, the remote is ahead of your local branch. {{annotate: ff-requirement "remote must be fast-forwardable"}} Git refuses the push to protect their work. The fix is always fetch first, rebase, then push.

```code:push-commands lang=bash
# Explicit form: push main to origin's main
$ git push origin main

# If upstream is set: argumentless push
$ git push

# Push and set upstream simultaneously (first push)
$ git push -u origin feature/login

# Push all branches
$ git push --all origin

# Push all annotated tags too
$ git push --follow-tags

# What a rejected push looks like:
$ git push origin main
To https://github.com/you/repo.git
 ! [rejected]        main -> main (non-fast-forward)
error: failed to push some refs to 'https://github.com/you/repo.git'
hint: Updates were rejected because the tip of your current branch is behind
hint: its remote counterpart. Integrate the remote changes (e.g.
hint: 'git pull ...') before pushing again.
---
ff-requirement: 13-19
```

{{clear: slide}}

# Push Rejected

Rejection is not failure. {{show: rejected-diagram slide 0.4s ease-out}} It's git protecting the remote from having its history broken. {{annotate: protect-remote "git protecting shared history"}} Someone merged a PR while you were working. Their commits are on origin/main. Yours are not yet. Pushing would overwrite their history — git refuses.

```data:rejected-diagram type=graph layout=tree
(c1 "base") -> (c2 "their commit") -> (c3 "their commit 2")
(c1 "base") -> (y1 "your commit") -> (y2 "your commit 2")
^origin-main=c3
^local-main=y2
---
protect-remote: c2, c3
```

{{clear: slide}}

The fix is always the same sequence. {{show: fix-rejected typewriter 1.5s linear}} Fetch what's on the remote. Rebase your commits on top. Push. {{annotate: fix-sequence "fetch → rebase → push"}} This puts their work first and your work on top — a fast-forward from the remote's perspective.

```code:fix-rejected lang=bash
# Push fails
$ git push origin main
! [rejected] main -> main (non-fast-forward)

# Step 1: fetch what they pushed
$ git fetch origin

# Step 2: rebase your commits onto origin/main
$ git rebase origin/main

# Step 3: push (now fast-forwardable)
$ git push origin main

# Or do fetch + rebase in one step:
$ git pull --rebase
$ git push
---
fix-sequence: 4-12
```

{{clear: slide}}

Never force push main. {{show: force-push-warning slide 0.4s ease-out}} `git push --force origin main` overwrites the remote's main with your local main, regardless of what's on the remote. {{annotate: danger-force "destroys teammates' history"}} Every teammate who has the old commits in their local history now has commits that don't exist on the remote. They'll have to reset and lose their local work, or create a mess of re-merged commits. This is a recoverable disaster but it's always a disaster.

```diagram:force-push-warning
your-rewritten [your rewritten history]
remote-old [remote: shared history]
teammate [teammate: has old commits]
disaster [broken local repos]
your-rewritten --> remote-old: git push --force (DANGER)
remote-old --> teammate: already pulled
teammate --> disaster: their history broken
{Danger: your-rewritten, disaster}
{Protected: remote-old, teammate}
---
danger-force: your-rewritten, disaster
```

{{clear: slide}}

# git push --force-with-lease

`--force-with-lease` is the safe force push. {{show: fwl-explainer typewriter 2s linear}} The lease is a condition: "only overwrite the remote if it still matches what I last saw". {{annotate: lease-check "conditional overwrite"}} If someone pushed to the remote since your last fetch, the lease fails and git aborts. You're protected from accidentally overwriting their work.

```code:fwl-explainer lang=bash
# UNSAFE: overwrites remote no matter what
$ git push --force origin feature/login

# SAFE: checks remote hasn't moved since last fetch
$ git push --force-with-lease origin feature/login

# If the lease fails:
$ git push --force-with-lease origin feature/login
To https://github.com/you/repo.git
 ! [rejected] feature/login -> feature/login
   (stale info)
error: failed to push some refs
# Someone else pushed — go fetch their changes first

# The correct workflow after rebasing your feature branch:
$ git rebase -i origin/main          # clean up history
$ git push --force-with-lease        # push safely
---
lease-check: 4-5
```

{{clear: slide}}

When to use `--force-with-lease`. {{show: fwl-workflow slide 0.4s ease-out}} The only legitimate use case: you rebased your own feature branch (local-only history rewrite), and need to update the remote to match. {{annotate: rebase-then-fwl "rebase → force-with-lease"}} This is common before opening a PR: clean up history locally, then update the remote feature branch.

Before a force push, always run `git log --oneline` to verify your local history is what you intend to push. {{focus: rebase-then-fwl}} Never force push without knowing exactly what you're sending. {{annotate: verify-first "log before force push"}} The 10 seconds of verification prevents painful recovery. {{focus: none}}

```code:fwl-workflow lang=bash
# Common workflow: clean up feature branch before PR

# 1. Interactive rebase to clean up commits
$ git rebase -i origin/main

# 2. Verify the result looks correct
$ git log --oneline --graph
* f3e2d1c Add login with session handling
* 9b8f4a2 base work

# 3. Push the cleaned history to remote
$ git push --force-with-lease origin feature/login

# Never --force on main or any shared branch
# Feature branches you own: OK with --force-with-lease
---
rebase-then-fwl: 1-11
verify-first: 5-7
```

{{clear: slide}}

# Tracking Setup and Cleanup

Tracking setup is a one-time operation per branch. {{show: tracking-setup typewriter 1.5s linear}} After `-u` or `--set-upstream-to`, you never need to specify the remote or branch name again for push/pull. {{annotate: upstream-benefit "set once, forget arguments"}} `git status` gains the ability to tell you ahead/behind counts.

```code:tracking-setup lang=bash
# Long form: set upstream for current branch
$ git push --set-upstream origin feature/login
# Equivalent short form:
$ git push -u origin feature/login

# Check tracking relationships for all branches
$ git branch -vv
* feature/login  a3f2d9c [origin/feature/login: ahead 2] Add form
  main           7e1a4c9 [origin/main] Latest main
  old-branch     3b4c5d6 [origin/old-branch: gone] Old work

# Remote branch deleted? You still have the stale tracking ref
$ git fetch --prune     # remove tracking refs for deleted remote branches
$ git remote prune origin    # same thing, more explicit

# Full status of all tracking relationships
$ git remote show origin
* remote origin
  Fetch URL: https://github.com/you/repo.git
  Push  URL: https://github.com/you/repo.git
  HEAD branch: main
  Remote branches:
    main           tracked
    feature/login  tracked
    feature/old    stale (use 'git remote prune' to remove)
  Local branches configured for 'git pull':
    main merges with remote main
    feature/login merges with remote feature/login
  Local refs configured for 'git push':
    main           pushes to main           (up to date)
    feature/login  pushes to feature/login  (local out of date)
---
upstream-benefit: 1-4
```

{{clear: slide}}

`git fetch --prune` is housekeeping. {{zoom: 1.2x}} {{annotate: prune-flag "--prune removes stale refs"}} A teammate deletes a remote branch after merging. Your local repo still has the `origin/feature/old` remote-tracking ref. `--prune` removes all tracking refs that no longer have a matching branch on the remote. {{zoom: 1x}} Run it as part of your regular fetch habit: `git fetch --prune`. {{focus: none}}
