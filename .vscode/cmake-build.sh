#!/usr/bin/env bash

set -euo pipefail

readonly script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
readonly workspace_dir="$(cd -- "${script_dir}/.." && pwd)"
readonly build_dir="${workspace_dir}/build"
readonly compile_commands="${build_dir}/compile_commands.json"
readonly compile_commands_link="${workspace_dir}/compile_commands.json"

mkdir -p -- "${build_dir}"

# Serialise rapid consecutive saves so two CMake processes never share the
# same build tree at the same time.
exec 9>"${build_dir}/.vscode-build.lock"
flock 9

cmake \
  -S "${workspace_dir}" \
  -B "${build_dir}" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if [[ ! -f "${compile_commands}" ]]; then
  echo "CMake did not generate ${compile_commands}" >&2
  exit 1
fi

if [[ -L "${compile_commands_link}" ]]; then
  rm -- "${compile_commands_link}"
elif [[ -e "${compile_commands_link}" ]]; then
  echo "Refusing to replace non-symlink ${compile_commands_link}" >&2
  exit 1
fi

ln -s "build/compile_commands.json" "${compile_commands_link}"
cmake --build "${build_dir}" --parallel
