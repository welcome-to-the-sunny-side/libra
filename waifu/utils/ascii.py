import os
from typing import Dict, List, Optional


def load_ascii_template(templates_dir: str, metadata: Dict, name: Optional[str] = None) -> List[str]:
    """
    Load ASCII template lines by name using the provided metadata.

    Falls back to the default template from metadata, and finally to a simple placeholder
    if the file cannot be found.
    """
    template_name = name or metadata.get("default_template")
    if not template_name:
        # Ultimate fallback name
        template_name = "default.txt"

    path = os.path.join(templates_dir, template_name)
    try:
        with open(path, "r") as f:
            # Keep exact characters, strip only trailing newlines
            lines = [line.rstrip("\n") for line in f.readlines()]
            # Avoid empty list which breaks modulo usage later
            if not lines:
                # derive dimensions if present
                dims = metadata.get("ascii_templates", {}).get(template_name, {})
                height = int(dims.get("height", 20))
                lines = [""] * height
            return lines
    except FileNotFoundError:
        # Fallback: synthesize a simple block so the pipeline still works
        dims = metadata.get("ascii_templates", {}).get(template_name, {})
        width = int(dims.get("width", 40))
        height = int(dims.get("height", 20))
        return ["#" * width for _ in range(height)]
