---
title: What GitHub Adds
---

# Step 1: Git vs GitHub

{{show: git-vs-github grow 0.5s spring}}
These are two different things. Confusing them is extremely common.

```diagram:git-vs-github-diagram
git-cli [git: git CLI]
local-repo [storage: .git/ object store]
network [service: SSH / HTTPS]
github-server [github: GitHub server]
github-features [service: Issues · PRs · Actions]
git-cli --> local-repo
git-cli --> network
network --> github-server
github-server --> github-features
{Your machine: git-cli, local-repo}
{GitHub cloud: github-server, github-features}
---
git-node: git-cli
local: local-repo
net: network
github: github-server
features: github-features
```

{{show: git-vs-github-diagram grow 0.5s spring}}
{{focus: git-node}}
{{annotate: git-node "Local CLI tool"}}
Git is a command-line tool that runs on your machine. It manages the object store and history. It works without a network connection.

{{focus: github}}
{{annotate: github "Cloud hosting + web UI"}}
GitHub is a web service that hosts git repositories. It adds authentication, a web UI, and collaboration features that git itself doesn't have.

{{focus: net}}
{{annotate: net "Git speaks over SSH or HTTPS"}}
Git's `push` and `fetch` commands use SSH or HTTPS to communicate with remote servers. GitHub is one such server.

{{clear: slide}}

```diagram:alternatives
gh [github: GitHub]
gl [gitlab: GitLab]
bb [service: Bitbucket]
gt [service: Gitea]
sh [service: Sourcehut]
gh --> gh-best [service: Largest open source community]
gl --> gl-best [service: Strong built-in CI/CD]
bb --> bb-best [service: Atlassian integration]
gt --> gt-best [service: Lightweight self-hosted]
sh --> sh-best [service: Minimal, email-based]
{Cloud hosts: gh, gl, bb}
{Self-hosted: gt, sh}
---
gh-node: gh
gl-node: gl
bb-node: bb
gt-node: gt
sh-node: sh
```

{{show: alternatives slide 0.4s ease-out}}
{{focus: gh-node}}
{{annotate: gh-node "Largest open source community"}}
GitHub has the largest community, most open source projects, and the most integrations. It's the default for public projects.

{{focus: gl-node}}
{{annotate: gl-node "GitLab calls PRs 'merge requests'"}}
GitLab calls pull requests "merge requests." Same concept, different name. GitLab has stronger built-in CI/CD and is the only major platform you can self-host at enterprise scale for free.

{{focus: gt-node}}
{{annotate: gt-node "Self-host your own"}}
Gitea is a lightweight git server you can run yourself. Similar UI to GitHub. Good for organizations that can't use cloud hosting.

---

# Step 2: What GitHub adds that git doesn't have

{{clear: slide}}

```diagram:github-features-diagram
github-core [github: GitHub]
issues [service: Issues]
pull-requests [service: Pull Requests]
actions [service: Actions CI/CD]
packages [storage: Packages]
pages [service: Pages]
projects [service: Projects]
github-core --> issues
github-core --> pull-requests
github-core --> actions
github-core --> packages
github-core --> pages
github-core --> projects
pull-requests --> actions
issues --> pull-requests
---
core: github-core
issue-node: issues
pr-node: pull-requests
actions-node: actions
packages-node: packages
pages-node: pages
projects-node: projects
```

{{show: github-features-diagram grow 0.5s spring}}
{{focus: issue-node}}
{{annotate: issue-node "Bug tracker + feature requests"}}
Issues track bugs, feature requests, and discussions. They link to code, commits, and PRs.

{{focus: pr-node}}
{{annotate: pr-node "Code review with line comments"}}
Pull requests are GitHub's core feature: a UI for reviewing diffs, commenting on specific lines, approving, and merging.

{{focus: actions-node}}
{{annotate: actions-node "CI/CD triggered by git events"}}
GitHub Actions runs automated workflows on push, PR open, schedule, or manual trigger. Covered in depth later.

{{focus: packages-node}}
{{annotate: packages-node "Artifact registry"}}
GitHub Packages hosts npm packages, Docker images, Maven artifacts, and more — tied to your repository.

