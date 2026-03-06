---
title: Log Archaeology
---

# Step 1: Searching diffs — the pickaxe

{{show: pickaxe-intro grow 0.5s spring}}
A function disappeared. You don't know when. Git does.

```code:pickaxe-s lang=bash
git log -S "processPayment"
# commit 8a4f2c1
# Author: Ben Shapiro <ben@company.com>
# Date:   Tue Mar 4 09:12:44 2025
#
#     refactor: extract payment module to separate service
#
# commit 3d7a9b2
# Author: Clara Oswald <clara@company.com>
# Date:   Wed Jan 15 14:30:22 2025
#
#     feat: add payment processing
---
cmd: 1
commit-1: 2-7
commit-2: 9-14
```

{{show: pickaxe-s typewriter 2s linear}}
{{focus: cmd}}
{{annotate: cmd "Find where count changed"}}
`git log -S` finds commits where the number of occurrences of that string changed — it appeared for the first time, or was removed entirely.

{{zoom: 1.2x}}
{{focus: commit-2}}
{{annotate: commit-2 "First appearance"}}
This is when `processPayment` was introduced — the count went from 0 to something.

{{zoom: 1x}}
{{focus: commit-1}}
{{annotate: commit-1 "Last appearance or move"}}
This is when it was refactored away or moved — the count dropped.

{{clear: slide}}

```code:pickaxe-g lang=bash
# -S: count changed (added or removed the string entirely)
git log -S "processPayment"

# -G: pattern appears ANYWHERE in the diff
git log -G "process.*Payment"

# -G catches: changes TO lines containing the pattern
# Even if the function was only modified, not added/removed
---
s-flag: 1-2
g-flag: 4-5
g-note: 7-8
```

{{show: pickaxe-g typewriter 2s linear}}
{{focus: s-flag}}
{{annotate: s-flag "Count of occurrences changed"}}
`-S` is precise: the string was added or removed. It catches the introduction and deletion of a function.

{{focus: g-flag}}
{{annotate: g-flag "Regex in the diff"}}
`-G` is broader: the regex appears anywhere in the added or removed lines of any commit's diff.

{{zoom: 1.2x}}
{{focus: g-note}}
{{annotate: g-note "Catches modifications too"}}
If a function signature changed but wasn't added or removed, `-S` misses it. `-G` catches it.

{{zoom: 1x}}
{{clear: slide}}

```data:s-vs-g type=array
["git log -S │ added: ✓ found │ removed: ✓ found │ modified: ✗ MISSED", "git log -G │ added: ✓ found │ removed: ✓ found │ modified: ✓ found"]
---
s-node: 0
g-node: 1
miss: 0
hit: 1
```

{{show: s-vs-g grow 0.5s spring}}
{{focus: s-node}}
{{annotate: s-node "Precise — count changed"}}
Use `-S` when you want to find where a function first appeared or was deleted.

{{focus: g-node}}
{{annotate: g-node "Broader — any diff match"}}
Use `-G` when the code was modified but not added or removed entirely.

{{focus: miss}}
{{annotate: miss "Only -S misses this"}}
A function that had its signature changed — same name, different args — `-S` misses because the count didn't change to zero.

{{focus: hit}}
{{annotate: hit "-G catches everything"}}
`-G` catches any line in the diff that matches the pattern.

---

# Step 2: Following a file through renames

{{clear: slide}}

```code:follow-rename lang=bash
# History stops at the rename — wrong
git log -- src/utils/old-name.js
# commit abc1234
# feat: move utils to dedicated module    <-- last entry, git stops here

# Follows through renames — correct
git log --follow -- src/utils/new-name.js
# commit abc1234
# feat: move utils to dedicated module
# commit def5678
# fix: handle edge case in formatter
# commit ghi9012
# feat: add string formatter utility     <-- original creation
---
without-follow: 1-4
with-follow: 7-13
```

