---
title: Fork and Open Source
---

# Step 1: What a fork is

{{show: fork-intro grow 0.5s spring}}
You want to contribute to a project. You don't have push access. What now?

```diagram:fork-diagram
upstream-repo [github: upstream repo]
your-fork [github: your fork]
your-machine [git: your machine]
upstream-repo --> your-fork: GitHub forks (server-side)
your-fork --> your-machine: git clone
{GitHub: upstream-repo, your-fork}
{Local: your-machine}
---
upstream: upstream-repo
fork: your-fork
local: your-machine
```

{{show: fork-diagram grow 0.5s spring}}
{{focus: upstream}}
{{annotate: upstream "The original — you can't push here"}}
The upstream repository is owned by someone else. You can read it. You cannot push to it.

{{focus: fork}}
{{annotate: fork "Your copy on GitHub's servers"}}
A fork is a server-to-server copy. GitHub copies the upstream repo to your account. No download required — it happens on GitHub's infrastructure.

{{focus: local}}
{{annotate: local "You clone your fork"}}
Then you clone your fork to your machine. You have full push access to your fork.

{{clear: slide}}

```data:fork-use-cases type=graph layout=force
(fork "Forking") -> (contribute "Contributing to open source")
(fork "Forking") -> (experiment "Experimenting without risk")
(fork "Forking") -> (diverge "Maintaining a diverged version")
(contribute "Contributing to open source") -> (pr "Send PR to upstream")
(diverge "Maintaining a diverged version") -> (never-merge "May never merge back")
(experiment "Experimenting without risk") -> (delete "Delete fork when done")
---
fork-node: fork
contrib: contribute
exp: experiment
div: diverge
never: never-merge
```

{{show: fork-use-cases slide 0.4s ease-out}}
{{focus: contrib}}
{{annotate: contrib "Most common reason"}}
Open source contribution is the primary use case. Fork, make changes, send a PR to the upstream.

{{focus: div}}
{{annotate: div "Maintained forks"}}
Some projects are maintained forks of other projects — intentionally diverged. The original may have stopped development, or the fork has a different focus.

{{focus: exp}}
{{annotate: exp "Safe experimentation"}}
Fork a project to try something risky. If it works, send a PR. If it doesn't, delete the fork.

---

# Step 2: The open source contribution workflow

{{clear: slide}}

```code:oss-workflow lang=bash
# Step 1: Fork on GitHub (click the Fork button)

# Step 2: Clone YOUR fork
git clone git@github.com:YOUR-USERNAME/project.git
cd project

# Step 3: Add the original as "upstream" remote
git remote add upstream https://github.com/original/project.git
git remote -v
# origin   git@github.com:YOUR-USERNAME/project.git (fetch)
# origin   git@github.com:YOUR-USERNAME/project.git (push)
# upstream https://github.com/original/project.git (fetch)
# upstream https://github.com/original/project.git (push)

# Step 4: Create a branch for your change
git checkout -b fix/correct-date-parsing

# Step 5: Make commits, push to YOUR fork
git add -p
git commit -m "fix: correct UTC offset in date parsing"
git push -u origin fix/correct-date-parsing

# Step 6: Open PR from your fork's branch to upstream main
---
fork-step: 1
clone: 3-4
upstream: 6-12
branch: 14-15
commits: 17-19
pr-step: 21
```

{{show: oss-workflow typewriter 2s linear}}
{{focus: clone}}
{{annotate: clone "Clone YOUR fork, not upstream"}}
You clone your fork (on YOUR account), not the original. You have push access to your fork.

{{zoom: 1.2x}}
{{focus: upstream}}
{{annotate: upstream "Two remotes: origin and upstream"}}
After this, you have two remotes. `origin` is your fork. `upstream` is the original project.

{{zoom: 1x}}
{{focus: branch}}
{{annotate: branch "Never commit to main directly"}}
Always work on a branch. Keeping your fork's `main` clean makes staying up to date easier.

{{focus: pr-step}}
{{annotate: pr-step "GitHub UI shows the option"}}
After pushing, GitHub shows a "Compare and open pull request" banner. It knows which branch to compare against upstream.

{{clear: slide}}

```data:oss-flow-full type=graph layout=tree
(start "Upstream repo") -> (fork-step "Fork on GitHub") -> (clone-step "Clone your fork") -> (remote-step "Add upstream remote") -> (branch-step "Create branch") -> (commit-step "Make commits") -> (push-step "Push to origin") -> (pr-step "Open PR to upstream") -> (review-step "Review + iteration") -> (merge-step "Upstream merges")
^origin=fork-step
^upstream=start
---
up: start
fork: fork-step
clone: clone-step
remote: remote-step
branch: branch-step
commit: commit-step
push: push-step
pr: pr-step
review: review-step
merged: merge-step
path: start, fork-step, clone-step, remote-step, branch-step, commit-step, push-step, pr-step, review-step, merge-step
```

