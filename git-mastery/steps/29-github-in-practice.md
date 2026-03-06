---
title: GitHub in Practice
---

# Step 1: Branch protection rules

{{show: unprotected-risk grow 0.5s spring}}
Without protection, main is one bad push away from breaking production.

```data:unprotected-risks type=graph layout=force
(main "main branch") -- (force-push "Direct force push — rewrites history")
(main "main branch") -- (delete "Branch deletion")
(main "main branch") -- (bypass "Bypass CI — skip tests")
(main "main branch") -- (unreviewed "Merge without review")
(force-push "Direct force push — rewrites history") -> (disaster "History gone")
(delete "Branch deletion") -> (disaster "History gone")
---
branch: main
force: force-push
del: delete
danger: disaster
```

{{show: unprotected-risks slide 0.4s ease-out}}
{{focus: force}}
{{annotate: force "Rewrites shared history"}}
A force push to main rewrites commits that other developers have already pulled. Everyone's local branches are now diverged from the "official" history.

{{focus: del}}
{{annotate: del "Can be recovered, but shouldn't happen"}}
Deleting main by accident is recoverable if someone has a recent clone. But it shouldn't be possible at all.

{{clear: slide}}

```code:branch-protection lang=yaml
# Branch protection settings (via GitHub UI or API)
# Settings → Branches → Add branch protection rule

Rules for: main

[x] Require a pull request before merging
    Required approvals: 1

[x] Require status checks to pass before merging
    Status checks: CI / test (ubuntu-latest)
    Status checks: CI / lint (ubuntu-latest)

[x] Require conversation resolution before merging

[x] Require linear history (no merge commits)

[x] Restrict who can push to matching branches
    Allowed: @org/release-managers

[ ] Allow force pushes  # LEAVE UNCHECKED
[ ] Allow deletions     # LEAVE UNCHECKED
---
pr-required: 3-5
status: 7-10
conversation: 12
linear: 14
restrict: 16-17
force: 19
delete: 20
```

{{show: branch-protection typewriter 2s linear}}
{{focus: pr-required}}
{{annotate: pr-required "No direct pushes to main"}}
This is the most important rule. Every change goes through a PR. Direct pushes to main are blocked.

{{zoom: 1.2x}}
{{focus: status}}
{{annotate: status "CI must be green"}}
Name the exact status checks that must pass. If CI doesn't pass, the merge button is greyed out.

{{zoom: 1x}}
{{focus: conversation}}
{{annotate: conversation "All review threads resolved"}}
Reviewers can mark comments as resolved. This rule ensures no open threads slip through on merge.

{{focus: force}}
{{annotate: force "Never allow this on main"}}
Force pushes to main should always be blocked. There is no valid reason to allow them in a team setting.

{{clear: slide}}

```diagram:protection-gate
commit [git: git push]
pr-open [github: open PR]
ci-run [service: CI checks pass]
review [github: code review approval]
merge [github: merge to main]
production [service: production deploy]
commit --> pr-open
pr-open --> ci-run
ci-run --> review
review --> merge
merge --> production
{Gates: ci-run, review}
---
c: commit
pr: pr-open
ci: ci-run
rev: review
m: merge
prod: production
path: commit, pr-open, ci-run, review, merge, production
```

{{show: protection-gate grow 0.5s spring}}
{{flow: path}}
{{focus: ci-run}}
{{annotate: ci-run "Automated gate"}}
CI is the automated gate. It runs without human intervention and cannot be bypassed.

{{focus: rev}}
{{annotate: rev "Human gate"}}
Review is the human gate. Required approvals must be granted before merge is possible.

{{focus: prod}}
{{annotate: prod "Only clean code reaches here"}}
Production only receives code that passed both gates. Every merge to main is deployable.

---

# Step 2: CODEOWNERS

{{clear: slide}}

```code:codeowners lang=text
# .github/CODEOWNERS
# Last matching rule wins.

# Default: any team member can review
* @company/engineering

# Auth and security code: security team required
src/auth/                     @company/security-team
src/middleware/auth*.ts        @company/security-team

# Payment code: two specific senior engineers
src/payments/                 @ada @ben

# Database migrations: any two of the DB team
db/migrations/                @company/db-team

# CI/CD infrastructure: platform team only
.github/                      @company/platform-team
Dockerfile                    @company/platform-team
docker-compose*.yml           @company/platform-team

# Documentation: anyone (override the default)
docs/                         @company/docs-team @company/engineering
---
default: 4-5
auth: 7-9
payments: 11-12
db: 14-15
ci: 17-20
docs: 22-23
```

{{show: codeowners typewriter 2s linear}}
{{focus: default}}
{{annotate: default "Catch-all rule"}}
`*` matches everything. Any engineer on the team can review general files. More specific rules below override this.

{{zoom: 1.2x}}
{{focus: auth}}
{{annotate: auth "Glob patterns work"}}
`auth*.ts` matches `auth.ts`, `auth-token.ts`, `auth-middleware.ts`. Both the directory and the pattern match.

