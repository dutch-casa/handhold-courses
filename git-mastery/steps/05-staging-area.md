---
title: The Staging Area
---

# Three Zones

Git tracks your work in three distinct places. {{show: three-zones slide 0.4s ease-out}} Working directory, staging area, repository. Most version control systems have two zones. Git's third zone is the most misunderstood thing in the entire tool. {{annotate: staging-box "the index"}} Every confusing git moment you've had probably traces back here.

The working directory is your filesystem. {{focus: working-box}} Regular files. You edit them with any tool you like. Git watches but doesn't record. {{annotate: working-box "your normal files"}} Nothing that happens here is tracked until you explicitly tell git about it.

The staging area is also called the index. {{focus: staging-box}} It lives in `.git/index`. It's a binary file that records exactly which version of each file you've chosen to include in your next commit. {{annotate: index-label ".git/index"}} Not your current files — the files as they existed when you ran `git add`.

The repository is the object store. {{focus: repo-box}} Everything that's been committed lives here as immutable objects. {{annotate: repo-box "permanent snapshots"}} SHA hashes, trees, blobs. Nothing in the repository can be changed without producing new objects with new hashes.

```diagram:three-zones
working-box [working directory]
staging-box [staging area / index]
repo-box [repository]
working-box --> staging-box: git add
staging-box --> repo-box: git commit
repo-box --> working-box: git checkout
---
working-box: working-box
staging-box: staging-box
repo-box: repo-box
index-label: staging-box
```

# What git add Does

{{clear: slide}}

`git add` moves content from your working directory into the index. {{show: add-demo typewriter 2s linear}} Not a reference to the file — the actual content, hashed and stored as a blob object. {{annotate: before-add "changes not staged"}} The index is updated to point to this new blob.

Run `git status` before and after. {{focus: before-section}} Before `git add`: the file shows up under "Changes not staged for commit." Git can see that your working file differs from the index. {{focus: after-section}} After `git add`: it moves to "Changes to be committed." The index now matches your working file. {{annotate: after-section "index updated"}} The working file hasn't moved — only the index changed.

This means you can stage partial work. {{focus: none}} Edit a file. Stage it. Edit it more. {{annotate: partial-note "snapshot at add time"}} The index holds the version as of your last `git add`. The working directory has your newer edits. Git status will show both situations simultaneously — staged changes and unstaged changes on the same file.

```code:add-demo lang=bash
# Start: modified file, nothing staged
$ git status
On branch main
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
        modified:   src/auth.ts

# Stage the file
$ git add src/auth.ts

# After: file is staged
$ git status
On branch main
Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
        modified:   src/auth.ts
---
before-add: 2-6
before-section: 3-6
after-section: 10-14
partial-note: 8
```

# What git commit Does

{{clear: slide}}

`git commit` takes a snapshot of the index. {{show: commit-demo typewriter 1.5s linear}} Not your working directory — the index. {{annotate: index-snapshot "index state at commit time"}} It builds a tree object from the index, creates a commit object pointing to that tree, and advances the current branch to the new commit.

The working directory is irrelevant to a commit. {{focus: status-clean}} You could have uncommitted edits in your working directory and that doesn't affect the commit at all. {{annotate: status-clean "clean = index matches last commit"}} What gets committed is whatever was in the index at the time you ran `git commit`. Nothing more, nothing less.

This three-zone model gives you precise control. {{focus: none}} Stage exactly the changes you want. {{annotate: new-commit-line "new SHA in log"}} Review what's staged. Then commit. The commit reflects your intention, not an accidental snapshot of whatever happened to be in your working directory at the time.

```code:commit-demo lang=bash
# After staging, commit
$ git commit -m "Add input validation to login form"
[main 7fd1a60] Add input validation to login form
 1 file changed, 12 insertions(+), 2 deletions(-)

# Working directory status is clean
$ git status
On branch main
nothing to commit, working tree clean

# The commit appears in the log
$ git log --oneline -3
7fd1a60 Add input validation to login form
3e5a219 Refactor auth middleware
9b2c184 Initial commit
---
index-snapshot: 1-4
status-clean: 6-9
new-commit-line: 11-14
```

# Why the Staging Area Exists

{{clear: slide}}

Linus needed to compose commits from a giant working tree. {{show: add-patch typewriter 2s linear}} The Linux kernel has tens of thousands of files. A single session of work might touch files related to three different fixes. {{annotate: hunk-prompt "choose per hunk"}} The staging area lets him group those changes into coherent commits — one per fix — even though all the edits happened at the same time.

`git add -p` is where this shines. {{focus: hunk-prompt}} Patch mode walks through each changed hunk in your working directory and asks: stage this? {{annotate: yes-no-options "y/n/s/e/q"}} `y` stages it, `n` skips it, `s` splits the hunk into smaller pieces, `e` opens an editor to stage individual lines.

This is how senior engineers commit. {{focus: none}} Not `git add .`, which stages everything indiscriminately. {{annotate: split-option "s = split hunk"}} Patch mode, one logical change per commit, every commit reviewable in isolation. The extra thirty seconds per commit saves hours in code review and archaeology later.

```code:add-patch lang=bash
$ git add -p src/auth.ts
diff --git a/src/auth.ts b/src/auth.ts
@@ -14,6 +14,10 @@ export function login(user: string, pass: string) {
+  if (!user.trim()) {
+    throw new Error("username required");
+  }
Stage this hunk [y,n,q,a,d,s,e,?]? y

@@ -28,4 +32,2 @@ export function logout() {
-  console.log("debug: logout");
Stage this hunk [y,n,q,a,d,s,e,?]? n
---
hunk-prompt: 5-8
yes-no-options: 8
split-option: 11-13
```

# git diff vs git diff --staged

Two comparisons, two different pairs of zones. {{split}} {{clear: slide}} {{show: diff-unstaged slide 0.3s ease-out}} {{show: diff-staged slide 0.3s ease-out}} `git diff` with no flags compares your working directory to the index. {{annotate: unstaged-header "working dir vs index"}} `git diff --staged` compares the index to the last commit.

Think about what this means when you partially stage a file. {{focus: unstaged-header}} `git diff` shows you the changes you haven't staged yet. {{focus: staged-header}} `git diff --staged` shows you what will be committed. {{annotate: staged-header "index vs HEAD"}} These can be completely different hunks from the same file.

A clean workflow always checks `git diff --staged` before committing. {{unsplit}} {{focus: none}} You are verifying that the index contains exactly what you intend to commit. {{annotate: review-note "review before commit"}} No surprises, no accidentally included debug logs, no half-baked changes.

```code:diff-unstaged lang=diff
$ git diff src/auth.ts
diff --git a/src/auth.ts b/src/auth.ts
index e69b633..7f6db9c 100644
--- a/src/auth.ts
+++ b/src/auth.ts
@@ -32,4 +32,6 @@
 function logout() {
+  // TODO: remove this debug log
+  console.log("debug: logout called");
   clearSession();
 }
---
unstaged-header: 1
```

```code:diff-staged lang=diff
$ git diff --staged src/auth.ts
diff --git a/src/auth.ts b/src/auth.ts
index 3b18e51..e69b633 100644
--- a/src/auth.ts
+++ b/src/auth.ts
@@ -14,6 +14,10 @@
 export function login(user: string, pass: string) {
+  if (!user.trim()) {
+    throw new Error("username required");
+  }
   return checkCredentials(user, pass);
 }
---
staged-header: 1
review-note: 5-12
```
