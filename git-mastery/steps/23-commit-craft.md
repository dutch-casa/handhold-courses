---
title: Commit Craft
---

# Step 1: Why commit messages matter

{{show: bad-log grow 0.5s spring}}
Two teams. Same codebase. Different commit disciplines.

```code:bad-log lang=bash
git log --oneline
# a4f2c1b fix stuff
# 3d7a9b2 wip
# 8f3a2b1 changes
# 4d2c9e0 more changes
# 9e3a7b1 wip2
# 6f2c4d0 asdfgh
# 5d1b8e3 ok i think this works
# 2b8a0f3 finally
---
all-bad: 1-9
```

{{show: bad-log typewriter 2s linear}}
{{focus: all-bad}}
{{annotate: all-bad "Useless for archaeology"}}
You cannot search this log. You cannot bisect with it. You cannot auto-generate a changelog from it. It's a pile of timestamps with no meaning.

{{clear: slide}}

```code:good-log lang=bash
git log --oneline
# a4f2c1b feat(auth): add OAuth2 login with Google
# 3d7a9b2 fix(payments): correct rounding on EUR amounts
# 8f3a2b1 refactor(api): extract rate-limiting to middleware
# 4d2c9e0 test(payments): add integration tests for refunds
# 9e3a7b1 feat(dashboard): add 30-day retention chart
# 6f2c4d0 chore: update dependencies to address CVE-2025-1234
# 5d1b8e3 fix(auth): handle token expiry on refresh
# 2b8a0f3 perf(db): add index on orders.created_at
---
searchable: 1-9
```

{{show: good-log typewriter 2s linear}}
{{focus: searchable}}
{{annotate: searchable "Every line tells a story"}}
`git log --grep="payments"` finds every payment-related commit instantly. Bisect can stop at any of these and understand what changed. A changelog tool can read this.

{{clear: slide}}

```data:commit-value type=graph layout=force
(good-commits "Good commit messages") -> (archaeology "Log archaeology")
(good-commits "Good commit messages") -> (bisect "Effective bisect")
(good-commits "Good commit messages") -> (changelog "Auto changelog")
(good-commits "Good commit messages") -> (review "PR review context")
(good-commits "Good commit messages") -> (debug "Debugging context")
(bad-commits "Bad commit messages") -> (noise "None of the above")
---
good: good-commits
bad: bad-commits
archaeology-node: archaeology
none: noise
good-to-arch: good-commits, archaeology
good-to-bisect: good-commits, bisect
good-to-changelog: good-commits, changelog
```

{{show: commit-value grow 0.5s spring}}
{{focus: good}}
{{annotate: good "Documentation in the DAG"}}
Your commits are documentation. They live forever in the repo history.

{{flow: good-to-arch}}
{{flow: good-to-bisect}}
{{flow: good-to-changelog}}
{{focus: none}}
{{annotate: none "Dead weight"}}
Bad commit messages are worse than no message — they waste the space and teach nothing.

Write them for the person debugging your code 2 years from now. That person might be you.

---

# Step 2: The 7 rules

{{clear: slide}}

```code:good-commit lang=text
feat(auth): add OAuth2 login with Google

Users were abandoning the registration flow because of the
password requirement. OAuth2 reduces friction to a single
click for users who already have a Google account.

Adds the google-auth-library dependency. The callback URL
must be configured in Google Cloud Console before deploying.

Closes #482
Refs #301
---
subject: 1
blank: 2
body: 3-6
why-detail: 7-9
footer: 10-11
```

{{show: good-commit typewriter 2s linear}}
{{zoom: 1.3x}}
{{focus: subject}}
{{annotate: subject "50 chars max, imperative mood"}}
The subject line is a command: "add OAuth2 login." Not "added," not "adds." Imperative mood.

{{focus: blank}}
{{annotate: blank "Required separator"}}
The blank line separates subject from body. Without it, git treats everything as the subject.

