---
title: Code Review
---

# Step 1: What code review is actually for

{{show: review-purpose grow 0.5s spring}}
Most people think code review is for catching bugs. It's not — not primarily.

```data:review-vs-tests type=graph layout=force
(review "Code Review") -- (design "Design problems")
(review "Code Review") -- (knowledge "Knowledge transfer")
(review "Code Review") -- (ownership "Shared ownership")
(review "Code Review") -- (context "Missing business context")
(tests "Automated Tests") -- (bugs "Runtime bugs")
(tests "Automated Tests") -- (regressions "Regressions")
(tests "Automated Tests") -- (edge-cases "Edge cases")
(tests "Automated Tests") -- (contracts "API contracts")
---
rev: review
test: tests
design-node: design
knowledge-node: knowledge
bugs-node: bugs
```

{{show: review-vs-tests grow 0.5s spring}}
{{focus: test}}
{{annotate: test "Tests catch runtime behavior"}}
Tests are better at catching runtime bugs than humans are. A test catches the same bug every time. A reviewer might miss it on a tired Thursday afternoon.

{{focus: rev}}
{{annotate: rev "Review catches what tests can't"}}
Review catches things tests can't: wrong abstractions, missing edge cases in the design, knowledge gaps, security assumptions you didn't know you were making.

{{focus: design-node}}
{{annotate: design-node "Wrong approach entirely"}}
The most valuable review comment is: "this works, but we already have X that does this — let me show you." Tests never say that.

{{focus: knowledge-node}}
{{annotate: knowledge-node "The underrated benefit"}}
Every reviewer learns something about every PR they read. This is why code review matters even for experienced teams.

{{clear: slide}}

```data:bus-factor type=graph layout=force
(codebase "Codebase") -- (auth "Auth module — only Ada knows")
(codebase "Codebase") -- (payments "Payments — only Ben knows")
(codebase "Codebase") -- (reports "Reports — only Clara knows")
(auth "Auth module — only Ada knows") -> (bus "Ada gets hit by a bus")
(bus "Ada gets hit by a bus") -> (problem "No one can maintain auth")
---
code: codebase
auth-node: auth
bus-node: bus
problem-node: problem
bus-path: auth, bus, problem
```

{{show: bus-factor slide 0.4s ease-out}}
{{focus: auth-node}}
{{annotate: auth-node "Single point of knowledge"}}
A module only one person understands is a liability. It's not a compliment — it's technical debt.

{{flow: bus-path}}
{{focus: problem-node}}
{{annotate: problem-node "Bus factor: 1"}}
If any one person leaves, you lose that module's history, intent, and edge cases. Code review distributes that knowledge.

---

# Step 2: The author's job

{{clear: slide}}

```code:pr-author-checklist lang=markdown
Author checklist before requesting review:

[ ] Self-reviewed: read your own diff first
[ ] Under 400 lines changed (under 200 is better)
[ ] Tests added for new behavior
[ ] CI is green
[ ] PR description: what, why, how to test
[ ] Relevant issues linked: Fixes #123
[ ] Complex code has comments explaining WHY
[ ] Screenshots for UI changes
[ ] No debug code, no commented-out blocks
[ ] Responding within 24 hours after submitting
---
self-review: 3
size: 4
tests: 5
ci: 6
links: 8
comments: 9
screenshots: 10
clean: 11
response: 12
```

{{show: pr-author-checklist typewriter 2s linear}}
{{focus: self-review}}
{{annotate: self-review "Do this before anything else"}}
Read your own diff in the GitHub UI before requesting review. You will catch embarrassing things. You will catch real things.

{{zoom: 1.2x}}
{{focus: description}}
{{annotate: description "Context the diff can't provide"}}
The diff shows what changed. The description explains why. Reviewers without context approve things they shouldn't.

{{zoom: 1x}}
{{focus: response}}
{{annotate: response "Don't submit and disappear"}}
If review comments sit unanswered for days, the reviewer context evaporates. They have to re-read everything. Respond fast.

{{clear: slide}}