{{zoom: 1x}}
{{focus: payments}}
{{annotate: payments "Named individuals or teams"}}
You can require specific people. In high-stakes code — payments, auth, billing — named reviewers are appropriate.

{{focus: ci}}
{{annotate: ci "Last matching rule wins"}}
`.github/` is more specific than `*`, so it overrides the default. Only the platform team reviews workflow changes.

{{clear: slide}}

```data:codeowners-effect type=graph layout=tree
(pr-opened "PR opened") -> (github "GitHub checks changed files") -> (matches "Matches CODEOWNERS rules") -> (adds-reviewers "Auto-adds required reviewers") -> (merge-blocked "Merge blocked until they approve")
---
start: pr-opened
gh: github
match: matches
add: adds-reviewers
block: merge-blocked
path: pr-opened, github, matches, adds-reviewers, merge-blocked
```

{{show: codeowners-effect grow 0.5s spring}}
{{flow: path}}
{{focus: adds-reviewers}}
{{annotate: adds-reviewers "Automatic — no manual step"}}
When a PR touches a CODEOWNERS file, the listed owners are automatically added as required reviewers. No one has to remember.

{{focus: merge-blocked}}
{{annotate: merge-blocked "Required reviewers must approve"}}
With branch protection requiring CODEOWNERS approval, the merge button is blocked until the required owners have approved.

---

# Step 3: Dependabot

{{clear: slide}}

```code:dependabot-config lang=yaml
# .github/dependabot.yml
version: 2

updates:
  # npm dependencies
  - package-ecosystem: "npm"
    directory: "/"
    schedule:
      interval: "weekly"
    groups:
      dev-dependencies:
        patterns: ["eslint*", "jest*", "typescript*"]
    ignore:
      - dependency-name: "some-legacy-package"
        versions: [">= 3.0"]

  # GitHub Actions
  - package-ecosystem: "github-actions"
    directory: "/"
    schedule:
      interval: "weekly"

  # Docker base images
  - package-ecosystem: "docker"
    directory: "/"
    schedule:
      interval: "monthly"
---
npm: 4-13
ignore: 14-16
actions: 18-22
docker: 24-27
```

{{show: dependabot-config typewriter 2s linear}}
{{focus: npm}}
{{annotate: npm "Weekly PR for dependency updates"}}
Dependabot opens PRs once a week to update dependencies. Each PR updates one or a group of packages.

{{zoom: 1.2x}}
{{focus: ignore}}
{{annotate: ignore "Pin problematic packages"}}
If a package has a breaking change you're not ready for, ignore it until you are. Dependabot won't open a PR for it.

{{zoom: 1x}}
{{focus: actions}}
{{annotate: actions "Update action versions too"}}
GitHub Actions themselves have versions. Dependabot keeps them updated, including pinned SHA references.

{{focus: docker}}
{{annotate: docker "Base image updates"}}
`FROM node:22` in a Dockerfile can be updated by Dependabot when a new patch release of Node 22 is available.

{{clear: slide}}

```data:dependabot-flow type=graph layout=tree
(schedule "Weekly schedule") -> (scan "Dependabot scans dependencies") -> (outdated "Finds outdated packages") -> (pr-open "Opens a PR per package (or group)") -> (ci-runs "CI runs on the PR") -> (auto-merge "Auto-merge if CI passes") -> (merged "Dependency updated")
---
sched: schedule
scan-step: scan
found: outdated
pr: pr-open
ci: ci-runs
merge: auto-merge
done: merged
path: schedule, scan, outdated, pr-open, ci-runs, auto-merge, merged
```

{{show: dependabot-flow grow 0.5s spring}}
{{flow: path}}
{{focus: auto-merge}}
{{annotate: auto-merge "Enable auto-merge for patch updates"}}
For patch and minor updates, enable auto-merge on Dependabot PRs. CI passes, auto-merge fires. Zero human time spent on routine updates.

{{focus: merged}}
{{annotate: merged "Dependencies stay current"}}
With Dependabot + auto-merge, your dependencies are updated constantly without manual work. Security vulnerabilities get patched within days.

---

# Step 4: GitHub Releases

{{clear: slide}}

```code:create-release lang=bash
# Tag the commit first
git tag -a v1.2.0 -m "Release v1.2.0"
git push origin v1.2.0

# Create release with auto-generated notes from PR titles
gh release create v1.2.0 \
  --title "v1.2.0 — Dashboard & Performance" \
  --generate-notes

# Or: attach binary artifacts
gh release create v1.2.0 \
  --generate-notes \
  dist/my-app-linux-amd64 \
  dist/my-app-darwin-arm64 \
  dist/my-app-windows-amd64.exe
---
tag: 1-3
release-notes: 5-8
with-artifacts: 10-14
```

