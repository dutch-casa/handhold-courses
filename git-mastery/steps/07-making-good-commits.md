---
title: Making Good Commits
---

# git add — Specific Files vs git add .

`git add .` is a footgun. {{show: add-specific typewriter 1.5s linear}} It stages everything in the current directory and below: your changes, your debug logs, your half-baked experiments, your accidentally modified config file. {{annotate: danger-line "stages everything"}} You probably don't want all of that in the next commit.

Stage specific paths instead. {{focus: specific-files}} `git add src/auth.ts` stages one file. `git add src/auth.ts src/session.ts` stages two. {{annotate: specific-files "precise control"}} You know exactly what's going in. No surprises.

Path prefixes let you be surgical without being tedious. {{focus: prefix-line}} `git add src/` stages everything under `src/`. Still more intentional than `git add .` because you're naming a scope. {{annotate: prefix-line "directory prefix"}} Get comfortable with tab completion for filenames. It takes five seconds and prevents a whole category of bad commits.

The only time `git add .` is defensible: you've just initialized a repository and you genuinely want every file in the initial commit. {{focus: none}} After that? Use specific paths.

```code:add-specific lang=bash
# Footgun — stages everything, including junk
$ git add .

# Good — specific file
$ git add src/auth.ts

# Good — specific files
$ git add src/auth.ts src/session.ts package.json

# Good — specific directory
$ git add src/
---
danger-line: 1-2
specific-files: 4-9
prefix-line: 11-12
```

# git add -p — Staging Patches

{{clear: slide}}

Patch mode is the professional tool for precise staging. {{show: add-patch typewriter 2s linear}} Run `git add -p` and git walks through every changed hunk in your working directory, asking what to do with each one. {{annotate: hunk-display "one hunk at a time"}} You see exactly what changed before you decide to include it.

The prompt options are terse but learnable. {{focus: stage-prompt}} `y` stages the hunk. `n` skips it. `s` splits it into smaller hunks — useful when a hunk covers two unrelated changes. {{annotate: stage-prompt "y=yes n=no s=split"}} `e` opens your editor so you can manually pick individual lines within the hunk.

This workflow produces commits with one logical purpose each. {{focus: none}} If a reviewer checks out that commit in isolation, it does exactly one thing. {{annotate: split-prompt "break it up"}} Every commit in the history is meaningful. `git bisect` can find bugs efficiently. Rollbacks are safe. Archaeology is fast.

The habit to build: never `git add .` when `git add -p` would give you a cleaner result. {{focus: split-prompt}} The extra thirty seconds compounds. Your future self will read these commits. Make them worth reading.

```code:add-patch lang=bash
$ git add -p src/auth.ts
diff --git a/src/auth.ts b/src/auth.ts
@@ -14,6 +14,10 @@ export function login(
+  if (!user.trim()) {
+    throw new Error("username required");
+  }
Stage this hunk [y,n,q,a,d,s,e,?]? y

@@ -32,4 +36,6 @@
+  // TODO remove before merge
+  console.log("debug session:", session);
Stage this hunk [y,n,q,a,d,s,e,?]? n
---
hunk-display: 1-8
stage-prompt: 8
split-prompt: 11-13
```

# git commit -m vs Editor Commit

{{clear: slide}}

The `-m` flag is for simple commits. {{show: commit-structure typewriter 1.5s linear}} One-line message, one thought. `git commit -m "Fix null check in login"`. {{annotate: subject-line "50 chars max"}} If you can describe the change in under 50 characters, use `-m`.

The editor is for everything else. {{focus: body-section}} `git commit` with no flags opens `$EDITOR`. Write a subject line, leave a blank line, then write the body. {{annotate: blank-line "blank line is required"}} That blank line is not optional — git uses it to separate the subject from the body. `git log --oneline` shows only the subject. `git show` shows both.

The body is where the reasoning lives. {{focus: body-section}} Subject says what changed. Body says why. {{annotate: why-section "why, not what"}} "Prevents null pointer in session lookup when token expires before the cache clears." That's not obvious from the diff. It belongs in the commit.

The footer is for structured metadata. {{focus: footer-section}} Issue references, breaking change markers, co-authors. {{annotate: footer-section "refs and metadata"}} Tools like GitHub parse `Fixes: #482` and close the issue automatically when the commit lands on main. Use this.

```code:commit-structure lang=text
Fix null check in session lookup

Token expiry and cache TTL can race on slow networks.
When the token expires before the cache invalidates,
session lookup returns null and crashes the middleware.
Guard the return value before dereferencing.

Fixes: #482
Co-authored-by: Dave Chen <dave@example.com>
---
subject-line: 1
blank-line: 2
body-section: 3-7
why-section: 3-7
footer-section: 8-9
```

# The 7 Rules of a Great Commit Message

Seven rules. {{split}} {{clear: slide}} {{show: good-commit slide 0.3s ease-out}} {{show: bad-commit slide 0.3s ease-out}} Memorize them. Apply them every time. The discipline pays forward to everyone who reads your history.

One: imperative mood in the subject. {{focus: good-subject}} "Fix", "Add", "Remove" — not "Fixed", "Adding", "Removed". {{annotate: good-subject "imperative: Fix"}} Read it as completing the sentence "If applied, this commit will ___."

Two: 50 characters max in the subject. Three: blank line between subject and body. {{focus: bad-subject}} Four: wrap the body at 72 characters. {{annotate: bad-subject "past tense, too long"}} Five: explain why, not what — the diff shows what. Six: reference issues and PRs. Seven: one logical change per commit.

The bad example violates most of these. {{unsplit}} {{focus: none}} Past tense, no body, no issue reference, probably covers three different changes. {{annotate: bad-wrap "no wrapping"}} This is every junior engineer's commit history. Then they wonder why git bisect doesn't help them find bugs.

```code:good-commit lang=text
Fix null check in session lookup

Token expiry and cache TTL race on slow networks.
When the token expires before the cache flushes,
session lookup returns null and crashes middleware.
Guard the return value before dereferencing.

Fixes: #482
---
good-subject: 1
```

```code:bad-commit lang=text
fixed the bug where the session thing was broken and
also updated the readme and changed some other stuff
that dave mentioned in the meeting yesterday and
also bumped the version number to 1.2.3
---
bad-subject: 1-2
bad-wrap: 1-4
```

# git commit --amend — Fixing the Last Commit

{{clear: slide}}

Amend fixes mistakes in the last commit. {{show: amend-demo typewriter 1.5s linear}} Typo in the message, forgot to stage a file, left a debug log in. {{annotate: amend-command "--amend rewrites"}} `git commit --amend` reopens the commit — you can edit the message and the contents.

Stage what you need to add before running amend. {{focus: stage-fix}} If you forgot a file, `git add` it first. Then `git commit --amend`. {{annotate: stage-fix "add first, then amend"}} The amended commit replaces the old one with a new hash. The old commit is gone from the branch.

The hard rule: amend is only safe for local commits. {{focus: none}} If you've pushed the commit, amend produces a new hash that diverges from what your teammates have. {{annotate: local-only "never amend pushed commits"}} Force-pushing to fix it rewrites shared history. That breaks everyone's local clones. Use `git revert` instead.

```code:amend-demo lang=bash
# Typo in last commit message
$ git commit --amend -m "Fix null check in session lookup"

# Forgot to stage a file
$ git add src/session.ts
$ git commit --amend --no-edit

# Amend with editor open for message editing
$ git commit --amend
---
amend-command: 1-2
stage-fix: 4-6
local-only: 1-2
```