{{focus: pages-node}}
{{annotate: pages-node "Free static hosting"}}
GitHub Pages hosts static sites from a repository branch. Your docs, portfolio, or project site — free, deployed on push.

---

# Step 3: SSH keys and authentication

{{clear: slide}}

```code:ssh-setup lang=bash
# Generate an ed25519 key pair
ssh-keygen -t ed25519 -C "ada@company.com"
# Generating public/private ed25519 key pair.
# Enter file in which to save the key (~/.ssh/id_ed25519):
# Enter passphrase (empty for no passphrase): [use one]
# Your identification has been saved in ~/.ssh/id_ed25519
# Your public key has been saved in ~/.ssh/id_ed25519.pub

# Print the PUBLIC key — copy this
cat ~/.ssh/id_ed25519.pub
# ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAA... ada@company.com

# Test authentication
ssh -T git@github.com
# Hi ada! You've successfully authenticated.
---
keygen: 1-8
pub-key: 10-12
test: 14-16
```

{{show: ssh-setup typewriter 2s linear}}
{{focus: keygen}}
{{annotate: keygen "ed25519 is the right algorithm"}}
Use ed25519, not RSA. It's smaller, faster, and more secure. `-C` is just a comment — your email by convention.

{{zoom: 1.2x}}
{{focus: pub-key}}
{{annotate: pub-key "This is safe to share"}}
The public key (`id_ed25519.pub`) can be shared freely. Add it to GitHub in Settings > SSH and GPG keys. Never share the private key.

{{zoom: 1x}}
{{focus: test}}
{{annotate: test "Verify before using"}}
If you see "successfully authenticated," everything is configured correctly. If you see "permission denied," the key isn't added yet.

{{clear: slide}}

```data:ssh-vs-https type=graph layout=force
(ssh "SSH authentication") -- (https "HTTPS + token authentication")
(ssh "SSH authentication") -> (ssh-url "git@github.com:user/repo.git")
(https "HTTPS + token authentication") -> (https-url "https://github.com/user/repo.git")
(ssh "SSH authentication") -> (ssh-best "Best for daily use")
(https "HTTPS + token authentication") -> (https-ok "OK for CI/CD with PATs")
(ssh-best "Best for daily use") -> (no-expire "Key never expires")
(https-ok "OK for CI/CD with PATs") -> (expire "PATs expire, need rotation")
---
ssh-node: ssh
https-node: https
ssh-url-node: ssh-url
https-url-node: https-url
best: ssh-best
expire: expire
```

{{show: ssh-vs-https grow 0.5s spring}}
{{focus: ssh-url-node}}
{{annotate: ssh-url-node "SSH remote URL format"}}
SSH remotes use the `git@github.com:` prefix. HTTPS remotes use `https://`. You can switch with `git remote set-url`.

{{focus: best}}
{{annotate: best "SSH for humans"}}
SSH keys don't expire. You set them up once and they just work. No token rotation, no credential manager.

{{focus: expire}}
{{annotate: expire "Tokens expire by design"}}
Personal access tokens expire. This is a security feature, not a bug. Rotate them on a schedule.

---

# Step 4: The GitHub web UI for history

{{clear: slide}}

```code:github-blame lang=bash
# CLI equivalent of GitHub's blame view
git blame src/auth/oauth.ts

# GitHub's blame view adds:
# - Click a commit to see the full diff
# - Click a line to see the commit message
# - Navigate backwards through blame at any point
# - "Ignore whitespace" toggle
# - "Ignore revs" for bulk-format commits

# The GitHub file history view shows:
# - Every commit that touched this file
# - Side-by-side and unified diff views
# - Which PR introduced each commit
---
blame: 1-2
blame-extras: 4-9
history: 11-14
```

{{show: github-blame typewriter 2s linear}}
{{focus: blame}}
{{annotate: blame "Same data as the CLI"}}
GitHub's blame view shows the same data as `git blame`. The advantage is navigability — clicking links through the history.

{{zoom: 1.2x}}
{{focus: blame-extras}}
{{annotate: blame-extras "Navigate at any point"}}
You can go back through time: view the blame as of any historical commit. The CLI can do this too, but it requires flags.

