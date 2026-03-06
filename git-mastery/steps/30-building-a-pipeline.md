---
title: Building a Pipeline
---

# Step 1: A real workflow file

{{show: pipeline-overview grow 0.5s spring}}
A single job that does everything is fragile and slow. Split it into concerns.

```code:full-workflow lang=yaml
name: CI/CD

on:
  push:
    branches: [main]
  pull_request:

jobs:
  lint:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with: { node-version: '22', cache: 'npm' }
      - run: npm ci
      - run: npm run lint
      - run: npm run typecheck

  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with: { node-version: '22', cache: 'npm' }
      - run: npm ci
      - run: npm test

  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with: { node-version: '22', cache: 'npm' }
      - run: npm ci && npm run build
      - uses: actions/upload-artifact@v4
        with: { name: dist, path: dist/ }

  deploy:
    runs-on: ubuntu-latest
    needs: [lint, test, build]
    if: github.ref == 'refs/heads/main'
    steps:
      - uses: actions/download-artifact@v4
        with: { name: dist }
      - run: ./scripts/deploy.sh
        env:
          DEPLOY_TOKEN: ${{ secrets.DEPLOY_TOKEN }}
---
lint-job: 8-16
test-job: 18-25
build-job: 27-35
deploy-job: 37-47
needs-line: 39
if-line: 40
```

{{show: full-workflow typewriter 2s linear}}
{{focus: lint-job}}
{{annotate: lint-job "Fails fast on code style"}}
Lint runs in parallel with test. It's cheaper to fail early on a lint error than to run 200 tests and then find out.

{{zoom: 1.2x}}
{{focus: needs-line}}
{{annotate: needs-line "Dependency declaration"}}
`needs: [lint, test, build]` — deploy won't even start unless all three succeed.

{{zoom: 1x}}
{{focus: if-line}}
{{annotate: if-line "Deploy only from main"}}
`if: github.ref == 'refs/heads/main'` prevents deploy from running on PRs. Tests run on PRs. Deploys run only when code lands on main.

{{clear: slide}}

```diagram:job-dag
start [github: push to main / PR]
lint-node [service: lint]
test-node [service: test]
build-node [service: build]
dep [service: deploy]
start --> lint-node
start --> test-node
start --> build-node
lint-node --> dep: needs
test-node --> dep: needs
build-node --> dep: needs
{Parallel: lint-node, test-node, build-node}
{Gate: dep}
---
start: start
lint-node: lint-node
test-node: test-node
build-node: build-node
dep: dep
start-lint: start, lint-node
start-test: start, test-node
start-build: start, build-node
lint-dep: lint-node, dep
test-dep: test-node, dep
build-dep: build-node, dep
```

{{show: job-dag grow 0.5s spring}}
{{draw: start-lint}} {{draw: start-test}} {{draw: start-build}}
{{focus: start}}
{{annotate: start "Single trigger"}}
One push or PR open triggers all four jobs at the point the trigger fires.

{{focus: lint-node}}
{{annotate: lint-node "Parallel with test and build"}}
Lint, test, and build run simultaneously. Total wall clock time is the slowest of the three — not their sum.

{{draw: lint-dep}} {{draw: test-dep}} {{draw: build-dep}}
{{focus: dep}}
{{annotate: dep "Waits for all three"}}
Deploy only starts when all three parent jobs succeed. One failure blocks it.

---

# Step 2: Matrix builds

{{clear: slide}}

```code:matrix-build lang=yaml
jobs:
  test:
    runs-on: ${{ matrix.os }}
    strategy:
      fail-fast: false
      matrix:
        node: [18, 20, 22]
        os: [ubuntu-latest, windows-latest, macos-latest]

    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with:
          node-version: ${{ matrix.node }}
      - run: npm ci
      - run: npm test
---
runs-on: 3
strategy: 4-8
node-matrix: 6
os-matrix: 7
use-matrix: 13
```

{{show: matrix-build typewriter 2s linear}}
{{focus: strategy}}
{{annotate: strategy "Combinatorial expansion"}}
Two matrix dimensions: 3 Node versions × 3 OS = 9 parallel test jobs.

{{zoom: 1.2x}}
{{focus: runs-on}}
{{annotate: runs-on "Matrix value as variable"}}
`${{ matrix.os }}` interpolates the current matrix value. Each job in the matrix gets a different `os` value.

{{zoom: 1x}}
{{focus: use-matrix}}
{{annotate: use-matrix "Node version from matrix"}}
`${{ matrix.node }}` sets the Node.js version. Job 1 uses 18, job 2 uses 20, job 3 uses 22.

{{clear: slide}}

```data:matrix-jobs type=graph layout=force
(trigger "push event") -> (ubuntu-18 "ubuntu + node 18")
(trigger "push event") -> (ubuntu-20 "ubuntu + node 20")
(trigger "push event") -> (ubuntu-22 "ubuntu + node 22")
(trigger "push event") -> (windows-18 "windows + node 18")
(trigger "push event") -> (windows-20 "windows + node 20")
(trigger "push event") -> (windows-22 "windows + node 22")
(trigger "push event") -> (mac-18 "macos + node 18")
(trigger "push event") -> (mac-20 "macos + node 20")
(trigger "push event") -> (mac-22 "macos + node 22")
---
start: trigger
u18: ubuntu-18
u20: ubuntu-20
u22: ubuntu-22
w18: windows-18
m18: mac-18
```

