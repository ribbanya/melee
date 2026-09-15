#!/usr/bin/env python3
"""
Extract the first matching .tar.gz file based on a glob pattern,
filter by git-tracked files, and move them to destination.
"""

import argparse
from pathlib import Path
import tarfile
import tempfile
import shutil
import glob
import subprocess
from typing import Optional


def extract_and_move_git_tracked(
    pattern: str = "*.tar.gz",
    dest_path: Optional[Path] = None,
    repo_root: Optional[Path] = None
) -> None:
    """
    Find the most recently modified file matching the glob pattern,
    verify it's a .tar.gz, extract to temp, filter by git ls-files,
    and move only tracked files to destination.
    
    Args:
        pattern: Glob pattern to match files
        dest_path: Destination path for extracted files (defaults to current directory)
        repo_root: Root of the git repository (defaults to current directory)
    """
    # Find all files matching the pattern
    matched_files = glob.glob(pattern)
    
    if not matched_files:
        print(f"No files found matching pattern: {pattern}")
        return
        
    # Get the most recently modified file
    tar_file_path = max(matched_files, key=lambda f: Path(f).stat().st_mtime)
    tar_file_path = Path(tar_file_path)
    
    # Verify it has .tar.gz extension
    if not tar_file_path.suffixes[-2:] == ['.tar', '.gz']:
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
    
    # Extract to temporary directory
    with tempfile.TemporaryDirectory() as tmp_dir:
        tmp_path = Path(tmp_dir)
        
        print(f"Extracting {tar_file_path} to temporary directory...")
        with tarfile.open(tar_file_path, 'r:gz') as tar:
            tar.extractall(path=tmp_path)
            
        # Get list of git-tracked files in the repository
        try:
            result = subprocess.run(
                ['git', '-C', str(repo), 'ls-files'],
                capture_output=True,
                text=True,
                check=True
            )
            git_tracked = set(result.stdout.strip().split('\n'))
        except subprocess.CalledProcessError as e:
            print(f"Error running git ls-files: {e}")
            return
            
        # Find all extracted files
        extracted_files = []
        for root, dirs, files in os.walk(tmp_path):
            for file in files:
                file_path = Path(root) / file
                rel_path = file_path.relative_to(tmp_path)
                
                # Skip .git directory entirely - check every component of the path
                if '.git' in rel_path.parts:
                    print(f"Skipping .git-related path: {rel_path}")
                    continue
                    
                extracted_files.append((file_path, rel_path))
        
        # Move only git-tracked files
        moved_count = 0
        for src_file, rel_path in extracted_files:
            rel_str = str(rel_path)
            
            if rel_str not in git_tracked:
                print(f"Skipping non-tracked file: {rel_str}")
                continue
                
            # Double-check: skip if any part of the path is .git
            if '.git' in rel_path.parts:
                print(f"Skipping .git-related path (double-check): {rel_path}")
                continue
            
            dst_file = dest / rel_path
            dst_file.parent.mkdir(parents=True, exist_ok=True)
            
            print(f"Moving {rel_str} to {dst_file}")
            shutil.move(str(src_file), str(dst_file))
            moved_count += 1
            
        print(f"Moved {moved_count} git-tracked files to {dest}")


def main() -> None:
    """Main function to parse arguments and call extract_and_move_git_tracked."""
    parser = argparse.ArgumentParser(
        description="Extract the most recent .tar.gz and move only git-tracked files"
    )
    parser.add_argument(
        "--pattern",
        type=str,
        default="*.tar.gz",
        help="Glob pattern to match files (default: *.tar.gz)"
    )
    parser.add_argument(
        "--dest",
        type=Path,
        default=None,
        help="Destination directory for extracted files (default: current directory)"
    )
    parser.add_argument(
        "--repo-root",
        type=Path,
        default=None,
        help="Root of the git repository (default: current directory)"
    )
    
    args = parser.parse_args()
    
    extract_and_move_git_tracked(args.pattern, args.dest, args.repo_root)


if __name__ == "__main__":
    main()
