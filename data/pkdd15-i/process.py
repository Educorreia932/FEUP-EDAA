import sys
import csv
import json
import time

input_filename = sys.argv[1]
f1 = open(input_filename, "r")
r1 = csv.reader(f1)

cols = next(r1)

TRIP_ID_IDX      = cols.index("TRIP_ID")
TIMESTAMP_IDX    = cols.index("TIMESTAMP")
MISSING_DATA_IDX = cols.index("MISSING_DATA")
POLYLINE_IDX     = cols.index("POLYLINE")

total, missing = 0, 0

start = time.time()

lines = []

for row in r1:
    total += 1
    missing_data = row[MISSING_DATA_IDX]
    if missing_data != "False":
        missing += 1
    if total%100000 == 0:
        print(f"i={total}", file=sys.stderr)

print(total-missing)
print("Number of runs: ", total, file=sys.stderr)
print("Number of runs with missing: ", missing, file=sys.stderr)

f2 = open(input_filename, "r")
r2 = csv.reader(f2)

cols = next(r2)

total_points = 0

i = 0
for row in r2:
    i += 1
    id           = row[TRIP_ID_IDX]
    timestamp    = row[TIMESTAMP_IDX]
    missing_data = row[MISSING_DATA_IDX]
    polyline     = json.loads(row[POLYLINE_IDX])
    if missing_data != "False":
        continue
    print(f"{id} {timestamp} {len(polyline)}")
    total_points += len(polyline)
    print("\n".join(map(lambda c: f"{c[1]} {c[0]}", polyline)))
    if i%20000 == 0:
        print(f"i={i} ({100.0 * i/(total-missing):.2f}%)", file=sys.stderr)

end = time.time()
print(f"Total number of coordinates: {total_points}", file=sys.stderr)
print(f"Took {end-start:.3f}s", file=sys.stderr)
