---
title: The Remote Model
---

# What a Remote Is

"Origin" is not special. {{show: remote-config typewriter 2s linear}} It's a name. A convention that every git hosting tutorial uses. {{annotate: origin-name "origin = just a name"}} You could name it `github`, `upstream`, `backup`, or `xyzzy`. When you run `git clone`, git automatically names the source remote `origin`. That's the only reason it's everywhere.

A remote is a name and a URL stored in `.git/config`. {{focus: origin-name}} That's the entire data structure. {{annotate: config-entry "stored in .git/config"}} When you run `git push origin main`, git looks up `origin`, finds the URL, and connects to it. Nothing more magical than that.

```code:remote-config lang=bash
# See all remotes with their URLs
$ git remote -v
origin  https://github.com/you/yourrepo.git (fetch)
origin  https://github.com/you/yourrepo.git (push)

# See the raw config entry
$ cat .git/config
[core]
    repositoryformatversion = 0
    filemode = true
[remote "origin"]
    url = https://github.com/you/yourrepo.git
    fetch = +refs/heads/*:refs/remotes/origin/*
[branch "main"]
    remote = origin
    merge = refs/heads/main
---
origin-name: 2-4
config-entry: 9-12
```

{{clear: slide}}

Multiple remotes are common. {{show: multi-remote-diagram slide 0.4s ease-out}} Open source contribution pattern: `origin` is your fork, `upstream` is the original project. {{annotate: fork-pattern "origin = fork, upstream = source"}} You pull from `upstream` to stay current, push to `origin` for PRs.

```diagram:multi-remote-diagram
upstream-remote [github: upstream (original)]
origin-remote [github: origin (your fork)]
local-repo [git: local repository]
local-repo --> origin-remote: git push origin
upstream-remote --> local-repo: git fetch upstream
upstream-remote --> origin-remote: source of truth
{Yours: origin-remote, local-repo}
{External: upstream-remote}
---
fork-pattern: origin-remote, upstream-remote
```

{{clear: slide}}

```code:multi-remote-code lang=bash
# Fork workflow: two remotes
$ git remote add upstream https://github.com/original/repo.git

# Now you have two:
$ git remote -v
origin    https://github.com/you/repo.git (fetch)
origin    https://github.com/you/repo.git (push)
upstream  https://github.com/original/repo.git (fetch)
upstream  https://github.com/original/repo.git (push)

# Sync with the original project
$ git fetch upstream
$ git rebase upstream/main

# Push your changes to your fork
$ git push origin feature/new-thing
---
fork-pattern: 1-8
```

{{clear: slide}}

# Remote-Tracking Branches

`origin/main` is not your `main`. {{show: tracking-diagram slide 0.4s ease-out}} It's a local copy of what main looked like the last time you communicated with origin. {{annotate: snapshot-note "snapshot, not live"}} It is read-only. You cannot commit to it. You cannot switch to it with `git switch origin/main` (well, you can, but you'd detach HEAD). It's a reference point.

```diagram:tracking-diagram
remote-server [github: origin server]
remote-main [service: remote main branch]
tracking-branch [git: origin/main (local snapshot)]
local-main [git: main (your branch)]
remote-server --> remote-main: what exists there
remote-main --> tracking-branch: git fetch copies this
tracking-branch --> local-main: you merge/rebase into this
{Remote: remote-server, remote-main}
{Local: tracking-branch, local-main}
---
snapshot-note: tracking-branch
```

{{clear: slide}}

Remote-tracking branches only update when you fetch. {{show: tracking-staleness typewriter 1.5s linear}} Your teammate merged a PR to main 10 minutes ago. {{annotate: stale-tracking "stale until fetched"}} Your `origin/main` still points to the commit before their merge. You are working with outdated information until you `git fetch`.

`git branch -a` shows everything. {{focus: stale-tracking}} {{zoom: 1.2x}} Local branches in white. Remote-tracking branches in red (or prefixed with `remotes/`). {{annotate: branch-a-flag "-a shows all branches"}} The remote-tracking branches are in `.git/refs/remotes/` — a separate namespace from `.git/refs/heads/`. {{zoom: 1x}} {{focus: none}}

```code:tracking-staleness lang=bash
# Show all branches: local + remote-tracking
$ git branch -a
* main
  feature/login
  remotes/origin/HEAD -> origin/main
  remotes/origin/main
  remotes/origin/feature/dashboard
  remotes/origin/feature/login

# Check where origin/main currently is
$ git log origin/main --oneline -3
a3f2d9c Add login endpoint
7e1a4c9 Fix null check
9b8f4a2 Initial setup

# Your local main may be ahead or behind
$ git log main...origin/main --oneline
---
stale-tracking: 4-7
branch-a-flag: 1-7
```

{{clear: slide}}

# git fetch

Fetch is safe. {{show: fetch-before slide 0.4s ease-out}} It downloads new commit objects from the remote. It updates remote-tracking branches. It touches nothing else. {{annotate: fetch-safe "does not touch local branches"}} Your working directory is unchanged. Your local `main` is unchanged. Your current branch is unchanged. Fetch is reconnaissance.

```data:fetch-before type=graph layout=tree
(c1 "init") -> (c2 "setup") -> (c3 "add feature")
(c3 "add feature") -> (c4 "teammate fix") -> (c5 "teammate feat")
^remote-main=c5
^origin-main=c2
^local-main=c2
^HEAD=c2
---
fetch-safe: origin-main
```

