#!/bin/sh

# Script to install Git hooks
echo "Installing Git hooks..."

# Copy hooks to .git/hooks directory
cp git-hooks/post-checkout .git/hooks/
cp git-hooks/post-merge .git/hooks/

# Make hooks executable
chmod +x .git/hooks/post-checkout
chmod +x .git/hooks/post-merge

echo "Git hooks installed successfully!"
echo "The sbagen.c file will be automatically set to read-only after checkout and merge operations."

exit 0 