---
title: The Problem Git Solved
---

# The Disaster

Picture this. {{show: email-chaos slide 0.3s ease-out}} Two developers, one codebase, no coordination. Sarah emails Dave the file. Dave edits it. Sarah edits the same file. Dave emails back. {{annotate: conflict-arrow "file overwritten"}} Sarah opens it and her work is gone.

This wasn't rare. {{focus: email-chain}} This was Tuesday. Every Tuesday. For every team. The "solution" was a naming convention: `report-final.doc`, `report-final-v2.doc`, `report-FINAL-actually-final.doc`. {{annotate: lost-version "lost forever"}} You know how this ends.

The alternative was a shared network drive with a single file. {{focus: none}} One person edits at a time. Everyone else waits. This isn't collaboration — it's taking turns. {{clear: slide}} {{show: file-versions slide 0.4s ease-out}} The "history" was whatever files you remembered to copy before overwriting them.

```diagram:email-chaos
dev-a [developer]
dev-b [developer]
email-1 [email v1]
email-2 [email v2]
dev-a --> email-1: sends report.c
email-1 --> dev-b: receives
dev-b --> email-2: edits, sends back
email-2 --> dev-a: overwrites local
---
conflict-arrow: email-2
email-chain: email-1, email-2
lost-version: email-2
```

```data:file-versions type=linked-list
(v1 "report-v1.c") -> (v2 "report-v2.c") -> (v3 "report-final.c") -> (lost "??")
---
first: v1
last: lost
```

# Version Control History

{{clear: slide}}

The industry knew this was broken. {{show: centralized-vcs slide 0.4s ease-out}} SCCS in 1972 was the first real answer — it tracked changes to individual files on a single machine. RCS followed in 1982 with a better diffing algorithm. Both were single-machine tools. Fine for one person, useless for teams.

CVS arrived in 1986. {{focus: server-node}} It bolted a network layer onto RCS and gave teams a shared server. Real collaboration, finally. But look at what that server means. {{annotate: server-node "single point of failure"}} Every commit, every update, every branch operation requires a live connection to that one machine.

SVN cleaned up CVS's rough edges in 2000. {{focus: none}} Atomic commits, directory versioning, better branching. Still centralized. {{annotate: x-mark "everything stops"}} If the server goes down, nobody commits. If the server disk dies, the entire project history dies with it. Every egg, one basket.

The lock model made it worse. {{focus: client-nodes}} To edit a file, you check it out — locking it. Everyone else waits. If you go on vacation without checking it back in, your teammates are blocked until IT unlocks it. Version control that prevents work is a bad trade.

```diagram:centralized-vcs
server [vcs-server]
client-a [developer]
client-b [developer]
client-c [developer]
client-d [developer]
x-mark [failure]
client-a --> server: commit/update
client-b --> server: commit/update
client-c --> server: commit/update
client-d --> server: commit/update
server --> x-mark: disk fails
---
server-node: server
client-nodes: client-a, client-b, client-c, client-d
x-mark: x-mark
```

# Enter Distributed VCS

{{clear: slide}}

By 2000, BitKeeper emerged with a different model. {{show: distributed-vcs slide 0.4s ease-out}} Every developer has the full repository. Not a working copy — the entire history, every branch, every commit. {{annotate: full-copy "full history"}} You commit locally. You push when you want. No server required for the daily workflow.

This changes everything about failure modes. {{focus: any-node}} If any node disappears, every other node has the complete history. {{focus: none}} The server is just another clone — a convenient meeting point, not the source of truth.

{{clear: slide}}

Linus Torvalds was maintaining the Linux kernel. {{show: linux-scale-note slide 0.3s ease-out}} At its peak, the kernel received thousands of patches per week from hundreds of contributors worldwide. CVS was out of the question — it couldn't handle the volume or the distributed contributor model. SVN wasn't fundamentally different. He needed something built for this scale.

```diagram:distributed-vcs
dev-a [developer]
dev-b [developer]
dev-c [developer]
dev-d [developer]
dev-e [developer]
dev-a -- dev-b: sync
dev-b -- dev-c: sync
dev-c -- dev-d: sync
dev-d -- dev-e: sync
dev-e -- dev-a: sync
{Ring: dev-a, dev-b, dev-c, dev-d, dev-e}
---
any-node: dev-a
full-copy: dev-b
```

# BitKeeper and the License Bomb

{{clear: slide}}

From 2002 to 2005, the Linux kernel used BitKeeper. {{show: bitkeeper-timeline slide 0.4s ease-out}} Larry McVoy, BitKeeper's creator, offered free licenses to open-source projects. It wasn't open source itself — you agreed not to reverse-engineer the protocol.

This arrangement worked until it didn't. {{focus: tridgell-node}} Andrew Tridgell, of Samba fame, started writing a tool that could talk to BitKeeper's servers. His goal was interoperability. McVoy's view: this violated the license agreement. {{annotate: license-node "terms violated"}} In April 2005, McVoy revoked the free licenses for the Linux kernel project.

The community was left without a version control system. {{focus: none}} Linus had three options: pay for BitKeeper, adopt an existing free alternative, or write something new. {{annotate: revocation-node "no more free licenses"}} Subversion was too slow and too centralized. No existing tool could handle the kernel's scale.

Option three it was. {{focus: git-start-node}} Linus had strong opinions about what the next tool needed to be. And he had just been handed free time he hadn't planned for.

```diagram:bitkeeper-timeline
bk-license [free-license]
kernel-use [linux-kernel]
tridgell-node [tridgell-tool]
license-node [license-terms]
revocation-node [revocation]
git-start-node [git-begins]
bk-license --> kernel-use: 2002, kernel adopts BitKeeper
kernel-use --> tridgell-node: 2005, Tridgell's client
tridgell-node --> license-node: McVoy objects
license-node --> revocation-node: April 2005, revoked
revocation-node --> git-start-node: Linus starts git
---
tridgell-node: tridgell-node
license-node: license-node
revocation-node: revocation-node
git-start-node: git-start-node
```

# Ten Days, One Person, Git

{{clear: slide}}

April 3, 2005. {{show: git-milestones slide 0.4s ease-out}} Linus starts writing git. April 7 — four days later — git is self-hosting. He's using git to develop git. April 29: the first Linux kernel commit using git, with 6.7 patches merged per second during the import. {{annotate: speed-bar "6.7 patches/sec"}} BitKeeper managed about 2. Linus shipped something faster than what he replaced.

The name. {{focus: quote-section}} Linus: "I'm an egotistical bastard, and I name all my projects after myself. First Linux, now git." {{annotate: quote-section "his words, not mine"}} The British slang: "git" means a stupid or unpleasant person. He was making a point about tool design by naming it after its author's worst qualities.

The design goals were set before a line of code. {{focus: none}} Not a better CVS. Not SVN with a network card. Something fundamentally different — a content-addressed object store with DAG history and distributed operation as a first-class property. {{annotate: milestones-bar "10 days to self-host"}} The first commit message: "Initial revision of 'git', the information manager from hell."

Ten days. One person. The tool that every developer on Earth uses today.

```chart:git-milestones type=bar
April 3 - First commit: 1
April 7 - Self-hosting: 4
April 29 - Kernel import: 26
Patches per second (BitKeeper): 2
Patches per second (git): 6.7
---
highlight: April 29 - Kernel import
speed-bar: Patches per second (git)
milestones-bar: April 7 - Self-hosting
quote-section: April 3 - First commit
```
