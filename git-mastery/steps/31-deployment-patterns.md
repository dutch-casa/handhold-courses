---
title: Deployment Patterns
---

# Step 1: The deployment pipeline

{{show: pipeline-diagram grow 0.5s spring}}
A deployment pipeline is a series of gates. Each gate adds confidence.

```data:deploy-pipeline type=graph layout=tree
(pr "PR opened") -> (ci-pr "CI: lint + test") -> (merge "Merge to main") -> (ci-main "CI again on main") -> (staging "Deploy to staging") -> (approval "Manual approval gate") -> (production "Deploy to production")
---
pr-node: pr
ci-pr-node: ci-pr
merge-node: merge
ci-main-node: ci-main
staging-node: staging
approval-node: approval
prod-node: production
path: pr, ci-pr, merge, ci-main, staging, approval, production
```

{{show: deploy-pipeline grow 0.5s spring}}
{{flow: path}}
{{focus: ci-pr-node}}
{{annotate: ci-pr-node "First gate: automated"}}
CI on the PR catches most regressions before code is even merged. This is the cheapest gate.

{{focus: ci-main-node}}
{{annotate: ci-main-node "Second gate: clean state"}}
Running CI again on main catches rare race conditions — two PRs that each pass individually but break when merged together.

{{focus: approval-node}}
{{annotate: approval-node "Human gate: optional"}}
Manual approval exists for regulated environments or high-risk releases. For most teams, staging smoke tests replace this.

{{focus: prod-node}}
{{annotate: prod-node "Always the same process"}}
Production deployment uses the exact same process as staging. No special cases. No "just this once."

{{clear: slide}}

```code:boring-deploys lang=text
Goal: make deployment boring.
Boring means: automated, repeatable, reversible.

Signs you've achieved boring deployments:
  - Deploying takes under 10 minutes
  - Anyone on the team can trigger it
  - Rolling back takes under 5 minutes
  - Deployments happen multiple times per day
  - No one dreads Friday deploys

Signs you haven't:
  - Deployments require a manual checklist
  - Only specific people know how to deploy
  - Rollback is manual and takes 30 minutes
  - Deployments happen once a month
  - "We don't deploy on Fridays"
---
goal: 1-2
good: 4-9
bad: 11-16
```

{{show: boring-deploys typewriter 2s linear}}
{{focus: good}}
{{annotate: good "This is the target"}}
Automated, reversible, frequent. When deployment is routine, everyone deploys. When it's scary, people avoid it.

{{zoom: 1.2x}}
{{focus: bad}}
{{annotate: bad "These are symptoms of a broken pipeline"}}
Monthly deploys mean large changes, large risk, large blast radius. Friday fear means no confidence in the rollback process.

{{zoom: 1x}}

---

# Step 2: Feature flags

{{clear: slide}}

```code:feature-flag lang=typescript
// Feature flag check — the principle is simple
function renderDashboard(user: User): JSX.Element {
  if (flags.isEnabled('new-dashboard', user)) {
    return <NewDashboard />;
  }
  return <LegacyDashboard />;
}

// The flag service handles:
// - On/off globally
// - On for specific users or orgs
// - On for a % of traffic (gradual rollout)
// - On for internal users only (dogfooding)
---
check: 1-7
capabilities: 9-13
```

{{show: feature-flag typewriter 2s linear}}
{{focus: check}}
{{annotate: check "Conditional around the feature"}}
This is the entire pattern. Incomplete code ships to production. The flag controls who sees it.

{{zoom: 1.2x}}
{{focus: capabilities}}
{{annotate: capabilities "Gradual rollout"}}
The flag service does the heavy lifting: per-user targeting, percentage rollouts, environment-based flags.

{{zoom: 1x}}
{{clear: slide}}

```data:flag-rollout type=graph layout=tree
(merged "Merged to main — behind flag") -> (internal "Enable for internal users (0%)") -> (canary "Enable for 1% of users") -> (ten-percent "Enable for 10%") -> (fifty "Enable for 50%") -> (hundred "Enable for 100% — remove flag")
---
start: merged
internal: internal
one: canary
ten: ten-percent
fifty: fifty
full: hundred
path: merged, internal, canary, ten-percent, fifty, hundred
```

{{show: flag-rollout grow 0.5s spring}}
{{flow: path}}
{{focus: internal}}
{{annotate: internal "Dogfood first"}}
Enable for internal users first. You find most bugs before any customers are affected.

{{focus: canary}}
{{annotate: canary "1% is a small blast radius"}}
If 1% of users see a bug, 99% don't. You can disable the flag immediately. No code change, no deployment.

{{focus: hundred}}
{{annotate: hundred "Then remove the flag"}}
Once at 100%, remove the flag from the code. Dead flags are technical debt — they make code harder to read.

{{clear: slide}}

```data:flag-tools type=graph layout=force
(tools "Feature Flag Tools") -- (launchdarkly "LaunchDarkly — enterprise, full-featured")
(tools "Feature Flag Tools") -- (unleash "Unleash — open source, self-hosted")
(tools "Feature Flag Tools") -- (flipt "Flipt — open source, GitOps model")
(tools "Feature Flag Tools") -- (env-vars "Plain env vars — simple, no UI, limited")
---
tool-node: tools
ld: launchdarkly
un: unleash
fl: flipt
ev: env-vars
```