{{show: follow-rename typewriter 2s linear}}
{{focus: without-follow}}
{{annotate: without-follow "History cuts off at rename"}}
Without `--follow`, git looks for the exact filename. When the file was renamed, the history stops. You see only post-rename commits.

{{zoom: 1.2x}}
{{focus: with-follow}}
{{annotate: with-follow "Full history across renames"}}
`--follow` traces the file backwards through renames. You see the complete history from creation.

{{zoom: 1x}}
{{clear: slide}}

```code:find-renames lang=bash
# Find every rename in history
git log --diff-filter=R --summary
# commit 8f3a2b1
# Author: Ada Lovelace <ada@example.com>
#  rename src/utils/{old-name.js => new-name.js} (94%)
#
# commit 4c2d9e0
# Author: Ada Lovelace <ada@example.com>
#  rename src/{helpers.ts => utils/helpers.ts} (87%)
---
cmd: 1
output: 2-9
```

{{show: find-renames typewriter 2s linear}}
{{focus: cmd}}
{{annotate: cmd "Find all renames ever"}}
`--diff-filter=R` shows only commits that included a rename. `--summary` shows the rename details.

{{zoom: 1.2x}}
{{focus: output}}
{{annotate: output "Similarity score in parens"}}
The percentage shows how similar the old and new files are. 94% means nearly identical — almost certainly a rename, not a rewrite.

{{zoom: 1x}}
{{clear: slide}}

```data:rename-history type=graph layout=tree
(creation "feat: add string formatter") -> (edits "fix: handle edge case") -> (rename "feat: move utils") -> (more-edits "refactor: simplify API") -> (now "current HEAD")
^main=now
---
create: creation
edit: edits
rename-point: rename
after: more-edits
head: now
path: creation, edits, rename, more-edits, now
```

{{show: rename-history grow 0.5s spring}}
{{flow: path}}
{{focus: rename-point}}
{{annotate: rename-point "Without --follow, git stops here"}}
The rename is a wall in the history. Without `--follow`, you never see what came before it.

{{focus: create}}
{{annotate: create "Only visible with --follow"}}
The original creation commit is invisible unless you follow the rename chain backward.

{{focus: head}}
{{annotate: head "Where you start searching"}}
You always search backward from HEAD. `--follow` keeps the chain intact.

---

# Step 3: Advanced log flags

{{clear: slide}}

```code:log-flags lang=bash
# Filter by author
git log --author="Ada"

# Filter by date
git log --since="2 weeks ago"
git log --after="2025-01-01" --before="2025-03-01"

# Filter by commit message content
git log --grep="fix"

# Combine filters (all conditions must match)
git log --author="Ada" --since="1 month ago" --grep="fix"

# The full picture across ALL branches
git log --all --graph --decorate --oneline
---
author: 1-2
date: 4-6
grep: 8-9
combined: 11-12
all-graph: 14-15
```

{{show: log-flags typewriter 2s linear}}
{{focus: author}}
{{annotate: author "Substring or regex match"}}
`--author` does substring matching on the name and email. Partial names work.

{{focus: date}}
{{annotate: date "Human-readable dates"}}
Git accepts natural language dates. "2 weeks ago", "yesterday", ISO dates — all valid.

{{focus: grep}}
{{annotate: grep "Commit message search"}}
`--grep` matches against the commit message, not the diff. For diff searching, use `-S` or `-G`.

{{zoom: 1.2x}}
{{focus: combined}}
{{annotate: combined "All filters must match"}}
Multiple flags are AND conditions. This finds Ada's bug fixes from the last month.

{{zoom: 1x}}
{{focus: all-graph}}
{{annotate: all-graph "Best overview command"}}
This combination is the single most useful log alias. Memorize it or put it in your gitconfig.

{{clear: slide}}

