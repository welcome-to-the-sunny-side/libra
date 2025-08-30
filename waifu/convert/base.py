from abc import ABC, abstractmethod
from typing import List


class Converter(ABC):
    """Abstract base class for waifu converters."""

    @abstractmethod
    def convert(self, code_lines: List[str], art_lines: List[str]) -> List[str]:
        """
        Transform the given code lines using the provided ASCII art lines.

        Args:
            code_lines: The source code, one line per element (without trailing newlines).
            art_lines: The ASCII art, one line per element (without trailing newlines).
        Returns:
            List of transformed code lines (without trailing newlines).
        """
        raise NotImplementedError
