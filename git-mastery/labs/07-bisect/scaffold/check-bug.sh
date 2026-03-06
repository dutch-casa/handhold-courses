#!/bin/bash
node -e "$(cat app.js); process.exit(greet() === 'hello' ? 0 : 1)"
