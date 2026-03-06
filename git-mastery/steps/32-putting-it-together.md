---
title: Putting It Together
---

# Step 1: The complete daily workflow

{{show: daily-loop grow 0.5s spring}}
This is what you do every day. The full loop.

```data:daily-workflow type=graph layout=tree
(start "Start of day: git pull") -> (branch "git checkout -b feat/my-thing") -> (code "write code") -> (stage "git add -p") -> (commit "git commit -m 'feat: ...'") -> (push "git push -u origin feat/my-thing") -> (pr "open PR") -> (ci "CI runs") -> (review "review + iterate") -> (merge "merge to main") -> (cleanup "git branch -d feat/my-thing") -> (pull "git pull") -> (start "Start of day: git pull")
^main=merge
---
pull-node: start
branch-node: branch
code-node: code
stage-node: stage
commit-node: commit
push-node: push
pr-node: pr
ci-node: ci
review-node: review
merge-node: merge
cleanup-node: cleanup
path: start, branch, code, stage, commit, push, pr, ci, review, merge, cleanup, pull
```

{{show: daily-workflow grow 0.5s spring}}
{{flow: path}}
{{focus: pull-node}}
{{annotate: pull-node "Always start fresh"}}
Start every day with `git pull`. You want to work on top of the latest changes, not yesterday's.

{{focus: stage-node}}
{{annotate: stage-node "git add -p for atomic commits"}}
Stage hunks, not files. This is how you make atomic commits from a messy working tree.

{{focus: ci-node}}
{{annotate: ci-node "CI is automatic"}}
CI runs automatically when you push. You don't trigger it — the push does.

{{focus: cleanup-node}}
{{annotate: cleanup-node "Delete merged branches"}}
Delete the branch after merge. It served its purpose. Stale branches are noise.

{{clear: slide}}

```code:daily-commands lang=bash
# Start of day
git pull

# New feature
git checkout -b feat/user-dashboard

# Work, then stage atomically
git add -p

# Commit with conventional commit format
git commit -m "feat(dashboard): add user retention chart"

# Push (first push sets upstream)
git push -u origin feat/user-dashboard

# Check status at any point
git status
git log --oneline --graph -10

# After PR merges: clean up
git checkout main
git pull
git branch -d feat/user-dashboard
---
pull: 1-2
branch: 4-5
stage: 7-8
commit: 10-11
push: 13-14
status: 16-17
cleanup: 19-22
```

{{show: daily-commands typewriter 2s linear}}
{{focus: pull}}
{{annotate: pull "Two characters, saves hours"}}
`git pull` at the start prevents merge conflicts before they happen.

{{zoom: 1.2x}}
{{focus: stage}}
{{annotate: stage "Never git add ."}}
`git add .` stages everything, including debug code, env files, and accidental changes. `git add -p` is surgical.

{{zoom: 1x}}
{{focus: status}}
{{annotate: status "Check orientation at any time"}}
When you're confused about where you are, `git status` and a short log reorient you in seconds.

---

# Step 2: Git aliases for daily use

{{clear: slide}}

```code:git-aliases lang=ini
[alias]
    # Pretty graph log
    lg = log --oneline --graph --decorate --all

    # Short status
    st = status --short --branch

    # Switch (modern checkout)
    sw = switch

    # Recent branches (last 10 touched)
    recent = branch --sort=-committerdate --format='%(refname:short)' -10

    # What am I about to push?
    outgoing = log --oneline @{u}..HEAD

    # What just came in on pull?
    incoming = log --oneline HEAD..@{u}

    # Undo last commit (keep changes staged)
    undo = reset --soft HEAD~1

    # Show stash list with context
    sl = stash list --date=local
---
lg: 2-3
st: 5-6
sw: 8-9
recent: 11-12
outgoing: 14-15
incoming: 17-18
undo: 20-21
sl: 23-24
```

{{show: git-aliases typewriter 2s linear}}
{{focus: lg}}
{{annotate: lg "The most useful alias"}}
`git lg` gives you the full DAG across all branches, compactly. This is the alias everyone should have.

{{zoom: 1.2x}}
{{focus: recent}}
{{annotate: recent "Find branches you worked on"}}
`git recent` lists the 10 branches you most recently committed to, sorted by last commit date. Faster than trying to remember branch names.

{{zoom: 1x}}
{{focus: outgoing}}
{{annotate: outgoing "Preview before push"}}
`git outgoing` shows commits on your branch that aren't on the remote yet. See what you're about to push.

{{focus: undo}}
{{annotate: undo "Soft undo — changes stay staged"}}
`git undo` removes the last commit but keeps the changes staged. Useful for rewriting a commit message or splitting a commit.

{{clear: slide}}