{{show: flag-tools slide 0.4s ease-out}}
{{focus: ev}}
{{annotate: ev "Start here"}}
Start with environment variables. It's not sophisticated, but it works until you need per-user targeting or gradual rollouts.

{{focus: un}}
{{annotate: un "Self-hosted option"}}
Unleash is free, open source, and self-hostable. Reasonable choice when you need more than env vars but can't justify SaaS pricing.

---

# Step 3: Rollback

{{clear: slide}}

```data:bad-commit-graph type=graph layout=tree
(good-a "main: A — good") -> (good-b "main: B — good") -> (bad "main: C — BROKEN") -> (revert "main: Revert C")
^main=bad
---
a: good-a
b: good-b
bad-node: bad
revert-node: revert
path: good-a, good-b, bad, revert
```

{{show: bad-commit-graph grow 0.5s spring}}
{{focus: bad-node}}
{{annotate: bad-node "Bug is live in production"}}
Commit C introduced a bug. It deployed. Customers are affected.

{{flow: path}}
{{focus: revert-node}}
{{annotate: revert-node "Revert is the fix"}}
`git revert C` creates a new commit that undoes C's changes. Main history is preserved. CI runs on the revert. It deploys.

{{clear: slide}}

```code:rollback-options lang=bash
# Option 1: git revert — PREFERRED
# Creates a new commit that undoes the bad one
git revert HEAD
git push origin main
# CI runs → deploy → production is clean

# Option 2: Roll back the deployment directly
# (container restart, switch to previous image)
# Use this when the revert would take too long
# or the problem is in infrastructure, not code

# Option 3: Fix forward
# Write a fix, push, deploy
# Use when the issue is simple and well-understood
# NOT when you're unsure of the full impact
---
revert: 1-6
direct: 8-11
forward: 13-17
```

{{show: rollback-options typewriter 2s linear}}
{{focus: revert}}
{{annotate: revert "Cleanest — history shows what happened"}}
`git revert` is the cleanest rollback. The history shows: bad commit, then revert. Auditors, post-mortems, and future archeologists can see exactly what happened.

{{zoom: 1.2x}}
{{focus: direct}}
{{annotate: direct "Fastest in emergencies"}}
If the CI pipeline takes 10 minutes and production is on fire, roll back the deployment directly. Then create the revert commit to clean up the code.

{{zoom: 1x}}
{{focus: forward}}
{{annotate: forward "Only when you're certain"}}
Fix-forward means: push a new commit that fixes the bug. Only do this when the issue is simple, contained, and well-understood. Not when you're guessing.

{{clear: slide}}

```data:revert-after type=graph layout=tree
(before "main: A — B — C(bad) — Revert-C") -> (ci-run "CI runs on Revert-C") -> (deploy "Deploy Revert-C") -> (fixed "Production stable")
^main=before
---
state: before
ci: ci-run
dep: deploy
stable: fixed
path: before, ci-run, deploy, fixed
```

{{show: revert-after grow 0.5s spring}}
{{flow: path}}
{{focus: ci-run}}
{{annotate: ci-run "Revert gets the same treatment"}}
The revert commit goes through the full pipeline. CI validates it. This is the right approach — don't bypass CI even for rollbacks.

{{focus: stable}}
{{annotate: stable "Minutes to fix"}}
A practiced rollback via `git revert` + CI takes 10-15 minutes from "bug detected" to "production fixed." That's the target.

---

# Step 4: DORA metrics

{{clear: slide}}

```data:dora-chart type=graph layout=force
(dora "DORA Metrics") -> (df "Deployment Frequency") -> (elite-df "Elite: multiple/day")
(dora "DORA Metrics") -> (lt "Lead Time for Changes") -> (elite-lt "Elite: < 1 hour")
(dora "DORA Metrics") -> (cfr "Change Failure Rate") -> (elite-cfr "Elite: 0-15%")
(dora "DORA Metrics") -> (mttr "Time to Restore Service") -> (elite-mttr "Elite: < 1 hour")
---
root: dora
deploy-freq: df
lead-time: lt
failure-rate: cfr
restore: mttr
elite1: elite-df
elite2: elite-lt
elite3: elite-cfr
elite4: elite-mttr
```

{{show: dora-chart grow 0.5s spring}}
{{focus: deploy-freq}}
{{annotate: deploy-freq "How often to production"}}
Elite teams deploy multiple times per day. This is only possible with CI/CD and small changes. Large monthly deploys are a low-performer signal.

{{focus: lead-time}}
{{annotate: lead-time "Commit to production time"}}
Lead time measures the cycle: from "code written" to "in production." Under 1 hour is elite. Many teams measure days or weeks.

{{zoom: 1.2x}}
{{focus: failure-rate}}
{{annotate: failure-rate "% of deploys that cause incidents"}}
0-15% change failure rate at elite. Not zero — perfect is impossible. 15% means most deploys succeed, and the ones that don't are caught and fixed quickly.

