---
title: The Stash
---

# The Problem — Mid-Task Context Switch

You're three hours into a feature. {{show: context-switch slide 0.4s ease-out}} Five files modified. Half the logic written. You haven't committed because it doesn't work yet. {{annotate: mid-feature "work in progress"}} Then your phone rings. Production is down. You need to hotfix main right now.

You can't commit broken work to main. {{focus: urgent-bug}} You can't leave your working directory dirty when you switch branches — or git will refuse, or worse, carry your changes into the hotfix branch. {{annotate: urgent-bug "need clean state now"}} You need your work to disappear temporarily and come back exactly as it was when you're done.

This is exactly what stash is for. {{focus: none}} One command to park your work. Fix the bug. One command to restore everything. {{annotate: stash-node "temporary storage"}} Not a commit. Not a branch. A stack of saved states, separate from your history.

```diagram:context-switch
mid-feature [feature work in progress]
stash-node [stash]
urgent-bug [urgent hotfix needed]
clean-state [clean working directory]
restored [feature work restored]
mid-feature --> stash-node: git stash
stash-node --> clean-state: working dir clean
clean-state --> urgent-bug: fix bug, commit
urgent-bug --> stash-node: git stash pop
stash-node --> restored: feature work back
---
mid-feature: mid-feature
stash-node: stash-node
urgent-bug: urgent-bug
```

# git stash and git stash pop

`git stash` saves your current state. {{clear: slide}} {{show: stash-stack slide 0.4s ease-out}} {{clear: slide}} {{show: stash-basic typewriter 2s linear}} It takes every tracked, modified file and every staged file, stores them in a new stash entry, and restores your working directory and index to match HEAD. {{annotate: clean-after "working dir = HEAD"}} Your dirty state is off the table. Clean working directory.

The stash is a stack. {{focus: stash-stack}} Last in, first out. Push multiple stashes and they stack on top of each other. {{annotate: stash-stack "LIFO stack"}} `git stash pop` takes the most recent stash and applies it back to your working directory, then removes it from the stack. One push, one pop.

`pop` can conflict. {{focus: none}} If the hotfix changed the same lines your stash modified, you'll get a merge conflict just like a merge. {{annotate: conflict-note "resolve like a merge"}} Resolve it normally, `git add` the resolved files. The stash is removed from the stack even when there's a conflict — use `git stash apply` instead if you want to keep the stash entry until you're confident the apply succeeded.

```data:stash-stack type=stack
[stash@{2}: oldest, stash@{1}: older, stash@{0}: latest]
^top=2
---
stash-stack: 0, 1, 2
top-entry: 2
```

```code:stash-basic lang=bash
# Mid-feature: dirty working directory
$ git status
On branch feature/login
Changes not staged for commit:
        modified:   src/auth.ts
        modified:   src/session.ts

# Stash everything
$ git stash
Saved working directory and index state WIP on feature/login: 9b2c184 Initial commit

# Working directory is now clean
$ git status
On branch feature/login
nothing to commit, working tree clean

# ... fix bug, commit to main, come back ...

# Restore stashed work
$ git stash pop
---
clean-after: 8-10
conflict-note: 19-20
```

# Naming Stashes — git stash push -m

{{clear: slide}}

Anonymous stashes are useless after two days. {{show: stash-named typewriter 1.5s linear}} `stash@{0}` tells you nothing. You're staring at a list of stashes trying to remember which one had the login feature work and which one was the database refactor. {{annotate: name-arg "-m gives it a name"}} Name them.

`git stash push -m "name"` is the modern syntax. {{focus: push-command}} The `-m` flag attaches a message to the stash entry. {{annotate: push-command "push -m is explicit"}} That message shows up in `git stash list` so you can identify it at a glance. This costs nothing and saves real time.

You can also stash only specific files. {{focus: path-arg}} `git stash push -m "login validation" src/auth.ts` stashes only `auth.ts`, leaving other changes in place. {{annotate: path-arg "path restricts scope"}} More surgical than stashing everything when you only need to park one file.

```code:stash-named lang=bash
# Named stash — readable in the list
$ git stash push -m "login input validation WIP"
Saved working directory and index state On feature/login: login input validation WIP

# Named stash of specific files
$ git stash push -m "auth refactor partial" src/auth.ts src/session.ts
Saved working directory and index state On feature/login: auth refactor partial
---
push-command: 1-3
name-arg: 1
path-arg: 5-7
```

# git stash list and git stash apply

{{clear: slide}}

Multiple stashes pile up. {{show: stash-list typewriter 1.5s linear}} `git stash list` shows all of them, numbered from newest to oldest. {{annotate: stash-indices "stash@{N} = index"}} `stash@{0}` is always the most recent. The numbers shift when you push or pop.

`git stash apply stash@{N}` applies a specific stash without removing it from the stack. {{focus: apply-line}} Useful when you want to apply the same stash to multiple branches. {{annotate: apply-line "apply vs pop"}} `pop` applies and removes. `apply` applies and keeps. If the apply fails or you're not sure, use `apply` — you can always `git stash drop stash@{N}` to clean up later.

`git stash drop` removes a stash entry without applying it. {{focus: drop-line}} When you decide the stash is no longer useful, drop it. {{annotate: drop-line "remove without applying"}} `git stash clear` drops all stash entries at once. Nuclear option. Use it when you're certain.

```code:stash-list lang=bash
# List all stashes
$ git stash list
stash@{0}: On feature/login: login input validation WIP
stash@{1}: On feature/login: auth refactor partial
stash@{2}: WIP on main: 7fd1a60 Fix null check

# Apply a specific stash (keeps it in the stack)
$ git stash apply stash@{1}

# Drop a stash you no longer need
$ git stash drop stash@{2}
Dropped stash@{2} (7fd1a60b01f91b314f59955a4e4d4e80d8edf11d)
---
stash-indices: 2-5
apply-line: 7-8
drop-line: 10-12
```

# What Stash Doesn't Save

{{clear: slide}}

Stash has a blind spot: untracked files. {{show: stash-untracked typewriter 1.5s linear}} `git stash` only saves tracked files — files that git knows about. {{annotate: untracked-warning "new files skipped"}} If you created a new file during your feature work and haven't run `git add` on it, `git stash` leaves it sitting in your working directory.

This matters. {{focus: untracked-warning}} Switch to another branch and that untracked file follows you. {{annotate: include-flag "--include-untracked"}} Now it's in your hotfix branch, potentially getting committed where it doesn't belong. Use `--include-untracked` to also stash untracked files.

`-u` is the short form. {{focus: include-flag}} `git stash push -u -m "my feature"` stashes everything: modified tracked files, staged files, and untracked files. {{annotate: gitignore-note "ignores .gitignore'd files"}} Deliberately ignored files (`.gitignore` matches) are still excluded — use `--all` if you need those too, but you almost never do.

```code:stash-untracked lang=bash
# New file created, not yet tracked
$ git status
On branch feature/login
Changes not staged for commit:
        modified:   src/auth.ts
Untracked files:
        src/new-feature.ts

# Regular stash — new file stays behind
$ git stash
$ ls src/new-feature.ts   # still there

# Include untracked files
$ git stash push -u -m "feature with new files"
---
untracked-warning: 5-8
include-flag: 13-14
gitignore-note: 13-14
```
