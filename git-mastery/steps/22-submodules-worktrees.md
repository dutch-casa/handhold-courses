---
title: Submodules and Worktrees
---

# Step 1: git worktree — two branches at once

{{show: problem-scenario grow 0.5s spring}}
You're halfway through a feature. An urgent hotfix arrives.

```data:stash-problem type=graph layout=tree
(feature "feature/dashboard — 4 hours of work") -> (stash "git stash") -> (hotfix "checkout hotfix branch") -> (fix "make the fix") -> (unstash "git stash pop") -> (conflicts "merge conflicts?")
---
start: feature
stash-node: stash
fix-node: fix
problem: conflicts
path: feature, stash, hotfix, fix, unstash, conflicts
```

{{show: problem-scenario slide 0.4s ease-out}}
{{focus: start}}
{{annotate: start "Don't stash this"}}
Stashing works. It's just slow and risky — you're interleaving two workstreams through a stack.

{{flow: path}}
{{focus: problem}}
{{annotate: problem "Stash pop can conflict"}}
If the hotfix touched the same files as your feature work, `stash pop` creates conflicts you didn't expect.

{{clear: slide}}

```code:worktree-add lang=bash
# Create a second working directory for the hotfix
git worktree add ../project-hotfix hotfix-branch

# Now you have:
# ~/project/           → feature/dashboard (your current work)
# ~/project-hotfix/    → hotfix-branch (the urgent fix)

# Work on the hotfix in a separate terminal
cd ../project-hotfix
# fix the bug, commit, done
# Your feature work is untouched in ~/project/
---
cmd: 1-2
structure: 4-6
work: 8-11
```

{{show: worktree-add typewriter 2s linear}}
{{focus: cmd}}
{{annotate: cmd "One command, two directories"}}
`git worktree add` creates a new directory with a different branch checked out.

{{zoom: 1.2x}}
{{focus: structure}}
{{annotate: structure "Same repo, two directories"}}
Both directories are part of the same git repository. No cloning. No duplication. One `.git/`.

{{zoom: 1x}}
{{focus: work}}
{{annotate: work "Two terminals, no interference"}}
Run the hotfix in one terminal. Your feature work sits untouched in the other directory.

{{clear: slide}}

```diagram:worktree-diagram
dot-git [storage]
worktree-main [directory]
worktree-hotfix [directory]
feature-branch [branch]
hotfix-branch [branch]
worktree-main --> dot-git
worktree-hotfix --> dot-git
dot-git --> feature-branch
dot-git --> hotfix-branch
{Group: worktree-main, worktree-hotfix}
---
storage: dot-git
main-dir: worktree-main
hotfix-dir: worktree-hotfix
feat: feature-branch
hot: hotfix-branch
```

{{show: worktree-diagram grow 0.5s spring}}
{{focus: storage}}
{{annotate: storage "One .git/ directory"}}
A single `.git/` directory tracks the state for both working trees. No duplication of object storage.

{{focus: main-dir}}
{{annotate: main-dir "Your feature work"}}
The main working tree has your feature branch checked out.

{{focus: hotfix-dir}}
{{annotate: hotfix-dir "The urgent fix"}}
The hotfix worktree is a second directory, completely separate on disk.

{{focus: feat}}
{{annotate: feat "feature/dashboard"}}
Each working tree has its own independent HEAD pointing to a different branch.

---

# Step 2: git worktree commands

{{clear: slide}}

```code:worktree-commands lang=bash
# List all worktrees
git worktree list
# /Users/ada/project          abc1234 [feature/dashboard]
# /Users/ada/project-hotfix   def5678 [hotfix-branch]

# Create a worktree (branch must already exist)
git worktree add ../project-hotfix hotfix-branch

# Create a worktree AND a new branch
git worktree add ../project-experiment -b experiment/new-algo

# Remove a worktree (branch is NOT deleted)
git worktree remove ../project-hotfix

# Clean up stale references (directory deleted without remove)
git worktree prune
---
list-cmd: 1-4
add-existing: 6-7
add-new: 9-10
remove: 12-13
prune: 15-16
```

{{show: worktree-commands typewriter 2s linear}}
{{focus: list-cmd}}
{{annotate: list-cmd "See all active worktrees"}}
`git worktree list` shows every worktree, its path, the current commit, and the checked-out branch.

{{focus: add-existing}}
{{annotate: add-existing "Branch must exist first"}}
The branch must already exist. Use `-b` to create one simultaneously.

{{focus: add-new}}
{{annotate: add-new "Create branch and worktree together"}}
`-b` creates both the branch and the directory in one step.

{{zoom: 1.2x}}
{{focus: remove}}
{{annotate: remove "Directory removed, branch kept"}}
Removing the worktree deletes the directory but does not delete the branch. Your commits are safe.

{{zoom: 1x}}
{{focus: prune}}
{{annotate: prune "Fix stale references"}}
If you deleted the directory manually without `git worktree remove`, prune cleans up the stale reference in `.git/`.

{{clear: slide}}

