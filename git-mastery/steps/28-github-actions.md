---
title: GitHub Actions
---

# Step 1: What CI/CD is

{{show: ci-problem grow 0.5s spring}}
"It works on my machine." You've heard it. You've probably said it.

```data:without-ci type=graph layout=tree
(dev-a "Dev A: works!") -> (push "git push") -> (broken "broken in production")
(dev-b "Dev B: works!") -> (push "git push")
(dev-c "Dev C: works!") -> (push "git push")
---
a: dev-a
b: dev-b
c: dev-c
commit: push
prod: broken
path: dev-a, push, broken
```

{{show: without-ci slide 0.4s ease-out}}
{{focus: a}}
{{annotate: a "Works on Ada's machine"}}
Three developers. Three local environments. Three different versions of Node, three different OS versions.

{{flow: path}}
{{focus: broken}}
{{annotate: broken "Integrated, tests never ran"}}
Nobody ran the tests. Nobody knows it's broken until a customer reports it at 2am.

{{clear: slide}}

```data:with-ci type=graph layout=tree
(push-ci "git push") -> (trigger "GitHub receives event") -> (runner "Clean VM spins up") -> (tests "Tests run") -> (pass "Pass → merge allowed") -> (deploy "Deploy")
(tests "Tests run") -> (fail "Fail → merge blocked")
^main=pass
---
push: push-ci
event: trigger
vm: runner
test: tests
pass-node: pass
fail-node: fail
deploy-node: deploy
path: push-ci, trigger, runner, tests, pass, deploy
```

{{show: with-ci grow 0.5s spring}}
{{flow: path}}
{{focus: vm}}
{{annotate: vm "Clean state every time"}}
CI runs on a fresh virtual machine. Your "works on my machine" environment is irrelevant. The clean VM is the standard.

{{focus: pass-node}}
{{annotate: pass-node "Gate before merge"}}
If tests pass, the PR is allowed to merge. If they fail, it's blocked. The gate is automatic.

{{focus: fail-node}}
{{annotate: fail-node "Fail fast, fail cheap"}}
A failure at this point costs minutes to fix. A failure in production costs hours plus customer trust.

{{clear: slide}}

```chart:dora-ci type=bar
Deploy Frequency: 208
Recovery Speed: 2604
Failure Rate: 7
---
deploy-freq: Deploy Frequency
recovery-speed: Recovery Speed
failure-rate: Failure Rate
```

{{show: dora-ci slide 0.5s ease-out}}
{{focus: deploy-freq}}
{{annotate: deploy-freq "208× more frequent deployments"}}
The DORA research number is not an estimate — it's a measured median across thousands of engineering teams. Two hundred and eight times more frequent. That means teams deploying once a quarter versus multiple times per day. CI/CD is the only variable that explains the gap.

{{focus: recovery-speed}}
{{annotate: recovery-speed "2604× faster incident recovery"}}
When something breaks, teams with CI/CD recover in minutes. Without it, recovery takes hours or days. The pipeline that catches failures before production is the same pipeline that tells you exactly what changed and when.

{{focus: failure-rate}}
{{annotate: failure-rate "Lower change failure rate"}}
Counter-intuitive: teams that deploy more often have a lower change failure rate. Smaller, frequent changes are easier to test and easier to roll back. Fear of deploying is self-defeating.

---

# Step 2: Workflow file structure

{{clear: slide}}

```code:minimal-workflow lang=yaml
# .github/workflows/ci.yml
name: CI

on:
  push:
    branches: [main]
  pull_request:

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with:
          node-version: '22'
      - run: npm ci
      - run: npm test
---
name-field: 2
on-block: 4-6
on-pr: 7
jobs-block: 9
job-name: 10
runs-on: 11
steps-block: 12-18
```

{{show: minimal-workflow typewriter 2s linear}}
{{focus: name-field}}
{{annotate: name-field "Appears in GitHub UI"}}
The `name` field appears in the Actions tab and on PR status checks. Make it descriptive.

{{zoom: 1.2x}}
{{focus: on-block}}
{{annotate: on-block "Trigger events"}}
The `on` section defines what triggers this workflow. Pushes to main AND all pull requests.

{{zoom: 1x}}
{{focus: runs-on}}
{{annotate: runs-on "What VM type"}}
`ubuntu-latest` is the cheapest and fastest. Use `windows-latest` or `macos-latest` only when you need them.

{{focus: steps-block}}
{{annotate: steps-block "Sequential commands"}}
Steps run in order. Each `uses` runs a pre-built action. Each `run` runs a shell command.

{{clear: slide}}

```diagram:workflow-structure
workflow-file [github: .github/workflows/ci.yml]
on-trigger [service: on: — triggers]
jobs-section [service: jobs: — job list]
job-definition [service: test: — one job]
steps-list [service: steps: — ordered commands]
workflow-file --> on-trigger
workflow-file --> jobs-section
jobs-section --> job-definition
job-definition --> steps-list
{Top level: on-trigger, jobs-section}
{Job level: job-definition, steps-list}
---
file: workflow-file
trigger: on-trigger
jobs: jobs-section
job: job-definition
steps: steps-list
```