```code:graph-output lang=bash
git log --all --graph --decorate --oneline
# * 8f3a2b1 (HEAD -> main) feat: add dashboard
# * 4d2c9e0 fix: correct date parsing
# | * 3a1b9f0 (feature/auth) feat: OAuth2 integration
# | * 7c4d2e1 fix: token expiry handling
# |/
# * 2b8a0f3 (tag: v1.2.0) chore: release v1.2.0
# *   1c5f9d2 Merge branch 'feature/payments'
# |\
# | * 9e3a7b1 feat: add payment processing
# | * 6f2c4d0 feat: payment models
# |/
# * 5d1b8e3 feat: initial commit
---
head-line: 2
current-branch: 2
feature-branch: 3-5
merge-node: 6-11
init: 12
```

{{show: graph-output typewriter 2s linear}}
{{zoom: 1.3x}}
{{focus: head-line}}
{{annotate: head-line "Your current position"}}
`(HEAD -> main)` tells you where you are in the graph.

{{focus: current-branch}}
{{annotate: current-branch "Branch label"}}
Branch names appear in parentheses next to the commit they point to.

{{focus: feature-branch}}
{{annotate: feature-branch "Parallel work"}}
The `|` and `|/` characters show branching and merging. This is the ASCII art DAG.

{{focus: merge-node}}
{{annotate: merge-node "Merge commit shape"}}
`|\` indicates a merge commit — two parents joined into one.

{{zoom: 1x}}

---

# Step 4: git shortlog

{{clear: slide}}

```code:shortlog lang=bash
# Contributor stats
git shortlog -s -n
#   342  Ada Lovelace
#   287  Ben Shapiro
#    94  Clara Oswald
#    12  Dana Kim

# Recent activity only
git shortlog -s -n --since="1 year ago"
#   198  Ada Lovelace
#   156  Ben Shapiro
#    43  Clara Oswald

# Last 30 commits by author
git shortlog --group=author HEAD~30..HEAD
# Ada Lovelace (14):
#       feat: add dashboard component
#       fix: correct date parsing
#       ...
# Ben Shapiro (9):
#       refactor: extract auth service
#       ...
---
basic: 1-5
recent: 7-11
range: 13-22
```

{{show: shortlog typewriter 2s linear}}
{{focus: basic}}
{{annotate: basic "Commit count per author"}}
`-s` suppresses commit messages (just counts), `-n` sorts by count descending.

{{zoom: 1.2x}}
{{focus: recent}}
{{annotate: recent "Who is still active"}}
The full count is misleading for old projects. `--since` shows you who's been contributing recently.

{{zoom: 1x}}
{{focus: range}}
{{annotate: range "Last N commits breakdown"}}
`HEAD~30..HEAD` limits to the last 30 commits. Shows how recent work is distributed.

{{clear: slide}}

```data:contributor-graph type=array
["Ada   │ 342 total │ 198 recent │ primary active author", "Ben   │ 287 total │ 156 recent │ active", "Clara │  94 total │  43 recent │ active", "Dana  │  12 total │   0 recent │ inactive"]
---
full: 0
recent-ada: 0
inactive: 3
ada-row: 0
```

{{show: contributor-graph grow 0.5s spring}}
{{focus: full}}
{{annotate: full "Total commits, all time"}}
Raw totals don't tell you about current ownership.

{{flow: ada-row}}
{{focus: recent-ada}}
{{annotate: recent-ada "Still very active"}}
Ada has 198 of her 342 commits in the last year. She's the current primary author.

{{focus: inactive}}
{{annotate: inactive "No recent activity"}}
Dana committed 12 times but not recently. Don't page Dana for code questions.

---

# Step 5: git blame

{{clear: slide}}

```code:blame-output lang=bash
git blame src/payment/processor.ts
# 8a4f2c1 (Ada Lovelace  2025-03-04 09:12) export function processPayment(
# 8a4f2c1 (Ada Lovelace  2025-03-04 09:12)   amount: number,
# 8a4f2c1 (Ada Lovelace  2025-03-04 09:12)   currency: string,
# 3d7a9b2 (Ben Shapiro   2025-01-15 14:30)   options: PaymentOptions
# 8a4f2c1 (Ada Lovelace  2025-03-04 09:12) ): Promise<PaymentResult> {
# f2c4d890 (Clara Oswald 2025-02-20 11:45)   const validated = validate(amount);
# 3d7a9b2 (Ben Shapiro   2025-01-15 14:30)   if (!validated) throw new PaymentError();
# 8a4f2c1 (Ada Lovelace  2025-03-04 09:12)   return gateway.charge(amount, currency);
# 8a4f2c1 (Ada Lovelace  2025-03-04 09:12) }
---
header: 1
ada-lines: 2-4
ben-line: 5
ada-sig: 6
clara-line: 7
ben-throw: 8
ada-body: 9-10
```

{{show: blame-output typewriter 2s linear}}
{{zoom: 1.3x}}
{{focus: header}}
{{annotate: header "Four columns per line"}}
Each line shows: commit SHA, author name, date, and the actual code. Everything you need to investigate.

{{focus: ada-lines}}
{{annotate: ada-lines "Ada wrote the function signature"}}
All these lines have the same SHA — same commit, same author.

{{focus: ben-line}}
{{annotate: ben-line "Ben added this option"}}
Different SHA means different commit. Ben added the `options` parameter separately.

{{zoom: 1x}}
{{focus: clara-line}}
{{annotate: clara-line "Clara added validation"}}
Clara's commit added the validation line. If validation is broken, Clara's commit is where to look.

{{clear: slide}}

```code:blame-flags lang=bash
# Blame only specific lines
git blame -L 10,30 src/payment/processor.ts

