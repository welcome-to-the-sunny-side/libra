import re
from typing import List, Tuple

from .base import Converter


_WORD_KINDS = {"ID", "NUM", "STRING", "CHAR"}


class InlineFitConverter(Converter):
    """
    Reshape the code itself to fit the target ASCII art geometry.

    Steps:
    1) Keep preprocessor lines (#...) intact at the top.
    2) Convert // end-of-line comments to /* ... */ where safe.
    3) Tokenize the remaining code and rebuild a compact stream, inserting
       /**/ between adjacent word-like tokens to preserve lexing.
    4) Wrap the stream across lines to match the art width. For each art line,
       place as many tokens as fit (without splitting a token), then fill the
       remainder with a trailing // + art slice to visually approximate the art.

    Notes:
    - Lines ending with backslash (macro continuations) are preserved as-is.
    - We avoid changing preprocessor lines to keep compilation semantics.
    - This is a heuristic formatter; it aims to be safe and non-splitting.
    """

    def __init__(self, default_width: int = 80):
        self.default_width = default_width

    # -------------------- Public API --------------------
    def convert(self, code_lines: List[str], art_lines: List[str]) -> List[str]:
        if not code_lines:
            return []

        art_width = max((len(s) for s in art_lines), default=self.default_width)
        art_height = len(art_lines)
        if art_width <= 0:
            art_width = self.default_width

        # 1) Pull out leading preprocessor lines (and macro continuations)
        head, tail = self._extract_header(code_lines)

        # 2) Normalize tail lines: convert // to /* */ safely
        tail_norm = [self._convert_eol_comment(line) for line in tail]

        # 3) Tokenize and rebuild compact code stream
        tokens = self._tokenize("\n".join(tail_norm))
        compact_stream = self._rebuild_stream(tokens)

        # 4) Shape into lines by art
        shaped = self._shape_by_art(compact_stream, art_lines, art_width)

        return head + shaped

    # -------------------- Header handling --------------------
    def _extract_header(self, lines: List[str]) -> Tuple[List[str], List[str]]:
        head: List[str] = []
        i = 0
        n = len(lines)
        # collect initial preprocessor block
        while i < n:
            s = lines[i]
            if s.rstrip().endswith("\\"):
                # Keep macro continuation blocks entirely in header to avoid wrapping
                head.append(s)
                i += 1
                continue
            stripped = s.lstrip()
            if stripped.startswith('#'):
                head.append(s)
                i += 1
            else:
                break
        tail = lines[i:]
        return head, tail

    # -------------------- Comment normalization --------------------
    def _convert_eol_comment(self, line: str) -> str:
        # Convert //... (not in quotes) to /*...*/
        i = 0
        in_s = False
        in_c = False
        esc = False
        while i < len(line):
            ch = line[i]
            if in_s:
                if esc:
                    esc = False
                elif ch == '\\':
                    esc = True
                elif ch == '"':
                    in_s = False
                i += 1
                continue
            if in_c:
                if esc:
                    esc = False
                elif ch == '\\':
                    esc = True
                elif ch == "'":
                    in_c = False
                i += 1
                continue
            # not in string/char
            if ch == '"':
                in_s = True
                i += 1
                continue
            if ch == "'":
                in_c = True
                i += 1
                continue
            if ch == '/' and i + 1 < len(line) and line[i + 1] == '/':
                body = line[i + 2 :]
                body = body.replace('*/', '* /')  # avoid closing comment accidentally
                return line[:i] + '/*' + body + '*/'
            i += 1
        return line

    # -------------------- Tokenization --------------------
    def _tokenize(self, src: str) -> List[Tuple[str, str]]:
        # Order matters for multi-char operators
        tok_spec = [
            ("ML_COMMENT", r"/\*.*?\*/", re.S),
            ("SL_COMMENT", r"//[^\n]*"),
            ("STRING", r'"([^"\\]|\\.)*?"'),
            ("CHAR", r"'([^'\\]|\\.)*?'") ,
            ("ID", r"[A-Za-z_][A-Za-z0-9_]*"),
            ("NUM", r"0[xX][0-9A-Fa-f]+|\d+(?:\.\d+)?(?:[eE][+-]?\d+)?"),
            ("OP", r"::|->\*|->|\+\+|--|<<=|>>=|==|!=|<=|>=|&&|\|\||<<|>>|\+=|-=|\*=|/=|%=|&=|\|=|\^=|\.\.\.|\.|,|;|:|\?|\(|\)|\[|\]|\{|\}|\+|-|\*|/|%|&|\||\^|!|~|=|<|>"),
            ("WS", r"[ \t\r\n]+"),
        ]
        parts = []
        idx = 0
        flags = 0
        while idx < len(src):
            match = None
            for kind, pattern, *opt in tok_spec:
                fl = opt[0] if opt else 0
                rx = re.compile(pattern, flags=fl)
                match = rx.match(src, idx)
                if match:
                    text = match.group(0)
                    if kind == "SL_COMMENT":
                        # convert to ML to avoid newline issues
                        body = text[2:]
                        body = body.replace('*/', '* /')
                        parts.append(("ML_COMMENT", f"/*{body}*/"))
                    elif kind == "WS":
                        # collapse whitespace entirely
                        pass
                    else:
                        parts.append((kind, text))
                    idx = match.end()
                    break
            if not match:
                # Unknown character; treat as single-char operator to remain robust
                parts.append(("OP", src[idx]))
                idx += 1
        return parts

    # -------------------- Stream rebuild --------------------
    def _rebuild_stream(self, tokens: List[Tuple[str, str]]) -> List[str]:
        out: List[str] = []
        prev_kind = None
        for kind, text in tokens:
            if prev_kind in _WORD_KINDS and kind in _WORD_KINDS:
                out.append("/**/")
            out.append(text)
            prev_kind = kind
        return out

    # -------------------- Shaping --------------------
    def _shape_by_art(self, stream: List[str], art_lines: List[str], width: int) -> List[str]:
        # Flatten stream to tokens with exact lengths
        i = 0  # index over stream list
        shaped: List[str] = []
        art_h = max(1, len(art_lines))

        def art_line_at(row: int) -> str:
            if art_h == 1:
                return art_lines[0] if art_lines else ""
            # map rows across available art rows
            idx = round(row * (art_h - 1) / max(1, len(shaped) + 1)) if shaped else 0
            return art_lines[min(max(idx, 0), art_h - 1)]

        # We'll iterate until stream is exhausted; if art shorter, repeat last line
        row = 0
        while i < len(stream):
            line_buf: List[str] = []
            line_len = 0
            # Fill line with as many tokens as fit (reserve 4 chars for /* */ when possible)
            reserve = 4
            while i < len(stream):
                tok = stream[i]
                tlen = len(tok)
                if line_len + tlen <= width - reserve:
                    line_buf.append(tok)
                    line_len += tlen
                    i += 1
                else:
                    # If nothing placed yet and token is too large, place it anyway to avoid stalling
                    if line_len == 0:
                        line_buf.append(tok)
                        line_len += tlen
                        i += 1
                    break
            # Build filler from art remainder
            if line_len < width:
                art = art_lines[row] if row < len(art_lines) else (art_lines[-1] if art_lines else "")
                art_rem = art[line_len:width]
                # sanitize for block comment
                art_rem = art_rem.replace('\t', '    ').replace('*/', '* /')
                # ensure at least 4 for /* */
                need = width - line_len
                if need >= 4:
                    filler = '/*' + art_rem[: need - 4] + '*/'
                    line_buf.append(filler)
                    line_len = width
                else:
                    # Not enough space for a proper block comment; pad with spaces
                    line_buf.append(' ' * need)
                    line_len = width
            shaped.append(''.join(line_buf).rstrip())
            row += 1
        # If art has remaining rows but no code, we can append pure comment lines
        while row < len(art_lines):
            art = art_lines[row]
            if len(art) >= 4:
                safe = art.replace('*/', '* /')
                shaped.append('/*' + safe[: max(0, width - 4)] + '*/')
            else:
                shaped.append('')
            row += 1
        return shaped