{{zoom: 1x}}
{{focus: body}}
{{annotate: body "The WHY — not the HOW"}}
The body explains why this change was made. The code shows how. Don't repeat the code in prose.

{{zoom: 1.2x}}
{{focus: why-detail}}
{{annotate: why-detail "Deployment note — invaluable"}}
This kind of operational detail lives here forever. Future deployers will thank you.

{{zoom: 1x}}
{{focus: footer}}
{{annotate: footer "Link to issues"}}
Footer links close issues automatically on merge and provide cross-references.

{{clear: slide}}

```data:seven-rules type=array
["Separate subject from body", "Limit subject to 50 characters", "Capitalize the subject line", "No period at end of subject", "Imperative mood in subject line", "Wrap body at 72 characters", "Body explains what and why"]
---
rule1: 0
rule2: 1
rule3: 2
rule4: 3
rule5: 4
rule6: 5
rule7: 6
path: 0, 1, 2, 3, 4, 5, 6
```

{{show: seven-rules grow 0.5s spring}}
{{flow: path}}
{{focus: rule5}}
{{annotate: rule5 "Most important rule"}}
Imperative mood matters most. "Add feature" reads like a command in the git log. "Added feature" reads like a diary entry.

{{focus: rule7}}
{{annotate: rule7 "Code shows how — commit shows why"}}
The most valuable information in a commit message is the reasoning. The diff already shows what changed.

{{focus: rule2}}
{{annotate: rule2 "Terminal width constraint"}}
50 characters fits in a single terminal line without wrapping. Long subjects get truncated in most tools.

---

# Step 3: Conventional Commits

{{clear: slide}}

```code:conventional-format lang=text
<type>(<scope>): <description>

[optional body]

[optional footer(s)]

# Examples:
feat(auth): add OAuth2 login with Google
fix(payments): correct rounding on EUR amounts
chore: update dependencies
docs(api): add rate limiting documentation
test(auth): add integration tests for token refresh
refactor(db): extract query builder to separate module
perf(images): switch to WebP format for thumbnails
ci: add matrix build for Node 18/20/22
build: migrate from webpack to vite
---
format: 1-5
examples: 7-16
```

{{show: conventional-format typewriter 2s linear}}
{{focus: format}}
{{annotate: format "Type, scope, description"}}
Three parts: the type (what kind of change), the optional scope (what part of the codebase), and the description (what changed).

{{zoom: 1.2x}}
{{focus: examples}}
{{annotate: examples "Nine standard types"}}
`feat` and `fix` are the main ones. The others categorize work: `test`, `docs`, `refactor`, `perf`, `ci`, `build`, `chore`.

{{zoom: 1x}}
{{clear: slide}}

```code:breaking-changes lang=text
# Breaking change with !
feat!: drop Node.js 16 support

BREAKING CHANGE: Node.js 16 reached end-of-life.
Minimum supported version is now Node.js 18.

# Breaking change in footer
feat(api): redesign authentication endpoints

BREAKING CHANGE: /auth/login now returns a JWT in the
response body instead of setting a cookie. Clients must
be updated to read the Authorization header.
---
exclamation: 1-2
breaking-footer-header: 4-5
breaking-footer: 7-12
```

{{show: breaking-changes typewriter 2s linear}}
{{focus: exclamation}}
{{annotate: exclamation "! signals major version bump"}}
The `!` suffix marks a breaking change. Tools that read conventional commits know to bump the major version.

{{zoom: 1.2x}}
{{focus: breaking-footer}}
{{annotate: breaking-footer "Full explanation in footer"}}
The `BREAKING CHANGE:` footer explains what broke and how to migrate. This is what gets auto-generated into the changelog.

{{zoom: 1x}}
{{clear: slide}}

```code:changelog-output lang=markdown
# Changelog

## [2.0.0] - 2025-03-01

### Breaking Changes
- **api:** redesign authentication endpoints — JWT in body instead of cookie

### Features
- **auth:** add OAuth2 login with Google
- **dashboard:** add 30-day retention chart

### Bug Fixes
- **payments:** correct rounding on EUR amounts
- **auth:** handle token expiry on refresh

### Performance
- **images:** switch to WebP format for thumbnails
---
version: 3
breaking: 5-6
features: 8-10
fixes: 12-14
perf: 16-17
```

