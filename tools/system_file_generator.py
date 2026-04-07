#!/usr/bin/env python3

import argparse
import sys
from pathlib import Path
from typing import TypedDict, List, Optional

ALLOWED_FILES = {".h", ".hpp", ".hh"}


class SystemMeta(TypedDict):
    name: str
    namespace: str
    include: str
    class_name: str
    phase: str
    tags: List[str]
    dependencies: List[str]
    isSystem: bool


def iter_files(roots):
    for root in roots:
        path = Path(root)
        if not path.exists():
            print(f"[CodeGen] WARN: root does not exist: {root}", file=sys.stderr)
            continue

        for file in path.rglob("*"):
            if file.suffix.lower() in ALLOWED_FILES and file.is_file():
                yield file


def extract_ecs_system_call(content: str) -> Optional[str]:
    marker = "ECS_SYSTEM("
    start = content.find(marker)
    if start == -1:
        return None

    i = start + len(marker)
    depth = 1

    while i < len(content):
        ch = content[i]

        if ch == '(':
            depth += 1
        elif ch == ')':
            depth -= 1
            if depth == 0:
                return content[start + len(marker):i].strip()

        i += 1

    return None


def split_top_level_args(arg_string: str) -> List[str]:
    parts = []
    current = []
    depth = 0

    for ch in arg_string:
        if ch == '(':
            depth += 1
            current.append(ch)
        elif ch == ')':
            depth -= 1
            current.append(ch)
        elif ch == ',' and depth == 0:
            parts.append("".join(current).strip())
            current = []
        else:
            current.append(ch)

    if current:
        parts.append("".join(current).strip())

    return parts


def extract_macro_list(value: str, macro_name: str) -> List[str]:
    value = value.strip()
    prefix = f"{macro_name}("

    if not value.startswith(prefix) or not value.endswith(")"):
        raise ValueError(f"[CodeGen] Expected {macro_name}(...) but got: {value}")

    inner = value[len(prefix):-1].strip()
    if not inner:
        return []

    return [item.strip() for item in inner.split(",") if item.strip()]


def build_includes(metas: List[SystemMeta]) -> str:
    include_str = "#include \"Generated.hpp\"\n"

    for meta in metas:
        if not meta['isSystem']:
            continue
        include_str += f"#include \"{meta['include']}\"\n"

    include_str += "\n\n"
    return include_str


def build_system_ctors(metas: List[SystemMeta]) -> str:
    system_ctors = ""

    for meta in metas:
        if not meta['isSystem']:
            continue

        ctor = f"static std::unique_ptr<Engine::Ecs::ISystem> Create_{meta['name']}(){{\n"
        ctor += f"\treturn std::make_unique<{meta['namespace']}::{meta['name']}>();\n"
        ctor += "}\n\n"
        system_ctors += ctor

    return system_ctors


def build_string_list(field_name: str, values: List[str]) -> str:
    out = f"\t\t\t.{field_name} = std::vector<std::string>{{"

    if values:
        out += ", ".join(f"\"{value}\"" for value in values)

    out += "},\n"
    return out


def build_meta_list_entry(meta: SystemMeta) -> str:
    if not meta['isSystem']:
        return ""

    meta_string = "\t\tEngine::Ecs::SystemMeta{\n"
    meta_string += f"\t\t\t.name = \"{meta['name']}\",\n"
    meta_string += f"\t\t\t.phase = Engine::Ecs::Phase::{meta['phase']},\n"
    meta_string += build_string_list("tags", meta['tags'])
    meta_string += build_string_list("dependencies", meta['dependencies'])
    meta_string += f"\t\t\t.factory = &Create_{meta['name']}\n"
    meta_string += "\t\t},\n"

    return meta_string


def normalize_include_path(file: Path) -> str:
    file_path = str(file)
    file_path = file_path.split("Maze Game")[-1]

    if file_path.startswith("/engine/systems/"):
        file_path = file_path.replace("/engine/systems/", "../engine/systems/")
    elif file_path.startswith("/gameplay/"):
        file_path = file_path.replace("/gameplay/", "../gameplay/")

    return file_path


def extract_namespace(content: str) -> str:
    for line in content.splitlines():
        stripped = line.strip()
        if stripped.startswith("namespace "):
            parts = stripped.split()
            if len(parts) >= 2:
                return parts[1]
    return ""


def find_ecs_meta_in_file(file: Path) -> SystemMeta:
    sys_meta: SystemMeta = {
        "name": "",
        "namespace": "",
        "include": normalize_include_path(file),
        "class_name": "",
        "phase": "",
        "tags": [],
        "dependencies": [],
        "isSystem": False,
    }

    with open(file, encoding="utf-8") as f:
        content = f.read()

    sys_meta['namespace'] = extract_namespace(content)

    ecs_call = extract_ecs_system_call(content)
    if ecs_call is None:
        return sys_meta

    args = split_top_level_args(ecs_call)
    if len(args) != 4:
        raise ValueError(
            f"[CodeGen] Invalid ECS_SYSTEM signature in {file}. "
            f"Expected 4 arguments (name, phase, tags, dependencies), got {len(args)}: {args}"
        )

    sys_meta['name'] = args[0]
    sys_meta['class_name'] = args[0]
    sys_meta['phase'] = args[1]
    sys_meta['tags'] = extract_macro_list(args[2], "TAGS")
    sys_meta['dependencies'] = extract_macro_list(args[3], "DEPENDENCIES")
    sys_meta['isSystem'] = True

    return sys_meta


def build_cpp_string(meta_containers: List[SystemMeta]) -> str:
    cpp_string = """// ECS_Systems.gen.cpp
// Auto-generated file.

"""
    cpp_string += build_includes(meta_containers)
    cpp_string += build_system_ctors(meta_containers)
    cpp_string += "std::vector<Engine::Ecs::SystemMeta> MazeGame::GetSystemsFromGeneratedSource(){\n"
    cpp_string += "\tstd::vector<Engine::Ecs::SystemMeta> systems{\n"

    for meta_container in meta_containers:
        cpp_string += build_meta_list_entry(meta_container)

    cpp_string += """\t};
\treturn systems;
}"""
    return cpp_string


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--out", required=True, help="Path to generated cpp file")
    parser.add_argument("--roots", nargs="+", required=True, help="Files to analyze for system meta information")
    args = parser.parse_args()

    out_path = Path(args.out)
    out_path.parent.mkdir(parents=True, exist_ok=True)

    meta_containers: List[SystemMeta] = []

    files = list(iter_files(args.roots))
    print(f"[CodeGen] roots: {len(args.roots)}")
    for r in args.roots:
        print("\t-", r)

    print(f"[CodeGen] files: {len(files)}")
    for f in files:
        print("\t*", f)
        try:
            meta = find_ecs_meta_in_file(f)
            meta_containers.append(meta)
        except Exception as ex:
            print(f"[CodeGen] ERROR while parsing {f}: {ex}", file=sys.stderr)
            raise

    cpp_string = build_cpp_string(meta_containers)
    out_path.write_text(cpp_string, encoding="utf-8")
    print(f"[CodeGen] Wrote to {out_path}")


if __name__ == "__main__":
    main()