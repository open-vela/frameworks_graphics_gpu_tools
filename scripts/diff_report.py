"""
Copyright (C) 2025 Xiaomi Corporation

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import argparse
import csv
from collections import defaultdict
from tabulate import tabulate


def parse_csv(file_path):
    """Parse CSV file and return a dictionary of {test case: time data}."""
    with open(file_path, "r") as f:
        reader = csv.reader(f)

        # Locate the header row
        headers = next(row for row in reader if row and row[0] == "Testcase")

        # Get the indices of key fields
        indices = {
            field: headers.index(field)
            for field in ["Setup Time(ms)", "Draw Time(ms)", "Finish Time(ms)"]
        }

        # Parse data rows
        return {
            row[0]: {
                "setup": safe_float(row[indices["Setup Time(ms)"]]),
                "draw": safe_float(row[indices["Draw Time(ms)"]]),
                "finish": safe_float(row[indices["Finish Time(ms)"]]),
            }
            for row in reader
            if row and not row[0].startswith("Test result")
        }


def safe_float(value):
    """Safely convert a value to a float."""
    try:
        return float(value.strip()) if value.strip() else None
    except:
        return None


def compare_data(base, comp, pct_th, abs_th):
    """Compare performance data and generate an anomaly report."""
    anomalies = defaultdict(list)

    for case in base:
        if case not in comp:
            continue

        base_data = base[case]
        comp_data = comp[case]

        for field in ["setup", "draw", "finish"]:
            b = base_data[field]
            c = comp_data[field]

            if None in (b, c):
                continue

            abs_diff = c - b
            pct_diff = (abs_diff / b * 100) if b else float("inf")

            if abs(pct_diff) > pct_th and abs(abs_diff) > abs_th:
                anomalies[case].append(
                    {
                        "field": field.upper(),
                        "pct": pct_diff,
                        "abs": abs_diff,
                        "base_val": b,
                        "comp_val": c,
                    }
                )

    return anomalies


def print_report(anomalies, pct_th, abs_th):
    """Format and output the comparison report."""
    if not anomalies:
        print(
            f"\n\033[32mAll test cases normal (Threshold >{pct_th}% and ≥{abs_th}ms)\033[0m"
        )
        return

    # Prepare table data
    table_data = []
    for case, details in anomalies.items():
        for d in details:
            table_data.append(
                [
                    case,
                    d["field"],
                    f"{d['pct']:.2f}%",
                    f"{d['abs']:.3f}ms",
                    f"{d['base_val']:.3f}ms/{d['comp_val']:.3f}ms",
                ]
            )

    # Print table
    headers = [
        "Test Case",
        "Anomaly Metric",
        "Percentage Diff",
        "Absolute Diff",
        "Base/Comp Value",
    ]
    print(f"\n\033[31mAnomalies found (Threshold >{pct_th}% and >{abs_th}ms):\033[0m")
    print(tabulate(table_data, headers=headers, tablefmt="grid"))


def main():
    parser = argparse.ArgumentParser(
        description="Performance Comparison Tool",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    parser.add_argument("-b", "--base", required=True, help="Base CSV file")
    parser.add_argument("-c", "--compare", required=True, help="Comparison CSV file")
    parser.add_argument(
        "-p",
        "--pct-threshold",
        type=float,
        default=20.0,
        help="Percentage difference threshold",
    )
    parser.add_argument(
        "-a",
        "--abs-threshold",
        type=float,
        default=0.05,
        help="Absolute difference threshold(ms)",
    )

    args = parser.parse_args()

    try:
        # Print comparison configuration
        print("\n\033[36mComparison Configuration:\033[0m")
        print(f"| Base file: {args.base}")
        print(f"| Comparison file: {args.compare}")
        print(f"| Percentage threshold: {args.pct_threshold:.2f}%")
        print(f"| Absolute threshold: {args.abs_threshold:.3f}ms")

        base = parse_csv(args.base)
        comp = parse_csv(args.compare)
        anomalies = compare_data(base, comp, args.pct_threshold, args.abs_threshold)
        print_report(anomalies, args.pct_threshold, args.abs_threshold)

    except Exception as e:
        print(f"\033[31mError: {str(e)}\033[0m")


if __name__ == "__main__":
    main()
