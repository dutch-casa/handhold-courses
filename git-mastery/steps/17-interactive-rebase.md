---
title: Interactive Rebase
---

# The Problem — Real Commit History

Look at an honest feature branch history. {{show: messy-log typewriter 2s linear}} Not what you planned to write. What you actually wrote. {{annotate: noise-commits "your scratch pad, not history"}} "WIP". "added stuff". "fix test". "fix lint". "lol". "actually done". "typo". Seven commits that document your process, not your work.

```code:messy-log lang=bash
$ git log --oneline
a4b2c1d (HEAD -> feature/login) typo
9e8f7d6 actually done
3c2b1a0 lol
7f6e5d4 fix lint
2b1c0d9 fix test
8a7b6c5 added stuff
1d2e3f4 WIP: auth form
4c3b2a1 (origin/main) Add cache layer
---
noise-commits: 1-8
```

{{clear: slide}}

Nobody should see this. {{show: history-purpose slide 0.4s ease-out}} The commit log is communication. {{annotate: log-is-docs "history = documentation"}} It tells the next person — or you in six months — what changed, why, and in what logical units. "WIP" and "lol" communicate nothing. They are noise in a permanent record.

Before you push, clean this up. {{focus: log-is-docs}} Turn seven chaotic commits into one (or two, or three) coherent commits with real messages. {{annotate: before-push "clean before first push"}} The tool is interactive rebase. Nobody has your old SHAs. You haven't pushed yet. You can rewrite freely. {{focus: none}}

# git rebase -i

`git rebase -i HEAD~7` opens the todo file. {{show: rebase-i-cmd typewriter 1.5s linear}} The `~7` means "go back 7 commits from HEAD". {{annotate: head-tilde "HEAD~N = N commits back"}} Git writes a file listing those 7 commits in chronological order (oldest first — opposite of `git log`), opens it in your editor, and waits.

```code:rebase-i-cmd lang=bash
# Open interactive rebase for last 7 commits
$ git rebase -i HEAD~7
---
head-tilde: 1-2
```

{{clear: slide}}

The todo file. {{show: todo-file typewriter 2s linear}} Each line starts with a command word. {{annotate: pick-default "pick = keep as-is"}} Change the command word to change what happens to that commit. The commits are in chronological order — the first line is the oldest commit, the last line is the newest.

```code:todo-file lang=bash
# Rebase todo file — edit this, save, and close
pick 1d2e3f4 WIP: auth form
pick 8a7b6c5 added stuff
pick 2b1c0d9 fix test
pick 7f6e5d4 fix lint
pick 3c2b1a0 lol
pick 9e8f7d6 actually done
pick a4b2c1d typo

# Commands:
# p, pick   = use commit as-is
# r, reword = keep commit, edit the message
# e, edit   = stop here, amend the commit
# s, squash = merge into previous commit, combine messages
# f, fixup  = merge into previous commit, discard this message
# d, drop   = delete this commit entirely
---
pick-default: 1-8
```

{{clear: slide}}

The six action words. {{show: action-words slide 0.4s ease-out}} Learn them all. {{annotate: squash-action "squash = merge + combine messages"}} You'll use `fixup` and `squash` the most. `reword` next. `edit` and `drop` occasionally.

```diagram:action-words
pick-node [pick: keep commit]
reword-node [reword: keep, edit message]
edit-node [edit: stop to amend]
squash-node [squash: merge, combine messages]
fixup-node [fixup: merge, drop message]
drop-node [drop: delete commit]
{Common: pick-node, reword-node, squash-node, fixup-node}
{Occasional: edit-node, drop-node}
---
squash-action: squash-node
pick-default: pick-node
```

{{clear: slide}}

# Squash and Fixup in Action

Before: seven messy commits. {{show: squash-before typewriter 1.5s linear}} {{annotate: messy-state "7 commits of noise"}} After editing the todo file: one commit with a real message.

```code:squash-before lang=bash
# BEFORE: the todo file before you edit it
pick 1d2e3f4 WIP: auth form
pick 8a7b6c5 added stuff
pick 2b1c0d9 fix test
pick 7f6e5d4 fix lint
pick 3c2b1a0 lol
pick 9e8f7d6 actually done
pick a4b2c1d typo
---
messy-state: 1-7
```

{{clear: slide}}

Edit the todo file. {{show: squash-after typewriter 1.5s linear}} Change every line after the first from `pick` to `fixup`. {{annotate: fixup-lines "fixup discards the message"}} `fixup` merges each commit into the one above it and silently discards the commit message. The first commit's message is kept. Since that one says "WIP: auth form", you'll `reword` it too.

```code:squash-after lang=bash
# AFTER: the todo file you save
reword 1d2e3f4 WIP: auth form
fixup  8a7b6c5 added stuff
fixup  2b1c0d9 fix test
fixup  7f6e5d4 fix lint
fixup  3c2b1a0 lol
fixup  9e8f7d6 actually done
fixup  a4b2c1d typo
---
fixup-lines: 2-7
```

{{clear: slide}}

Git processes the todo. {{show: squash-result typewriter 1.5s linear}} It opens your editor to let you write the final commit message (because you used `reword`). {{annotate: clean-history "one commit, real message"}} You write: "Add login form with validation and session handling". Save. Close. Done.

```code:squash-result lang=bash
# RESULT: git log after the interactive rebase
$ git log --oneline
b5c4d3e (HEAD -> feature/login) Add login form with validation and session handling
4c3b2a1 (origin/main) Add cache layer

# Now push for the first time — clean history
$ git push -u origin feature/login
---
clean-history: 1-4
```

{{clear: slide}}

