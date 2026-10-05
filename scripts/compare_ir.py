from __future__ import annotations

import re
import sys
from pathlib import Path

INSTRUCTION_RE = re.compile(r"^\s*%.*=\s+([a-zA-Z.]+)\b")


def count_instructions(path: Path) -> int:
    total = 0
    for line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
        if INSTRUCTION_RE.match(line):
            total += 1
    return total


def main() -> int:
    if len(sys.argv) != 3:
        print("usage: python scripts/compare_ir.py before.ll after.ll")
        return 1

    before = Path(sys.argv[1])
    after = Path(sys.argv[2])

    before_count = count_instructions(before)
    after_count = count_instructions(after)
    delta = after_count - before_count

    print(f"before: {before_count}")
    print(f"after:  {after_count}")
    print(f"delta:  {delta:+}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
