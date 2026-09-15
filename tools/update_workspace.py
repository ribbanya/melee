#!/usr/bin/env python3
"""
Extract the most recently modified .tar.gz file based on a glob pattern.
"""

import argparse
import tarfile
from pathlib import Path

# ==================== CONFIGURATION ====================
# Set your glob pattern here (e.g., "*.tar.gz", "build/*.tar.gz", "data/**/*.tar.gz")


ROOT = Path.home() / "Downloads"
GLOB_PATTERN = "*.tar.gz"
# =======================================================


def extract_tar_gz(pattern: str, src: Path, dst: Path) -> None:
    """
    Find the most recently modified file matching the glob pattern, verify it's a .tar.gz,
    and extract its contents.

    Args:
        pattern: Glob pattern to match files
        dest_path: Destination path for extraction (defaults to current directory)
    """
    # Find all files matching the pattern
    matched_files = src.rglob(pattern)

    if not matched_files:
        print(f"No files found matching pattern: {pattern}")
        return

    # Get the most recently modified file
    tar_file_path = max(matched_files, key=lambda p: p.stat().st_mtime)

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

    # Extract the tar.gz file
    with tarfile.open(tar_file_path, "r:gz") as tar:
        tar.extractall(path=dst)

    print(f"Extracted {tar_file_path} to {dst}")


def main() -> None:
    """Main function to parse arguments and call extract_tar_gz."""
    parser = argparse.ArgumentParser(
        description="Find and extract the most recently modified .tar.gz file matching a glob pattern"
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
        help=f"Glob pattern to match files (default: {ROOT.as_posix()})",
    )
    parser.add_argument(
        "--dst",
        type=Path,
        default=Path.cwd(),
        help="Destination directory for extraction (default: current directory)",
    )

    args = parser.parse_args()

    extract_tar_gz(args.pattern, args.src, args.dst)


if __name__ == "__main__":
    main()