```data:self-review-flow type=graph layout=tree
(write "Write the code") -> (stage "git add -p") -> (diff "git diff --staged") -> (github-view "View in GitHub PR") -> (self-check "Find your own mistakes") -> (request "Request review")
---
write-node: write
stage-node: stage
diff-node: diff
github-node: github-view
check-node: self-check
request-node: request
path: write, stage, diff, github-view, self-check, request
```

{{show: self-review-flow grow 0.5s spring}}
{{flow: path}}
{{focus: github-node}}
{{annotate: github-node "Different context, different eyes"}}
Reading your changes in the GitHub UI, as a reviewer would see them, catches things you missed while writing. The context switch is real.

{{focus: self-check}}
{{annotate: self-check "Find it before they do"}}
Every mistake you catch in self-review is a mistake that doesn't waste a reviewer's time and doesn't make you look sloppy.

---

# Step 3: The reviewer's job

{{clear: slide}}

```code:review-comment-types lang=markdown
# Three kinds of comments:

**Blocking:** Must be resolved before this can merge.
> Blocking: This doesn't handle the case where `amount` is
> negative. `processPayment(-50, "USD")` would charge a
> negative amount. Add validation or a guard clause.

**Non-blocking (Nit):** Suggestion, not a requirement.
> Nit: Could rename `x` to `amountInCents` for clarity.

**Question:** I don't understand — not necessarily wrong.
> Q: Why does this path skip the validation step? Is that
> intentional for internal callers?
---
blocking: 3-6
nit: 8-9
question: 11-13
```

{{show: review-comment-types typewriter 2s linear}}
{{focus: blocking}}
{{annotate: blocking "Must be resolved"}}
Blocking comments are non-negotiable. The code has a correctness or security problem that must be fixed.

{{zoom: 1.2x}}
{{focus: nit}}
{{annotate: nit "Author chooses"}}
Nits are improvements, not requirements. The author can take them or leave them. Prefix clearly so they know.

{{zoom: 1x}}
{{focus: question}}
{{annotate: question "Curiosity is a valid comment"}}
Questions are not criticism. They might reveal a genuine problem. They might just teach the reviewer something. Both outcomes are good.

{{clear: slide}}

```code:reviewer-anti-patterns lang=markdown
# Anti-patterns to avoid:

BAD: "Why did you write it this way?"
(Accusatory — assumes it's wrong)

BAD: "This is wrong."
(What's wrong? Why? How should it be changed?)

BAD: "I would have done X instead."
(That's not useful unless X is actually better)

GOOD: "Blocking: This doesn't handle the zero case.
       `divide(0, x)` returns Infinity. Add a guard:
       `if (denominator === 0) return null;`"

GOOD: "Nit: `userData` could be `user` here — it's
       already clear from context that it's user data."
---
bad1: 3-4
bad2: 6-7
bad3: 9-10
good1: 12-14
good2: 16-17
```

{{show: reviewer-anti-patterns typewriter 2s linear}}
{{focus: bad1}}
{{annotate: bad1 "Puts author on the defensive"}}
"Why did you..." starts a conversation about justification, not improvement. You want to improve the code, not interrogate the author.

{{focus: good1}}
{{annotate: good1 "Specific, actionable, kind"}}
A good blocking comment names the prefix, describes the problem concretely, explains why it matters, and suggests the fix.

{{focus: good2}}
{{annotate: good2 "Nit: no friction"}}
A good nit is clearly labeled so the author knows it's optional, and briefly explains the reasoning.

{{clear: slide}}

```data:review-goals type=graph layout=force
(reviewer "Good Reviewer") -> (understand "Understand the change fully")
(reviewer "Good Reviewer") -> (blocking "Catch real problems")
(reviewer "Good Reviewer") -> (learn "Learn from the code")
(reviewer "Good Reviewer") -> (teach "Share your knowledge")
(reviewer "Good Reviewer") -> (approve "Approve when good enough")
(approve "Approve when good enough") -> (not-perfect "Not when perfect")
---
rev: reviewer
learn-node: learn
approve-node: approve
not: not-perfect
```

