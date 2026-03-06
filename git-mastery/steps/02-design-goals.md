---
title: Design Goals
---

# Distributed

The first goal isn't a feature. {{show: distributed-backup slide 0.4s ease-out}} It's a philosophy. Every clone of a git repository is a complete repository. {{annotate: clone-a "full history"}} Not a working copy. Not a pointer to the server. The entire object store, all branches, all tags, all history.

This is called a distributed VCS, but that undersells it. {{focus: all-nodes}} Every one of these developers can commit, branch, merge, and log without a network connection. The server isn't privileged — it's just another clone that everyone's agreed to treat as canonical. {{annotate: server-node "just another clone"}} Pull the plug on the server and work continues.

The practical consequence is backup by default. {{focus: none}} Five developers means five full copies of the repository. You'd have to lose all five simultaneously to lose history. {{annotate: backup-label "5x redundancy"}} In a centralized VCS, one bad disk wipes the project. In git, that's just a bad day for one developer.

```diagram:distributed-backup
dev-a [developer]
dev-b [developer]
dev-c [developer]
dev-d [developer]
dev-e [developer]
server-node [origin-server]
dev-a --> server-node: push/pull
dev-b --> server-node: push/pull
dev-c --> server-node: push/pull
dev-d --> server-node: push/pull
dev-e --> server-node: push/pull
{Ring: dev-a, dev-b, dev-c, dev-d, dev-e}
---
all-nodes: dev-a, dev-b, dev-c, dev-d, dev-e
server-node: server-node
backup-label: dev-c
clone-a: dev-a
```

# Speed

{{clear: slide}}

Linus's benchmark was the Linux kernel. {{show: speed-comparison slide 0.4s ease-out}} Thousands of files, decades of history, hundreds of contributors. CVS took minutes for a full checkout. SVN was better but still slow. {{annotate: cvs-bar "minutes"}} Neither was built for a codebase at this scale.

The requirement was brutal and simple. {{focus: git-bar}} Every operation you do more than once a day must complete in under a second. Branch creation: instant. Status check: instant. Commit: instant. Log: instant. {{annotate: git-bar "under 1 second"}} If it's slow, developers work around it — and workarounds are where bugs hide.

The key architectural decision: operations happen locally. {{focus: none}} `git log` reads from your local object store, not a server. `git branch` writes a 41-byte file. `git diff` compares local objects. {{annotate: network-bar "no network needed"}} The only operations that touch the network are explicit push and fetch. Everything else: no round trip.

```chart:speed-comparison type=bar
CVS checkout (Linux kernel): 180
SVN checkout (Linux kernel): 45
git clone (Linux kernel): 8
git log (any size): 0.1
git branch: 0.01
---
highlight: git log (any size)
cvs-bar: CVS checkout (Linux kernel)
git-bar: git clone (Linux kernel)
network-bar: git log (any size)
```

# Integrity

{{clear: slide}}

Every object in git has a name: its SHA-1 hash. {{show: sha-demo typewriter 2s linear}} Not a sequence number, not a filename — the cryptographic hash of its content. {{annotate: hash-output "content-derived name"}} Change one byte in a file and its hash changes completely. Two identical files always get the same hash.

This is called content-addressing. {{focus: hash-output}} The name of a thing is derived from the thing itself. You cannot rename an object, because the name is the content. {{focus: none}} You cannot silently corrupt a file in the object store — on next read, git recomputes the hash and compares. Any mismatch is detected immediately.

The chain extends to commits. {{focus: sha-demo}} A commit's hash covers its content, its tree, and its parent commit hash. {{annotate: chain-label "hash over hash"}} This means the entire history is cryptographically chained. To alter a commit, you alter its hash — which breaks the child commit's hash — which breaks everything downstream. {{focus: none}} History is tamper-evident by construction.

```code:sha-demo lang=bash
# Hash of a string — same every time, everywhere
$ echo "hello" | sha1sum
f572d396fae9206628714fb2ce00f72e94f2258f

# git uses the same mechanism for file content
$ echo "hello" | git hash-object --stdin
8baef1b4abc478178b004d62031cf7fe6db6dcae

# git's hash includes the object type header
$ printf "blob 6\0hello\n" | sha1sum
8baef1b4abc478178b004d62031cf7fe6db6dcae
---
hash-output: 2-3
chain-label: 5-8
```

# Non-Goals

{{clear: slide}}

Git is a version control system. {{show: git-scope slide 0.4s ease-out}} That sounds obvious until you see what people try to use it for. Let's be precise about what git is and what it isn't.

Git is NOT a file sync tool. {{focus: not-column}} Dropbox syncs files automatically as you change them. Git does not. You decide when to commit. You decide what to include. {{annotate: sync-row "intentional, not automatic"}} This is a feature — accidental changes don't propagate.

Git is NOT a backup service. {{focus: not-column}} A backup runs on a schedule and captures everything. Git captures only what you explicitly stage and commit. {{annotate: backup-row "not scheduled, not automatic"}} If you never commit, git has nothing. Don't mistake a git remote for a backup.

Git is NOT GitHub. {{focus: none}} GitHub is a hosting service built on top of git. Pull requests, issues, Actions, the web UI — none of that is git. {{annotate: github-row "git is the protocol"}} Git predates GitHub by three years and will outlast it. The tool and the platform are different things.

```diagram:git-scope
is-vcs [git IS: version control]
is-history [git IS: history tracking]
is-branch [git IS: branching and merging]
is-distributed [git IS: distributed operation]
not-sync [git is NOT: file sync]
not-backup [git is NOT: backup service]
not-github [git is NOT: GitHub]
not-deploy [git is NOT: deployment tool]
{IsColumn: is-vcs, is-history, is-branch, is-distributed}
{NotColumn: not-sync, not-backup, not-github, not-deploy}
---
not-column: not-sync, not-backup, not-github, not-deploy
sync-row: not-sync
backup-row: not-backup
github-row: not-github
```

# The Name and the Attitude

{{clear: slide}}

Linus named every project after himself. {{show: linus-quote typewriter 1.5s linear}} Linux. Now git. "Git" is British slang for a contemptible person. {{annotate: name-line "intentionally unflattering"}} He's made no apologies for this.

The naming reflects the design philosophy. {{focus: philosophy-section}} Git is not trying to be approachable. It's trying to be correct, fast, and honest. {{annotate: philosophy-section "correctness over friendliness"}} The interface is famously rough. The model underneath is clean and precise. That's a deliberate trade-off — Linus optimized for the thing that outlasts the learning curve.

The original announcement email, April 7 2005. {{focus: none}} Subject: "git — the stupid content tracker." His words: "I'm doing it because I need it to do so." {{annotate: stupid-label "self-aware naming"}} Not because the world deserved a new VCS. Not because he had a vision for developer tooling. He needed a tool, he built it, he shipped it. That's the whole story.

Fifteen years later, 90% of developers use git. The stupid content tracker won.

```code:linus-quote lang=text
From: Linus Torvalds
Subject: [ANNOUNCE] GIT - the stupid content tracker
Date: Thu, 07 Apr 2005

It is designed to be "stupid" in the sense that it doesn't
try to be smart about the content it stores. It is designed
to track content, not files.

The name "git" was given by Linus Torvalds when he wrote the
very first version. He described the tool as "the stupid
content tracker" and said:

"I'm an egotistical bastard, and I name all my projects
after myself. First Linux, now git."
---
name-line: 1-2
philosophy-section: 5-8
stupid-label: 10-13
```
