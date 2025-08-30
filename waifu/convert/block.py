from typing import List

from .base import Converter


class BlockConverter(Converter):
    """
    Replace each code line with a comment-only ASCII art line, creating
    a solid block regardless of the source code content.
    """

    def __init__(self, comment_prefix: str = "//"):
        self.comment_prefix = comment_prefix

    def convert(self, code_lines: List[str], art_lines: List[str]) -> List[str]:
        if not code_lines:
            return []
        if not art_lines:
            # no art, just preserve code as comments minimally
            return [self.comment_prefix for _ in code_lines]

        art_h = len(art_lines)

        def art_for_index(i: int) -> str:
            if len(code_lines) == 1:
                idx = 0
            else:
                idx = round(i * (art_h - 1) / (len(code_lines) - 1))
            line = art_lines[idx] if 0 <= idx < art_h else ""
            return line

        out: List[str] = []
        for i, _ in enumerate(code_lines):
            overlay = art_for_index(i)
            # if overlay empty, still emit a bare comment to keep shape
            out.append(f"{self.comment_prefix} {overlay}".rstrip())
        return out