```diagram:worktree-constraint
branch-a [service label="feature/dashboard"]
worktree-a [service label="~/project — checked out here"]
branch-b [service label="feature/dashboard (same)"]
worktree-b [service label="~/project2 — try again here"]
error-node [client label="ERROR: already checked out"]
branch-a --> worktree-a: ✓ allowed
branch-b --> worktree-b: attempt
worktree-b --> error-node: BLOCKED
---
branch: branch-a
first: worktree-a
second: worktree-b
err: error-node
conflict-path: branch-b, worktree-b, error-node
```

{{show: worktree-constraint slide 0.4s ease-out}}
{{focus: branch}}
{{annotate: branch "One branch, one worktree"}}
A single branch cannot be checked out in two worktrees simultaneously. Git enforces this.

{{flow: conflict-path}}
{{focus: err}}
{{annotate: err "Git blocks this"}}
The error message is clear. Switch to a different branch, or create a new one.

---

# Step 3: git submodule — repos inside repos

{{clear: slide}}

```diagram:submodule-diagram
parent-repo [repository]
submodule-lib [repository]
dotgitmodules [file]
pinned-sha [commit]
parent-repo --> dotgitmodules
parent-repo --> pinned-sha
pinned-sha --> submodule-lib
{Group: parent-repo, dotgitmodules}
---
parent: parent-repo
sub: submodule-lib
modules-file: dotgitmodules
pin: pinned-sha
```

{{show: submodule-diagram grow 0.5s spring}}
{{focus: parent}}
{{annotate: parent "Your main project"}}
The parent repo knows about the submodule, but only stores a pointer — not the content.

{{focus: sub}}
{{annotate: sub "An independent git repo"}}
The submodule is a complete, independent git repository. It has its own commits, branches, history.

{{focus: pin}}
{{annotate: pin "Exact commit SHA pinned"}}
The parent repo stores the exact commit SHA of the submodule. Not a branch — a specific commit. This is intentional: reproducible builds.

{{focus: modules-file}}
{{annotate: modules-file "URL + path stored here"}}
`.gitmodules` records the submodule's remote URL and where it lives inside the parent.

{{clear: slide}}

```code:gitmodules lang=ini
# .gitmodules (auto-generated by git submodule add)
[submodule "libs/parser"]
    path = libs/parser
    url = https://github.com/company/parser-lib.git

[submodule "libs/renderer"]
    path = libs/renderer
    url = https://github.com/company/renderer-lib.git
    branch = stable
---
first-sub: 2-4
second-sub: 6-9
branch-pin: 9
```

{{show: gitmodules typewriter 2s linear}}
{{focus: first-sub}}
{{annotate: first-sub "Path and URL"}}
Two fields are required: where to put it (`path`) and where to fetch it from (`url`).

{{focus: second-sub}}
{{annotate: second-sub "Optional: track a branch"}}
The `branch` field lets the submodule track a remote branch with `--remote`.

{{focus: branch-pin}}
{{annotate: branch-pin "Still pinned to a SHA in parent"}}
Even with `branch = stable`, the parent commit still stores a specific SHA. The branch matters for `--remote` updates.

{{clear: slide}}

```code:submodule-add lang=bash
# Add a submodule
git submodule add https://github.com/company/parser-lib.git libs/parser

# After: two new things are staged
git status
# new file: .gitmodules
# new file: libs/parser

# Clone a project that has submodules
git clone --recurse-submodules https://github.com/company/project.git

# Or: initialize submodules in an already-cloned repo
git submodule update --init --recursive
---
add-cmd: 1-2
status: 4-7
clone-flag: 9-10
init-cmd: 12-13
```

{{show: submodule-add typewriter 2s linear}}
{{focus: add-cmd}}
{{annotate: add-cmd "One command to add"}}
`git submodule add` fetches the repo, places it at the path, and updates `.gitmodules`.

{{zoom: 1.2x}}
{{focus: status}}
{{annotate: status "Two staged files"}}
Two things get staged: the `.gitmodules` file and the submodule directory (which is actually a commit pointer).

{{zoom: 1x}}
{{focus: clone-flag}}
{{annotate: clone-flag "Must pass this on clone"}}
Without `--recurse-submodules`, cloning gives you empty submodule directories. This is the most common submodule gotcha.

{{focus: init-cmd}}
{{annotate: init-cmd "Fix an empty submodule"}}
If you already cloned without the flag, run this. `--recursive` handles nested submodules.

---

# Step 4: git submodule operations

{{clear: slide}}

```code:submodule-ops lang=bash
# Update submodule to latest on its tracked branch
git submodule update --remote libs/parser

# Pull all submodules
git submodule foreach git pull

# After updating, the parent's pointer changed
git status
# modified: libs/parser (new commits)
# Commit this to pin the new version
git add libs/parser
git commit -m "chore: update parser-lib to latest"
---
update-remote: 1-2
foreach: 4-5
status-after: 7-11
commit: 12-13
```

{{show: submodule-ops typewriter 2s linear}}
{{focus: update-remote}}
{{annotate: update-remote "Pull latest on tracked branch"}}
`--remote` fetches and checks out the latest commit from the submodule's tracked branch.