# Ignore whitespace-only changes
git blame -w src/payment/processor.ts

# Detect code moved from other files in same commit
git blame -C src/payment/processor.ts

# Detect code copied from anywhere in history
git blame -CCC src/payment/processor.ts

# Ignore recent commits (find original author)
git blame --since="3 months ago" src/payment/processor.ts
---
line-range: 1-2
whitespace: 4-5
copy-detect: 7-8
deep-copy: 10-11
since: 13-14
```

{{show: blame-flags typewriter 2s linear}}
{{focus: line-range}}
{{annotate: line-range "Don't blame the whole file"}}
`-L` limits to a line range. Running blame on a 2000-line file is noisy. Use it on the 10 lines you care about.

{{focus: whitespace}}
{{annotate: whitespace "Skip reformatting commits"}}
`-w` ignores commits that only changed whitespace — reformatters, autoformatters. Find the real author.

{{zoom: 1.2x}}
{{focus: copy-detect}}
{{annotate: copy-detect "Find where code came from"}}
`-C` detects code that was copied or moved from another file in the same commit. Without it, blame says the person who moved it wrote it.

{{zoom: 1x}}
{{focus: since}}
{{annotate: since "Ignore recent churn"}}
`--since` ignores commits newer than the cutoff. Useful on files with lots of reformatting — find the original author, not the last person who ran prettier.

{{clear: slide}}

```data:blame-workflow type=graph layout=tree
(bug "Bug in line 47") -> (blame "git blame -L 45,50") -> (commit "Found commit f2c4d890") -> (show "git show f2c4d890") -> (context "Full diff with context") -> (root-cause "Found root cause")
---
start: bug
cmd: blame
found: commit
show-step: show
ctx: context
end: root-cause
path: bug, blame, commit, show, context, root-cause
```

{{show: blame-workflow grow 0.5s spring}}
{{flow: path}}
{{focus: start}}
{{annotate: start "You found a bad line"}}
Start with the file and the suspicious line.

{{focus: cmd}}
{{annotate: cmd "Who wrote it, which commit"}}
Blame gives you the commit SHA. One command.

{{focus: show-step}}
{{annotate: show-step "Full picture of that change"}}
`git show` on that SHA gives you the complete diff — everything that changed in that commit.

{{focus: end}}
{{annotate: end "Minutes, not hours"}}
This workflow — blame to find the commit, show to read the context — takes minutes.
