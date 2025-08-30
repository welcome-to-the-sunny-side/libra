from typing import List

from .base import Converter


class OverlayConverter(Converter):
    """
    Overlay ASCII art to the right of source code using line comments.

    This converter keeps code intact and appends a line comment with a slice
    of the ASCII art per line. Lines that end with a trailing backslash (e.g.,
    multi-line macros) are left unchanged to preserve semantics.
    """

    def __init__(self, comment_prefix: str = "//", code_column: int = 72, padding: int = 1):
        self.comment_prefix = comment_prefix
        self.code_column = max(0, code_column)
        self.padding = max(0, padding)

    def convert(self, code_lines: List[str], art_lines: List[str]) -> List[str]:
        if not code_lines:
            return []
        if not art_lines:
            return code_lines[:]

        # Precompute art width and height
        art_h = len(art_lines)
        art_w = max((len(s) for s in art_lines), default=0)

        def art_for_index(i: int) -> str:
            # Scale art vertically to match code length
            if len(code_lines) == 1:
                idx = 0
            else:
                # round to nearest index in [0, art_h-1]
                idx = round(i * (art_h - 1) / (len(code_lines) - 1))
            line = art_lines[idx] if 0 <= idx < art_h else ""
            # Right-pad ASCII line to stable width
            return line + (" " * max(0, art_w - len(line)))

        out: List[str] = []
        for i, raw in enumerate(code_lines):
            line = raw.rstrip("\n\r")
            # Safety: don't append to lines that end with a backslash (macro continuations)
            if line.rstrip().endswith("\\"):
                out.append(line)
                continue

            # Build overlay
            overlay = art_for_index(i)

            # If the line is shorter than code_column, pad with spaces so comments align
            if len(line) < self.code_column:
                padded = line + (" " * (self.code_column - len(line)))
            else:
                padded = line

            spacer = (" " * self.padding) if padded else ""
            converted = f"{padded}{spacer}{self.comment_prefix} {overlay}" if overlay else padded
            out.append(converted.rstrip())

        return out
