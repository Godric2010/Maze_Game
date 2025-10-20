#!/usr/bin/env bash
# Aggregates JUnit XML across one or more directories.
# Usage:
#   scripts/parse_junit.sh DIR [DIR2 ...]
#   scripts/parse_junit.sh -l path/to/dir_list.txt
#
# Exits: 0 (all green), 1 (any failures/errors), 2 (no XML found)

set -euo pipefail

# --- helpers -----------------------------------------------------------------
die() { echo "ERROR: $*" >&2; exit 2; }

is_dir_nonempty_glob() {
  # $1 = DIR, $2 = pattern (e.g., *.xml)
  shopt -s nullglob
  local arr=( "$1"/$2 )
  shopt -u nullglob
  [[ ${#arr[@]} -gt 0 ]]
}

sum_attr() {
  # $1 = DIR, $2 = attribute (tests|failures|errors|skipped)
  # Return: sum across all XMLs
  local dir="$1" attr="$2"
  local txt
  txt="$(grep -ho "${attr}=\"[0-9]\+\"" "$dir"/*.xml 2>/dev/null || true)"
  # Extract numbers and sum them
  printf '%s\n' "$txt" | sed 's/[^0-9]/ /g' | awk '{s+=$1} END{print s+0}'
}

# --- args --------------------------------------------------------------------
if [[ $# -lt 1 ]]; then
  cat >&2 <<USAGE
Usage:
  $0 DIR [DIR2 ...]
  $0 -l dir_list.txt   # file with one directory per line (comments with #)
USAGE
  exit 2
fi

declare -a DIRS=()
if [[ "${1:-}" == "-l" ]]; then
  [[ $# -ge 2 ]] || die "missing list file after -l"
  LIST_FILE="$2"
  [[ -f "$LIST_FILE" ]] || die "list file not found: $LIST_FILE"
  while IFS= read -r line; do
    line="${line%%#*}"             # strip comments
    line="$(echo "$line" | xargs)"  # trim
    [[ -n "$line" ]] || continue
    DIRS+=( "$line" )
  done < "$LIST_FILE"
else
  DIRS=( "$@" )
fi

# normalize and filter dirs that exist
TMP=()
for d in "${DIRS[@]}"; do
  [[ -d "$d" ]] && TMP+=( "$d" )
done
DIRS=( "${TMP[@]}" )

if [[ ${#DIRS[@]} -eq 0 ]]; then
  die "no valid JUnit directories provided"
fi

# --- aggregate ----------------------------------------------------------------
overall_tests=0
overall_failures=0
overall_errors=0
overall_skipped=0
found_any_xml=0

echo "=== JUnit aggregation ==="
for dir in "${DIRS[@]}"; do
  if ! is_dir_nonempty_glob "$dir" "*.xml"; then
    echo "• $dir : ❓  (no XML files)"
    continue
  fi

  found_any_xml=1

  t=$(sum_attr "$dir" tests)
  f=$(sum_attr "$dir" failures)
  e=$(sum_attr "$dir" errors)
  s=$(sum_attr "$dir" skipped)

  overall_tests=$((overall_tests + t))
  overall_failures=$((overall_failures + f))
  overall_errors=$((overall_errors + e))
  overall_skipped=$((overall_skipped + s))

  if [[ ${f:-0} -eq 0 && ${e:-0} -eq 0 ]]; then
    echo "• $dir : ✅  (tests=$t, failures=$f, errors=$e, skipped=$s)"
  else
    echo "• $dir : ❌  (tests=$t, failures=$f, errors=$e, skipped=$s)"
  fi
done

if [[ $found_any_xml -eq 0 ]]; then
  echo "No JUnit XML found in provided directories." >&2
  # still write GitHub outputs if requested (with safe defaults)
  if [[ -n "${GITHUB_OUTPUT:-}" ]]; then
    {
      echo "overall_status<<EOF";   echo "❓"; echo "EOF"
      echo "total_tests<<EOF";      echo "0";  echo "EOF"
      echo "total_failures<<EOF";   echo "0";  echo "EOF"
      echo "total_errors<<EOF";     echo "0";  echo "EOF"
      echo "total_skipped<<EOF";    echo "0";  echo "EOF"
    } >> "$GITHUB_OUTPUT"
  fi
  exit 2
fi

# overall status
overall_status="❌"
if [[ $overall_failures -eq 0 && $overall_errors -eq 0 ]]; then
  overall_status="✅"
fi

echo "----------------------------------------------------------------"
echo "Overall: $overall_status  (tests=$overall_tests, failures=$overall_failures, errors=$overall_errors, skipped=$overall_skipped)"

# --- GitHub Outputs (optional) -----------------------------------------------
if [[ -n "${GITHUB_OUTPUT:-}" ]]; then
  {
    echo "overall_status<<EOF";   echo "$overall_status";      echo "EOF"
    echo "total_tests<<EOF";      echo "$overall_tests";       echo "EOF"
    echo "total_failures<<EOF";   echo "$overall_failures";    echo "EOF"
    echo "total_errors<<EOF";     echo "$overall_errors";      echo "EOF"
    echo "total_skipped<<EOF";    echo "$overall_skipped";     echo "EOF"
  } >> "$GITHUB_OUTPUT"
fi

# exit code for CI
if [[ "$overall_status" == "✅" ]]; then
  exit 0
else
  exit 1
fi