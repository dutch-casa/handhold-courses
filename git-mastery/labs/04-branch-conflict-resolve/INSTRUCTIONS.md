# Lab 4: Branch and Conflict Resolution

Merge two diverged branches that have conflicting edits to the same file.

## Tasks

- [ ] Merge the `feature` branch into `main` (this will produce a conflict)
- [ ] Resolve the conflict in `README.md` — keep BOTH paragraphs, with the install instructions appearing after the project description
- [ ] Stage the resolved file and complete the merge commit

## Hints

- `git merge feature` attempts the merge. When there is a conflict, git stops and marks the file.
- Conflict markers look like this:
  ```
  <<<<<<< HEAD
  content from main
  =======
  content from feature
  >>>>>>> feature
  ```
- Delete the conflict markers and write the final content you want to keep.
- The goal: README.md should have both the "amazing things" paragraph AND the "npm install" paragraph.
- After editing: `git add README.md` stages the resolution, then `git commit` finalises the merge.
- `git status` shows files with unresolved conflicts (marked `UU`).

Use **Run Tests** in the testing panel to check your progress.