{{show: review-goals grow 0.5s spring}}
{{focus: approve-node}}
{{annotate: approve-node "Approve when good enough"}}
Perfect is the enemy of merged. If the code is correct, readable, and doesn't make things worse, approve it. Open follow-up issues for improvements.

{{focus: not}}
{{annotate: not "Not when perfect"}}
Code is never perfect. Waiting for perfect means it never ships. Approve and move forward.

---

# Step 4: Merging strategies

{{clear: slide}}

```data:before-pr type=graph layout=tree
(main "main: A") -> (base "main: B") -> (main-tip "main: C")
(base "main: B") -> (pr-1 "PR: D") -> (pr-2 "PR: E") -> (pr-tip "PR: F")
^main=main-tip
^feature=pr-tip
---
m1: main
m2: base
m3: main-tip
p1: pr-1
p2: pr-2
p3: pr-tip
```

{{show: before-pr grow 0.5s spring}}
{{focus: m3}}
{{annotate: m3 "main is here"}}
Main has moved since you branched. Your PR has three commits: D, E, F.

{{focus: p3}}
{{annotate: p3 "PR branch tip"}}
Three commits on the feature branch, one of which might be "fix review comments" or "WIP."

{{clear: slide}}

```data:squash-merge type=graph layout=tree
(main-a "main: A") -> (main-b "main: B") -> (main-c "main: C") -> (squashed "main: D+E+F (squashed)")
^main=squashed
---
m1: main-a
m2: main-b
m3: main-c
sq: squashed
path: main-a, main-b, main-c, squashed
```

{{show: squash-merge grow 0.5s spring}}
{{focus: sq}}
{{annotate: sq "All PR commits become one"}}
Squash merge takes D, E, F and creates a single new commit. The PR's messy "WIP" and "fix review comments" history disappears.

{{flow: path}}
Clean main history. But you lose the individual commit story from the PR.

{{clear: slide}}

```data:merge-commit type=graph layout=tree
(main-a "main: A") -> (main-b "main: B") -> (main-c "main: C") -> (merge "Merge PR #42")
(pr-d "PR: D") -> (pr-e "PR: E") -> (pr-f "PR: F") -> (merge "Merge PR #42")
^main=merge
---
m1: main-a
m2: main-b
m3: main-c
mc: merge
p1: pr-d
p2: pr-e
p3: pr-f
```

{{show: merge-commit grow 0.5s spring}}
{{focus: mc}}
{{annotate: mc "Explicit merge commit"}}
A merge commit preserves all individual PR commits AND creates a merge node that labels the PR. Full history. Noisy graph on large projects.

{{focus: p1}}
{{annotate: p1 "All PR commits visible"}}
Every commit from the PR lives in the main history. Good for attribution. Bad for readability when there are thousands of PRs.

{{clear: slide}}

```data:rebase-merge type=graph layout=tree
(main-a "main: A") -> (main-b "main: B") -> (main-c "main: C") -> (rebased-d "main: D'") -> (rebased-e "main: E'") -> (rebased-f "main: F'")
^main=rebased-f
---
m1: main-a
m2: main-b
m3: main-c
d: rebased-d
e: rebased-e
f: rebased-f
path: main-a, main-b, main-c, rebased-d, rebased-e, rebased-f
```

