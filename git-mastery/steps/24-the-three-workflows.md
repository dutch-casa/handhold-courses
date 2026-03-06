---
title: The Three Workflows
---

# Step 1: GitFlow — the old way

{{show: gitflow-diagram grow 0.5s spring}}
GitFlow was invented in 2010. It made sense then.

```diagram:gitflow-diagram
master [git: main / master]
develop [git: develop]
feature-one [git: feature/one]
feature-two [git: feature/two]
release-branch [git: release/1.0]
hotfix [git: hotfix/critical]
feature-one --> develop: merge
feature-two --> develop: merge
develop --> release-branch: cut release
release-branch --> master: release
master --> hotfix: branch
hotfix --> master: merge
hotfix --> develop: backport
{Features: feature-one, feature-two}
{Release: release-branch}
---
prod: master
dev: develop
feat1: feature-one
feat2: feature-two
rel: release-branch
hot: hotfix
```

{{show: gitflow-diagram grow 0.5s spring}}
{{focus: prod}}
{{annotate: prod "main — production only"}}
`master` (or `main`) only ever receives merges from release branches and hotfixes. Never directly.

{{focus: dev}}
{{annotate: dev "Integration branch"}}
`develop` is where features land. It's always one step ahead of `master`.

{{focus: feat1}}
{{annotate: feat1 "Long-lived feature branches"}}
Features branch off `develop` and merge back when complete. These can live for weeks.

{{focus: rel}}
{{annotate: rel "Stabilization before release"}}
A `release` branch is cut from `develop` when it's ready to ship. Only bug fixes go here.

{{focus: hot}}
{{annotate: hot "Emergency patches"}}
Hotfixes branch from `master` directly and merge into both `master` and `develop`.

{{clear: slide}}

```data:gitflow-critique type=graph layout=tree
(gitflow "GitFlow") -> (complex "5 branch types to manage")
(gitflow "GitFlow") -> (conflicts "Long-lived branches = big merges")
(gitflow "GitFlow") -> (overhead "Release branch ceremony")
(gitflow "GitFlow") -> (good-for "Multiple parallel release versions")
(complex "5 branch types to manage") -> (atlassian "Atlassian labels it legacy")
(good-for "Multiple parallel release versions") -> (example "Kernel, enterprise, embedded")
---
flow: gitflow
problem1: complex
problem2: conflicts
creator: atlassian
good: good-for
```

{{show: gitflow-critique slide 0.4s ease-out}}
{{focus: problem2}}
{{annotate: problem2 "The main killer"}}
Long-lived branches diverge. Merging a 3-week-old feature branch creates massive conflicts. The longer it lives, the worse it gets.

{{focus: atlassian}}
{{annotate: atlassian "Even Atlassian walked it back"}}
The creator of GitFlow added a note to his original 2010 blog post: teams without multiple release versions in production should use simpler workflows.

{{focus: good}}
{{annotate: good "Only valid use case"}}
If you ship v1.x security patches while v2.x is in development — firmware, enterprise software, open source libraries — GitFlow fits. Otherwise it doesn't.

---

# Step 2: GitHub Flow — the practical default

{{clear: slide}}

```diagram:github-flow-diagram
main-branch [git: main — always deployable]
feature-branch [git: feature/your-work]
pull-request [github: pull request]
ci-check [service: CI checks]
deploy [service: deploy]
feature-branch --> pull-request: open PR
pull-request --> ci-check: triggers
ci-check --> main-branch: gate passes
main-branch --> deploy: every merge
{Gate: pull-request, ci-check}
---
main: main-branch
feat: feature-branch
pr: pull-request
ci: ci-check
deploy-node: deploy
```

{{show: github-flow-diagram grow 0.5s spring}}
{{focus: main}}
{{annotate: main "Always deployable"}}
`main` is the single rule of GitHub Flow: it is always in a deployable state. Never break it.

{{focus: feat}}
{{annotate: feat "Short-lived — hours to 2 days"}}
Feature branches exist only long enough to get the work done and reviewed. Not weeks.

{{focus: pr}}
{{annotate: pr "Where review happens"}}
The PR is the gate. Review happens here. No PR, no merge.

{{focus: ci}}
{{annotate: ci "Automated quality gate"}}
CI must pass. Tests green, lint clean, build succeeds. No exceptions.

{{focus: deploy-node}}
{{annotate: deploy-node "Deploy after every merge"}}
Every merge to `main` triggers a deployment. This is continuous delivery.

{{clear: slide}}

