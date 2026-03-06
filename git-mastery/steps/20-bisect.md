---
title: git bisect
---

# Step 1: The problem

{{show: intro-tree grow 0.5s spring}}
A bug is in your codebase. It wasn't there three weeks ago.

```data:intro-tree type=binary-tree
[commits, first-100, last-100, 1-25, 26-50, 51-75, 76-100, 1-12, 13-25, 26-37, 38-50, null, null, 76-87, 88-100]
---
all: 0
left-half: 1
right-half: 2
q1: 3
q2: 4
q3: 5
q4: 6
eighth-1: 7
eighth-2: 8
eighth-3: 9
eighth-4: 10
eighth-7: 13
eighth-8: 14
```

{{focus: all}}
{{annotate: all "200 commits to search"}}
You have 200 commits between now and the last known good state.

{{zoom: 1.3x}}
{{focus: left-half}}
{{annotate: left-half "Checkout #100 first"}}
Brute force means checking out every single commit. That's 200 checkouts.

{{zoom: 1x}}
{{focus: right-half}}
{{annotate: right-half "Half the space each step"}}
Binary search cuts the problem in half at every step.

{{zoom: 1.2x}}
{{focus: q3}}
{{annotate: q3 "Step 2: half again"}}
Log base two of 200 is roughly 8. Eight steps to find the culprit.

{{zoom: 1x}}
{{focus: eighth-3}}
{{annotate: eighth-3 "Step 3: narrowing in"}}
Each level of the tree is one test. One checkout, one question: good or bad?

{{focus: eighth-7}}
{{annotate: eighth-7 "Found it"}}
By step 8, you've isolated the exact commit.

{{clear: slide}}

```data:brute-vs-binary type=array
["Brute force  │ 200 checkouts │ hours of work", "Binary search │ 8 checkouts  │ minutes", "Binary search wins — git automates this"]
---
brute: 0
binary: 1
winner: 2
path: 0, 1, 2
```

{{show: brute-vs-binary slide 0.4s ease-out}}
{{focus: brute}}
{{annotate: brute "O(n) — never do this"}}
Checking out 200 commits by hand takes hours.

{{focus: binary}}
{{annotate: binary "O(log n) — 8 steps"}}
Binary search takes minutes.

{{flow: path}}
Git can do this automatically.

---

# Step 2: Manual bisect

{{clear: slide}}

```code:bisect-start lang=bash
git bisect start
git bisect bad
git bisect good abc1234
# Bisecting: 99 revisions left to test after this
# (roughly 7 steps)
# [deadbeef] Add user authentication module
---
start-cmd: 1
bad-cmd: 2
good-cmd: 3
output: 4-6
```

{{show: bisect-start typewriter 2s linear}}
{{focus: start-cmd}}
{{annotate: start-cmd "Enter bisect mode"}}
`git bisect start` puts git into bisect mode.

{{focus: bad-cmd}}
{{annotate: bad-cmd "Current HEAD is bad"}}
`git bisect bad` marks the current commit — the one you know has the bug.

{{focus: good-cmd}}
{{annotate: good-cmd "Known good SHA"}}
`git bisect good` marks the last commit you know was clean.

{{zoom: 1.3x}}
{{focus: output}}
{{annotate: output "Git does the math"}}
Git tells you how many revisions remain and roughly how many steps.

{{zoom: 1x}}
{{clear: slide}}

```data:bisect-tree type=binary-tree
[start, bad-zone, good-zone, midpoint, null, null, null, tested-bad, null, null, null, null, null, null, null]
---
start: 0
bad: 1
good: 2
mid: 3
tested: 7
search-path: 0, 3
```

{{show: bisect-tree grow 0.5s spring}}
{{focus: start}}
{{annotate: start "You are here"}}
After running those three commands, git checks out the midpoint commit automatically.

{{focus: bad}}
{{annotate: bad "HEAD is bad"}}
The bad end is the commit you're on now.

{{focus: good}}
{{annotate: good "abc1234 is good"}}
The good end is the hash you provided.