{{show: create-release typewriter 2s linear}}
{{focus: tag}}
{{annotate: tag "Release is built on a tag"}}
GitHub releases are git tags with extra metadata. You must tag the commit first.

{{zoom: 1.2x}}
{{focus: release-notes}}
{{annotate: release-notes "--generate-notes from PR titles"}}
GitHub reads all PR titles merged since the last release and auto-generates release notes. This is why conventional commit PR titles matter.

{{zoom: 1x}}
{{focus: with-artifacts}}
{{annotate: with-artifacts "Binary downloads"}}
Binary files attached to a release appear as download links on the release page. Users download directly — no build required.

{{clear: slide}}

```code:release-page lang=markdown
## v1.2.0 — Dashboard & Performance

**Release Date:** 2025-03-01

### What's Changed

#### Features
- feat(dashboard): add 30-day retention chart by @ada in #48
- feat(auth): add OAuth2 login with Google by @ada in #45

#### Bug Fixes
- fix(payments): correct rounding on EUR amounts by @ben in #52
- fix(auth): handle token expiry on refresh by @ada in #49

#### Performance
- perf(db): add index on orders.created_at by @ben in #51

**Full Changelog:** v1.1.0...v1.2.0

### Assets
my-app-linux-amd64           (12 MB)
my-app-darwin-arm64          (11 MB)
my-app-windows-amd64.exe     (13 MB)
Source code (zip)
Source code (tar.gz)
---
version: 1
date: 3
changes: 5-16
changelog-link: 18
assets: 20-24
```

{{show: release-page typewriter 2s linear}}
{{focus: changes}}
{{annotate: changes "Auto-generated from PR titles"}}
Every section was generated from PR titles and conventional commit prefixes. This is why PR title discipline matters.

{{zoom: 1.2x}}
{{focus: changelog-link}}
{{annotate: changelog-link "Full diff link"}}
GitHub auto-generates a link comparing this release to the previous one. Every commit is one click away.

{{zoom: 1x}}
{{focus: assets}}
{{annotate: assets "Download section"}}
Binary artifacts appear here. Source code archives are always included automatically.

---

# Step 5: The .github/ directory

{{clear: slide}}

```code:dot-github lang=bash
.github/
├── CODEOWNERS                    # auto-reviewer rules
├── FUNDING.yml                   # GitHub Sponsors config
├── dependabot.yml                # automated dependency updates
├── PULL_REQUEST_TEMPLATE.md      # pre-fills PR description
├── ISSUE_TEMPLATE/               # form templates for issues
│   ├── bug-report.yml
│   ├── feature-request.yml
│   └── config.yml                # template chooser config
└── workflows/                    # GitHub Actions
    ├── ci.yml                    # test on push + PR
    ├── release.yml               # publish on tag push
    └── dependabot-auto-merge.yml # auto-merge patch updates
---
codeowners: 2
funding: 3
dependabot: 4
pr-template: 5
issue-dir: 6-9
workflows: 10-13
```

{{show: dot-github typewriter 2s linear}}
{{focus: pr-template}}
{{annotate: pr-template "Pre-fills the PR description box"}}
`PULL_REQUEST_TEMPLATE.md` contents automatically appear in the PR description box when you open a PR. Enforces structure without requiring an external tool.

{{zoom: 1.2x}}
{{focus: issue-dir}}
{{annotate: issue-dir "Different forms per issue type"}}
`ISSUE_TEMPLATE/` can contain multiple templates. GitHub shows a chooser when someone opens a new issue. Bug reports and feature requests have different fields.

{{zoom: 1x}}
{{focus: workflows}}
{{annotate: workflows "Every workflow is a .yml file"}}
Multiple workflows are fine. A typical setup: one for CI, one for releases, one for Dependabot auto-merge.

{{clear: slide}}

```code:pr-template lang=markdown
## What
<!-- One sentence: what does this change? -->

## Why
<!-- Why is this change needed? Link to issue. -->
Fixes #

## How to test
<!-- Step by step instructions. Be specific. -->
1.
2.
3.

## Screenshots
<!-- For UI changes. Delete if not applicable. -->

## Notes
<!-- Deployment notes, config changes, gotchas. -->

## Checklist
- [ ] Tests added or updated
- [ ] Documentation updated (if needed)
- [ ] Self-reviewed the diff
---
what: 1-2
why: 4-6
test: 8-12
screenshots: 14-15
notes: 17-18
checklist: 20-23
```

{{show: pr-template typewriter 2s linear}}
{{focus: what}}
{{annotate: what "Constraint: one sentence"}}
Forcing the "what" into one sentence enforces PR scope discipline. If it takes a paragraph, the PR is too big.

{{focus: why}}
{{annotate: why "Issue link is mandatory"}}
The `Fixes #` placeholder is intentional. Every PR should trace to a tracked issue.

{{focus: checklist}}
{{annotate: checklist "Author accountability"}}
The checklist is for the author, not the reviewer. It catches common omissions before the PR goes out.
