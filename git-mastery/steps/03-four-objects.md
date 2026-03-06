---
title: The Four Object Types
---

# The Content-Addressed Store

Git doesn't store files. {{show: object-store slide 0.4s ease-out}} Git stores objects. Every piece of data that goes into git gets compressed, prefixed with a type header, and hashed with SHA-1. {{annotate: sha-arrow "hash = name"}} That hash becomes the object's permanent name and address.

The storage path comes from the hash itself. {{focus: storage-path}} Take the 40-character hex hash, use the first two characters as a directory name, the rest as the filename. {{annotate: storage-path ".git/objects/ab/cdef..."}} Open `.git/objects/` and you'll see this structure. Every file in there is a git object, named by its own content.

This means identical content is stored once. {{focus: none}} Two files with the same bytes produce the same hash and occupy the same slot in the store. {{annotate: dedup-note "automatic deduplication"}} Git doesn't store your 10-megabyte header file 500 times across 500 commits. It stores it once and points to it everywhere.

Four types of objects live in this store. Everything git knows how to do is built from these four.

```diagram:object-store
write-op [write content]
compress [zlib compress]
sha-hash [SHA-1 hash]
storage-path [.git/objects/ab/cdef...]
write-op --> compress: raw bytes
compress --> sha-hash: compute hash
sha-hash --> storage-path: first 2 chars = dir
---
sha-arrow: sha-hash
storage-path: storage-path
dedup-note: sha-hash
```

# Blobs

{{clear: slide}}

A blob is the simplest object. {{show: blob-demo typewriter 2s linear}} It's just bytes. Raw file content, nothing else. No filename, no permissions, no metadata. {{annotate: content-lines "pure content"}} Git stores the content of your files, not the files themselves.

This is a meaningful distinction. {{focus: content-lines}} Rename a file and its blob hash doesn't change — same content, same hash. Move it to a different directory: same hash. Change one character: completely different hash. {{annotate: type-line "type + content only"}} The blob object has no idea what it's called.

The `git cat-file` command lets you inspect any object. {{focus: hash-line}} Give it a hash and the type flag, it gives you back the raw content. No magic. No abstraction. {{annotate: hash-line "40 chars = object address"}} What you see is what git stored.

```code:blob-demo lang=bash
# Write a file and commit it
$ echo "console.log('hello')" > app.js
$ git add app.js
$ git status
Changes to be committed:
  new file: app.js

# Find the blob's hash
$ git ls-files --cached -s app.js
100644 e69b6336a5f5d52d67ab62e9f72f0e1f1a2b8b5e 0  app.js

# Inspect the blob directly
$ git cat-file -p e69b6336a5f5d52d67ab62e9f72f0e1f1a2b8b5e
console.log('hello')
---
type-line: 1-2
content-lines: 10-11
hash-line: 7-8
```

# Trees

{{clear: slide}}

A tree maps names to hashes. {{show: tree-demo typewriter 1.5s linear}} Where blobs are pure content, trees are directories — they record the filename, permissions, and hash for each entry. {{annotate: tree-header "directory = tree"}} Each entry is either a blob (a file) or another tree (a subdirectory).

Look at the output of `git cat-file -p` on a tree. {{focus: tree-entries}} Mode, type, hash, name. That's the entire record. {{annotate: blob-entries "blobs = files"}} The tree doesn't contain the file data — it contains a pointer to a blob that does.

Trees are recursive. {{clear: slide}} {{show: tree-structure slide 0.4s ease-out}} A tree can point to other trees. {{focus: subtree-node}} That subtree points to more blobs and trees. {{annotate: subtree-node "nested trees = subdirs"}} Unwind the recursion and you get the complete snapshot of your project at a moment in time — every file, every directory, every permission bit.

