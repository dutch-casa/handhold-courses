# Lab 2: The Object Store

Explore git's internal object store by inspecting blobs, trees, and commits with `git cat-file`.

## Tasks

- [ ] Use `git log --oneline` to see the commit history
- [ ] Use `git cat-file -t <hash>` to identify a blob, a tree, and a commit object
- [ ] Write the three type names — one per line — to `answers.txt` (the strings: `blob`, `tree`, `commit`)
- [ ] Use `git log --oneline` to find the oldest (first) commit hash and write it to `first-commit.txt`

## Hints

- Every commit has a hash. Every commit points to a tree. Every tree points to blobs.
- `git log --oneline` lists commits newest-first. The last line is the oldest commit.
- `git cat-file -t <hash>` prints the type: `blob`, `tree`, or `commit`.
- `git cat-file -p <hash>` prints the content — useful for walking from commit → tree → blob.
- A commit object's content shows the tree hash and parent hash(es).
- `echo "blob" >> answers.txt` appends a line. Make sure the file has exactly three lines.

Use **Run Tests** in the testing panel to check your progress.
