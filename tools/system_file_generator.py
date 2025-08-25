#!/usr/bin/env python3

import argparse
import sys
from pathlib import Path
from typing import TypedDict, List

ALLOWED_FILES = {".h", ".hpp", ".hh"}


class SystemMeta(TypedDict):
    name: str
    namespace: str
    include: str
    class_name: str
    phase: str
    tags: List[str]
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


def build_includes(metas):
    include_str = "#include \"Generated.hpp\"\n"
    for meta in metas:
        if meta['isSystem'] is False:
            continue
        include_str += "#include \"" + meta['include'] + "\"\n"

    include_str += "\n\n"
    return include_str


def build_system_ctors(metas):
    system_ctors = ""

    for meta in metas:
        if meta['isSystem'] is False:
            continue
        ctor = "static std::unique_ptr<Engine::Ecs::ISystem> Create_" + meta['name'] + "(){\n"
        ctor += "\treturn std::make_unique<" + meta['namespace'] + "::" + meta['name'] + ">();\n}\n\n"
        system_ctors += ctor

    return system_ctors


def build_meta_list_entry(meta):
    if meta['isSystem'] is False:
        return ""
    meta_string = "\t\tEngine::Ecs::SystemMeta{\n"
    meta_string += "\t\t\t.name = \"" + meta['name'] + "\",\n"
    meta_string += "\t\t\t.phase = Engine::Ecs::Phase::" + meta['phase'] + ",\n"
    meta_string += "\t\t\t.tags = std::vector<std::string>{},\n"
    meta_string += "\t\t\t.factory = &Create_" + meta['name'] + "\n"
    meta_string += "\t\t},\n"
    return meta_string


def find_ecs_meta_in_file(file):
    sys_meta = SystemMeta(name="", namespace="", include="", class_name="", phase="", tags=[], isSystem=False)

    file_path = str(file)
    file_path = file_path.split("Maze Game")[-1]
    if file_path.startswith("/engine/core/"):
        file_path = file_path.replace("/engine/core/", "")
    elif file_path.startswith("/gameplay/"):
        file_path = file_path.replace("/gameplay/", "../gameplay/")

    sys_meta['include'] = file_path

    with open(file, encoding="utf-8") as f:
        content = f.read()
        lines = content.split("\n")
        for line in lines:
            line = line.strip()
            if line.startswith("ECS_SYSTEM"):
                l = line.split("(")
                l = l[1].split(")")
                meta = l[0].split(",")
                sys_meta['name'] = meta[0].strip()
                sys_meta['phase'] = meta[1].strip()
                sys_meta['tags'] = meta[2].strip().split(",")
                sys_meta['isSystem'] = True

            if line.startswith("namespace"):
                l = line.split(" ")
                sys_meta['namespace'] = l[1]

    return sys_meta


def build_cpp_string(meta_containers):
    cpp_string = """// ECS_Systems.gen.cpp\n// Auto-generated file.\n\n"""
    cpp_string += build_includes(meta_containers)
    cpp_string += build_system_ctors(meta_containers)
    cpp_string += "std::vector<Engine::Ecs::SystemMeta> MazeGame::GetSystemsFromGeneratedSource(){\n\tstd::vector<Engine::Ecs::SystemMeta> systems{\n"
    for meta_container in meta_containers:
        cpp_string += build_meta_list_entry(meta_container)
    cpp_string += """\n\t};\n\treturn systems;\n}"""
    return cpp_string


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--out", required=True, help="Path to generated cpp file")
    parser.add_argument("--roots", nargs="+", help="Files to analyze for system meta information")
    args = parser.parse_args()

    out_path = Path(args.out)
    out_path.parent.mkdir(parents=True, exist_ok=True)

    meta_containers = []
    files = list(iter_files(args.roots))
    print(f"[CodeGen] roots: {len(args.roots)}")
    for r in args.roots: print("\t-", r)
    print(f"[CodeGen] files: {len(files)}")
    for f in files:
        print("\t*", f)
        meta = find_ecs_meta_in_file(f)
        meta_containers.append(meta)

    cpp_string = build_cpp_string(meta_containers)

    out_path.write_text(cpp_string, encoding="utf-8")
    print(f"[CodeGen] Wrote to {out_path}")


if __name__ == "__main__":
    main()