{{show: matrix-jobs grow 0.5s spring}}
{{focus: start}}
{{annotate: start "One push, nine test jobs"}}
A single push spawns nine simultaneous test runs.

{{focus: u18}}
{{annotate: u18 "Unique environment"}}
Each job is a different combination of OS and Node. If something breaks on Windows but not Ubuntu, this catches it.

{{focus: w18}}
{{annotate: w18 "Cross-platform coverage"}}
Windows path separators, line endings, and file permissions differ from Unix. Running on Windows finds bugs that Linux testing misses.

{{clear: slide}}

```code:matrix-exclude lang=yaml
strategy:
  matrix:
    node: [18, 20, 22]
    os: [ubuntu-latest, windows-latest, macos-latest]
    exclude:
      # Node 18 EOL support dropped on Windows
      - os: windows-latest
        node: 18
    include:
      # Extra combination not in the base matrix
      - os: ubuntu-latest
        node: 24
        experimental: true
---
exclude: 5-8
include: 9-13
experimental: 13
```

{{show: matrix-exclude typewriter 2s linear}}
{{focus: exclude}}
{{annotate: exclude "Remove specific combinations"}}
`exclude` drops combinations from the matrix. 9 jobs minus the excluded pair = 8 jobs.

{{focus: include}}
{{annotate: include "Add extra combinations"}}
`include` adds combinations outside the base matrix. Here: Node 24 on Ubuntu, marked experimental.

---

# Step 3: Secrets and environment variables

{{clear: slide}}

```code:secrets-usage lang=yaml
jobs:
  deploy:
    runs-on: ubuntu-latest
    env:
      # Non-secret: visible in logs
      NODE_ENV: production
      REGION: us-east-1

    steps:
      - name: Deploy
        run: ./scripts/deploy.sh
        env:
          # Secret: encrypted, never visible in logs
          API_KEY: ${{ secrets.DEPLOY_API_KEY }}
          DB_URL: ${{ secrets.DATABASE_URL }}

      # WRONG: secret in run command
      # - run: deploy --token=${{ secrets.TOKEN }}
      # Appears as: deploy --token=***  (masked but visible pattern)
---
non-secret: 4-7
secret: 12-14
wrong: 16-18
```

{{show: secrets-usage typewriter 2s linear}}
{{focus: non-secret}}
{{annotate: non-secret "vars.* for non-secrets"}}
Non-sensitive configuration goes in `vars.*` (set in GitHub Settings > Variables) or directly in the workflow. These appear in logs.

{{zoom: 1.2x}}
{{focus: secret}}
{{annotate: secret "secrets.* for credentials"}}
Secrets are encrypted at rest and never printed in logs. Use them for tokens, passwords, API keys.

{{zoom: 1x}}
{{focus: wrong}}
{{annotate: wrong "Never in the run command"}}
Even masked, passing secrets as command-line arguments exposes them in process lists and shell history on the runner. Use `env:` instead.

{{clear: slide}}

```code:secret-scopes lang=yaml
# Workflow-level env (available to all jobs)
env:
  APP_ENV: production

jobs:
  deploy:
    # Job-level env (available to all steps in this job)
    env:
      REGION: us-east-1

    steps:
      - name: Deploy
        # Step-level env (available only to this step)
        env:
          API_KEY: ${{ secrets.DEPLOY_API_KEY }}
        run: deploy.sh
---
workflow-env: 1-3
job-env: 6-8
step-env: 11-15
```

{{show: secret-scopes typewriter 2s linear}}
{{focus: workflow-env}}
{{annotate: workflow-env "All jobs see this"}}
Workflow-level `env` values are available to every job and step in the file.

{{focus: step-env}}
{{annotate: step-env "Minimum scope for secrets"}}
Put secrets at the step level — the minimum scope needed. Don't expose them to steps that don't need them.

{{zoom: 1.2x}}
{{focus: job-env}}
{{annotate: job-env "Available to all steps in the job"}}
Job-level env is the right place for non-sensitive config that multiple steps in the job need.

{{zoom: 1x}}

---

# Step 4: Caching

{{clear: slide}}

```code:cache-step lang=yaml
- uses: actions/cache@v4
  with:
    path: ~/.npm
    key: ${{ runner.os }}-node-${{ hashFiles('**/package-lock.json') }}
    restore-keys: |
      ${{ runner.os }}-node-

# Then install normally
- run: npm ci
---
cache: 1-7
install: 9-10
```

{{show: cache-step typewriter 2s linear}}
{{focus: cache}}
{{annotate: cache "Key is a content hash"}}
The cache key is a hash of `package-lock.json`. If the lockfile hasn't changed, the key matches, the cache is restored, and `npm ci` is fast.