{{show: changelog-output typewriter 2s linear}}
{{focus: version}}
{{annotate: version "Bumped from commit types"}}
The major version bumped because a breaking change was present. Tools like `semantic-release` do this automatically.

{{focus: breaking}}
{{annotate: breaking "Auto-generated from BREAKING CHANGE:"}}
This section was generated from the `BREAKING CHANGE:` footer in the commits.

{{focus: features}}
{{annotate: features "Auto-generated from feat: commits"}}
Every `feat:` commit appears here. Scopes become bold prefixes.

{{focus: fixes}}
{{annotate: fixes "Auto-generated from fix: commits"}}
Zero manual editing. The changelog is a mechanical transformation of well-written commits.

---

# Step 4: Atomic commits

{{clear: slide}}

```code:non-atomic lang=bash
git log --oneline feature/user-dashboard
# 4a2c9f1 WIP dashboard + fix auth bug + update deps + cleanup
---
bad-commit: 2
```

{{show: non-atomic typewriter 2s linear}}
{{focus: bad-commit}}
{{annotate: bad-commit "Four things in one commit"}}
This commit is untestable, unsplittable, and un-bisectable. If the auth fix needs cherry-picking, you get the dashboard half-work too.

{{clear: slide}}

```code:atomic lang=bash
git log --oneline feature/user-dashboard
# 8f3a2b1 chore: update dependencies
# 4d2c9e0 fix(auth): handle expired refresh tokens
# 9e3a7b1 feat(dashboard): add user retention chart
# 6f2c4d0 feat(dashboard): add dashboard layout and routing
---
dep-update: 2
auth-fix: 3
chart: 4
layout: 5
```

{{show: atomic typewriter 2s linear}}
{{focus: auth-fix}}
{{annotate: auth-fix "Cherry-pickable"}}
The auth fix is isolated. You can `git cherry-pick` it to main without touching the dashboard work.

{{focus: dep-update}}
{{annotate: dep-update "Dependency update stands alone"}}
Dependency updates are separate from feature work. Bisect can skip them if they break something.

{{focus: chart}}
{{annotate: chart "Bisect stops here cleanly"}}
If the retention chart introduces a regression, bisect will stop exactly at this commit. The message explains what changed.

{{clear: slide}}

```code:git-add-patch lang=bash
# You've edited three files with two separate concerns.
# Use -p to stage only the hunks you want.
git add -p

# diff --git a/src/auth/token.ts b/src/auth/token.ts
# Stage this hunk [y,n,q,a,d,e,?]? y

# diff --git a/src/dashboard/chart.tsx b/src/dashboard/chart.tsx
# Stage this hunk [y,n,q,a,d,e,?]? n

git commit -m "fix(auth): handle expired refresh tokens"
git add -p
git commit -m "feat(dashboard): add user retention chart"
---
p-flag: 1-3
auth-hunk: 5-6
dash-hunk: 8-9
first-commit: 11
second-add: 12
second-commit: 13
```

{{show: git-add-patch typewriter 2s linear}}
{{focus: p-flag}}
{{annotate: p-flag "Interactive staging"}}
`git add -p` shows each hunk of each diff and asks: stage this? `y` to stage, `n` to skip.

{{zoom: 1.2x}}
{{focus: auth-hunk}}
{{annotate: auth-hunk "Stage just this fix"}}
Stage the auth-related hunk. Skip the dashboard hunk.

{{zoom: 1x}}
{{focus: dash-hunk}}
{{annotate: dash-hunk "Skip the dashboard work"}}
The dashboard changes stay unstaged. You'll commit them separately.

{{focus: first-commit}}
{{annotate: first-commit "Atomic commit one"}}
Commit only what's staged. One logical change.