{{zoom: 1x}}
{{focus: restore}}
{{annotate: restore "How fast to recover"}}
Under 1 hour to restore service. This requires: practiced rollback, good monitoring, paged-on-call rotation.

{{clear: slide}}

```code:dora-in-practice lang=text
How to improve DORA metrics:

Deployment Frequency:
  → Smaller PRs + fast CI + automated deploys
  → Stop doing "sprint releases" — deploy every merge

Lead Time:
  → Fast CI (< 10 min) + no manual approval gates for most changes
  → Short PR review SLAs (< 4 hours)

Change Failure Rate:
  → Better test coverage, especially integration tests
  → Feature flags for risky changes (deploy without releasing)

Time to Restore:
  → Practiced rollback process (revert + CI)
  → Good monitoring and alerting
  → On-call rotation with clear escalation
---
freq: 3-5
lead: 7-9
failure: 11-13
restore: 15-18
```

{{show: dora-in-practice typewriter 2s linear}}
{{focus: freq}}
{{annotate: freq "Small + automated = frequent"}}
Deployment frequency is a consequence of PR size and CI automation. Fix those, frequency goes up.

{{focus: restore}}
{{annotate: restore "Monitoring is half the battle"}}
You can't restore in under an hour if you don't know something broke. Monitoring and alerting are prerequisites for a good MTTR.

---

# Step 5: Release strategies

{{clear: slide}}

```diagram:blue-green
blue-env [service: Blue (live)]
green-env [service: Green (standby)]
load-balancer [load-balancer: Load Balancer]
traffic [client: User Traffic]
traffic --> load-balancer
load-balancer --> blue-env: 100% traffic
load-balancer --> green-env: 0% traffic
{Active: blue-env}
{Standby: green-env}
---
blue: blue-env
green: green-env
lb: load-balancer
t: traffic
```

{{show: blue-green grow 0.5s spring}}
{{focus: lb}}
{{annotate: lb "Traffic switch point"}}
Two identical environments exist simultaneously. The load balancer points to one of them. Rollback means switching the pointer.

{{focus: blue}}
{{annotate: blue "Current production"}}
Blue is the live environment serving all traffic.

{{focus: green}}
{{annotate: green "New version, no traffic yet"}}
Deploy the new version to green first. Run smoke tests. When confident, switch the load balancer to green. Blue is now the rollback target.

{{clear: slide}}

```data:canary-deploy type=graph layout=tree
(deploy "Deploy new version") -> (one-pct "Route 1% of traffic to new") -> (watch "Watch error rates and latency") -> (increase "Increase to 10%") -> (full "Route 100% to new") -> (cleanup "Remove old version")
(watch "Watch error rates and latency") -> (rollback "Error spike? Route back to old")
---
start: deploy
one: one-pct
watch-node: watch
ten: increase
hundred: full
done: cleanup
rb: rollback
path: deploy, one-pct, watch, increase, full, cleanup
rollback-path: watch, rollback
```

{{show: canary-deploy grow 0.5s spring}}
{{flow: path}}
{{focus: one-pct}}
{{annotate: one-pct "1% blast radius"}}
If the new version has a bug, only 1% of users hit it while you're monitoring. The other 99% are on the stable version.

{{focus: watch-node}}
{{annotate: watch-node "Metrics drive the decision"}}
Watch error rates, latency, and business metrics. If they look good, increase. If anything spikes, route back.

{{flow: rollback-path}}
{{focus: rollback}}
{{annotate: rollback "Route back, zero downtime"}}
Routing back to the old version takes seconds. No deployment needed. This is why canary works.

{{clear: slide}}

```data:strategy-comparison type=graph layout=force
(blue-green "Blue/Green") -- (canary "Canary") -- (flag-rollout "Feature Flag Rollout")
(blue-green "Blue/Green") -> (bg-when "When: infrastructure-level changes")
(canary "Canary") -> (c-when "When: unknown risk, need real traffic data")
(flag-rollout "Feature Flag Rollout") -> (f-when "When: application-level features")
(bg-when "When: infrastructure-level changes") -> (same-goal "Reduce blast radius when something goes wrong")
(c-when "When: unknown risk, need real traffic data") -> (same-goal)
(f-when "When: application-level features") -> (same-goal)
---
bg: blue-green
c: canary
ff: flag-rollout
goal: same-goal
```

{{show: strategy-comparison grow 0.5s spring}}
{{focus: goal}}
{{annotate: goal "One goal across all strategies"}}
Every deployment strategy has the same purpose: reduce the blast radius when something goes wrong. They differ in mechanism, not intent.

{{focus: bg}}
{{annotate: bg "Fastest rollback"}}
Blue/green has the fastest rollback — switch the load balancer pointer. Best for infrastructure changes where you want an instant escape hatch.

{{focus: ff}}
{{annotate: ff "No deployment needed to rollback"}}
Feature flags are the most granular — per-user control. Rollback is disabling the flag: instant, no deployment required.