{{zoom: 1.3x}}
{{focus: install}}
{{annotate: install "npm ci still runs"}}
Even with a cache hit, `npm ci` runs — but it restores from cache instead of downloading from the registry. Much faster.

{{zoom: 1x}}
{{clear: slide}}

```data:cache-flow type=graph layout=tree
(push "push event") -> (check "Check cache: key = os + hash(lockfile)") -> (hit "Cache HIT") -> (restore "Restore ~/.npm") -> (install-fast "npm ci in ~10s")
(check "Check cache: key = os + hash(lockfile)") -> (miss "Cache MISS") -> (download "Download all packages") -> (install-slow "npm ci in ~90s") -> (save "Save ~/.npm to cache")
---
start: push
check-node: check
hit-node: hit
miss-node: miss
fast: install-fast
slow: install-slow
save-node: save
hit-path: push, check, hit, restore, install-fast
miss-path: check, miss, download, install-slow, save
```

{{show: cache-flow grow 0.5s spring}}
{{flow: hit-path}}
{{flow: miss-path}}
{{focus: hit-node}}
{{annotate: hit-node "~10 seconds"}}
Cache hit: restore takes seconds. `npm ci` verifies what's already there. Typical CI run drops from 90 seconds to 10.

{{focus: miss-node}}
{{annotate: miss-node "First run or lockfile changed"}}
Cache misses happen on first run or when `package-lock.json` changes. The package download is slow once — then cached.

{{focus: save-node}}
{{annotate: save-node "Cache saved after a miss"}}
After a miss, the cache is saved so the next run is a hit. The miss only costs you once per lockfile version.

{{clear: slide}}

```code:cache-keys lang=yaml
# Node (npm)
key: ${{ runner.os }}-node-${{ hashFiles('**/package-lock.json') }}
path: ~/.npm

# Node (pnpm)
key: ${{ runner.os }}-pnpm-${{ hashFiles('**/pnpm-lock.yaml') }}
path: ~/.local/share/pnpm/store

# Rust (cargo)
key: ${{ runner.os }}-cargo-${{ hashFiles('**/Cargo.lock') }}
path: |
  ~/.cargo/registry
  ~/.cargo/git
  target/

# Python (pip)
key: ${{ runner.os }}-pip-${{ hashFiles('**/requirements*.txt') }}
path: ~/.cache/pip
---
npm-key: 1-3
pnpm-key: 5-7
cargo-key: 9-13
pip-key: 15-17
```

{{show: cache-keys typewriter 2s linear}}
{{focus: npm-key}}
{{annotate: npm-key "Include OS in key"}}
Include `runner.os` in every cache key. The Windows npm cache is not compatible with the Ubuntu npm cache.

{{focus: cargo-key}}
{{annotate: cargo-key "Rust has three cache paths"}}
Cargo has multiple cache directories. Cache all three: the registry, git sources, and the compiled target directory.

---

# Step 5: Artifacts

{{clear: slide}}

```code:artifact-pattern lang=yaml
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with: { node-version: '22', cache: 'npm' }
      - run: npm ci && npm run build
      - uses: actions/upload-artifact@v4
        with:
          name: dist-files
          path: dist/
          retention-days: 7

  deploy:
    runs-on: ubuntu-latest
    needs: build
    steps:
      - uses: actions/download-artifact@v4
        with:
          name: dist-files
          path: dist/
      - run: ./scripts/deploy.sh
---
upload: 9-13
retention: 13
download: 17-21
deploy-cmd: 22
```

{{show: artifact-pattern typewriter 2s linear}}
{{focus: upload}}
{{annotate: upload "Save build output"}}
`upload-artifact` persists the `dist/` directory after the build job. It's available to later jobs and as a download from the GitHub UI.

{{zoom: 1.2x}}
{{focus: retention}}
{{annotate: retention "Artifacts expire"}}
Artifacts are stored for 90 days by default. Set `retention-days` to save storage costs for artifacts you only need briefly.

{{zoom: 1x}}
{{focus: download}}
{{annotate: download "Retrieve in deploy job"}}
`download-artifact` retrieves what upload saved. The deploy job doesn't need to rebuild — it uses the already-built output.

{{clear: slide}}

```data:artifact-flow type=graph layout=tree
(build-job "build job") -> (upload-step "upload-artifact: dist-files") -> (github-storage "GitHub artifact storage") -> (download-step "download-artifact: dist-files") -> (deploy-job "deploy job")
---
build: build-job
up: upload-step
storage: github-storage
down: download-step
deploy: deploy-job
path: build-job, upload-step, github-storage, download-step, deploy-job
```

{{show: artifact-flow grow 0.5s spring}}
{{flow: path}}
{{focus: storage}}
{{annotate: storage "Survives job boundaries"}}
Artifacts live in GitHub's storage. Jobs run on separate VMs. The only way to pass files between jobs is through artifact storage.

{{focus: deploy}}
{{annotate: deploy "Downloads exact build output"}}
The deploy job gets the exact files the build job produced — not a rebuilt version. What was tested is what gets deployed.