{{zoom: 1.2x}}
{{focus: mid}}
{{annotate: mid "Git checks out this"}}
Git picks the commit exactly halfway between them.

{{zoom: 1x}}
{{flow: search-path}}
You run your tests at that midpoint.

{{clear: slide}}

```code:bisect-loop lang=bash
# After checkout, run your tests...
npm test
# Tests fail — this commit is bad
git bisect bad

# Git checks out another midpoint...
# Tests pass — this commit is good
git bisect good

# Repeat until:
# deadbeef is the first bad commit
# commit deadbeef
# Author: Ada Lovelace <ada@example.com>
# Date:   Mon Feb 10 14:23:11 2025
#
#     feat: switch sorting algorithm
---
bad-mark: 4
good-mark: 8
result: 10-16
bisect-loop-path: 4, 8, 10-16
```

{{show: bisect-loop slide 0.4s ease-out}}
{{focus: bad-mark}}
{{annotate: bad-mark "Tests fail — mark bad"}}
When tests fail, mark it bad. Git narrows to the lower half.

{{focus: good-mark}}
{{annotate: good-mark "Tests pass — mark good"}}
When tests pass, mark it good. Git narrows to the upper half.

{{zoom: 1.2x}}
{{focus: result}}
{{annotate: result "The first bad commit"}}
After 7 or 8 iterations, git prints the exact culprit.

{{zoom: 1x}}
{{flow: bisect-loop-path}}
Each answer cuts the search space in half.

---

# Step 3: Automated bisect

{{clear: slide}}

```code:test-script lang=bash
#!/usr/bin/env bash
# test-sort.sh
# Exit 0 = good, non-zero = bad, 125 = skip

set -e

npm ci --silent
npm run build --silent

# Run the specific test for the bug
npm test -- --testNamePattern="sort returns stable order"
---
shebang: 1
exit-codes: 2-3
install: 5-6
specific-test: 8-9
```

{{show: test-script typewriter 2s linear}}
{{focus: shebang}}
{{annotate: shebang "Must be executable"}}
The test script runs at each bisect midpoint. Make it executable with `chmod +x`.

{{zoom: 1.2x}}
{{focus: exit-codes}}
{{annotate: exit-codes "Exit codes are the protocol"}}
Exit 0 means the commit is good. Anything else means bad. Exit 125 is special — it tells git to skip this commit entirely.

{{zoom: 1x}}
{{focus: install}}
{{annotate: install "Clean install each time"}}
Always do a clean install. You're testing the full state of the repo at each commit.

{{focus: specific-test}}
{{annotate: specific-test "Test exactly the bug"}}
Test the specific behavior you're hunting. Don't run the full suite — keep it fast.

{{clear: slide}}

```code:bisect-run lang=bash
git bisect start
git bisect bad
git bisect good abc1234
git bisect run ./test-sort.sh

# running ./test-sort.sh
# ...
# running ./test-sort.sh
# ...
# running ./test-sort.sh
# ...
# deadbeef is the first bad commit
# commit deadbeef
# Author: Ada Lovelace <ada@example.com>
# Date:   Mon Feb 10 14:23:11 2025
#
#     feat: switch sorting algorithm
#
# bisect run success
---
setup: 1-3
run-cmd: 4
output-lines: 6-18
```

{{show: bisect-run typewriter 2s linear}}
{{focus: setup}}
{{annotate: setup "Same setup as manual"}}
The setup is identical. The difference is what comes next.

{{zoom: 1.3x}}
{{focus: run-cmd}}
{{annotate: run-cmd "Fully automated from here"}}
`git bisect run` hands control to the script. You walk away.

{{zoom: 1x}}
{{focus: output-lines}}
{{annotate: output-lines "Git drives, script answers"}}
Git runs the script at each midpoint, reads the exit code, and decides which half to search next.

{{clear: slide}}