```code:use-aliases lang=bash
# Instead of:
git log --oneline --graph --decorate --all
# Type:
git lg

# Instead of:
git status --short --branch
# Type:
git st

# Instead of:
git branch --sort=-committerdate --format='%(refname:short)' -10
# Type:
git recent
---
before-lg: 1-2
after-lg: 3-4
before-st: 6-7
after-st: 8-9
before-recent: 11-12
after-recent: 13-14
```

{{show: use-aliases typewriter 2s linear}}
{{focus: after-lg}}
{{annotate: after-lg "Two keystrokes"}}
The full command is 44 characters. The alias is 6. You'll type `git lg` dozens of times a day.

{{focus: after-recent}}
{{annotate: after-recent "Never forget a branch name"}}
Branch names are hard to remember. `git recent` shows you what you worked on last. Essential on any project with many branches.

---

# Step 3: Critical gitconfig settings

{{clear: slide}}

```code:complete-gitconfig lang=ini
[user]
    name = Ada Lovelace
    email = ada@company.com

[core]
    editor = vim
    autocrlf = input        # Mac/Linux: LF only
    # autocrlf = true       # Windows: auto-convert
    pager = less -R

[pull]
    rebase = true           # rebase on pull, never merge commit

[merge]
    conflictstyle = diff3   # show base in conflict markers

[push]
    autoSetupRemote = true  # auto --set-upstream on first push

[rerere]
    enabled = true          # remember conflict resolutions

[diff]
    algorithm = histogram   # better diff algorithm for humans

[branch]
    sort = -committerdate   # git branch lists recently touched first

[alias]
    lg = log --oneline --graph --decorate --all
    st = status --short --branch
    sw = switch
    recent = branch --sort=-committerdate --format='%(refname:short)' -10
    outgoing = log --oneline @{u}..HEAD
    incoming = log --oneline HEAD..@{u}
    undo = reset --soft HEAD~1
    sl = stash list --date=local
---
user: 1-3
core: 5-9
pull: 11-12
merge: 14-15
push: 17-18
rerere: 20-21
diff: 23-24
branch: 26-27
aliases: 29-37
```

{{show: complete-gitconfig typewriter 2s linear}}
{{focus: pull}}
{{annotate: pull "Never see accidental merge commits"}}
Without `pull.rebase = true`, `git pull` when you have local commits creates a merge commit: "Merge branch 'main' of github.com/...". Nobody wants this.

{{zoom: 1.2x}}
{{focus: merge}}
{{annotate: merge "See the original in conflicts"}}
With `diff3`, conflict markers show three versions: yours, base (original), theirs. Resolving conflicts is dramatically easier.

{{zoom: 1x}}
{{focus: diff}}
{{annotate: diff "Better algorithm for humans"}}
Histogram diff produces more intuitive diffs for code. It's slower than the default but not perceptibly so on typical files.

{{focus: branch}}
{{annotate: branch "Most recent first"}}
`git branch` alphabetical order is useless. `sort = -committerdate` shows branches in the order you actually worked on them.

{{clear: slide}}

```code:conflict-diff3 lang=text
<<<<<<< HEAD (your version)
const result = sortStable(items);
||||||| base (the original)
const result = sort(items);
======= (their version)
const result = sortFast(items);
>>>>>>> feature/sort-optimization

# With diff3, you know:
# - You changed it to sortStable
# - They changed it to sortFast
# - The original was sort()
# - Resolution: probably sortStable (you had a reason)
---
yours: 2
base: 4
theirs: 6
analysis: 9-13
```

{{show: conflict-diff3 typewriter 2s linear}}
{{focus: base}}
{{annotate: base "This is what diff3 adds"}}
The middle section (between `|||||||` and `=======`) shows the original. Without it, you can only see two versions and have to guess what the original was.

{{focus: analysis}}
{{annotate: analysis "Informed decision"}}
With the original visible, you know both parties changed from `sort()`. You can reason about which change was more correct.

---

# Step 4: What you now know

{{clear: slide}}

```data:concept-map type=graph layout=tree
(objects "Git object model") -> (dag "Commit DAG") -> (branches "Branches as pointers") -> (remotes "Remotes and fetch/push") -> (workflows "Branching workflows") -> (github "GitHub collaboration layer") -> (cicd "CI/CD automation")
---
obj: objects
dag-node: dag
branch-node: branches
remote-node: remotes
workflow-node: workflows
gh-node: github
ci-node: cicd
path: objects, dag, branches, remotes, workflows, github, cicd
```

{{show: concept-map grow 0.5s spring}}
{{flow: path}}
{{focus: obj}}
{{annotate: obj "Layer 1: the foundation"}}
The object model — blobs, trees, commits, tags — is what git actually stores. Every command manipulates these objects.

{{focus: dag-node}}
{{annotate: dag-node "Layer 2: history structure"}}
Commits form a directed acyclic graph. Understanding the DAG makes rebase, merge, and cherry-pick obvious.