Squash vs fixup distinction. {{show: squash-vs-fixup typewriter 1.5s linear}} `squash` opens the editor and shows you the combined messages so you can write something from them. {{annotate: squash-prompts "squash opens editor"}} `fixup` silently discards the message and continues. Use `squash` when the discarded messages have content worth reading before you write the final message. Use `fixup` when they're pure noise.

```code:squash-vs-fixup lang=bash
# squash: opens editor with BOTH commit messages pre-populated
# You edit them together into one message
reword 1d2e3f4 WIP: auth form
squash 8a7b6c5 added form fields and validation logic

# fixup: silent, discards the message entirely
reword 1d2e3f4 WIP: auth form
fixup  8a7b6c5 ok i think i fixed the bug
---
squash-prompts: 2-3
```

{{clear: slide}}

# Reword and Edit

`reword` changes a message without touching the commit's content. {{show: reword-demo typewriter 1.5s linear}} Git replays the commit as-is, then stops and opens your editor with the current message. {{annotate: reword-use "fix message, keep content"}} You edit the message. Save. Git continues the rebase. One stopped step. No conflict risk.

```code:reword-demo lang=bash
# In the todo file:
reword 1d2e3f4 WIP: auth form
pick   2b1c0d9 add session integration
pick   7f6e5d4 add unit tests

# Git stops, opens editor with:
WIP: auth form

# You change it to:
Add login form with client-side validation

# Save and close — rebase continues automatically
---
reword-use: 1-3
```

{{clear: slide}}

`edit` is more powerful. {{show: edit-demo typewriter 1.5s linear}} Git stops at that commit and gives you a shell. {{annotate: edit-amend "edit → amend → continue"}} You can modify files, run `git commit --amend` to change the commit content and/or message, then `git rebase --continue` to proceed.

Use case: commit 3 of 7 has a bug you didn't notice until reviewing. {{focus: edit-amend}} Stop there, fix the code, amend, continue. {{annotate: bug-in-history "fix a bug in a past commit"}} The remaining commits replay on top of the fixed version. If the fix touches lines that later commits also touch, you may hit conflicts. {{focus: none}}

```code:edit-demo lang=bash
# In the todo file: stop at commit 3
pick   1d2e3f4 Add login form
pick   8a7b6c5 Add session integration
edit   2b1c0d9 Add validation   ← git stops here
pick   7f6e5d4 Add unit tests

# Git stops, gives you a shell. You fix the bug:
$ vim src/auth.ts            # fix the bug
$ git add src/auth.ts
$ git commit --amend --no-edit   # amend the stopped commit

# Continue the rebase
$ git rebase --continue

# Abort if something goes wrong
$ git rebase --abort
---
edit-amend: 1-3
bug-in-history: 5-9
```

{{clear: slide}}

# The drop Command and Reordering

`drop` removes a commit entirely. {{show: drop-demo typewriter 1.5s linear}} {{zoom: 1.2x}} The diff is gone. The changes in that commit do not appear in any subsequent commit. {{annotate: drop-warning "drop = changes disappear"}} Use it deliberately: if the commit introduced something you're removing entirely from this branch. {{zoom: 1x}}

```code:drop-demo lang=bash
# In the todo file: drop a debugging commit
pick   1d2e3f4 Add login form
pick   8a7b6c5 Add console.log debugging
drop   2b1c0d9 Temporary: disable auth for testing   ← deleted
pick   7f6e5d4 Add unit tests
pick   a4b2c1d Cleanup and final polish

# Result: that commit's changes never happened in history
# (make sure later commits don't depend on what you dropped)
---
drop-warning: 3
```

{{clear: slide}}

Reorder by moving lines. {{show: reorder-demo typewriter 1.5s linear}} {{zoom: 1.2x}} The order of lines in the todo file is the order commits will be replayed. {{annotate: reorder-risk "reorder = possible conflicts"}} Move a line up and that commit's changes land earlier. This can cause conflicts if the reordered commit touches files that the commits before it also touch — you'll need to resolve them. {{zoom: 1x}}

```code:reorder-demo lang=bash
# BEFORE: original order
pick 1d2e3f4 Add login form
pick 8a7b6c5 Add unit tests
pick 2b1c0d9 Update README

# AFTER: reordered (docs first, feature second)
pick 2b1c0d9 Update README
pick 1d2e3f4 Add login form
pick 8a7b6c5 Add unit tests

# Verify result after rebase
$ git log --oneline
2b1c0d9 Update README
1d2e3f4 Add login form
8a7b6c5 Add unit tests

# Verify content is correct
$ git diff HEAD~3
---
reorder-risk: 4-6
```

{{clear: slide}}

Verify after every interactive rebase. {{show: verify-after typewriter 1.5s linear}} `git log --oneline` first — confirm the commit count and messages look right. {{annotate: verify-habit "always verify after -i"}} Then `git diff <old-branch>` to confirm the final code matches what you intended. Nothing should be missing or duplicated. If something looks wrong, check reflog — the old commits are still there.

```code:verify-after lang=bash
# After interactive rebase: verify
$ git log --oneline
b5c4d3e (HEAD -> feature/login) Add login form with validation
4c3b2a1 (origin/main) Add cache layer

# Make sure no code was lost
$ git diff 4c3b2a1..HEAD --stat
 src/login.ts   | 67 +++++++++++++++++++++++++++++++
 src/session.ts | 23 +++++++++++
 src/auth.ts    |  8 ++++
 3 files changed, 98 insertions(+)

# If something is wrong: the old commits are in reflog
$ git reflog
b5c4d3e HEAD@{0}: rebase -i (finish): returning to refs/heads/feature/login
a4b2c1d HEAD@{1}: rebase -i (reword): WIP: auth form
...
1d2e3f4 HEAD@{7}: rebase -i (start): checkout HEAD~7
---
verify-habit: 1-2
```