{{show: oss-flow-full grow 0.5s spring}}
{{flow: path}}
{{focus: remote-step}}
{{annotate: remote-step "Critical step — often skipped"}}
Adding the upstream remote is what allows you to stay in sync with the original project. Without it, your fork drifts.

{{focus: review-step}}
{{annotate: review-step "Be responsive"}}
The maintainer may request changes. Respond promptly. Add commits to the same branch — the PR auto-updates.

---

# Step 3: Keeping your fork up to date

{{clear: slide}}

```data:diverged-fork type=graph layout=tree
(base "common ancestor") -> (upstream-a "upstream: A") -> (upstream-b "upstream: B") -> (upstream-c "upstream: C — you need this")
(base "common ancestor") -> (your-a "your fork: D") -> (your-b "your fork: E")
^upstream=upstream-c
^fork=your-b
---
common: base
up-a: upstream-a
up-b: upstream-b
up-c: upstream-c
your-a: your-a
your-b: your-b
```

{{show: diverged-fork grow 0.5s spring}}
{{focus: common}}
{{annotate: common "Where you forked from"}}
Your fork and upstream share this ancestor. Since then, upstream moved forward.

{{focus: up-c}}
{{annotate: up-c "Upstream has new commits"}}
The upstream project has merged A, B, and C since you forked. Your branch needs to incorporate these.

{{focus: your-b}}
{{annotate: your-b "Your work on top of old base"}}
Your commits D and E sit on top of the old base. They need to be replayed on top of the latest upstream.

{{clear: slide}}

```code:sync-fork lang=bash
# Fetch new commits from upstream
git fetch upstream
# remote: Enumerating objects: 15, done.
# From https://github.com/original/project
#  * [new branch]      main -> upstream/main

# Rebase your branch on top of latest upstream main
git rebase upstream/main
# Successfully rebased and updated refs/heads/fix/correct-date-parsing

# Force push (your branch history changed due to rebase)
git push --force-with-lease origin fix/correct-date-parsing

# Alternative: merge instead of rebase (creates a merge commit)
git merge upstream/main
---
fetch: 1-5
rebase: 7-9
force-push: 11-12
merge-alt: 14-15
```

{{show: sync-fork typewriter 2s linear}}
{{focus: fetch}}
{{annotate: fetch "Download, don't integrate"}}
`git fetch upstream` downloads upstream's commits but doesn't change your branch. Safe to run anytime.

{{zoom: 1.2x}}
{{focus: rebase}}
{{annotate: rebase "Replay on top of latest"}}
Rebase replays your commits on top of `upstream/main`. Your PR will have no conflicts and show a clean diff against the latest code.

{{zoom: 1x}}
{{focus: force-push}}
{{annotate: force-push "Rebase rewrites history"}}
After rebasing, you must force push. `--force-with-lease` is safer than `--force` — it fails if someone else pushed to your branch.

{{focus: merge-alt}}
{{annotate: merge-alt "Creates a merge commit"}}
`git merge upstream/main` keeps your branch intact but adds a merge commit. GitHub's "Sync fork" button does this. Prefer rebase for cleaner PR history.

{{clear: slide}}

```data:rebase-vs-merge-sync type=graph layout=tree
(before-both "D — E (your commits)") -> (after-rebase "upstream A — B — C — D' — E'") -> (clean-pr "Clean PR — no merge commit")
(before-both "D — E (your commits)") -> (after-merge "upstream A — B — C + Merge — D — E") -> (noisy-pr "PR has merge commit in it")
---
before: before-both
rebase-result: after-rebase
merge-result: after-merge
clean: clean-pr
noisy: noisy-pr
```

{{show: rebase-vs-merge-sync grow 0.5s spring}}
{{focus: clean-pr}}
{{annotate: clean-pr "Preferred for PRs"}}
The rebase result is what reviewers want to see: just your changes, applied on top of the latest code.

{{focus: noisy-pr}}
{{annotate: noisy-pr "Sync commits pollute the diff"}}
When you sync with merge, the PR includes the merge commit. Reviewers see your changes AND the sync. Rebase is cleaner.

---

# Step 4: The anatomy of a pull request

{{clear: slide}}

```code:pr-anatomy lang=markdown
Title: fix: correct UTC offset in date parsing

Fixes #892

The `parseDate()` function was interpreting timestamps in
local time instead of UTC. This caused events created in
timezones west of UTC to show with the wrong date.

Reproducing:
1. Set system timezone to US/Pacific
2. Create an event at 11pm
3. Event shows as previous day in the UI

Fix: pass `{ timeZone: 'UTC' }` to `Intl.DateTimeFormat`
in `src/utils/dates.ts`.

**Testing:**
- [ ] Create event at 11pm in US/Pacific timezone
- [ ] Confirm correct date shown in UI
- [ ] Confirm `parseDate.test.ts` passes

Reviewers: @maintainer-name
Labels: bug, good-first-fix
---
title: 1
closes: 3
problem: 5-9
reproduce: 11-14
fix-desc: 16
testing: 18-21
meta: 23-24
```

