import os
import json

def create_snippet(file_path):
    """Create a VS Code snippet for a given cpp file."""
    filename = os.path.basename(file_path).replace('.cpp', '')
    
    # Special case for "Template.cpp"
    if filename == "template":
        tab_trigger = "sport"
    elif filename == "hacker_cup":
        tab_trigger = "sport_hacker"
    else:
        tab_trigger = f"template_{filename}"
    
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
    # Determine the project folder (two levels up from the current script location)
    project_root = os.path.abspath(os.path.join(os.getcwd(), "../"))
    
    # Define the .vscode folder path in the project root
    vscode_folder = os.path.join(project_root, ".vscode")
    snippets_file_path = os.path.join(vscode_folder, "snippets.code-snippets")
    
    # Ensure the .vscode folder exists
    if not os.path.exists(vscode_folder):
        os.makedirs(vscode_folder)

    snippets = {}
    
    # Get the current directory (F/libra) and process .cpp files
    current_directory = os.getcwd()

    # Traverse through the directory and find all .cpp files
    for root, _, files in os.walk(current_directory):
        for file in files:
            if file.endswith('.cpp'):
                file_path = os.path.join(root, file)
                snippet = create_snippet(file_path)
                snippets.update(snippet)

    # Save all the snippets to the JSON file inside F/.vscode
    with open(snippets_file_path, 'w') as f:
        json.dump(snippets, f, indent=4)

    print(f"Snippets successfully saved to {snippets_file_path}")

if __name__ == "__main__":
    main()  