{{focus: branch-node}}
{{annotate: branch-node "Layer 3: navigation"}}
Branches are just named pointers to commits. HEAD is where you are. Knowing this demystifies detached HEAD and branch switching.

{{focus: gh-node}}
{{annotate: gh-node "Layer 4: collaboration"}}
GitHub adds the collaboration layer: PRs, issues, review, branch protection. None of it changes git — it builds on top.

{{focus: ci-node}}
{{annotate: ci-node "Layer 5: automation"}}
CI/CD automates quality gates and deployment. It runs git events — pushes, PRs — through automated workflows.

{{clear: slide}}

```data:plumbing-vs-porcelain type=graph layout=force
(plumbing "Plumbing commands") -- (porcelain "Porcelain commands")
(plumbing "Plumbing commands") -> (hash-object "git hash-object")
(plumbing "Plumbing commands") -> (cat-file "git cat-file")
(plumbing "Plumbing commands") -> (ls-files "git ls-files")
(plumbing "Plumbing commands") -> (update-index "git update-index")
(porcelain "Porcelain commands") -> (add "git add")
(porcelain "Porcelain commands") -> (commit "git commit")
(porcelain "Porcelain commands") -> (push "git push")
(porcelain "Porcelain commands") -> (log "git log")
---
plumb: plumbing
porch: porcelain
hobj: hash-object
add-node: add
```

{{show: plumbing-vs-porcelain slide 0.4s ease-out}}
{{focus: plumb}}
{{annotate: plumb "Raw object access"}}
Plumbing commands directly manipulate git's internals. `git cat-file -p <sha>` prints the content of any object. You rarely use these directly.

{{focus: porch}}
{{annotate: porch "Human-facing commands"}}
Porcelain commands are the ones you use daily. They call the plumbing underneath. Understanding both levels means you understand what your commands actually do.

---

# Step 5: Where to go from here

{{clear: slide}}

```code:resources lang=bash
# Git's own everyday guide
git help everyday

# Show the man page for any command
git help <command>
git help rebase

# The git object model, low-level
git cat-file -p HEAD
git cat-file -p HEAD^{tree}
git ls-files --stage
---
everyday: 1-2
man: 4-6
low-level: 8-10
```

{{show: resources typewriter 2s linear}}
{{focus: everyday}}
{{annotate: everyday "Curated by git itself"}}
`git help everyday` shows a curated list of commands that covers daily developer use. Written by the git project, not a tutorial.

{{zoom: 1.2x}}
{{focus: low-level}}
{{annotate: low-level "See the internals directly"}}
`git cat-file -p HEAD` prints the commit object for your current HEAD. Run it. See what git actually stores. The object model becomes concrete.

{{zoom: 1x}}
{{clear: slide}}

```data:learning-paths type=graph layout=tree
(now "You are here") -> (practice "Practice on real projects") -> (oss "Contribute to open source") -> (kernel "Read kernel dev practices")
(now "You are here") -> (git-book "The Git Book (git-scm.com/book)") -> (internals "Git internals chapter")
(now "You are here") -> (own-project "Your own daily workflow") -> (refine "Refine aliases and config")
---
start: now
prac: practice
oss-node: oss
book: git-book
own: own-project
```

{{show: learning-paths grow 0.5s spring}}
{{focus: prac}}
{{annotate: prac "The only way to learn git"}}
You cannot learn git by reading about it. You learn it by using it — making branches, creating conflicts, rebasing, bisecting real bugs.

{{focus: oss-node}}
{{annotate: oss-node "Best accelerator"}}
Contributing to an open source project teaches the fork workflow, proper commit messages, responding to reviews, and CI in one project. Find a project you use. Look for "good first issue."

{{focus: book}}
{{annotate: book "Free, official, comprehensive"}}
The Pro Git book (git-scm.com/book) is free online, written by the git project, and covers everything from beginner to internals. The internals chapter in particular is worth reading twice.

{{clear: slide}}

```code:final-advice lang=text
The single most useful habit:

  git log --oneline --graph --all

Run it on every project you work with.
Real graphs teach more than any diagram.
See merges, branches, tags, HEADs —
all the concepts in the actual history
of real code written by real teams.

The concepts you've learned become obvious
when you can see them in context.
---
command: 3
explanation: 5-11
```

{{show: final-advice typewriter 2s linear}}
{{zoom: 1.3x}}
{{focus: command}}
{{annotate: command "Run this everywhere"}}
This command is the best way to see all the concepts from this course in a real codebase. Commits, branches, merges, tags — all visible at once.

{{zoom: 1x}}
{{focus: explanation}}
{{annotate: explanation "Context beats theory"}}
A concept you've read about becomes obvious when you see it in a graph with 10,000 commits from a 50-person team. Seek that out.
