---
title: Tags and Cleanup
---

# What Tags Are For

Branches move. Tags don't. {{show: tag-vs-branch slide 0.4s ease-out}} That's the core difference. {{annotate: fixed-ptr "tag = fixed pointer"}} A branch pointer advances every time you commit. A tag points at one commit forever. It never moves. It is immutable by convention.

```data:tag-vs-branch type=graph layout=tree
(c1 "1.0 work") -> (c2 "fix auth") -> (c3 "add feature") -> (c4 "fix bug") -> (c5 "refactor")
^v1.0.0=c2
^v1.1.0=c4
^main=c5
^HEAD=c5
---
fixed-ptr: v1.0.0, v1.1.0
```

{{clear: slide}}

Tags mark releases. {{show: tag-purpose slide 0.4s ease-out}} `v1.0.0`, `v2.3.1`, `v3.0.0-rc1`. {{annotate: semver-ref "follows semver"}} The tag name is what your users, your CI system, and your deployment tooling refer to. When someone files a bug "reproducible in v1.2.3", you `git checkout v1.2.3` to see exactly that code. Without tags you'd be searching through commits by date.

Two kinds of tags exist. {{focus: semver-ref}} Lightweight tags are just a named pointer — like a branch that never moves. {{annotate: annotated-better "annotated tags for releases"}} Annotated tags are full git objects: they store the tagger's name, email, date, and a message. They can be signed with GPG. For releases, always use annotated. For private bookmarks, lightweight is fine. {{focus: none}}

```diagram:tag-purpose
release-v1 [v1.0.0]
release-v2 [v2.0.0]
release-rc [v2.1.0-rc1]
commit-a [release commit]
commit-b [next release commit]
commit-c [rc commit]
release-v1 --> commit-a: annotated tag
release-v2 --> commit-b: annotated tag
release-rc --> commit-c: lightweight tag
{Releases: release-v1, release-v2}
{PreRelease: release-rc}
---
annotated-better: release-v1, release-v2
semver-ref: release-v1, release-v2, release-rc
```

{{clear: slide}}

# Creating Tags

Lightweight vs annotated. {{show: tag-create typewriter 2s linear}} Two commands, two very different objects. {{annotate: annotated-flag "-a creates annotated tag"}} Know which you're creating and why.

```code:tag-create lang=bash
# Lightweight: just a pointer, no metadata
$ git tag v1.0.0

# Annotated: full object with message, author, date
$ git tag -a v1.0.0 -m "Release 1.0.0: initial public release"

# Tag a specific past commit (not HEAD)
$ git tag -a v0.9.0 -m "Retroactive tag" 9b8f4a2

# List all tags
$ git tag
v0.9.0
v1.0.0

# List tags matching a pattern
$ git tag -l "v1.*"
v1.0.0

# Show full annotated tag details
$ git show v1.0.0
tag v1.0.0
Tagger: Jane Smith <jane@example.com>
Date:   Mon Mar 01 10:00:00 2026 +0000

Release 1.0.0: initial public release

commit a3f2d9c1b7e4a88f0d5e3c2b9a6f4d1e8c7b3a5f
Author: Jane Smith <jane@example.com>
...
---
annotated-flag: 4-5
```

{{clear: slide}}

{{zoom: 1.2x}} {{focus: annotated-flag}} Annotated tags are stored as git objects in the object database. {{annotate: annotated-flag "tag object → commit object"}} A lightweight tag is just a ref (like a branch). An annotated tag is an object that points to a commit — two hops instead of one. `git show` on an annotated tag shows the tag metadata first, then the commit. {{zoom: 1x}} {{focus: none}}

```code:tag-show-diff lang=bash
# Show what type of object a tag is
$ git cat-file -t v1.0.0
tag               # annotated: a tag object

$ git cat-file -t v1.0.0-lite
commit            # lightweight: points directly to commit

# Annotated tag object contents:
$ git cat-file -p v1.0.0
object a3f2d9c1b7e4a88f0d5e3c2b9a6f4d1e8c7b3a5f
type commit
tag v1.0.0
tagger Jane Smith <jane@example.com> 1709300000 +0000

Release 1.0.0: initial public release
---
annotated-flag: 1-3
```

{{clear: slide}}

# Pushing Tags

Tags are not pushed automatically. {{show: push-tags-explainer slide 0.4s ease-out}} This surprises almost everyone the first time. {{annotate: not-auto "tags need explicit push"}} `git push origin main` sends commits. Tags sit in `.git/refs/tags/` and do not travel with `git push` unless you tell it to.

```code:push-tags-explainer lang=bash
# Push a specific tag
$ git push origin v1.0.0

# Push all local tags
$ git push origin --tags

# BEST PRACTICE: push commits + annotated tags together
$ git push origin --follow-tags
# --follow-tags pushes commits AND reachable annotated tags
# It skips lightweight tags (which you usually don't want pushed)

# Verify the tag is on the remote
$ git ls-remote --tags origin
a3f2d9c1b7e4a88f0d5e3c2b9a6f4d1e8c7b3a5f  refs/tags/v1.0.0
c8d7e6f5a4b3c2d1e0f9a8b7c6d5e4f3a2b1c0d9  refs/tags/v1.0.0^{}
---
not-auto: 1-2
```

{{clear: slide}}

`--follow-tags` is the right default. {{zoom: 1.2x}} {{annotate: follow-tags "--follow-tags = safe auto-push"}} It pushes annotated tags that are reachable from the commits you're pushing. Lightweight tags are excluded. This means your CI pipeline will see the tag as soon as the commits arrive. {{zoom: 1x}} Add it to your push workflow by default.

