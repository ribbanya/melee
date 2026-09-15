#!/usr/bin/env python3
"""
Extract the first (most recently modified) .tar.gz file matching a glob pattern,
then move only files that are tracked by git to the destination.
"""

import argparse
import glob
import os
import shutil
import subprocess
from pathlib import Path
from typing import Optional
import tarfile
import tempfile


def get_git_tracked_files(repo_root: Path) -> set[str]:
    """Get the set of files tracked by git in the repository."""
    result = subprocess.run(
        ["git", "ls-files"],
        cwd=repo_root,
        capture_output=True,
        text=True,
        check=True,
    )
    return set(result.stdout.strip().split("\n"))


def extract_tar_gz_selective(
    pattern: str = "*.tar.gz",
    dest_path: Optional[Path] = None,
    repo_root: Optional[Path] = None,
) -> None:
    """
    Find the most recently modified file matching the glob pattern, verify it's a .tar.gz,
    extract to a temp directory, and move only git-tracked files to destination.

    Args:
        pattern: Glob pattern to match files
        dest_path: Destination path for extraction (defaults to current directory)
        repo_root: Root of the git repository (defaults to current directory)
    """
    # Find all files matching the pattern
    matched_files = glob.glob(pattern)

    if not matched_files:
        print(f"No files found matching pattern: {pattern}")
        return

    # Get the most recently modified file
    tar_file_path = max(matched_files, key=lambda f: os.path.getmtime(f))
    tar_file_path = Path(tar_file_path)

    # Verify it has .tar.gz extension
    if not tar_file_path.suffixes[-2:] == [".tar", ".gz"]:
        print(f"File {tar_file_path} does not have .tar.gz extension")
        return

    # Verify it's actually a tar file
    if not tarfile.is_tarfile(tar_file_path):
        print(f"File {tar_file_path} is not a valid tar archive")
        return

    # Set paths
    dest = dest_path or Path.cwd()
    repo = repo_root or Path.cwd()

    # Create destination directory if it doesn't exist
    dest.mkdir(parents=True, exist_ok=True)

    # Get git-tracked files
    try:
        git_tracked = get_git_tracked_files(repo)
    except subprocess.CalledProcessError as e:
        print(f"Error getting git-tracked files: {e}")
        return

    # Create a temporary directory for extraction
    with tempfile.TemporaryDirectory() as tmp_dir:
        tmp_path = Path(tmp_dir)

        # Extract the tar.gz file to temp directory
        with tarfile.open(tar_file_path, "r:gz") as tar:
            tar.extractall(path=tmp_path)
            extracted_names = tar.getnames()

        print(f"Extracted {len(extracted_names)} files/directories to temp directory")

        # Move only git-tracked files
        moved_count = 0
        for rel_path_str in extracted_names:
            rel_path = Path(rel_path_str)

            # Check if this file (or any file within this directory) is git-tracked
            # For files: check exact path
            # For directories: check if any tracked file starts with this directory
            should_move = False

            if rel_path in git_tracked:
                should_move = True
            else:
                # Check if it's a directory containing tracked files
                for tracked in git_tracked:
                    tracked_path = Path(tracked)
                    if rel_path in tracked_path.parents:
                        should_move = True
                        break

            if should_move:
                src = tmp_path / rel_path
                dst = dest / rel_path

                if src.exists():
                    # Create parent directories if needed
                    dst.parent.mkdir(parents=True, exist_ok=True)

                    if src.is_dir():
                        if dst.exists():
                            shutil.rmtree(dst)
                        shutil.copytree(src, dst)
                    else:
                        shutil.copy2(src, dst)

                    moved_count += 1
                    print(f"  Moved: {rel_path}")

        print(f"\nMoved {moved_count} git-tracked files/directories to {dest}")


def main() -> None:
    """Main function to parse arguments and call extract_tar_gz_selective."""
    parser = argparse.ArgumentParser(
        description="Find and extract the most recently modified .tar.gz file, "
        "moving only git-tracked files to destination"
    )
    parser.add_argument(
        "--pattern",
        type=str,
        default="*.tar.gz",
        help="Glob pattern to match files (default: *.tar.gz)",
    )
    parser.add_argument(
        "--dest",
        type=Path,
        default=None,
        help="Destination directory for extraction (default: current directory)",
    )
    parser.add_argument(
        "--repo-root",
        type=Path,
        default=None,
        help="Root of the git repository for tracking (default: current directory)",
    )

    args = parser.parse_args()

    extract_tar_gz_selective(args.pattern, args.dest, args.repo_root)


if __name__ == "__main__":
    main()
