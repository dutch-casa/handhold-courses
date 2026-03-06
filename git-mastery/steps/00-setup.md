---
title: Installing and configuring git
---

# Do you have git?

{{show: check-command typewriter 1s linear}} Before anything else — check if git is already installed.

{{focus: command-line}} Run this in your terminal. {{annotate: command-line "type this, press enter"}} One command. If it works, skip ahead.

{{zoom: 1.3x}} {{focus: version-output}} If you see a version number, you're done with this step. {{annotate: version-output "you're good"}} If you see "command not found," keep reading.

{{zoom: 1x}} {{focus: none}}

```code:check-command lang=bash
git --version
# git version 2.43.0
---
command-line: 1
version-output: 2
```

# Installing on macOS

{{clear: slide}}

{{show: mac-options slide 0.4s ease-out}} macOS ships with an old git through Xcode Command Line Tools. Use Homebrew instead — you'll always have the latest version.

{{focus: homebrew-path}} {{annotate: homebrew-path "recommended"}} Homebrew is the standard package manager for macOS. If you don't have it, the one-liner on brew.sh installs it.

{{zoom: 1.2x}} {{focus: brew-install}} Run this. It downloads and installs git, usually in under a minute. {{annotate: brew-install "run this"}}

{{zoom: 1x}} {{focus: xcode-path}} The Xcode path works too. {{annotate: xcode-path "slower install"}} Running `git` for the first time triggers a system dialog to install Command Line Tools. Click install, wait, done.

{{focus: none}}

```code:mac-options lang=bash
# Option A: Homebrew (recommended)
brew install git

# Option B: Xcode Command Line Tools (built-in trigger)
git --version
# → "Install Command Line Tools?" dialog appears
# Click install. Takes 5-10 minutes.

# Verify either way
git --version
---
homebrew-path: 2
brew-install: 2
xcode-path: 5-6
```

# Installing on Linux

{{clear: slide}}

{{show: linux-options slide 0.4s ease-out}} Linux package managers make this fast. One command, done.

{{focus: ubuntu-line}} {{annotate: ubuntu-line "Debian/Ubuntu"}} `apt` is what Ubuntu and Debian-based distros use.

{{zoom: 1.2x}} {{focus: fedora-line}} {{annotate: fedora-line "Fedora/RHEL"}} Fedora, CentOS, and RHEL use `dnf`.

{{zoom: 1x}} {{focus: arch-line}} {{annotate: arch-line "Arch"}} Arch users know what they're doing.

{{zoom: 1x}} {{focus: none}} {{annotate: verify-line "always verify after"}} Every path ends the same way — `git --version` to confirm it worked.

```code:linux-options lang=bash
# Ubuntu / Debian / Mint
sudo apt update && sudo apt install git

# Fedora / RHEL / CentOS
sudo dnf install git

# Arch Linux
sudo pacman -S git

# Verify
git --version
---
ubuntu-line: 2
fedora-line: 5
arch-line: 8
verify-line: 11
```

# Installing on Windows

{{clear: slide}}

{{show: windows-options slide 0.4s ease-out}} Two options. Use WSL if you plan to do serious development on Windows. Use Git for Windows if you just want git working immediately.

{{focus: wsl-path}} {{annotate: wsl-path "serious devs: use this"}} WSL — Windows Subsystem for Linux — gives you a real Linux environment inside Windows. Once you're in WSL, follow the Linux instructions. Everything works the same.

{{zoom: 1.2x}} {{focus: installer-path}} Git for Windows is the standard installer from git-scm.com. {{annotate: installer-path "quickest path"}} It includes Git Bash — a minimal Unix-style shell — plus GUI tools. Accept all defaults during install.

{{zoom: 1x}} {{focus: gui-note}} There are also GUI clients — GitHub Desktop, Sourcetree. {{annotate: gui-note "skip these for now"}} Ignore them for now. Learn the command line first. GUIs become useful once you understand what they're hiding.

{{focus: none}}

```code:windows-options lang=bash
# Option A: WSL (recommended for developers)
# 1. Open PowerShell as Administrator
wsl --install
# 2. Restart, open Ubuntu from Start menu
# 3. Follow Linux instructions above

# Option B: Git for Windows
# Download from: https://git-scm.com/download/win
# Run installer, accept defaults
# Opens "Git Bash" — use this terminal for git commands

# Verify (in Git Bash or WSL terminal)
git --version
---
wsl-path: 2-5
installer-path: 7-9
gui-note: 11-12
```

# Configuring your identity

{{clear: slide}}

{{show: config-demo typewriter 1.5s linear}} Git tags every commit with your name and email. Before your first commit, you have to tell it who you are.

{{focus: name-line}} {{zoom: 1.2x}} {{annotate: name-line "your real name"}} This is what shows up in `git log`. Use the name you want collaborators to see.

{{focus: email-line}} {{annotate: email-line "matches GitHub account"}} The email should match whatever account you use on GitHub or GitLab. This is how platforms link commits to your profile.

{{zoom: 1x}} {{focus: global-flag}} `--global` means this applies to every repo on your machine. {{annotate: global-flag "set once, works everywhere"}} You can override it per-repo by running the same command without `--global` inside that repo's directory.

{{focus: editor-line}} {{annotate: editor-line "pick one"}} The editor setting matters when git opens a file for you — merge commits, interactive rebase. Set it now so you don't get stuck in vim unexpectedly.

{{focus: none}} {{zoom: 1x}}

```code:config-demo lang=bash
# Set your identity (required before first commit)
git config --global user.name "Ada Lovelace"
git config --global user.email "ada@example.com"

# Set your preferred editor
git config --global core.editor "code --wait"   # VS Code
# git config --global core.editor "nano"        # nano (safe default)
# git config --global core.editor "vim"         # vim (if you know it)

# Verify
git config --list | grep user
---
name-line: 2
email-line: 3
global-flag: 2-3
editor-line: 6-8
```

# The help system

{{clear: slide}}

{{show: help-demo slide 0.4s ease-out}} Git ships with full documentation for every command. You don't need to Google most things.

{{focus: man-form}} {{zoom: 1.2x}} {{annotate: man-form "opens a man page"}} `git help <command>` opens the full manual in your browser or terminal pager. Comprehensive, but dense.

{{zoom: 1x}} {{focus: short-form}} {{annotate: short-form "use this daily"}} The `-h` flag gives you a quick reference — flags and usage in ~20 lines. This is the one you'll use most.

{{focus: config-list}} {{annotate: config-list "inspect your config"}} `git config --list` shows all your current configuration — global, local, and system-level, merged together.

{{focus: none}} Two things from here. The next lesson is why version control exists at all — the history behind git, including why Linus built it in 10 days in 2005. {{zoom: 1x}} Then your first lab puts everything into practice.

```code:help-demo lang=bash
# Full manual for a command
git help commit
git help log

# Quick usage summary (use this constantly)
git commit -h
git log -h

# See all your config
git config --list

# See where a config value is set
git config --show-origin user.name
---
man-form: 2-3
short-form: 6-7
config-list: 10
```