Delete tags when you need to. {{focus: not-auto}} Deleting a tag on the remote does not delete it locally and vice versa. {{annotate: delete-both "delete locally and remotely"}} Two separate commands. {{focus: none}}

```code:delete-tags lang=bash
# Delete a tag locally
$ git tag -d v1.0.0-bad

# Delete a tag on the remote
$ git push origin --delete v1.0.0-bad
# Or the older form:
$ git push origin :refs/tags/v1.0.0-bad
---
delete-both: 1-5
```

{{clear: slide}}

# Deleting Remote Branches

The PR is merged. {{show: cleanup-workflow slide 0.4s ease-out}} The feature branch has served its purpose. Now clean it up. {{annotate: post-merge-cleanup "standard post-merge cleanup"}} Keeping merged branches around is technical debt — they appear in `git branch -a`, in remote listings, and confuse future contributors.

```diagram:cleanup-workflow
feature-br [feature/login branch]
pr-merged [PR merged to main]
delete-remote [delete remote branch]
delete-local [delete local branch]
prune-tracking [prune tracking ref]
clean-state [clean repo]
feature-br --> pr-merged: merge + approve
pr-merged --> delete-remote: git push --delete
delete-remote --> prune-tracking: git fetch --prune
pr-merged --> delete-local: git branch -d
delete-local --> clean-state
prune-tracking --> clean-state
{Action: delete-remote, delete-local, prune-tracking}
---
post-merge-cleanup: delete-remote, delete-local, prune-tracking
```

{{clear: slide}}

```code:cleanup-workflow lang=bash
# 1. Merge the PR on GitHub/GitLab (or locally)
$ git switch main
$ git merge --no-ff feature/login

# 2. Delete the remote branch
$ git push origin --delete feature/login

# 3. Delete your local branch
$ git branch -d feature/login

# 4. Prune stale remote-tracking refs
$ git fetch --prune
# OR:
$ git remote prune origin

# Full post-PR cleanup in one sequence:
$ git switch main && git pull --rebase && \
  git branch -d feature/login && \
  git push origin --delete feature/login && \
  git fetch --prune
---
post-merge-cleanup: 1-16
```

{{clear: slide}}

GitHub auto-deletes remote branches after merge if you enable it. {{show: github-autodelete typewriter 1.5s linear}} In Settings > Repository > "Automatically delete head branches". {{annotate: github-setting "enable auto-delete in settings"}} This removes the remote branch cleanup step. You still need to prune locally with `git fetch --prune`. {{focus: github-setting}} This setting is worth enabling on every project. {{focus: none}}

# Semantic Versioning

Version numbers communicate intent. {{show: semver-diagram slide 0.4s ease-out}} `v2.3.1` tells you three things in three numbers. {{annotate: major-field "MAJOR = breaking changes"}} The structure is MAJOR.MINOR.PATCH.

```diagram:semver-diagram
major-node [MAJOR: breaking change]
minor-node [MINOR: new feature, compatible]
patch-node [PATCH: bug fix]
version-node [v2.3.1]
version-node --> major-node: 2 = major
version-node --> minor-node: 3 = minor
version-node --> patch-node: 1 = patch
{Breaking: major-node}
{Feature: minor-node}
{Fix: patch-node}
---
major-field: major-node
```

{{clear: slide}}

MAJOR goes up when you break the API. {{show: semver-examples typewriter 1.5s linear}} A function changes signature. A behavior changes incompatibly. Users must update their code to use the new version. {{annotate: major-rule "MAJOR = users must adapt"}} Go from `v1.4.2` to `v2.0.0`.

MINOR goes up for new capabilities that don't break existing users. {{focus: major-rule}} Add a new endpoint, a new CLI flag, a new optional parameter. {{annotate: minor-rule "MINOR = additive only"}} Existing code continues to work without changes. Go from `v1.4.2` to `v1.5.0`. Reset PATCH to zero.

PATCH goes up for bug fixes that don't change the API. {{focus: minor-rule}} Same behavior, just correctly. {{annotate: patch-rule "PATCH = fix without change"}} Go from `v1.4.2` to `v1.4.3`. {{focus: none}}

```code:semver-examples lang=bash
# Tagging pattern for a full release cycle
$ git tag -a v1.0.0-alpha.1 -m "Alpha 1"
$ git tag -a v1.0.0-beta.1 -m "Beta 1: feature complete"
$ git tag -a v1.0.0-rc.1 -m "Release candidate 1"
$ git tag -a v1.0.0 -m "Release 1.0.0"

# Patch release for a bug fix
$ git tag -a v1.0.1 -m "Fix login timeout off-by-one"

# Minor release for a new feature
$ git tag -a v1.1.0 -m "Add dashboard feature"

# Major release for breaking API change
$ git tag -a v2.0.0 -m "Rework auth API (breaking)"

# Push all new tags
$ git push origin --follow-tags
---
major-rule: 12-13
minor-rule: 9-10
patch-rule: 6-7
```

{{clear: slide}}

GitHub Releases are tags with a UI wrapper. {{show: github-releases typewriter 1.5s linear}} Create a release on GitHub and it creates an annotated tag and a release page with a changelog editor and file attachments. {{annotate: release-page "GitHub UI wraps git tags"}} The underlying mechanism is git tags. The release page, changelog, and attached binaries are GitHub features on top of that. Push your annotated tags and GitHub will offer to create a release for each one.