```diagram:automated-flow
git [service label="git bisect run"]
checkout [service label="checkout midpoint"]
script [service label="run script"]
result [service label="exit code?"]
good-path [service label="exit 0 → mark good"]
bad-path [service label="exit 1 → mark bad"]
skip-path [service label="exit 125 → skip"]
done [client label="found it"]
git --> checkout
checkout --> script
script --> result
result --> good-path: exit 0
result --> bad-path: exit 1
result --> skip-path: exit 125
good-path --> checkout: continue
bad-path --> checkout: continue
skip-path --> checkout: continue
good-path --> done
bad-path --> done
---
start: git
mid: checkout
run: script
branch: result
good: good-path
bad: bad-path
skip: skip-path
end: done
main-loop: git, checkout, script, result
```

{{show: automated-flow grow 0.5s spring}}
{{focus: start}}
{{annotate: start "You run this once"}}
One command starts the whole process.

{{flow: main-loop}}
Git runs the loop. Script answers. Git narrows the range.

{{focus: skip-path}}
{{annotate: skip-path "Exit 125 = skip"}}
Exit 125 tells git this commit is untestable — broken build, won't compile. Git skips it.

{{focus: done}}
{{annotate: done "Printed automatically"}}
When the search space collapses to one commit, git prints the answer.

---

# Step 4: Advanced bisect

{{clear: slide}}

```code:advanced-bisect lang=bash
# Skip a commit that won't compile
git bisect skip deadbeef

# Skip a range of commits
git bisect skip v2.3.0..v2.3.5

# View the bisect session history
git bisect log
# git bisect start
# # bad: [abc1234] feat: switch sorting algorithm
# # good: [def5678] chore: update dependencies
# git bisect bad abc1234
# git bisect good def5678

# Save the log to replay later
git bisect log > bisect-session.log

# Replay a saved session
git bisect replay bisect-session.log

# ALWAYS do this when done
git bisect reset
---
skip-one: 2
skip-range: 5
log-cmd: 8
log-output: 9-14
save-log: 17
replay-cmd: 20
reset-cmd: 23
```

{{show: advanced-bisect typewriter 2s linear}}
{{focus: skip-one}}
{{annotate: skip-one "Can't test this commit"}}
`git bisect skip` is for commits where the code won't compile or the test environment is broken. Git works around them.

{{focus: skip-range}}
{{annotate: skip-range "Skip entire ranges"}}
You can skip a whole range. Git will find the first bad commit outside the skipped region.

{{zoom: 1.2x}}
{{focus: log-cmd}}
{{annotate: log-cmd "Full session history"}}
`git bisect log` shows every decision made in this bisect session.

{{zoom: 1x}}
{{focus: log-output}}
{{annotate: log-output "Reproducible record"}}
The log format is also the replay format. What you see is what you can feed back in.

{{focus: save-log}}
{{annotate: save-log "Save for sharing"}}
Save the log if you need to share the session, or continue it on another machine.

{{zoom: 1.3x}}
{{focus: reset-cmd}}
{{annotate: reset-cmd "CRITICAL — do not forget"}}
`git bisect reset` exits bisect mode and returns you to the original HEAD. Forgetting this leaves you on a random detached HEAD commit.

{{zoom: 1x}}
{{clear: slide}}

```data:stuck-state type=graph layout=tree
(original "main HEAD") -> (bisect-commits "8 random commits checked out") -> (forgot-reset "forgot bisect reset") -> (confusion "HEAD is detached at deadbeef")
(original "main HEAD") -> (clean-exit "git bisect reset") -> (back "back on main")
---
start: original
problem-path: forgot-reset
stuck: confusion
good-path: clean-exit
safe: back
problem-route: original, bisect-commits, forgot-reset, confusion
safe-route: original, clean-exit, back
```

{{show: stuck-state slide 0.4s ease-out}}
{{focus: problem-path}}
{{annotate: problem-path "Common mistake"}}
The most common mistake: finishing bisect but not resetting. You are now on a detached HEAD somewhere random in history.

{{flow: problem-route}}
{{focus: confusion}}
{{annotate: confusion "Scary but fixable"}}
Not catastrophic — just `git bisect reset` or `git checkout main` — but disorienting.

