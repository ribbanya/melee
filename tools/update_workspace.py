#!/usr/bin/env python3
"""
Extract the most recently modified .tar.gz file based on a glob pattern,
then move only files that are tracked by git to the destination.
"""

import argparse
import glob
import os
import shutil
import subprocess
import tarfile
import tempfile
from pathlib import Path
from typing import Optional

# ==================== CONFIGURATION ====================
# Set your glob pattern here (e.g., "*.tar.gz", "build/*.tar.gz", "data/**/*.tar.gz")

ROOT = Path.home() / "Downloads"
GLOB_PATTERN = "*.tar.gz"
# =======================================================


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


def extract_tar_gz_selective(pattern: str, src: Path, dst: Path, repo_root: Optional[Path] = None) -> None:
    """
    Find the most recently modified file matching the glob pattern, verify it's a .tar.gz,
    extract to a temp directory, and move only git-tracked files to destination.

    Args:
        pattern: Glob pattern to match files
        src: Source directory to search for tar files
        dst: Destination path for extracted files
        repo_root: Root of the git repository (defaults to dst)
    """
    # Change to source directory to apply glob pattern
    original_cwd = Path.cwd()
    os.chdir(src)
    
    try:
        # Find all files matching the pattern
        matched_files = glob.glob(pattern, recursive=True)

        if not matched_files:
            print(f"No files found matching pattern: {pattern} in {src}")
            return

        # Get the most recently modified file (as absolute path)
        tar_file_path = max(matched_files, key=lambda f: os.path.getmtime(f))
        tar_file_path = Path(tar_file_path).resolve()

        # Verify it has .tar.gz extension
        if not tar_file_path.suffixes[-2:] == [".tar", ".gz"]:
            print(f"File {tar_file_path} does not have .tar.gz extension")
            return

        # Verify it's actually a tar file
        if not tarfile.is_tarfile(tar_file_path):
            print(f"File {tar_file_path} is not a valid tar archive")
            return

        # Create destination directory if it doesn't exist
        dst.mkdir(parents=True, exist_ok=True)

        # Set repo root
        repo = repo_root or dst

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

                # Skip .git directory/file explicitly
                if rel_path.name == ".git" or (rel_path.parts and rel_path.parts[0] == ".git"):
                    print(f"  Skipped: {rel_path} (git internal)")
                    continue

                # Check if this file (or any file within this directory) is git-tracked
                should_move = False

                if rel_path_str in git_tracked:
                    should_move = True
                else:
                    # Check if it's a directory containing tracked files
                    for tracked in git_tracked:
                        tracked_path = Path(tracked)
                        if rel_path in tracked_path.parents:
                            should_move = True
                            break

                if should_move:
                    src_file = tmp_path / rel_path
                    dst_file = dst / rel_path

                    if src_file.exists():
                        # Create parent directories if needed
                        dst_file.parent.mkdir(parents=True, exist_ok=True)

                        if src_file.is_dir():
                            if dst_file.exists():
                                shutil.rmtree(dst_file)
                            shutil.copytree(src_file, dst_file)
                        else:
                            shutil.copy2(src_file, dst_file)

                        moved_count += 1
                        print(f"  Moved: {rel_path}")

            print(f"\nMoved {moved_count} git-tracked files/directories to {dst}")
    finally:
        os.chdir(original_cwd)


def main() -> None:
    """Main function to parse arguments and call extract_tar_gz_selective."""
    parser = argparse.ArgumentParser(
        description="Find and extract the most recently modified .tar.gz file, "
        "moving only git-tracked files to destination"
    )
    parser.add_argument(
        "--pattern",
        type=str,
        default=GLOB_PATTERN,
        help=f"Glob pattern to match files (default: {GLOB_PATTERN})",
    )
    parser.add_argument(
        "--src",
        type=Path,
        default=ROOT,
        help=f"Source directory to search (default: {ROOT.as_posix()})",
    )
    parser.add_argument(
        "--dst",
        type=Path,
        default=Path.cwd(),
        help="Destination directory for extraction (default: current directory)",
    )
    parser.add_argument(
        "--repo-root",
        type=Path,
        default=None,
        help="Root of the git repository for tracking (default: destination directory)",
    )

    args = parser.parse_args()

    extract_tar_gz_selective(args.pattern, args.src, args.dst, args.repo_root)


if __name__ == "__main__":
    main()
