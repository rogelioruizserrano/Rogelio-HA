#!/bin/bash
cd /config || exit 1
export GIT_SSH_COMMAND="ssh -i /config/.ssh/id_ed25519 -o UserKnownHostsFile=/config/.ssh/known_hosts -o StrictHostKeyChecking=accept-new"

git add -A
if git diff --cached --quiet; then
  echo "Sin cambios"
  exit 0
fi

git commit -q -m "Auto: $(date '+%Y-%m-%d %H:%M') — $(git diff --cached --name-only | wc -l) ficheros"
git push -q origin main 2>&1
