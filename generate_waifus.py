import os
import json
import shutil

class WaifuGenerator:
    def __init__(self):
        self.project_root = os.path.abspath(os.getcwd())
        self.src_dir = os.path.join(self.project_root, "src")
        self.waifu_src_dir = os.path.join(self.project_root, "waifu_src")
        self.waifu_templates_dir = os.path.join(self.project_root, "waifu_templates")
        self.vscode_folder = os.path.join(self.project_root, ".vscode")
        self.snippets_file = os.path.join(self.vscode_folder, "snippets.code-snippets")
        
        self.load_metadata()
    
    def load_metadata(self):
        """Load waifufication metadata."""
        metadata_path = os.path.join(self.waifu_templates_dir, "metadata.json")
        try:
            with open(metadata_path, 'r') as f:
                self.metadata = json.load(f)
        except FileNotFoundError:
            print(f"Warning: metadata.json not found at {metadata_path}")
            self.metadata = {"default_template": "default.txt", "templates": {}, "ascii_templates": {}}
    
    def create_snippet(self, file_path, prefix_type="template"):
        """Create a VS Code snippet for a given cpp file."""
        # Get relative path from src directory for better naming
        rel_path = os.path.relpath(file_path, self.src_dir if prefix_type == "template" else self.waifu_src_dir)
        filename = os.path.basename(file_path).replace('.cpp', '')
        
        # Special case for "Template.cpp" 
        if filename == "template":
            tab_trigger = "sport" if prefix_type == "template" else "sport_waifu"
        elif filename == "hacker_cup":
            tab_trigger = "sport_hacker" if prefix_type == "template" else "sport_hacker_waifu"
        else:
            tab_trigger = f"{prefix_type}_{filename}" if prefix_type == "template" else f"template_waifu_{filename}"
        
        try:
            with open(file_path, 'r') as f:
                content = f.readlines()
        except FileNotFoundError:
            print(f"Warning: Could not read {file_path}")
            return {}

        # Convert content to a snippet-compatible format
        snippet_content = [line.rstrip('\n') for line in content]
        
        # Escape special characters like \ and $
        snippet_content = [line.replace('\\', '\\\\').replace('$', '\\$') for line in snippet_content]
        
        # Replace {{CURSOR}} with $0 for cursor position
        snippet_content = [line.replace('{{CURSOR}}', '$0') for line in snippet_content]

        return {
            tab_trigger: {
                "prefix": tab_trigger,
                "body": snippet_content,
                "description": f"Snippet for {filename} ({'waifu' if 'waifu' in prefix_type else 'normal'} version)"
            }
        }
    
    def waifufy_template(self, source_file, target_file):
        """
        For now, simply copy the template exactly as-is.
        Later this will implement the actual ASCII art formatting.
        """
        # Ensure target directory exists
        target_dir = os.path.dirname(target_file)
        os.makedirs(target_dir, exist_ok=True)
        
        # For now, just copy the file
        shutil.copy2(source_file, target_file)
        
        print(f"Waifufied: {source_file} -> {target_file}")
    
    def generate_normal_snippets(self):
        """Generate snippets from src/ directory."""
        snippets = {}
        
        for root, _, files in os.walk(self.src_dir):
            for file in files:
                if file.endswith('.cpp'):
                    file_path = os.path.join(root, file)
                    snippet = self.create_snippet(file_path, "template")
                    snippets.update(snippet)
        
        return snippets
    
    def generate_waifu_templates(self):
        """Generate waifufied versions of all templates."""
        for root, _, files in os.walk(self.src_dir):
            for file in files:
                if file.endswith('.cpp'):
                    source_file = os.path.join(root, file)
                    # Maintain directory structure in waifu_src
                    rel_path = os.path.relpath(source_file, self.src_dir)
                    target_file = os.path.join(self.waifu_src_dir, rel_path)
                    
                    self.waifufy_template(source_file, target_file)
    
    def generate_waifu_snippets(self):
        """Generate snippets from waifu_src/ directory."""
        snippets = {}
        
        for root, _, files in os.walk(self.waifu_src_dir):
            for file in files:
                if file.endswith('.cpp'):
                    file_path = os.path.join(root, file)
                    snippet = self.create_snippet(file_path, "waifu")
                    snippets.update(snippet)
        
        return snippets
    
    def generate_all(self):
        """Main generation pipeline."""
        # Ensure .vscode folder exists
        os.makedirs(self.vscode_folder, exist_ok=True)
        
        print("Generating normal snippets from src/...")
        normal_snippets = self.generate_normal_snippets()
        
        print("Generating waifufied templates...")
        self.generate_waifu_templates()
        
        print("Generating waifu snippets from waifu_src/...")
        waifu_snippets = self.generate_waifu_snippets()
        
        # Combine all snippets
        all_snippets = {**normal_snippets, **waifu_snippets}
        
        # Save to VS Code snippets file
        with open(self.snippets_file, 'w') as f:
            json.dump(all_snippets, f, indent=4)
        
        print(f"\nGenerated {len(normal_snippets)} normal snippets")
        print(f"Generated {len(waifu_snippets)} waifu snippets")
        print(f"Snippets successfully saved to {self.snippets_file}")

def main():
    generator = WaifuGenerator()
    generator.generate_all()

if __name__ == "__main__":
    main()
