import os
import json

def create_snippet(file_path):
    """Create a VS Code snippet for a given cpp file"""
    filename = os.path.basename(file_path).replace('.cpp', '')
    
    # Special case for "Template.cpp"
    if filename == "Template":
        tab_trigger = "sport"
    elif filename == "HackerCup":
        tab_trigger = "sport_hacker"
    else:
        tab_trigger = f"Template{filename}"
    
    with open(file_path, 'r') as f:
        content = f.readlines()

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
            "description": f"Snippet for {filename}"
        }
    }

def main():
    # Use the current user's home directory dynamically
    home_dir = os.path.expanduser("~")
    
    # Update directory paths using the current user's home directory
    directory = os.path.join(home_dir, "Study/Algorithm/sport/libra/")
    snippets_file_path = os.path.join(home_dir, ".config/Code/User/snippets/cpp_snippets.code-snippets")
    
    snippets = {}

    # Traverse through the directory and find all .cpp files
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith('.cpp'):
                file_path = os.path.join(root, file)
                snippet = create_snippet(file_path)
                snippets.update(snippet)

    # Save all the snippets to a JSON file
    with open(snippets_file_path, 'w') as f:
        json.dump(snippets, f, indent=4)

if __name__ == "__main__":
    main()