{{show: workflow-structure grow 0.5s spring}}
{{focus: file}}
{{annotate: file "Lives in .github/workflows/"}}
Any `.yml` file in `.github/workflows/` is a workflow. You can have as many as you need.

{{focus: trigger}}
{{annotate: trigger "What starts the workflow"}}
The `on:` key defines triggers. Multiple triggers are supported per workflow.

{{focus: steps}}
{{annotate: steps "Ordered commands in a job"}}
Steps run sequentially. A failed step stops the job. The job failure blocks the PR.

---

# Step 3: Events and triggers

{{clear: slide}}

```code:workflow-triggers lang=yaml
on:
  # Any push to any branch
  push:

  # Push only to specific branches
  push:
    branches: [main, 'release/*']

  # PR events
  pull_request:
    types: [opened, synchronize, reopened]

  # Scheduled — cron syntax
  schedule:
    - cron: '0 9 * * 1'   # every Monday at 9am UTC

  # Manual trigger from GitHub UI
  workflow_dispatch:
    inputs:
      environment:
        description: 'Deploy to'
        required: true
        default: 'staging'
---
push-all: 2-3
push-branch: 5-7
pr-events: 9-11
schedule: 13-15
manual: 17-23
```

{{show: workflow-triggers typewriter 2s linear}}
{{focus: push-branch}}
{{annotate: push-branch "Glob patterns supported"}}
Branch filters accept exact names and glob patterns. `release/*` matches `release/v1.0`, `release/v2.0`, etc.

{{zoom: 1.2x}}
{{focus: pr-events}}
{{annotate: pr-events "Most common PR trigger"}}
`synchronize` fires when new commits are pushed to an open PR. This is how CI re-runs on every new commit.

{{zoom: 1x}}
{{focus: schedule}}
{{annotate: schedule "Standard cron syntax"}}
`0 9 * * 1` means: minute 0, hour 9, any day, any month, Monday (1). Runs at 9am UTC every Monday.

{{focus: manual}}
{{annotate: manual "Inputs appear in the UI"}}
`workflow_dispatch` adds a "Run workflow" button in the Actions tab. Inputs appear as form fields. Useful for deploy workflows.

{{clear: slide}}

```data:trigger-use-cases type=graph layout=force
(ci-trigger "push + pull_request") -> (test-job "Run tests on every change")
(schedule-trigger "schedule") -> (nightly "Nightly integration tests")
(schedule-trigger "schedule") -> (dependency-scan "Weekly dependency audit")
(manual-trigger "workflow_dispatch") -> (deploy "Manual deploy to staging")
(release-trigger "push: tags: v*") -> (release-job "Build and publish release")
---
ci: ci-trigger
sched: schedule-trigger
manual: manual-trigger
rel: release-trigger
```

{{show: trigger-use-cases grow 0.5s spring}}
{{focus: ci}}
{{annotate: ci "Default CI trigger"}}
Tests should run on every push and every PR. Both triggers together cover all code changes.

{{focus: rel}}
{{annotate: rel "Tags trigger releases"}}
A common pattern: `push: tags: ['v*']` — whenever you push a version tag, a release workflow runs. Builds the artifacts and publishes them.

---

# Step 4: Jobs and steps

{{clear: slide}}

```code:complete-test-job lang=yaml
jobs:
  test:
    runs-on: ubuntu-latest

    steps:
      # 1. Check out the repository code
      - uses: actions/checkout@v4

      # 2. Set up Node.js
      - uses: actions/setup-node@v4
        with:
          node-version: '22'
          cache: 'npm'

      # 3. Install dependencies (ci = clean install from lockfile)
      - name: Install dependencies
        run: npm ci

      # 4. Lint
      - name: Lint
        run: npm run lint

      # 5. Type check
      - name: Type check
        run: npm run typecheck

      # 6. Test
      - name: Test
        run: npm test -- --coverage

      # 7. Upload coverage report
      - uses: actions/upload-artifact@v4
        with:
          name: coverage-report
          path: coverage/
---
checkout: 5-7
setup: 9-13
install: 15-17
lint: 19-21
types: 23-25
test: 27-29
upload: 31-35
```

{{show: complete-test-job typewriter 2s linear}}
{{focus: checkout}}
{{annotate: checkout "First step always"}}
Every job starts from a clean runner with nothing on it. `actions/checkout` clones your repo into the workspace.

{{zoom: 1.2x}}
{{focus: setup}}
{{annotate: setup "Language toolchain"}}
`actions/setup-node` downloads and installs Node.js. `cache: 'npm'` caches `~/.npm` automatically.

{{zoom: 1x}}
{{focus: install}}
{{annotate: install "npm ci not npm install"}}
`npm ci` installs exactly from `package-lock.json` and removes `node_modules` first. Reproducible. Fast.

{{focus: lint}}
{{annotate: lint "Fail early on obvious problems"}}
Run lint before tests. If lint fails, tests won't run. Fast feedback, saves runner minutes.

{{focus: upload}}
{{annotate: upload "Persist artifacts after job"}}
`upload-artifact` saves files so they can be downloaded later — from another job or from the GitHub UI.