{{show: rebase-merge grow 0.5s spring}}
{{focus: d}}
{{annotate: d "D' is D replayed on C"}}
Rebase merge replays the PR commits on top of main. The commits get new SHAs (D becomes D') but the history is linear.

{{flow: path}}
{{focus: rebased-f}}
{{annotate: rebased-f "Linear — no merge node"}}
No merge commit. Clean linear history. Individual commits preserved. Best of both worlds, but rebasing rewrites commits.

{{clear: slide}}

```data:strategy-recommendation type=graph layout=force
(feature-branches "Feature branches") -> (squash "Squash merge")
(release-branches "Release/version branches") -> (merge-commit "Merge commit")
(short-tbd "TBD short branches") -> (rebase-merge "Rebase merge")
(squash "Squash merge") -> (why-squash "One logical change per feature in main")
(merge-commit "Merge commit") -> (why-merge "Preserve branch context for releases")
---
feat: feature-branches
rel: release-branches
tbd: short-tbd
sq: squash
mc: merge-commit
rb: rebase-merge
```

{{show: strategy-recommendation grow 0.5s spring}}
{{focus: sq}}
{{annotate: sq "Default for feature PRs"}}
Squash merge is the right default for feature branches. The PR review has the individual commits. Main history stays clean.

{{focus: mc}}
{{annotate: mc "Preserve release branch context"}}
For release or version branches where the individual commits matter for release notes, merge commit preserves the structure.

---

# Step 5: Code review culture

{{clear: slide}}

```data:culture-principles type=graph layout=force
(culture "Healthy Review Culture") -> (code-not-author "Review the code, not the author")
(culture "Healthy Review Culture") -> (good-enough "Approve good enough — open issues for later")
(culture "Healthy Review Culture") -> (sla "Review SLA: max 24 hours")
(culture "Healthy Review Culture") -> (learn "Reviewing to learn counts")
(code-not-author "Review the code, not the author") -> (psychological-safety "Psychological safety")
(sla "Review SLA: max 24 hours") -> (flow-state "Protects author's flow state")
---
cult: culture
code: code-not-author
sla-node: sla
learn-node: learn
safety: psychological-safety
flow: flow-state
```

{{show: culture-principles grow 0.5s spring}}
{{focus: code}}
{{annotate: code "Fundamental separation"}}
The code being wrong doesn't make the author wrong. This separation is the foundation of productive review culture.

{{focus: sla-node}}
{{annotate: sla-node "Slow review is a tax"}}
A PR sitting in review for 5 days creates merge conflicts, kills momentum, and discourages small PRs. Review quickly or the system degrades.

{{focus: learn-node}}
{{annotate: learn-node "Even juniors can review"}}
A junior engineer reviewing a senior's PR might not catch bugs. But they will ask questions that catch design problems. Both learn.

{{clear: slide}}

```code:codeowners lang=text
# .github/CODEOWNERS
# Maps file patterns to required reviewers

# Everything requires any team member
* @company/engineering

# Auth requires the security team
src/auth/           @company/security-team

# Payment code requires two specific people
src/payments/       @ada @ben

# CI/CD requires the platform team
.github/workflows/  @company/platform-team
---
everything: 4-5
auth: 7-8
payments: 10-11
ci: 13-14
```

{{show: codeowners typewriter 2s linear}}
{{focus: auth}}
{{annotate: auth "Automatic reviewer assignment"}}
When a PR touches `src/auth/`, the security team is automatically added as required reviewers. No one has to remember to add them.

{{zoom: 1.2x}}
{{focus: payments}}
{{annotate: payments "Two-person rule"}}
Payment code requires both Ada and Ben to review. This is a standard control for financial code.

{{zoom: 1x}}
{{focus: ci}}
{{annotate: ci "Platform team owns workflows"}}
Changes to CI/CD pipelines require platform team approval. Nobody accidentally breaks the deployment pipeline.

{{clear: slide}}

```code:request-pull lang=bash
# git request-pull — git's built-in PR summary
git request-pull v1.0 https://github.com/original/repo.git main

# Generates:
# The following changes since commit v1.0 are available...
#
# Ada Lovelace (3):
#       feat(auth): add OAuth2 login with Google
#       fix(auth): handle token expiry on refresh
#       test(auth): add integration tests
#
# src/auth/oauth.ts      | 145 +++++++++++++
# src/auth/oauth.test.ts |  89 ++++++++
# 2 files changed, 234 insertions(+)
---
cmd: 1-2
output: 4-13
```

{{show: request-pull typewriter 2s linear}}
{{focus: cmd}}
{{annotate: cmd "Git's built-in PR summary"}}
`git request-pull` generates a summary of changes for email-based review — the workflow used in kernel development where PRs don't exist.

{{focus: output}}
{{annotate: output "Summary format"}}
The output lists commits by author, shows changed files with line counts, and links to the base commit. Useful if you're contributing to projects that use email patches instead of PRs.