{{show: pr-anatomy typewriter 2s linear}}
{{focus: title}}
{{annotate: title "Conventional commit format"}}
PR titles should follow conventional commit format when possible. Many projects use the PR title as the squash commit message.

{{zoom: 1.2x}}
{{focus: closes}}
{{annotate: closes "Auto-closes the issue on merge"}}
`Fixes #892` automatically closes issue 892 when the PR merges to main. `Closes` and `Resolves` also work.

{{zoom: 1x}}
{{focus: reproduce}}
{{annotate: reproduce "Reproduce steps are gold"}}
Exact reproduction steps mean the maintainer can verify the bug and verify the fix without guessing.

{{focus: testing}}
{{annotate: testing "Tell them how to verify"}}
A testing checklist tells reviewers exactly how to confirm the fix works. Don't make them figure it out.

{{clear: slide}}

```data:pr-labels type=graph layout=force
(pr "Pull Request") -- (bug-label "bug — something's broken")
(pr "Pull Request") -- (feat-label "feature — new behavior")
(pr "Pull Request") -- (breaking-label "breaking-change — semver major")
(pr "Pull Request") -- (review-label "needs-review — waiting on someone")
(pr "Pull Request") -- (wip-label "WIP — not ready")
(pr "Pull Request") -- (good-first "good-first-issue — for newcomers")
---
pr-node: pr
bug: bug-label
feat: feat-label
breaking: breaking-label
wip: wip-label
```

{{show: pr-labels grow 0.5s spring}}
{{focus: wip-label}}
{{annotate: wip-label "Or use draft PR"}}
For work-in-progress, use GitHub's "Draft PR" feature instead of a WIP label. Draft PRs can't accidentally be merged.

{{focus: breaking}}
{{annotate: breaking "Signals semver major bump"}}
A `breaking-change` label alerts maintainers and release tooling that this needs a major version bump.

{{focus: good-first}}
{{annotate: good-first "Invite new contributors"}}
Maintainers use this label to signal which issues are accessible to new contributors. It's a deliberate community-building gesture.

---

# Step 5: Forks vs branches — when to use which

{{clear: slide}}

```data:fork-vs-branch type=graph layout=tree
(start "Do you have push access?") -> (yes "Yes") -> (branch "Use a branch + PR")
(start "Do you have push access?") -> (no "No") -> (fork "Use a fork + PR")
(branch "Use a branch + PR") -> (same-org "Same org or team repo")
(fork "Use a fork + PR") -> (external "External or open source repo")
(same-org "Same org or team repo") -> (rule "Never fork your own project")
(rule "Never fork your own project") -> (reason "Forks duplicate overhead for no benefit")
---
decision: start
yes-path: yes
no-path: no
branch-node: branch
fork-node: fork
same: same-org
rule-node: rule
yes-route: start, yes, branch, same-org, rule
no-route: start, no, fork, external
```

{{show: fork-vs-branch grow 0.5s spring}}
{{flow: yes-route}}
{{focus: rule-node}}
{{annotate: rule-node "Common mistake"}}
New contributors sometimes fork their own organization's repo. Don't. Use branches. Forks are for repos where you don't have write access.

{{flow: no-route}}
{{focus: fork-node}}
{{annotate: fork-node "The correct use case"}}
If you're contributing to a project you don't own — open source, a library, someone else's tool — fork it.

{{clear: slide}}

```code:fork-shortcuts lang=bash
# Check if you're using fork correctly
git remote -v
# If you see:
# origin   git@github.com:YOUR-NAME/project.git (push)
# upstream https://github.com/ORIGINAL/project.git (push)
# → Correct fork setup

# If you see:
# origin   git@github.com:YOUR-NAME/YOUR-NAME-project.git (push)
# → You forked your own project. Just use the original with a branch.

# Convert fork to branch workflow:
git remote set-url origin git@github.com:ORIGINAL/project.git
# Now you're working with the original. Add your branch and PR normally.
---
correct: 2-6
wrong: 8-10
fix: 12-14
```

{{show: fork-shortcuts typewriter 2s linear}}
{{focus: correct}}
{{annotate: correct "Two remotes is the signal"}}
A correctly configured fork has two remotes: `origin` pointing to your fork, `upstream` pointing to the original.

{{zoom: 1.2x}}
{{focus: wrong}}
{{annotate: wrong "Forked your own project"}}
If both your fork name and the project name are in your username, you've forked your own repository. Unnecessary.

{{zoom: 1x}}
{{focus: fix}}
{{annotate: fix "Redirect origin to the real remote"}}
You can fix it by changing where `origin` points. Then work normally — branch and PR against the real repo.