{{focus: foreach}}
{{annotate: foreach "All submodules at once"}}
`git submodule foreach` runs a command inside every submodule directory.

{{zoom: 1.2x}}
{{focus: status-after}}
{{annotate: status-after "Parent sees a new SHA"}}
When the submodule updates, the parent repo sees the pointer changed. You must commit this change to preserve the pin.

{{zoom: 1x}}
{{focus: commit}}
{{annotate: commit "Pin the new version"}}
Committing the updated pointer is how you tell other developers "use this new version of the submodule."

{{clear: slide}}

```data:submodule-honesty type=graph layout=tree
(submodule "git submodule") -> (painful "Complex to use")
(submodule "git submodule") -> (powerful "Pinned dependency versions")
(painful "Complex to use") -> (prefer "Prefer package managers when possible")
(prefer "Prefer package managers when possible") -> (npm "npm / yarn")
(prefer "Prefer package managers when possible") -> (cargo "cargo / crates.io")
(prefer "Prefer package managers when possible") -> (go "go modules")
(submodule "git submodule") -> (use-when "Use when: embedding git repos, not libraries")
---
sub: submodule
pain: painful
pref: prefer
use: use-when
```

{{show: submodule-honesty grow 0.5s spring}}
{{focus: sub}}
{{annotate: sub "Powerful but painful"}}
Submodules work. They're just operationally complex. Clone flags, update flags, foreach loops.

{{focus: prefer}}
{{annotate: prefer "Better options exist"}}
For libraries — things with version numbers and releases — use the package manager for your language. It's cleaner.

{{focus: use}}
{{annotate: use "When submodules make sense"}}
Submodules make sense when you need to embed another git repository directly — kernel modules, firmware, documentation repos.

---

# Step 5: git config — the three levels

{{clear: slide}}

```data:config-levels type=graph layout=tree
(system "/etc/gitconfig") -> (global "~/.gitconfig") -> (local ".git/config")
^priority=local
---
sys: system
glob: global
loc: local
priority: local
path: system, global, local
```

{{show: config-levels grow 0.5s spring}}
{{flow: path}}
{{focus: sys}}
{{annotate: sys "All users on this machine"}}
System config applies to every git user on the machine. Usually set by sysadmins. You rarely touch it.

{{focus: glob}}
{{annotate: glob "Your user, all repos"}}
Global config is the most common place to set things: your name, email, preferred editor, and defaults.

{{focus: loc}}
{{annotate: loc "This repo only"}}
Local config overrides everything. Set project-specific settings here — different email, different merge strategy.

{{clear: slide}}

```code:config-commands lang=bash
# See which file is providing a value
git config --show-origin user.email
# file:/Users/ada/.gitconfig   ada@personal.com

# Set at each level
git config --system core.editor vim       # all users
git config --global user.email ada@a.com  # your user
git config --local  user.email ada@b.com  # this repo only

# List all config with origins
git config --list --show-origin
---
show-origin: 1-3
set-levels: 5-8
list-all: 10-11
```

{{show: config-commands typewriter 2s linear}}
{{focus: show-origin}}
{{annotate: show-origin "Find who set this value"}}
`--show-origin` prints the file path next to the value. Essential for debugging "why is this setting wrong."

{{zoom: 1.2x}}
{{focus: set-levels}}
{{annotate: set-levels "Level flags"}}
`--system`, `--global`, `--local` explicitly target each level. Without a flag, default is local.

{{zoom: 1x}}
{{focus: list-all}}
{{annotate: list-all "Complete audit"}}
Useful to see every setting and where it came from — all three levels merged, with the winning values shown.

{{clear: slide}}

```code:important-settings lang=bash
# ~/.gitconfig — recommended settings

[user]
    name = Ada Lovelace
    email = ada@company.com

[core]
    editor = vim             # or code --wait, or nano

[pull]
    rebase = true            # rebase on pull, not merge

[merge]
    conflictstyle = diff3    # show base text in conflicts

[push]
    autoSetupRemote = true   # git 2.37+: auto-set upstream

[rerere]
    enabled = true           # remember and reuse conflict resolutions
---
user: 3-6
editor: 8-9
pull: 11-12
merge: 14-15
push: 17-18
rerere: 20-21
```

{{show: important-settings typewriter 2s linear}}
{{focus: pull}}
{{annotate: pull "Avoid accidental merge commits"}}
`pull.rebase = true` means `git pull` rebases your local commits on top of fetched changes instead of creating a merge commit.

{{zoom: 1.2x}}
{{focus: merge}}
{{annotate: merge "Three-way conflict view"}}
`diff3` shows the original base text alongside yours and theirs. Without it, resolving conflicts is guesswork.

{{zoom: 1x}}
{{focus: push}}
{{annotate: push "No more --set-upstream"}}
`push.autoSetupRemote` automatically sets the upstream on first push. You never need to type `--set-upstream` again.

{{focus: rerere}}
{{annotate: rerere "Reuse recorded resolutions"}}
`rerere` records how you resolved a conflict. If the same conflict appears again (during rebase, for example), git resolves it automatically.