```code:github-flow-rules lang=bash
# The complete workflow — 5 rules
# 1. main is always deployable
# 2. Create a branch for your work
git checkout -b feat/add-dashboard

# 3. Commit on the branch
git add -p
git commit -m "feat(dashboard): add user retention chart"
git push -u origin feat/add-dashboard

# 4. Open a PR. Get review. CI must pass.
# 5. Merge to main. Deploy. Delete the branch.
git checkout main
git pull
git branch -d feat/add-dashboard
---
rules: 1-2
branch: 4
commits: 6-8
merge: 10-12
cleanup: 13
```

{{show: github-flow-rules typewriter 2s linear}}
{{focus: rules}}
{{annotate: rules "Five rules, total"}}
GitHub Flow has five rules. This is intentional. A workflow too complex to explain in a sentence won't be followed.

{{zoom: 1.2x}}
{{focus: branch}}
{{annotate: branch "Name it what it does"}}
Branch names communicate intent. `feat/add-dashboard` is clear. `johns-branch` is not.

{{zoom: 1x}}
{{focus: merge}}
{{annotate: merge "Pull before anything"}}
After the PR merges, pull main before starting the next thing. You want the latest state.

{{focus: cleanup}}
{{annotate: cleanup "Delete merged branches"}}
Delete the branch. It served its purpose. Stale branches are noise.

---

# Step 3: Trunk-Based Development

{{clear: slide}}

```diagram:tbd-diagram
trunk [git: trunk / main]
short-branch-a [git: short-lived branch (< 1 day)]
short-branch-b [git: short-lived branch (< 1 day)]
feature-flags [service: feature flags]
production [service: production]
short-branch-a --> trunk: merge within hours
short-branch-b --> trunk: merge within hours
trunk --> production: continuous deploy
feature-flags --> production: controls visibility
{Short-lived: short-branch-a, short-branch-b}
---
main: trunk
branch-a: short-branch-a
branch-b: short-branch-b
flags: feature-flags
prod: production
```

{{show: tbd-diagram grow 0.5s spring}}
{{focus: main}}
{{annotate: main "Single source of truth"}}
In TBD, `main` is the only long-lived branch. Everything else is hours-long, not days.

{{focus: branch-a}}
{{annotate: branch-a "Lives less than a day"}}
Branches in TBD are extremely short. Some teams commit directly to main. Branches exist only to get CI to run before merging.

{{focus: flags}}
{{annotate: flags "Incomplete code hides behind flags"}}
Unfinished features ship to production but are invisible. The flag controls who sees them.

{{focus: prod}}
{{annotate: prod "Deployed constantly"}}
Merging to main triggers deployment. Multiple times per day.

{{clear: slide}}

```data:tbd-requirements type=graph layout=force
(tbd "Trunk-Based Development") -> (ci "Strong CI — tests must be fast and reliable")
(tbd "Trunk-Based Development") -> (flags "Feature flag infrastructure")
(tbd "Trunk-Based Development") -> (coverage "Good test coverage")
(tbd "Trunk-Based Development") -> (team "Experienced team")
(ci "Strong CI — tests must be fast and reliable") -> (hard "This is the hard part")
(flags "Feature flag infrastructure") -> (hard)
---
tbd: tbd
ci-req: ci
flags-req: flags
hard: hard
```

{{show: tbd-requirements slide 0.4s ease-out}}
{{focus: tbd}}
{{annotate: tbd "Highest performer ceiling"}}
TBD teams deploy more frequently and recover from failures faster. DORA research is consistent on this.

{{focus: hard}}
{{annotate: hard "Prerequisites are non-trivial"}}
The requirements are real. Committing to main multiple times per day without breaking production requires infrastructure most teams don't have on day one.

{{focus: flags-req}}
{{annotate: flags-req "LaunchDarkly, Unleash, Flipt"}}
You need a feature flag service. Rolling your own with env vars works to start, but breaks down at scale.

---

# Step 4: My recommendation

{{clear: slide}}

```data:recommendation type=graph layout=tree
(start "Your situation") -> (solo "Solo or 1-2 people?") -> (commit-main "Commit to main directly")
(start "Your situation") -> (small-team "2-10 people?") -> (github-flow "GitHub Flow + short branches")
(start "Your situation") -> (medium-team "10+ people?") -> (protected "GitHub Flow + branch protection")
(start "Your situation") -> (mature "Mature platform + strong CI?") -> (tbd "Consider TBD + feature flags")
(start "Your situation") -> (multi-version "Multiple parallel versions?") -> (gitflow "GitFlow")
---
root: start
solo-path: solo
small-path: small-team
medium-path: medium-team
mature-path: mature
multi-path: multi-version
solo-route: start, solo, commit-main
small-route: start, small-team, github-flow
multi-route: start, multi-version, gitflow
```

