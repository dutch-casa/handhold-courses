---
title: Reading the Repository
---

# git status — The Five File States

Every file in your project is in exactly one state. {{show: file-states slide 0.4s ease-out}} Five possibilities. {{clear: slide}} {{show: status-output slide 0.4s ease-out}} Most developers know two or three of them and are surprised by the rest.

Untracked: git has never seen this file. {{focus: untracked-state}} It exists in your working directory and nowhere else. {{annotate: untracked-state "git ignores it"}} `git add` moves it out of untracked. Until then, commits can't include it.

Unmodified: the file is tracked and matches the last commit exactly. {{focus: unmodified-state}} Nothing to report. {{annotate: unmodified-state "clean"}} This is the quiet state — git status doesn't even mention these files by default.

Modified: tracked, but the working directory version differs from the index. {{focus: modified-state}} Not staged. {{annotate: modified-state "changed, not staged"}} Visible in `git diff`. Not visible in `git diff --staged`. Not going into the next commit.

Staged: the working directory change has been added to the index. {{focus: staged-state}} This version will be in the next commit. {{annotate: staged-state "will be committed"}} You can still modify the file again after staging — that creates both staged and unstaged changes simultaneously.

Deleted: the file has been removed. {{focus: deleted-state}} Staging the deletion with `git rm` or `git add` makes it staged-for-deletion. {{annotate: deleted-state "removal tracked too"}} An unstaged deletion shows as modified.

```diagram:file-states
untracked-state [untracked]
unmodified-state [unmodified]
modified-state [modified]
staged-state [staged]
deleted-state [deleted]
untracked-state --> staged-state: git add
staged-state --> unmodified-state: git commit
unmodified-state --> modified-state: edit file
modified-state --> staged-state: git add
unmodified-state --> deleted-state: delete file
deleted-state --> staged-state: git rm
---
untracked-state: untracked-state
unmodified-state: unmodified-state
modified-state: modified-state
staged-state: staged-state
deleted-state: deleted-state
```

```code:status-output lang=bash
$ git status
On branch main
Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
        new file:   src/feature.ts
        modified:   src/auth.ts

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
        modified:   README.md

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        src/scratch.ts
---
staged-output: 3-6
modified-output: 8-10
untracked-output: 12-14
```

# git diff — The Unified Diff Format

{{clear: slide}}

The unified diff format is the language of changes. {{show: diff-output slide 0.4s ease-out}} You'll see it constantly — in `git diff`, in pull requests, in code review tools. {{annotate: file-header "which files changed"}} It's worth reading fluently.

The file header tells you which files are involved. {{focus: file-header}} `---` is the old version, `+++` is the new version. {{annotate: file-header "a/ = old, b/ = new"}} For new files, the old version is `/dev/null`. For deleted files, the new version is `/dev/null`.

The hunk header marks a section of changes. {{focus: hunk-header}} The `@@` markers surround line position information. `@@ -14,6 +14,10 @@` means: in the old file, this hunk starts at line 14 and spans 6 lines; in the new file, it starts at line 14 and spans 10 lines. {{annotate: hunk-header "@@ = line context"}} The function name or context often appears after the closing `@@`.

Lines with `-` were removed. Lines with `+` were added. {{focus: removed-lines}} Context lines (no prefix) appear for orientation — they're unchanged. {{annotate: removed-lines "minus = deleted"}} {{annotate: added-lines "plus = added"}} Three context lines on each side of a hunk, by default.

```code:diff-output lang=diff
$ git diff src/auth.ts
diff --git a/src/auth.ts b/src/auth.ts
index 3b18e51..7f6db9c 100644
--- a/src/auth.ts
+++ b/src/auth.ts
@@ -14,6 +14,10 @@ export function login(
   const session = createSession();
   if (!session) return null;

+  if (!user.trim()) {
+    throw new Error("username required");
+  }
+
   return checkCredentials(user, pass);
 }

@@ -28,7 +32,6 @@ export function logout() {
   clearSession();
-  console.log("debug: logout");
   removeToken();
 }
---
file-header: 1-5
hunk-header: 6
added-lines: 9-12
removed-lines: 20
```

# git log — The Flags That Matter

{{clear: slide}}

Raw `git log` is verbose. {{show: log-commands typewriter 2s linear}} Commit hash, author, date, message — one commit fills the screen. You want `--oneline` for a quick scan, and then the flags that filter it to what you need. {{annotate: oneline-line "one line per commit"}} Most day-to-day use is `git log --oneline`.

`--graph` draws the branch structure in ASCII. {{focus: graph-line}} Indispensable for understanding a messy merge history. {{annotate: graph-line "branch topology"}} Combine it with `--oneline --all` and you see every branch's position relative to every other.