{{flow: safe-route}}
{{focus: safe}}
{{annotate: safe "Always end here"}}
Always end a bisect session with `git bisect reset`.

---

# Step 5: Using bisect in practice

{{clear: slide}}

```code:practical-tips lang=bash
# See the range you're about to search
git log --oneline abc1234..HEAD
# 8f3a2b1 fix: handle null pointer in renderer
# 4d2c9e0 feat: add keyboard shortcuts
# ...
# deadbeef feat: switch sorting algorithm (200 commits total)

# Write fast test scripts — under 5 seconds
time ./test-sort.sh
# real    0m2.341s

# Scope the test to ONE behavior
# BAD:  npm test (runs 800 tests, takes 3 minutes)
# GOOD: npm test -- --testNamePattern="sort"
---
log-range: 2-7
timing: 10-12
scope: 14-17
```

{{show: practical-tips typewriter 2s linear}}
{{focus: log-range}}
{{annotate: log-range "Preview before searching"}}
Before bisecting, run this to see the full commit range. Understand what you're searching before you search it.

{{zoom: 1.2x}}
{{focus: timing}}
{{annotate: timing "Fast scripts save minutes"}}
Eight iterations at 2 seconds each is 16 seconds. Eight iterations at 3 minutes each is 24 minutes. Make it fast.

{{zoom: 1x}}
{{focus: scope}}
{{annotate: scope "One test, one bug"}}
Run the minimum test that proves the bug exists. The full test suite introduces noise — something else might fail at an older commit.

{{clear: slide}}

```diagram:bisect-vs-manual
decision [service label="Which do you use?"]
automated [service label="Automatable test?"]
run [service label="git bisect run"]
manual-ui [service label="UI / visual bug?"]
manual-bisect [service label="git bisect manually"]
result [client label="Found!"]
decision --> automated: yes
decision --> manual-ui: no
automated --> run
manual-ui --> manual-bisect
run --> result
manual-bisect --> result
---
fork: decision
auto-path: automated
manual-path: manual-ui
run: run
manual-bisect: manual-bisect
auto-route: decision, automated, run, result
manual-route: decision, manual-ui, manual-bisect, result
```

{{show: bisect-vs-manual grow 0.5s spring}}
{{focus: fork}}
{{annotate: fork "One decision"}}
Can you write a script that exits 0 for passing and non-zero for failing?

{{flow: auto-route}}
{{focus: run}}
{{annotate: run "Best case: fully automated"}}
If yes, `git bisect run` handles everything. This is the best outcome.

{{flow: manual-route}}
{{focus: manual-bisect}}
{{annotate: manual-bisect "Still beats manual search"}}
Even manual bisect — checking out 8 commits yourself — is vastly better than checking out 200. Use it.

{{clear: slide}}

```diagram:power-combo
tests [service label="Automated test suite"]
bisect [service label="git bisect run"]
bug [service label="Exact bad commit"]
blame [service label="git blame"]
show [service label="git show <hash>"]
author [client label="Who wrote it"]
diff [client label="Exactly what changed"]
tests --> bisect: git bisect run
bisect --> bug: exact commit
bug --> blame: git blame
bug --> show: git show
blame --> author
show --> diff
---
tests: tests
bisect: bisect
bug: bug
blame: blame
author: author
show: show
diff: diff
path: tests, bisect, bug
```

{{show: power-combo grow 0.5s spring}}
{{flow: path}}
Bisect plus automated tests is one of the most powerful debugging tools in git.

{{focus: blame}}
{{annotate: blame "Who wrote the change"}}
Once you have the commit hash, `git blame` shows you exactly which lines changed.

{{focus: show}}
{{annotate: show "What the change was"}}
`git show <hash>` shows the full diff. You know what changed, when, and why.

{{focus: diff}}
{{annotate: diff "Complete picture"}}
The combination — bisect to find the commit, show to read the diff — is the fastest path from "there's a bug" to "here's the cause."