{{clear: slide}}

```code:parallel-jobs lang=yaml
jobs:
  lint:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: npm ci && npm run lint

  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: npm ci && npm test

  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: npm ci && npm run build

  deploy:
    runs-on: ubuntu-latest
    needs: [lint, test, build]
    steps:
      - uses: actions/checkout@v4
      - run: npm run deploy
---
lint-job: 2-6
test-job: 8-12
build-job: 14-18
deploy-job: 20-25
needs-field: 22
```

```diagram:jobs-dag
push-event [github: push / PR]
lint-node [service: lint]
test-node [service: test]
build-node [service: build]
deploy-node [service: deploy]
push-event --> lint-node
push-event --> test-node
push-event --> build-node
lint-node --> deploy-node: needs
test-node --> deploy-node: needs
build-node --> deploy-node: needs
{Parallel: lint-node, test-node, build-node}
{Gate: deploy-node}
---
lint: lint-node
test: test-node
build: build-node
deploy: deploy-node
gate-edges: lint-node, test-node, build-node, deploy-node
push-lint: push-event, lint-node
push-test: push-event, test-node
push-build: push-event, build-node
lint-deploy: lint-node, deploy-node
test-deploy: test-node, deploy-node
build-deploy: build-node, deploy-node
```

{{show: parallel-jobs typewriter 2s linear}}
{{focus: lint-job}}
{{annotate: lint-job "Runs in parallel with test and build"}}
Lint, test, and build run simultaneously. No waiting.

{{zoom: 1.2x}}
{{focus: needs-field}}
{{annotate: needs-field "Deploy waits for all three"}}
`needs: [lint, test, build]` means deploy only starts if all three pass. If any fails, deploy is blocked.

{{zoom: 1x}} {{clear: slide}}

{{show: jobs-dag grow 0.5s spring}}
{{draw: push-lint}} {{draw: push-test}} {{draw: push-build}}
{{annotate: lint "lint — in parallel"}} {{annotate: test "test — in parallel"}} {{annotate: build "build — in parallel"}}

{{draw: lint-deploy}} {{draw: test-deploy}} {{draw: build-deploy}}
{{focus: deploy}} {{annotate: deploy "all three must pass"}}
The deploy job has no runner until the gate closes. If lint fails, deploy never starts — even if test and build pass.

---

# Step 5: Reusable actions

{{clear: slide}}

```code:common-actions lang=yaml
# Checkout — always first
- uses: actions/checkout@v4

# Language toolchains
- uses: actions/setup-node@v4
  with: { node-version: '22' }

- uses: actions/setup-python@v5
  with: { python-version: '3.12' }

- uses: actions/setup-go@v5
  with: { go-version: '1.22' }

# Caching
- uses: actions/cache@v4
  with:
    path: ~/.npm
    key: ${{ runner.os }}-node-${{ hashFiles('**/package-lock.json') }}

# Artifacts
- uses: actions/upload-artifact@v4
  with: { name: dist, path: dist/ }

- uses: actions/download-artifact@v4
  with: { name: dist }
---
checkout: 1-2
node: 4-6
python: 8-10
go: 12-14
cache: 16-20
upload: 22-24
download: 26-28
```

{{show: common-actions typewriter 2s linear}}
{{focus: checkout}}
{{annotate: checkout "First step in every job"}}
Every job starts from a bare runner. `checkout` is always the first step.

{{focus: cache}}
{{annotate: cache "Cache key is a content hash"}}
The cache key uses a hash of the lockfile. When `package-lock.json` changes, the key changes, the cache misses, and a fresh install runs. Same lockfile = cache hit.

{{zoom: 1.2x}}
{{focus: upload}}
{{annotate: upload "Pass files between jobs"}}
`upload-artifact` saves files to GitHub's artifact storage. `download-artifact` retrieves them in a later job.

{{zoom: 1x}}
{{clear: slide}}

```code:action-security lang=yaml
# BAD: floating tag — can change at any time
- uses: actions/checkout@v4

# GOOD: pinned to a specific commit SHA
- uses: actions/checkout@11bd71901bbe5b1630ceea73d27597364c9af683

# Why: a malicious update to @v4 affects every workflow
# that uses the floating tag immediately.
# A pinned SHA cannot be changed after the fact.

# Also: don't use third-party actions from unknown publishers
# Review the source code before using a community action
---
bad: 1-2
good: 4-5
why: 7-10
third-party: 12-13
```

{{show: action-security typewriter 2s linear}}
{{focus: bad}}
{{annotate: bad "Convenient but risky"}}
`@v4` is a git tag. Tags can be moved. A compromised action maintainer could push malicious code to `@v4` and every workflow using it runs it.

{{zoom: 1.3x}}
{{focus: good}}
{{annotate: good "Immutable SHA reference"}}
A full commit SHA cannot be changed. If an action's SHA is `11bd71...`, that exact code runs forever.

{{zoom: 1x}}
{{focus: third-party}}
{{annotate: third-party "Read the source first"}}
Community actions run arbitrary code in your pipeline with access to your secrets. Review the source before adding them. Prefer official actions from `actions/` and `github/`.
