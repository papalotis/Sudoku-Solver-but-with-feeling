import urllib.request
import json
import sys


def getOneSudoku(diff="hard"):
    json_string = urllib.request.urlopen(
        "https://sugoku.herokuapp.com/board?difficulty=" + diff).read()

    json_dict = json.loads(json_string)

    sud_string = ""

    for line in json_dict["board"]:
        sud_string += str(line)

    toRemove = "[], "

    for c in toRemove:
        sud_string = sud_string.replace(c, "")

    return sud_string


num_to_get = int(sys.argv[1]) if (len(sys.argv) > 1) else 10

with open("data/sudokus_from_api.txt", "a") as f:
    n = 10
    f.write("\n")
    for i in range(n):
        f.write(getOneSudoku())
        if i < n - 1:
            f.write("\n")
