#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="${1:-build}"
shift || true

# Find tools (prefer homebrew-llvm)
if ! command -v llvm-profdata >/dev/null 2>&1 || ! command -v llvm-cov >/dev/null 2>&1; then
  if [ -d "/opt/homebrew/opt/llvm/bin" ]; then
    export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
  fi
fi

command -v llvm-profdata >/dev/null 2>&1 || { echo "llvm-profdata not found"; exit 1; }
command -v llvm-cov >/dev/null 2>&1 || { echo "llvm-cov not found"; exit 1; }

# check if coverage is active
echo "ENABLE_COVERAGE=${ENABLE_COVERAGE:-unset}"

cd "$BUILD_DIR"
ABS_RAW_DIR="$(pwd)/coverage/raw"
ABS_HTML_DIR="$(pwd)/coverage/html"
mkdir -p "$ABS_RAW_DIR" "$ABS_HTML_DIR"

export LLVM_PROFILE_FILE="${ABS_RAW_DIR}/%p-%m.profraw"
echo "LLVM_PROFILE_FILE=${LLVM_PROFILE_FILE}"

# execute tests -> create .profraw files
echo "CTest will run tests; raw profiles to: ${LLVM_PROFILE_FILE}"

if command -v sysctl >/dev/null 2>&1; then
  J=$(sysctl -n hw.ncpu 2>/dev/null || echo 2)
else
  J=2
fi

echo "Running ctest -j ${J}..."
ctest --output-on-failure -j "${J}"

for f in $(find . -name "default.profraw" 2>/dev/null); do
  bn="$(basename "$(dirname "$f")")"
  mv "$f" "$ABS_RAW_DIR/${bn}-default.profraw" || true
done

# check if raw data is available
set +e
PROF_COUNT=$(find "$ABS_RAW_DIR" -name "*.profraw" | wc -l | tr -d ' ')
set -e
if [ "${PROF_COUNT}" = "0" ]; then
  echo "ERROR: No .profraw-files have been created..."
  exit 2
fi

# merging profiles
echo "Merging profiles"
llvm-profdata merge -sparse coverage/raw/*.profraw -o coverage/coverage.profdata

# define test executables
EXECUTABLES=("$@")
if [ ${#EXECUTABLES[@]} -eq 0 ]; then
  # fallback: find typical test binary names
  while IFS= read -r exe; do
    echo "Exe: $exe"
    EXECUTABLES+=("$exe")
  done < <(find . -type f -perm -111 \( -iname "*tests" -o -iname "test*" \) 2>/dev/null)
fi

if [ ${#EXECUTABLES[@]} -eq 0 ]; then
  echo "WARN: No test executables found. Reports only will be generated if the binaries ran..."
fi

# generating short text overview
echo "Generating text summary..."
{
  for exe in "${EXECUTABLES[@]}"; do
    if [ -f "$exe" ]; then
      echo "=== report for $exe ==="
      llvm-cov report "$exe" \
      -instr-profile=coverage/coverage.profdata \
      -ignore-filename-regex '(/tests?/|/test/|_tests?\.cpp$|/vcpkg/|/external/|/Catch2)' || true
      echo
    fi
  done
} | tee coverage/summary.txt

# generate html report
rm -rf "$ABS_HTML_DIR" && mkdir -p "$ABS_HTML_DIR"

echo "EXECUTABLES for HTML:"
printf ' %s\n' "${EXECUTABLES[@]:-<none>}"

set -euo pipefail

CMD=(llvm-cov show \
  -instr-profile=coverage/coverage.profdata \
  -format=html \
  -output-dir="$ABS_HTML_DIR" \
  -show-line-counts-or-regions \
  -show-branches=percent
  -compilation-dir="$(pwd)"
)
for exe in "${EXECUTABLES[@]}"; do
  [ -f "$exe" ] || { echo "WARN: missing exe: $exe"; continue; }
  CMD+=(-object "$exe")
done
CMD+=(-ignore-filename-regex '(/tests?/|/test/|_tests?\.cpp$|/vcpkg/|/external/|/Catch2)')

echo "${CMD[@]}"
"${CMD[@]}"

echo "HTML: $ABS_HTML_DIR/index.html"

echo "Done."
echo "Text:   $BUILD_DIR/coverage/summary.txt"
echo "HTML:   $BUILD_DIR/coverage/html/index.html"