{{zoom: 1x}}
{{focus: history}}
{{annotate: history "File history is powerful"}}
The file history view shows every commit that touched a file, who wrote it, when, and the full diff. This is the GitHub alternative to `git log --follow`.

{{clear: slide}}

```diagram:github-ui-flow
code-view [github: Code view]
blame-view [github: Blame view]
commit-view [github: Commit view]
pr-view [github: PR view]
search [github: Search]
code-view --> blame-view: who wrote this?
blame-view --> commit-view: what changed?
commit-view --> pr-view: why was this merged?
code-view --> search
pr-view --> code-view: see the result
---
code: code-view
blame: blame-view
commit: commit-view
pr: pr-view
search-node: search
nav-path: code-view, blame-view, commit-view, pr-view
```

{{show: github-ui-flow grow 0.5s spring}}
{{flow: nav-path}}
{{focus: blame}}
{{annotate: blame "Who wrote this line"}}
Start at any line in the code view and immediately navigate to blame.

{{focus: commit}}
{{annotate: commit "What changed and why"}}
From blame, navigate to the full commit. See the complete diff and commit message.

{{focus: pr}}
{{annotate: pr "What was the conversation"}}
From the commit, navigate to the PR. See the review discussion and why the decision was made.

{{focus: search-node}}
{{annotate: search-node "Search across everything"}}
GitHub's search covers code, commits, issues, PRs, and comments. `org:company is:pr is:merged fix` finds all merged fix PRs in your org.

---

# Step 5: GitHub repositories — structure and settings

{{clear: slide}}

```code:repo-settings lang=text
Repository structure:
  README.md          ← auto-rendered on homepage
  LICENSE            ← required for open source (MIT, Apache 2.0, GPL)
  .gitignore         ← specify ignored files
  .github/           ← GitHub-specific configuration
    workflows/       ← Actions CI/CD pipelines
    CODEOWNERS       ← auto-reviewer rules
    PULL_REQUEST_TEMPLATE.md
    ISSUE_TEMPLATE/
      bug-report.yml
      feature-request.yml
    FUNDING.yml      ← GitHub Sponsors config
    dependabot.yml   ← automated dependency updates
---
readme: 2
license: 3
gitignore: 4
dot-github: 5-13
```

{{show: repo-settings typewriter 2s linear}}
{{focus: readme}}
{{annotate: readme "First thing visitors see"}}
README.md is auto-rendered on the repository homepage. It's your project's front door. Keep it accurate.

{{zoom: 1.2x}}
{{focus: license}}
{{annotate: license "Required for open source"}}
Without a LICENSE file, your code is legally "all rights reserved" by default. No one can legally use it. Pick a license or no one can contribute.

{{zoom: 1x}}
{{focus: dot-github}}
{{annotate: dot-github "GitHub-specific configuration"}}
The `.github/` directory is read by GitHub's servers. None of it affects git itself — it's purely for GitHub features.

{{clear: slide}}

```data:repo-visibility type=graph layout=force
(public "Public") -- (private "Private") -- (internal "Internal")
(public "Public") -> (pub-rule "Anyone can see and fork")
(private "Private") -> (priv-rule "Only collaborators")
(internal "Internal") -> (int-rule "All org members (GitHub Enterprise)")
(pub-rule "Anyone can see and fork") -> (open-source "Good for open source")
(priv-rule "Only collaborators") -> (closed-source "Good for closed source")
---
pub: public
priv: private
int: internal
os: open-source
cs: closed-source
```

{{show: repo-visibility grow 0.5s spring}}
{{focus: pub}}
{{annotate: pub "Discoverable, forkable"}}
Public repos are visible to everyone, indexed by search engines, and can be forked by anyone. The default for open source.

{{focus: priv}}
{{annotate: priv "Invitation only"}}
Private repos require explicit collaboration access. Use for closed-source, early-stage work, or anything you're not ready to share.

{{focus: int}}
{{annotate: int "GitHub Enterprise only"}}
Internal visibility means all members of your GitHub organization can see it. Not available on personal accounts — requires GitHub Enterprise.