```code:tree-demo lang=bash
# Inspect the tree for the current commit
$ git cat-file -p HEAD^{tree}
100644 blob 8baef1b4abc478178b004d62031cf7fe6db6dcae  README.md
100644 blob e69b6336a5f5d52d67ab62e9f72f0e1f1a2b8b5e  app.js
040000 tree 4b825dc642cb6eb9a060e54bf8d69288fbee4904  src
100644 blob 7f6db9c256c4be0e83d4af0f47f7e66b6b5cda04  package.json

# Inspect a subtree
$ git cat-file -p 4b825dc642cb6eb9a060e54bf8d69288fbee4904
100644 blob d670460b4b4aece5915caf5c68d12f560a9fe3e4  index.ts
100644 blob 3b18e512dba79e4c8300dd08aeb37f8e728b8dad  utils.ts
---
tree-header: 1-2
tree-entries: 3-6
blob-entries: 3-5
```

```data:tree-structure type=graph layout=tree
root-tree -> readme-blob: README.md
root-tree -> app-blob: app.js
root-tree -> src-tree: src/
src-tree -> index-blob: index.ts
src-tree -> utils-blob: utils.ts
---
subtree-node: src-tree
```

# Commits

{{clear: slide}}

A commit is a snapshot. {{show: commit-demo typewriter 1.5s linear}} Not a diff. Not a patch. A pointer to a complete tree of the entire project at one point in time. {{annotate: tree-line "points to full snapshot"}} This is the most important thing to understand about git's data model.

Every commit also records context. {{focus: metadata-section}} Author with email and timestamp, committer (may differ from author for patches), and the commit message. {{annotate: author-line "who and when"}} If you've ever wondered why git has both "author" and "committer" — patches from email can be authored by one person and committed by another.

The parent field is what makes commits a chain. {{focus: parent-line}} One parent: a normal commit. Two parents: a merge commit. Zero parents: the very first commit in the repository. {{annotate: parent-line "link to history"}} Follow the parent pointers and you traverse the entire history of the project.

```code:commit-demo lang=bash
# Inspect a commit object directly
$ git cat-file -p HEAD
tree 4b825dc642cb6eb9a060e54bf8d69288fbee4904
parent 7fd1a60b01f91b314f59955a4e4d4e80d8edf11d
author Jane Smith <jane@example.com> 1714521600 -0700
committer Jane Smith <jane@example.com> 1714521600 -0700

Add input validation to the login form

Prevents empty username submissions and trims whitespace
before comparison. Fixes a regression from the last sprint.

Fixes: #482
---
tree-line: 2
parent-line: 3
author-line: 4-5
metadata-section: 4-5
```

# Tags

{{clear: slide}}

Tags are named pointers. {{show: tag-demo typewriter 1.5s linear}} Two kinds, and they're meaningfully different. {{annotate: lightweight-section "41-byte alias"}} A lightweight tag is just a name stored as a file containing a commit hash. It's an alias. Nothing more.

An annotated tag is its own object. {{focus: annotated-section}} It has a type, a tagger, a timestamp, and a message. {{annotate: annotated-section "has its own hash"}} It can be GPG-signed. It participates in git's object store as a first-class citizen. Releases should always be annotated tags.

The practical rule: for releases, use annotated. {{focus: none}} For a local scratch marker, use lightweight. {{annotate: sign-line "cryptographic proof"}} If you care about who tagged it, when they tagged it, or why — you need an annotated tag. A lightweight tag tells you nothing except that a commit has a name.

```code:tag-demo lang=bash
# Lightweight tag — just a pointer
$ git tag v1.0.0
$ cat .git/refs/tags/v1.0.0
7fd1a60b01f91b314f59955a4e4d4e80d8edf11d

# Annotated tag — its own object with metadata
$ git tag -a v1.0.0 -m "Release 1.0.0: stable production build"
$ git cat-file -p v1.0.0
object 7fd1a60b01f91b314f59955a4e4d4e80d8edf11d
type commit
tag v1.0.0
tagger Jane Smith <jane@example.com> 1714521600 -0700

Release 1.0.0: stable production build
---
lightweight-section: 2-4
annotated-section: 7-13
sign-line: 7
```