---

# Step 5: Git hooks for enforcement

{{clear: slide}}

```code:commit-msg-hook lang=bash
#!/usr/bin/env bash
# .git/hooks/commit-msg
# Validates Conventional Commit format

COMMIT_MSG_FILE="$1"
COMMIT_MSG=$(cat "$COMMIT_MSG_FILE")

PATTERN="^(feat|fix|chore|docs|test|refactor|perf|ci|build)(\(.+\))?(!)?: .{1,72}"

if ! echo "$COMMIT_MSG" | grep -qE "$PATTERN"; then
  echo "ERROR: commit message must follow Conventional Commits format."
  echo "  Example: feat(auth): add OAuth2 login"
  echo "  Got: $COMMIT_MSG"
  exit 1
fi
---
shebang: 1
hook-name: 2
read-msg: 5-6
pattern: 8
check: 10-14
```

{{show: commit-msg-hook typewriter 2s linear}}
{{focus: hook-name}}
{{annotate: hook-name "commit-msg runs after you write"}}
The `commit-msg` hook receives the message file as `$1`. It runs after you write the message but before the commit is created.

{{zoom: 1.2x}}
{{focus: pattern}}
{{annotate: pattern "Validates the format"}}
This regex validates the type, optional scope, optional `!`, and description length.

{{zoom: 1x}}
{{focus: check}}
{{annotate: check "Exit 1 rejects the commit"}}
Exit non-zero to reject. Git will not create the commit. The message is preserved so you can fix it.

{{clear: slide}}

```diagram:hooks-diagram
commit-msg-hook [hook]
pre-commit-hook [hook]
prepare-commit-msg [hook]
post-commit [hook]
git-commit [action]
commit-msg-hook --> git-commit
pre-commit-hook --> git-commit
prepare-commit-msg --> git-commit
git-commit --> post-commit
{Group: commit-msg-hook, pre-commit-hook, prepare-commit-msg}
---
pre: pre-commit-hook
msg: commit-msg-hook
prepare: prepare-commit-msg
action: git-commit
after: post-commit
```

{{show: hooks-diagram grow 0.5s spring}}
{{focus: pre}}
{{annotate: pre "Runs before editor opens"}}
`pre-commit` runs before the editor opens. Use it for lint, format, type checking. Fast checks only — slow hooks frustrate developers.

{{focus: msg}}
{{annotate: msg "Validates the message"}}
`commit-msg` runs after you write the message. Use it for format validation.

{{focus: after}}
{{annotate: after "Runs after commit created"}}
`post-commit` can't block the commit. Use it for notifications.

{{clear: slide}}

```code:hooks-sharing lang=bash
# Hooks live in .git/hooks/ — NOT committed to the repo by default
ls .git/hooks/
# applypatch-msg  commit-msg  post-commit
# pre-commit      prepare-commit-msg

# Option 1: Husky (Node.js projects)
# .husky/commit-msg
# .husky/pre-commit

# Option 2: Lefthook (any language, faster)
# lefthook.yml defines hooks as config

# Option 3: git core.hooksPath
git config core.hooksPath .githooks
# Now .githooks/ IS committed and shared with the team
---
git-hooks: 1-4
husky: 6-8
lefthook: 10-12
core-hooks: 14-16
```

{{show: hooks-sharing typewriter 2s linear}}
{{focus: git-hooks}}
{{annotate: git-hooks "Not committed by default"}}
`.git/hooks/` is never committed. New team members don't get your hooks automatically.

{{zoom: 1.2x}}
{{focus: core-hooks}}
{{annotate: core-hooks "Best option for sharing"}}
`core.hooksPath` tells git to look in a different directory for hooks. Put that directory in version control. Everyone who clones gets the hooks — they just need to run the config command once.

{{zoom: 1x}}
{{focus: husky}}
{{annotate: husky "npm install auto-installs hooks"}}
Husky and Lefthook install hooks automatically on `npm install`. Convenient for JavaScript projects.
