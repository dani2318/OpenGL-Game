from pathlib import Path

paths = ["src", "include"]

extensions = {".cpp",".h",".hpp"}


def file_search() :
    files = []
    for base in paths:
        for path in Path(base).rglob("*"):
            if path.suffix.lower() in extensions:
                files.append(path)

    return files