{{clear: slide}}

After fetch. {{show: fetch-after slide 0.4s ease-out}} `origin/main` moved. Local `main` did not. {{annotate: post-fetch "origin/main updated"}} You now have C4 and C5 locally as commit objects. You can examine them, diff against them, log them. You just can't use them until you explicitly merge or rebase.

```data:fetch-after type=graph layout=tree
(c1 "init") -> (c2 "setup") -> (c3 "add feature")
(c3 "add feature") -> (c4 "teammate fix") -> (c5 "teammate feat")
^remote-main=c5
^origin-main=c5
^local-main=c2
^HEAD=c2
---
post-fetch: origin-main
```

{{clear: slide}}

Fetch is reconnaissance. Pull is commitment. {{show: fetch-commands typewriter 1.5s linear}} {{zoom: 1.2x}} Run `git fetch` at the start of every work session. {{annotate: habit-fetch "fetch before you start work"}} See what changed. Check if main moved. Then decide what to do — merge, rebase, or keep working independently. {{zoom: 1x}}

```code:fetch-commands lang=bash
# Fetch all remotes
$ git fetch

# Fetch a specific remote
$ git fetch origin

# Fetch and prune deleted remote branches
$ git fetch --prune

# See what changed after fetching
$ git log origin/main --oneline -5

# Diff your branch against the updated remote
$ git diff main..origin/main

# See commits on origin/main not on local main
$ git log main..origin/main --oneline
---
habit-fetch: 1-2
```

{{clear: slide}}

# git pull = fetch + merge

`git pull` is a shorthand. {{show: pull-internals typewriter 2s linear}} It does two things: `git fetch` followed by `git merge origin/<current-branch-upstream>`. {{annotate: pull-shortcut "fetch + merge in sequence"}} You might never have realized that merge commit in your history came from a pull.

```code:pull-internals lang=bash
# git pull is literally this:
$ git fetch origin
$ git merge origin/main   # (if your branch tracks origin/main)

# vs what git pull does — same thing:
$ git pull

# The result with default pull (fetch + merge):
$ git log --oneline --graph -5
*   a3f2d9c (HEAD -> main) Merge branch 'main' of github.com/you/repo
|\
| * 7e1a4c9 (origin/main) Teammate's commit
|/
* 9b8f4a2 Your last commit

# With --rebase (fetch + rebase):
$ git pull --rebase
$ git log --oneline --graph -5
* 7e1a4c9 (HEAD -> main, origin/main) Teammate's commit
* 9b8f4a2 Your last commit
---
pull-shortcut: 1-5
```

{{clear: slide}}

`--rebase` changes the outcome. {{focus: pull-shortcut}} Without it: you get a merge commit for every pull where your branch and origin diverged. {{annotate: rebase-cleaner "pull --rebase = linear"}} With it: your local commits are replanted on top of origin's commits. Linear history. No noise commits.

Set it globally once. {{focus: none}} `git config --global pull.rebase true`. {{annotate: global-rebase "set once, works everywhere"}} Every `git pull` now does rebase. If you ever need the merge behavior for a specific pull, `git pull --no-rebase`.

```code:pull-config lang=bash
# Set rebase as the pull strategy globally
$ git config --global pull.rebase true

# Now git pull is equivalent to:
$ git fetch && git rebase origin/main

# Override for a single pull if you need merge behavior
$ git pull --no-rebase
---
global-rebase: 1-2
rebase-cleaner: 1-4
```

{{clear: slide}}

# The Upstream Concept

Every local branch can track a remote branch. {{show: upstream-diagram slide 0.4s ease-out}} The tracking relationship is called the "upstream". {{annotate: upstream-def "upstream = remote counterpart"}} When set, `git push` and `git pull` work without arguments — they know where to go.

```diagram:upstream-diagram
local-feature [git: feature/login]
remote-feature [github: origin/feature/login]
local-main [git: main]
remote-main [github: origin/main]
local-feature --> remote-feature: tracks (upstream)
local-main --> remote-main: tracks (upstream)
{Local: local-feature, local-main}
{Remote: remote-feature, remote-main}
---
upstream-def: local-feature, remote-feature
```

{{clear: slide}}

`git status` knows about your upstream. {{show: upstream-commands typewriter 1.5s linear}} Once tracking is set, `git status` shows "Your branch is ahead of 'origin/main' by 2 commits" or "behind 3 commits". {{annotate: status-upstream "status shows ahead/behind"}} You always know where you stand relative to the remote.

```code:upstream-commands lang=bash
# Set upstream for the current branch
$ git branch --set-upstream-to=origin/main
# Short form:
$ git branch -u origin/main

# Push and set upstream simultaneously (most common)
$ git push -u origin feature/login

# Once set: push and pull without arguments
$ git push
$ git pull

# git status shows the tracking relationship
$ git status
On branch feature/login
Your branch is ahead of 'origin/feature/login' by 3 commits.
  (use "git push" to publish your local commits)

# Check tracking for all branches
$ git branch -vv
* feature/login  a3f2d9c [origin/feature/login: ahead 3] Add login form
  main           7e1a4c9 [origin/main] Sync with upstream
---
status-upstream: 12-15
upstream-def: 6-7
```
