clang_tidy_flags = [
    "-Ivendor/glad/include",
    "-Ivendor/glfw/include",
    "-Ivendor/glm/include",
    "-Ivendor/stb/include",
    "-Iinclude",
    "-std=c++23"
]

from utils.file_search import file_search
import subprocess

files = file_search()

for file in files:
    print(f"Running clang-tidy on {file}")
    subprocess.run(["clang-tidy", "-fix-errors", str(file), "--", *clang_tidy_flags])