`--author` and `--since` filter by person and time. {{focus: filter-lines}} `--since="2 weeks ago"` uses human-readable dates. `--until` works the same way. {{annotate: filter-lines "scope your search"}} Combine with `--author="jane"` to find what a specific person shipped in a specific window.

`--follow` is critical for renamed files. {{focus: follow-line}} Without it, `git log src/auth.ts` stops at the rename — it won't show history from before the file was called `auth.ts`. {{annotate: follow-line "tracks renames"}} With `--follow`, the log continues through renames. Always use it when looking at file history.

```code:log-commands lang=bash
# Quick overview — one line per commit
$ git log --oneline -10
7fd1a60 Add input validation to login form
3e5a219 Refactor auth middleware
9b2c184 Initial commit

# Branch structure visualization
$ git log --oneline --graph --all

# Filter by author and time window
$ git log --oneline --author="jane" --since="2 weeks ago"

# File history that follows renames
$ git log --oneline --follow src/auth.ts

# Combine flags for a full picture
$ git log --oneline --graph --all --since="1 month ago"
---
oneline-line: 1-6
graph-line: 8-9
filter-lines: 11-12
follow-line: 14-15
```

# git show — Inspecting One Commit

{{clear: slide}}

`git show` is the inspector. {{show: show-output typewriter 1.5s linear}} Point it at any commit and you get the commit metadata followed by the diff it introduced — the full unified diff of every file changed. {{annotate: commit-meta "author, date, message"}} This is the most direct answer to "what exactly did this commit do?"

You can pass any ref, not just a hash. {{focus: commit-meta}} `git show HEAD` shows the latest commit. `git show HEAD~3` shows three commits back. `git show v1.0.0` shows the commit a tag points to. {{annotate: ref-flexibility "any ref works"}} Branch names, tags, relative refs — all valid.

`git show` with a path narrows the diff. {{focus: path-filter}} `git show HEAD -- src/auth.ts` shows only the changes to that one file in that one commit. {{annotate: path-filter "narrow to one file"}} Critical when a commit touches 20 files and you only care about one.

```code:show-output lang=bash
$ git show 7fd1a60
commit 7fd1a60b01f91b314f59955a4e4d4e80d8edf11d
Author: Jane Smith <jane@example.com>
Date:   Mon Apr 14 10:32:19 2025 -0700

    Add input validation to login form

    Prevents empty username submissions. Refs #482.

diff --git a/src/auth.ts b/src/auth.ts
index 3b18e51..7f6db9c 100644
--- a/src/auth.ts
+++ b/src/auth.ts
@@ -14,6 +14,10 @@ export function login(
+  if (!user.trim()) {
+    throw new Error("username required");
+  }
---
commit-meta: 1-7
ref-flexibility: 2
path-filter: 10-17
```

# git blame — Finding Who Wrote What

{{clear: slide}}

`git blame` annotates every line of a file. {{show: blame-output typewriter 1.5s linear}} Commit hash, author, date, line number, content — one line per line of the file. {{annotate: commit-col "which commit"}} Not to assign guilt — to find context. The question `git blame` answers is: when was this line introduced, and by whom, so you can find the original reasoning.

The commit hash is the starting point. {{focus: commit-col}} Copy it. Run `git show` on it. Read the commit message. {{annotate: author-col "who wrote it"}} Nine times out of ten, the commit message explains why the code looks the way it does. Context that would take thirty minutes to reverse-engineer from the code alone.

`-L` limits blame to a line range. {{focus: line-col}} `git blame -L 14,28 src/auth.ts` annotates only lines 14 through 28. {{annotate: line-col "source line"}} For large files, this keeps the output usable. Also works with function names: `git blame -L :login src/auth.ts` finds the function's definition and annotates it.

```code:blame-output lang=bash
$ git blame src/auth.ts
7fd1a60b (Jane Smith  2025-04-14 10:32 -0700 14) if (!user.trim()) {
7fd1a60b (Jane Smith  2025-04-14 10:32 -0700 15)   throw new Error("username required");
7fd1a60b (Jane Smith  2025-04-14 10:32 -0700 16) }
3e5a219c (Dave Chen   2025-03-28 09:14 -0700 17) const session = createSession();
9b2c184a (Jane Smith  2025-02-10 14:50 -0700 18) if (!session) return null;
9b2c184a (Jane Smith  2025-02-10 14:50 -0700 19) return checkCredentials(user, pass);
3e5a219c (Dave Chen   2025-03-28 09:14 -0700 20) }
---
commit-col: 1
author-col: 2
line-col: 5-7
```