{{show: recommendation grow 0.5s spring}}
{{flow: solo-route}}
{{focus: commit-main}}
{{annotate: commit-main "No ceremony needed"}}
If you're working alone, a branch-and-PR workflow adds friction with no benefit. Commit to main. Keep a clean history.

{{flow: small-route}}
{{focus: github-flow}}
{{annotate: github-flow "Right default for most teams"}}
GitHub Flow with short-lived branches works for 2-person startups and scales to hundreds of contributors. GitHub itself uses it.

{{flow: multi-route}}
{{focus: gitflow}}
{{annotate: gitflow "Only for multi-version projects"}}
GitFlow is not the wrong tool — it's the wrong tool for most projects. For projects with v1.x maintenance alongside v2.x development, it fits.

{{clear: slide}}

```code:branch-age-rule lang=text
Branch age guidelines:
  Solo:         commit to main, no branches
  2-10 people:  branches live 4-48 hours
  10+ people:   branches live 1-3 days (max)
  TBD:          branches live < 4 hours

If your branch is older than 3 days, it needs to merge.
Not because it's a rule — because long-lived branches
create merge conflicts and increase review burden.
---
solo: 2
small: 3
medium: 4
tbd: 5
rule: 7-9
```

{{show: branch-age-rule typewriter 2s linear}}
{{focus: rule}}
{{annotate: rule "The friction is the signal"}}
If your branch is hard to merge because it's been open for a week, the problem isn't the conflicts. The problem is the branch was open for a week.

{{zoom: 1.2x}}
{{focus: tbd}}
{{annotate: tbd "Less than 4 hours"}}
In TBD, a branch that lives 4 hours is a long branch. The goal is to integrate continuously, not accumulate.

{{zoom: 1x}}

---

# Step 5: The shape of a good PR

{{clear: slide}}

```code:pr-template lang=markdown
## What
Add OAuth2 login with Google.

## Why
Users were abandoning registration. OAuth2 removes the
password requirement for users with a Google account.
Addresses the 34% dropoff we measured in last sprint.

## How to test
1. `npm run dev`
2. Click "Sign in with Google" on /login
3. Complete the OAuth flow
4. Verify redirect to /dashboard
5. Verify user appears in the users table

## Notes
- Google Cloud Console callback URL must be configured
  before deploying to staging
- Feature is behind `auth.oauth_google` flag
---
what: 1-2
why: 4-8
test: 10-15
notes: 17-20
```

{{show: pr-template typewriter 2s linear}}
{{focus: what}}
{{annotate: what "One sentence max"}}
The "what" should be describable in one sentence. If it takes a paragraph, the PR is too big.

{{zoom: 1.2x}}
{{focus: why}}
{{annotate: why "Business context, not code"}}
Reviewers need to understand why this change was worth making. Data is better than assertion.

{{zoom: 1x}}
{{focus: test}}
{{annotate: test "Specific steps, not vague"}}
"Test this feature" is useless. Step-by-step instructions make reviewers more likely to actually test.

{{focus: notes}}
{{annotate: notes "Deployment and operational notes"}}
Anything a deployer needs to know. Gotchas, config requirements, feature flags.

{{clear: slide}}

```data:pr-size-rules type=graph layout=force
(ideal "Ideal PR: < 200 lines") -- (max "Hard max: 400 lines")
(max "Hard max: 400 lines") -- (too-big "Over 400: split it")
(ideal "Ideal PR: < 200 lines") -> (review-time "Review: 15-20 min")
(too-big "Over 400: split it") -> (review-time-bad "Review: 2 hours, missed bugs")
(ideal "Ideal PR: < 200 lines") -> (merge-conflicts "Low conflict probability")
(too-big "Over 400: split it") -> (merge-conflicts-bad "High conflict probability")
---
good: ideal
bad: too-big
time-good: review-time
time-bad: review-time-bad
bad-time-path: too-big, review-time-bad
```

{{show: pr-size-rules grow 0.5s spring}}
{{focus: good}}
{{annotate: good "The golden zone"}}
200 lines is a rough guide. More important: one logical concern per PR.

{{focus: bad}}
{{annotate: bad "Hard to review well"}}
Reviewers skip large PRs or rubber-stamp them. The code review stops being useful.

{{flow: bad-time-path}}
{{focus: time-bad}}
{{annotate: time-bad "Cognitive overload kills quality"}}
Nobody does careful review on a 600-line PR. The bigger the PR, the fewer bugs get caught.

{{focus: time-good}}
{{annotate: time-good "Small is fast and effective"}}
A 150-line PR gets reviewed carefully, quickly. Everyone's time is respected. Merge happens the same day.
