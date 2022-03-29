import sys
import csv
import json
import time
import math

INF = 180.0

input_filename = sys.argv[1]
f1 = open(input_filename, "r")
r1 = csv.reader(f1)

cols = next(r1)

TRIP_ID_IDX      = cols.index("TRIP_ID")
TIMESTAMP_IDX    = cols.index("TIMESTAMP")
MISSING_DATA_IDX = cols.index("MISSING_DATA")
POLYLINE_IDX     = cols.index("POLYLINE")

total, missing, speedErrors, coordErrors, totalPoints = 0, 0, 0, 0, 0

start = time.time()

notGoodSet = set()

# MINLON, MINLAT, MAXLON, MAXLAT = -9.5,38.5,-6.5,43.0
MINLON, MINLAT, MAXLON, MAXLAT = -8.7863,40.9980,-8.4718,41.3722

for row in r1:
    total += 1

    if total%100000 == 0:
        print(f"i={total}", file=sys.stderr)

    id           = row[TRIP_ID_IDX]
    missing_data = row[MISSING_DATA_IDX]
    polyline     = json.loads(row[POLYLINE_IDX])

    if missing_data != "False":
        missing += 1
        notGoodSet.add(id)
        continue

    prevLat, prevLon = 0, 0
    isGood = True

    for c in polyline:
        lat, lon = c[1], c[0]

        if prevLat != 0 and prevLon != 0:
            dx = prevLon-lon
            dy = prevLat-lat
            d = math.sqrt(dx*dx + dy*dy)
            if d > 0.0005*15*4: # 0.0005 deg/s = 200kph, 15s, 4 is a margin factor
                # print(f"DANGER! {id}: {prevLat}, {prevLon} and {lat},{lon} are too far appart to be correct", file=sys.stderr)
                speedErrors += 1
                isGood = False
                break

        if lat < MINLAT or lat > MAXLAT or lon < MINLON or lon > MAXLON:
            # print(f"DANGER! {id}: {lat},{lon} outside ordinary bounds", file=sys.stderr)
            coordErrors += 1
            isGood = False
            break

        prevLat, prevLon = lat, lon

    if not isGood:
        notGoodSet.add(id)
        continue

    totalPoints += len(polyline)

f2 = open(input_filename, "r")
r2 = csv.reader(f2)
cols = next(r2)
totalGood = 0
for row in r2:
    id = row[TRIP_ID_IDX]
    if(id in notGoodSet):
        continue
    totalGood += 1

print(totalGood)
print("Number of runs: ", totalGood, file=sys.stderr)
print("Number of runs with missing/speedErrors/coordErrors: ", missing, speedErrors, coordErrors, file=sys.stderr)
print(f"Total number of coordinates: {totalPoints}", file=sys.stderr)

f3 = open(input_filename, "r")
r3 = csv.reader(f3)

cols = next(r3)

minLat, maxLat, minLon, maxLon = +INF, -INF, +INF, -INF

i = 0
for row in r3:
    id           = row[TRIP_ID_IDX]
    timestamp    = row[TIMESTAMP_IDX]
    polyline     = json.loads(row[POLYLINE_IDX])
    if(id in notGoodSet):
        continue

    i += 1

    for c in polyline:
        lat, lon = c[1], c[0]
        minLat = min(minLat, lat)
        maxLat = max(maxLat, lat)
        minLon = min(minLon, lon)
        maxLon = max(maxLon, lon)

    print(f"{id} {timestamp} {len(polyline)}")
    print("\n".join(map(lambda c: f"{c[1]} {c[0]}", polyline)))
    if i%20000 == 0:
        print(f"i={i} ({100.0 * i/total:.2f}%)", file=sys.stderr)


print(f"totalGood: {totalGood}; i: {i}", file=sys.stderr)

end = time.time()
print(f"Lat: [{minLat}, {maxLat}]; Lon: [{minLon}, {maxLon}]", file=sys.stderr)
print(f"Took {end-start:.3f}s", file=sys.stderr)
